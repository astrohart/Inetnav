// splash.h : header file
//

#ifndef __SPLASH_H__
#define __SPLASH_H__

/////////////////////////////////////////////////////////////////////////////
// CBigIcon button

class CBigIcon : public CButton
{
// Attributes
public:

// Operations
public:
	void SizeToContent();

// Implementation
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//{{AFX_MSG(CBigIcon)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd dialog

class CSplashWnd : public CDialog
{          
friend class CInternetNavApp;
// Construction
public:
	CSplashWnd(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSplashWnd)
	enum { IDD = IDD_SPLASH };
	CInternetNavApp*	m_pApp;
	CString	m_strUserCompany;
	CString	m_strUserName;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	CBigIcon m_icon;
public:
	BOOL Create() { return CDialog::Create(IDD_SPLASH, CWnd::GetDesktopWindow()); }

	// Generated message map functions
	//{{AFX_MSG(CSplashWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////
#endif //__SPLASH_H__
