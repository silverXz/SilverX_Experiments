/**************************************************************************
	
	File name:	xMeshKdTree.h      Version 1.0
---------------------------------------------------------------------------
	Purpose :	利用MESH构建Kd tree, 便于查询
    Author  :   Dr.Wu (jh.wu@siat.ac.cn)
	Created :	2014/07/23   12:15
    Modified:
---------------------------------------------------------------------------
   Copyright (c) 2014 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences
**************************************************************************/
#pragma once


#include "xRay.h"
#include "xSphere.h"



class CxMesh;
class CxMeshKdTree
{
public:

	typedef std::vector<int>     FaceIndexArray;

private:

	struct KdNode
	{
		KdNode*				mLeft;
		KdNode*				mRight;

		CxBoundingBox		mBoundingBox;
		FaceIndexArray      mTriangleArray;

		KdNode():mLeft(NULL), mRight(NULL), mBoundingBox(CxBoundingBox()) 
		{ mTriangleArray = FaceIndexArray();}

		~KdNode(){}

	};

	// class members:

	const CxMesh*				m_pMesh;
	KdNode*						mKdNodeRoot;
	int                         mMaxDepth; // kd tree 分割的最大深度; default: 10;
	std::vector<CxBoundingBox>  mAABBs;    // 存放每个面片的包围盒;

private:

	KdNode* build(FaceIndexArray& rTriangles, int nDepth);

	// 计算存放于数组faces的所有三角面片的包围盒, 返回方差最大的轴;
	int calculateBoundingBox(const FaceIndexArray& faces, CxBoundingBox& box)const;
	void calculateBoundingBox(); // 计算所有面片的包围盒;

	// 计算存放于数组faces的所有三角面片的平均点;
	const CxVertex3d calculateCenter(const FaceIndexArray& faces)const;

	void destroy( CxMeshKdTree::KdNode* pNode );
	bool hit( KdNode* pNode, const CxRay& ray, std::vector<double>& time)const;
	bool hit( KdNode* pNode, const CxRay& ray, std::vector<std::pair<int, double> >& rFaceIndexTime)const;

	bool hit( KdNode* pNode, const CxSphere& rSphere, FaceIndexArray& rFaceIndexArray )const;


	// for debug; 收集叶节点的面片;
	int countLeaf(KdNode* pRoot, std::vector<FaceIndexArray>& faceIndexArray2d )const;

public:

	CxMeshKdTree(const CxMesh* pMesh);
	~CxMeshKdTree(void);

	

	// 构建KdTree, 默认深度为10;成功返回true否则返回false;
	bool build(int maxDepths = 10);  

	// 检测射线与三角形是否相交, 相交的时间存放于time数组;
	bool hit(const CxRay& ray, std::vector<double>& time)const;


	// 检测射线与三角形是否相交, 相交的三角形的索引, 及相交的时间存放于rFaceIndexTime数组;
	bool hit(const CxRay& ray, std::vector<std::pair<int, double> >& rFaceIndexTime)const;

	// 检测球是否与树节点的包围盒相交, 如果相交将该节点包含的面片输出;
	bool hit( const CxSphere& rSphere, FaceIndexArray& rFaceIndexArray )const;


	// 采用蛮力法，计算射线和三角形网格模型的交点,
	bool hitBruteForceMethod(const CxRay& ray, std::vector<double>& time )const;


};
