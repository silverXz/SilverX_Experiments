// xVertex3d.h: interface for the CxVertex3d class.
// ***************************************************************
//  xVertex3d   version:  1.0   
//  -------------------------------------------------------------
//  Description:  三维顶点类;
//  Author     :  wujh@sia.cn 
//  Created    :  07/18/2006
//  Modified   :
//	----------------------------------------------------------------
//	Copyright (c) 2006 - All Rights Reserved
//
//  Shenyang Institute of Automation, Chinese Academy of Sciences
//  *****************************************************************
//
//  *****************************************************************

#if !defined _XVERTEX3D_H__INCLUDED_
#define _XVERTEX3D_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cassert>   // for assert;
#include <vector>

#include "xVector3d.h"
#include "xColor.h"

typedef std::vector<int>  IntArray;


class CxVertex3d  
{
private:
	
	double       m_Coord[3];     // 顶点的三维坐标;
	int          m_Index;        // 顶点的索引号;
	CxVector3d   m_Normal;	     // 法向量;     
	
	IntArray     m_ArrayNeighborVertex;   // 储存1-邻域顶点;
	IntArray     m_ArrayNeighborFace;     // 储存连接到该顶点的面片;

	CxColor      m_Color;
 
public:
	
	// ctors
	CxVertex3d();
	CxVertex3d(double x, double y, double z);
	CxVertex3d(double x, double y, double z, int index);
	CxVertex3d(const CxVertex3d& rVertex);
	CxVertex3d(const CxVertex3d* pVertex);

	~CxVertex3d() { }

	// data setting
	void Set(const CxVertex3d& rVertex);
	void Set(const CxVertex3d* pVertex);
	void Set(double x, double y, double z);
	void x(double x) { m_Coord[0] = x; }
	void y(double y) { m_Coord[1] = y; }
	void z(double z) { m_Coord[2] = z; }
	
	void SetIndex(int index){ m_Index = index; }
	void SetNormal(double x, double y, double z);
	void SetNormal(const CxVector3d& rNormal);
	void SetNormal(const double normal[3]);
	void Clear();
	void SetColor(unsigned char r, unsigned char g, unsigned char b){m_Color.Set(r,g,b);}
	void SetColor(const CxColor& c) { m_Color.Set(c);}

	// data access
	double* GetVertexArray(void);
	CxVector3d& GetNormalRef() {return m_Normal;}
	double x()const { return m_Coord[0]; }
	double y()const { return m_Coord[1]; }
	double z()const { return m_Coord[2]; }
	int GetIndex()const { return m_Index; }
	const CxVector3d& GetNormal()const { return m_Normal; }
	void GetNormal(double& x, double& y, double& z)const;
	void GetNormal(double normal[3])const;
 //	double* GetNormal()const { m_Normal.GetArray(); }

	const CxColor& GetColor()const { return m_Color;}

	// operator
	const double& operator[](int coord)const; 
          double& operator[](int coord); 
	
	operator const double*() const { return &(m_Coord[0]); }
	operator const double*()       { return &(m_Coord[0]); }
	operator       double*()       { return &(m_Coord[0]); }

	bool operator==(const CxVertex3d& v)const;
	bool operator!=(const CxVertex3d& v)const;
	bool operator< (const CxVertex3d& v)const;
	CxVertex3d& operator+=(const CxVertex3d& rVertex);
	CxVertex3d& operator-=(const CxVertex3d& rVertex);
	CxVertex3d& operator*=(const CxVertex3d& rVertex);
	CxVertex3d& operator*=(const double& coef);
	CxVertex3d& operator/=(const CxVertex3d& rVertex);
	CxVertex3d& operator/=(const double& coef);
		
	void Move(const CxVector3d& rVector, float scale);
	void Move(double dx, double dy, double dz);
	
	// data processing
	// vertex neighbor;
	void ReserveVertexNeighbor(int size)   { m_ArrayNeighborVertex.reserve(size);}
	void AddVertexNeighbor(int index);
	int  NumVertexNeighbor()const     { return (int)m_ArrayNeighborVertex.size();}
	int  GetVertexNeighbor(int i)const{ return m_ArrayNeighborVertex[i];         }
	void ClearVertexNeighbor()        { m_ArrayNeighborVertex.clear();           }
	bool HasVertexNeighbor(int index);
	int  HasVertexNeighbor(int index)const;
	const IntArray& GetVertexNeighborArray() { return m_ArrayNeighborVertex;     }

	// face neighbor;
	void ReserveFaceNeigbhor(int size) { m_ArrayNeighborFace.reserve(size);     }
	void AddFaceNeighbor(int index);
	int  NumFaceNeighbor()const        { return (int)m_ArrayNeighborFace.size();}
	int  GetFaceNeighbor(int i)const   { return m_ArrayNeighborFace[i];         }
	void ClearFaceNeighbor()           { m_ArrayNeighborFace.clear();           }
	bool HasFaceNeighbor(int index);
	const IntArray& GetFaceNeighborArray() { return m_ArrayNeighborFace;        }
	
