// xMesh.cpp: 
// ***************************************************************
//  xMesh   version:  1.0   
//  -------------------------------------------------------------
//  Description:  implementation of the CxMesh class.
//  Author     :  wujh@sia.cn 
//  Created    :  07/27/2006
//  Modified   :  
//  -------------------------------------------------------------
//	Copyright (c) 2006 - All Rights Reserved
//
//  Shenyang Institute of Automation, Chinese Academy of Sciences
//  *****************************************************************
//
//  *****************************************************************

#include "stdafx.h"

//#include "../scene/glutils.h"

#include "xMesh.h"
#include "xUtil.h"
#include "xMeshKdTree.h"

#include "../xLib/xFileIO.h"

#include <set>
#include <algorithm>
using std::set;


#include <limits>
#include <iostream>
#include <fstream>
using namespace std;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CxMesh::CxMesh():m_strMeshName("")
{
	m_bHasNormalFace = m_bHasNormalVertex = false;
	m_bHasTextureCoords = false;
//	m_nListOpenGL = m_nListDone = 0;
	m_bModifiedFlag = false;

	m_bHasBoundingBox = false;
	m_bVisible = true;
	m_bBuffered = false;
	m_bTranspancey = false;

	m_animateCursor = 1.0f;

	m_iColorBinding = NOT_VERTEX_AND_FACE;

}

CxMesh::~CxMesh()
{
	m_ArrayVertex.erase(m_ArrayVertex.begin(), m_ArrayVertex.end());
	m_ArrayFace.erase(m_ArrayFace.begin(), m_ArrayFace.end());
	m_ArrayEdge.erase(m_ArrayEdge.begin(), m_ArrayEdge.end());

	ReleaseBuffers();

}

void CxMesh::Free()
{
	m_ArrayVertex.erase(m_ArrayVertex.begin(), m_ArrayVertex.end());
	m_ArrayFace.erase(m_ArrayFace.begin(), m_ArrayFace.end());

}

void CxMesh::SetFlagOnFaces(char flag)
{
	std::vector<CxFace>::iterator iter = m_ArrayFace.begin();
	for (; iter != m_ArrayFace.end(); ++iter )
		(*iter).SetFlag(flag);	
}

void CxMesh::CalculateBoundingBox()
{
	const unsigned int iNumVertices = NumVertices();
	if ( iNumVertices == 0 )
		return;

	if ( m_bHasBoundingBox )
		return; // 包围盒已计算过，返回;

	double max_x, min_x;
	double max_z, min_z;
	double max_y, min_y;
	
	const CxVertex3d& v = m_ArrayVertex[0];
	max_x = min_x = v.x();
	max_y = min_y = v.y();
	max_z = min_z = v.z();
	
	for ( VertexArray::size_type i=1; i<iNumVertices; ++i )
	{
		const CxVertex3d& vert = m_ArrayVertex[i];
		const double x = vert.x();
		const double y = vert.y();
		const double z = vert.z();
		max_x = x > max_x ? x : max_x;
		min_x = x < min_x ? x : min_x;
		max_y = y > max_y ? y : max_y;
		min_y = y < min_y ? y : min_y;
		max_z = z > max_z ? z : max_z;
		min_z = z < min_z ? z : min_z;
	}
	
	m_BoundingBox.SetMaxVertex(CxVertex3d(max_x, max_y, max_z));
	m_BoundingBox.SetMinVertex(CxVertex3d(min_x, min_y, min_z));

	m_bHasBoundingBox = true;
}

void CxMesh::DrawBoundingBox()const
{
	float xMax = (float) m_BoundingBox.xMax();
	float yMax = (float)m_BoundingBox.yMax();
	float zMax = (float)m_BoundingBox.zMax();
	float xMin = (float)m_BoundingBox.xMin();
	float yMin = (float)m_BoundingBox.yMin();
	float zMin = (float)m_BoundingBox.zMin();


	// turn on antialiasing;
	::glPushAttrib(GL_LIGHTING_BIT);
	::glEnable(GL_LINE_SMOOTH);		
	::glEnable(GL_BLEND);		
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	::glLineWidth(1.5f);	
	::glDisable(GL_LIGHTING);
	::glColor3f(1.0f, .0f, .0f);   // red pen color;
	::glBegin(GL_LINES); 
// 	::glBegin(GL_FILL);
	   
	   // along x axis
	   ::glVertex3f(xMin, yMin, zMin);
	   ::glVertex3f(xMax, yMin, zMin);
	   ::glVertex3f(xMin, yMin, zMax);
       ::glVertex3f(xMax, yMin, zMax);
	   ::glVertex3f(xMin, yMax, zMin);
       ::glVertex3f(xMax, yMax, zMin);
	   ::glVertex3f(xMin, yMax, zMax);
	   ::glVertex3f(xMax, yMax, zMax);
	   
	   // along y axis
	   ::glVertex3f(xMin, yMin, zMin);
	   ::glVertex3f(xMin, yMax, zMin);  
	   ::glVertex3f(xMin, yMin, zMax);
	   ::glVertex3f(xMin, yMax, zMax);   
	   ::glVertex3f(xMax, yMin, zMin);
	   ::glVertex3f(xMax, yMax, zMin);
	   ::glVertex3f(xMax, yMin, zMax);
	   ::glVertex3f(xMax, yMax, zMax);
	   
	   // along z axis
	   ::glVertex3f(xMin, yMin, zMin);
	   ::glVertex3f(xMin, yMin, zMax);
	   ::glVertex3f(xMin, yMax, zMin);
	   ::glVertex3f(xMin, yMax, zMax);
	   ::glVertex3f(xMax, yMin, zMin);
	   ::glVertex3f(xMax, yMin, zMax);
	   ::glVertex3f(xMax, yMax, zMin);
	   ::glVertex3f(xMax, yMax, zMax);
	   
	::glEnd();	
	//::glEnable(GL_LIGHTING);
	::glDisable(GL_LINE_SMOOTH);
	::glDisable(GL_BLEND);		
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	::glLineWidth(1.0f);
	::glPopAttrib();

}

void CxMesh::DrawVertexHighlight(double radius)const
{
	// this function is for debug;

 	::glNewList(HIGHLIGHT_VERTEX, GL_COMPILE_AND_EXECUTE);
 	int nv = (int)m_ArrayVertex.size();
 	for (int i=0; i<nv; ++i)
 		m_ArrayVertex[i].gltDraw();
 	::glEndList();

#if 0
	double r = .05;
	std::set<int> vertexSet2ring;
	for(int i=0; i<1; ++i)
	{
		const CxVertex3d& v = m_ArrayVertex.at(50);
		v.gltDraw(CxColor::black(), r);
		int n = v.NumVertexNeighbor();
		for(int j=0; j<n; ++j)
		{
			const CxVertex3d& neighbor = m_ArrayVertex.at( v.GetVertexNeighbor(j) );
			neighbor.gltDraw();
			int nNeigghbor2ring = neighbor.NumVertexNeighbor();
			for(int k=0; k<nNeigghbor2ring; ++k)
			{
				const CxVertex3d& neighbor_1ring = m_ArrayVertex.at( neighbor.GetVertexNeighbor(k));
				//neighbor_1ring.gltDraw(CxColor::pumpkin_orange(), r);
				vertexSet2ring.insert( neighbor.GetVertexNeighbor(k) );
			}
		} // end for j;

		int size = (int)vertexSet2ring.size();
		std::cout<< "size = " << size << std::endl;
		vertexSet2ring.erase(0);
		for(int j=0; j<n; ++j)
		{
			vertexSet2ring.erase(v.GetVertexNeighbor(j));
		} // end for j;
		std::cout<< "size = " << (int)vertexSet2ring.size() << std::endl;

		std::set<int>::const_iterator iter = vertexSet2ring.begin();
		for(; iter != vertexSet2ring.end(); ++iter)
		{
			int index = *iter;
			m_ArrayVertex.at(index).gltDraw(CxColor::pumpkin_orange(), r);
		}

	} // end for i;
#endif
	
}

/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-28 11:06:26
 * Function name	: CxMesh::DeleteRedundancyVertex
 * Description	    : 去除STL文件的冗余点, 节省储存空间及用于拓扑
 *                  : 信息重建; 
 * Return type		: void 
 *================================================================*/
