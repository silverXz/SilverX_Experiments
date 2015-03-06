// Matrix44.cpp
// ***************************************************************
//  xMatrix44   version:  1.0   
//  -------------------------------------------------------------
//  Description:  4x4 matrix to represent transformations.
//  Author     :  wujh@sia.cn 
//  Created    :  07/17/2006
//  Modified   : 
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "stdafx.h"
#include "xMatrix44.h"

#include <cmath>



CxMatrix44::CxMatrix44(const CMat44& m)
{
	m_data[0][0] = m[0][0];
	m_data[0][1] = m[0][1];
	m_data[0][2] = m[0][2];
	m_data[0][3] = m[0][3];
	m_data[1][0] = m[1][0];
	m_data[1][1] = m[1][1];
	m_data[1][2] = m[1][2];
	m_data[1][3] = m[1][3];
	m_data[2][0] = m[2][0];
	m_data[2][1] = m[2][1];
	m_data[2][2] = m[2][2];
	m_data[2][3] = m[2][3];
	m_data[3][0] = m[3][0];
	m_data[3][1] = m[3][1];
	m_data[3][2] = m[3][2];
	m_data[3][3] = m[3][3];
}


CxMatrix44::CxMatrix44(const double *data)
{
	Set(data);
}


CxMatrix44::CxMatrix44(double a11, double a12, double a13, double a14,
		               double a21, double a22, double a23, double a24,
		               double a31, double a32, double a33, double a34,
		               double a41, double a42, double a43, double a44)
{
	m_data[0][0] = a11;
	m_data[0][1] = a12;
	m_data[0][2] = a13;
	m_data[0][3] = a14;
	
	m_data[1][0] = a21;
	m_data[1][1] = a22;
	m_data[1][2] = a23;
	m_data[1][3] = a24;
	
	m_data[2][0] = a31;
	m_data[2][1] = a32;
	m_data[2][2] = a33;
	m_data[2][3] = a34;
	
	m_data[3][0] = a41;
	m_data[3][1] = a42;
	m_data[3][2] = a43;
	m_data[3][3] = a44;
}

CxMatrix44::CxMatrix44(const CxMatrix44 &rMatrix)
{
	Set(&rMatrix);
}


CxMatrix44::CxMatrix44(const CxMatrix44 *pMatrix)
{
	Set(pMatrix);
}

//  Sets the matrix to be the identity matrix.
void CxMatrix44::MakeIdentity()
{
	m_data[0][0] = 1.f;
	m_data[0][1] = 0.f;
	m_data[0][2] = 0.f;
	m_data[0][3] = 0.f;
	
	m_data[1][0] = 0.f;
	m_data[1][1] = 1.f;
	m_data[1][2] = 0.f;
	m_data[1][3] = 0.f;
	
	m_data[2][0] = 0.f;
	m_data[2][1] = 0.f;
	m_data[2][2] = 1.f;
	m_data[2][3] = 0.f;
	
	m_data[3][0] = 0.f;
	m_data[3][1] = 0.f;
	m_data[3][2] = 0.f;
	m_data[3][3] = 1.f;
}

void CxMatrix44::Clear()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] = 0.f;
}


void CxMatrix44::Set(const CMat44& m)
{
	m_data[0][0] = m[0][0];
	m_data[0][1] = m[0][1];
	m_data[0][2] = m[0][2];
	m_data[0][3] = m[0][3];
	m_data[1][0] = m[1][0];
	m_data[1][1] = m[1][1];
	m_data[1][2] = m[1][2];
	m_data[1][3] = m[1][3];
	m_data[2][0] = m[2][0];
	m_data[2][1] = m[2][1];
	m_data[2][2] = m[2][2];
	m_data[2][3] = m[2][3];
	m_data[3][0] = m[3][0];
	m_data[3][1] = m[3][1];
	m_data[3][2] = m[3][2];
	m_data[3][3] = m[3][3];
}


void CxMatrix44::Set(const double *data)
{
	int k = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] = data[k++];
}

void CxMatrix44::Set(const CxMatrix44 &rMatrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] = rMatrix.Get(i, j);
}


