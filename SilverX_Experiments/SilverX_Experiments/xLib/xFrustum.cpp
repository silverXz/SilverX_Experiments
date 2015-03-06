// xFrustum.cpp: implementation of the CxFrustum class.
// ***************************************************************
//  xFrustum   version:  1.0   ¡¤  date: 06/26/2008
//  -------------------------------------------------------------
//  Author: jh.wu@siat.ac.cn
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// ref:http://www.lighthouse3d.com/opengl/viewfrustum/index.php?clipspaceimp
// ***************************************************************

#include "stdafx.h"
#include "xFrustum.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxFrustum::CxFrustum()
{

}

CxFrustum::~CxFrustum()
{

}

void CxFrustum::CalculateFrustum(double *md, double *proj)
{
	   // Error checking.
	if(md == 0 || proj == 0)
		return;
	
	// Create the clip.
	// clip = md * proj;
	double clip[16] = {0};
	
	clip[0] = md[0] * proj[0] + md[1] * proj[4] + md[2] * proj[8]  + md[3] * proj[12];
	clip[1] = md[0] * proj[1] + md[1] * proj[5] + md[2] * proj[9]  + md[3] * proj[13];
	clip[2] = md[0] * proj[2] + md[1] * proj[6] + md[2] * proj[10] + md[3] * proj[14];
	clip[3] = md[0] * proj[3] + md[1] * proj[7] + md[2] * proj[11] + md[3] * proj[15];
	
	clip[4] = md[4] * proj[0] + md[5] * proj[4] + md[6] * proj[8]  + md[7] * proj[12];
	clip[5] = md[4] * proj[1] + md[5] * proj[5] + md[6] * proj[9]  + md[7] * proj[13];
	clip[6] = md[4] * proj[2] + md[5] * proj[6] + md[6] * proj[10] + md[7] * proj[14];
	clip[7] = md[4] * proj[3] + md[5] * proj[7] + md[6] * proj[11] + md[7] * proj[15];
	
	clip[8]  = md[8] * proj[0] + md[9] * proj[4] + md[10] * proj[8]  + md[11] * proj[12];
	clip[9]  = md[8] * proj[1] + md[9] * proj[5] + md[10] * proj[9]  + md[11] * proj[13];
	clip[10] = md[8] * proj[2] + md[9] * proj[6] + md[10] * proj[10] + md[11] * proj[14];
	clip[11] = md[8] * proj[3] + md[9] * proj[7] + md[10] * proj[11] + md[11] * proj[15];
	
	clip[12] = md[12] * proj[0] + md[13] * proj[4] + md[14] * proj[8]  + md[15] * proj[12];
	clip[13] = md[12] * proj[1] + md[13] * proj[5] + md[14] * proj[9]  + md[15] * proj[13];
	clip[14] = md[12] * proj[2] + md[13] * proj[6] + md[14] * proj[10] + md[15] * proj[14];
	clip[15] = md[12] * proj[3] + md[13] * proj[7] + md[14] * proj[11] + md[15] * proj[15];
	
	
	// Calculate the right side of the frustum.
	m_plane[RIGHT].Set(clip[3]  - clip[0],
		               clip[7]  - clip[4],
					   clip[11] - clip[8],
					   clip[15] - clip[12]);
					   
	// Calculate the left side of the frustum.
	m_plane[LEFT].Set(clip[3]  + clip[0],
		              clip[7]  + clip[4],
		              clip[11] + clip[8],
		              clip[15] + clip[12]);
	
	// Calculate the bottom side of the frustum.
	m_plane[BOTTOM].Set(clip[3]  + clip[1],
		                clip[7]  + clip[5],
		                clip[11] + clip[9],
		                clip[15] + clip[13]);
		
	// Calculate the top side of the frustum.
	m_plane[TOP].Set(clip[3]  - clip[1],
                     clip[7]  - clip[5],
                     clip[11] - clip[9],
                     clip[15] - clip[13]);
	
	// Calculate the far side of the frustum.
	m_plane[FARP].Set(clip[3]  - clip[2],
                      clip[7]  - clip[6],
                      clip[11] - clip[10],
                      clip[15] - clip[14]);

	// Calculate the near side of the frustum.
	m_plane[NEARP].Set(clip[3]  + clip[2],
                       clip[7]  + clip[6],
                       clip[11] + clip[10],
                       clip[15] + clip[14]);
	
	// Normalize the sides of the frustum.
	NormalizeFrustum();
}

void CxFrustum::NormalizeFrustum()
{
	// Loop through each side of the frustum and normalize it.
	for (int i = 0; i < MAX_PLANES; i++)
	{
		double magnitude = m_plane[i].GetNormal().Length();
		m_plane[i].a(m_plane[i].a()/magnitude);
		m_plane[i].b(m_plane[i].b()/magnitude);
		m_plane[i].c(m_plane[i].c()/magnitude);
		m_plane[i].d(m_plane[i].d()/magnitude);
	}
}

bool CxFrustum::isPointVisiable(double x, double y, double z)
{
  // Loop through each side of the frustum and test if the point lies outside any of them.
   for(int i = 0; i < MAX_PLANES; i++)
   {
	   if ( m_plane[i].GetDistance(x, y, z) < 0 )
		   return false;
   }
   
   return true;
}

