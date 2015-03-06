/**************************************************************************
	
	File name:	xMeshKdTree.cpp      Version 1.0
---------------------------------------------------------------------------
	Purpose :	
    Author  :   Dr.Wu (jh.wu@siat.ac.cn)
	Created :	2014/07/23   12:15
    Modified:
---------------------------------------------------------------------------
   Copyright (c) 2014 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences
**************************************************************************/
#include "StdAfx.h"
#include "xMeshKdTree.h"


#include <iostream>
#include <functional>   // for greater<>


#include "xMesh.h"




CxMeshKdTree::CxMeshKdTree(const CxMesh* pMesh)
{
	m_pMesh = pMesh;
	mMaxDepth = 10;
}

CxMeshKdTree::~CxMeshKdTree(void)
{
	destroy(mKdNodeRoot);
}

void CxMeshKdTree::destroy(CxMeshKdTree::KdNode* pNode )
{
	if ( pNode == NULL ) 
		return;
	
	destroy(pNode->mLeft);
	destroy(pNode->mRight);
	delete pNode;
	
}

bool CxMeshKdTree::build(int maxDepths /*= 10*/)
{
	if ( m_pMesh == NULL ) return false;
    this->mMaxDepth = maxDepths;
	calculateBoundingBox();
  
	int nfaces = m_pMesh->NumFaces();
	std::vector<int> tris( nfaces );

	// 初始化面片数组, 记录每个面片的索引号;
	for ( int i=0; i<nfaces; ++i )
	{
		tris[i] = i;
	}

	mKdNodeRoot = build( tris, 0 );

#if 0
	std::vector<FaceIndexArray> faceIndexArray2d;
	int leaf = countLeaf(mKdNodeRoot, faceIndexArray2d );
	for (int i=0; i<leaf; ++i)
	{
		FaceIndexArray indexs = faceIndexArray2d[i];
		std::cout<< indexs.size()<<"\n";
	}
#endif

	return ( mKdNodeRoot != NULL ) ? true : false;
}


CxMeshKdTree::KdNode* CxMeshKdTree::build(std::vector<int>& rTriangles, int nDepth)
{
	std::size_t  nfaces = rTriangles.size();
	if ( nfaces == 0 )
		return NULL;

	// calculate bounding box and split axis;
	CxBoundingBox box;
	int splitAxis = this->calculateBoundingBox(rTriangles, box );

	if ( nfaces == 1 )
	{
		KdNode* pNode = new KdNode();
		pNode->mBoundingBox.Set(box);
		pNode->mTriangleArray = rTriangles;
		pNode->mLeft = NULL;
		pNode->mRight = NULL;
		return pNode;
	}

	// compute the center point
	const CxVertex3d midpt = calculateCenter( rTriangles );


	std::vector<int> leftTris;
	std::vector<int> rightTris;

	for ( int i = 0; i < (int)nfaces; ++i )
	{
		const int& id = rTriangles[i];
		const CxFace& f = m_pMesh->GetFace( id );
		const CxVertex3d center = m_pMesh->Center(f);
		switch ( splitAxis )
		{
		case 0: 
			{
				( midpt.x() >= center.x() ) ? rightTris.push_back(id) : leftTris.push_back(id);
				break;
			}
		case 1:
			{
				( midpt.y() >= center.y() ) ? rightTris.push_back(id) : leftTris.push_back(id);
				break;
			}
		case 2:
			{
				( midpt.z() >= center.z() ) ? rightTris.push_back(id) : leftTris.push_back(id);
				break;
			}
		}  // end for switch
	}  // for

	KdNode* pNode = new KdNode();
	pNode->mBoundingBox.Set(box);
	pNode->mTriangleArray = rTriangles;

	if ( nDepth < mMaxDepth )  // 分割10层停止;
//	if ( (leftTris.size() > 8) && (rightTris.size() > 8) )
	{
		// recursively split the tree;
		pNode->mLeft  = build(leftTris,  nDepth + 1);
		pNode->mRight = build(rightTris, nDepth + 1);
	}
#if 1
	else
	{
		// stop splitting;
		pNode->mLeft = NULL;
		pNode->mRight = NULL;
	}
#endif

	return pNode;
}

