// askstaff.h : header file
//

#ifndef __ASKSTAFF_H__
#define __ASKSTAFF_H__

#include "statbar.h"

/////////////////////////////////////////////////////////////////////////////
// CAskStaffView form view

class CAskStaffView : public CFormView
{
	DECLARE_DYNCREATE(CAskStaffView)
protected:
	CAskStaffView();			// protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CAskStaffView)
	enum { IDD = IDR_ASKSTAFF };
	CBitmapButton	m_btnTechSupport;
	CBitmapButton	m_btnSuggestionBox;
	CBitmapButton	m_btnISPSupport;
	CBitmapButton	m_btnFeedback;
	CBitmapButton	m_btnAskStaff;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA

// Attributes
public:         
	CInetnavStatusBar* m_pStatusBar;

// Operations
public:                
	virtual void OnInitialUpdate();

// Implementation
protected:
	virtual ~CAskStaffView();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CAskStaffView)
	afx_msg void OnTechSupport();
	afx_msg void OnSuggestionBox();
	afx_msg void OnISPTechSupport();
	afx_msg void OnFeedbackWizard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAskStaffDoc document

class CAskStaffDoc : public CDocument
{
	DECLARE_SERIAL(CAskStaffDoc)
protected:
	CAskStaffDoc();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:   
	virtual void SetTitle(const char* pszTitle); // for our custom title

// Implementation
protected:
	virtual ~CAskStaffDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual	BOOL OnNewDocument();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAskStaffDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAskStaffWnd frame

class CAskStaffWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAskStaffWnd)
protected:
	CAskStaffWnd();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:  
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child

// Implementation
protected:
	virtual ~CAskStaffWnd();

	// Generated message map functions
	//{{AFX_MSG(CAskStaffWnd)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif

