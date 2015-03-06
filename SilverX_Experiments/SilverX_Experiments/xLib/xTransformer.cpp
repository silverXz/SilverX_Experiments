#include "stdafx.h"
#include "xTransformer.h"
// ***************************************************************
//  xTransformer   version:  1.0   ¡¤  date: 04/22/2009
//  -------------------------------------------------------------
//  Affine tansform on mesh;
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************



/*=================================================*/
/*
/*     Affine transform, translate, scale, rotate.
/*
/*=================================================*/


// transform the mesh by the given matrix;
void CxTransformer::TransformByGivenMatrix(const xform& xf)
{
	int nv = (int)m_pMesh->m_ArrayVertex.size();
	for (int i = 0; i < nv; i++)
	{
		CxVertex3d& rVertex = m_pMesh->m_ArrayVertex[i];
		rVertex.Set(xf * rVertex);
	}

	// 	if (!m_ArrayNormalVertex.empty()) 
	// 	{
	// 		xform nxf = norm_xf(xf);
	// 		for (int i = 0; i < nv; i++) 
	// 		{
	// 			m_ArrayNormalVertex[i] = nxf * m_ArrayNormalVertex[i];
	// 			m_ArrayNormalVertex[i].normalize();
	// 		}
	// 	}

	xform nxf = norm_xf(xf);
	for (int i=0; i<nv; ++i )
	{
		double x = 0;
		double y = 0;
		double z = 0;
		m_pMesh->m_ArrayVertex[i].GetNormal(x, y, z);
		CxVector3d normal(x, y, z);
		normal = nxf * normal;
		normal.Normalize();
		m_pMesh->m_ArrayVertex[i].SetNormal(normal);
	}

	// boundingbox;
	CxVertex3d maxVertex, minVertex;
	m_pMesh->GetBoundingBox()->Get(maxVertex, minVertex);
	maxVertex.Set(xf*maxVertex);
	minVertex.Set(xf*minVertex);
	m_pMesh->SetBoundingBox(maxVertex, minVertex);
}


// Translate so center of model is at (0,0,0)\n");
void CxTransformer::Center()
{
	CxVertex3d center = CalculateCenterOfMesh();
	TransformByGivenMatrix(xform::trans(-center.x(), -center.y(), -center.z()));
	m_pMesh->SetModifiedFlag(true);
	return;
}

CxVertex3d CxTransformer::CalculateCenterOfMesh()
{
	CxVertex3d com;
	double totwt = 0;

	int nFaces = (int)m_pMesh->m_ArrayFace.size();

	for (int i = 0; i < nFaces; i++) 
	{
		const CxFace& rFace = m_pMesh->m_ArrayFace[i];
		const CxVertex3d &v0 = m_pMesh->m_ArrayVertex[rFace[0]];
		const CxVertex3d &v1 = m_pMesh->m_ArrayVertex[rFace[1]];
		const CxVertex3d &v2 = m_pMesh->m_ArrayVertex[rFace[2]];

		CxVertex3d face_com = (v0+v1+v2) / 3.0;
		double wt = (CxVector3d(v0, v1) ^ CxVector3d(v0, v2)).Length(); // cross product
		com += wt * face_com;
		totwt += wt;
	}
	return com / totwt;
}
// Translate so center of bbox is at (0,0,0);
void CxTransformer::CenterBoundingBox()
{
	m_pMesh->CalculateBoundingBox();
	const CxVertex3d& center = m_pMesh->m_BoundingBox.Center();
	TransformByGivenMatrix(xform::trans(-center.x(), -center.y(), -center.z()));

	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);

	return;
}
// Scale so bbox has maximum extent 1;
void CxTransformer::NormalizeBoundingBox()
{
	m_pMesh->CalculateBoundingBox();

	double ll = ::Max(m_pMesh->m_BoundingBox.xRange(), 
		              m_pMesh->m_BoundingBox.yRange(), 
					  m_pMesh->m_BoundingBox.zRange());

	const CxVertex3d& center = m_pMesh->m_BoundingBox.Center();
	Translate(-center[0], -center[1], -center[2]);

	double s = 1.0 / ll;
	TransformByGivenMatrix(xform::scale(s));
	
	Translate(center[0], center[1], center[2]);

	return;
}

