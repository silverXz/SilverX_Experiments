// xMaterial.cpp: implementation of the CxMaterial class.
// ***************************************************************
//  xMaterial   version:  1.0   ¡¤  date: 05/08/2008
//  -------------------------------------------------------------
//  Author: jh.wu@siat.ac.cn
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "stdafx.h"
#include "xMaterial.h"


CxMaterial::CxMaterial()
{
	// Default
	SetAmbient(0.0f, 0.33f, 0.5f, 1.0f);
	SetDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
	SetSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	SetShininess(84.0f);
	SetEmission(0.0f, 0.0f, 0.0f, 1.0f);
}

void CxMaterial::SetAmbient(float r, float g, float b, float a)													 
{
	m_pAmbient[0] = r;
	m_pAmbient[1] = g;
	m_pAmbient[2] = b;
	m_pAmbient[3] = a;
}

void CxMaterial::SetDiffuse(float r, float g, float b, float a)													
{
	m_pDiffuse[0] = r;
	m_pDiffuse[1] = g;
	m_pDiffuse[2] = b;
	m_pDiffuse[3] = a;
}

void CxMaterial::SetSpecular(float r, float g, float b, float a)
{
	m_pSpecular[0] = r;
	m_pSpecular[1] = g;
	m_pSpecular[2] = b;
	m_pSpecular[3] = a;
}


void CxMaterial::SetEmission(float r, float g, float b,	float a)												
{
	m_pEmission[0] = r;
	m_pEmission[1] = g;
	m_pEmission[2] = b;
	m_pEmission[3] = a;
}

void CxMaterial::SetShininess(float shininess)
{
	m_pShininess[0] = shininess;
}


void CxMaterial::Copy(CxMaterial *pMaterial)
{
	SetSpecular(pMaterial->GetSpecular()[0], pMaterial->GetSpecular()[1], pMaterial->GetSpecular()[2], pMaterial->GetSpecular()[3]);
	SetAmbient(pMaterial->GetAmbient()[0], pMaterial->GetAmbient()[1], pMaterial->GetAmbient()[2], pMaterial->GetAmbient()[3]);
	SetDiffuse(pMaterial->GetDiffuse()[0], pMaterial->GetDiffuse()[1], pMaterial->GetDiffuse()[2], pMaterial->GetDiffuse()[3]);
	SetEmission(pMaterial->GetEmission()[0], pMaterial->GetEmission()[1], pMaterial->GetEmission()[2], pMaterial->GetEmission()[3]);
	SetShininess(pMaterial->GetShininess()[0]);
}

CxMaterial CxMaterial::Aluminium()
{
	CxMaterial al;
	al.SetAmbient(0.25f, 0.25f, 0.25f, 1.0f);
	al.SetDiffuse(0.4f, 0.4f, 0.4f, 1.0f);
	al.SetSpecular(0.5774597f, 0.5774597f, 0.5774597f, 1.0f);
	al.SetShininess(25.8f);
	return al;
}

CxMaterial CxMaterial::Brass()
{
	CxMaterial brass;
	brass.SetAmbient(0.329412f, 0.223529f,  0.027451f, 1.0f);
	brass.SetDiffuse(0.780392f, 0.568627f,  0.113725f, 1.0f);
	brass.SetSpecular(0.992157f, 0.941176f, 0.807843f, 1.0f);
	brass.SetShininess(27.897400f);
	return brass;
}	

CxMaterial CxMaterial::Bronze()
{
	CxMaterial bronze;
	bronze.SetAmbient(0.2125f, 0.1275f, 0.054f, 1.0f);
	bronze.SetDiffuse(0.714f, 0.4248f, 0.18144f, 1.0f);
	bronze.SetSpecular(0.393548f, 0.271906f, 0.166721f, 1.0f);
	bronze.SetShininess(25.6f);
	return bronze;
}