void CxMeshKdTree::calculateBoundingBox()
{
	int nface = m_pMesh->NumFaces();
	if ( nface == 0 ) return;

	mAABBs.resize( nface );

	for ( int i = 0; i < nface; ++i )
	{
		const CxFace& f = m_pMesh->GetFace(i);
		const CxVertex3d& v0 = m_pMesh->GetVertex(f[0]);
		const CxVertex3d& v1 = m_pMesh->GetVertex(f[1]);
		const CxVertex3d& v2 = m_pMesh->GetVertex(f[2]);

		double xmin  = ::Min(v0[0], v1[0], v2[0] );
		double ymin  = ::Min(v0[1], v1[1], v2[1] );
		double zmin  = ::Min(v0[2], v1[2], v2[2] );

		double xmax  = ::Max(v0[0], v1[0], v2[0] );
		double ymax  = ::Max(v0[1], v1[1], v2[1] );
		double zmax  = ::Max(v0[2], v1[2], v2[2] );

		mAABBs[i] = CxBoundingBox(CxVertex3d(xmax, ymax, zmax),
			                      CxVertex3d(xmin, ymin, zmin));
	}
}

// 返回值, -1 : 无效, 0: x 轴, 1:  y轴, 2: z轴.
int CxMeshKdTree::calculateBoundingBox(const FaceIndexArray& faces, CxBoundingBox& box)const
{
	std::size_t nface = faces.size();
	if ( nface == 0 ) return -1;

#if 0
	CxVertex3d minVertex, maxVertex;

	const CxFace& f = m_pMesh->GetFace(faces[0]);
	const CxVertex3d& v0 = m_pMesh->GetVertex(f[0]);
	const CxVertex3d& v1 = m_pMesh->GetVertex(f[1]);
	const CxVertex3d& v2 = m_pMesh->GetVertex(f[2]);

	minVertex[0] = ::Min(v0[0], v1[0], v2[0] );
	minVertex[1] = ::Min(v0[1], v1[1], v2[1] );
	minVertex[2] = ::Min(v0[2], v1[2], v2[2] );

	maxVertex[0] = ::Max(v0[0], v1[0], v2[0] );
	maxVertex[1] = ::Max(v0[1], v1[1], v2[1] );
	maxVertex[2] = ::Max(v0[2], v1[2], v2[2] );

	for ( int i = 1; i < (int)nface; ++i )
	{
		const int& id = faces[i];
		const CxFace& f = m_pMesh->GetFace(id);
		const CxVertex3d& v0 = m_pMesh->GetVertex(f[0]);
		const CxVertex3d& v1 = m_pMesh->GetVertex(f[1]);
		const CxVertex3d& v2 = m_pMesh->GetVertex(f[2]);

		double xmin  = ::Min(v0[0], v1[0], v2[0] );
		double ymin  = ::Min(v0[1], v1[1], v2[1] );
		double zmin  = ::Min(v0[2], v1[2], v2[2] );

		double xmax  = ::Max(v0[0], v1[0], v2[0] );
		double ymax  = ::Max(v0[1], v1[1], v2[1] );
		double zmax  = ::Max(v0[2], v1[2], v2[2] );

		minVertex[0] = ::Min( xmin, minVertex[0] );
		minVertex[1] = ::Min( ymin, minVertex[1] );
		minVertex[2] = ::Min( zmin, minVertex[2] );

		maxVertex[0] = ::Max( xmax, maxVertex[0] );
		maxVertex[1] = ::Max( ymax, maxVertex[1] );
		maxVertex[2] = ::Max( zmax, maxVertex[2] );


	}


	box.Set(maxVertex, minVertex);
#endif

#if 1
	// 为了避免每次都如上代码从新计算已计算过的面片;
	// 先计算所有面片的包围盒, 然后需要时调用;
    // 这种方法是否速度快点？
	box.Set( mAABBs[faces[0]] );
	for ( std::size_t i = 1; i < nface; ++i )
	{
		box.Expand( mAABBs[ faces[i] ] );
	}
#endif
	// Find longest axis
	double xrange, yrange, zrange;
	box.Range(xrange, yrange, zrange );

	int splitAxis = 0;
	if ( zrange > yrange )
	{
		if ( zrange > xrange ) 
			splitAxis = 2;
	}
	else
	{
		if ( yrange > xrange ) 
			splitAxis = 1;
	}

	return splitAxis;

}

