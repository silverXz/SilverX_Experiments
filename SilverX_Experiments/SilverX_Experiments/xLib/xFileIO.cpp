// xFileIO.cpp
// ***************************************************************
//  xFileIO   version:  1.0   
//  -------------------------------------------------------------
//  Description:  implementation of the CxFileIO class.
//  Author     :  wujh@sia.cn 
//  Created    :  07/18/2006
//  Modified   : 
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "stdafx.h"
#include "xFileIO.h"




#include "xVertex3d.h"

#include "xMesh.h"

#include <vector>
#include <set>
#include <string>
#include <fstream>

using std::vector;
using std::set;
using std::string;

#include "xVector.h"


// #include "../ProgressBar.h"
// #include "../ProgDlg.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef WIN32
#define strcasecmp _stricmp
#endif
# ifndef strncasecmp
# define strncasecmp _strnicmp
# endif
//#define GET_LINE() if (!fgets(buf, 1024, pFile)) goto readerror
#define GET_LINE() (fgets(buf, 1024, pFile)) 
#define COND_READ(cond, where, len) if ((cond) && !fread((void *)&(where), (len), 1, pFile)) return false
#define LINE_IS(text) !strncasecmp(buf, text, strlen(text))

#define BIGNUM 1.0e10

CxFileIO::CxFileIO()
{

}

CxFileIO::~CxFileIO()
{

}


CxMesh* CxFileIO::ReadFile(const char* filename)
{
	const char* extension;
	extension = strrchr(filename, '.');
	   
	 if (extension)
		 extension++; //start looking after the .
	 else
		 return NULL; //don't try to place guess the file type

	 if ( !strcasecmp( "stl", extension))
		 return ReadSTLFile(filename);
	 else if ( !strcasecmp( "obj", extension))
		// return ReadObjFile(filename);
		return ReadObjFile2(filename);
	 else if ( !strcasecmp( "off", extension) )
		 return ReadOFFFile(filename);
	 else if ( !strcasecmp("vm", extension) )
		 return ReadVMBinFile(filename);
	 else
	 {
		 assert( 0 && "The input file format is unsuported.");
		 return 0;
	 }
	 return NULL;

}


/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2007-12-11 10:32:21
 * Function name	: CxFileIO::SaveFileAs
 * Description	    : 
 * Return type		: void 
 * Argument         : const char* strFileName
 * Argument         : const CxMesh* pMesh
 *================================================================*/
void CxFileIO::SaveFileAs(const char* strFileName, const CxMesh* pMesh)
{
	const char* extension;
	extension = strrchr(strFileName, '.');
	   
	 if (extension)
		 extension++; //start looking after the .
	 else
		 return; //don't try to place guess the file type

	 if (!strcasecmp( "stl", extension))
		 WriteAsSTLBinFile(strFileName, pMesh);
	 else if ( !strcasecmp( "obj", extension))
		 WriteAsObjFile(strFileName, pMesh);
	 else if ( !strcasecmp( "ply", extension) )
		 WriteAsciiPlyFile(strFileName, pMesh );
	 else if ( !strcasecmp( "npts", extension) )
		 WriteNPTSFile(strFileName, pMesh );

	 return;
}

CxMesh* CxFileIO::ReadSTLFile(const char* strFileName)
{
	FILE* pFile = fopen(strFileName, "r");

    #define GET_LINE() (fgets(buf, 1024, pFile)) 

	char buf[1024];
	GET_LINE();    // header file information, skit it;
	GET_LINE();	
	string str(buf);

	fclose(pFile);

	// if the second line has "facet normal", then it is a acsii format.
	// I do not know whether this method is robust or not, :-)
	//if ( str.find("facet normal") == 0 )
	if ( str.find("facet") == 0 )
		return ReadSTLAcsiiFile(strFileName);
	else
		return ReadSTLBinFile(strFileName);
}