void CxMesh::DeleteRedundancyVertex()
{
// 	vector<CxVertex3d> tmpVertexArray(m_ArrayVertex.begin(), m_ArrayVertex.end());
// 	std::sort(m_ArrayVertex.begin(), m_ArrayVertex.end());
// 	std::vector<CxVertex3d>::iterator new_end = std::unique(m_ArrayVertex.begin(), m_ArrayVertex.end());
// 	m_ArrayVertex.erase(new_end, m_ArrayVertex.end());
// 	
 	int size = (int)m_ArrayVertex.size();
// 	for(int t=0; t<m_ArrayVertex.size(); ++t)
// 		m_ArrayVertex[t].Trace();


	std::set<CxVertex3d> tmpVertexSet(m_ArrayVertex.begin(), m_ArrayVertex.end());
	int nVerts = (int)tmpVertexSet.size();
	std::set<CxVertex3d>::iterator iter =  tmpVertexSet.begin();
	int nIndex = 0;
	for (; iter != tmpVertexSet.end(); ++iter )
	{  
		CxVertex3d& v = const_cast<CxVertex3d&>(*iter);
		v.SetIndex(nIndex++);
		//	(*iter).SetIndex(nIndex++);
	}
	

	int nFaces = NumFaces();
	// 更新组成面的顶点的索引号;
	for ( int i=0; i<nFaces; ++i )
	{
		CxFace& face = m_ArrayFace[i];
		for ( int j=0; j<3; ++j )
		{
			std::set<CxVertex3d>::iterator it = tmpVertexSet.find(m_ArrayVertex[face[j]]);
			if ( it != tmpVertexSet.end() )
				face[j] = (*it).GetIndex();
		}
	}

	m_ArrayVertex.clear();
	m_ArrayVertex.reserve(tmpVertexSet.size());
	iter = tmpVertexSet.begin();
	for (; iter != tmpVertexSet.end(); ++iter )
		m_ArrayVertex.push_back(*iter);


// 	int isize = m_ArrayVertex.size();
// 	for ( i=0; i<isize; ++i)
// 		assert(i == m_ArrayVertex[i].GetIndex());

//	Trace();
}

void CxMesh::Trace()
{
	if ( NumFaces() == 0 )
		return;

	TRACE("nVerts: %d, nFaces: %d\n", NumVertices(), NumFaces());
	for (unsigned int i=0; i<NumVertices(); ++i)
		m_ArrayVertex[i].Trace();
	TRACE("FACE:\n");
	for (int i=0; i<(int)m_ArrayFace.size(); ++i )
	{
		const CxFace& face = m_ArrayFace[i];
		TRACE("%d: \n", i);
	//	TRACE("[%3d %3d %3d]\n", face[0], face[1], face[2]);
		for (int j=0; j<3; ++j)
			m_ArrayVertex[face[j]].Trace();
	}
}        

CxVertex3d CxMesh::Center(const CxFace& f)const
{
	return CxVertex3d((m_ArrayVertex[f[0]] + m_ArrayVertex[f[1]] + m_ArrayVertex[f[2]])/3.0f);
}

double CxMesh::Area(const CxFace& f)const
{
	return ::Area(m_ArrayVertex[f[0]], m_ArrayVertex[f[1]], m_ArrayVertex[f[2]]);

	CxVector3d v1(m_ArrayVertex[f[1]], m_ArrayVertex[f[0]]);
	CxVector3d v2(m_ArrayVertex[f[1]], m_ArrayVertex[f[2]]);
	CxVector3d v(v1^v2);
	return .50 * v.Length();
}


double CxMesh::Perimeter(const CxFace& f)const
{
	return ::Perimeter(m_ArrayVertex[f[0]], m_ArrayVertex[f[1]], m_ArrayVertex[f[2]]);
}

double CxMesh::Compacity(const CxFace& f) const
{
	double perimeter = Perimeter(f);
	if ( ::IsZero(perimeter) )
		return 0.0;
	else
		return (4.0*M_PI*Area(f)/(perimeter*perimeter));
}

void CxMesh::CreateAdjacency()
{
// 	BuildFaceNeighbor();
// 	BuildVertexNeighbor();
// 	return;

// 	Trace();
	unsigned int nFaces = NumFaces();
	if ( nFaces == 0 )
		return;

	unsigned nVerts = NumVertices();
	for ( unsigned int i=0; i<nVerts; ++i )
	{
		m_ArrayVertex[i].ClearFaceNeighbor();
		m_ArrayVertex[i].ClearVertexNeighbor();
		m_ArrayVertex[i].SetIndex(i); // 避免操作后没有赋索引出错？
	}

	// create face neighbor for vertex;
	for ( unsigned int i=0; i<nFaces; ++i )
	{
		CxFace& face = m_ArrayFace[i];
		face.SetIndex(i);  // 针对下面原因修改!!是否正确??
	//	assert( i == face.GetIndex() );  // 增加loop subdivision出现的bug；
		                                 // 原因是细分过程中索引号未更新!!
		for ( int j=0; j<3; ++j )
			m_ArrayVertex[face[j]].AddFaceNeighbor(i);

		for ( int k=0; k<3; ++k )
			face.SetFace(k, -1);
	}

	// create vertex neighbor for vertex;
	for ( unsigned int i=0; i<nVerts; ++i )
	{
		CxVertex3d& rVertex = m_ArrayVertex[i];
		int nFaceNeigbhor = rVertex.NumFaceNeighbor();
		for ( int j=0; j<nFaceNeigbhor; ++j )
		{
			const CxFace& face = m_ArrayFace[ rVertex.GetFaceNeighbor(j) ];
			for ( int k=0; k<3; ++k )
			{
				const CxVertex3d& v = m_ArrayVertex[ face[k] ];
				rVertex.AddVertexNeighbor(v.GetIndex());
				//rVertex.AddVertexNeighbor( face[k] );
			}
		} // end for j;
	} // end for i;

    
	// create face neighbor for face;
	for ( unsigned int i=0; i<nFaces; ++i )
	{
		CxFace& face = m_ArrayFace[i];
		for ( int j=0; j<3; ++j )
		{
			const CxVertex3d& rVertexThis = m_ArrayVertex[face[j]];
			const CxVertex3d& rVertexNext = m_ArrayVertex[face[(j+1)%3]];
			int nFaceNeighbor = rVertexThis.NumFaceNeighbor();
			for ( int k=0; k<nFaceNeighbor; ++k )
			{
				const CxFace& faceNeighbor = m_ArrayFace[rVertexThis.GetFaceNeighbor(k)];
				//if ( faceNeighbor.GetIndex() == face.GetIndex() )
				//	continue;
				if ( faceNeighbor.Equal(face) )
					continue;

				if ( face.GetFace(j) != -1 )
					continue;
				if ( faceNeighbor.HasVertex(rVertexThis.GetIndex()) && 
					 faceNeighbor.HasVertex(rVertexNext.GetIndex()))
					 face.SetFace(j, faceNeighbor.GetIndex());
					 //face.SetFace(j, rVertexThis.GetFaceNeighbor(k));

			} // end for k;
		}  // end for j;
	}  // end for i;

	// debug;
/*	
    for (i=0; i<NumVertices(); ++i)
	{
		const CxVertex3d& v = m_ArrayVertex[i];
		int nVertexNeighbor = v.NumVertexNeighbor();
		TRACE("\n");
		v.Trace();
		TRACE("   ", i);
		for (int j=0; j<nVertexNeighbor; ++j)
			TRACE("%d  ", v.GetVertexNeighbor(j));

		TRACE("==");
		for (j=0; j<v.NumFaceNeighbor(); ++j)
			TRACE("%d  ", v.GetFaceNeighbor(j));
	}

	for (i=0; i<NumFaces(); ++i)
	{
		const CxFace& f = m_ArrayFace[i];
		TRACE("\n%d  ", f.GetIndex());
		for (int j=0; j<3; j++)
			TRACE("%d ", f.GetFace(j));
	}
*/

	CreateEdgeArray();
}         

// Find the direct neighbors of each vertex
void CxMesh::BuildVertexNeighbor()
{

// 	if ( !m_ArrayNeigbhorVertex.empty() )
// 		return;

	if ( m_ArrayFace.empty() )
		return;

	vector<int> numneighbors( m_ArrayVertex.size() );
	unsigned i = 0;
	for ( i=0; i<m_ArrayFace.size(); ++i )
	{
		const CxFace& face = m_ArrayFace[i];
		numneighbors[face[0]]++;
		numneighbors[face[1]]++;
		numneighbors[face[2]]++;
	}

	// m_ArrayNeigbhorVertex.resize( m_ArrayVertex.size() );
	for ( i=0; i<m_ArrayVertex.size(); ++i )
	{
		//m_ArrayNeigbhorVertex[i].reserve( numneighbors[i] + 2 ); // slop for boundaries;
		m_ArrayVertex[i].ClearVertexNeighbor();
		m_ArrayVertex[i].ReserveVertexNeighbor( numneighbors[i] + 2 );
	}

	for ( i=0; i<m_ArrayFace.size(); ++i )
	{		
		const CxFace& face = m_ArrayFace[i];
		for ( int j=0; j<3; ++j )
		{
			CxVertex3d& rVertex = m_ArrayVertex[ face[j] ];
			const vector<int>& me = rVertex.GetVertexNeighborArray(); // m_ArrayNeigbhorVertex[ face[j] ];
			int n1 = face[(j+1)%3];
			int n2 = face[(j+2)%3];
			if ( find(me.begin(), me.end(), n1 ) == me.end())
				rVertex.AddVertexNeighbor(n1); // me.push_back(n1);
			if ( find(me.begin(), me.end(), n2 ) == me.end())
				rVertex.AddVertexNeighbor(n2); // me.push_back(n2);
		}
	}

}

