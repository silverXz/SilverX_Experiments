// xQuaternion.cpp: implementation of the CxQuaternion class.
//
//////////////////////////////////////////////////////////////////////
// ***************************************************************
//  xQuaternion   version:  1.0   ·  date: 05/07/2008
//  -------------------------------------------------------------
//  Author: 
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "stdafx.h"
#include "xQuaternion.h"


#include <cmath>




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxQuaternion::CxQuaternion(const CxQuaternion* pQuat)
{
	Set(pQuat);
}

CxQuaternion::CxQuaternion(const CxQuaternion& rQuat)
{
	Set(rQuat);
}

CxQuaternion::CxQuaternion(double w, const CxVector3d* pVec)
{
	Set(w, pVec);
}

CxQuaternion::CxQuaternion(double w, const CxVector3d& rVec)
{
	Set(w, rVec);
}

CxQuaternion::CxQuaternion(const CxVector3d& vecFrom, const CxVector3d& vecTo)
{
  CxVector3d& vecHalf = vecFrom + vecTo;
  vecHalf.Normalize();

  m_w = vecHalf.Dot(vecTo);
  m_vec = vecHalf.Cross(vecTo);
}


// Clears to a unit quaternion (no rotation)

void CxQuaternion::Clear()
{
	Set(1.0, .0, .0, .0);
}
void CxQuaternion::Set(const CxQuaternion &rQuat)
{
	Set(rQuat.m_w, rQuat.m_vec);
}

void CxQuaternion::Set(const CxQuaternion* pQuat)
{
	Set(pQuat->m_w, pQuat->m_vec);
}


void CxQuaternion::Set(const double w, const double x, const double y, const double z)
{
	m_w = w;
	m_vec.Set(x, y, z);
}

void CxQuaternion::Set(const double w, const CxVector3d *pV)
{
	m_w = w;
	m_vec.Set(pV);
}

void CxQuaternion::Set(const double w, const CxVector3d &v)
{
	m_w = w;
	m_vec.Set(v);
}

void CxQuaternion::SetRotation(double ax, double ay, double az, double angle)
{
	if ( ax + ay + az != 1 )
	{
		double length = (double)sqrt(ax*ax + ay*ay + az*az);
		ax = ax/length; 
		ay = ay/length;
		az = az/length;
	}

    // Convert the angle degrees into radians.
	double radAngle = angle/180.0 * 3.1415926;

	double halfAngle = radAngle / 2.0f;
	m_w = cos(halfAngle);
	m_vec.Set(ax, ay, az);

	m_vec *= sin(halfAngle);
}


//********************************************
// GetMatrix
// Construct rotation matrix from (possibly non-unit) quaternion.
// Assumes matrix is used to multiply column vector on the left:
// Vnew = Matrix * Vold.  Works correctly for right-handed 
// coordinate systems and right-handed rotations.
//********************************************
void CxQuaternion::GetMatrix(double *mat) const
{
	double Nq = LengthSquared();
	double c  = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
	double xc = x()*c,	      yc = y()*c,	  zc = z()*c;
	double sx = w()*xc,	      sy = w()*yc,	  sz = w()*zc;
	double xx = x()*xc,	      xy = x()*yc,	  xz = x()*zc;
	double yy = y()*yc,	      yz = y()*zc,	  zz = z()*zc;
	mat[0]  = double(1.0 - (yy + zz));
	mat[1]  = double(xy - sz);
	mat[2]  = double(xz + sy);
	mat[4]  = double(xy + sz);
	mat[5]  = double(1.0 - (xx + zz));
	mat[6]  = double(yz - sx);
	mat[8]  = double(xz - sy);
	mat[9]  = double(yz + sx);
	mat[10] = double(1.0 - (xx + yy));
	mat[3]  = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0.0f;
	mat[15] = 1.0f;
}

//********************************************
// GetMatrix 从四元数构造矩阵
// Construct rotation matrix from (possibly non-unit) quaternion.
// Assumes matrix is used to multiply column vector on the left:
// Vnew = Matrix * Vold.  Works correctly for right-handed 
// coordinate systems and right-handed rotations.
//********************************************
CxMatrix44 CxQuaternion::GetMatrix() const
{
	CxMatrix44 mat;
	double Nq = LengthSquared();
	double c  = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
	double xc = x()*c,	      yc = y()*c,	  zc = z()*c;
	double sx = w()*xc,	      sy = w()*yc,	  sz = w()*zc;
	double xx = x()*xc,	      xy = x()*yc,	  xz = x()*zc;
	double yy = y()*yc,	      yz = y()*zc,	  zz = z()*zc;
	mat[0][0] = double(1.0 - (yy + zz));
	mat[0][1] = double(xy - sz);
	mat[0][2] = double(xz + sy);
	mat[1][0] = double(xy + sz);
	mat[1][1] = double(1.0 - (xx + zz));
	mat[1][2] = double(yz - sx);
	mat[2][0] = double(xz - sy);
	mat[2][1] = double(yz + sx);
	mat[2][2] = double(1.0 - (xx + yy));
	mat[0][3] = mat[1][3] = mat[2][3] = mat[3][0] = mat[3][1] = mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;
	return mat;
}

void CxQuaternion::Trace() const
{
	TRACE("\n");
	TRACE("** Quaternion **\n");
	TRACE("Address      : %x\n", this);
	TRACE("Coordinates : (%g %g %g %g)\n",m_w,m_vec.x(),m_vec.y(),m_vec.z());
}

CxQuaternion& CxQuaternion::operator+=(const CxQuaternion& rQuad)
{
	m_w   += rQuad.w();
	m_vec += rQuad.v();
	return *this;
}


