/**************************************************************************
	
	File name:	xMeshRepair.h      Version 1.0
---------------------------------------------------------------------------
	Purpose :	ÐÞ²¹¿×¶´
    Author  :   Dr.Wu (jh.wu@siat.ac.cn)
	Created :	2014/08/01   16:02
    Modified:
---------------------------------------------------------------------------
   Copyright (c) 2014 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences
**************************************************************************/
#pragma once

#include "xMesh.h"


class CxMeshRepair
{
protected:
	CxMesh*		m_pMesh;

	std::vector<EdgeArray>   m_BoundaryEdges;

public:

	explicit CxMeshRepair(CxMesh* pMesh):m_pMesh(pMesh){};
	~CxMeshRepair(void);

	int generateBoundaryEdges();
	void fillingHoles();

};

