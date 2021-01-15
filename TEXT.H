// text.h: include file for text editor classes

#ifndef __TEXT_H__
#define __TEXT_H__

/////////////////////////////////////////////////////////////////////////////

class CTextDoc : public CDocument
{        
friend class CTextView;
protected: // create from serialization only
	CTextDoc();
	DECLARE_DYNCREATE(CTextDoc)

// Attributes
public: 
	CString m_strDocTitle; 
	CInternetNavApp* m_pApp;
	
// Operations
public:

// Implementation
public:
	virtual ~CTextDoc();

	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();
	virtual BOOL SaveModified();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CTextDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTextView view -- for viewing text files

class CTextView : public CEditView
{ 
friend class CTextWnd;
protected: // create from serialization only
	CTextView(); 
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_DYNCREATE(CTextView)

// Attributes
public:            
	// static init/term...
	static void Initialize();
	static void Terminate();

	CTextDoc* GetDocument();    
	CInternetNavApp* m_pApp;

// Operations
public:
	// Word wrap...
	BOOL IsWordWrap() const;
	BOOL SetWordWrap(BOOL bWordWrap);

	// Buffer access...
	LPCSTR LockBuffer() const
		{ return CEditView::LockBuffer(); }
	void UnlockBuffer() const
		{ CEditView::UnlockBuffer(); }

// Implementation
public:           
	BOOL IsUpdatePending() {return (m_uTimerID != NULL);}
	
protected:   
	virtual ~CTextView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	UINT m_uTimerID; // ==0 when no outstanding

	static LOGFONT NEAR m_lfDefFont;
	static LOGFONT NEAR m_lfDefFontOld;
	CFont m_font;

	static LOGFONT NEAR m_lfDefPrintFont;
	static LOGFONT NEAR m_lfDefPrintFontOld;
	CFont m_fontPrint;

	static UINT m_nDefTabStops;
	static UINT m_nDefTabStopsOld;
	static BOOL m_bDefWordWrap;
	static BOOL m_bDefWordWrapOld;
	
	int m_nHeaderTime;
	int m_nFooterTime;

	UINT m_nPreviewPage;
	CTime m_timeHeader;
	CTime m_timeFooter;

	BOOL m_bUndo;  // undo/redo flag

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo); 
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnScrollTo(CDC* pDC, CPrintInfo* pInfo, POINT point);

// Generated message map functions
protected:
	//{{AFX_MSG(CTextView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMirrorDisplayFont();
	afx_msg void OnWordWrap();
	afx_msg void OnUpdateWordWrap(CCmdUI* pCmdUI);
	afx_msg void OnChoosePrintFont();
	afx_msg void OnUpdateChoosePrintFont(CCmdUI* pCmdUI);
	afx_msg void OnChooseFont();
	afx_msg void OnPageSetup();
	afx_msg void OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI);
	afx_msg void OnWindowRemember();
	afx_msg void OnEditUndo();    
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in textview.cpp
inline CTextDoc* CTextView::GetDocument()
   { return (CTextDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextWnd MDI child frame

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTextWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CTextWnd)
protected:
	CTextWnd();			// protected constructor used by dynamic creation

// Attributes
public:
protected: // control bar embedded members
	CDialogBar m_wndRulerBar; // for a ruler along the top edge of the window
	CBitmapButton m_btnRuler;

// Operations
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child

// Implementation
public:
	virtual ~CTextWnd();
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif	// __TEXT_H__
