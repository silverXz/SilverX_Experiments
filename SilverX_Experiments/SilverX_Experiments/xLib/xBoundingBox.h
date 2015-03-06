// xBoundingBox.h: interface for the CxBoundingBox class.
// ***************************************************************
//  xBoundingBox   version:  1.0   
//  -------------------------------------------------------------
//  Description:  模型的最小包围盒类;
//  Author     :  wujh@sia.cn 
//  Created    :  7/19/2006 22:51:19
//  Modified   :
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if !defined _XBOUNDINGBOX_H__INCLUDED_
#define _XBOUNDINGBOX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "xVertex3d.h"
class CxBoundingBox  
{
private:

	CxVertex3d m_MaxVertex;   // 储存三个最大值的分量;
	CxVertex3d m_MinVertex;   // 储存三个最小值的分量;
 
public:

	CxBoundingBox();
	CxBoundingBox(const CxVertex3d& rMax, const CxVertex3d& rMin);
	CxBoundingBox(const CxBoundingBox& rBox);
	~CxBoundingBox() { }

	// data setting;
	void Set(const CxBoundingBox& rBox);
	void Set(const CxVertex3d& rMax, const CxVertex3d& rMin);
	void SetMaxVertex(const CxVertex3d& rVertex);
	void SetMinVertex(const CxVertex3d& rVertex);

	// data access;
	void Get(CxVertex3d& rMax, CxVertex3d& rMin)const;
	void GetMaxVertex(CxVertex3d& rMax)const;
	void GetMinVertex(CxVertex3d& rMin)const;
	const CxVertex3d& GetMaxVertex()const { return m_MaxVertex;}
	const CxVertex3d& GetMinVertex()const { return m_MinVertex;}

	double xMax()const { return m_MaxVertex.x(); }
	double xMin()const { return m_MinVertex.x(); }
	double yMax()const { return m_MaxVertex.y(); }
	double yMin()const { return m_MinVertex.y(); }
	double zMax()const { return m_MaxVertex.z(); }
	double zMin()const { return m_MinVertex.z(); }
	

	CxVertex3d Center()const;  // 包围盒的中心;
	void Range(double& xRange, double& yRange, double& zRange)const;
    double xRange()const { return abs(xMax() - xMin()); }
	double yRange()const { return abs(yMax() - yMin()); }
	double zRange()const { return abs(zMax() - zMin()); }

	void Expand(const CxBoundingBox& rBox); // 扩展包围盒;
	double Diagonal()const{return ::Distance(m_MaxVertex, m_MinVertex);}

	// debug;
	void Trace()const;

};

#endif // !defined(_XBOUNDINGBOX_H__INCLUDED_)
