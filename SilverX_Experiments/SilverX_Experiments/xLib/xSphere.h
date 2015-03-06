/*************************************************************************

   Filename: xSphere.h    Version 1.0
   -----------------------------------------------------------------------
   Purpose : 
   Author  : Dr.Wu (jh.wu@siat.ac.cn)
   Created : 2013/06/06   10:23
   Modified:
   -----------------------------------------------------------------------
   Copyright (c) 2013 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences

**************************************************************************/
#pragma once

#include "xUtil.h"
#include "xVector3d.h"

class CxBoundingBox;
class CxPlane;

class CxSphere
{
private:

	CxVector3d   mCenter; // ÇòÐÄ
	double       mRadius; // °ë¾¶

public:
	CxSphere(void) { Clear();}
	CxSphere(const CxVector3d& center, double radius) { Set(center, radius);}
	~CxSphere(void){}

	void Set(const CxVector3d& center, double radius){ mCenter = center, mRadius = radius;}
	void SetRadius(double radius) { mRadius = radius; }
	void SetCenter(const CxVector3d& center) { mCenter = center; }
	void Clear() { mCenter.Clear(); mRadius = 0;}

	const CxVector3d& GetCenter()       { return mCenter; }
	const CxVector3d& GetCenter() const { return mCenter; }

	double GetRadius()      { return mRadius; }
	double GetRadius()const { return mRadius; }

	//
	double Area()  { return 4.0*M_PI*mRadius*mRadius;}
	double Volume(){ return 4.0*M_PI*mRadius*mRadius*mRadius/3.0;}

	// 
	bool Collision(const CxSphere& rSphere)const;
	bool Collision(const CxBoundingBox& rAABB)const;
	bool Collision2(const CxBoundingBox& rAABB)const;
	bool Collision(const CxVector3d& pt, const CxVector3d& normal)const;
	bool IsPointInside(const CxVector3d& pt)const;

};

