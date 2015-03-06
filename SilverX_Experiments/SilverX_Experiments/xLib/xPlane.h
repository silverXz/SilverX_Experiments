// xPlane.h: interface for the CxPlane class.
// ***************************************************************
//  xPlane   version:  1.0   ·  date: 06/26/2008
//  -------------------------------------------------------------
//  Author: jh.wu@siat.ac.cn
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if !defined _XPLANE_H__INCLUDED_
#define _XPLANE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xVector3d.h"
#include "xVertex3d.h"


class CxPlane  
{

private:

	double m_a;
	double m_b;
	double m_c;
	double m_d;  // intercept 截距;
	
	// ax + by + cz + d = 0;

public:

	CxPlane(){ Clear(); }
	CxPlane(double a, double b, double c, double d):m_a(a),m_b(b),m_c(c),m_d(d){}
	CxPlane(const CxVector3d& rNormal, double d);
	CxPlane(const CxPlane& rPlane);
	CxPlane(const CxVertex3d& rV0, const CxVertex3d& rV1, const CxVertex3d& rV2);
	virtual ~CxPlane() {}

	void Clear() { Set(.0, .0, .0, .0);}
	void Set(double a, double b, double c, double d){ this->a(a); this->b(b); this->c(c); this->d(d);}
	void Set(const CxVector3d& rNormal, double d)  {  this->d(d); SetNormal(rNormal);}
	void Set(const CxPlane& rPlane);
	void Set(const CxVertex3d& rV0, const CxVertex3d& rV1, const CxVertex3d& rV2);
	
	void a(double _a) { m_a = _a;}
	void b(double _b) { m_b = _b;}
	void c(double _c) { m_c = _c;}
	void d(double _d) { m_d = _d;}
	
	void SetNormal(double a, double b, double c) { this->a(a); this->b(b); this->c(c);}
	void SetNormal(const CxVector3d& rNormal){ a(rNormal.x()); b(rNormal.y()); c(rNormal.z());}
	void SetIntercept(double d) { this->d(d);}

	// access
	double a()const { return m_a;}
	double b()const { return m_b;}
	double c()const { return m_c;}
	double d()const { return m_d;}
	void GetNormal(CxVector3d& rNormal)const             { rNormal.Set(m_a, m_b, m_c);}
	void GetNormal(double& x, double& y, double& z)const { x = a(); y = b(); z = c();}
	const CxVector3d GetNormal()const                    { return CxVector3d(m_a, m_b, m_c);}

	double GetDistance(double x, double y, double z);

	enum eType { ON_PLANE, FRONT_PLANE, BEHIND_PLANE };
	eType ClassifyPoint(const CxVertex3d& rVertex);
	eType ClassifyPoint(double x, double y, double z);

	bool IsVertical(const CxPlane& p); // 两平面是否垂直;
	bool IsParallel(const CxPlane& p); // 两平面是否平行;

	
	friend bool Intersect(CxVertex3d& intersectPoint, const CxPlane& p1, const CxPlane& p2, const CxPlane& p3);
	friend double Distance(const CxPlane& p, const CxVertex3d& v); // distance of vertex v to plane p
	friend double Distance(const CxPlane& p, double x, double y, double z);
	friend CxPlane Lerp(const CxPlane& p1, const CxPlane& p2, double t);


};

inline CxPlane::CxPlane(const CxPlane& rPlane)
{
	m_a = rPlane.m_a;
	m_b = rPlane.m_b;
	m_c = rPlane.m_c;
	m_d = rPlane.m_d;
}

inline CxPlane::CxPlane(const CxVector3d& rNormal, double d)
{
	m_d = d;
	rNormal.Get(m_a, m_b, m_c);
}


inline void CxPlane::Set(const CxPlane& rPlane)
{
	m_a = rPlane.m_a;
	m_b = rPlane.m_b;
	m_c = rPlane.m_c;
	m_d = rPlane.m_d;
}


#endif // !defined(_XPLANE_H__INCLUDED_)
