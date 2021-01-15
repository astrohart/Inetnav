// mainmenu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainMenuView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMainMenuView : public CFormView
{
	DECLARE_DYNCREATE(CMainMenuView)
protected:
	CMainMenuView();			// protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CMainMenuView)
	enum { IDD = IDR_MAINMENUTYPE };
	//}}AFX_DATA
	CBitmapButton m_btnPOFFICE, m_btnHelpInfo, m_btnFinger, m_btnFTP;
	CBitmapButton m_btnChat, m_btnNews, m_btnGopher, m_btnPrefs, m_btnWhois;

// Attributes
public:  
	CInternetNavApp* m_pApp;

// Operations
public: 
	void OnInitialUpdate();

// Implementation
protected:
	virtual ~CMainMenuView();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CMainMenuView)
	afx_msg void OnFinger();
	afx_msg void OnFtp();
	afx_msg void OnGopher();
	afx_msg void OnHelpAndInfo();
	afx_msg void OnIRC();
	afx_msg void OnMailManager();
	afx_msg void OnUsenetNews();
	afx_msg void OnWhois();
	afx_msg void OnPrefs();
	afx_msg void OnWindowRemember();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMainMenuDoc document

class CMainMenuDoc : public CDocument
{
	DECLARE_SERIAL(CMainMenuDoc)
protected:
	CMainMenuDoc();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:   
	virtual void SetTitle(const char* pszTitle);

// Implementation
protected:
	virtual ~CMainMenuDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual	BOOL OnNewDocument();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainMenuDoc)
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMainMenuFrm frame

class CMainMenuFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMainMenuFrm)
protected:
	CMainMenuFrm();			// protected constructor used by dynamic creation

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
	virtual ~CMainMenuFrm();

	// Generated message map functions
	//{{AFX_MSG(CMainMenuFrm)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Windows Sockets cleanup function

void FAR AfxSocketTerm();