CxMesh* CxFileIO::ReadSTLBinFile1(const CString& strFileName)
{

	CFileException ex;
	CFile stlFile;	
	if (!stlFile.Open((LPCTSTR)strFileName, CFile::modeRead | CFile::typeBinary, &ex))
	{
		return NULL;
	}

	int nFace;
	stlFile.Seek(80L, CFile::begin);
	stlFile.Read(&nFace, 4);

	CxMesh* pTriMesh = NULL;
 	pTriMesh = new CxMesh;
	pTriMesh->Reserve(nFace*3, nFace);

	for (int i=0; i<nFace; ++i)
	{
		// reading normal of triangle;
		float normal[3];
		stlFile.Read(normal, 4*3);
        
		CxFace face;
		face.SetNormal(normal[0], normal[1], normal[2]);
		face.SetIndex(i);

		// start reading three vertices of triangle;
		for (int j=0; j<3; ++j)
		{
			float vert[3];
			stlFile.Read(vert, 4*3);
			CxVertex3d v(vert[0], vert[1], vert[2]);
			v.SetIndex(3*i+j);
			face[j] = 3*i+j;
			pTriMesh->AddVertex(v);
		}

		pTriMesh->AddFace(face);
		stlFile.Seek((LONG)2, CFile::current);
	}

//	pNewModel->Trace();
	stlFile.Close();

	return pTriMesh;
}

/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-26 21:49:24
 * Function name	: CxFileIO::ReadSTLFile
 * Description	    : 读取二进制STL文件并去除冗余点;
 * Return type		: CxModel* 
 * Argument         : const CString& strFileName
 *================================================================*/
CxMesh* CxFileIO::ReadSTLBinFile(const CString& strFileName)
{
#if 1
	std::string fileName = strFileName;
	std::fstream stlFile;
	stlFile.open(fileName.c_str(),std::ios::in | std::ios::binary );
	if(!stlFile)
	{
		printf("Failed to open file!\n");
		return NULL;
	}

	stlFile.seekg(0,stlFile.end);
	int fileLength = stlFile.tellg();
	stlFile.seekg(0,stlFile.beg);
	char* buffer = new char[fileLength];
	printf("Reading Files....\n");
	stlFile.read(buffer,fileLength);
	if(stlFile)
		printf("All character read successfully!\n");
	else
		printf("Failed to read the file!\n");
	stlFile.close();

	char* ptr = buffer + 80;

	int nFace = *((int*)ptr);

	printf("NumFace: %d\n",nFace);

	ptr += 4;

	CxMesh* pTriMesh = NULL;
	pTriMesh = new CxMesh;
	pTriMesh->Reserve(nFace*3, nFace);

	typedef std::set<CxVertex3d, std::less<CxVertex3d> >  VertexSet;
	VertexSet tmpVertexSet;      // 储存模型的实际顶点数;
	std::pair<VertexSet::iterator, bool> pair_set;

	int nIndex = 0;
	int nCnt = 0;
	for (int i=0; i<nFace; ++i)
	{

		float normal[3];
		memcpy(normal,ptr,4 * 3);

		ptr += 12;

		CxFace face;
		face.SetNormal(normal[0], normal[1], normal[2]);
		face.SetIndex(i);

		// start reading three vertices of triangle;
		for (int j=0; j<3; ++j)
		{
			float vert[3];
			memcpy(vert,ptr,4*3);
			ptr += 4*3;
			CxVertex3d v(vert[0], vert[1], vert[2]);
			pair_set = tmpVertexSet.insert(v);
			if ( pair_set.second == true )
			{
				pTriMesh->AddVertex(CxVertex3d(vert[0], vert[1], vert[2], nIndex));
				CxVertex3d& cv = const_cast<CxVertex3d&>(*pair_set.first);
				cv.SetIndex(nIndex++);
			}

			VertexSet::iterator it = tmpVertexSet.find(v);
			if (it != tmpVertexSet.end())
				face[j] = (*it).GetIndex();

		}

		pTriMesh->AddFace(face);
		ptr += 2;
	}
	delete[] buffer;

	pTriMesh->SetHasNormalFace(true);
	pTriMesh->SetHasNormalVertex(false);

	return pTriMesh;
#endif
}


/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2008-3-24 17:16:02
 * Function name	: CxFileIO::ReadSTLAsciiFile
 * Description	    : 
 * Return type		: CxMesh* 
 * Argument         : const char* strFileName
 *================================================================*/
