// xBoundingBox.cpp
// ***************************************************************
//  xBoundingBox   version:  1.0   
//  -------------------------------------------------------------
//  Description:  implementation of the CxBoundingBox class.
//  Author     :  wujh@sia.cn 
//  Created    :  7/19/2006 22:51:19
//  Modified   :
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "stdafx.h"
#include "xBoundingBox.h"




CxBoundingBox::CxBoundingBox():m_MaxVertex(0, 0, 0), m_MinVertex(0, 0, 0)
{

}

CxBoundingBox::CxBoundingBox(const CxVertex3d& rMax, const CxVertex3d& rMin)
{
	m_MaxVertex.Set(rMax);
	m_MinVertex.Set(rMin);
}

CxBoundingBox::CxBoundingBox(const CxBoundingBox& rBox)
{
	m_MaxVertex.Set(rBox.m_MaxVertex);
	m_MinVertex.Set(rBox.m_MinVertex);
}

void CxBoundingBox::Set(const CxBoundingBox& rBox)
{
	rBox.Get(m_MaxVertex, m_MinVertex);
}

void CxBoundingBox::Set(const CxVertex3d& rMax, const CxVertex3d& rMin)
{
	m_MaxVertex.Set(rMax);
	m_MinVertex.Set(rMin);
}

void CxBoundingBox::SetMaxVertex(const CxVertex3d& rVertex)
{
	m_MaxVertex.Set(rVertex);
}

void CxBoundingBox::SetMinVertex(const CxVertex3d& rVertex)
{
	m_MinVertex.Set(rVertex);
}

void CxBoundingBox::Get(CxVertex3d& rMax, CxVertex3d& rMin)const
{
	rMax.Set(m_MaxVertex);
	rMin.Set(m_MinVertex);
}

void CxBoundingBox::GetMaxVertex(CxVertex3d& rMax)const
{
	rMax.Set(m_MaxVertex);
}

void CxBoundingBox::GetMinVertex(CxVertex3d& rMin)const
{
	rMin.Set(m_MinVertex);
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-20 8:34:30
 * Function name	: CxBoundingBox::Range
 * Description	    : 获得包围盒的长,宽,高
 * Return type		: void 
 * Argument         : double& xRange
 * Argument         : double& yRange
 * Argument         : double& zRange
 *================================================================*/
void CxBoundingBox::Range(double& xRange, double& yRange, double& zRange)const
{
	xRange = abs( m_MaxVertex.x() - m_MinVertex.x() );
	yRange = abs( m_MaxVertex.y() - m_MinVertex.y() );
	zRange = abs( m_MaxVertex.z() - m_MinVertex.z() );
}

/*================================================================
 * Author/date      : wujh@sia.cn   2006-7-20 8:36:07
 * Function name	: CxBoundingBox::Center
 * Description	    : 获得包围盒的中心;
 * Return type		: CxVertex3d 
 *================================================================*/
CxVertex3d CxBoundingBox::Center()const
{
	return CxVertex3d( .5*(m_MaxVertex.x() + m_MinVertex.x()),
		               .5*(m_MaxVertex.y() + m_MinVertex.y()),
					   .5*(m_MaxVertex.z() + m_MinVertex.z()));

}

void CxBoundingBox::Expand(const CxBoundingBox& rBox)
{
	CxVertex3d minVertex;
	minVertex[0] = ::Min( this->xMin(), rBox.xMin() );
	minVertex[1] = ::Min( this->yMin(), rBox.yMin() );
	minVertex[2] = ::Min( this->zMin(), rBox.zMin() );

	CxVertex3d maxVertex;
	maxVertex[0] = ::Max( this->xMax(), rBox.xMax() );
	maxVertex[1] = ::Max( this->yMax(), rBox.yMax() );
	maxVertex[2] = ::Max( this->zMax(), rBox.zMax() );
 
	this->Set(maxVertex, minVertex);
}

void CxBoundingBox::Trace()const
{
	TRACE("Max: [%10.7f, %10.7f, %10.7f]\n", m_MaxVertex.x(),m_MaxVertex.y(),m_MaxVertex.z());
	TRACE("Min: [%10.7f, %10.7f, %10.7f]\n", m_MinVertex.x(), m_MinVertex.y(), m_MinVertex.z());
}