const CxVertex3d CxMeshKdTree::calculateCenter(const FaceIndexArray& faces)const
{
	std::size_t nface = faces.size();
	CxVertex3d center(0.0, 0.0, 0.0);
	for ( int i=0; i<(int)nface; ++i )
	{
		const CxFace& f = m_pMesh->GetFace( faces[i] );
		center += m_pMesh->Center( f );
	}
	return center/(double)nface;
}

int CxMeshKdTree::countLeaf( KdNode* pRoot, std::vector<FaceIndexArray>& faceIndexArray2d )const
{
	if ( pRoot == NULL ) 
		return 0;
	else if ( ( pRoot->mLeft == NULL ) && ( pRoot->mRight == NULL ) )
	{

		faceIndexArray2d.push_back(pRoot->mTriangleArray);
		return 1;
	}
	else
	{
		return countLeaf(pRoot->mLeft, faceIndexArray2d ) + countLeaf(pRoot->mRight, faceIndexArray2d );
	}
}

bool CxMeshKdTree::hit( const CxRay& ray, std::vector<double>& time )const
{
	return this->hit(mKdNodeRoot, ray, time);
}


bool CxMeshKdTree::hit(KdNode* pNode, const CxRay& ray, std::vector<double>& time )const
{
	bool bHit = ray.isIntersectBoundingBox ( pNode->mBoundingBox );
	
	// NOTE Begin: 这里同时检测射线的反方向是否与包围盒相交; 通常的ray-box相交不需要以下3行代码!
	CxRay rayopp(ray.getOrigin(), -ray.getDirection());
	bool bHitopp = rayopp.isIntersectBoundingBox( pNode->mBoundingBox );
	bHit = ( bHit || bHitopp );
	// NOTE End;

	if ( !bHit ) 
		return false;

	// 如果与根节点的包围盒有交点, 那么递归检测于子节点是否有相交;
	if ( (  pNode->mLeft && pNode->mLeft->mTriangleArray.size()  > 0 ) || 
		 ( pNode->mRight && pNode->mRight->mTriangleArray.size() > 0 )   )
	{
		bool hitleft  = this->hit(pNode->mLeft,  ray, time );
		bool hitright = this->hit(pNode->mRight, ray, time );

		return hitleft || hitright;
	}
	else
	{
		// reach a leaf;
		int nfaces = (int)pNode->mTriangleArray.size();
		bool hitFace = false;
		for (int i=0; i<nfaces; ++i)
		{
			const int index = pNode->mTriangleArray[i];
			const CxFace& f = m_pMesh->GetFace( index );

			const CxVector3d v1( m_pMesh->GetVertex( f[0] ) );
			const CxVector3d v2( m_pMesh->GetVertex( f[1] ) );
			const CxVector3d v3( m_pMesh->GetVertex( f[2] ) );

			double t;
			if ( ray.isIntersectTriangle(v1, v2, v3, &t) )
			{  time.push_back( t ); hitFace = true; }

		}

		return hitFace;
	}

	return false;
}

bool CxMeshKdTree::hitBruteForceMethod(const CxRay& ray, std::vector<double>& time )const
{
	int nfaces = m_pMesh->NumFaces();
	if ( nfaces == 0 ) return false;

	bool hitFace = false;
	for ( int j=0; j<nfaces; ++j )
	{
		const CxFace& f = m_pMesh->GetFace(j);
		const CxVector3d v1( m_pMesh->GetVertex(f[0]) );
		const CxVector3d v2( m_pMesh->GetVertex(f[1]) );
		const CxVector3d v3( m_pMesh->GetVertex(f[2]) );

		double t;
		if ( ray.isIntersectTriangle(v1, v2, v3, &t) )
		{
			time.push_back(t);
			hitFace = true;
		}

	} // end for j;

	return hitFace;
}

