// xArcball.cpp: implementation of the CxArcball class.
// ***************************************************************
//  xArcball   version:  1.0   ¡¤  date: 05/08/2008
//  -------------------------------------------------------------
//  Author: 
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "stdafx.h"
#include "xArcball.h"

#include <cmath>

void CxArcball::Clear()
{
	m_Center.Set(0.f, 0.f, 0.f);
	m_Radius = 1.f;
	m_Dragging = 0;
	m_Mode = ARCBALL_ROTATE;
	m_Show = 0; // hide by default
	m_QuatTotal.Set(1.f, 0.f, 0.f, 0.f);
	m_QuatCurrent.Set(1.f, 0.f, 0.f, 0.f);
}


void CxArcball::Set(double x, double y, double z, double r /* = 1 */)
{
	m_Center.Set(x, y, z);
	m_Radius = r;
}


void CxArcball::Set(const CxVector3d& rCenter, double r /* = 1 */)
{
	m_Center.Set(rCenter);
	m_Radius = r;
}


void CxArcball::Set(const CxArcball& rArcball)
{
	m_Center.Set(rArcball.m_Center);
	m_Radius = rArcball.m_Radius;
	m_Dragging = rArcball.m_Dragging;
	m_Mode     = rArcball.m_Mode;
	m_Show     = rArcball.m_Show;

	m_VecDown.Set(rArcball.m_VecDown);
	m_QuatCurrent.Set(rArcball.m_QuatCurrent);
	m_QuatTotal.Set(rArcball.m_QuatTotal);

}

void CxArcball::Set(const CxArcball* pArcball)
{
	m_Radius   = pArcball->m_Radius;
	m_Dragging = pArcball->m_Dragging;
	m_Mode     = pArcball->m_Mode;
	m_Show     = pArcball->m_Show;
	
	m_VecDown.Set(pArcball->m_VecDown);
	m_Center.Set(pArcball->m_Center);
	m_QuatCurrent.Set(pArcball->m_QuatCurrent);
	m_QuatTotal.Set(pArcball->m_QuatTotal);
}


void CxArcball::GetMatrix(double *mat)
{
	CxQuaternion q = m_QuatCurrent * m_QuatTotal;
	q.GetMatrix(mat);
	mat[3]  = m_Center[0];
	mat[7]  = m_Center[1];
	mat[11] = m_Center[2];
}

void CxArcball::GetGLModelViewMat(double *mat)
{
	for(int i = 0 ; i < 16 ; ++i)
		mat[i] = ModelViewMat[i];
}

void CxArcball::GetGLProjectionMat(double *mat)
{
	for(int i = 0 ; i < 16 ; ++i)
		mat[i] = ProjectionMat[i];
}

void CxArcball::GetGLViewport(GLint *vp)
{
	for(int i = 0 ; i < 4 ; i ++)
		vp[i] = viewport[i];
}

void CxArcball::GetMatrix(double mat[4][4])
{
	CxQuaternion q = m_QuatCurrent * m_QuatTotal;
	double m[16];
	q.GetMatrix(m);
	m[3]  = m_Center[0];
	m[7]  = m_Center[1];
	m[11] = m_Center[2];
	
	mat[0][0] = m[0];
	mat[0][1] = m[1];
	mat[0][2] = m[2];
	mat[0][3] = m[3];
	
	mat[1][0] = m[4];
	mat[1][1] = m[5];
	mat[1][2] = m[6];
	mat[1][3] = m[7];
	
	mat[2][0] = m[8];
	mat[2][1] = m[9];
	mat[2][2] = m[10];
	mat[2][3] = m[11];
	
	mat[3][0] = m[12];
	mat[3][1] = m[13];
	mat[3][2] = m[14];
	mat[3][3] = m[15];
}


CxMatrix44 CxArcball::GetMatrix()
{
	CxQuaternion q = m_QuatCurrent * m_QuatTotal;
	CxMatrix44 mat = q.GetMatrix();
	mat[0][3] = m_Center[0];
	mat[1][3] = m_Center[1];
	mat[2][3] = m_Center[2];
	return mat;
}


void CxArcball::Trace() const
{
	TRACE("\n");
	TRACE("** Arcball **\n");
	TRACE("Address : %x\n", this);
	TRACE("Center  : (%g %g %g)\n", m_Center.x(), m_Center.y(), m_Center.z());
	TRACE("Radius  : %g\n", m_Radius);
}


