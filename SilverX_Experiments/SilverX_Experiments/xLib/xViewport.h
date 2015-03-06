// xViewport.h: interface for the CxViewport class.
// ***************************************************************
//  xViewport   version:  1.0   ¡¤  date: 05/08/2008
//  -------------------------------------------------------------
//  Author: 
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once


class CxViewport  
{

private:

	int m_OriginX;
	int m_OriginY;

	int	m_SizeX;
	int m_SizeY;
	
public :
	
	// Constructors
	CxViewport() { }
	CxViewport(int x, int y)     { m_SizeX = x; m_SizeY = y; }
	CxViewport(const CxViewport &v)  { Set(v);  }
	CxViewport(const CxViewport *pV) { Set(pV); }
	
	virtual ~CxViewport() { }
	
	
	void Clear() { SetSize(640, 480); SetOrigin(0,0); }
	void Set(const CxViewport& v);
	void Set(const CxViewport* pV);
	
	void SetOrigin(int x, int y)    { m_OriginX = x; m_OriginY = y;   }
	void SetSize(int xRes, int yRes){ m_SizeX = xRes; m_SizeY = yRes; }
	

	void SetWidth(int w)       { m_SizeX = w;    }
	void SetHeight(int h)      { m_SizeY = h;    }
	void SetOriginX(int ox)    { m_OriginX = ox; }
	void SetOriginY(int oy)    { m_OriginY = oy; }
	
	// Data access (explicit inline functions)
	void  GetSize(float& x, float& y) { x = (float)m_SizeX; y = (float)m_SizeY; }
	float GetAspectRatio() const      { return (float)m_SizeX / (float)m_SizeY; }
	
	// Per coordinate data access (explicit inline functions)
	int GetWidth()   const { return m_SizeX;   }
	int GetHeight()  const { return m_SizeY;   }
	int GetOriginX() const { return m_OriginX; }
	int GetOriginY() const { return m_OriginY; }
	
	// Debug
	void Trace() const;
	
	// OpenGL
	void glDraw() const;
};


