/*************************************************************************

   Filename: xRay.h    Version 1.0
   -----------------------------------------------------------------------
   Purpose : 
   Author  : Dr.Wu (jh.wu@siat.ac.cn)
   Created : 2013/09/04   14:50
   Modified:
   -----------------------------------------------------------------------
   Copyright (c) 2013 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences

**************************************************************************/
#pragma once


#include "xVector3d.h"
#include "xBoundingBox.h"

class CxRay
{
private:

	CxVector3d		mOrigin;    // 起点;
	CxVector3d		mDirection; // normalized;

public:
	CxRay() {}
	CxRay( const CxVector3d& origin, const CxVector3d& dir ) { set(origin, dir); }
	~CxRay(){}

	void set( const CxVector3d& origin, const CxVector3d& dir ) { mOrigin = origin, mDirection = dir; }
	void setOrigin( const CxVector3d& origin ) { mOrigin = origin; }
	void setDirection( const CxVector3d& dir ) { mDirection = dir; }

	const CxVector3d& getOrigin()   const { return mOrigin;    }
	const CxVector3d& getDirection()const { return mDirection; }

	CxVector3d getPosition( double time )const { return mOrigin + mDirection * time; }

	bool isIntersectTriangle( const CxVector3d& p1, const CxVector3d& p2, const CxVector3d& p3, double* result )const;
	bool isIntersectPlane( const CxVector3d& planeOrigin, const CxVector3d& planeNormal, double* result )const;

	// 以下3个函数都是ray-box相交检测，不知道哪个更鲁棒;
	bool isIntersectBoundingBox( const CxBoundingBox& rBox )const;
	bool isIntersectBoundingBox( const CxBoundingBox& rBox, float t0, float t1)const;
	bool isIntersectBoundingBox( const CxBoundingBox& rBox, CxVector3d& rPoint )const;

	// line-box 
	bool isIntersectBoundingBox( const CxVector3d& rPt0, const CxVector3d& rPt1, const CxBoundingBox& rBox)const;

}; // class CxRay


class CxLine
{

protected:

	CxVector3d m_Start;
	CxVector3d m_End;


public:

	CxLine():m_Start(CxVector3d(0.0, 0.0, 0.0)), m_End(CxVector3d(0.0, 0.0, 0.0)){}
	CxLine(const CxVector3d& rStart, const CxVector3d& rEnd ):m_Start(rStart), m_End(rEnd){}
	~CxLine(){}

	void set(const CxVector3d& rStart, const CxVector3d& rEnd ){ m_Start = rStart, m_End = rEnd; }
	void get(CxVector3d& rStart, CxVector3d& rEnd){ rStart = m_Start; rEnd = m_End; }

	// 检测直线段是否与包围盒相交，交点存放于rHit;
	bool isIntersectBoundingBox(const CxBoundingBox& rBox, CxVector3d& rHit)const;
	bool isIntersectBoundingBox(const CxBoundingBox& rBox)const;
	bool isIntersectBoundingBox2(const CxBoundingBox& rBox)const;



private:

	bool getIntersection(double dist1, double dist2, CxVector3d& rHit )const;
	bool isInBox(const CxBoundingBox& rBox, const CxVector3d& rHit, const int rAxis)const;
};

