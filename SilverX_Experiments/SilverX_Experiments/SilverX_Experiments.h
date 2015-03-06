
// SilverX_Experiments.h : main header file for the SilverX_Experiments application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSilverX_ExperimentsApp:
// See SilverX_Experiments.cpp for the implementation of this class
//

class CSilverX_ExperimentsApp : public CWinAppEx
{
public:
	CSilverX_ExperimentsApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSilverX_ExperimentsApp theApp;