CxMaterial CxMaterial::PolishedBronze()
{
	CxMaterial pb;
	pb.SetAmbient(0.25f, 0.148f, 0.06475f, 1.0f);
	pb.SetDiffuse(0.4f, 0.2368f, 0.1036f, 1.0f);
	pb.SetSpecular(0.774597f, 0.458561f, 0.200621f, 1.0f);
	pb.SetShininess(76.8f);
	return pb;
}

CxMaterial CxMaterial::Chrome()
{
	CxMaterial chrome;
	chrome.SetAmbient(0.25f, 0.25f, 0.25f, 1.0f);
	chrome.SetDiffuse(0.4f, 0.4f, 0.4f, 1.0f);
	chrome.SetSpecular(0.774597f, 0.774597f, 0.774597f, 1.0f);
	chrome.SetShininess(76.8f);
	return chrome;
}

CxMaterial CxMaterial::Copper()
{
	CxMaterial copper;
	copper.SetAmbient(0.19125f, 0.0735f, 0.0225f, 1.0f);
	copper.SetDiffuse(0.7038f, 0.27048f, 0.0828f, 1.0f);
	copper.SetSpecular(0.256777f, 0.137622f, 0.086014f, 1.0f);
	copper.SetShininess(12.8f);
	return copper;
}

CxMaterial CxMaterial::PolishedCopper()
{
	CxMaterial pc;
	pc.SetAmbient(0.2295f, 0.08825f, 0.0275f, 1.0f);
	pc.SetDiffuse(0.5508f, 0.2118f, 0.066f, 1.0f);
	pc.SetSpecular(0.580594f, 0.223257f, 0.0695701f, 1.0f);
	pc.SetShininess(51.2f);
	return pc;
}

CxMaterial CxMaterial::Emerald()
{
	CxMaterial emerald;
	emerald.SetAmbient(0.0215f, 0.1745f,	0.0215f,	1.0f);
	emerald.SetDiffuse(0.07568f, 0.61424f, 0.07568f, 1.0f);
	emerald.SetSpecular(0.633f,	0.727811f, 0.633f, 1.0f);
	emerald.SetShininess(76.8f);
	return emerald;
}

CxMaterial CxMaterial::Gold()
{
	CxMaterial gold;
	gold.SetAmbient(0.24725f, 0.1995f, 0.0745f, 1.0f);
	gold.SetDiffuse(0.75164f, 0.60648f, 0.22648f, 1.0f);
	gold.SetSpecular(0.628281f, 0.555802f, 0.366065f, 1.0f);
	gold.SetShininess(51.2f);
	return gold;
}

CxMaterial CxMaterial::PolishedGold()
{
	CxMaterial pg;
	pg.SetAmbient(0.24725f, 0.2245f, 0.0645f, 1.0f);
	pg.SetDiffuse(0.34615f, 0.3143f, 0.0903f, 1.0f);
	pg.SetSpecular(0.797357f, 0.723991f, 0.208006f, 1.0f);
	pg.SetShininess(83.2f);
	return pg;
}

CxMaterial CxMaterial::Jade()
{
	CxMaterial jade;
	jade.SetAmbient(0.135f, 0.2225f, 0.1575f, 1.0f);
	jade.SetDiffuse(0.54f, 0.89f, 0.63f, 1.0f);
	jade.SetSpecular(0.316228f, 0.316228f, 0.316228f, 1.0f);
	jade.SetShininess(12.8f);
	return jade;
}

CxMaterial CxMaterial::LightBlue()
{
	CxMaterial lb;
	lb.SetAmbient(0.0f, 0.5f, 0.75f, 1.0f);
	lb.SetDiffuse(0.0f, 0.5f, 1.0f, 1.0f);
	lb.SetSpecular(0.75f, 0.75f, 0.75f, 1.0f);
	lb.SetShininess(64.0f);
	return lb;
}

CxMaterial CxMaterial::Obsidian()
{
	CxMaterial obsidian;
	obsidian.SetAmbient(0.05375f, 0.05f, 0.6625f, 1.0f);
	obsidian.SetDiffuse(0.18275f, 0.17f, 0.22525f, 1.0f);
	obsidian.SetSpecular(0.332741f, 0.328634f, 0.346435f, 1.0f);
	obsidian.SetShininess(38.4f);
	return obsidian;
}