// 检测射线与三角形是否相交, 相交的三角形的索引, 及相交的时间存放于rFaceIndexTime数组;
bool CxMeshKdTree::hit( const CxRay& ray, std::vector<std::pair<int, double> >& rFaceIndexTime )const
{
	return this->hit(mKdNodeRoot, ray, rFaceIndexTime);
}

bool CxMeshKdTree::hit(KdNode* pNode, 
	                   const CxRay& ray, 
	                   std::vector<std::pair<int, double> >& rFaceIndexTime )const
{
	bool bHit = ray.isIntersectBoundingBox ( pNode->mBoundingBox );

	// NOTE Begin: 这里同时检测射线的反方向是否与包围盒相交; 通常的ray-box相交不需要以下3行代码!
	CxRay rayopp(ray.getOrigin(), -ray.getDirection());
	bool bHitopp = rayopp.isIntersectBoundingBox( pNode->mBoundingBox );
	bHit = ( bHit || bHitopp );
	// NOTE End;

	if ( !bHit ) 
		return false;

	// 如果与根节点的包围盒有交点, 那么递归检测于子节点是否有相交;
	if ( (  pNode->mLeft && pNode->mLeft->mTriangleArray.size()  > 0 ) || 
		( pNode->mRight && pNode->mRight->mTriangleArray.size() > 0 )   )
	{
		bool hitleft  = this->hit(pNode->mLeft,  ray, rFaceIndexTime );
		bool hitright = this->hit(pNode->mRight, ray, rFaceIndexTime );

		return hitleft || hitright;
	}
	else
	{
		// reach a leaf;
		int nfaces = (int)pNode->mTriangleArray.size();
		bool hitFace = false;
		for (int i=0; i<nfaces; ++i)
		{
			const int index = pNode->mTriangleArray[i];
			const CxFace& f = m_pMesh->GetFace( index );

			const CxVector3d v1( m_pMesh->GetVertex( f[0] ) );
			const CxVector3d v2( m_pMesh->GetVertex( f[1] ) );
			const CxVector3d v3( m_pMesh->GetVertex( f[2] ) );

			double t;
			if ( ray.isIntersectTriangle(v1, v2, v3, &t) )
			{  
				rFaceIndexTime.push_back( std::make_pair(index, t) );
				hitFace = true; 
			}

		}

		return hitFace;
	}

	return false;
}

// 检测球是否与树节点的包围盒相交, 如果相交将该节点包含的面片输出;
bool CxMeshKdTree::hit(const CxSphere& rSphere, FaceIndexArray& rFaceIndexArray )const
{
	return hit( mKdNodeRoot, rSphere, rFaceIndexArray);
}

bool CxMeshKdTree::hit( KdNode* pNode, const CxSphere& rSphere, FaceIndexArray& rFaceIndexArray )const
{
	bool bHit = rSphere.Collision( pNode->mBoundingBox ); 
	bool bHit2 = rSphere.Collision2( pNode->mBoundingBox ); 
	assert ( bHit == bHit2 );


	if ( !bHit ) 
		return false;

	// 如果与根节点的包围盒有交点, 那么递归检测于子节点是否有相交;
	if ( (  pNode->mLeft && pNode->mLeft->mTriangleArray.size()  > 0 ) || 
		 ( pNode->mRight && pNode->mRight->mTriangleArray.size() > 0 )   )
	{
		bool hitleft  = this->hit(pNode->mLeft,  rSphere, rFaceIndexArray );
		bool hitright = this->hit(pNode->mRight, rSphere, rFaceIndexArray );

		return hitleft || hitright;
	}
	else
	{
		// reach a leaf;
		rFaceIndexArray = pNode->mTriangleArray;
		return true;
	}

	return false;
}