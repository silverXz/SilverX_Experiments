// xCamera.cpp: implementation of the CxCamera class.
// ***************************************************************
//  xCamera   version:  1.0   ¡¤  date: 05/08/2008
//  -------------------------------------------------------------
//  Author: 
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#include "stdafx.h"
#include "xCamera.h"
#include "xMatrix44.h"

#include <cmath>

#include "xPlane.h"


void CxCamera::Set(CxCamera& c)
{
	m_Position     = c.m_Position;
	m_RadAngle     = c.m_RadAngle;
	m_RotAxis      = c.m_RotAxis;
	m_HeightAngle  = c. m_HeightAngle;
	m_NearDistance = c.m_NearDistance;
	m_FarDistance  = c.m_FarDistance;
	
}


void CxCamera::Set(CxCamera *pC)
{
	m_Position     = pC->m_Position;
	m_RadAngle     = pC->m_RadAngle;
	m_RotAxis      = pC->m_RotAxis;
	m_HeightAngle  = pC->m_HeightAngle;
	m_NearDistance = pC->m_NearDistance;
	m_FarDistance  = pC->m_FarDistance;
}


CxVector3d CxCamera::GetRight()
{
	CxMatrix44 rotMat;
	rotMat.SetRotate(m_RotAxis[0], m_RotAxis[1], m_RotAxis[2], m_RadAngle);
	return CxVector3d(rotMat[0][0], rotMat[1][0], rotMat[2][0]);
}


CxVector3d CxCamera::GetUp()
{
	CxMatrix44 rotMat;
	rotMat.SetRotate(m_RotAxis[0], m_RotAxis[1], m_RotAxis[2], m_RadAngle);
	return CxVector3d(rotMat[0][1], rotMat[1][1], rotMat[2][1]);
}


CxVector3d CxCamera::GetToward()
{
	CxMatrix44 rotMat;
	rotMat.SetRotate(m_RotAxis[0], m_RotAxis[1], m_RotAxis[2], m_RadAngle);
	return CxVector3d(-rotMat[0][2], -rotMat[1][2], -rotMat[2][2]);
}



//********************************************
//  GetRayDirection
//  Returns the ray direction given a point 
//  on the image plane.
//********************************************
CxVector3d CxCamera::GetRayDirection(int x, int y, CxViewport& vp)
{
	int xRes = vp.GetWidth();
	int yRes = vp.GetHeight();
	double aspectRatio = vp.GetAspectRatio();
	
	double h = 2.f * m_NearDistance * tan((m_HeightAngle/2.f)*(PI/180.));
	double w = aspectRatio * h;
	
	double rx = (((double)x)/((double)(xRes-1))-0.5f) * w;
	double ry = (((double)y)/((double)(yRes-1))-0.5f) * h;
	double rz = -m_NearDistance;
	
	CxVector3d rayDir(rx, ry, rz);
	CxMatrix44 rotMat;
	rotMat.SetRotate(m_RotAxis[0], m_RotAxis[1], m_RotAxis[2], m_RadAngle);
	return (rotMat*rayDir);
}


//  Sets the camera to view the bounding box.
void CxCamera::ViewAll(double xMin, double xMax, 
					   double yMin, double yMax, 
					   double zMin, double zMax,
				       CxViewport& vp)
{
	// Get bounding sphere of bounding box
	CxVector3d boxMin(xMin, yMin, zMin);
	CxVector3d boxMax(xMax, yMax, zMax);
	CxVector3d center = 0.5f * (boxMin + boxMax);
	double radius = (boxMax - center).Length();
	
	// Get the distance required to fit the object in view
	double hOverD = tan((m_HeightAngle/2.f)* (PI/180.));
	double aspectRatio = vp.GetAspectRatio();
	if (aspectRatio < 1.f)
		hOverD *= aspectRatio;
	
	// distance of eyes to center;
	double distToCenter = radius / hOverD;
	
	// Set the internal variables appropriately
	CxVector3d toward = GetToward();
	m_Position = center - distToCenter * toward;
	//   m_NearDistance = distToCenter - radius;
	//   m_FarDistance  = distToCenter + radius;
	m_NearDistance = 0.1;
	m_FarDistance  = 1000.0;
	
	
}

void CxCamera::ViewAll(const CxVector3d& rBoxMax, const CxVector3d& rBoxMin, const CxViewport& vp)
{
	// Get bounding sphere of bounding box
	CxVector3d center = 0.5f * (rBoxMin + rBoxMax);
	double radius = (rBoxMax - center).Length();
	
	// Get the distance required to fit the object in view
	double hOverD = tan((m_HeightAngle/2.f)* (PI/180.));
	double aspectRatio = vp.GetAspectRatio();
	if (aspectRatio < 1.f)
		hOverD *= aspectRatio;
	
	// distance of eyes to center;
	double distToCenter = radius / hOverD;
	
	// Set the internal variables appropriately
	CxVector3d toward = GetToward();
	m_Position = center - distToCenter * toward;
	//   m_NearDistance = distToCenter - radius;
	//   m_FarDistance  = distToCenter + radius;
	m_NearDistance = 0.1;
	m_FarDistance  = 1000.0;
	
}


void CxCamera::glDraw(CxViewport& vp) const
{
	double aspectRatio = vp.GetAspectRatio();
	// Position and Orientation
	glMatrixMode(GL_MODELVIEW);
	glRotated(m_RadAngle* (360.f/PI), m_RotAxis[0], m_RotAxis[1], m_RotAxis[2]);
	glTranslated(-m_Position.x(), - m_Position.y(), - m_Position.z());

	// m_Position.Trace();
		
	// Projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_HeightAngle, aspectRatio, m_NearDistance, m_FarDistance);
	glMatrixMode(GL_MODELVIEW);
}

void CxCamera::glDraw()const
{

}