// Find the faces touching each vertex
void CxMesh::BuildFaceNeighbor()
{
// 	if ( !m_ArrayNeigbhorFace.empty() )
// 		return;

	if ( m_ArrayFace.empty() )
		return;

	int nVert = (int)m_ArrayVertex.size();
	int nFace = (int)m_ArrayFace.size();

	vector<int> numadjacentfaces(nVert);
	
	for (int i=0; i<nFace; ++i )
	{
		int index = m_ArrayFace[i][0];
		numadjacentfaces[ m_ArrayFace[i][0] ]++;
		numadjacentfaces[ m_ArrayFace[i][1] ]++;
		numadjacentfaces[ m_ArrayFace[i][2] ]++;
	}

	//m_ArrayNeigbhorFace.resize( m_ArrayVertex.size() );
	for (int i=0; i<nVert; ++i )
	{
		m_ArrayVertex[i].ClearFaceNeighbor();
	//	m_ArrayNeigbhorFace[i].reserve(numadjacentfaces[i]);
		m_ArrayVertex[i].ReserveFaceNeigbhor( numadjacentfaces[i] );
	}

	for ( int i=0; i<nFace; ++i )
	{
		for (int j=0; j<3; j++ )
		{
			//m_ArrayNeigbhorFace[ m_ArrayFace[i][j] ].push_back(i);
			m_ArrayVertex[ m_ArrayFace[i][j] ].AddFaceNeighbor(i);
		}
	}
}

/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-4 16:57:36
 * Function name	: CxMesh::GetFaceNeighborInclusive
 * Description	    : 获得面f的邻面中包含顶点v1,v2的面索引;
 * Return type		: int 
 * Argument         : int f
 * Argument         : int v1
 * Argument         : int v2
 *================================================================*/
FaceId CxMesh::GetFaceNeighborInclusive(FaceId f, int v1, int v2)
{
	const CxFace& rFace = m_ArrayFace[f];
	for ( int i=0; i<3; ++i )
	{
		FaceId index = rFace.GetFace(i);
		if ( index != -1 )
			if ( m_ArrayFace[index].HasVertex(v1) && m_ArrayFace[index].HasVertex(v2) )
	    		return index;
	}

	return -1;	
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-16 19:36:34
 * Function name	: CxMesh::CalculateNormalPerFace
 * Description	    : Calcualte Normal for Every Face;
 * Return type		: void 
 *================================================================*/
void CxMesh::CalculateNormalPerFace()
{
	for ( VertexArray::size_type i=0; i<m_ArrayFace.size(); ++i )
	{
		CxFace& rFace = m_ArrayFace[i];
		const CxVertex3d& rV0 = m_ArrayVertex[rFace[0]];
		const CxVertex3d& rV1 = m_ArrayVertex[rFace[1]];
		const CxVertex3d& rV2 = m_ArrayVertex[rFace[2]];

 	    CxVector3d vec1(rV0, rV1);
 		CxVector3d vec2(rV0, rV2);

		CxVector3d normal = vec1.Cross(vec2);
		normal.Normalize();
		rFace.SetNormal(normal);
		
	}  // end for i;
}



/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-16 19:37:42
 * Function name	: CxMesh::CalculateNormalPerVertex
 * Description	    : Calculate Normal for Every Vertex;
 * Return type		: void 
 *================================================================*/
/*================================================================
 * 三角网格顶点法矢计算:最简单的方法为将连接改顶点每个三角面片(face)
 * 的法矢线性叠加, 缺点:未考虑邻点分步密度对该顶点法矢的影响,当该顶
 * 点的一侧密集,另一侧稀疏时,法矢方向会发生偏离,这种算法对于非均匀三
 * 角面片的计算精度较低
 * Reference:
 * Henri Gouraud, "Continuous Shading of curved Surfaces",
 * IEEE Transactions on Computers, C-20(6):623-629(June, 1971)
 *================================================================*/
void CxMesh::CalculateNormalPerVertex()
{
	if ( m_ArrayFace.empty() )
		return;

	if ( !HasNormalFace() )
		CalculateNormalPerFace();

	for ( VertexArray::size_type i=0; i<m_ArrayVertex.size(); ++i )
	{
		CxVertex3d& rVertex = m_ArrayVertex[i];
		int iNeighborFace = rVertex.NumFaceNeighbor();
		CxVector3d normal;
		
		for ( int j=0; j<iNeighborFace; ++j )
			normal += m_ArrayFace[rVertex.GetFaceNeighbor(j)].GetNormal();

	 	// normal /= normal.Length();
		normal.Normalize();
		rVertex.SetNormal(normal);

	} // end for i;

//	CalculateNormalPerVertexThurmer();

}

/*===================================================================*/
/* Reference:
/* Max N. "Weights for computing Vertex Normals from Facets Normals
/* Journal of Graphics Tools, 4(2):1-6(1999)
/*===================================================================*/
void CxMesh::CalculateNormalPerVertexMax()
{
	unsigned int nv = NumVertices();
	for ( unsigned int i=0; i<nv; ++i )
	{
		CxVertex3d& v = m_ArrayVertex.at(i);
		CxVector3d normal;
		unsigned int nFaceNeighbor = v.NumFaceNeighbor();
		for ( unsigned int j=0; j<nFaceNeighbor; ++j )
		{
			const CxFace& face_neighbor = m_ArrayFace.at(v.GetFaceNeighbor(j));
			int index = 0;
			CxVertex3d neigbhor[2];
			for ( unsigned int k=0; k<3; ++k )
			{
				const CxVertex3d& neigbhor_vertx = m_ArrayVertex.at(face_neighbor[k]);
				if ( v != neigbhor_vertx )
					neigbhor[index++] = neigbhor_vertx;
			}

			assert ( 2 == index );
			CxVector3d vec1(v, neigbhor[0]);
			CxVector3d vec2(v, neigbhor[1]);
			double angle = ::Angle(vec1, vec2);
			CxVector3d v = vec1 ^ vec2; // CROSS Product;   

			/*======================================
			          sin(a)              |E1 x E2|
			weight = --------  , sin(a) = ----------
			         |E1|*|E2|            |E1|*|E2|
			========================================*/
			double weight = v.Length() / (vec1.LengthSquared() * vec2.LengthSquared());
			normal += weight*face_neighbor.GetNormal();		
		}

		normal.Normalize();
		v.SetNormal(normal);

	} // end for i;

	/*====================================
	作者还提供了其他weight的计算方法,比如:                                      
	weight = |E1 x E2|, or
	weight = 1 / (|E1|*|E2|), or
	weight = 1 / ( sqrt(|E1|*|E2|) ).
	Refs: "A Comparison of Algorithms for 
	Vertex Normal Computation" S.S.Jin
	=====================================*/
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-17 20:05:00
 * Function name	: CxMesh::GetAverageLengthEdge
 * Description	    : 获得模型的边的平均长度;(所有边长之和/边数)
 * Return type		: void 
 *================================================================*/
double CxMesh::GetAverageLengthEdge()
{
	int nFaces = NumFaces();
	if ( nFaces <= 0 )
		return .0;

	double sum = .0;
	int nEdge = 0;
	for ( int i=0; i<nFaces; ++i )
	{
		const CxFace& rFace = m_ArrayFace[i];
		assert( i == rFace.GetIndex() );
		
		for ( int j=0; j<3; ++j )
		{
			int index = rFace.GetFace(j);
			if ( index == -1 )
				continue;

			if ( i > index )
			{
				sum += ::Distance(m_ArrayVertex[rFace[j]], 
					              m_ArrayVertex[rFace[(j+1)%3]] );
				nEdge++;
			}		
		} // end for j;
	} // end for i;

	return sum / nEdge;
}

void CxMesh::CreateEdgeArray()
{
	//if ( m_ArrayEdge.size() != 0 )
		m_ArrayEdge.clear();

	int nFaces = NumFaces();
	if ( nFaces <= 0 )
		return;

	for ( int i=0; i<nFaces; ++i )
	{
		const CxFace& rFace = m_ArrayFace[i];
		// assert( i == rFace.GetIndex() );
		
		for (int j = 0; j < 3; ++j)
		{
			int index = rFace.GetFace(j);
			
			// 	if ( index == -1 )
			// 		continue;
			
			if (i > index)
			{
				CxEdge edge(rFace[j], rFace[(j + 1)%3]);
				
				FaceArray tempArrayFace;
				if (FindNeighborFaceContainVertex(m_ArrayVertex[rFace[j]], m_ArrayVertex[rFace[(j + 1)%3]], tempArrayFace))
				{
					if ((IsVertexOnBoundary(m_ArrayVertex[rFace[j]]) == true) &&
						(IsVertexOnBoundary(m_ArrayVertex[rFace[(j + 1)%3]]) == true))
					{
						if (tempArrayFace.size() == 1)
							edge.SetFlag(true);
					}
					
					for (unsigned int k = 0; k < tempArrayFace.size(); ++k)
						edge.SetFace(k, tempArrayFace[k].GetIndex());
				}
				
				m_ArrayEdge.push_back(edge);

			}	
		} // end for j;	
		
	} // end for i;


// 	EdgeArray array;
// 	GenerateOrientationBoundaryEdge(array);

#if 0

	// debug;
	int size = m_ArrayEdge.size();
	for (i=0; i<size; ++i )
		m_ArrayEdge[i].Trace();

#endif

	return;
}


/*================================================================
 * Author/date      : wujh@sia.cn   2007-6-19 8:26:20
 * Function name	: CxMesh::IsVertexOnBoundary
 * Description	    : 判断顶点是否在边界, 是返回true, 否则为false
 * Return type		: bool 
 * Argument         : const CxVertex3d& rVertex
 *================================================================*/
bool CxMesh::IsVertexOnBoundary(const CxVertex3d& rVertex)
{
	return (rVertex.NumFaceNeighbor() != rVertex.NumVertexNeighbor() );

	/////////////////////////////
	int size = rVertex.NumFaceNeighbor();
	
	for ( int i=0; i<size; ++i )
		for ( int j=0; j<3; ++j )
		{
			const CxFace& rFace = m_ArrayFace[ rVertex.GetFaceNeighbor(i)];
			if ( rFace.GetFace(j) == -1 && ( rFace.v(j) == rVertex.GetIndex() ||
				 ( rFace.v((j+1)%3) == rVertex.GetIndex() ) ) )
				 return true;
		}

	return false;	
}

void CxMesh::DrawBoundaryEdge()
{
	if ( m_ArrayEdge.size () == 0 )
		return;

	int iEdges = static_cast<int>(m_ArrayEdge.size());
	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);             //to make edges look smooth  if there any
	glDisable(GL_POLYGON_OFFSET_FILL);		
	
	glLineWidth(2);
	::glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);
	::glColor3f(1.0f, 0.0f, .0f);    

#if 1
	for ( int i=0; i<iEdges; ++i )
	{
		const CxEdge& rEdge = m_ArrayEdge[i];
		if ( rEdge.IsOnBoundary() == true )
		{
			::glBegin(GL_LINES);
				::glVertex3d(m_ArrayVertex[rEdge.v1()].x(),
					         m_ArrayVertex[rEdge.v1()].y(),
							 m_ArrayVertex[rEdge.v1()].z());
				::glVertex3d(m_ArrayVertex[rEdge.v2()].x(),
					         m_ArrayVertex[rEdge.v2()].y(),
							 m_ArrayVertex[rEdge.v2()].z());
			::glEnd();
		}
	}
#endif

	
//	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glPopAttrib();


}