void CxMatrix44::Set(const CxMatrix44 *pMatrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] = pMatrix->Get(i, j);
}


void CxMatrix44::SetTranslate(double tx, double ty, double tz)
{
	m_data[0][0] = 1.f;
	m_data[0][1] = 0.f;
	m_data[0][2] = 0.f;
	m_data[0][3] = tx;
	
	m_data[1][0] = 0.f;
	m_data[1][1] = 1.f;
	m_data[1][2] = 0.f;
	m_data[1][3] = ty;
	
	m_data[2][0] = 0.f;
	m_data[2][1] = 0.f;
	m_data[2][2] = 1.f;
	m_data[2][3] = tz;
	
	m_data[3][0] = 0.f;
	m_data[3][1] = 0.f;
	m_data[3][2] = 0.f;
	m_data[3][3] = 1.f;
}


void CxMatrix44::SetRotate(double x, double y, double z, double radAngle)
{
#if 0
	double q[4];
	double c = cos(radAngle/2);
	double s = sin(radAngle/2);

	q[0] = c;
	q[1] = s * x;
	q[2] = s * y;
	q[3] = s * z;

	m_data[0][0] = 1.f - 2.f * (q[2] * q[2] + q[3] * q[3]);
	m_data[0][1] =       2.f * (q[1] * q[2] + q[3] * q[0]);
	m_data[0][2] =       2.f * (q[3] * q[1] - q[2] * q[0]);
	m_data[0][3] = 0.f;

	m_data[1][0] =       2.f * (q[1] * q[2] - q[3] * q[0]);
	m_data[1][1] = 1.f - 2.f * (q[3] * q[3] + q[1] * q[1]);
	m_data[1][2] =       2.f * (q[2] * q[3] + q[1] * q[0]);
	m_data[1][3] = 0.f;

	m_data[2][0] =       2.f * (q[3] * q[1] + q[2] * q[0]);
	m_data[2][1] =       2.f * (q[2] * q[3] - q[1] * q[0]);
	m_data[2][2] = 1.f - 2.f * (q[2] * q[2] + q[1] * q[1]);
	m_data[2][3] = 0.f;

	m_data[3][0] = 0.f;
	m_data[3][1] = 0.f;
	m_data[3][2] = 0.f;
	m_data[3][3] = 1.f;
#endif

	// If NULL vector passed in, this will blow up...
	if ( x == 0.0f && y == 0.0f && z == 0.0f )
	{
		MakeIdentity();
		return;
	}

	// Scale vector
	double vecLength = (double)sqrt( x*x + y*y + z*z );

	// Rotation matrix is normalized
	x /= vecLength;
	y /= vecLength;
	z /= vecLength;

	double sinSave = (double)sin(radAngle);
	double cosSave = (double)cos(radAngle);
	double oneMinusCos = 1.0f - cosSave;

	double xx = x * x;
	double yy = y * y;
	double zz = z * z;
	double xy = x * y;
	double yz = y * z;
	double zx = z * x;
	double xs = x * sinSave;
	double ys = y * sinSave;
	double zs = z * sinSave;

	m_data[0][0] = (oneMinusCos * xx) + cosSave;
	m_data[0][1] = (oneMinusCos * xy) - zs;
	m_data[0][2] = (oneMinusCos * zx) + ys;
	m_data[0][3] = 0.0f;

	m_data[1][0] = (oneMinusCos * xy) + zs;
	m_data[1][1] = (oneMinusCos * yy) + cosSave;
	m_data[1][2] = (oneMinusCos * yz) - xs;
	m_data[1][3] = 0.0f;

	m_data[2][0] = (oneMinusCos * zx) - ys;
	m_data[2][1] = (oneMinusCos * yz) + xs;
	m_data[2][2] = (oneMinusCos * zz) + cosSave;
	m_data[2][3] = 0.0f;

	m_data[3][0] = 0.0f;
	m_data[3][1] = 0.0f;
	m_data[3][2] = 0.0f;
	m_data[3][3] = 1.0f;
}


