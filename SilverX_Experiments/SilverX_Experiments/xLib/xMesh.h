// xMesh.h: interface for the CxMesh class.
// ***************************************************************
//  xMesh   version:  1.0   
//  -------------------------------------------------------------
//  Description:  非流形网格类，由三角面片构成;
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
#if !defined _XMESH_H__INCLUDED_
#define _XMESH_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma  warning(disable:4786)
#endif // _MSC_VER > 1000

#include <vector>
using std::vector;
#include <cstring>
using std::string;
#include <gl/glew.h>

#include "xVertex3d.h"
#include "xFace.h"
#include "xBoundingBox.h"
#include "xEdge.h"
#include "xColor.h"

#include "xMatrix44.h"

//#include <glm/glm.hpp>

class WxMaterial;
class GLMat4;

enum MeshType { MT_SKELETON = 0,
	MT_VASCULATURE};

typedef std::vector<CxVertex3d>  VertexArray;
typedef std::vector<CxFace>      FaceArray;
typedef std::vector<CxEdge>      EdgeArray;
typedef int                      FaceId;

enum {NOT_VERTEX_AND_FACE, COLOR_PER_VERTEX, COLOR_PER_FACE };
enum {CUR_GAUSS, CUR_MEAN, CUR_TOTAL, CUR_RMS};
enum {COMMON,VORONOI, DESBRUN, TAUBIN, RUSI};
enum {HIGHLIGHT_VERTEX = 3};

#define BUFFERNUM 3 //vertex indice normal, no color
#define BUFFER_OFFSET(bytes) ((byte*)NULL +  (bytes))

class CTexture;

class CxMesh
{

	struct ContextAttrib
	{
		GLuint hVAO;
		HGLRC  hRC;
		bool   bSet;
		ContextAttrib():hVAO(0),hRC(NULL),bSet(false) {}
	};

private:
	
	MeshType m_eMeshType;
	vector<CxVertex3d>       m_ArrayVertex;   // 储存三角网格的顶点;
	vector<CxFace>           m_ArrayFace;     // 储存三角面片;
	
	CxBoundingBox            m_BoundingBox;   // 模型的最小包围盒;
	
	vector<CxEdge>           m_ArrayEdge;     // 储存三角网格的边;

	bool                     m_bHasNormalFace;   // 是否已拥有面法向量;
	bool                     m_bHasNormalVertex; // 是否已拥有顶点法向量;
	bool					 m_bHasTextureCoords;
	bool                     m_bModifiedFlag;

	bool					 m_bBuffered;

	unsigned int             m_vao;
	unsigned int			 m_vbo[3];

	float					 m_animateCursor; //该变量的作用是控制画多少面片，用作造影剂渲染时使用。随时间变化。
	
	ContextAttrib			 m_contextAttrib[2];

	bool                      m_bHasBoundingBox; // 包围盒是否已计算过的标识;
	string                    m_strMeshName;
	CxColor                   m_Color;
	bool                      m_bVisible;       // 该模型是否可视;
	bool                      m_bTranspancey;  


	int                       m_iColorBinding;  // 0 for no color, 1 for vertex, 2 for face;

	friend class CxCurvature;
	friend class CxTransformer;

public:
	
	CxMesh();
	virtual ~CxMesh();
	void Free();
	void SetupBufferObjects(void);
	void ReleaseBuffers(void);
	void render(void);
	void render(float t);
	void render(float t,HGLRC hRC);
	void render(HGLRC hRC);

	float GetZRange();

	void SetupVBOs(void);

	void ResetAnimate() { m_animateCursor = 1.0f; }

	void SelectRightVAO(HGLRC hRC);

	void SetupBufferObjectsForThisContext( ContextAttrib& refContext);


	void SetPrimaryRC(HGLRC hRC) { m_contextAttrib[0].hRC = hRC ; }
	void SetSecondaryRC(HGLRC hRC) { m_contextAttrib[1].hRC = hRC; }