CxMesh* CxFileIO::ReadSTLAcsiiFile(const char* strFileName)
{
	FILE* pFile = fopen(strFileName, "r");
	if ( !pFile )
		return NULL;

	CxMesh* pNewMesh = new CxMesh;

	bool bHasNormalVertex = false;
	static int nCount = 0;

#define GET_LINE() (fgets(buf, 1024, pFile)) 
#define LINE_IS(text) !strncasecmp(buf, text, strlen(text))

	char buf[1024];
	GET_LINE();    // header file information, skit it;

	typedef std::set<CxVertex3d, std::less<CxVertex3d> >  VertexSet;
	VertexSet tmpVertexSet;      // 储存模型的实际顶点数;
	std::pair<VertexSet::iterator, bool> pair_set;
	
	int nIndex = 0;
	int nFace = 0;
	while ( !feof(pFile) )
	{	
		if ( buf[0] == '\n' )
			 GET_LINE();
		
		GET_LINE();
		if ( LINE_IS("endsolid") )
			break;

		float x = .0f, y = .0f, z = .0f;	
		CxFace face;
		if (sscanf(buf, "%*s%*s%f%f%f", &x, &y, &z) == 3)
		{
			face.SetIndex(nFace++);
			face.SetNormal(x, y, z);
		}

        GET_LINE();  // outer loop;	

		// start reading three vertices of triangle;
		for (int j=0; j<3; ++j)
		{
			GET_LINE();  // vtx;
			float vert[3];
			sscanf(buf, "%*s%f%f%f", &vert[0], &vert[1], &vert[2]);
			CxVertex3d v(vert[0], vert[1], vert[2]);
			pair_set = tmpVertexSet.insert(v);
			if ( pair_set.second == true )
			{
				pNewMesh->AddVertex(CxVertex3d(vert[0], vert[1], vert[2], nIndex));
				// (*pair_set.first).SetIndex(nIndex++);
				CxVertex3d& cv = const_cast<CxVertex3d&>(*pair_set.first);
				cv.SetIndex(nIndex++);
			}

			VertexSet::iterator it = tmpVertexSet.find(v);
			if (it != tmpVertexSet.end())
				face[j] = (*it).GetIndex();
			
		}

		GET_LINE();		
		GET_LINE();	
		
		pNewMesh->AddFace(face);	
	}


//	pNewMesh->Trace();
	fclose(pFile);
	return pNewMesh;
}

void CxFileIO::WriteAsSTLBinFile(const CString& strFileName, const CxMesh* pMesh)
{
	// Check for valid file
//	CFile file;	
	CStdioFile file;
	CFileException ex;

	// Try to open file 
	if (!file.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &ex))
	{
		AfxMessageBox("Unable to open file for writing");
		return;
	}

	// Meshes
    int nTriangles = pMesh->NumFaces();


	TRY
	{
		file.Seek(80L, CFile::begin);
		file.Write(&nTriangles, 4);
		
		for (int i = 0; i < nTriangles; i++)
		{
			const CxFace& rFace = pMesh->GetFace(i);
			
			double normal_x;
			double normal_y;
			double normal_z;

			rFace.GetNormal(normal_x, normal_y, normal_z);

			double normal[3] = {normal_x, normal_y, normal_z};
			file.Write(normal, 12);

			const CxVertex3d& v1 = pMesh->GetVertex(rFace[0]);
			double vert1[3] = {v1[0], v1[1], v1[2]};
			file.Write(vert1, 12);
			const CxVertex3d& v2 = pMesh->GetVertex(rFace[1]);
			double vert2[3] = {v2[0], v2[1], v2[2]};
			file.Write(vert2, 12);
			const CxVertex3d& v3 = pMesh->GetVertex(rFace[2]);
			double vert3[3] = {v3[0], v3[1], v3[2]};
			file.Write(vert3, 12);
					
			file.Seek(2L, CFile::current);
		}
	}
	CATCH(CFileException, e)
	{
		AfxMessageBox("Failed to write file!");
		return;
	}
	END_CATCH

   // Close file
   file.Close();

	
#if 0


	// write ascii format 

	TRY
	{

		CString string;
		file.WriteString("solid ");

//		file.WriteString(filename + "\n");
// 		file.WriteString(filename + " is produced by wujh@sia.cn");
////	file.WriteString("is produced by wujh@sia.cn");

		string.Format(": %d vertices, %d facets\n", pMesh->NumVertices(), nTriangles);
		file.WriteString(string);

		for ( int i=0; i<nTriangles; ++i )
		{

			const CxFace& face = pMesh->GetFace(i);
			const CxVector3d& normal = face.GetNormal();			

            file.WriteString("facet normal ");
			string.Format("%.7g %.7g %.7g\n", normal[0], normal[1], normal[2]);
			file.WriteString(string);
			file.WriteString("  outer loop");
			file.WriteString("\n");

			for ( int j=0; j<3; j++ )
			{
				const CxVertex3d& v = pMesh->GetVertex(face[j]);
		    	file.WriteString("     vertex  ");
				string.Format("%.7g %.7g %.7g\n", v[0], v[1], v[2]);
   				file.WriteString(string);			
			}

			file.WriteString("  endloop\n");
			file.WriteString("endfacet\n");
		}

		file.WriteString("endsolid");	
	}

	CATCH(CFileException, e)
	{
		AfxMessageBox("Error during writing");
		return;
	}

	END_CATCH
	file.Close();
