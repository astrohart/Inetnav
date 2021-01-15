// prefview.h : interface of the CPrefsView class
//  

#ifndef __PREFVIEW_H__
#define __PREFVIEW_H__
#include "prefsdoc.h"

/////////////////////////////////////////////////////////////////////////////

class CPrefsView : public CFormView
{
protected: // create from serialization only
	CPrefsView();
	DECLARE_DYNCREATE(CPrefsView)

public:
	//{{AFX_DATA(CPrefsView)
	enum { IDD = IDR_PREFERENCES };
	CBitmapButton	m_btnHelp;
	CBitmapButton	m_btnAppPrefs;
	CBitmapButton	m_btnUserInf;
	CBitmapButton	m_btnInetPrefs;
	//}}AFX_DATA

// Attributes
public:
	CPrefsDoc* GetDocument();

// Operations
public:
	virtual void OnInitialUpdate();

// Implementation
public:
	virtual ~CPrefsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Generated message map functions
protected:
	//{{AFX_MSG(CPrefsView)
	afx_msg void OnUserInf();
	afx_msg void OnAppPrefs();
	afx_msg void OnInetPrefs();
	afx_msg void OnClickedHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in prefview.cpp
inline CPrefsDoc* CPrefsView::GetDocument()
   { return (CPrefsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefsWnd frame

class CPrefsWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CPrefsWnd)
protected:
	CPrefsWnd();			// protected constructor used by dynamic creation

// Attributes
public:
	CInternetNavApp* m_pApp;

// Operations
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child
// Implementation
protected:
	virtual ~CPrefsWnd();

	// Generated message map functions
	//{{AFX_MSG(CPrefsWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__PREFVIEW_H__
