#pragma once
// ***************************************************************
//  xCurvature   version:  1.0   ¡¤  date: 04/22/2009
//  -------------------------------------------------------------
//  Author : jh.wu@siat.ac.cn 
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "xMesh.h"

class CxCurvature
{
private:
	CxMesh* m_pMesh;

public:

	vector<unsigned>     m_ArrayFlag;
	unsigned             m_nFlagCurr;

	vector<CxVector3d>   m_ArrayPrinDir1;
	vector<CxVector3d>   m_ArrayPrinDir2;
	vector<float>        m_ArrayCurvature1;
	vector<float>        m_ArrayCurvature2;


public:
	CxCurvature(CxMesh* pMesh):m_pMesh(pMesh){}
	virtual ~CxCurvature(){}

	// Curvature
	double CurvatureGauss(const CxVertex3d& v, int nMethod = COMMON )const;
	double CurvatureMean(const CxVertex3d& v)const;

	// smooth, noise..
	void DiffuseNormal(float sigma);
	void Smooth(float sigma);
	void DiffuseCurvature(float sigma);
	void DiffuseDerivativeCurvature(float sigma);
	

	void GradientColorVertex(const vector<float>& arrayCurvature, float sum);
	void GradientColorVertex(/*const */vector<float>& arrayCurvature);
	void ColorCurvature(int nType);

	void HistogramEqualization(vector<float>& arrayCurvature);


	void DrawCurvature();

};