bool CxFrustum::isSphereVisiable(double x, double y, double z, double radius)
{
	double dist = 0.0;

   // Loop through each side of the frustum and test if the sphere lies outside any of them.
   for (int i = 0; i < MAX_PLANES; ++i)
   {
	   dist = m_plane[i].GetDistance(x, y, z);
	   if ( dist < -radius )
		   return false;
   }

   return true;
}

bool CxFrustum::isBoxVisiable(double x, double y, double z, double size)
{
   // Calculate the bounding box.
   double minX = x - size; 
   double maxX = x + size;
   double minY = y - size; 
   double maxY = y + size;
   double minZ = z - size; 
   double maxZ = z + size;
   
   // Loop through each side of the frustum and test if the box lies outside any of them.
   for(int i = 0; i < MAX_PLANES; i++)
   {
	   if ( m_plane[i].GetDistance(minX, minY, minZ) > 0 )
		   continue;
	   if ( m_plane[i].GetDistance(maxX, minY, minZ) > 0 )
		   continue;
	   if ( m_plane[i].GetDistance(minX, maxY, minZ) > 0 )
		   continue;
	   if ( m_plane[i].GetDistance(maxX, maxY, minZ) > 0 )
		   continue;
	   if ( m_plane[i].GetDistance(minX, minY, maxZ) > 0 )
		   continue;
	   if ( m_plane[i].GetDistance(maxX, minY, maxZ) > 0 )
		   continue;
	   if ( m_plane[i].GetDistance(minX, maxY, maxZ) > 0 )
		   continue;
	   if ( m_plane[i].GetDistance(maxX, maxY, maxZ) > 0 )
		   continue;

	   return false;
   }
   
   return true;
}

void CxFrustum::drawLine()const
{
	CxVertex3d nearTopLeft; // top left vertex of near plane;
	CxVertex3d nearTopRight;
	CxVertex3d nearBottomLeft;
	CxVertex3d nearBottomRight;

	CxVertex3d farTopLeft;
	CxVertex3d farTopRight;
	CxVertex3d farBottomLeft;
	CxVertex3d farBottomRight;

	Intersect(nearTopLeft, m_plane[NEARP], m_plane[TOP], m_plane[LEFT]);
	Intersect(nearTopRight, m_plane[NEARP], m_plane[TOP], m_plane[RIGHT]);
	Intersect(nearBottomLeft, m_plane[NEARP], m_plane[BOTTOM], m_plane[LEFT]);
	Intersect(nearBottomRight, m_plane[NEARP], m_plane[BOTTOM], m_plane[RIGHT]);

	Intersect(farTopLeft, m_plane[FARP], m_plane[TOP], m_plane[LEFT]);
	Intersect(farTopRight, m_plane[FARP], m_plane[TOP], m_plane[RIGHT]);
	Intersect(farBottomLeft, m_plane[FARP], m_plane[BOTTOM], m_plane[LEFT]);
	Intersect(farBottomRight, m_plane[FARP], m_plane[BOTTOM], m_plane[RIGHT]);

	glColor3f(1.0f, .0, .0); // red pen;
	glBegin(GL_LINE_LOOP);   //near plane
		glVertex3d(nearTopLeft.x(), nearTopLeft.y(), nearTopLeft.z());
		glVertex3d(nearTopRight.x(), nearTopRight.y(), nearTopRight.z());
		glVertex3d(nearBottomRight.x(), nearBottomRight.y(), nearBottomRight.z());
		glVertex3d(nearBottomLeft.x(), nearBottomLeft.y(), nearBottomLeft.z());
	glEnd();

	glColor3f(.0, .0f, 1.0f);  // blue pen;
	glBegin(GL_LINE_LOOP);     // far plane
		glVertex3dv(farTopLeft);
		glVertex3dv(farTopRight);
		glVertex3dv(farBottomRight);
		glVertex3dv(farBottomLeft);
	glEnd();

	glColor3f(0.f, 1.0f, .0f); 
	glBegin(GL_LINE_LOOP);   // bottom plane
		glVertex3dv(nearBottomLeft);
		glVertex3dv(nearBottomRight);
		glVertex3dv(farBottomRight);
		glVertex3dv(farBottomLeft);
	glEnd();

	glBegin(GL_LINE_LOOP);	 // top plane
		glVertex3dv(nearTopLeft);
		glVertex3dv(nearTopRight);
		glVertex3dv(farTopRight);
		glVertex3dv(farTopLeft);	
	glEnd();

#if 0
	glBegin(GL_LINE_LOOP); // left plane
		glVertex3dv(nearTopLeft);
		glVertex3dv(nearBottomLeft);
		glVertex3dv(farBottomLeft);
		glVertex3dv(farTopLeft);
	glEnd();

	glBegin(GL_LINE_LOOP); // right plane
		glVertex3dv(nearTopRight);
		glVertex3dv(nearBottomRight);
		glVertex3dv(farBottomRight);
		glVertex3dv(farTopRight);
	glEnd();
#endif

}