
// SilverX_ExperimentsView.cpp : implementation of the CSilverX_ExperimentsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SilverX_Experiments.h"
#endif

#include "SilverX_ExperimentsDoc.h"
#include "SilverX_ExperimentsView.h"

#include "xUtilMfc.h"
#include "Scene.h"
#include "ArcBall.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSilverX_ExperimentsView

IMPLEMENT_DYNCREATE(CSilverX_ExperimentsView, CView)

BEGIN_MESSAGE_MAP(CSilverX_ExperimentsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSilverX_ExperimentsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SSAO_ENABLE, &CSilverX_ExperimentsView::OnSsaoEnable)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CSilverX_ExperimentsView construction/destruction

CSilverX_ExperimentsView::CSilverX_ExperimentsView()
{
	// TODO: add construction code here
	m_bLeftButtonDown = false;
	m_bRightButtonDown = false;
	m_bSSAO = false;

}

CSilverX_ExperimentsView::~CSilverX_ExperimentsView()
{
}

BOOL CSilverX_ExperimentsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSilverX_ExperimentsView drawing

void CSilverX_ExperimentsView::OnDraw(CDC* /*pDC*/)
{
	CSilverX_ExperimentsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSilverX_ExperimentsView printing


void CSilverX_ExperimentsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSilverX_ExperimentsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSilverX_ExperimentsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSilverX_ExperimentsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSilverX_ExperimentsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{

	SilverScene* pScene = GetDocument()->GetScenePtr();

	assert( pScene!= NULL );

	pScene->GetArcBall()->releaseRight(point.x,point.y);

	m_bRightButtonDown = false;
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSilverX_ExperimentsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSilverX_ExperimentsView diagnostics

#ifdef _DEBUG
void CSilverX_ExperimentsView::AssertValid() const
{
	CView::AssertValid();
}

void CSilverX_ExperimentsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSilverX_ExperimentsDoc* CSilverX_ExperimentsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSilverX_ExperimentsDoc)));
	return (CSilverX_ExperimentsDoc*)m_pDocument;
}
#endif //_DEBUG


// CSilverX_ExperimentsView message handlers


int CSilverX_ExperimentsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	UtilMFC::showWin32Console();

	//create OpenGL render context
	HWND hWnd = GetSafeHwnd();
	m_glContext.create30Context(hWnd);

	m_glContext.makeCurrent(hWnd);

	glClearColor(0.0,0.0,0.0,1.0);

	glClear(GL_COLOR_BUFFER_BIT);

	// TODO:  Add your specialized creation code here

	SilverScene* pScene = GetDocument()->GetScenePtr();

	assert(pScene != NULL);

	pScene->Init();



	return 0;
}


void CSilverX_ExperimentsView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

	RenderScene();

	SwapBuffers(dc.m_ps.hdc);

}

void CSilverX_ExperimentsView::RenderScene()
{
	SilverScene* pScene = GetDocument()->GetScenePtr();

	assert(pScene != NULL);

	//pScene->RenderScene();

	//pScene->RenderSceneMRT();

	if( m_bSSAO )
		pScene->RenderSceneSSAO();
	else
		pScene->RenderSceneMRT();

}


void CSilverX_ExperimentsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SilverScene* pScene = GetDocument()->GetScenePtr();

	assert(pScene != NULL);

	pScene->GetArcBall()->pinchLeft(point.x,point.y);

	InvalidateRect(NULL,FALSE);

	m_bLeftButtonDown = true;

	CView::OnLButtonDown(nFlags, point);
}


void CSilverX_ExperimentsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SilverScene* pScene = GetDocument()->GetScenePtr();

	assert(pScene != NULL);

	//pScene->GetArcBall()->move(point.x,point.y);
	pScene->GetArcBall()->MoveSilverX(point.x,point.y);
	InvalidateRect(NULL,FALSE);

	CView::OnMouseMove(nFlags, point);
}


void CSilverX_ExperimentsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SilverScene* pScene = GetDocument()->GetScenePtr();

	assert(pScene != NULL);

	pScene->GetArcBall()->releaseLeft(point.x,point.y);

	InvalidateRect(NULL,FALSE);

	m_bLeftButtonDown = false;

	CView::OnLButtonUp(nFlags, point);
}


BOOL CSilverX_ExperimentsView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CSilverX_ExperimentsView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	SilverScene* pScene = GetDocument()->GetScenePtr();

	assert(pScene != NULL);

	pScene->Resize(cx,cy);

	// TODO: Add your message handler code here
}


BOOL CSilverX_ExperimentsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	SilverScene* pScene = GetDocument()->GetScenePtr();

	assert(pScene != NULL);


	if (zDelta < 0)
	{
		pScene->ZoomIn();
	}
	else
	{
		pScene->ZoomOut();
	}

	InvalidateRect(NULL,FALSE);


	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CSilverX_ExperimentsView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SilverScene* pScene = GetDocument()->GetScenePtr();
	assert( pScene!= NULL );

	pScene->GetArcBall()->pinchRight(point.x,point.y);
	m_bRightButtonDown = true;
	CView::OnRButtonDown(nFlags, point);
}


void CSilverX_ExperimentsView::OnSsaoEnable()
{
	// TODO: Add your command handler code here
	m_bSSAO = !m_bSSAO;
	InvalidateRect(NULL,FALSE);
}


void CSilverX_ExperimentsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	SilverScene* pScene = GetDocument()->GetScenePtr();
	if( !pScene )
		return;
	switch(nChar)
	{
	case 'W':
		{
			break;
		}
	case 'A':
		{
			
			break;
		}
	case 'S':
		{
			break;
		}
	case 'D':
		{
			break;
		}
	default:
		break;
	}
	InvalidateRect(NULL,FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