	friend double DistanceSquared(const CxVertex3d& rV1, const CxVertex3d& rV2);
	friend double Distance(const CxVertex3d& rV1, const CxVertex3d& rV2);
	friend double Area(const CxVertex3d& rV1, const CxVertex3d& rV2, const CxVertex3d& rV3);
	friend double Perimeter(const CxVertex3d& rV1, const CxVertex3d& rV2, const CxVertex3d& rV3);
	friend double CosineMaxAngle(const CxVertex3d& v1, const CxVertex3d& v2, const CxVertex3d& v3);

	void gltDraw()const;
	void gltDraw(const CxColor& c, const double& radius)const;


	// debug;
	void Trace()const;

};

inline CxVertex3d::CxVertex3d()
{
	m_Coord[0] = 0.0;
	m_Coord[1] = 0.0;
	m_Coord[2] = 0.0;
	m_Index = -1;
}

inline CxVertex3d::CxVertex3d(double x, double y, double z)
{
	m_Coord[0] = x;
	m_Coord[1] = y;
	m_Coord[2] = z;
	m_Index    = -1;
}

inline CxVertex3d::CxVertex3d(double x, double y, double z, int index)
{
	m_Coord[0] = x;
	m_Coord[1] = y;
	m_Coord[2] = z;
	m_Index = index;
}

inline CxVertex3d::CxVertex3d(const CxVertex3d& rVertex)
{
	m_Coord[0] = rVertex.m_Coord[0];
	m_Coord[1] = rVertex.m_Coord[1];
	m_Coord[2] = rVertex.m_Coord[2];
	m_Index    = rVertex.m_Index;

	m_Normal = rVertex.m_Normal;
	m_Color = rVertex.m_Color;
 
	unsigned int size = (unsigned int)rVertex.m_ArrayNeighborVertex.size();
	m_ArrayNeighborVertex.resize(size);
	for (unsigned int i=0; i<size; ++i)
		m_ArrayNeighborVertex[i] = rVertex.m_ArrayNeighborVertex[i];
	
	size = (unsigned int)rVertex.m_ArrayNeighborFace.size();
	m_ArrayNeighborFace.resize(size);
	for (unsigned int i=0; i<size; ++i )
		m_ArrayNeighborFace[i] = rVertex.m_ArrayNeighborFace[i];

}

inline CxVertex3d::CxVertex3d(const CxVertex3d* pVertex)
{
	m_Coord[0] = pVertex->m_Coord[0];
	m_Coord[1] = pVertex->m_Coord[1];
	m_Coord[2] = pVertex->m_Coord[2];
	m_Index    = pVertex->m_Index;
}


inline const double& CxVertex3d::operator[](int coord)const 
{ 
	assert( 0 <= coord && coord <= 2 );
	return m_Coord[ coord ];
}

inline double& CxVertex3d::operator[](int coord) 
{ 
	assert( 0 <= coord && coord <= 2 );
	return m_Coord[ coord ];
}

inline void CxVertex3d::Clear()
{
	m_Coord[0] = m_Coord[0] = m_Coord[0] = 0.0;
	m_Index = -1;
}

inline CxVertex3d& CxVertex3d::operator+=(const CxVertex3d& rVertex)
{
	for ( int i=0; i<3; ++i )
		m_Coord[i] += rVertex[i];
	return *this;
}

inline CxVertex3d& CxVertex3d::operator-=(const CxVertex3d& rVertex)
{
	for ( int i=0; i<3; ++i )
		m_Coord[i] -= rVertex[i];
	return *this;
}
inline CxVertex3d& CxVertex3d::operator*=(const CxVertex3d& rVertex)
{
	for ( int i=0; i<3; ++i )
		m_Coord[i] *= rVertex[i];
	return *this;
}

inline CxVertex3d& CxVertex3d::operator*=(const double& coef)
{
	for ( int i=0; i<3; ++i )
		m_Coord[i] *= coef;
	return *this;
}

inline CxVertex3d& CxVertex3d::operator/=(const CxVertex3d& rVertex)
{
	for (int i=0; i<3; ++i )
		m_Coord[i] /= rVertex[i];
	return *this;
}
inline CxVertex3d& CxVertex3d::operator/=(const double& coef)
{
	for ( int i=0; i< 3; ++i )
		m_Coord[i] /= coef;
	return *this;
}	


// Nonmember operators that take two Vecs
static inline const CxVertex3d operator+(const CxVertex3d& v1, const CxVertex3d& v2)
{
	return CxVertex3d(v1) += v2;
}

static inline const CxVertex3d operator-(const CxVertex3d& v1, const CxVertex3d& v2)
{
	return CxVertex3d(v1) -= v2;
}

// Vec/scalar operators
static inline const CxVertex3d operator * (const double &x, const CxVertex3d& v)
{
	CxVertex3d result(v);
	for ( int i=0; i<3; ++i )
		result[i] = x*result[i];
	return result;
}

static inline const CxVertex3d operator*(const CxVertex3d&v, const double& x)
{
	CxVertex3d result(v);
	for ( int i=0; i<3; ++i )
		result[i] = x*result[i];
	return result;
}

static inline const CxVertex3d operator/(const double &x, const CxVertex3d& v)
{
	CxVertex3d result(v);
	for ( int i=0; i< 3; ++i )
		result[i] = x/result[i];
	return result;
}

static inline const CxVertex3d operator/(const CxVertex3d&v, const double& x)
{
	return CxVertex3d(v) /= x;
}


#endif // !defined(_XVERTEX3D_H__INCLUDED_)
