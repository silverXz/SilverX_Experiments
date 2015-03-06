// xVector3d.h
// ***************************************************************
//  xVector3d   version:  1.0   ? 
//  -------------------------------------------------------------
//  Description:  3D Vector to represent directions.
//  Author     :  wujh@sia.cn 
//  Created    :  07/17/2006
//  Modified   :
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef _XVECTOR_3D_
#define _XVECTOR_3D_

#include "xUtil.h"

class CxVertex3d;
class CxVector3d
{
private :
	
	double vec[3];  // 储存3维分量(x,y,z)
	
public :
	
	// Constructors
	CxVector3d() { Clear(); }
	CxVector3d(double x, double y, double z);
	CxVector3d(const double v[3]);
	CxVector3d(const CxVector3d &rVector)                { Set(rVector); }
	CxVector3d(const CxVector3d *pVector)                { Set(pVector); }
	CxVector3d(const CxVector3d &a, const CxVector3d& b) { Set(b - a);   }
	CxVector3d(const CxVector3d *a, const CxVector3d *b) { Set(*b - *a); }
	CxVector3d(const CxVertex3d& rV1, const CxVertex3d& rV2);
	
	
	~CxVector3d() { }
	
	
	// Data setting
	void Clear() { vec[0] = 0.f; vec[1] = 0.f;	vec[2] = 0.f; }
	void Set(const CxVector3d *pVector)    { Set(pVector->GetArray()); }
	void Set(const CxVector3d& v)          { Set(v.GetArray());        }
	void Set(double x, double y, double z) { vec[0] = x; vec[1] = y; vec[2] = z; }
	void Set(const double v[3])   { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }
	void Set(const CxVector3d& a, const CxVector3d& b) { Set(b - a);   }
	void Set(const CxVector3d *a, const CxVector3d *b) { Set(*b - *a); }
	
	// Data Access
	const double* GetArray() const { return vec; }
	void  Get(double& x, double& y, double& z) const;

	operator const double*() const { return &(vec[0]); }
	operator const double*()       { return &(vec[0]); }
	operator       double*()       { return &(vec[0]); }

	void x(double x) { vec[0] = x;      }
	void y(double y) { vec[1] = y;      }
	void z(double z) { vec[2] = z;      }
	double x() const { return (vec[0]); }
	double y() const { return (vec[1]); }
	double z() const { return (vec[2]); }
	

	double* GetArrayPtr()  {return vec;}
	// Data access using indices
	double&       operator[](int i)       { return (vec[i]); }
	const double& operator[](int i) const { return (vec[i]); }
	
	// Operators
	CxVector3d& operator+=(const CxVector3d& rVector);
	CxVector3d& operator+=(const CxVector3d* pVector);
	CxVector3d& operator-=(const CxVector3d& rVector);
	CxVector3d& operator-=(const CxVector3d* pVector);
	CxVector3d& operator*=(double d);
	CxVector3d& operator/=(double d) { return *this *=(1.f/d); }
	
	CxVector3d  operator-() const;
	
	// Binary operators
	friend CxVector3d operator+ (const CxVector3d& u, const CxVector3d& v);
	friend CxVector3d operator- (const CxVector3d& u, const CxVector3d& v);
	friend CxVector3d operator* (double s,            const CxVector3d& u);
	friend CxVector3d operator* (const CxVector3d& u, double s) { return s * u;       }
	friend CxVector3d operator/ (const CxVector3d& u, double s) { return (1.f/s) * u; }
	friend CxVector3d operator^ (const CxVector3d& u,  const CxVector3d& v); // cross product;
	friend int       operator== (const CxVector3d& v1, const CxVector3d& v2);
	friend int       operator!= (const CxVector3d& v1, const CxVector3d& v2)
    { return !(v1 == v2); }
	
	int Equals(const CxVector3d& v, double tolerence) const;
	
	double     Dot(const CxVector3d& v)   const;
	double     Dot(const CxVector3d* pV)  const;
	CxVector3d Cross(const CxVector3d& v) const;
	CxVector3d Cross(const CxVector3d* pV)const;
	
	// Misc
	double Normalize();
	double Normalize(double value);
	double Length() const;
	double LengthSquared() const;
	int  IsCollinear(CxVector3d *pVector) const;
	int  IsCollinear(CxVector3d &rVector) const;
	void Negate();
	CxVector3d Rotate(double angle, const CxVector3d& Around)const; 
	CxVector3d Projection(const CxVector3d* pV) const;

	double GetLengthXY()const; // 在XY平面投影上的长度;
	double GetLengthXZ()const;
	double GetLengthYZ()const;

	friend double Angle(const CxVector3d& v1, const CxVector3d& v2);
	friend double CosAngle(const CxVector3d& v1, const CxVector3d& v2);
	friend double SinAngle(const CxVector3d& v1, const CxVector3d& v2);
	friend CxVector3d getTransformedVector(const double* m,const CxVector3d &v);

	void Trace()const;

};

inline CxVector3d::CxVector3d(double x, double y, double z)
{
	vec[0] = x; vec[1] = y; vec[2] = z; 
}

inline CxVector3d::CxVector3d(const double v[3])
{
	vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; 
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-17 8:48:56
 * Function name	: CxVector3d::Dot
 * Description	    : return the dot product of this vector and v
 * Return type		: double 
 * Argument         : const CxVector3d& v
 *================================================================*/
inline double CxVector3d::Dot(const CxVector3d& v) const
{
	return (x() * v.x() + y() * v.y() + z() * v.z());
}

inline double CxVector3d::Dot(const CxVector3d* pV) const
{
	return (x() * pV->x() + y() * pV->y() + z() * pV->z());
}



//求t在由x,y所确定平面内的投影
inline CxVector3d Projection(CxVector3d t,CxVector3d x,CxVector3d y)
{
	CxVector3d tmp = x.Cross(y);
	return CxVector3d(t.Projection(&tmp));
}


//计算target向量在vecX和vecY组成的直角坐标系中与vecY轴的夹角，范围从[-PI,PI]
inline double getAccurateAngle(CxVector3d target,CxVector3d vecX,CxVector3d vecY)
{
	if (target.Dot(vecX) > 0) return safeAcos(target.Dot(vecY) / (target.Length() * vecY.Length()));
	else return -safeAcos(target.Dot(vecY) / (target.Length() * vecY.Length()));
}





#endif // _XVECTOR_3D_