void CxMesh::DrawBoundaryVertex()
{
	int iVertices = static_cast<int>(m_ArrayVertex.size());
	if ( iVertices <= 0 )
		return;
	::glPushAttrib(GL_LIGHTING_BIT);
	::glDisable(GL_LIGHTING);

	for ( int i=0; i<iVertices; ++i )
	{
		const CxVertex3d& rVertex = m_ArrayVertex[i];
		if ( IsVertexOnBoundary(rVertex) == true )
		{
			rVertex.gltDraw();
#if 0
			::glPointSize(3);
			::glBegin(GL_POINTS);
			    ::glColor3f(1.0f, .0f, .0f);
				::glVertex3f(rVertex.x(), rVertex.y(), rVertex.z() );
			::glEnd();
#endif		
		}
	}

	//::glEnable(GL_LIGHTING);
	glPopAttrib();
	
	return;

}

void CxMesh::DrawBoundaryFace()
{
	unsigned int iFaces = NumFaces();
	if ( iFaces == 0 )
		return;

#if 1
	::glColor3f(.98f, .625f, .12f);//
	for ( std::vector<CxFace>::size_type i=0; i<iFaces; ++i )
	{
		CxFace& face = m_ArrayFace[i];
		for ( int j=0; j<3; ++j )
		{
			if ( IsVertexOnBoundary(m_ArrayVertex[face[j]]) )
				//if ( IsVertexOnBoundary(m_ArrayVertex[face[(j+1)%3]] ))
			{
				//boundary.push_back(face);
				face.SetColor(CxColor::pumpkin_orange());
				face.SetFlag(1);
				break;
			}	
		}
	} // end for i;
#endif

#if 0
	::glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT );
	::glDisable(GL_LIGHTING);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	::glEnable(GL_POLYGON_OFFSET_FILL);
	::glPolygonOffset(1.0f, 1.0f); // avoid z-fighting;
	::glColor3f(.98f, .625f, .12f);// pumpkin-orange;

	FaceArray boundary;
	boundary.reserve(iFaces);
	for ( std::vector<CxFace>::size_type i=0; i<iFaces; ++i )
	{
		CxFace& face = m_ArrayFace[i];
		for ( int j=0; j<3; ++j )
		{
			if ( IsVertexOnBoundary(m_ArrayVertex[face[j]]) )
				//if ( IsVertexOnBoundary(m_ArrayVertex[face[(j+1)%3]] ))
				{
					boundary.push_back(face);
					break;
				}
		}
	} // end for i;

	std::size_t size = boundary.size();
	::glBegin(GL_TRIANGLES);
	for (std::size_t k=0; k<size; ++k )
	{
		const CxFace& f = boundary[k];
		for ( int j=0; j<3; ++j )
			::glVertex3dv(m_ArrayVertex[f[j]]);
	}
	::glEnd();

	// Restore light and mode
	//::glDisable(GL_POLYGON_OFFSET_LINE);
	//::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//::glEnable(GL_LIGHTING);	
	::glPopAttrib();
#endif

	SetColorBinding(COLOR_PER_FACE);
}


/*================================================================
 * Author/date      : wujh@sia.cn   2007-6-19 12:35:56
 * Function name	: CxMesh::FindNeighborFaceContainVertex
 * Description	    : 从顶点1的邻接面找包含顶点2的面;
 * Return type		: int 
 * Argument         : const CxVertex3d& rV1
 * Argument         : const CxVertex3d& rV2
 * Argument         : vector<CxFace>& rArrayFace
 *================================================================*/
int CxMesh::FindNeighborFaceContainVertex(const CxVertex3d& rV1, 
										  const CxVertex3d& rV2, 
										  vector<CxFace>& rArrayFace)const
{
	int size = rV1.NumFaceNeighbor();
	for ( int i=0; i<size; ++i )
	{		
		if ( rV1.GetFaceNeighbor(i) != -1 )
		{
			const CxFace& rFace = m_ArrayFace[rV1.GetFaceNeighbor(i)];
			if (rFace.HasVertex(rV2.GetIndex()))
				rArrayFace.push_back(rFace);
		}

	}

	return rArrayFace.empty() ? 0 : 1;
}
int CxMesh::FindNeighborFaceContainVertex(const int v1, 
										  const int v2, 
										  vector<CxFace>& rArrayFace)const
{
	const CxVertex3d& rV1 = m_ArrayVertex[v1];
	int size = rV1.NumFaceNeighbor();
	for ( int i=0; i<size; ++i )
	{		
		if ( rV1.GetFaceNeighbor(i) != -1 )
		{
			const CxFace& rFace = m_ArrayFace[rV1.GetFaceNeighbor(i)];
			if (rFace.HasVertex(v2))
				rArrayFace.push_back(rFace);
		}

	}

	return rArrayFace.empty() ? 0 : 1;
}

/*================================================================
 * Author/date      : wujh@sia.cn   2007-6-19 14:42:43
 * Function name	: CxMesh::GenerateOrientationBoundaryEdge
 * Description	    : 建立边界数组, 上一条边的端点是下一条边的起始点;
 * Return type		: int 
 * Argument         : EdgeArray& rArrayBoundaryEdge
 *================================================================*/
int CxMesh::GenerateOrientationBoundaryEdge(EdgeArray& rArrayBoundaryEdge)
{
	if ( m_ArrayEdge.empty() )
		CreateEdgeArray();

	int size = static_cast<int>(m_ArrayEdge.size());
	EdgeArray tempArrayEdge;
	for (int i=0; i<size; ++i )
	{
		const CxEdge& rEdge = m_ArrayEdge[i];
		if ( rEdge.IsOnBoundary() )
			tempArrayEdge.push_back(rEdge);
	}
	
	
	size = (int)tempArrayEdge.size();

	if ( size == 0 )
	{
		return 0; // it is a closed mesh;
	}

// 	// debug;
// 	for ( i=0; i<size; ++i )
// 		tempArrayEdge[i].Trace();


	rArrayBoundaryEdge.push_back(tempArrayEdge[0]);
	vector<int> flagVec(tempArrayEdge.size(), 0);
	for ( int i=1; i<size; ++i )
	{
		const CxEdge& edge = rArrayBoundaryEdge[i-1];
		//const CxEdge& edge = tempArrayEdge[i-1];

		CxEdge e = RetrivelEdgeStartedVertex(tempArrayEdge, edge.v2(), flagVec);
	//	rArrayBoundaryEdge.push_back(RetrivelEdgeStartedVertex(tempArrayEdge, edge.v2()));
		rArrayBoundaryEdge.push_back(e);

	}
    // 
// 	TRACE("\nAfter orientation....\n");
// 	for (i=0; i<size; ++i )
// 		rArrayBoundaryEdge[i].Trace();

	return 1;

}