CxMaterial CxMaterial::Pearl()
{
	CxMaterial pearl;
	pearl.SetAmbient(0.25f, 0.20725f, 0.20725f, 1.0f);
	pearl.SetDiffuse(1.0f, 0.829f, 0.829f, 1.0f);
	pearl.SetSpecular(0.296648f, 0.296648f, 0.296648f, 1.0f);
	pearl.SetShininess(11.264f);
	return pearl;
}

CxMaterial CxMaterial::PlasticBlack()
{
	CxMaterial pb;
	pb.SetAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	pb.SetDiffuse(0.01f, 0.01f, 0.01f, 1.0f);
	pb.SetSpecular(0.50f, 0.50f, 0.50f, 1.0f);
	pb.SetShininess(32.0f);
	return pb;
}

CxMaterial CxMaterial::PlasticCyan()
{
	CxMaterial pc;
	pc.SetAmbient(0.0f, 0.1f, 0.06f, 1.0f);
	pc.SetDiffuse(0.0f, 0.50980392f, 0.50980392f, 1.0f);
	pc.SetSpecular(0.50196078f, 0.50196078f, 0.50196078f, 1.0f);
	pc.SetShininess(32.0f);
	return pc;
} 

CxMaterial CxMaterial::PlasticGreen()
{
	CxMaterial pg;
	pg.SetAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	pg.SetDiffuse(0.1f, 0.35f, 0.1f, 1.0f);
	pg.SetSpecular(0.45f, 0.55f, 0.45f, 1.0f);
	pg.SetShininess(32.0f);
	return pg;
} 

CxMaterial CxMaterial::PlasticRed()
{
	CxMaterial pr;
	pr.SetAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	pr.SetDiffuse(0.5f, 0.0f, 0.0f, 1.0f);
	pr.SetSpecular(0.7f, 0.6f, 0.6f, 1.0f);
	pr.SetShininess(32.0f);
	return pr;
} 

CxMaterial CxMaterial::PlasticWhite()
{
	CxMaterial pw;
	pw.SetAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	pw.SetDiffuse(0.55f, 0.55f, 0.55f, 1.0f);
	pw.SetSpecular(0.7f, 0.7f, 0.7f, 1.0f);
	pw.SetShininess(32.0f);
	return pw;
} 

CxMaterial CxMaterial::PlasticYellow()
{
	CxMaterial py;
	py.SetAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	py.SetDiffuse(0.5f, 0.5f, 0.0f, 1.0f);
	py.SetSpecular(0.6f, 0.6f, 0.5f, 1.0f);
	py.SetShininess(32.0f);
	return py;
} 

CxMaterial CxMaterial::RubberBlack()
{
	CxMaterial rb;
	rb.SetAmbient(0.02f, 0.02f, 0.02f, 1.0f);
	rb.SetDiffuse(0.01f, 0.01f, 0.01f, 1.0f);
	rb.SetSpecular(0.4f, 0.4f, 0.4f, 1.0f);
	rb.SetShininess(10.0f);
	return rb;
} 

CxMaterial CxMaterial::RubberCyan()
{
	CxMaterial rc;
	rc.SetAmbient(0.0f, 0.05f, 0.05f, 1.0f);
	rc.SetDiffuse(0.4f, 0.5f, 0.5f, 1.0f);
	rc.SetSpecular(0.04f, 0.7f, 0.7f, 1.0f);
	rc.SetShininess(10.0f);
	return rc;
} 

CxMaterial CxMaterial::RubberGreen()
{
	CxMaterial rg;
	rg.SetAmbient(0.0f, 0.05f, 0.0f, 1.0f);
	rg.SetDiffuse(0.4f, 0.5f, 0.4f, 1.0f);
	rg.SetSpecular(0.04f, 0.7f, 0.04f, 1.0f);
	rg.SetShininess(10.0f);
	return rg;
} 

