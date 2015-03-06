// xUtilMfc.cpp: implementation of the CxUtilMfc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xUtilMfc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace UtilMFC
{
	///////////////////////////////////////////////////////////
	// Replace the current view in the splitter pSplitter
	// at pane (row, col) by a new view of class pViewClass
	
	void EkSwitchViewInSplitter(	CSplitterWnd* pSplitter,
								int row, int col,
								CRuntimeClass* pViewClass )
	{
		ASSERT_VALID( pSplitter );
		ASSERT( pViewClass != NULL );
		
		ASSERT( pViewClass->
			IsDerivedFrom( RUNTIME_CLASS( CView ) ) );
		
		// 1 - Find the view to be replaced
		CWnd* pPaneWnd = pSplitter->GetPane( row, col );
		if( !pPaneWnd->IsKindOf( RUNTIME_CLASS( CView ) ) )
		{
			TRACE2(	"Unable to switch: pane (%d,%d) is not a view\n",
				row, col );
			return;
		}
		
		CView* pCurrentView = static_cast<CView*>( pPaneWnd );
		ASSERT_VALID( pCurrentView );
		ASSERT_KINDOF( CView, pCurrentView );
		
		if( pCurrentView->IsKindOf( pViewClass ) )
		{
			// No need to switch for same view class
			return;
		}
		
		// 2 - Store current view position and activation state
		CRect rcView;
		pCurrentView->GetWindowRect( &rcView );
		
		CView* pActiveView = pSplitter->
			GetParentFrame()->GetActiveView();
		BOOL bSaveActive = ( pActiveView == NULL ) 
			|| ( pActiveView == pCurrentView );
		
		// 3 - Find the associated document
		CDocument* pDoc = pCurrentView->GetDocument();
		ASSERT_VALID( pDoc );
		
		// 4 - Make sure the document won't self-destruct
		// when current view is destroyed
		BOOL bSaveAutoDelete = pDoc->m_bAutoDelete;
		pDoc->m_bAutoDelete = FALSE;
		
		// 5 - Destroy the current view
		pCurrentView->DestroyWindow();
		
		// 6 - Restore document to initial state
		pDoc->m_bAutoDelete = bSaveAutoDelete;
		
		// 7 - Initialize creation context used by CreateView()
		CCreateContext context;
		context.m_pNewDocTemplate = NULL;
		context.m_pLastView = NULL;
		context.m_pCurrentFrame = NULL;
		
		context.m_pNewViewClass = pViewClass;
		context.m_pCurrentDoc = pDoc;
		
		// 8 - Create the new view
		pSplitter->CreateView(	row, col, pViewClass,
			rcView.Size(), &context );
		
		CView* pNewView = static_cast<CView*>
			( pSplitter->GetPane( row, col ) );
		ASSERT_VALID( pNewView );
		ASSERT_KINDOF( CView, pNewView );
		
		// 9 - Position the new view like the old one and
		// activate it if needed
		pSplitter->ScreenToClient( &rcView );
		pNewView->MoveWindow( &rcView, TRUE );
		if( bSaveActive )
		{
			pSplitter->GetParentFrame()->SetActiveView( pNewView );
		}
		
		// 10 - Send WM_INITIALUPDATE to the view
		pNewView->GetParentFrame()->InitialUpdateFrame( pDoc, TRUE );
	}
	
	void StatusMessage(char* fmt,...)
	{   
		CWinApp *pApp = AfxGetApp();
		if(pApp->m_pMainWnd != NULL) 
		{ 
			char buffer[256];
#if 0
			CStatusBar* pStatus = 
				(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(
				AFX_IDW_STATUS_BAR);
#endif
			// vs 2010 中使用CMFCStatusBar类
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

	void StatusMessageNUM(char* fmt,...)
	{   
		CWinApp *pApp = AfxGetApp();
		if(pApp->m_pMainWnd != NULL) 
		{ 
			char buffer[256];
#if 0
			CStatusBar* pStatus = 
				(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(
				AFX_IDW_STATUS_BAR);
#endif
			// vs 2010 中使用CMFCStatusBar类
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
				pStatus->SetPaneText(2, buffer);
				pStatus->UpdateWindow(); 
			}
		}
		return;
	}

	// console includes
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <commdlg.h>
	void showWin32Console()
	{
#if defined(_WIN32)
		AllocConsole();
		// stdout
		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((intptr_t)handle_out, _O_TEXT);
		FILE* hf_out = _fdopen(hCrt, "w"); 
		setvbuf(hf_out, NULL, _IONBF, 1);
		*stdout = *hf_out;
		// stderr
		handle_out = GetStdHandle(STD_ERROR_HANDLE);
		hCrt = _open_osfhandle((intptr_t)handle_out, _O_TEXT);
		hf_out = _fdopen(hCrt, "w");
		setvbuf(hf_out, NULL, _IONBF, 1);
		*stderr = *hf_out;
#endif
	}



	int GetFPS()
	{
		// This will hold the number of frames per second in between seconds.
		static int FPS          = 0;
		static int CurrentFps   = 0;

		// This will be used to check if a second has passed.
		float nextSecond        = 0.0f;

		// This is the last second that occured.
		static float prevSecond	= 0.0f;

		// Add to the frames per second every time this function is called.
		CurrentFps++;

		// Get the second in millisecond then multiply it by 0.001 to convert this to
		// seconds.
		nextSecond = GetTickCount() * 0.001f;				

		// If the time we have now substracted from the previous second is greater than
		// or equal to 1 (i.e. if a second has passed) then we display the FPS number.
		if( nextSecond - prevSecond > 1.0f )
		{
			// Make the second we just got the previous second so we can used this the
			// next time the function is called.
			prevSecond = nextSecond;

			FPS = CurrentFps;

			// Reset the FPS counter.
			CurrentFps = 0;
		}

		return FPS;
	}


}  // end namespace UtilMFC