double CxMesh::CalculateAreaFace(const CxFace& rFace)
{
	//assert( rFace.GetIndex() != -1 );
	
	const CxVertex3d& v1 = m_ArrayVertex[rFace[0]];
	const CxVertex3d& v2 = m_ArrayVertex[rFace[1]];
	const CxVertex3d& v3 = m_ArrayVertex[rFace[2]];

	CxVector3d vec1(v2, v3);
	CxVector3d vec2(v2, v1);

	CxVector3d vec = vec1.Cross(vec2);

	return vec.Length()/2.0;

}

/*================================================================
 * Author/date      : wujh@sia.cn   2007-6-19 14:47:19
 * Function name	: CxMesh::RetrivelEdgeStartedVertex
 * Description	    : 在边数组里查询端点为 rVertexStarted 的边,
                    : 并返回以它为起始点的边;
 * Return type		: CxEdge 
 * Argument         : EdgeArray& rArrayEdge
 * Argument         : int vertexStarted
 *================================================================*/
CxEdge CxMesh::RetrivelEdgeStartedVertex(EdgeArray& rArrayEdge, 
										 int vertexStarted,
										 vector<int>& flagVec)
{
	int iEdges = (int)rArrayEdge.size();
	for ( int i=1; i<iEdges; ++i )
	{
		const CxEdge& rEdge = rArrayEdge[i];
		if ( rEdge.HasVertex(vertexStarted) && flagVec[i] == 0 )
		{
			flagVec[i] = 1;   // 处理过的边做标识, 下次不在处理;
			return CxEdge(vertexStarted, rEdge.GetVertex(vertexStarted));
		}
	}

	return CxEdge();

}


/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2007-12-8 19:39:23
 * Function name	: CxMesh::ScaleMesh
 * Description	    : 将模型缩放到边长为scale*2的长方体空间
                    : Center mesh around origin.
 * Return type		: void 
 * Argument         : double scale  = 1.0f
 *================================================================*/
void CxMesh::ScaleMesh(double scale /* = 1.0 */)
{
	size_t size = m_ArrayVertex.size();
	if ( size == 0 )
		return;

	double max_x, min_x;
	double max_z, min_z;
	double max_y, min_y;

	const CxVertex3d& v = m_ArrayVertex[0];
	max_x = min_x = v[0];
	max_y = min_y = v[1];
	max_z = min_z = v[2];

	for ( size_t i=1; i<size; ++i )
	{
		const CxVertex3d& v = m_ArrayVertex[i];
		const double x = v[0];
		const double y = v[1];
		const double z = v[2];
		max_x = x > max_x ? x : max_x;
		min_x = x < min_x ? x : min_x;
		max_y = y > max_y ? y : max_y;
		min_y = y < min_y ? y : min_y;
		max_z = z > max_z ? z : max_z;
		min_z = z < min_z ? z : min_z;
	}

	double xRange = max_x - min_x;
	double yRange = max_y - min_y;
	double zRange = max_z - min_z;
	double range = (xRange > yRange )? (xRange > zRange ? xRange : zRange) :
		(yRange > zRange ? yRange : zRange);

	CxVertex3d center;
	center[0] = 0.5*(max_x + min_x);
	center[1] = 0.5*(max_y + min_y);
	center[2] = 0.5*(max_z + min_z);

	scale = scale/range;

	for ( size_t i=0; i<size; ++i )
	{
		CxVertex3d& v = m_ArrayVertex.at(i);
		v -= center;
		v *= scale;
	} // end for i;

}


/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2007-12-12 10:07:15
 * Function name	: CxMesh::DrawNormalVertex
 * Description	    : 
 * Return type		: void 
 *================================================================*/
void CxMesh::DrawNormalVertex()
{
	if ( !HasNormalVertex() )
		CalculateNormalPerVertex();

	int nVertex = NumVertices();

	const float scale = .15f;
	
	::glPushAttrib(GL_LIGHTING_BIT);
	::glDisable(GL_LIGHTING);	
	::glColor3f(.75, .75, .5);
	for ( int i=0; i<nVertex; ++i )
	{
		const CxVertex3d& v = m_ArrayVertex[i];
		CxVector3d normal = v.GetNormal();
		normal *= scale;

		double x = v[0] + normal.x();
		double y = v[1] + normal.y();
		double z = v[2] + normal.z();
	
		::glBegin(GL_LINES);
		  ::glVertex3dv(v);
		  ::glVertex3d(x, y, z);
		::glEnd();	 	
	}

	//::glEnable(GL_LIGHTING);
	glPopAttrib();
}


/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2007-12-12 10:29:15
 * Function name	: CxMesh::DrawNormalFace
 * Description	    : 
 * Return type		: void 
 *================================================================*/
void CxMesh::DrawNormalFace()
{
	if ( !HasNormalFace() )
		CalculateNormalPerFace();

	int nFace = NumFaces();
	const float scale = .15f;
	::glPushAttrib(GL_LIGHTING_BIT);
	::glDisable(GL_LIGHTING);
	::glColor3f(.25, .5, .5);

	for ( int i=0; i<nFace; ++i )
	{
		const CxFace& f = GetFace(i);
		const CxVertex3d& v1 = GetVertex( f[0] );
		const CxVertex3d& v2 = GetVertex( f[1] );
		const CxVertex3d& v3 = GetVertex( f[2] );

		CxVertex3d center( ( v1.x() + v2.x() + v3.x() )/3.0,
			               ( v1.y() + v2.y() + v3.y() )/3.0,
					       ( v1.z() + v2.z() + v3.z() )/3.0);

		CxVector3d normal = f.GetNormal();
		normal *= scale;
		double x = center.x() + normal.x();
		double y = center.y() + normal.y();
		double z = center.z() + normal.z();

		::glBegin(GL_LINES);
		  ::glVertex3dv(center);
		  ::glVertex3d(x, y, z);
		::glEnd();	
	}

//	::glEnable(GL_LIGHTING);
	glPopAttrib();
}

void CxMesh::Update()
{
	SetModifiedFlag(true);
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
}

double CxMesh::Volume()const
{
	double vol = 0.0;
	int nf = NumFaces();
	for(int i=0; i<nf; ++i)
	{
		const CxFace& f = m_ArrayFace.at(i);
		CxVertex3d center(this->Center(f));
		CxVector3d vec(center[0], center[1], center[2]);
		CxVector3d vec1(m_ArrayVertex.at(f[0]), m_ArrayVertex.at(f[1]));
		CxVector3d vec2(m_ArrayVertex.at(f[0]), m_ArrayVertex.at(f[2]));
		vol += vec.Dot(vec1^vec2);
	}

	vol /= 6.0;

	return vol;
}

// A characteristic "feature size" for the mesh.  Computed as an approximation
// to the median edge length
double CxMesh::FeatureSize()
{
	if ( m_ArrayFace.empty())
		return 0.0;

	int nf = (int)m_ArrayFace.size();
	unsigned int nsamp  = nf/2 > 333 ? 333 : nf/2 ;//= std::min( nf/2,333 );



	vector<double> samples;
	samples.reserve(nsamp * 3);

	for (unsigned i = 0; i < nsamp; i++)
	{
		// Quick 'n dirty portable random number generator
		static unsigned randq = 0;
		randq = unsigned(1664525) * randq + unsigned(1013904223);

		int ind = randq % nf;
		const CxFace& rFace = m_ArrayFace[ind];
		const CxVertex3d& p0 = m_ArrayVertex[rFace[0]];
		const CxVertex3d& p1 = m_ArrayVertex[rFace[1]];
		const CxVertex3d& p2 = m_ArrayVertex[rFace[2]];

		samples.push_back(::DistanceSquared(p0, p1)); // p0.dist2(p1));
		samples.push_back(::DistanceSquared(p1, p2)); // p1.dist2(p2));
		samples.push_back(::DistanceSquared(p2, p0)); // p2.dist2(p0));

	}

	nth_element(samples.begin(), samples.begin() + samples.size()/2, samples.end());

	return sqrt(samples[samples.size()/2]);		
}

