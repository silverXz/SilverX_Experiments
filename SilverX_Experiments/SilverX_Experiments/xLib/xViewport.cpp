// xViewport.cpp: implementation of the CxViewport class.
// ***************************************************************
//  xViewport   version:  1.0   ¡¤  date: 05/08/2008
//  -------------------------------------------------------------
//  Author: 
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "stdafx.h"
#include "xViewport.h"



void CxViewport::Set(const CxViewport& v)
{
	m_SizeX = v.m_SizeX;
	m_SizeY = v.m_SizeY;
	m_OriginX = v.m_OriginX;
	m_OriginY = v.m_OriginY;
}

void CxViewport::Set(const CxViewport *pV)
{
	m_SizeX = pV->m_SizeX;
	m_SizeY = pV->m_SizeY;
	m_OriginX = pV->m_OriginX;
	m_OriginY = pV->m_OriginY;
}


void CxViewport::Trace() const
{
	TRACE("\n");
	TRACE("** Viewport **\n");
	TRACE("Image Origin : (%i %i)\n", m_OriginX, m_OriginY);
	TRACE("Image Size   : (%i %i)\n", m_SizeX,  m_SizeY);
}


void CxViewport::glDraw() const
{
	glViewport(m_OriginX, m_OriginY, m_SizeX, m_SizeY);
}

