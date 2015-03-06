// xPlane.cpp: implementation of the CxPlane class.
// ***************************************************************
//  xPlane   version:  1.0   ·  date: 06/26/2008
//  -------------------------------------------------------------
//  Author: jh.wu@siat.ac.cn
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "stdafx.h"
#include "xPlane.h"

#include "xUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxPlane::CxPlane(const CxVertex3d& rV0, const CxVertex3d& rV1, const CxVertex3d& rV2)
{
	Set(rV0, rV1, rV2);
}


void CxPlane::Set(const CxVertex3d& rV0, const CxVertex3d& rV1, const CxVertex3d& rV2)
{
	// This function will create a plane out of a triangle.  First we normalize
	// the triangle.
	CxVector3d vec0(rV2, rV0);
	CxVector3d vec1(rV1, rV0);
	CxVector3d normal(vec0 ^ vec1);
	normal.Normalize();
	
	// Save the normal data to our plane data.  To calculate the d we take the
	// dot product of the triangle normal with the first triangle.
	m_a = normal.x();
	m_b = normal.y();
	m_c = normal.z();
	m_d = - (m_a * rV0.x() + m_b * rV0.y() + m_c * rV0.z());

}

CxPlane::eType CxPlane::ClassifyPoint(double x, double y, double z)
{
	// This function will determine if a point is on, in front of, or behind
	// the plane.  First we store the dot product of the plane and the point.
	double dist = a()*x+ b()*y + c()*z + d();

	
	// Simply put if the dot product is greater than 0 then it is infront of it.
	// If it is less than 0 then it is behind it.  And if it is 0 then it is on it.
	if (dist > 0.001)
		return FRONT_PLANE;
	
	if (dist < -0.001)
		return BEHIND_PLANE;
	
	return ON_PLANE;
}

CxPlane::eType CxPlane::ClassifyPoint(const CxVertex3d& rVertex)
{
	// This function will determine if a point is on, in front of, or behind
	// the plane.  First we store the dot product of the plane and the point.
	double dist = a()*rVertex.x() + b()*rVertex.y() + c()*rVertex.z() + d();

	
	// Simply put if the dot product is greater than 0 then it is infront of it.
	// If it is less than 0 then it is behind it.  And if it is 0 then it is on it.
	if (dist > 0.001)
		return FRONT_PLANE;
	
	if (dist < -0.001)
		return BEHIND_PLANE;
	
	return ON_PLANE;
}


bool CxPlane::IsVertical(const CxPlane& p)
{
	// 两平面的法向量点积为零则相互垂直;
	return ::IsZero(this->GetNormal().Dot(p.GetNormal())) ? true : false;
}

bool CxPlane::IsParallel(const CxPlane& p)
{
	double t1 = a() / p.a();
	double t2 = b() / p.b();
	double t3 = c() / p.c();
	double t4 = d() / p.d();
    // 待补充;
	return false;
}

double CxPlane::GetDistance(double x, double y, double z)
{
	return a()*x+ b()*y + c()*z + d();
}

// ultimate programming
bool Intersect(CxVertex3d& intersectPoint, const CxPlane& p1, const CxPlane& p2, const CxPlane& p3)
{
	// This will find the intersection of 3 planes and store it in intersectPoint.
	CxVector3d vec = p2.GetNormal() ^ p3.GetNormal();
 
	// Get the dot product of p1 with the cross product of p2 and p3.
	double dp = p1.GetNormal().Dot(vec);
	if ( ::IsZero(dp) )
		return false;

	CxVector3d intersection;
	intersection = vec * p1.d();

	vec = p3.GetNormal() ^ p1.GetNormal();
	intersection = intersection + ( vec * p2.d());

	vec = p1.GetNormal() ^ p2.GetNormal();
	intersection += vec * p3.d();

	intersection /= -dp;

	intersectPoint.Set(intersection.x(), intersection.y(), intersection.z());

	return true;
}

double Distance(const CxPlane& p, const CxVertex3d& v)
{
	double dist = p.a()*v.x() + p.b()*v.y() + p.c()*v.z() + p.d();
	return dist / p.GetNormal().Length();
}

double Distance(const CxPlane& p, double x, double y, double z)
{
	double dist = p.a()* x + p.b()* y + p.c()* z + p.d();
	return dist / p.GetNormal().Length();
}

CxPlane Lerp(const CxPlane& p1, const CxPlane& p2, double t)
{
   // This function will perform a linear interpolation between two planes
   CxVector3d normal = p1.GetNormal()*(1.0 - t) + p2.GetNormal()* t;
   normal.Normalize();
   double d = p1.d()*(1.0 - t) + p2.d()*t;

   return CxPlane(normal, d);
}