	//render state operation


	//Very very messy temporary methods just for the real-time updated tube vertices and rendering. Hate it!
	//The following two function should called just once for every CxMesh instance for rendering tube.
	void PreGenBuffers();
	void PreAllocBufferSpace(int vertexNum, int faceNum);
	
	//This funciton should called every time the rube has been regenerated. Because the vertices, normals and indices
	//all have changed.
	void ExchangeDataWithGpuBuffers(void);
	void renderForCube(void);

	// data setting;
	void Reserve(int nverts, int nfaces){ m_ArrayVertex.reserve(nverts); 
	                                      m_ArrayFace.reserve(nfaces);  }
	void ReserveArrayVertex(int nverts) { m_ArrayVertex.reserve(nverts);}
	void ReserveArrayFace(int nfaces)   { m_ArrayFace.reserve(nfaces);  }
	void ResizeArrayVertex(int nVerts)  { m_ArrayVertex.resize(nVerts); }
	void ResizeArrayFace(int nFaces)    { m_ArrayFace.resize(nFaces);   }
	void AddVertex(const CxVertex3d& v) { m_ArrayVertex.push_back(v);   }
	void AddFace(const CxFace& f)       { m_ArrayFace.push_back(f);     }
	

	bool HasNormalFace()   { return m_bHasNormalFace;   }
	bool HasNormalVertex() { return m_bHasNormalVertex; }
	void SetHasNormalFace(bool bHad)  { m_bHasNormalFace = bHad;  }
	void SetHasNormalVertex(bool bHad){ m_bHasNormalVertex = bHad;}
	bool HasBoundingBox()  { return m_bHasBoundingBox;  }
	void SetHasBoundingBox(bool bHad) { m_bHasBoundingBox = bHad; }

	void SetModifiedFlag(bool bFlag) { m_bModifiedFlag = bFlag; }
	bool GetModifiedFlag()const      { return m_bModifiedFlag;  }

	string GetMeshName()const { return m_strMeshName; }
	void SetMeshName(const string& strName) { m_strMeshName = strName;}

	const CxColor* GetColor()const { return &m_Color; }
	void SetColor(const CxColor& rColor) { m_Color.Set(rColor);}
	void SetColor(unsigned char r, unsigned char g, unsigned char b) { m_Color.Set(r,g,b);}
	void SetColor(float r, float g, float b) { m_Color.Set(r,g,b);}

	void SetColorBinding(int i) { m_iColorBinding = i; SetModifiedFlag(true);} // 0 for none;

	bool IsVisible()const { return ( m_bVisible == true );}
	void SetVisible(bool bVisible) { m_bVisible = bVisible;}
	bool& SetVisible() { return m_bVisible;}

	void SetTranspaency(bool bTranspancy) { m_bTranspancey = bTranspancy;}

	// data accessing;
	// note: index must be in a valid range;  don not use assert here;
	const CxVertex3d& GetVertex(int index)const { return m_ArrayVertex[index]; }
	      CxVertex3d& GetVertex(int index)      { return m_ArrayVertex[index]; }
	const CxFace&     GetFace(int index)  const { return m_ArrayFace[index];   }
	      CxFace&     GetFace(int index)        { return m_ArrayFace[index];   }
	const CxEdge&     GetEdge(int index)  const { return m_ArrayEdge.at(index);}
	      CxEdge&     GetEdge(int index)        { return m_ArrayEdge.at(index);}