void CxTransformer::Translate(const CxVector3d& transvec)
{
	TransformByGivenMatrix(xform::trans(transvec));
}

// Scale the mesh so that mean squared distance from center of mass is 1
void CxTransformer::NormalizeVariance()
{
	CxVertex3d center = CalculateCenterOfMesh();

	Translate(-CxVector3d(center[0], center[1], center[2]));

	double var = 0;
	int nv = (int)m_pMesh->m_ArrayVertex.size();
	for (int i = 0; i < nv; i++)
	{
		const CxVertex3d& rVertex = m_pMesh->m_ArrayVertex[i];
		var += rVertex[0]*rVertex[0] + rVertex[1]*rVertex[1] + rVertex[2]*rVertex[2];
		
	}

	double s = 1.0f / sqrt(var);
	TransformByGivenMatrix(xform::scale(s));

	Translate(CxVector3d(center[0], center[1], center[2]));

	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);
}

void CxTransformer::Normalize()
{
	m_pMesh->ScaleMesh(1.0);
	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);
}

void CxTransformer::Rotation(double angle, double ax, double ay, double az)
{
	double ang = M_PI/180.0*angle;
	TransformByGivenMatrix(xform::rot(ang, ax, ay, az));

	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);
	return;
}

void CxTransformer::Translate(double tx, double ty, double tz)
{
	TransformByGivenMatrix(xform::trans(tx, ty, tz));
	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);
	return;
}

void CxTransformer::Scale(double s)
{
	TransformByGivenMatrix(xform::scale(s));

	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);
	return;
}

void CxTransformer::Scale(double sx, double sy, double sz)
{
	TransformByGivenMatrix(xform::scale(sx, sy, sz));

	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);
	return;
}

void CxTransformer::Scale(double s, double sx, double sy, double sz)
{
	TransformByGivenMatrix( xform::scale(s, sx, sy, sz) );

	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);
	return;
}

// Rotate model so that first principal axis is along +X (using
// forward weighting), and the second is along +Y
void CxTransformer::PcaRotate()
{
	CxVertex3d com = CalculateCenterOfMesh();
	
	Translate(-CxVector3d(com[0], com[1], com[2]));

	double C[3][3];
	int nv = (int)m_pMesh->m_ArrayVertex.size();
	for (int i = 0; i < nv; i++) 
	{
		const CxVertex3d &p = m_pMesh->m_ArrayVertex[i];
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				C[j][k] += p[j] * p[k];
	}
	double e[3];
	eigdc<3, double>(C, e); // wu;

	// Sorted in order from smallest to largest, so grab third column
	CxVector3d first(C[0][2], C[1][2], C[2][2]);
	int npos = 0;
	for (int i = 0; i < nv; i++)
	{
		CxVector3d vec(m_pMesh->m_ArrayVertex[i][0], 
			           m_pMesh->m_ArrayVertex[i][1], 
					   m_pMesh->m_ArrayVertex[i][2]);
		double result = vec.Dot(first);
		if ( result > 0.0 )
			npos++;
	}

	if (npos < nv/2)
		first = -first;

	CxVector3d second(C[0][1], C[1][1], C[2][1]);
	npos = 0;
	for (int i = 0; i < nv; i++)
	{
		CxVector3d vec(m_pMesh->m_ArrayVertex[i][0], 
			           m_pMesh->m_ArrayVertex[i][1], 
					   m_pMesh->m_ArrayVertex[i][2]);
		double result = vec.Dot(second);
		if ( result > 0.0f )
			npos++;
	}
	if (npos < nv/2)
		second = -second;

	CxVector3d third = first ^ second;

	xform xf;
	xf[0] = first[0];  xf[1] = first[1];  xf[2] = first[2];
	xf[4] = second[0]; xf[5] = second[1]; xf[6] = second[2];
	xf[8] = third[0];  xf[9] = third[1];  xf[10] = third[2];

	invert(xf);
	for (int i = 0; i < nv; i++)
	{
		CxVertex3d& rVertex = m_pMesh->m_ArrayVertex[i];
		rVertex.Set( xf * rVertex );
	}

	Translate(CxVector3d(com[0], com[1], com[2]));

	m_pMesh->SetModifiedFlag(true);

	return;
}