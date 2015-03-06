// xMaterial.h: interface for the CxMaterial class.
// ***************************************************************
//  xMaterial   version:  1.0   ¡¤  date: 05/08/2008
//  -------------------------------------------------------------
//  Author: jh.wu@siat.ac.cn
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if !defined _XMATERIAL_H__INCLUDED_
#define _XMATERIAL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CxMaterial  
{
private :

	float m_pAmbient[4];
	float m_pDiffuse[4];
	float m_pSpecular[4];
	float m_pEmission[4];
	float m_pShininess[1];


public :

	// Constructor
	CxMaterial(); 
	~CxMaterial() {}

	// Data access
	float *GetAmbient() { return m_pAmbient; }
	float *GetDiffuse() { return m_pDiffuse; }
	float *GetSpecular() { return m_pSpecular; }
	float *GetEmission() { return m_pEmission; }
	float *GetShininess() { return m_pShininess;}

	// Data setting
	void SetAmbient(float r, float g, float b, float a);
	void SetDiffuse(float r, float g, float b, float a);
	void SetSpecular(float r,float g, float b, float a);
	void SetEmission(float r,float g, float b, float a);
	void SetShininess(float shininess);

	// Datas
	void Copy(CxMaterial *pMaterial);
  
	// common material
	static CxMaterial Aluminium();
	static CxMaterial Brass();
	static CxMaterial Bronze();
	static CxMaterial PolishedBronze();
	static CxMaterial Chrome();
	static CxMaterial Copper();
	static CxMaterial PolishedCopper();
	static CxMaterial Emerald();
	static CxMaterial Gold();
	static CxMaterial PolishedGold();
	static CxMaterial Jade();
	static CxMaterial LightBlue();
	static CxMaterial Obsidian();
	static CxMaterial Pearl();
	static CxMaterial Pewter();
	static CxMaterial PlasticBlack(); 
	static CxMaterial PlasticCyan();
	static CxMaterial PlasticGreen();
	static CxMaterial PlasticRed();
	static CxMaterial PlasticWhite();
	static CxMaterial PlasticYellow();
	static CxMaterial RubberBlack();
	static CxMaterial RubberCyan();
	static CxMaterial RubberGreen();
	static CxMaterial RubberRed();
	static CxMaterial RubberWhite();
	static CxMaterial RubberYellow();
	static CxMaterial Ruby();
	static CxMaterial Sliver();
	static CxMaterial Steel();
	static CxMaterial PolishedSliver();
	static CxMaterial Turquoise();
	

};



 
 
 
#endif // !defined(_XMATERIAL_H__INCLUDED_)
