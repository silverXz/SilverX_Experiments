// Vector3d.cpp
// ***************************************************************
//  xVector3d   version:  1.0   ? 
//  -------------------------------------------------------------
//  Description: 3D Vector to represent directions.
//  Author     :  wujh@sia.cn 
//  Created    :  07/17/2006
//  Modified   :
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "stdafx.h"
#include "xVector3d.h"

#include <cmath>

#include "xVertex3d.h"



CxVector3d::CxVector3d(const CxVertex3d& rV1, const CxVertex3d& rV2)
{
	vec[0] = rV2.x() - rV1.x();
	vec[1] = rV2.y() - rV1.y();
	vec[2] = rV2.z() - rV1.z();
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-17 8:36:29
 * Function name	: CxVector3d::Get
 * Description	    : 获得向量的三个分量;
 * Return type		: void 
 * Argument         : double& x
 * Argument         : double& y
 * Argument         : double& z
 *================================================================*/
void CxVector3d::Get(double& x, double& y, double& z) const
{
	x = vec[0];
	y = vec[1];
	z = vec[2];
}


CxVector3d& CxVector3d::operator+=(const CxVector3d& rVector)
{
	vec[0] += rVector.x();
	vec[1] += rVector.y();
	vec[2] += rVector.z();
	return *this;
}

CxVector3d& CxVector3d::operator+=(const CxVector3d* pVector)
{
	vec[0] += pVector->x();
	vec[1] += pVector->y();
	vec[2] += pVector->z();
	return *this;
}

CxVector3d& CxVector3d::operator-=(const CxVector3d& rVector)
{
	vec[0] -= rVector.x();
	vec[1] -= rVector.y();
	vec[2] -= rVector.z();
	return *this;
}


CxVector3d& CxVector3d::operator-=(const CxVector3d* pVector)
{
	vec[0] -= pVector->x();
	vec[1] -= pVector->y();
	vec[2] -= pVector->z();
	return *this;
}


CxVector3d& CxVector3d::operator*=(double d)
{
	vec[0] *= d;
	vec[1] *= d;
	vec[2] *= d;
	return *this;
}


CxVector3d CxVector3d::operator-() const
{
	return CxVector3d(-vec[0], - vec[1], - vec[2]);
}


CxVector3d operator+ (const CxVector3d& u, const CxVector3d& v)
{
	return CxVector3d(u.vec[0] + v.vec[0], u.vec[1] + v.vec[1], u.vec[2] + v.vec[2]);
}


CxVector3d operator- (const CxVector3d& u, const CxVector3d& v)
{
	return CxVector3d(u.vec[0] - v.vec[0], u.vec[1] - v.vec[1], u.vec[2] - v.vec[2]);
}

CxVector3d operator* (double s, const CxVector3d& u)
{
	return CxVector3d(u.vec[0] * s, u.vec[1] * s, u.vec[2] * s);
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-17 8:42:23
 * Function name	: operator^
 * Description	    : Returns the cross product of u and v.
 * Return type		: CxVector3d 
 * Argument         : const CxVector3d& u
 * Argument         : const CxVector3d& v
 *================================================================*/
CxVector3d operator^(const CxVector3d& u, const CxVector3d& v)
{
	return CxVector3d(u.vec[1] * v.vec[2] - u.vec[2] * v.vec[1],
	                	u.vec[2] * v.vec[0] - u.vec[0] * v.vec[2],
	                    	u.vec[0] * v.vec[1] - u.vec[1] * v.vec[0]);
}

int operator==(const CxVector3d& v1, const CxVector3d& v2)
{
	return (v1.vec[0] == v2.vec[0] &&
		      v1.vec[1] == v2.vec[1] &&
	             v1.vec[2] == v2.vec[2]);
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-17 8:41:32
 * Function name	: CxVector3d::Equals
 * Description	    : Determines if two vectors are equal
 *                  : within a tolerence (squared distance).
 * Return type		: int 
 * Argument         : const CxVector3d& v
 * Argument         : double tolerence
 *================================================================*/
int CxVector3d::Equals(const CxVector3d& v, double tolerence) const
{
	CxVector3d diff = *this - v;
	
	return diff.LengthSquared() <= tolerence;
}


CxVector3d CxVector3d::Cross(const CxVector3d& v) const
{
	return CxVector3d(y() * v.z() - z() * v.y(),
		              z() * v.x() - x() * v.z(),
		              x() * v.y() - y() * v.x());
}


CxVector3d CxVector3d::Cross(const CxVector3d* pV) const
{
	return CxVector3d(y() * pV->z() - z() * pV->y(),
		              z() * pV->x() - x() * pV->z(),
		              x() * pV->y() - y() * pV->x());
}

double CxVector3d::Normalize()
{
	double len = Length();
	if (len != 0.0f)
		(*this) *=(1.0/len);
	else
		Set(0.f, 0.f, 0.f);
	
	return len;
}


double CxVector3d::Normalize(double value)
{
	double len = Length();
	if (len != 0.0f)
		(*this) *=(value/len);
	else
		Set(0.f, 0.f, 0.f);
	
	return len;
}


double CxVector3d::LengthSquared() const
{
	return ((double)vec[0]* (double)vec[0]
		       + (double)vec[1]* (double)vec[1]
		            + (double)vec[2]* (double)vec[2]);
}
	
double CxVector3d::Length() const
{
	return sqrt((double)vec[0]* (double)vec[0]
		        + (double)vec[1]* (double)vec[1]
	              	+ (double)vec[2]* (double)vec[2]);
}
	

int CxVector3d::IsCollinear(CxVector3d *pVector) const
{
	double x = pVector->x() / vec[0];
	double y = pVector->y() / vec[1];
	double z = pVector->z() / vec[2];
	return ((x == y) && (y == z));
}

int CxVector3d::IsCollinear(CxVector3d &rVector) const
{
	double x = rVector.x() / vec[0];
	double y = rVector.y() / vec[1];
	double z = rVector.z() / vec[2];
	return ((x == y) && (y == z));
}

void CxVector3d::Negate()
{
	vec[0] = -vec[0];
	vec[1] = -vec[1];
	vec[2] = -vec[2];
}

/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-17 8:45:48
 * Function name	: CxVector3d::Rotate
 * Description	    : Rotate this vector around pAround by angle
 * Return type		: CxVector3d 
 * Argument         : double angle
 * Argument         : CxVector3d Around
 *================================================================*/
CxVector3d CxVector3d::Rotate(double angle, const CxVector3d& Around)const
{
	double f1, f2, f3;
	CxVector3d t1, t2;

	f1 = (double)cos((double)angle);
	f2 = (double)sin((double)angle);
	t1 = Projection(&Around);
	t2 = Around.Cross(this);
	f3 = Dot(Around);
	
	return CxVector3d((double)(f1*t1.x() + f2*t2.x() + f3*Around.x()),
		                  (double)(f1*t1.y() + f2*t2.y() + f3*Around.y()),
	                            	(double)(f1*t1.z() + f2*t2.z() + f3*Around.z()));
}

CxVector3d  CxVector3d::Projection(const CxVector3d* pV) const
{
	double alpha = Dot(pV)/pV->Dot(pV);
	return CxVector3d(x() - alpha* pV->x(), 
		                y() - alpha*pV->y(),
		                      z() - alpha*pV->z());
}

double CxVector3d::GetLengthXY()const
{
	return sqrt(vec[0]*vec[0] + vec[1]*vec[1]);
}

double CxVector3d::GetLengthXZ()const
{
	return sqrt(vec[0]*vec[0] + vec[2]*vec[2]);
}

double CxVector3d::GetLengthYZ()const
{
	return sqrt(vec[1]*vec[1] + vec[2]*vec[2]);
}


void CxVector3d::Trace() const
{
	TRACE("\n");
	TRACE("** CxVector3d **\n");
	TRACE("Address      : %x\n", this);
	TRACE("Coordinates : (%g %g %g)\n", vec[0], vec[1], vec[2]);
}


// Angle between two vectors (in radians)
// uv = |u||v| cos(u,v)
// u  ^ v  = w
// |w| = |u||v| |sin(u,v)|
double Angle(const CxVector3d& u, const CxVector3d& v)
{
	double dLengthV = v.Length();
	double dLengthU = u.Length();
	double product = dLengthU * dLengthV;

	if ( ::IsZero(product) )
		return 0.0;

	double dotproduct = u.Dot(v);
	double cosinus = dotproduct / product;

	CxVector3d w = u.Cross(v);
	double dLengthW = w.Length();
	double absSinus = dLengthW / product;

	// Remove degeneracy
	absSinus = (absSinus > 1 ) ? 1 : absSinus;
	absSinus = (absSinus < -1) ? -1 : absSinus;

	if ( cosinus >= 0 )    // acute angle
		return asin(absSinus);
	else                   // obtuse angle
		return M_PI - asin(absSinus);
}

double CosAngle(const CxVector3d& u, const CxVector3d& v)
{
	double dLengthV = v.Length();
	double dLengthU = u.Length();
	double product = dLengthU * dLengthV;

	if ( ::IsZero(product) )
		return 0.0;

	double dotproduct = u.Dot(v);
	double cosinus = dotproduct / product;

	return cosinus;
}

double SinAngle(const CxVector3d& u, const CxVector3d& v)
{
	double dLengthV = v.Length();
	double dLengthU = u.Length();
	double product = dLengthU * dLengthV;

	if ( ::IsZero(product) )
		return 0.0;

	CxVector3d w = u.Cross(v);
	double dLengthW = w.Length();
	double absSinus = dLengthW / product;

	// Remove degeneracy
	absSinus = (absSinus > 1 ) ? 1 : absSinus;
	absSinus = (absSinus < -1) ? -1 : absSinus;

	return absSinus;
}

//计算经OpenGL ModelView变换后的世界坐标
CxVector3d getTransformedVector(const double* m,const CxVector3d &v)
{
	double n0,n1,n2,n3;
	n0 = m[0] * v.x() + m[4] * v.y() + m[8] * v.z() + m[12];
	n1 = m[1] * v.x() + m[5] * v.y() + m[9] * v.z() + m[13];
	n2 = m[2] * v.x() + m[6] * v.y() + m[10] * v.z() + m[14];
	n3 = m[3] * v.x() + m[7] * v.y() + m[11] * v.z() + m[15];
	return CxVector3d(n0,n1,n2);
}
// ** EOF **