void CxMatrix44::SetScale(double sx, double sy, double sz)
{
	m_data[0][0] = sx;
	m_data[0][1] = 0.f;
	m_data[0][2] = 0.f;
	m_data[0][3] = 0.f;
	
	m_data[1][0] = 0.f;
	m_data[1][1] = sy;
	m_data[1][2] = 0.f;
	m_data[1][3] = 0.f;
	
	m_data[2][0] = 0.f;
	m_data[2][1] = 0.f;
	m_data[2][2] = sz;
	m_data[2][3] = 0.f;
	
	m_data[3][0] = 0.f;
	m_data[3][1] = 0.f;
	m_data[3][2] = 0.f;
	m_data[3][3] = 1.f;
}



CxMatrix44& CxMatrix44::operator+=(const CxMatrix44& rMatrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] += rMatrix[i][j];
	
	return *this;
}


CxMatrix44& CxMatrix44::operator+=(const CxMatrix44* pMatrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] += pMatrix->Get(i, j);
	
	return *this;
}


CxMatrix44& CxMatrix44::operator-=(const CxMatrix44& rMatrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] -= rMatrix[i][j];
	
	return *this;
}


CxMatrix44& CxMatrix44::operator-=(const CxMatrix44 *pMatrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] -= pMatrix->Get(i, j);
	
	return *this;
}


CxMatrix44& CxMatrix44::operator*=(const double d)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_data[i][j] *= d;
	
	return *this;
}


CxMatrix44 operator+ (const CxMatrix44& u, const CxMatrix44& v)
{
	CxMatrix44 w;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			w[i][j] = u[i][j] + v[i][j];
	
	return w;
}


CxMatrix44 operator- (const CxMatrix44& u, const CxMatrix44& v)
{
	CxMatrix44 w;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			w[i][j] = u[i][j] - v[i][j];
	
	return w;
}


CxMatrix44 operator* (const double s, const CxMatrix44& u)
{
	CxMatrix44 w;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			w[i][j] = s * u[i][j];
	
	return w;
}



// Multiply a matrix times a column vector
CxVector3d CxMatrix44::MultMatVec(const CxVector3d& v) const
{
	CxVector3d res(0.f, 0.f, 0.f);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			res[i] += m_data[i][j] * v[j];
		res[i] += m_data[i][3];
	}
	double w = m_data[3][0]*v[0] + m_data[3][1]*v[1] +
		       m_data[3][2]*v[2] + m_data[3][3];
	
	return (res/w);
}


// Multiply a row vector times a matrix
CxVector3d CxMatrix44::MultVecMat(const CxVector3d& v) const
{
	CxVector3d res(0.f, 0.f, 0.f);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			res[i] += v[j] * m_data[j][i];
		res[i] += m_data[3][i];
	}
	
	double w = v[0]*m_data[0][3] + v[1]*m_data[1][3] +
		       v[2]*m_data[2][3] + m_data[3][3];
	
	return (res/w);
}


// Multiply a matrix times a column direction vector
CxVector3d CxMatrix44::MultMatDir(const CxVector3d& v) const
{
	CxVector3d res(0.f, 0.f, 0.f);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			res[i] += m_data[i][j] * v[j];
	
	return res;
}


// Multiply a row direction vector times a matrix
CxVector3d CxMatrix44::MultDirMat(const CxVector3d& v) const
{
	CxVector3d res(0.f, 0.f, 0.f);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			res[i] += v[j] * m_data[j][i];
	
	return res;
}

// Left multiply the matrix : mat * this
CxMatrix44 CxMatrix44::MultLeft(const CxMatrix44& mat) const
{
	CxMatrix44 res;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				res[i][j] += mat[i][k] * m_data[k][j];
			
	return res;
}

// Right multiply the matrix : this * mat
CxMatrix44 CxMatrix44::MultRight(const CxMatrix44& mat) const
{
	CxMatrix44 res;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				res[i][j] += m_data[i][k] * mat[k][j];
	
	return res;
}

double CxMatrix44::Det4() const
{
	return (m_data[0][0] * Det3(1, 2, 3, 1, 2, 3)
	      - m_data[0][1] * Det3(1, 2, 3, 0, 2, 3)
		  + m_data[0][2] * Det3(1, 2, 3, 0, 1, 3)
		  - m_data[0][3] * Det3(1, 2, 3, 0, 1, 2));
}


