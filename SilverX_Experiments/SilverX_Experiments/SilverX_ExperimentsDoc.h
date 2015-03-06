
// SilverX_ExperimentsDoc.h : interface of the CSilverX_ExperimentsDoc class
//


#pragma once


class SilverScene;

class CSilverX_ExperimentsDoc : public CDocument
{
protected: // create from serialization only
	CSilverX_ExperimentsDoc();
	DECLARE_DYNCREATE(CSilverX_ExperimentsDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSilverX_ExperimentsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	SilverScene* m_pScene;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	SilverScene* GetScenePtr(void);
};
