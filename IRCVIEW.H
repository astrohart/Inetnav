// ircview.h : interface of the CIRClientView class
//   

#ifndef __IRCVIEW_H__
#define __IRCVIEW_H__
#include "ircdoc.h"
/////////////////////////////////////////////////////////////////////////////

class CIRClientView : public CEditView
{  
friend class CIRClientWnd;
protected: // create from serialization only
	CIRClientView(); 
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_DYNCREATE(CIRClientView)

// Attributes
public:
	// static init/term...
	static void Initialize();
	static void Terminate();

	CIRClientDoc* GetDocument();    
	CInternetNavApp* m_pApp;

// Operations
public:                                                   
	// Chat transcript printing
	void OnPrintTranscript();
	
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

	virtual ~CIRClientView();
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

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo); 
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnScrollTo(CDC* pDC, CPrintInfo* pInfo, POINT point);

// Generated message map functions
protected:
	//{{AFX_MSG(CIRClientView)
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
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFilePrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in textview.cpp
inline CIRClientDoc* CIRClientView::GetDocument()
   { return (CIRClientDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
#endif //__IRCVIEW_H__