#endif

   return;
}


/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2007-12-10 13:47:11
 * Function name	: CxFileIO::ReadObjFile
 * Description	    : 
 * Return type		: CxMesh* 
 * Argument         : const char* strFileName
 *================================================================*/
CxMesh* CxFileIO::ReadObjFile(const char* strFileName)
{

	FILE* pFile = fopen(strFileName, "r");
	if ( !pFile )
		return NULL;

	CxMesh* pNewMesh = new CxMesh;

	bool bHasNormalVertex = false;
	static int nCount = 0;

#define GET_LINE() (fgets(buf, 1024, pFile)) 
#define LINE_IS(text) !strncasecmp(buf, text, strlen(text))

	static int id = 0;	
	static int index = 0;

	while (1) 
	{
		char buf[1024];
		if (!GET_LINE())
			break;
		if (LINE_IS("v ") || LINE_IS("v\t")) 
		{
			float x, y, z;
			if (sscanf(buf+1, "%f %f %f", &x, &y, &z) != 3) 
			{
				delete pNewMesh;
				fclose(pFile);
				return NULL;
			}

			CxVertex3d v(x, y, z);
			v.SetIndex(id++);
			pNewMesh->AddVertex(v);
		} 
		else if ( LINE_IS("vn ") || LINE_IS("vn\t"))
		{
			bHasNormalVertex = true;
			float nx, ny, nz;
			if ( 3 == sscanf(buf, "%*s %f %f %f", &nx, &ny, &nz))
			{
				pNewMesh->GetVertex(nCount++).SetNormal(nx, ny, nz);
			}
		}
		else if ( LINE_IS("vt ") || LINE_IS("vt\t"))
		{
			// nothing to do;
			;
		}
		else if (LINE_IS("f ") || LINE_IS("f\t")) 
		{
			int vert1, vert2, vert3;
			int n1, n2,  n3;
			int t1, t2, t3;
		
			if (sscanf(buf+1, "%d %d %d", &vert1, &vert2, &vert3) == 3)
			{
				// this face only has vertex;
				CxFace face(vert1 - 1, vert2 - 1, vert3 - 1);
				face.SetIndex(index++);
				pNewMesh->AddFace(face);
			}
			else if (6 == sscanf(buf+1, "%d//%d %d//%d %d//%d", &vert1, &n1, &vert2, &n2, &vert3, &n3))
			{
				// This face has vertex and normal index
				CxFace face(vert1 - 1, vert2 - 1, vert3 - 1);
				face.SetIndex(index++);
				pNewMesh->AddFace(face);
			}
			else if ( 6 == sscanf(buf+1, "%d/%d %d/%d %d/%d", &vert1, &t1, &vert2, &t2, &vert3, &t3))
			{
				// This face has vertex and texture coordinate index
				CxFace face(vert1 - 1, vert2 - 1, vert3 - 1);
				face.SetIndex(index++);
				pNewMesh->AddFace(face);
			}

		}
	}

	if ( bHasNormalVertex )
	{
		assert( nCount == pNewMesh->NumVertices() );
		pNewMesh->SetHasNormalVertex(true);
		pNewMesh->SetHasNormalFace(false);
	}

	fclose(pFile);
 
	id = 0;	
	index = 0;
	nCount = 0;

//	pNewMesh->Trace();

	return pNewMesh;
}


/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2008-3-17 13:43:05
 * Function name	: CxFileIO::ReadObjFile2
 * Description	    : 
 * Return type		: CxMesh* 
 * Argument         : const char* strFileName
 *================================================================*/
