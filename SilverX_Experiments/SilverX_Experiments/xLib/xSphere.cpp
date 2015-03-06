/*************************************************************************

   Filename: xSphere.cpp    Version 1.0
   -----------------------------------------------------------------------
   Purpose : 
   Author  : Dr.Wu (jh.wu@siat.ac.cn)
   Created : 2013/06/06   10:23
   Modified:
   -----------------------------------------------------------------------
   Copyright (c) 2013 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences

**************************************************************************/

#include "StdAfx.h"
#include "xSphere.h"


#include "xBoundingBox.h"

bool CxSphere::Collision(const CxBoundingBox& rAABB)const
{
	const CxVertex3d& rMin = rAABB.GetMinVertex();
	const CxVertex3d& rMax = rAABB.GetMaxVertex();

	double dist = 0.0;
	for (int i=0; i<3; ++i)
	{
		if ( mCenter[i] < rMin[i] )
		{
			double fv = mCenter[i] - rMin[i];
			dist += fv * fv;
		}
		else if ( mCenter[i] > rMax[i] )
		{
			double fv = mCenter[i] - rMax[i];
			dist += fv * fv;
		}
	}

	if ( dist <= ( mRadius * mRadius ) )
		return true;

	return false;
}

bool CxSphere::Collision2(const CxBoundingBox& rAABB)const
{
	const CxVertex3d& rMin = rAABB.GetMinVertex();
	const CxVertex3d& rMax = rAABB.GetMaxVertex();

	double tmp[3];
	for (int i=0; i<3; ++i)
		tmp[i] = ::Max(mCenter[i], rMin[i]);
	for (int i=0; i<3; ++i)
		tmp[i] = ::Min(tmp[i], rMax[i]);

	CxVector3d closetPtInAabb = CxVector3d(tmp);
	double dist2 = ( mCenter - closetPtInAabb ).LengthSquared();
	
	// The AABB and the sphere overlap if the closest point within the rectangle is
	// within the sphere's radius
	return dist2 < mRadius * mRadius;
}



bool CxSphere::Collision(const CxSphere& rSphere)const
{
	const CxVector3d r = mCenter - rSphere.GetCenter();
	double dist = r.LengthSquared();
	double radius = mRadius + rSphere.GetRadius();
	
	// 如果两圆心之间的距离小于两半径之和，那么两球相交
	if ( dist <= (radius*radius) )
		return true;

	return false;
}

bool CxSphere::Collision(const CxVector3d& pt, const CxVector3d& normal)const
{	
	//Calculate a vector from the point on the plane to the center of the sphere
	CxVector3d tmp(mCenter - pt);

	//Calculate the distance: dot product of the new vector with the plane's normal
	double dist = tmp.Dot(normal);

	if ( dist > mRadius )
	{	
		//The sphere is not touching the plane
		return false;
	}

	//Else, the sphere is colliding with the plane
	return true;
}



bool CxSphere::IsPointInside(const CxVector3d& pt)const
{
	double dist = (pt - mCenter).LengthSquared();
	return (dist < mRadius*mRadius) ? true : false;
}
