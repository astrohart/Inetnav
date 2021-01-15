// helpinfo.h : header file
//

#ifndef __HELPINFO_H__
#define __HELPINFO_H__

/////////////////////////////////////////////////////////////////////////////
// CHelpInfoView form view

class CHelpInfoView : public CFormView
{
	DECLARE_DYNCREATE(CHelpInfoView)
protected:
	CHelpInfoView();			// protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CHelpInfoView)
	enum { IDD = IDR_HELPINFO };
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA       
	CInetnavStatusBar* m_pStatusBar;

// Attributes
public: 
	CBitmapButton m_btnAbout, m_btnInet, m_btnHelpContents, m_btnSearch;
	CBitmapButton m_btnTechHelp, m_btnLabel;

// Operations
public:
	virtual void OnInitialUpdate(); 
	
// Implementation
protected:
	virtual ~CHelpInfoView();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CHelpInfoView)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpContents();
	afx_msg void OnInetClientsHelp();
	afx_msg void OnSearchHelp();
	afx_msg void OnTechSupport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CHelpInfoDoc document

class CHelpInfoDoc : public CDocument
{
	DECLARE_SERIAL(CHelpInfoDoc)
protected:
	CHelpInfoDoc();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Implementation
protected:
	virtual ~CHelpInfoDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual	BOOL OnNewDocument();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHelpInfoDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CHelpInfoWnd frame

class CHelpInfoWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CHelpInfoWnd)
protected:
	CHelpInfoWnd();			// protected constructor used by dynamic creation

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
	virtual ~CHelpInfoWnd();

	// Generated message map functions
	//{{AFX_MSG(CHelpInfoWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__HELPINFO_H__