CxMesh* CxFileIO::ReadObjFile2(const char* strFileName)
{
	FILE* pFile = fopen(strFileName, "r");
	if ( !pFile )
		return NULL;
	
	CxMesh* pNewMesh = new CxMesh;
	
#define BUF_SIZE 256
	
    char buf[BUF_SIZE];
    int match = 0;
	static int nIdVertex  = 0;
	static int nIdFace = 0;
	static int nCount = 0;
	bool bHasNormalVertex = false;

#define GET_LINE() (fgets(buf, 1024, pFile)) 
#define LINE_IS(text) !strncasecmp(buf, text, strlen(text))
string str;	
string::size_type loc;

    while ( fscanf( pFile, "%s", buf) != EOF ) 
	{
		
        switch (buf[0]) 
		{
		case '#':
			//comment line, eat the remainder
			fgets( buf, BUF_SIZE, pFile);
			str = buf;
			loc = str.find( "Mesh Color", 0 );   
			if( loc != string::npos ) 
			{
				int r, g, b;
				sscanf(buf, "%*s%*s%d %d %d", &r, &g, &b);
				pNewMesh->SetColor((unsigned char)r, (unsigned char)g, (unsigned char)b);
			}
			str = buf;
			loc = str.find("Vertex count", 0);
			if ( loc != string::npos )
			{
				int nVertex;
				sscanf(buf, "%*s%*s%*s%d", &nVertex);
				pNewMesh->ReserveArrayVertex(nVertex);
			}
			str = buf;
			loc = str.find("Face count", 0);
			if ( loc != string::npos )
			{
				int nFace;
				sscanf(buf, "%*s%*s%*s%d", &nFace);
				pNewMesh->ReserveArrayVertex(nFace);
			}
			break;
			
		case 'v':
			switch (buf[1]) 
			{            
			case '\0':
				float x, y, z;
				match = fscanf(pFile, "%f %f %f", &x, &y, &z);
				if ( match == 3 )
				{
					CxVertex3d v(x, y, z);
					v.SetIndex(nIdVertex++);
					pNewMesh->AddVertex(v);
				}
				
				break;
				
			case 'n':
				// normal, 3 components
				bHasNormalVertex = true;						
				float nx, ny, nz;
				match = fscanf(pFile, "%f %f %f", &nx, &ny, &nz);
				
				if ( match == 3 )
				{
					assert( match == 3);
					pNewMesh->GetVertex(nCount++).SetNormal(nx, ny, nz);					
				}
				
				break;				
			case 't':
				// texcoord, skip currently;
				
				break;
			}
			break;
			
            case 'f':
                //face
                fscanf( pFile, "%s", buf);
				
                int v1, v2, v3;
				int n1, n2, n3;
				int t1, t2, t3;
				
				//This face has vertex/texture coordinate/normal indices
				if (sscanf(buf,"%d/%d/%d",&v1,&t1,&n1) == 3)
				{
					fscanf(pFile,"%d/%d/%d %d/%d/%d",&v2,&t2,&n2,&v3,&t3,&n3);
					CxFace f(v1-1,v2-1,v3-1);
					f.SetIndex(nIdFace++);
					pNewMesh->AddFace(f);
				}
				//This face has vertex and normal indices
                else if ( sscanf(buf, "%d//%d", &v1, &n1) == 2 )
				{
					fscanf(pFile, "%d//%d %d//%d", &v2, &n2, &v3, &n3 );
					CxFace f(v1-1, v2-1, v3-1);
					f.SetIndex(nIdFace++);
					pNewMesh->AddFace(f);
				}
				// this face has vertex and texture coordinates indices;
				else if ( sscanf(buf, "%d/%d", &v1, &t1) == 2 )
				{
					fscanf(pFile, "%d/%d %d/%d", &v2, &t2, &v3, &t3);
					CxFace f(v1-1, v2-1, v3-1);
					f.SetIndex(nIdFace++);
					pNewMesh->AddFace(f);
					
				}
				// this face only has vertex;
				else if ( sscanf(buf, "%d", &v1) == 1 )
				{
					fscanf(pFile, "%d %d", &v2, &v3 );
					CxFace f(v1-1, v2-1, v3-1);
					f.SetIndex(nIdFace++);
					pNewMesh->AddFace(f);
				}             		
                else 
				{
                    //bad format
                    assert(0);
                    fgets( buf, BUF_SIZE, pFile);
                }
                break;
				
            case 's':
            case 'g':
            case 'u':
                //all presently ignored
            default:
                fgets( buf, BUF_SIZE, pFile);
        };
    }
	
    fclose(pFile);
	
	if ( bHasNormalVertex )
	{
		assert( nCount == pNewMesh->NumVertices() );
		pNewMesh->SetHasNormalVertex(true);
		pNewMesh->SetHasNormalFace(false);
	}
	
	
    nIdVertex  = 0;
    nIdFace = 0;
    nCount = 0;

#undef BUF_SIZE

	return pNewMesh;
}

