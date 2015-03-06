// xVertex3d.cpp: 
// ***************************************************************
//  xVertex3d   version:  1.0   
//  -------------------------------------------------------------
//  Description: implementation of the CxVertex3d class.
//  Author     :  wujh@sia.cn 
//  Created    :  07/18/2006
//  Modified   :
//  -------------------------------------------------------------
//	Copyright (c) 2006 - All Rights Reserved
//
//  Shenyang Institute of Automation, Chinese Academy of Sciences
//  *****************************************************************
//
//  *****************************************************************

#include "stdafx.h"
#include "xVertex3d.h"
#include "xMesh.h"

#include <cmath>


/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-18 16:11:04
 * Function name	: CxVertex3d::Set
 * Description	    : 
 * Return type		: void 
 * Argument         : double x
 * Argument         : double y
 * Argument         : double z
 *================================================================*/
void CxVertex3d::Set(double x, double y, double z)
{
	m_Coord[0] = x;
	m_Coord[1] = y;
	m_Coord[2] = z;	
}

void CxVertex3d::Set(const CxVertex3d* pVertex)
{
	m_Coord[0] = pVertex->m_Coord[0];
	m_Coord[1] = pVertex->m_Coord[1];
	m_Coord[2] = pVertex->m_Coord[2];
	m_Index    = pVertex->m_Index;
}

void CxVertex3d::Set(const CxVertex3d& rVertex)
{
	m_Coord[0] = rVertex.m_Coord[0];
	m_Coord[1] = rVertex.m_Coord[1];
	m_Coord[2] = rVertex.m_Coord[2];
	m_Index    = rVertex.m_Index;	
}

void CxVertex3d::Trace()const
{
	TRACE("%d: [%10.7f, %10.7f, %10.7f]\n", m_Index, m_Coord[0], m_Coord[1], m_Coord[2]);
}


bool CxVertex3d::operator ==(const CxVertex3d& v)const
{
	return ((m_Coord[0] == v.m_Coord[0]) &&
			(m_Coord[1] == v.m_Coord[1]) &&
		    (m_Coord[2] == v.m_Coord[2]));

}


bool CxVertex3d::operator !=(const CxVertex3d& v)const
{
	return !(*this == v);
}

bool CxVertex3d::operator<(const CxVertex3d& v)const
{
	return ((m_Coord[0] < v.m_Coord[0]) ||
		      ((m_Coord[0] == v.m_Coord[0]) && (m_Coord[1] < v.m_Coord[1])) ||
				((m_Coord[0] == v.m_Coord[0]) && (m_Coord[1] == v.m_Coord[1]) && (m_Coord[2] < v.m_Coord[2])));	
}

void CxVertex3d::AddVertexNeighbor(int index)
{
	if ( m_Index == index )
		return;
	if ( HasVertexNeighbor(index) == false )
		m_ArrayNeighborVertex.push_back(index);
}

bool CxVertex3d::HasVertexNeighbor(int index)
{
	for ( int i=0; i<NumVertexNeighbor(); ++i )
	{
		if ( index == m_ArrayNeighborVertex[i] )
			return true;
	}
	return false;
}

double* CxVertex3d::GetVertexArray()
{
	return m_Coord;
}

int CxVertex3d::HasVertexNeighbor(int index)const
{
	for ( int i=0; i<NumVertexNeighbor(); ++i )
	{
		if ( index == m_ArrayNeighborVertex[i] )
			return i;
	}
	return -1;
}

bool CxVertex3d::HasFaceNeighbor(int index)
{
	for ( int i=0; i<NumFaceNeighbor(); ++i )
	{
		if ( index == m_ArrayNeighborFace[i] )
			return true;
	}
	return false;
}

void CxVertex3d::AddFaceNeighbor(int index)
{
	if ( HasFaceNeighbor(index) == false )
		m_ArrayNeighborFace.push_back(index);
}


void CxVertex3d::SetNormal(double x, double y, double z)
{
	m_Normal.Set(x, y, z);
}

void CxVertex3d::SetNormal(const CxVector3d& rNormal)
{
	m_Normal.Set(rNormal);
}

void CxVertex3d::SetNormal(const double normal[3])
{
	m_Normal.Set(normal);
}

