// xQuaternion.h: interface for the CxQuaternion class.
// ***************************************************************
//  xQuaternion   version:  1.0   ¡¤  date: 05/07/2008
//  -------------------------------------------------------------
//  Author: 
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#if !defined _XQUATERNION_H__INCLUDED_
#define _XQUATERNION_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xVector3d.h"
#include "xMatrix44.h"

class CxQuaternion  
{
private:

	double      m_w;
	CxVector3d  m_vec;


public:
  
	// Constructors
	CxQuaternion() { Clear();}
	CxQuaternion(double w, double x, double y, double z) { Set(w, x, y, z);}
	CxQuaternion(const CxQuaternion& rQuat);
	CxQuaternion(const CxQuaternion* pQuat);
	CxQuaternion(double w, const CxVector3d& rQuat);
	CxQuaternion(double w, const CxVector3d* pQuat);
	CxQuaternion(const CxVector3d& vecFrom, const CxVector3d& vecTo);
	
	virtual ~CxQuaternion(){};
	
	
	// Data setting
	void Clear();
	void Set(const CxQuaternion *pQuat);
	void Set(const CxQuaternion &rQuat);
	void Set(const double w, const CxVector3d &v);
	void Set(const double w, const CxVector3d *pV);
	void Set(const double w, const double x, const double y, const double z);
	void SetRotation(double ax, double ay, double az, double angle);

	void Trace()const;
	
	void GetMatrix(double *mat) const;
	CxMatrix44 GetMatrix() const;
	
	// Per coordinate (explicit inline functions)
	void w(const double w)     { m_w = w;    }
	void v(const CxVector3d& v){ m_vec = v;  }
	void x(const double x)     { m_vec.x(x); }
	void y(const double y)     { m_vec.y(y); }
	void z(const double z)     { m_vec.z(z); } 
	
	// Data access (explicit inline functions)
	double      w() const { return m_w;   }
	CxVector3d  v() const { return m_vec; }
	double      x() const { return m_vec.x(); }
	double      y() const { return m_vec.y(); }
	double      z() const { return m_vec.z(); }
	
	      double& operator[](int i)       { if(i==0) return (double&)m_w; else return (double&)m_vec[i-1];}
	const double& operator[](int i) const { if(i==0) return (double&)m_w; else return (double&)m_vec[i-1];}
	
	// Operators
	CxQuaternion& operator+=(const CxQuaternion& rQuad);
	CxQuaternion& operator+=(const CxQuaternion* pQuad);
	CxQuaternion& operator-=(const CxQuaternion& rQuad);
	CxQuaternion& operator-=(const CxQuaternion* pQuad);
	CxQuaternion& operator*=(const double d);
	CxQuaternion& operator/=(const double d)
    { return *this *= (1.f/d); }
	
	// Nondestructive unary negation - returns a new vector
	CxQuaternion  operator -() const;
	
	// Binary operators
	friend CxQuaternion operator+(const CxQuaternion& u, const CxQuaternion& v);
	friend CxQuaternion operator-(const CxQuaternion& u, const CxQuaternion& v);
	friend CxQuaternion operator*(const CxQuaternion& u, const CxQuaternion& v);
	friend CxQuaternion operator*(const double s,        const CxQuaternion& u);
	friend CxQuaternion operator*(const CxQuaternion& u, const double s){ return s * u; }
	friend CxQuaternion operator/(const CxQuaternion& u, const double s){ return (1.f/s) * u; }
	friend int         operator==(const CxQuaternion& q1, const CxQuaternion& q2);
	friend int         operator!=(const CxQuaternion& q1, const CxQuaternion& q2){ return !(q1 == q2); }
	
	
	int Equals(const CxQuaternion& q, const double tolerence) const;
	
	// Misc
	double Normalize();
	double Length() const;
	double LengthSquared() const;
	void   Negate();
	CxQuaternion Conjugate();

	void Slerp(const CxQuaternion& q1, const CxQuaternion& q2, double t);

};

#endif // !defined(_XQUATERNION_H__INCLUDED_)
