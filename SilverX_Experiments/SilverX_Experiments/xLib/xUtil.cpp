// xUtil.cpp

#include "StdAfx.h"
#include "xUtil.h"
#include <cmath>

bool IsZero(float x)
{
	if ( fabs(x) < EPSILON )
		return true;
	else
		return false;

}

// this algorithm is devised by Marsaglia, recommended by Knuth

double GaussianRandom()
{
	static double V1, V2, S;
	static int phass = 0;
	double x;

	if ( phass == 0)
	{
		do 
		{
			double u1 = (double)rand() / RAND_MAX;
			double u2 = (double)rand() / RAND_MAX;

			V1 = 2*u1 - 1;
			V2 = 2*u2 - 1;
			
			S  = V1*V1 + V2*V2;

		} while( (S >= 1) || (S == 0));

		x = V1 * sqrt(-2 * log(S) / S);
	}
	else
	{
		x = V2 * sqrt(-2 * log(S) / S);
	}

	phass = 1 - phass;

	return x;

}


void StatusMessage(char* fmt,...)
{   
	CWinApp *pApp = AfxGetApp();
	if(pApp->m_pMainWnd != NULL) 
	{ 
		char buffer[256];
		CMFCStatusBar* pStatus = 
			(CMFCStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(
			AFX_IDW_STATUS_BAR);
		
		// fill buffer
		va_list argptr;      
		va_start(argptr,fmt);
		vsprintf_s(buffer,fmt,argptr);
		va_end(argptr);
		
		if(pStatus != NULL) 
		{
			pStatus->SetPaneText(0,buffer);
			pStatus->UpdateWindow(); 
		}
  }
	return;
}

