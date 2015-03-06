// xEdge.h: interface for the CxEdge class.
//
//////////////////////////////////////////////////////////////////////
// ***************************************************************
//  xEdge   version:  1.0   
//  -------------------------------------------------------------
//  Description:  三角形边类;
//  Author     :  wujh@sia.cn 
//  Created    :  06/18/2007
//  Modified   : 
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if !defined _XEDGE_H__INCLUDED_
#define _XEDGE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CxEdge  
{
private:

	int  m_iVertex[2];   //  构成该边的两端点的索引号;
	int  m_iFace[2];     //  共享该边的两个面片的索引, 如果存在的话;
	bool m_bFlag;        //  boundary edge or not;

public:
	
	// ctor & dtor
	CxEdge();
	CxEdge(int v1, int v2);
	CxEdge(int v1, int v2, int f1, int f2);
	CxEdge(const CxEdge& rEdge);
	virtual ~CxEdge();

	CxEdge& operator=(const CxEdge& rhs);

	// data setting
	void SetEdge(const CxEdge& rEdge);
	void SetFace(int i, int f)     { m_iFace[i%2] = f; }
	void SetVertex(int i, int v)   { m_iVertex[i%2] = v; }
    void v1(int v1) { m_iVertex[0] = v1; }
	void v2(int v2) { m_iVertex[1] = v2; }
	void f1(int f1) { m_iFace[0]   = f1; }
	void f2(int f2) { m_iFace[1]   = f2; }
	void SetFlag(bool bFlag) { m_bFlag = bFlag; }

	// data accessing
    void GetVertex(int& v1, int& v2)const{ v1 = m_iVertex[0]; v2 = m_iVertex[1]; }
	void GetFace(int& f1, int& f2)const  { f1 = m_iFace[0];   f2 = m_iFace[1];   }
	int  v1()const { return m_iVertex[0]; }
	int  v2()const { return m_iVertex[1]; }
	int  f1()const { return m_iFace[0]  ; }
	int  f2()const { return m_iFace[1]  ; }
	bool GetFlag()const { return m_bFlag; }
	bool IsOnBoundary()const { return m_bFlag ? true : false; }
	
	//
	bool HasVertex(int v)const { return (m_iVertex[0] == v || m_iVertex[1] == v ); }
	int  GetVertex(int v)const;  // 返回边的另一个端点;

	// 
	void Trace() { TRACE("(%d , %d)\n", m_iVertex[0], m_iVertex[1]); }
	
};

#endif // !defined(_XEDGE_H__INCLUDED_)