void CxMesh::SetupBufferObjects()
{
	if(m_bBuffered)
		return;
	unsigned  iFaces = NumFaces();


	if(iFaces == 0)
		return;
	//indices 索引数组
	size_t IndicesArraySize =  iFaces * 3 * sizeof(unsigned int);
	void* indices = malloc(IndicesArraySize);
	void* des = indices;
	for (unsigned i=0; i<iFaces; ++i)
	{
		des = (char*)indices + i * 3 * sizeof(unsigned);
		void* source = m_ArrayFace[i].GetVertexArray();
		memcpy(des,source,3*sizeof(unsigned int));
	}
	unsigned iVertex = NumVertices();
	if(iVertex == 0) return;
	//vertices 顶点数组
	size_t VertexArraySize = iVertex *3* sizeof(double);
	void* vertices = malloc(VertexArraySize);
	des = vertices;
	for (unsigned i = 0 ; i < iVertex ; i++)
	{
		des = (char*)vertices + i * 3 * sizeof(double);
		void* source = m_ArrayVertex[i].GetVertexArray();
		memcpy(des,source,3*sizeof(double));
	}
	//normals 法向量数组
	size_t NormalArraySize = iVertex * 3 * sizeof(double);
	void* normals = malloc(NormalArraySize);
	des = normals;
	for(unsigned i = 0 ; i < iVertex ; i ++)
	{
		des = (char*)normals + i * 3 * sizeof(double);
		void* source = m_ArrayVertex[i].GetNormalRef().GetArrayPtr();
		memcpy(des,source,3*sizeof(double));
	}


	glGenVertexArrays( 1, &m_vao );
	glBindVertexArray(m_vao);


	glGenBuffers(3,m_vbo);
	//绑定vertex数组
	

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,VertexArraySize,(double*)vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );



	//绑定normal数组
	glEnableVertexAttribArray(1);  // Vertex normal
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,NormalArraySize,(double*)normals,GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );


	//绑定Index数组
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,IndicesArraySize,(unsigned int*)indices,GL_STATIC_DRAW);




	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[2]);

	glBindVertexArray(0);

	free(vertices);
	free(indices);
	free(normals);

	m_bBuffered = true;

	//recorder.close();
}

void CxMesh::SetupVBOs( void )
{
	if(m_bBuffered)
		return;
	unsigned  iFaces = NumFaces();


	if(iFaces == 0)
		return;
	//indices 索引数组
	size_t IndicesArraySize =  iFaces * 3 * sizeof(unsigned int);
	void* indices = malloc(IndicesArraySize);
	void* des = indices;
	for (unsigned i=0; i<iFaces; ++i)
	{
		des = (char*)indices + i * 3 * sizeof(unsigned);
		void* source = m_ArrayFace[i].GetVertexArray();
		memcpy(des,source,3*sizeof(unsigned int));
	}
	unsigned iVertex = NumVertices();
	if(iVertex == 0) return;
	//vertices 顶点数组
	size_t VertexArraySize = iVertex *3* sizeof(double);
	void* vertices = malloc(VertexArraySize);
	des = vertices;
	for (unsigned i = 0 ; i < iVertex ; i++)
	{
		des = (char*)vertices + i * 3 * sizeof(double);
		void* source = m_ArrayVertex[i].GetVertexArray();
		memcpy(des,source,3*sizeof(double));
	}
	//normals 法向量数组
	size_t NormalArraySize = iVertex * 3 * sizeof(double);
	void* normals = malloc(NormalArraySize);
	des = normals;
	for(unsigned i = 0 ; i < iVertex ; i ++)
	{
		des = (char*)normals + i * 3 * sizeof(double);
		void* source = m_ArrayVertex[i].GetNormalRef().GetArrayPtr();
		memcpy(des,source,3*sizeof(double));
	}

	glGenBuffers(3,m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,VertexArraySize,(double*)vertices,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,NormalArraySize,(double*)normals,GL_STATIC_DRAW);

	//绑定Index数组
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,IndicesArraySize,(unsigned int*)indices,GL_STATIC_DRAW);

	free(vertices);
	free(indices);
	free(normals);

	m_bBuffered = true;
}



void CxMesh::ReleaseBuffers()
{
	if(m_vao >= 0)
		glDeleteBuffers(1,&m_vao);
	for(int i = 0 ; i < 3 ; i++)
	{
		if(m_vbo[i] >= 0)
			glDeleteBuffers(1,&m_vbo[i]);
	}
}

void CxMesh::render()
{
	SetupBufferObjects();
	unsigned iFaces = NumFaces();
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES,3*iFaces,GL_UNSIGNED_INT,BUFFER_OFFSET(0));
}

void CxMesh::render(float t)
{
	SetupBufferObjects();
	unsigned int iFaces = NumFaces();

	glBindVertexArray(m_vao);
	float speed = 7000.0f;
	m_animateCursor += speed * t;
	unsigned int iDraw = (unsigned int)m_animateCursor;
	if( iDraw > 3 * iFaces)
		iDraw = 3 * iFaces;
	glDrawElements(GL_TRIANGLES, iDraw, GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
}

void CxMesh::render( HGLRC hRC )
{
	SelectRightVAO(hRC);
	unsigned iFaces = NumFaces();
	glDrawElements(GL_TRIANGLES,3*iFaces,GL_UNSIGNED_INT,BUFFER_OFFSET(0));

}

void CxMesh::render( float t,HGLRC hRC )
{
	SelectRightVAO(hRC);
	unsigned iFaces = NumFaces();
	float speed = 7000.0f;
	m_animateCursor += speed * t;
	unsigned int iDraw = (unsigned int)m_animateCursor;
	if( iDraw > 3 * iFaces)
		iDraw = 3 * iFaces;
	glDrawElements(GL_TRIANGLES,iDraw,GL_UNSIGNED_INT,BUFFER_OFFSET(0));
}

void CxMesh::SelectRightVAO( HGLRC hRC )
{
	if( m_contextAttrib[0].hRC == hRC )
	{
		SetupBufferObjectsForThisContext(m_contextAttrib[0]);
		glBindVertexArray(m_contextAttrib[0].hVAO);
	}
	else
	{
		SetupBufferObjectsForThisContext(m_contextAttrib[1]);
		glBindVertexArray(m_contextAttrib[1].hVAO);
	}
}

void CxMesh::SetupBufferObjectsForThisContext( ContextAttrib& refContext )
{
	if( refContext.bSet == true )
		return;

	glGenVertexArrays( 1, &refContext.hVAO );
	glBindVertexArray(refContext.hVAO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );


	//绑定normal数组
	glEnableVertexAttribArray(1);  // Vertex normal
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[1]);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

	//绑定Index数组
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vbo[2]);
	glBindVertexArray(0);
	refContext.bSet = true;
}



bool CxMesh::IsFaceOnRightSide(const CxVertex3d& basePoint, const CxVector3d& baseOrientation, const CxFace& targetFace)
{
	const CxVector3d edge0(basePoint, m_ArrayVertex[targetFace.v1()]);
	const CxVector3d edge1(basePoint, m_ArrayVertex[targetFace.v2()]);
	const CxVector3d edge2(basePoint, m_ArrayVertex[targetFace.v3()]);

	bool bp0 = false;
	bool bp1 = false;
	bool bp2 = false;

	if(edge0.Dot(baseOrientation) > 0.0)
		bp0 = true;
	if(edge1.Dot(baseOrientation) > 0.0)
		bp1 = true;
	if(edge2.Dot(baseOrientation) > 0.0)
		bp2 = true;

	if(bp0 && bp1 && bp2)
		return true;
	else
		return false;
}

bool CxMesh::IsFaceCrossSurface(const CxVertex3d& basePoint, const CxVector3d& baseOrientation, const CxFace& targetFace)
{
	const CxVector3d edge0(basePoint, m_ArrayVertex[targetFace.v1()]);
	const CxVector3d edge1(basePoint, m_ArrayVertex[targetFace.v2()]);
	const CxVector3d edge2(basePoint, m_ArrayVertex[targetFace.v3()]);

	bool bp0 = false;
	bool bp1 = false;
	bool bp2 = false;

	if(edge0.Dot(baseOrientation) > 0.0)    //在目标面
		bp0 = true;
	if(edge1.Dot(baseOrientation) > 0.0)    //在目标面
		bp1 = true;
	if(edge2.Dot(baseOrientation) > 0.0)	//在目标面
		bp2 = true;

	if((bp0||bp1||bp2) && !(bp0&&bp1&&bp2))
		return true;
	else
		return false;
}

bool CxMesh::IsFaceInsideSphere(const CxFace& face, const CxVertex3d& xcener, const double xradius)
{
#if 0
	const CxVertex3d& p0 = m_ArrayVertex[face.v1()];
	const CxVertex3d& p1 = m_ArrayVertex[face.v2()];
	const CxVertex3d& p2 = m_ArrayVertex[face.v3()];

	if( Distance(p0,xcener) <= xradius || Distance(p1,xcener) <= xradius || Distance(p2,xcener) <= xradius)
		return true;
	else
		return false;
#endif

	for ( int i=0; i<3; ++ i )
	{
		const CxVertex3d& v = m_ArrayVertex[ face[i] ];
		double dist = ::DistanceSquared(v, xcener);
		if ( dist <= xradius * xradius )
			return true;
	}
	return false;
}