void CxArcball::BeginDrag(const CxVector3d& currentVec)
{
	if (m_Mode == ARCBALL_NO_MOTION)
		return;

	m_VecDown  = currentVec;
	m_Dragging = 1;

}

void CxArcball::EndDrag(const CxVector3d& currentVec)
{
	if (!m_Dragging)
		return;

	Motion(currentVec);

	if (m_Mode == ARCBALL_ROTATE)
	{
		m_QuatTotal = m_QuatCurrent * m_QuatTotal;
		m_QuatCurrent.Clear();
	}

	m_VecDown.Clear();
	m_Dragging = 0;

}

void CxArcball::Motion(const CxVector3d& currentVec)
{
	if (!m_Dragging)
		return;
	if (m_Mode == ARCBALL_ROTATE)
	{
		m_QuatCurrent = CxQuaternion(m_VecDown, currentVec);//from para0 to para1
	}
	else if (m_Mode == ARCBALL_TRANSLATE_XY)
	{
		m_Center += currentVec - m_VecDown;
		m_VecDown = currentVec;
	}
	else if (m_Mode == ARCBALL_TRANSLATE_Z)
	{
		double speed = 0.5;
		double zDisp    = (m_VecDown[1] - currentVec[1]) * speed; //?
		double zCurrent = m_Center[2];
		m_Center.z(zCurrent + zDisp);
		m_VecDown = currentVec;
	}
}

int CxArcball::glDraw()
{
	double mat[16];
	CxQuaternion q = m_QuatCurrent * m_QuatTotal;
	q.Conjugate().GetMatrix(mat);
	glTranslated(m_Center[0], m_Center[1], m_Center[2]);
	glMultMatrixd(mat);

	glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewMat);
	glGetDoublev(GL_PROJECTION_MATRIX, ProjectionMat);
	glGetIntegerv(GL_VIEWPORT,viewport);

	if ( m_Show )
	{
		::glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glColor3f(0.75, .5, 0.75f);
		glBegin(GL_POLYGON);
		glVertex3d(-m_Radius, 0     , 0);
		glVertex3d(0     , - m_Radius, 0);
		glVertex3d(m_Radius, 0     , 0);
		glVertex3d(0     , m_Radius, 0);
		glEnd();
		
		float radius = (float)m_Radius;
	//	TRACE(" Arcball radius = %f\n", radius);
		
		float stepsize = .05f;
	   
		//glTranslated(-m_Center[0], -m_Center[1], -m_Center[2]);
		
	//	TRACE("%f %f %f\n", m_Center[0], m_Center[1], m_Center[2]);
		glPushMatrix();	
		glColor3f(1.0, .0f, .0f);
		glBegin(GL_LINE_LOOP);
		for (float angle = 0; angle <=(2.0*PI); angle += stepsize)
			glVertex3f(radius * sin(angle), radius * cos(angle), 0);
		glEnd();

		glColor3f(0.0f, 1.0f, .0);
		glBegin(GL_LINE_LOOP);
		for (float angle = 0; angle <=(2.0*PI); angle += stepsize)
			glVertex3f(radius * sin(angle), 0, radius * cos(angle));
		glEnd();
		
		glColor3f(0.0, .0, 1.0f);
		glBegin(GL_LINE_LOOP);
		for ( float angle = 0; angle <=(2.0*PI); angle += stepsize)
			glVertex3f(0, radius * sin(angle), radius * cos(angle));
		glEnd();
		glPopMatrix();

		//glEnable(GL_LIGHTING);
		glPopAttrib();
	}

	return 1;
}

int CxArcball::glDraw(double rad,bool hide)
{
	double mat[16];
	CxQuaternion q = m_QuatCurrent * m_QuatTotal;
	q.Conjugate().GetMatrix(mat);
	glTranslated(m_Center[0], m_Center[1], m_Center[2]);
	glMultMatrixd(mat);

	glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewMat);
	glGetDoublev(GL_PROJECTION_MATRIX, ProjectionMat);
	glGetIntegerv(GL_VIEWPORT,viewport);

	if ( hide )
	{
		::glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glColor3f(0.75, .5, 0.75f);

		double radius = m_Radius;
		//	TRACE(" Arcball radius = %f\n", radius);

		float stepsize = .05f;

		//glTranslated(-m_Center[0], -m_Center[1], -m_Center[2]);

		//	TRACE("%f %f %f\n", m_Center[0], m_Center[1], m_Center[2]);
		glPushMatrix();	
		glColor3f(1.0, .0f, .0f);
		glBegin(GL_LINE_LOOP);
		for (float angle = 0; angle <=(2.0*PI); angle += stepsize)
			glVertex3d(rad * sin(angle), rad * cos(angle), 0);
		glEnd();

		glColor3f(0.0f, 1.0f, .0);
		glBegin(GL_LINE_LOOP);
		for (float angle = 0; angle <=(2.0*PI); angle += stepsize)
			glVertex3d(rad * sin(angle), 0, rad * cos(angle));
		glEnd();

		glColor3f(0.0, .0, 1.0f);
		glBegin(GL_LINE_LOOP);
		for ( float angle = 0; angle <=(2.0*PI); angle += stepsize)
			glVertex3d(0, rad * sin(angle), rad * cos(angle));
		glEnd();
		glPopMatrix();

		//glEnable(GL_LIGHTING);
		glPopAttrib();
	}

	return 1;
}

