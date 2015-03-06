// xEulerAngles.h: interface for the CxEulerAngles class.
// ***************************************************************
//  xEulerAngles   version:  1.0   ¡¤  date: 05/07/2008
//  -------------------------------------------------------------
//  Author: jh.wu@siat.ac.cn
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#if !defined _XEULERANGLES_H__INCLUDED_
#define _XEULERANGLES_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CxEulerAngles  
{
private:

	double   m_heading;
	double   m_pitch;
	double   m_bank;

public:
	CxEulerAngles();
	virtual ~CxEulerAngles();

};

#endif // !defined(_XEULERANGLES_H__INCLUDED_)
