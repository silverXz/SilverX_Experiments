// xMeshSmooth.h: interface for the CxMeshSmooth class.
// ***************************************************************
//  xMeshSmooth   version:  1.0   
//  -------------------------------------------------------------
//  Description:  对三角网格模型光顺/噪声去除;
//  Author     :  wujh@sia.cn 
//  Created    :  08/16/2006
//  Modified   :  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if !defined _XMESHSMOOTH_H__INCLUDED_
#define _XMESHSMOOTH_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CxMesh;
class CxMeshSmooth  
{
private:

	CxMesh*  m_pMesh;    // 网格模型;

public:

	// Ctors;
	CxMeshSmooth();
	CxMeshSmooth(CxMesh* pMesh);
	
	// dtor
	virtual ~CxMeshSmooth();

	// data processing;
	void AddGaussianNoise(double sigma);
	void AddPerlinNoise();
	void Noisify(float fNoise);

	void Laplacian(double delta = 0.6307);

	void LambdaSmooth(int nIters); // Taubin, 2001

	// Refs: Semi-Regular Mesh Extration from Volumes. Z.J.Wood, Mathieu.Desbrun.
    void LaplacianTangential();
	// Refs: Bilateral Mesh Denoising. Shachar Fleishman etc.  SIGGRAPH2003
	void Bilateral(double sigmac, double sigmas);

};

#endif // !defined(_XMESHSMOOTH_H__INCLUDED_)
