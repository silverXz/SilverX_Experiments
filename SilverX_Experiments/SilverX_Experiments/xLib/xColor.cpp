// xColor.cpp 
// ***************************************************************
//  xColor   version:  1.0   
//  -------------------------------------------------------------
//  Description:  implementation of the CxColor class.
//  Author     :  wujh@sia.cn 
//  Created    :  07/17/2006
//  Modified   :
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#include "stdafx.h"
#include "xColor.h"







CxColor::CxColor(const COLORREF& cr)
{
	m_Red   = (unsigned char)(  cr & 0x000000ff        );
	m_Green = (unsigned char)(( cr & 0x0000ff00 ) >>  8);
	m_Blue  = (unsigned char)(( cr & 0x00ff0000 ) >> 16);
	m_Alpha = 0;
}

void CxColor::Set(unsigned char red,
				 unsigned char green,
				 unsigned char blue,
				 unsigned char alpha)
{
	m_Red = red;
	m_Green = green;
	m_Blue = blue;
	m_Alpha = alpha;
}

void CxColor::Set(unsigned char red, unsigned char green, unsigned char blue)
{
	m_Red = red;
	m_Green = green;
	m_Blue = blue;
	m_Alpha = 255;
}

void CxColor::Set(unsigned char grey)
{
	m_Red = grey;
	m_Green = grey;
	m_Blue = grey;
	m_Alpha = 255;
}

void CxColor::Set(const CxColor &c)
{
	m_Red = c.r();
	m_Green = c.g();
	m_Blue = c.b();
	m_Alpha = c.a();
}

void CxColor::Set(float red, float green, float blue)
{
	m_Red = (unsigned char)(red * 255.0);
	m_Green = (unsigned char)(green * 255.0);
	m_Blue = (unsigned char)(blue * 255.0);
}

void CxColor::Set(const COLORREF& cr)
{
	m_Red   = (unsigned char)(  cr & 0x000000ff        );
	m_Green = (unsigned char)(( cr & 0x0000ff00 ) >>  8);
	m_Blue  = (unsigned char)(( cr & 0x00ff0000 ) >> 16);
	m_Alpha = 0;
}

bool CxColor::operator==(const CxColor& color)const
{ 
	return (color.r() == m_Red) && (color.g() == m_Green) && (color.b() == m_Blue); 
}
	
bool CxColor::operator!=(const CxColor& color)const 
{ 
	return !( *this == color );
}




void CxColor::Trace() const
{
	TRACE("\n");
	TRACE("** CxColor **\n");
	TRACE("Address           : %x\n", this);
	TRACE("Color(R, G, B, A) : (%d %d %d %d)\n", m_Red, m_Green, m_Blue, m_Alpha);
}