#pragma once
// ***************************************************************
//  xTransformer   version:  1.0   ¡¤  date: 04/22/2009
//  -------------------------------------------------------------
//  Affine tansform on mesh;
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "xMesh.h"
#include "xForm.h"

class CxTransformer
{

private:

	CxMesh* m_pMesh;

public:
	CxTransformer(CxMesh* pMesh):m_pMesh(pMesh){}
	virtual~CxTransformer(){}


	// translate, scale, rotation...
	void TransformByGivenMatrix(const xform& xf);
	CxVertex3d CalculateCenterOfMesh();
	void Center();  // Translate so center of model is at (0,0,0);
	void CenterBoundingBox(); // Translate so center of bbox is at (0,0,0);
	void NormalizeVariance(); // Scale so variance (RMS distance) from center is 1;
	void NormalizeBoundingBox(); // Scale so bbox has maximum extent 1
	void PcaRotate();  // Rotate so that principal axes lie along X, Y, Z;
	void Rotation(double angle, double ax, double ay, double az);
	void Translate(double tx, double ty, double tz);	
	void Translate(const CxVector3d& transvec);
	void Scale(double s);   // uniform scale by s;
	void Scale(double sx, double sy, double sz);  // scale in the direction (x,y,z);
	void Scale(double s, double sx, double sy, double sz);

	void Normalize();

};