/**************************************************************************
	
	File name:	xMeshRepair.cpp      Version 1.0
---------------------------------------------------------------------------
	Purpose :	
    Author  :   Dr.Wu (jh.wu@siat.ac.cn)
	Created :	2014/08/01   16:02
    Modified:
---------------------------------------------------------------------------
   Copyright (c) 2014 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences
**************************************************************************/
#include "StdAfx.h"
#include "xMeshRepair.h"


CxMeshRepair::~CxMeshRepair(void)
{
	std::size_t size = m_BoundaryEdges.size();
	if ( size == 0 ) return;

	for ( std::size_t k=0; k<size; ++k )
	{
		EdgeArray edgeArray = m_BoundaryEdges[k];
		edgeArray.clear();
	}

	m_BoundaryEdges.clear();

}

int CxMeshRepair::generateBoundaryEdges()
{
	if ( m_pMesh->GetArrayEdge().empty() )
		m_pMesh->CreateEdgeArray();

	EdgeArray boundaryEdges; // 存储所有边界边;
	std::size_t size = m_pMesh->GetArrayEdge().size();
	for (std::size_t i=0; i<size; ++i )
	{
		const CxEdge& rEdge = m_pMesh->GetArrayEdge()[i];
		if ( rEdge.IsOnBoundary() )
		{
			boundaryEdges.push_back(rEdge);
			//std::cout<< rEdge.v1()<<"\t"<<rEdge.v2()<<"\n";
		}
	}

	// 从边界边分类，闭环的边界为一个;
	std::size_t edges = boundaryEdges.size();
	if ( edges == 0 ) return 0; // 没有边界边,模型是封闭的;

	std::vector<bool> tag(edges, false ); // 用于标识边;
	for ( std::size_t i=0; i < edges; ++i )
	{
		if ( tag[i] == true ) 
			continue;
		// edge:v1----->v2
		const CxEdge& edge = boundaryEdges[ i ];  // 种子边;
		EdgeArray edgeLoop;
		edgeLoop.push_back( edge );
		tag[i] = true;

		// 如果另一个索引等于种子边的第一个索引, 说明闭环了;
		while ( edge.v1() != edgeLoop.back().v2() ) 
		{
			int v2 = edgeLoop.back().v2();

			// 遍历边数组，查找边相连的边, 类似于区域增长;
			for ( std::size_t j=0; j<edges; ++j ) 
			{
				if ( tag[j] == true )
					continue;
				const CxEdge& nextEdge = boundaryEdges[j];
				int v1 = nextEdge.GetVertex( v2 ); // 得到边的另一个端点的索引,也就是v1;
				if ( nextEdge.HasVertex( v2 ))
				{
					edgeLoop.push_back( CxEdge(v2, v1) );
					tag[j] = true;
					break;
				}
			} // end for
		} // end while

		m_BoundaryEdges.push_back(edgeLoop);
	} // end for

	int nloop = (int)m_BoundaryEdges.size();
	return nloop;
}

void CxMeshRepair::fillingHoles()
{

	std::size_t size = m_BoundaryEdges.size();
	if ( size == 0 )
		generateBoundaryEdges();
	size = m_BoundaryEdges.size();
	if ( size == 0 ) return;

	for ( std::size_t k=0; k<size; ++k )
	{
		int nv = m_pMesh->NumVertices();
		int nf = m_pMesh->NumFaces();

		EdgeArray edgeArray = m_BoundaryEdges[k];
		std::size_t edges = edgeArray.size();

		// compute the average center of boundary vertices;
		CxVertex3d center(0, 0, 0);
		center.SetIndex( nv );
		for ( std::size_t i=0; i<edges; ++i )
		{
			const CxEdge& edge = edgeArray[i];
			CxVertex3d& v = m_pMesh->GetVertex( edge.v1() );
			center += v;
		}

		center /= edges;

		// put the newly born vertex to the original mesh;
		m_pMesh->AddVertex( center );

		// construct new triangles between the center point and the boundary vertices;
		for ( std::size_t i=0; i<edges; ++i )
		{
			const CxEdge& edge = edgeArray[i];
			int v1 = edge.v1();
			int v2 = edge.v2();
			int v3 = center.GetIndex();

			//const CxVector3d n1(center, GetVertex(v2));
			//const CxVector3d n2(center, GetVertex(v1));
			//CxVector3d n = n1.Cross(n2); n.Normalize();

			CxFace newFace(v3, v2, v1); // counter-clock wise
			newFace.SetIndex( nf++ );
			//newFace.SetNormal(n);
			m_pMesh->AddFace( newFace );
		} // end for i
	} // end for k


	// rebuild
#if 1
	m_pMesh->CreateAdjacency();
	m_pMesh->CalculateNormalPerFace();
	m_pMesh->CalculateNormalPerVertex();
	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
#endif
	m_pMesh->SetModifiedFlag(true);
	//this->m_ArrayEdge.clear();
}