// xEdge.cpp: implementation of the CxEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xEdge.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxEdge::CxEdge()
{
	m_bFlag = false;
	m_iFace[0] = m_iFace[1] = -1;
	m_iVertex[0] = m_iVertex[1] = -1;

}

CxEdge::CxEdge(int v1, int v2)
{
	m_iVertex[0] = v1;
	m_iVertex[1] = v2;

	m_bFlag = false;
	m_iFace[0] = m_iFace[1] = -1;	
}

CxEdge::CxEdge(int v1, int v2, int f1, int f2)
{
	m_iVertex[0] = v1;
	m_iVertex[1] = v2;
	m_iFace[0] = f1;
	m_iFace[1] = f2;
	m_bFlag = false;

}

CxEdge::CxEdge(const CxEdge& rEdge)
{
	m_iVertex[0] = rEdge.m_iVertex[0];
	m_iVertex[1] = rEdge.m_iVertex[1];
	m_iFace[0] = rEdge.m_iFace[0];
	m_iFace[1] = rEdge.m_iFace[1];
	m_bFlag = rEdge.m_bFlag;
}



CxEdge::~CxEdge()
{

}

CxEdge& CxEdge::operator =(const CxEdge& rhs)
{
	if ( this != &rhs )
		SetEdge(rhs);
	return *this;
}

void CxEdge::SetEdge(const CxEdge& rEdge)
{
	m_iVertex[0] = rEdge.m_iVertex[0];
	m_iVertex[1] = rEdge.m_iVertex[1];
	m_iFace[0] = rEdge.m_iFace[0];
	m_iFace[1] = rEdge.m_iFace[1];
	m_bFlag = rEdge.m_bFlag;
}

int CxEdge::GetVertex(int v)const
{
	if ( m_iVertex[0] == v )
		return m_iVertex[1];
	else
		return m_iVertex[0];
}