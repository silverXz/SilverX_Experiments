
// SilverX_ExperimentsView.h : interface of the CSilverX_ExperimentsView class
//

#pragma once

#include "glcontext.h"
#include "SilverX_ExperimentsDoc.h"

class CSilverX_ExperimentsView : public CView
{
protected: // create from serialization only
	CSilverX_ExperimentsView();
	DECLARE_DYNCREATE(CSilverX_ExperimentsView)

// Attributes
public:
	CSilverX_ExperimentsDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);



protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSilverX_ExperimentsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	OpenGLContext m_glContext;

	bool m_bLeftButtonDown;
	bool m_bRightButtonDown;

	bool m_bSSAO;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:

	void RenderScene(void);


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSsaoEnable();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in SilverX_ExperimentsView.cpp
inline CSilverX_ExperimentsDoc* CSilverX_ExperimentsView::GetDocument() const
   { return reinterpret_cast<CSilverX_ExperimentsDoc*>(m_pDocument); }
#endif