/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2007-12-10 16:11:24
 * Function name	: CxFileIO::WriteAsObjFile
 * Description	    : 
 * Return type		: void 
 * Argument         : const char* strFileName
 * Argument         : const CxMesh* pMesh
 *================================================================*/
void CxFileIO::WriteAsObjFile(const char* strFileName, const CxMesh* pMesh)
{
	if ( pMesh == NULL )
		return;

	int nVerts = pMesh->NumVertices();
	int nFaces = pMesh->NumFaces();

	FILE* pFile = fopen(strFileName, "wb");
	if ( pFile == NULL )
		return;

// 	fprintf(pFile, "# Wavefront OBJ file format with ext .obj\n");
// 	fprintf(pFile, "# This file is producted by jh.wu@siat.ac.cn\n");
// 	fprintf(pFile, "# Vertex count = %d\n", nVerts);
// 	fprintf(pFile, "# Face count = %d\n", nFaces);
// 	fprintf(pFile, "# Mesh Color %d %d %d\n", pMesh->GetColor()->r(), pMesh->GetColor()->g(), pMesh->GetColor()->b());

	for ( int i=0; i<nVerts; ++i )
	{
		const CxVertex3d& v = pMesh->GetVertex(i);
		fprintf(pFile, "v %.7g %.7g %.7g\n", v[0], v[1], v[2]);
	}
// 	for ( int i=0; i<nVerts; ++i )
// 	{
// 		const CxVertex3d& v = pMesh->GetVertex(i);
// 		const CxVector3d& normal = v.GetNormal();
// 		fprintf(pFile, "vn %.7g %.7g %.7g\n", normal[0], normal[1], normal[2]);
// 	}

	for ( int j=0; j<nFaces; ++j )
	{
		const CxFace& f = pMesh->GetFace(j);
//		fprintf(pFile, "f %d//%d %d//%d %d//%d\n", f[0]+1, f[0]+1, f[1]+1,  f[1]+1, f[2]+1, f[2]+1);
		fprintf(pFile, "f %d %d %d\n", f[0]+1, f[1]+1,  f[2]+1);

	}

	fclose(pFile);
}

void CxFileIO::WriteAsciiPlyFile(const char* filename, const CxMesh* pMesh)
{
	if (!filename || *filename == '\0')
		return;

	unsigned int NbVertex = pMesh->NumVertices();
	unsigned int NbFace   = pMesh->NumFaces();

	if ( NbVertex == 0 || NbFace == 0 )
		return;


	FILE* pFile = fopen(filename, "wb");

	if ( !pFile )
	{
		AfxMessageBox("Can not open file to write!\n");
		return;
	}

	// write ply header;
	fprintf(pFile,"ply\nformat ascii 1.0\nelement vertex  ");
	fprintf(pFile,"%d\n", NbVertex);
	fprintf(pFile, "property float32 x\nproperty float32 y\nproperty float32 z\n");
	fprintf(pFile,"element face %d\n", NbFace);
	fprintf(pFile, "property list uint8 int32 vertex_indices\nend_header\n");

	// write vertices...
	for (unsigned i = 0; i<NbVertex; ++i)
	{
		const CxVertex3d& v = pMesh->GetVertex(i);
		fprintf(pFile, "%.7g %.7g %.7g\n", v[0], v[1], v[2]);
	}

	// write faces...
	for ( unsigned i = 0; i<NbFace; ++i)
	{
		const CxFace& f = pMesh->GetFace(i);
		fprintf(pFile,"3 %d %d %d\n", f[0], f[1], f[2]);
	}

	fclose(pFile);

	return;
}


//////////////////////PLY////////////////////////////////////////////


