// xFace.cpp: 
// ***************************************************************
//  xFace   version:  1.0   
//  -------------------------------------------------------------
//  Description:  implementation of the CxFace class.
//  Author     :  wujh@sia.cn 
//  Created    :  07/27/2006
//  Modified   :   
//  -------------------------------------------------------------
//	Copyright (c) 2006 - All Rights Reserved
//
//  Shenyang Institute of Automation, Chinese Academy of Sciences
//  *****************************************************************
//
//  *****************************************************************

#include "stdafx.h"
#include "xFace.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxFace::CxFace()
{
	m_nIndex = -1;
	m_nVertex[0] = m_nVertex[1] = m_nVertex[2] = -1;
	m_nFace[0] = m_nFace[1] = m_nFace[2] = -1;
	m_nFlag = 0;
}

CxFace::~CxFace()
{

}

CxFace::CxFace(int v1, int v2, int v3)
{
	Set(v1, v2, v3);
	SetFace(-1, -1, -1);
	SetIndex(-1);
	SetFlag(0);
}

CxFace::CxFace(const CxFace* pFace)
{
	Set(pFace);
}

CxFace::CxFace(const CxFace& rhs)
{
	Set(rhs);
}


CxFace::CxFace(const int* vertex)
{
	m_nVertex[0] = vertex[0];
	m_nVertex[1] = vertex[1];
	m_nVertex[2] = vertex[2];
}

int* CxFace::GetVertexArray()
{
	return m_nVertex;
}

void CxFace::Set(const CxFace* pFace)
{
	m_nVertex[0] = pFace->m_nVertex[0];
	m_nVertex[1] = pFace->m_nVertex[1];
	m_nVertex[2] = pFace->m_nVertex[2];
	m_nIndex = pFace->m_nIndex;
	m_Normal.Set(pFace->m_Normal);
	m_nFlag = pFace->m_nFlag;
	m_Color.Set(pFace->m_Color);
}

void CxFace::Set(const CxFace& rhs)
{
	m_nVertex[0] = rhs.m_nVertex[0];
	m_nVertex[1] = rhs.m_nVertex[1];
	m_nVertex[2] = rhs.m_nVertex[2];
	m_nIndex = rhs.m_nIndex;
	m_Normal = rhs.m_Normal;

	m_nFace[0] = rhs.m_nFace[0];
	m_nFace[1] = rhs.m_nFace[1];
	m_nFace[2] = rhs.m_nFace[2];

	m_nFlag = rhs.m_nFlag;
	m_Color = rhs.m_Color;
}

void CxFace::Set(int v1, int v2, int v3)
{
	m_nVertex[0] = v1;
	m_nVertex[1] = v2;
	m_nVertex[2] = v3;
}

void CxFace::Set(const int* vertex)
{
	m_nVertex[0] = vertex[0];
	m_nVertex[1] = vertex[1];
	m_nVertex[2] = vertex[2];
}

void CxFace::SetNormal(const double* normal)
{
	m_Normal.Set(normal[0], normal[1], normal[2]);
}

void CxFace::GetNormal(double&x, double& y, double& z)const
{
	x = m_Normal.x();
	y = m_Normal.y();
	z = m_Normal.z();
}

void CxFace::SetFace(int f1, int f2, int f3)
{
	m_nFace[0] = f1;
	m_nFace[1] = f2;
	m_nFace[2] = f3;
}

bool CxFace::HasVertex(int index)const
{
	return (m_nVertex[0] == index ||
		    m_nVertex[1] == index ||
			m_nVertex[2] == index);

/*	for (int i=0; i<3; ++i )
		if ( m_nVertex[i] == index )
			return true;
	return false;*/
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-4 16:43:13
 * Function name	: CxFace::GetFaceNeighborInclusive
 * Description	    : 获得拥有顶点v1,v2的邻面;
 * Return type		: int 
 * Argument         : int v1
 * Argument         : int v2
 *================================================================*/
int CxFace::GetFaceNeighborInclusive(int v1, int v2)
{
	return -1;
}

int CxFace::IndexFrom(int v)const
{
	return (m_nVertex[0] == v) ? 0 : (m_nVertex[1] == v) ? 1 : (m_nVertex[2] == v) ? 2 : -1;
}



/*================================================================
 * Author/date      : wujh@sia.cn   2007-6-19 19:09:57
 * Function name	: CxFace::GetVertexExclusive
 * Description	    : 获取该面的第三个顶点索引(除了v1, v2)
 * Return type		: int 
 * Argument         : int v1
 * Argument         : int v2
 *================================================================*/
int CxFace::GetVertexExclusive(int v1, int v2)const
{
	for ( int i=0; i<3; ++i )
		if ( m_nVertex[i] != v1 && m_nVertex[i] != v2 )
			return m_nVertex[i];

	return -1;

}
// 获取该面的另外两个顶点的索引;
int CxFace::GetVertexExclusive(int other[2], int v)const
{
	int n = 0;
	for ( int i=0; i<3; ++i )
	{
		if ( m_nVertex[i] != v )
			other[n++] = m_nVertex[i];
	}
	assert ( n == 2 );
	return 1;
}

bool CxFace::Equal(const CxFace& f)const
{
	if ( (m_nVertex[0] == f[0]) && (m_nVertex[1]== f[1]) 
		&& (m_nVertex[2]==f[2]))
		return true;
	return false;
}


// CosAngle between two faces
// uv = |u||v| cos(u,v)
double CosAngle(const CxFace& f1, const CxFace& f2)
{
	// assert ( f1 != -1 && f2 != -1 );
	const CxVector3d& u = f1.GetNormal();
	const CxVector3d& v = f2.GetNormal();

	double length = u.Length() * v.Length();
	if ( ::IsZero(length) )
		return .0;

	double product = u.Dot(v);

	return product / length;
}

// SinAngle between two faces
// u  ^ v  = w
// |w| = |u||v| |sin(u,v)|
double SinAngle(const CxFace& f1, const CxFace& f2)
{
	const CxVector3d& u = f1.GetNormal();
	const CxVector3d& v = f2.GetNormal();

	double length = u.Length() * v.Length();
	if ( ::IsZero(length) )
		return .0;

	CxVector3d cross = u^v;
	return cross.Length() / length;
}

// Angle between two faces (in radians)
// we use this formula
// uv = |u||v| cos(u,v)
// u  ^ v  = w
// |w| = |u||v| |sin(u,v)|
double Angle(const CxFace& f1, const CxFace& f2)
{
	return ::Angle(f1.GetNormal(), f2.GetNormal());

	const CxVector3d& u = f1.GetNormal();
	const CxVector3d& v = f2.GetNormal();

	return ::Angle(u, v);
}