CxQuaternion& CxQuaternion::operator+=(const CxQuaternion* pQuad)
{
	m_w   += pQuad->m_w;
	m_vec += pQuad->m_vec;
	return *this;
}

CxQuaternion& CxQuaternion::operator-=(const CxQuaternion& rQuad)
{
	m_w   -= rQuad.w();
	m_vec -= rQuad.v();
	return *this;
}


CxQuaternion& CxQuaternion::operator-=(const CxQuaternion* pQuad)
{
	m_w   -= pQuad->w();
	m_vec -= pQuad->v();
	return *this;
}


CxQuaternion& CxQuaternion::operator*=(const double d)
{
	m_w *= d;
	m_vec *= d;
	return *this;
}

//********************************************
// Operator -
//  Nondestructive unary -
//  Returns a new vector.
//********************************************
CxQuaternion CxQuaternion::operator -() const
{
	return CxQuaternion(-m_w,-m_vec);
}


CxQuaternion operator+(const CxQuaternion& u, const CxQuaternion& v)
{
	return CxQuaternion(u.m_w + v.m_w, u.m_vec + v.m_vec);
}


CxQuaternion operator-(const CxQuaternion& u, const CxQuaternion& v)
{
	return CxQuaternion(u.m_w - v.m_w, u.m_vec - v.m_vec);
}


CxQuaternion operator*(const CxQuaternion& u, const CxQuaternion& v)
{
	CxQuaternion w;
	CxVector3d a  = u.m_vec;
	CxVector3d b  = v.m_vec;
	double     ws = u.m_w * v.m_w - a.Dot(b);
	CxVector3d wv = u.m_w*b + v.m_w*a + a.Cross(b);
	w.Set(ws, wv);
	return w;
}


CxQuaternion operator*(const double s, const CxQuaternion& u)
{
	return CxQuaternion(u.m_w * s, u.m_vec * s);
}

int operator==(const CxQuaternion& q1, const CxQuaternion& q2)
{
	return (q1.m_w == q2.m_w && q1.m_vec == q2.m_vec);
}

//********************************************
// Equals
//  Determines if two quaternions are equal
//  within a tolerence (squared distance).
//********************************************
int CxQuaternion::Equals(const CxQuaternion& q, const double tolerence) const
{
	CxQuaternion diff = *this - q;
	return diff.LengthSquared() <= tolerence;
}


double CxQuaternion::Normalize()
{
	double len = Length();
	if(len != 0.0f)
		(*this) *= (1.0/len);
	else
		Set(0.f,0.f,0.f,0.f);
	
	return len;
}

double CxQuaternion::Length()const
{
	return sqrt((double)m_w * (double)m_w + 
	        	(double)m_vec.x()*(double)m_vec.x() + 
		        (double)m_vec.y()*(double)m_vec.y() + 
		        (double)m_vec.z()*(double)m_vec.z());
}

double CxQuaternion::LengthSquared()const
{
	return ((double)m_w      *(double)m_w       + 
	    	(double)m_vec.x()*(double)m_vec.x() + 
	    	(double)m_vec.y()*(double)m_vec.y() + 
	    	(double)m_vec.z()*(double)m_vec.z());
}

//  Negate each component of the quaternion
void CxQuaternion::Negate()
{
	m_w = -m_w;
	m_vec = -m_vec;
}

//  Return the conjugate of the quaternion
CxQuaternion CxQuaternion::Conjugate()
{
	return CxQuaternion(m_w, -m_vec[0], -m_vec[1], -m_vec[2]);
}


/*================================================================
 * Author/date      : jh.wu@siat.ac.cn  2008-5-7 21:06:04
 * Function name	: CxQuaternion::Slerp
 * Description	    : 在两个四元数之间平滑插值;
 *                     Spherical Linear Interpolation
 *
 *                      sin(1-t)w            sin(tw)
 *   slerp(q1, q2, t) = _________ q1  +      ________ q2
 *                       sin(w)               sin(w)
 * Return type		: void 
 * Argument         : const CxQuaternion& q1
 * Argument         : const CxQuaternion& q2
 * Argument         : double t
 *================================================================*/
void CxQuaternion::Slerp(const CxQuaternion& q1, const CxQuaternion& q2, double t)
{
	double cosOmega = 0.0;

	double w = q2.w();
	double x = q2.x();
	double y = q2.y();
	double z = q2.z();

	// 点乘计算两四元数夹角的cos值;
	cosOmega = q1.x()*q2.x() + q1.y()*q2.y() + q1.z()*q2.z() + q1.w()*q2.w();

    // 如果点乘为负, 则反转一个四元数已取得最短的4D"弧"
	if ( cosOmega < 0.0f )
	{
		// Flip sigh if so.
		w = -w;
		x = -x;
		y = -y;
		z = -z;
		cosOmega = -cosOmega;
	}
	
    double k0 = 0.0 , k1 = .0;
	if ( cosOmega > 0.9999f )
	{
		// 非常接近，即线性插值;
		k0 = 1.0 - t;
		k1 = t;
	}
	else
	{
		// 利用三角公式: sin^2(omega) + cos^2(omega) = 1 计算sin值;
		double sinOmega = sqrt( 1.0 - cosOmega*cosOmega );

		// 计算角度
		double omega = atan2(sinOmega, cosOmega);
		double oneOverSinOmega = 1.0 / sinOmega;

		// 计算插值
		k0 = sin( ( 1.0 - t )*omega ) * oneOverSinOmega;
		k1 = sin( t*omega ) * oneOverSinOmega;
	}
	
	// Interpolation.
	m_w   = k0*q1.w() + k1*q2.w();
	m_vec = k0*q1.v() + k1*q2.v();
}