void CxFileIO::WriteNPTSFile(const char* filename, const CxMesh* pMesh)
{
	if (!filename || *filename == '\0')
		return;

	unsigned int NbVertex = pMesh->NumVertices();
	unsigned int NbFace   = pMesh->NumFaces();

	if ( NbVertex == 0 || NbFace == 0 )
		return;


	FILE* pFile = fopen(filename, "wb");

	if ( !pFile )
	{
		AfxMessageBox("Can not open file to write!\n");
		return;
	}

	// write vertices...
	for (unsigned i = 0; i<NbVertex; ++i)
	{
		const CxVertex3d& v = pMesh->GetVertex(i);
		const CxVector3d& n = v.GetNormal();
		fprintf(pFile, "%.7g %.7g %.7g %.7g %.7g %.7g\n", v[0], v[1], v[2], -n[0], -n[1], -n[2]);
	}

	fclose(pFile);

	return;
}

CxMesh* CxFileIO::ReadOFFFile(const char* strFileName)
{
	FILE* pFile = fopen(strFileName, "r");
	if ( !pFile )
		return NULL;


	bool bHasNormalVertex = false;
	static int nCount = 0;

#define GET_LINE() (fgets(buf, 1024, pFile)) 
#define LINE_IS(text) !strncasecmp(buf, text, strlen(text))

	char buf[1024];
	GET_LINE();    // header file information, skit it;
	if (!LINE_IS("OFF"))
		return NULL;

	GET_LINE();
	int nf, nv, ne;
	if ( sscanf(buf, "%d %d %d", &nv, &nf, &ne) != 3 )
		return NULL;
	
	CxMesh* pNewMesh = new CxMesh;
	pNewMesh->Reserve(nv, nf);

	for(int i=0; i<nv; ++i)
	{
		GET_LINE();
		float x, y, z;
		if ( sscanf(buf, "%f %f %f", &x, &y, &z) == 3 )
		{
			CxVertex3d v(x, y, z);
			v.SetIndex(i);
			pNewMesh->AddVertex(v);
		}
	}

	for (int i=0; i<nf; ++i)
	{
		GET_LINE();
		int v1, v2, v3;
		int n;
		if ( sscanf(buf, "%d %d %d %d", &n, &v1, &v2, &v3) == 4 && ( n == 3))
		{
			CxFace face(v1, v2, v3);
			face.SetIndex(i);
			pNewMesh->AddFace(face);
		}
	}

	fclose(pFile);

	return pNewMesh;
}

CxMesh* CxFileIO::ReadVMBinFile(const char* strFileName )
{
	std::string file = std::string(strFileName);
	std::size_t length = file.size();
	std::size_t pos = file.rfind("\\");
//	std::string meshName = file.substr(pos+1, length-1);
	std::size_t posdot = file.rfind(".");
	std::string meshName = file.substr(pos+1, posdot-pos-1);

	std::ifstream is(file, std::ifstream::binary);
	if (!is ) return NULL;

	assert ( 4 == sizeof(float) );
	assert ( 4 == sizeof(unsigned int) );

	// 读取模型的顶点个数和面片个数;
	unsigned int nv;
	is.read(reinterpret_cast<char*>(&nv), sizeof(unsigned int) );
	unsigned int nf;
	is.read(reinterpret_cast<char*>(&nf), sizeof(unsigned int) );

	CxMesh* pNewMesh = new CxMesh();
	pNewMesh->SetMeshName(meshName);
	pNewMesh->Reserve(nv, nf);

	struct triple{ float x, y, z;};

	for ( unsigned int i=0; i<nv; ++i )
	{
#if 0
		float x, y, z;
		is.read(reinterpret_cast<char*>(&x), sizeof(float) );
		is.read(reinterpret_cast<char*>(&y), sizeof(float) );
		is.read(reinterpret_cast<char*>(&z), sizeof(float) );
		pNewMesh->AddVertex(CxVertex3d(x, y, z));
#endif

		triple v;
		is.read(reinterpret_cast<char*>(&v), sizeof(triple) );
		pNewMesh->AddVertex( CxVertex3d( v.x, v.y, v.z) );
	}

	for ( unsigned int i=0; i<nv; ++i )
	{
		triple n;
		is.read(reinterpret_cast<char*>(&n), sizeof(triple) );
		pNewMesh->GetVertex(i).SetNormal((double)n.x, (double)n.y, (double)n.z );
	}

	for ( unsigned int j=0; j<nf; ++j )
	{
		unsigned int f[3];
		is.read(reinterpret_cast<char*>(&f), sizeof(f) );
		pNewMesh->AddFace( CxFace(f[0], f[1], f[2]) );
	}
	return pNewMesh;
}