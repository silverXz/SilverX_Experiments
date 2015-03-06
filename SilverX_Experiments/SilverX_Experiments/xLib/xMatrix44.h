// ***************************************************************
//  xMatrix44   version:  1.0   ? date: 
//  -------------------------------------------------------------  
//  Description:  implementation of the CxMatrix44 class
//  Author     :  wujh@sia.cn 
//  Created    :  07/17/2006
//  Modified   :
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef _XMATRIX_44_
#define _XMATRIX_44_


#include "xVector3d.h"

typedef double CMat44[4][4];

class CxMatrix44
{
private :
	
	CMat44 m_data;// Data row-major not column-major(OpenGL-Specific);
	
public :
	
	// Constructors
	CxMatrix44() {Clear();}
	CxMatrix44(const CMat44& m);
	CxMatrix44(const double *data);
	CxMatrix44(double a11, double a12, double a13, double a14,
		       double a21, double a22, double a23, double a24,
		       double a31, double a32, double a33, double a34,
		       double a41, double a42, double a43, double a44);
	CxMatrix44(const CxMatrix44 &rMatrix);
	CxMatrix44(const CxMatrix44 *pMatrix);
	virtual ~CxMatrix44() { }
	
	// Debug
	void Trace() const;
	
	// Data setting
	void Clear();
	void MakeIdentity();
	void Set(const CMat44& m);
	void Set(const double *data);
	void Set(const CxMatrix44 &rMatrix);
	void Set(const CxMatrix44 *pMatrix);
	
	void SetTranslate(double tx, double ty, double tz);
	void SetRotate(double ax, double ay, double az, double radAngle);
	void SetScale(double sx, double sy, double sz);
	
	void Set(int i, int j, double data) { m_data[i][j] = data; }
	
	// Make it look like a usual matrix (so you can do m[3][2])
	double*       operator[](int i)       { return &m_data[i][0]; }
	const double* operator[](int i) const { return &m_data[i][0]; }
	double        Get(int i, int j) const { return m_data[i][j];  }
	double*       GetData()               { return &m_data[0][0]; }
	
	// Operators
	CxMatrix44& operator+=(const CxMatrix44& rMatrix);
	CxMatrix44& operator+=(const CxMatrix44* pMatrix);
	CxMatrix44& operator-=(const CxMatrix44& rMatrix);
	CxMatrix44& operator-=(const CxMatrix44* pMatrix);
	CxMatrix44& operator*=(const double d);
	CxMatrix44& operator/=(const double d){ return *this *=(1.f/d); }
	
	// Binary operators
	friend CxMatrix44 operator+ (const CxMatrix44& u, const CxMatrix44& v);
	friend CxMatrix44 operator- (const CxMatrix44& u, const CxMatrix44& v);
	friend CxMatrix44 operator* (const double s,      const CxMatrix44& u);
	friend CxMatrix44 operator* (const CxMatrix44& u, const double s){return s * u; }
	friend CxMatrix44 operator/ (const CxMatrix44& u, const double s){return (1.f/s) * u; }
	friend CxVector3d operator* (const CxMatrix44& m, const CxVector3d& v)
    { return m.MultMatVec(v); }
	friend CxVector3d operator* (const CxVector3d& v, const CxMatrix44& m)
    { return m.MultVecMat(v); }
	friend CxMatrix44 operator* (const CxMatrix44& u, const CxMatrix44& v)
	{ return u.MultLeft(v); }
	
	// Math
	CxVector3d MultMatVec(const CxVector3d& v) const;
	CxVector3d MultVecMat(const CxVector3d& v) const;
	CxVector3d MultMatDir(const CxVector3d& v) const;
	CxVector3d MultDirMat(const CxVector3d& v) const;
	CxMatrix44 MultLeft(const CxMatrix44& mat) const;
	CxMatrix44 MultRight(const CxMatrix44& mat)const;
	
	// Misc
	double Determinant() const { return Det4(); }
	double Det4() const;
	double Det3(int r1, int r2, int r3, int c1, int c2, int c3) const;
	double Det2(int r1, int r2, int c1, int c2) const
	{ return (m_data[r1][c1]*m_data[r2][c2] - m_data[r2][c1]*m_data[r1][c2]); }
	CxMatrix44 Transpose() const;
	CxMatrix44 Adjoint() const;
	CxMatrix44 Inverse() const;
	
	static CxMatrix44  Identity();
};


#endif // _XMATRIX_44_
