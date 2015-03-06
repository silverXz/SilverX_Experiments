// xArcball.h: interface for the CxArcball class.
// ***************************************************************
//  xArcball   version:  1.0   ¡¤  date: 05/08/2008
//  -------------------------------------------------------------
//  Author: 
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if !defined _XARCBALL_H__INCLUDED_
#define _XARCBALL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ARCBALL_NO_MOTION    0
#define ARCBALL_ROTATE       1
#define ARCBALL_TRANSLATE_XY 2
#define ARCBALL_TRANSLATE_Z  3

#include "xMatrix44.h"
#include "xViewport.h"
#include "xQuaternion.h"
#include "xVector3d.h"
#include "xCamera.h"


class CxArcball  
{
private :

	CxQuaternion m_QuatTotal;
	CxQuaternion m_QuatCurrent;

	CxVector3d   m_VecDown;
	CxVector3d   m_Center;

	double ModelViewMat[16];
	double ProjectionMat[16];
	GLint viewport[4];

	double       m_Radius;
	int          m_Dragging;
	int          m_Mode;
	int          m_Show;
	
public :
	
	// Constructors
	CxArcball() { Clear(); }
	CxArcball(double x, double y, double z, double r = 1.0f){ Clear(); Set(x, y, z, r); }
	CxArcball(CxVector3d center, const double r = 1.0f)     { Clear(); Set(center,r);   }
	CxArcball(CxArcball& rArcball)  { Set(rArcball); }
	CxArcball(CxArcball *pArcball)  { Set(pArcball); }
	
	virtual ~CxArcball() { }
	
	// Data setting
	void Clear();
	void Set(double x, double y, double z, double r = 1.0f);
	void Set(const CxVector3d& rCenter, double r = 1.0f);
	void Set(const CxArcball &rArcball);
	void Set(const CxArcball *pArcball);
	
	void SetMode(int mode) { m_Mode = mode; }
	int  GetMode()         { return m_Mode; }
	void Show()		       { m_Show = 1;    }
	void Hide()		       { m_Show = 0;    }
	
	void SetCenter(double x, double y, double z){ m_Center.Set(x,y,z);   }
	void SetCenter(const CxVector3d& rCenter)   { m_Center.Set(rCenter); }
	void SetRadius(const double r) { m_Radius = r; }
	
	CxVector3d GetCenter() const { return m_Center; }
	double GetRadius()     const { return m_Radius; }


	CxQuaternion GetQuatCurrent()const { return m_QuatCurrent;}
	CxQuaternion GetQuatTotal()const   { return m_QuatTotal;  }
	
	void GetMatrix(double *mat);
	void GetGLModelViewMat(double *mat);
	void GetGLProjectionMat(double *mat);
	void GetGLViewport(GLint *vp);
	void GetMatrix(double mat[4][4]);
	CxMatrix44 GetMatrix();
	
	// Debug
	void Trace() const;
	
	void BeginDrag(const CxVector3d& currentVec);
	void EndDrag(const CxVector3d& currentVec);
	void Motion(const CxVector3d& currentVec);
	int  glDraw();
	int glDraw(double rad,bool hide);
	
	int IntersectSphere(const CxVector3d& rayStart, const CxVector3d& rayDir, CxVector3d& result);
	int IntersectPlane(const CxVector3d& rayStart, const CxVector3d& rayDir, CxVector3d& result, int whichPlane = 2);
	int Intersect(const CxVector3d& rayStart, const CxVector3d& rayDir, CxVector3d& result);
	CxVector3d Intersect(int x, int y, CxCamera& theCam, CxViewport& vp);

};

#endif // !defined(_XARCBALL_H__INCLUDED_)