int CxArcball::IntersectSphere(const CxVector3d& rayStart, const CxVector3d& rayDir, CxVector3d& result)
{
	CxVector3d q = rayStart - m_Center;
	double a = rayDir.Dot(rayDir);
	double b = 2.f*rayDir.Dot(q);
	double c = q.Dot(q) - m_Radius*m_Radius;
	
	double d = b*b - 4.f*a*c;
	if (d < 0.f)
	{
		IntersectPlane(rayStart, rayDir, result);
		result -= m_Center;
		result.Normalize();
	//	TRACE("mVec   : (%g,%g,%g)\n", result.x(), result.y(), result.z());
		return 0;
	}
	double t1  = (-b + sqrt(d))/ (2.f*a);
	double t2  = (-b - sqrt(d))/ (2.f*a);
	if (t2 < 0.f)
		t2 = t1;
	result = rayStart + t2 * rayDir - m_Center;
	result.Normalize();
//	TRACE("mVec   : (%g,%g,%g)\n", result.x(), result.y(), result.z());
	return 1;
}


int CxArcball::IntersectPlane(const CxVector3d& rayStart, const CxVector3d& rayDir, 
			                  CxVector3d& result, int whichPlane /* = 2 */)
{
	double denom = rayDir[whichPlane];
	if (denom == 0.f)
	{
		result = m_VecDown;
		return 0;
	}
	
	CxVector3d numerVec = m_Center - rayStart;
	double t = (numerVec[whichPlane])/denom;
	result = rayStart + t * rayDir;
	if (t < 0.f)
	{
		result = m_VecDown;
		return 0;
	}
	return 1;
}

int CxArcball::Intersect(const CxVector3d& rayStart, const CxVector3d& rayDir, CxVector3d& result)
{
	if (m_Mode == ARCBALL_ROTATE)
		return IntersectSphere(rayStart, rayDir, result);
	else if (m_Mode == ARCBALL_TRANSLATE_XY)
		return IntersectPlane(rayStart, rayDir, result, 2);
	else if (m_Mode == ARCBALL_TRANSLATE_Z)
		return IntersectPlane(rayStart, rayDir, result, 2);
	return 0;
}


CxVector3d CxArcball::Intersect(int x, int y, CxCamera& theCam, CxViewport& vp)
{
	int xRes = vp.GetWidth();
	int yRes = vp.GetHeight();
	int xc   = (xRes + vp.GetOriginX())/2;
	int yc   = (yRes + vp.GetOriginY())/2;
	double r  = xRes - xc;
	r = (yRes - yc) < r ?(yRes - yc) : r;

	CxVector3d theVec, p;
	double screenWidth;
	if (m_Mode == ARCBALL_ROTATE)
	{
		theVec.Set((x - xc)/r, (y - yc)/r, 0.f);
		double len = theVec.Length();
		if (len >= 1.f)
			theVec /= len;
		else
		{
			theVec.z(sqrt(1.f - len * len));
			theVec.Normalize();
		}
	}
	else if (m_Mode == ARCBALL_TRANSLATE_XY)
	{
		theVec.Set(((double)x)/xRes, ((double)y)/yRes, 0.f);
		p = m_Center - theCam.GetPosition();
		screenWidth = vp.GetAspectRatio() * 2.f
			*tan((theCam.GetHeightAngle()/2.f)* (PI/180.));
		theVec *= p.Dot(theCam.GetToward())*screenWidth;
	}
	else if (m_Mode == ARCBALL_TRANSLATE_Z)
	{
		theVec.Set(0.f, ((double)y)/yRes, 0.f);
		theVec *= .3f* (theCam.GetFarDistance() - theCam.GetNearDistance());
	}
	return theVec;
}