void CxVertex3d::GetNormal(double& x, double& y, double& z)const
{
	m_Normal.Get(x, y, z);
}

void CxVertex3d::GetNormal(double normal[3])const
{
	normal[0] = m_Normal[0];
	normal[1] = m_Normal[1];
	normal[2] = m_Normal[2];
}

void CxVertex3d::Move(double dx, double dy, double dz)
{
	m_Coord[0] += dx;
	m_Coord[1] += dy;
	m_Coord[2] += dz;
}

void CxVertex3d::Move(const CxVector3d& rVector, float scale)
{
	m_Coord[0] += rVector.x()*scale;
	m_Coord[1] += rVector.y()*scale;
	m_Coord[2] += rVector.z()*scale;
}


void CxVertex3d::gltDraw()const
{
	::glPushAttrib(GL_POLYGON_BIT | GL_LIGHTING_BIT);
	::glDisable(GL_LIGHTING);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLUquadricObj* pQuadric = gluNewQuadric();
	::glColor3ub(125, 25, 25);
	::glPushMatrix();
	::glTranslated(m_Coord[0], m_Coord[1], m_Coord[2]);
	::gluQuadricNormals(pQuadric, GLU_SMOOTH);
	::gluSphere(pQuadric, 0.05, 8, 8); 	
	::glPopMatrix();
	::gluDeleteQuadric(pQuadric); 
	::glPopAttrib();

}

void CxVertex3d::gltDraw(const CxColor& c, const double& radius)const
{
	::glPushAttrib(GL_POLYGON_BIT | GL_LIGHTING_BIT);
	::glDisable(GL_LIGHTING);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLUquadricObj* pQuadric = gluNewQuadric();
	//::glColor3ub(125, 25, 25);
	::glColor3ub(c.r(), c.g(), c.b());
	::glPushMatrix();
	::glTranslated(m_Coord[0], m_Coord[1], m_Coord[2]);
	::gluQuadricNormals(pQuadric, GLU_SMOOTH);
	//::gluSphere(pQuadric, 0.05, 8, 8); 
	::gluSphere(pQuadric, radius, 10, 10);
	::glPopMatrix();
	::gluDeleteQuadric(pQuadric); 
	::glPopAttrib();
}

/*==================================================================*/
/*
/* friend functions;
/*
/*==================================================================*/

double DistanceSquared(const CxVertex3d& rV1, const CxVertex3d& rV2)
{
	return ( (rV2.x() - rV1.x())*(rV2.x() - rV1.x()) +
		     (rV2.y() - rV1.y())*(rV2.y() - rV1.y()) + 
			 (rV2.z() - rV1.z())*(rV2.z() - rV1.z()));
}


double Distance(const CxVertex3d& rV1, const CxVertex3d& rV2)
{
	return sqrt(DistanceSquared(rV1, rV2));
}

double Area(const CxVertex3d& rV1, const CxVertex3d& rV2, const CxVertex3d& rV3)
{
	CxVector3d v1(rV2, rV3);
	CxVector3d v2(rV2, rV1);
	CxVector3d v(v1^v2);
	//(v1^v2).Length();
	return .50 * v.Length();
}

double Perimeter(const CxVertex3d& rV1, const CxVertex3d& rV2, const CxVertex3d& rV3)
{
	return Distance(rV1, rV2) + Distance(rV2, rV3) + Distance(rV3, rV1);
}

// Cosine of the maximum angle in triangle (v1,v2,v3)
// ÓàÏÒ¶¨Àí; a^2 = b^2 + c^2 - 2*b*c * cosA;
double CosineMaxAngle(const CxVertex3d& v1, const CxVertex3d& v2, const CxVertex3d& v3)
{
	double a = ::Distance(v2, v3); // v2.dist(v3);
	double b = ::Distance(v3, v1); // v3.dist(v1);
	double c = ::Distance(v1, v2); // v1.dist(v2);
	double A = a * (b*b + c*c - a*a);
	double B = b * (c*c + a*a - b*b);
	double C = c * (a*a + b*b - c*c);
	return 0.5 * ::Min(::Min(A, B), C) / (a*b*c); // min cosine == max angle
}
