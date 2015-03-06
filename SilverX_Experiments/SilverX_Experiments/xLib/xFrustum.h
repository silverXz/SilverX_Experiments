// xFrustum.h: interface for the CxFrustum class.
// ***************************************************************
//  xFrustum   version:  1.0   ¡¤  date: 06/26/2008
//  -------------------------------------------------------------
//  Author: jh.wu@siat.ac.cn
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#if !defined _XFRUSTUM_H__INCLUDED_
#define _XFRUSTUM_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_PLANES       6

#include "xPlane.h"


class CxFrustum  
{
private:
	
	CxPlane m_plane[MAX_PLANES];
	enum { RIGHT = 0, LEFT, BOTTOM, TOP, FARP, NEARP};
	
public:

	CxFrustum();
	virtual ~CxFrustum();
	
	void CalculateFrustum(double *md, double *proj);
	bool isPointVisiable(double x, double y, double z);
	bool isSphereVisiable(double x, double y, double z, double radius);
	bool isBoxVisiable(double x, double y, double z, double size);

	void drawLine()const;
	
protected:
	
	void NormalizeFrustum();

};

#endif // !defined(_XFRUSTUM_H__INCLUDED_)