double CxMatrix44::Det3(int r1, int r2, int r3, int c1, int c2, int c3) const
{
	return (m_data[r1][c1] * Det2(r2, r3, c2, c3)
		  - m_data[r1][c2] * Det2(r2, r3, c1, c3)
		  + m_data[r1][c3] * Det2(r2, r3, c1, c2));
}


// Returns the transpose of the 4x4 matrix.
CxMatrix44 CxMatrix44::Transpose() const
{
	return CxMatrix44(m_data[0][0], m_data[1][0], m_data[2][0], m_data[3][0],
		              m_data[0][1], m_data[1][1], m_data[2][1], m_data[3][1],
		              m_data[0][2], m_data[1][2], m_data[2][2], m_data[3][2],
		              m_data[0][3], m_data[1][3], m_data[2][3], m_data[3][3]);
}

//********************************************
// Adjoint
// Returns the adjoint of the 4x4 matrix.
// Adjoint_ij = (-1)^(i+j) * alpha_ji
//  where alpha_ij is the determinant of the 
//  submatrix of A without row i and column j
//********************************************
CxMatrix44 CxMatrix44::Adjoint() const
{
	CxMatrix44 a;
	
	a[0][0] =  Det3(1, 2, 3, 1, 2, 3);
	a[0][1] = -Det3(0, 2, 3, 1, 2, 3);
	a[0][2] =  Det3(0, 1, 3, 1, 2, 3);
	a[0][3] = -Det3(0, 1, 2, 1, 2, 3);
	
	a[1][0] = -Det3(1, 2, 3, 0, 2, 3);
	a[1][1] =  Det3(0, 2, 3, 0, 2, 3);
	a[1][2] = -Det3(0, 1, 3, 0, 2, 3);
	a[1][3] =  Det3(0, 1, 2, 0, 2, 3);
	
	a[2][0] =  Det3(1, 2, 3, 0, 1, 3);
	a[2][1] = -Det3(0, 2, 3, 0, 1, 3);
	a[2][2] =  Det3(0, 1, 3, 0, 1, 3);
	a[2][3] = -Det3(0, 1, 2, 0, 1, 3);
	
	a[3][0] = -Det3(1, 2, 3, 0, 1, 2);
	a[3][1] =  Det3(0, 2, 3, 0, 1, 2);
	a[3][2] = -Det3(0, 1, 3, 0, 1, 2);
	a[3][3] =  Det3(0, 1, 2, 0, 1, 2);
	
	return a;
}

//********************************************
// Inverse
// Returns the inverse of the 4x4 matrix.
// A^-1 = Adjoint(A) / Determinant(A)
//********************************************
CxMatrix44 CxMatrix44::Inverse() const
{
	double det = Determinant();
	CxMatrix44 mat = Adjoint();
	mat /= det;
	return mat;
}

// Returns the identity matrix.
CxMatrix44 CxMatrix44::Identity()
{
	return CxMatrix44(1.f, 0.f, 0.f, 0.f,
		              0.f, 1.f, 0.f, 0.f,
		              0.f, 0.f, 1.f, 0.f,
		              0.f, 0.f, 0.f, 1.f);
}


void CxMatrix44::Trace() const
{
	TRACE("\n");
	TRACE("** Matrix **\n");
	TRACE("Address : %x\n",this);
	TRACE("Data    : (%4g %4g %4g %4g)\n",m_data[0][0],m_data[0][1],m_data[0][2],m_data[0][3]);
	TRACE("        : (%4g %4g %4g %4g)\n",m_data[1][0],m_data[1][1],m_data[1][2],m_data[1][3]);
	TRACE("        : (%4g %4g %4g %4g)\n",m_data[2][0],m_data[2][1],m_data[2][2],m_data[2][3]);
	TRACE("        : (%4g %4g %4g %4g)\n",m_data[3][0],m_data[3][1],m_data[3][2],m_data[3][3]);
}
// ** EOF **