CxMaterial CxMaterial::RubberRed()
{
	CxMaterial rr;
	rr.SetAmbient(0.05f, 0.0f, 0.0f, 1.0f);
	rr.SetDiffuse(0.5f, 0.4f, 0.4f, 1.0f);
	rr.SetSpecular(0.7f, 0.04f, 0.04f, 1.0f);
	rr.SetShininess(10.0f);
	return rr;
} 

CxMaterial CxMaterial::RubberWhite()
{
	CxMaterial rw;
	rw.SetAmbient(0.05f, 0.05f, 0.05f, 1.0f);
	rw.SetDiffuse(0.5f, 0.5f, 0.5, 1.0f);
	rw.SetSpecular(0.7f, 0.7f, 0.7f, 1.0f);
	rw.SetShininess(10.0f);
	return rw;
} 

CxMaterial CxMaterial::RubberYellow()
{
	CxMaterial ry;
	ry.SetAmbient(0.05f, 0.05f, 0.0f, 1.0f);
	ry.SetDiffuse(0.5f, 0.5f, 0.4f, 1.0f);
	ry.SetSpecular(0.7f, 0.7f, 0.04f, 1.0f);
	ry.SetShininess(10.0f);
	return ry;
} 

CxMaterial CxMaterial::Ruby()
{
	CxMaterial ruby;
	ruby.SetAmbient(0.1745f, 0.01175f, 0.01175f, 1.0f);
	ruby.SetDiffuse(0.61424f, 0.04136f, 0.04136f, 1.0f);
	ruby.SetSpecular(0.727811f, 0.626959f, 0.626959f, 1.0f);
	ruby.SetShininess(76.8f);
	return ruby;
} 

CxMaterial CxMaterial::Sliver()
{
	CxMaterial sliver;
	sliver.SetAmbient(0.19225f, 0.19225f, 0.19225f, 1.0f);
	sliver.SetDiffuse(0.50754f, 0.50754f, 0.50754f, 1.0f);
	sliver.SetSpecular(0.508273f, 0.508273f, 0.508273f, 1.0f);
	sliver.SetShininess(51.2f);
	return sliver;
} 

CxMaterial CxMaterial::Steel()
{
	CxMaterial steel;
	steel.SetAmbient(0.231250f, 0.231250f, 0.231250f, 1.0f);
	steel.SetDiffuse(0.2775f, 0.2775f, 0.2775f, 1.0f);
	steel.SetSpecular(0.973911f, 0.973911f, 0.973911f, 1.0f);
	steel.SetShininess(40.599998f);
	return steel;
}

CxMaterial CxMaterial::PolishedSliver()
{
	CxMaterial ps;
	ps.SetAmbient(0.23125f, 0.23125f, 0.23125f, 1.0f);
	ps.SetDiffuse(0.2775f, 0.2775f, 0.2775f, 1.0f);
	ps.SetSpecular(0.773911f, 0.773911f, 0.773911f, 1.0f);
	ps.SetShininess(89.6f);
	return ps;
}

CxMaterial CxMaterial::Turquoise()
{
	CxMaterial tq;
	tq.SetAmbient(0.1f, 0.18725f, 0.1745f, 1.0f);
	tq.SetDiffuse(0.396f, 0.74151f, 0.69102f, 1.0f);
	tq.SetSpecular(0.297254f, 0.30829f, 0.306678f, 1.0f);
	tq.SetShininess(12.8f);
	return tq;
} 

CxMaterial CxMaterial::Pewter()
{
	CxMaterial pewter;
	pewter.SetAmbient(0.105882f, 0.058824f, 0.113725f, 1.0f);
	pewter.SetDiffuse(0.427451f, 0.470588f, 0.541176f, 1.0f);
	pewter.SetSpecular(0.333333f, 0.333333f, 0.521569f, 1.0f);
	pewter.SetShininess(9.84615f);
	return pewter;
} 