CxMesh* CxMesh::GenerateSubMesh(const CxVector3d& basePoint, const CxVector3d& baseOrientation, const double xradius)
{
	std::vector<CxFace> faceArray;
	std::vector<CxFace> tmpFaceArray; 
	std::vector<CxFace> tmpFaceArray2;

	const CxVertex3d xcenter(basePoint.x(),basePoint.y(),basePoint.z());
	
	int last = 0;
	int cur = 0;

	const CxVertex3d baseP(basePoint.x(),basePoint.y(),basePoint.z());

	//Step 1: Get all the inter-cross section triangles
	int faceNum = NumFaces();

	for(int i = 0 ; i < faceNum ; i++)
	{
		const CxFace& xface = m_ArrayFace[i];

		if ( IsFaceCrossSurface(baseP, baseOrientation, xface) )
		{
			//m_FaceArray.push_back(xface);
			tmpFaceArray.push_back(xface);
		}
	}
	
	

	//Step 2: Exclude those triangles that are outside the bounding sphere.
	std::vector<bool> checkFace(faceNum, false);
	//for(int i = 0 ; i < faceNum ; i++) checkFace[i] = false;



	for(std::size_t i = 0 ; i < tmpFaceArray.size() ; i++)
	{
		const CxFace& xface = tmpFaceArray[i];
		if ( IsFaceInsideSphere(xface, xcenter, xradius) )
		{
			faceArray.push_back(xface);
			checkFace[xface.GetIndex()] = true;
		}
	}

	// ******************更新 last 为当前数组的元素量
	last = faceArray.size();

	//Step 3: Process the neighbor facets of the triangles from step 1. Exclude those neighbor facets that in the
	//wrong side of the define surface and keep the right ones.

	tmpFaceArray.clear();

	for(std::size_t i = 0 ; i < faceArray.size(); i++)
	{
#if 0
		const CxFace& face0 = m_ArrayFace[faceArray[i].f1()];
		const CxFace& face1 = m_ArrayFace[faceArray[i].f2()];
		const CxFace& face2 = m_ArrayFace[faceArray[i].f3()];
		if( IsFaceOnRightSide(baseP,baseOrientation,face0) && !checkFace[face0.GetIndex()])
		{
			tmpFaceArray.push_back(face0);
			checkFace[face0.GetIndex()] = true;
		}

		if( IsFaceOnRightSide(baseP,baseOrientation,face1) && !checkFace[face1.GetIndex()])
		{
			tmpFaceArray.push_back(face1);
			checkFace[face1.GetIndex()] = true;
		}

		if( IsFaceOnRightSide(baseP,baseOrientation,face2) && !checkFace[face2.GetIndex()])
		{
			tmpFaceArray.push_back(face2);
			checkFace[face2.GetIndex()] = true;
		}
#endif

#if 1
		// 三个邻面;
		for ( int j=0; j<3; ++j )
		{
			const CxFace& face = m_ArrayFace[ faceArray[i].GetFace(j) ];
			if ( checkFace[ face.GetIndex() ] == true )
				continue;
			if ( IsFaceOnRightSide( baseP, baseOrientation, face ) )
			//if( !checkFace[ face.GetIndex()] && IsFaceOnRightSide( baseP, baseOrientation, face ) )
			{
				tmpFaceArray.push_back(face);
				checkFace[face.GetIndex()] = true;
			}
		}
#endif
	}

#if 0
	for( std::size_t i = 0 ; i < tmpFaceArray.size() ; i++)
	{
		faceArray.push_back(tmpFaceArray[i]);
	}
#endif

	// 将查询到面片存入待构建的数组里;
	faceArray.insert(faceArray.end(), tmpFaceArray.begin(), tmpFaceArray.end() );

	// ******************更新 cur 为当前数组的元素量
	cur = faceArray.size();

	int calState = 1;

	while( last != cur )
	{
		last = cur;
		switch(calState)
		{
		case 1:      //tmpFaceArray is SOURCE, tmpFaceArray2 is DESTINATION!
			{
				for( std::size_t i = 0 ; i < tmpFaceArray.size(); i++)
				{
#if 0
					if(tmpFaceArray[i].f1() >= 0)
					{
						CxFace face0 = m_ArrayFace[tmpFaceArray[i].f1()];

						if( !checkFace[face0.GetIndex()])
						{
							tmpFaceArray2.push_back(face0);
							checkFace[face0.GetIndex()] = true;
						}
					}

					if ( tmpFaceArray[i].f2() >= 0 )
					{
						CxFace face1 = m_ArrayFace[tmpFaceArray[i].f2()];

						if( !checkFace[face1.GetIndex()])
						{
							tmpFaceArray2.push_back(face1);
							checkFace[face1.GetIndex()] = true;
						}
					}

					if(tmpFaceArray[i].f3() >= 0)
					{
						CxFace face2 = m_ArrayFace[tmpFaceArray[i].f3()];
						if( !checkFace[face2.GetIndex()])
						{
							tmpFaceArray2.push_back(face2);
							checkFace[face2.GetIndex()] = true;
						}
					}
#endif
#if 1
					// 遍历3个邻面;
					for ( int j=0; j<3; ++j )
					{
						int index = tmpFaceArray[i].GetFace(j);
						if ( index < 0 ) 
							continue;
						const CxFace& face = m_ArrayFace[index];
						if ( !checkFace[ face.GetIndex() ] )
						{
							tmpFaceArray2.push_back(face);
							checkFace[ face.GetIndex() ] = true;
						}
					}
#endif

				}  // end for

				//tmpFaceArray is the next Destination, so It should be cleared.
				tmpFaceArray.clear();

#if 0
				for(int i = 0; i < tmpFaceArray2.size() ; i++)
				{
					faceArray.push_back(tmpFaceArray2[i]);
				}
#endif
				faceArray.insert(faceArray.end(), tmpFaceArray2.begin(), tmpFaceArray2.end() );

				cur = faceArray.size();

				break;
			}
		case -1:	//tmpFaceArray2 is SOURCE, tmpFaceArray is DESTINATION!
			{
				for ( std::size_t i = 0 ; i < tmpFaceArray2.size(); i++)
				{
#if 0
					if(tmpFaceArray2[i].f1() >= 0)
					{
						CxFace face0 = m_ArrayFace[tmpFaceArray2[i].f1()];
						if( !checkFace[face0.GetIndex()])
						{
							tmpFaceArray.push_back(face0);
							checkFace[face0.GetIndex()] = true;
						}
					}

					if(tmpFaceArray2[i].f2() >=0)
					{
						CxFace face1 = m_ArrayFace[tmpFaceArray2[i].f2()];
						if( !checkFace[face1.GetIndex()])
						{
							tmpFaceArray.push_back(face1);
							checkFace[face1.GetIndex()] = true;
						}
					}

					if(tmpFaceArray2[i].f3() >= 0)
					{
						CxFace face2 = m_ArrayFace[tmpFaceArray2[i].f3()];
						if(!checkFace[face2.GetIndex()])
						{
							tmpFaceArray.push_back(face2);
							checkFace[face2.GetIndex()] = true;
						}
					}
#endif

#if 1
					for ( int j=0; j<3; ++j )
					{
						int index = tmpFaceArray2[i].GetFace(j);
						if ( index < 0 )
							continue;

						const CxFace& face = m_ArrayFace[index];
						if ( !checkFace[ face.GetIndex() ] )
						{
							tmpFaceArray.push_back(face);
							checkFace[ face.GetIndex() ] = true;
						}
					}
#endif

				}

				tmpFaceArray2.clear();

#if 0
				for(int i = 0 ; i < tmpFaceArray.size(); i++)
				{
					faceArray.push_back(tmpFaceArray[i]);
				}
#endif
				faceArray.insert(faceArray.end(), tmpFaceArray.begin(), tmpFaceArray.end() );

				cur = faceArray.size();

				break;	
			}
		default:
			break;
		}

		calState = -calState;

	} // end while
	

	//Create a new CxMesh Instance.
	CxMesh* pMesh = new CxMesh;

	int meshNumFace = faceArray.size();
	pMesh->Reserve(3 * meshNumFace, meshNumFace);


	//Dr. Wu's xFileIO code, read BIN_STL_FILE
	typedef std::set<CxVertex3d, std::less<CxVertex3d> >  VertexSet;
	VertexSet tmpVertexSet;      // 储存模型的实际顶点数;
	std::pair<VertexSet::iterator, bool> pair_set;

	int nIndex = 0;
	int nCnt = 0;

	for( int i = 0 ; i < meshNumFace ; i++)
	{

		//Calculate the normal for the face first!
		CxVertex3d vert[3] = {m_ArrayVertex[faceArray[i].v1()],
			                  m_ArrayVertex[faceArray[i].v2()],
			                  m_ArrayVertex[faceArray[i].v3()]}; 
#if 0
		CxVector3d edge0(vert[0],vert[1]);
		CxVector3d edge1(vert[1],vert[2]);
		CxVector3d Normalx = edge0.Cross(edge1);
#endif
		const CxVector3d& Normalx = faceArray[i].GetNormal();
		CxFace xface;
		xface.SetNormal(Normalx);
		xface.SetIndex(i);

		for(int j = 0 ; j < 3 ; j++)
		{
			CxVertex3d v(vert[j]);
			pair_set = tmpVertexSet.insert(v);
			if( pair_set.second == true )
			{
				pMesh->AddVertex(CxVertex3d(vert[j].x(),vert[j].y(),vert[j].z(),nIndex));
				CxVertex3d& cv = const_cast<CxVertex3d&>(*pair_set.first);
				cv.SetIndex(nIndex++);
			}
			VertexSet::iterator it = tmpVertexSet.find(v);
			if(it != tmpVertexSet.end())
				xface[j] = (*it).GetIndex();
		}

		pMesh->AddFace(xface);
	}




#if 0
	for( int i = 0 ; i < meshNumFace ; i++)
	{
		//Calculate the normal for the face first!
		CxVertex3d vert0 = m_ArrayVertex[faceArray[i].v1()];
		vert0.SetIndex(3*i);
		CxVertex3d vert1 = m_ArrayVertex[faceArray[i].v2()];
		vert1.SetIndex(3*i +1);
		CxVertex3d vert2 = m_ArrayVertex[faceArray[i].v3()];
		vert2.SetIndex(3*i +2);
		CxVector3d edge0(vert0,vert1);
		CxVector3d edge1(vert1,vert2);
		CxVector3d Normalx = edge0.Cross(edge1);
		CxFace xface;
		xface.SetNormal(Normalx);
		xface.SetIndex(i);
		xface.Set(3*i,3*i+1,3*i+2);
		pMesh->AddFace(xface);
		pMesh->AddVertex(vert0);
		pMesh->AddVertex(vert1);
		pMesh->AddVertex(vert2);
	}
#endif

	pMesh->SetHasNormalFace(true);
	pMesh->SetHasNormalVertex(false);
	pMesh->CreateAdjacency();
	pMesh->CalculateNormalPerFace();
	pMesh->CalculateNormalPerVertex();
	pMesh->CalculateBoundingBox();
	pMesh->SetModifiedFlag(true);

	FillHoles(pMesh);

	//CxFileIO::SaveFileAs("AngiPart.stl",pMesh);
	return pMesh;
}


