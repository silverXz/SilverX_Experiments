/**************************************************************************
	
	File name:	xMeshKdTree.h      Version 1.0
---------------------------------------------------------------------------
	Purpose :	����MESH����Kd tree, ���ڲ�ѯ
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
	int                         mMaxDepth; // kd tree �ָ��������; default: 10;
	std::vector<CxBoundingBox>  mAABBs;    // ���ÿ����Ƭ�İ�Χ��;

private:

	KdNode* build(FaceIndexArray& rTriangles, int nDepth);

	// ������������faces������������Ƭ�İ�Χ��, ���ط���������;
	int calculateBoundingBox(const FaceIndexArray& faces, CxBoundingBox& box)const;
	void calculateBoundingBox(); // ����������Ƭ�İ�Χ��;

	// ������������faces������������Ƭ��ƽ����;
	const CxVertex3d calculateCenter(const FaceIndexArray& faces)const;

	void destroy( CxMeshKdTree::KdNode* pNode );
	bool hit( KdNode* pNode, const CxRay& ray, std::vector<double>& time)const;
	bool hit( KdNode* pNode, const CxRay& ray, std::vector<std::pair<int, double> >& rFaceIndexTime)const;

	bool hit( KdNode* pNode, const CxSphere& rSphere, FaceIndexArray& rFaceIndexArray )const;


	// for debug; �ռ�Ҷ�ڵ����Ƭ;
	int countLeaf(KdNode* pRoot, std::vector<FaceIndexArray>& faceIndexArray2d )const;

public:

	CxMeshKdTree(const CxMesh* pMesh);
	~CxMeshKdTree(void);

	

	// ����KdTree, Ĭ�����Ϊ10;�ɹ�����true���򷵻�false;
	bool build(int maxDepths = 10);  

	// ����������������Ƿ��ཻ, �ཻ��ʱ������time����;
	bool hit(const CxRay& ray, std::vector<double>& time)const;


	// ����������������Ƿ��ཻ, �ཻ�������ε�����, ���ཻ��ʱ������rFaceIndexTime����;
	bool hit(const CxRay& ray, std::vector<std::pair<int, double> >& rFaceIndexTime)const;

	// ������Ƿ������ڵ�İ�Χ���ཻ, ����ཻ���ýڵ��������Ƭ���;
	bool hit( const CxSphere& rSphere, FaceIndexArray& rFaceIndexArray )const;


	// �������������������ߺ�����������ģ�͵Ľ���,
	bool hitBruteForceMethod(const CxRay& ray, std::vector<double>& time )const;


};
