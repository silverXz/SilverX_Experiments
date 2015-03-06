/**************************************************************************
	
	File name:	xMeshAngiography.h      Version 1.0
---------------------------------------------------------------------------
	Purpose :	construct mesh for simulating angiography
    Author  :   Wang HaoYu (wang.haoyu@siat.ac.cn) 
	            Dr.Wu (jh.wu@siat.ac.cn)
	Created :	2014/08/01   16:05
    Modified:
---------------------------------------------------------------------------
   Copyright (c) 2014 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences
**************************************************************************/
#pragma once

#include "xMesh.h"
#include "xMeshKdTree.h"
#include "xMeshRepair.h"

class CxMeshAngiography
{
protected:

	CxMesh*		     m_pMesh;
	CxMeshKdTree*    m_pMeshKdTree;

	CxVector3d		 m_BasePoint;  // 切割的基准点，通常导丝当前位置的后3个点。
	CxVector3d		 m_BaseOrientation;  // 导丝的前进方向;
	double           m_Radius;  // 包围球半径;

private:

	bool IsFaceOnRightSide(const CxFace& rTargetFace)const;
	bool IsFaceCrossSurface(const CxFace& rTargetFace)const;
	bool IsFaceInsideSphere(const CxFace& rTargetFace)const;
	double EstimateRadius()const;
	CxMesh* ConstructMesh(const std::vector<CxFace>& rFaceArray )const;

public:
	CxMeshAngiography(){};
	CxMeshAngiography(CxMesh* pMesh);
	CxMeshAngiography(const CxVector3d& rBasePoint, const CxVector3d& rBaseOrientation, const double rRadius);
	~CxMeshAngiography(void);



	bool IsFaceInsideSphere(const CxFace& rFace,const CxVertex3d& rCenter,const double rRadius)const;

#if 0
	bool IsFaceOnRightSide(const CxVertex3d& rBasePoint, const CxVector3d& rBaseOrientation, const CxFace& rTargetFace)const;
	bool IsFaceCrossSurface(const CxVertex3d& rBasePoint, const CxVector3d& rBaseOrientation, const CxFace& rTargetFace)const;
	CxMesh* GenerateSubMesh(const CxVector3d& rBasePoint, const CxVector3d& rBaseOrientation, const double rRadius)const;
#endif	

	CxMesh* GenerateSubMesh(const CxVector3d& rBasePoint, const CxVector3d& rBaseOrientation, const double rRadius);

	




};