CxMesh* CxMesh::GenerateSubMesh(const CxVector3d& basePoint, const CxVector3d& baseOrientation, double xradius)const
{
	CxMeshKdTree meshKdTree(this);
	meshKdTree.build();

	// 得到与 base方向垂直的向量;
	const CxVector3d orth = CxVector3d( -baseOrientation[2], 0, -baseOrientation[0]);
	assert( orth.Dot(baseOrientation) == 0 );
	const CxRay ray(basePoint, orth);

	std::vector<std::pair<int, double> > indexTimeArray;
	if ( !meshKdTree.hit(ray, indexTimeArray) )
		return NULL;

	// 至少有两个交点;注意，这里时间会有负数,表示反方向相交
	std::size_t size = indexTimeArray.size();
	assert ( size >= 2 );
	
	class CompTime
	{
	public:
		bool operator()(std::pair<int, double>& p1, std::pair<int, double>& p2 )const
		{ return p1.second < p2.second;}
	};

	// 按时间排序;小到到大, 
	std::sort(indexTimeArray.begin(), indexTimeArray.end(), CompTime() );

	// 通常来讲，相交时间最短的两个面片属于同一段血管;
	int index0 = indexTimeArray[0].first;
	int index1 = indexTimeArray[1].first;

	return NULL;
}

// The pMesh to be filled must have one and only one hole!!!!!!!!
void CxMesh::FillHoles(CxMesh* pMesh)
{
	std::vector<CxEdge> EArray;
	std::vector<CxVertex3d> VArray;

	//If there is no edge information, return
	if ( pMesh->NumEdges() == 0 )
		return;

	int iEdges = pMesh->NumEdges();

	for ( int i=0; i<iEdges; ++i )
	{
		CxEdge rEdge = pMesh->GetEdge(i);
		if ( rEdge.IsOnBoundary() == true )
		{
			EArray.push_back(rEdge);
		}
	}


	// TODO: Add your command handler code here
	int iVertices = pMesh->NumVertices();
	if ( iVertices <= 0 )
		return;

	for ( int i=0; i<iVertices; ++i )
	{
		CxVertex3d rVertex = pMesh->m_ArrayVertex[i];
		if ( pMesh->IsVertexOnBoundary(rVertex) == true )	
		{
			VArray.push_back(rVertex);
		}
	}

	CxVertex3d holeCenter;
	holeCenter.Clear();
	for(size_t i = 0 ; i < VArray.size() ; i++)
	{
		holeCenter = holeCenter + VArray[i];
	}
	holeCenter = holeCenter / VArray.size();


	int iFaces = pMesh->NumFaces();

	int toAddNumFace = EArray.size();

	for( int i = 0 ; i < toAddNumFace ; i++)
	{
		CxVertex3d p0 = holeCenter;
		CxVertex3d p1 = pMesh->GetArrayVertex()[EArray[i].v2()];
		CxVertex3d p2 = pMesh->GetArrayVertex()[EArray[i].v1()];
		CxVector3d edge0(p0,p1);
		CxVector3d edge1(p1,p2);
		CxVector3d Normalx = edge0.Cross(edge1);
		Normalx.Normalize();
		CxFace face;
		face.SetNormal(Normalx);
		face.SetIndex(iFaces++);
		face.Set(iVertices,iVertices+1,iVertices+2); 
		pMesh->AddFace(face);
		p0.SetIndex(iVertices++);
		p1.SetIndex(iVertices++);
		p2.SetIndex(iVertices++);
		pMesh->AddVertex(p0);
		pMesh->AddVertex(p1);
		pMesh->AddVertex(p2);
	}
}

void CxMesh::PreGenBuffers()
{
	glGenVertexArrays( 1, &m_vao );
	glGenBuffers(3,m_vbo);
	//绑定vertex数组
}

void CxMesh::PreAllocBufferSpace(int vertexNum, int faceNum)
{
	glBindVertexArray(m_vao);
	
	//Allocate space for Vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,vertexNum * 3 * sizeof(double),0,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

	//Allocate space for Normals
	glEnableVertexAttribArray(1);  // Vertex normal
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,vertexNum * 3 * sizeof(double),0,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );


	//绑定Index数组
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,faceNum * 3 * sizeof(unsigned int),0,GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

void CxMesh::ExchangeDataWithGpuBuffers()
{
	unsigned int iFaces = NumFaces();

	if(iFaces == 0)
		return;

	printf("The NumFace is %d\n",iFaces);

	//indices 索引数组
	size_t IndicesArraySize =  iFaces * 3 * sizeof(unsigned int);

	printf("Indices Array Size is %d \n", IndicesArraySize);
	void* indices = malloc(IndicesArraySize);
	void* des = indices;
	for (unsigned i=0; i<iFaces; ++i)
	{
		des = (char*)indices + i * 3 * sizeof(unsigned);
		void* source = m_ArrayFace[i].GetVertexArray();
		memcpy(des,source,3*sizeof(unsigned int));
	}
	unsigned iVertex = NumVertices();
	if(iVertex == 0) return;
	//vertices 顶点数组
	size_t VertexArraySize = iVertex *3* sizeof(double);
	void* vertices = malloc(VertexArraySize);
	des = vertices;
	for (unsigned i = 0 ; i < iVertex ; i++)
	{
		des = (char*)vertices + i * 3 * sizeof(double);
		void* source = m_ArrayVertex[i].GetVertexArray();
		memcpy(des,source,3*sizeof(double));
	}
	//normals 法向量数组
	size_t NormalArraySize = iVertex * 3 * sizeof(double);
	void* normals = malloc(NormalArraySize);
	des = normals;
	for(unsigned i = 0 ; i < iVertex ; i ++)
	{
		des = (char*)normals + i * 3 * sizeof(double);
		void* source = m_ArrayVertex[i].GetNormalRef().GetArrayPtr();
		memcpy(des,source,3*sizeof(double));
	}
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER,0,VertexArraySize,vertices);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER,0,NormalArraySize,normals);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vbo[2]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, IndicesArraySize,indices);
	glBindVertexArray(0);
	free(vertices);
	free(normals);
	free(indices);
}

void CxMesh::renderForCube()
{
	unsigned iFaces = NumFaces();
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES,3*iFaces,GL_UNSIGNED_INT,BUFFER_OFFSET(0));
}

void CxMesh::PrintLog()
{
	printf("......This is the Information for CxMesh:\n");
	printf("The VAO handle : %d\n",m_vao);
	printf("The VBO handle : %d, %d,%d\n",m_vbo[0],m_vbo[1],m_vbo[2]);
	printf("The NumVertices: %d\n",m_ArrayVertex.size());
	printf("The NumFaces: %d\n",m_ArrayFace.size());
}

float CxMesh::GetZRange()
{
	return m_BoundingBox.zRange();
}
