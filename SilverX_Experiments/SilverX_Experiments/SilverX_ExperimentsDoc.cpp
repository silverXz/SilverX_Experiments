
// SilverX_ExperimentsDoc.cpp : implementation of the CSilverX_ExperimentsDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SilverX_Experiments.h"
#endif

#include "SilverX_ExperimentsDoc.h"
#include "Scene.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSilverX_ExperimentsDoc

IMPLEMENT_DYNCREATE(CSilverX_ExperimentsDoc, CDocument)

BEGIN_MESSAGE_MAP(CSilverX_ExperimentsDoc, CDocument)
END_MESSAGE_MAP()


// CSilverX_ExperimentsDoc construction/destruction

CSilverX_ExperimentsDoc::CSilverX_ExperimentsDoc()
{
	// TODO: add one-time construction code here
	m_pScene = NULL;

	m_pScene = new SilverScene;
}

CSilverX_ExperimentsDoc::~CSilverX_ExperimentsDoc()
{
	if( m_pScene )
		delete m_pScene, m_pScene = NULL;
}

BOOL CSilverX_ExperimentsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSilverX_ExperimentsDoc serialization

void CSilverX_ExperimentsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSilverX_ExperimentsDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSilverX_ExperimentsDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSilverX_ExperimentsDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSilverX_ExperimentsDoc diagnostics

#ifdef _DEBUG
void CSilverX_ExperimentsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSilverX_ExperimentsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSilverX_ExperimentsDoc commands


BOOL CSilverX_ExperimentsDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	if( m_pScene != NULL )
		m_pScene->LoadMesh(lpszPathName);

	return TRUE;
}

SilverScene* CSilverX_ExperimentsDoc::GetScenePtr( void )
{
	return m_pScene;
}