	unsigned int NumVertices()const  { return (unsigned int)m_ArrayVertex.size();}
	unsigned int NumFaces()   const  { return (unsigned int)m_ArrayFace.size();  }
	unsigned int NumEdges()   const  { return (unsigned int)m_ArrayEdge.size();  }
	void SetBoundingBox(const CxVertex3d& rMax, const CxVertex3d& rMin) { m_BoundingBox.Set(rMax, rMin);}
	const CxBoundingBox* GetBoundingBox()const    { return &m_BoundingBox;     }
	const vector<CxFace>& GetArrayFace()const     { return m_ArrayFace;        }
	const vector<CxVertex3d>& GetArrayVertex()const{ return m_ArrayVertex;     }
// 	const vector<CxFace>& GetArrayAcrossEdge()const{ return m_ArrayAcrossEdge; }
 	const vector<CxEdge>& GetArrayEdge()const { return m_ArrayEdge; }
	// open the array to public, it is dangerous to hacker ?!
 	//    vector<CxFace>& GetArrayAcrossEdge()     { return m_ArrayAcrossEdge; }
 	//	  vector<CxFace>& GetArrayFace()           { return m_ArrayFace;        }
 	//	  vector<CxVertex3d>& GetArrayVertex()     { return m_ArrayVertex;     }

	void Copy(CxMesh* pMesh);
	void CreateCopy();

    // data processing;
	void SetFlagOnFaces(char flag);
	void CalculateBoundingBox();
	void DeleteRedundancyVertex();
	void CalculateNormalPerFace();
	void CalculateNormalPerVertex();
	// 不同的方法计算顶点的法向量;
	void CalculateNormalPerVertexThurmer();
	void CalculateNormalPerVertexMax();

	// topology infos;
	void CreateAdjacency();
    void BuildFaceNeighbor();
	void BuildVertexNeighbor();

	// Misc..., property, 
	FaceId GetFaceNeighborInclusive(FaceId f, int v1, int v2);
	CxVertex3d Center(const CxFace& f)const; // get the center of face i;
	double Area(const CxFace& f)const;
	double Perimeter(const CxFace& f)const;
	double Compacity(const CxFace& f)const;
	double Volume()const;
	double FeatureSize();

	double CalculateAreaFace(const CxFace& rFace);

	void   CreateEdgeArray();
	int    GenerateOrientationBoundaryEdge(EdgeArray& rArrayBoundaryEdge);
	CxEdge RetrivelEdgeStartedVertex(EdgeArray& rArrayEdge, int vertexStarted,
		                               vector<int>& flagVec);

	
	double GetAverageLengthEdge(); // 获得模型边的平均长度;(所有边长之和/边数)
	int  FindNeighborFaceContainVertex(const int v1, const int v2, vector<CxFace>& rArrayFace)const;
	int  FindNeighborFaceContainVertex(const CxVertex3d& rV1, const CxVertex3d& rV2, 
		                               vector<CxFace>& rArrayFace)const;
	bool IsVertexOnBoundary(const CxVertex3d& rVertex);

	void ScaleMesh(double scale = 1.0);

	bool IsFaceOnRightSide(const CxVertex3d& basePoint, const CxVector3d& baseOrientation, const CxFace& targetFace);
	bool IsFaceCrossSurface(const CxVertex3d& basePoint, const CxVector3d& baseOrientation, const CxFace& targetFace);
	bool IsFaceInsideSphere(const CxFace& face,const CxVertex3d& xcener,const double xradius);
	CxMesh* GenerateSubMesh(const CxVector3d& basePoint, const CxVector3d& baseOrientation, const double xradius);
	CxMesh* GenerateSubMesh(const CxVector3d& basePoint, const CxVector3d& baseOrientation, double xradius)const;

	double EstimateRadius(const CxVector3d& basePoint, const CxVector3d& baseOrientation)const;
	
	void FillHoles(CxMesh* pMesh);


	void DrawBoundingBox()const;
	void DrawVertexHighlight(double radius =0.05)const;

	void DrawBoundaryEdge();
	void DrawBoundaryVertex();
	void DrawBoundaryFace();

	void DrawNormalVertex();
	void DrawNormalFace();

	void Update();

	void PrintLog(void);
	void SetMeshType(MeshType mt) { m_eMeshType = mt; }
	MeshType GetMeshType(void) { return m_eMeshType; }

	// debug;
	void Trace();

}; // end for CxMesh;




#endif // !defined(_XMESH_H__INCLUDED_)
