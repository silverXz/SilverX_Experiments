// xCamera.h: interface for the CxCamera class.
// ***************************************************************
//  xCamera   version:  1.0   ¡¤  date: 05/08/2008
//  -------------------------------------------------------------
//  Author: 
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if !defined _XCAMERA_H__INCLUDED_
#define _XCAMERA_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xVector3d.h"
#include "xViewport.h"

#if !defined PI
#define PI 3.1415926535897932
#endif

//#define DEG2RAD(a) ((a)/57.295)




class CxCamera  
{

protected:

	CxVector3d		m_Position;
	double			m_RadAngle;
	CxVector3d		m_RotAxis;

	double			m_HeightAngle;  
	double			m_NearDistance;
	double			m_FarDistance;
	
public :
	
	// Constructor
	CxCamera() { }
	CxCamera(const CxVector3d& rVec) { SetPosition(rVec); }
	CxCamera(CxCamera& rCamera)      { Set(rCamera);      }
	CxCamera(CxCamera* pCamera)      { Set(pCamera);      }
	
	~CxCamera() { }
	
	// Data setting
	void Set(CxCamera& rCamera);
	void Set(CxCamera* pCamera);
	void SetPosition(const CxVector3d& p)         { m_Position = p;        }
	void SetPosition(double x, double y, double z){ m_Position.Set(x,y,z); }
	void SetOrientation(double ax, double ay, double az, double ra) 
	{ m_RotAxis.Set(ax,ay,az); m_RadAngle = ra; }
	void SetHeightAngle(double ha)   { m_HeightAngle = ha;  }
	void SetNearDistance(double nd)  { m_NearDistance = nd; }
	void SetFarDistance(double fd)   { m_FarDistance = fd;  }
	
	// Data access
	CxVector3d GetPosition()        const { return m_Position;     }
	CxVector3d GetOrientationAxis() const { return m_RotAxis;      }
	double GetOrientationAngle()    const { return m_RadAngle;     }
	double GetHeightAngle()         const { return m_HeightAngle;  }
	double GetNearDistance()        const { return m_NearDistance; }
	double GetFarDistance()         const { return m_FarDistance;  }
	
	// Coordinate Vectors
	CxVector3d GetRight();
	CxVector3d GetUp();
	CxVector3d GetToward();
	
	// Ray Casting
	CxVector3d GetRayDirection(int x, int y, CxViewport& vp);
	
	// Viewing
	void ViewAll(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax,
		         CxViewport& vp);
	void ViewAll(const CxVector3d& rBoxMax, const CxVector3d& rBoxMin, const CxViewport& vp);
	
	// OpenGL
	void glDraw(CxViewport& vp) const;
	void glDraw()const;

};




#endif // !defined(_XCAMERA_H__INCLUDED_)
