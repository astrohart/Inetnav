// propshts.h : header file for Preferences and Setup dialog boxes
//

#ifndef __PROPSHTS_H__
#define __PROPSHTS_H__
/////////////////////////////////////////////////////////////////////////////
// CPageSetupPage property page

class CPageSetupPage : public CPropertyPage
{
// Construction
public:
	CPageSetupPage();	// standard constructor
	void Initialize();
	void Terminate();

// Dialog Data
	//{{AFX_DATA(CPageSetupPage)
	enum { IDD = IDD_PAGE_SETUP };
	CButton	m_btnHeaderFile;  
	CButton m_btnHeaderSystem;
	CStatic	m_lblHeader;
	CEdit	m_ctlHeader;
	CString	m_strHeader;
	CString	m_strFooter;
	int		m_nFooterTime;
	int		m_nHeaderTime;
	//}}AFX_DATA
	CInternetNavApp* m_pApp;
	
	CString m_strFooterOld;
	CString m_strHeaderOld;
	int     m_nFooterTimeOld;
	int     m_nHeaderTimeOld;  
	BOOL	m_bOutputView;               

// Operations
public:
	void FormatHeader(CString& strHeader, CTime& time,
		CString strFileName, UINT nPage);
	void FormatFooter(CString& strFooter, CTime& time,
		CString strFileName, UINT nPage);

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	static void FormatFilePage(
		CString& strFormat, CString strFileName, UINT nPage);

	// Generated message map functions
	//{{AFX_MSG(CPageSetupPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CSetTabsPage property page

class CSetTabsPage : public CPropertyPage
{
// Construction
public:
	CSetTabsPage();	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTabsPage)
	enum { IDD = IDD_SET_TABSTOPS };
	int		m_nTabStops;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CSetTabsPage)
	afx_msg void OnChangeEditTabs();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAppGeneralPrefs property page

class CAppGeneralPrefs : public CPropertyPage
{
// Construction
public:
	CAppGeneralPrefs();	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAppGeneralPrefs)
	enum { IDD = IDD_APP_GENERAL_PREFS };
	CButton	m_btnShowTipsInet;
	CButton	m_btnShowTipsApp;
	CInternetNavApp*	m_pApp;
	BOOL	m_bStartOnline;
	BOOL	m_bShowMainMenu;
	BOOL	m_bConfirmExit;
	BOOL	m_bShowTipOfDay;
	int		m_nShowTips;
	BOOL	m_bConfirmSignOff;
	//}}AFX_DATA
	void Serialize(BOOL IsStoring); // for data transfer
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	
// Implementation
protected:                                               
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CAppGeneralPrefs)
	afx_msg void OnBtnClick();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAppAreasPrefs dialog

class CAppAreasPrefs : public CPropertyPage
{
// Construction
public:
	CAppAreasPrefs();	// default constructor

// Property Page Data
	//{{AFX_DATA(CAppAreasPrefs)
	enum { IDD = IDD_APP_AREAS_PREFS };
	CInternetNavApp*	m_pApp;
	BOOL	m_bStartCheckMail;
	UINT	m_nTimeOut;
	//}}AFX_DATA
	void Serialize(BOOL IsStoring);

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CAppAreasPrefs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAppPrefsSheet property sheet, or 'tabbed dialog box'

class CAppPrefsSheet : public CPropertySheet
{
// Construction  
public:
	CAppPrefsSheet(CWnd* pParentWnd = NULL);		// standard constructor

// Attributes
public: 	// property pages, or 'tabs' in this 'dialog box'

	CAppGeneralPrefs m_appGeneralPrefs; 
	CAppAreasPrefs	 m_appAreasPrefs;
	// We don't want all of the usual buttons which are in a modal property
	// sheet, and in order to do that we would have to make it modeless and
	// create and manage the windows for the buttons ourselves. But
	// the overriden functions below give us a modal 'tabbed dialog' with 
	// only the OK, Cancel, and Help buttons instead of the OK, Cancel,
	// Help, and Apply Now buttons!!!

// Operations
public:	// function overrides
	BOOL CreateStandardButtons();  // this function creates the buttons
	void RecalcLayout();  // this function puts the buttons 
							// where they're supposed to be in the window
	
	void Serialize(BOOL IsStoring); // this function is responsible for
										// transferring page data to the
										// app and app's INI file
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAppPrefsSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetFingerPrefs dialog

class CInetFingerPrefs : public CPropertyPage
{
// Construction
public:
	CInetFingerPrefs();	// standard constructor

// Property Page Data
	//{{AFX_DATA(CInetFingerPrefs)
	enum { IDD = IDD_INTERNET_FINGER_PREFS };
	BOOL	m_bAskFingerAddr;
	BOOL	m_bAutoFingerSave;
	BOOL	m_bAutoFingerPrint;
	CInternetNavApp*	m_pApp;
	BOOL	m_bFingerRQP;
	//}}AFX_DATA
	void Serialize(BOOL IsStoring); // for data transfer
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInetFingerPrefs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetFTPPrefs FTP preferences property page

class CInetFTPPrefs : public CPropertyPage
{
// Construction
public:
	CInetFTPPrefs();	// default constructor

// Property Page Data
	//{{AFX_DATA(CInetFTPPrefs)
	enum { IDD = IDD_INTERNET_FTP_PREFS };
	CInternetNavApp*	m_pApp;
	BOOL	m_bOpenASCII;
	BOOL	m_bOpenBin;
	//}}AFX_DATA
	void Serialize(BOOL IsStoring); // for data transfer

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInetFTPPrefs)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdvanced();
	afx_msg void OnViruses();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetWhoisPrefs property page

class CInetWhoisPrefs : public CPropertyPage
{
// Construction
public:
	CInetWhoisPrefs();	// standard constructor

// Property Page Data
	//{{AFX_DATA(CInetWhoisPrefs)
	enum { IDD = IDD_INTERNET_WHOIS_PREFS };
	BOOL	m_bReportQueryProgress;
	BOOL	m_bAutoSave;
	BOOL	m_bAutoPrint;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA
	void Serialize(BOOL IsStoring); // for data transfer
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();	

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInetWhoisPrefs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetChatPrefs property page

class CInetChatPrefs : public CPropertyPage
{
// Construction
public:
	CInetChatPrefs();	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInetChatPrefs)
	enum { IDD = IDD_INTERNET_CHAT_PREFS };
	CInternetNavApp*	m_pApp;
	CString	m_strServer;
	CString	m_strLocal;
	BOOL	m_bNotifyA;
	BOOL	m_bNotifyL;
	//}}AFX_DATA
	void Serialize(BOOL IsStoring);

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInetChatPrefs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetGopherPrefs property sheet	

class CInetGopherPrefs : public CPropertyPage
{
// Construction
public:
	CInetGopherPrefs();	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInetGopherPrefs)
	enum { IDD = IDD_INTERNET_GOPHER_PREFS };
	CString	m_strGopherDocDir;
	BOOL	m_bAutoOpenFile;
	BOOL	m_bShowSaveAs;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA
	void Serialize(BOOL IsStoring);

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInetGopherPrefs)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetMailPrefs property page

class CInetMailPrefs : public CPropertyPage
{
// Construction
public:
	CInetMailPrefs();	// standard constructor

// Property Page Data
	//{{AFX_DATA(CInetMailPrefs)
	enum { IDD = IDD_INTERNET_MAIL_PREFS };
	BOOL	m_bSendClose;
	BOOL	m_bSendConfirm;
	BOOL	m_bSetupNew;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA      
	void Serialize(BOOL IsStoring);

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInetMailPrefs)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetNewsPrefs property page

class CInetNewsPrefs : public CPropertyPage
{
// Construction
public:
	CInetNewsPrefs();	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInetNewsPrefs)
	enum { IDD = IDD_INTERNET_NEWS_PREFS };
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInetNewsPrefs)
	virtual BOOL OnInitDialog();
	afx_msg void OnArticleAge();
	afx_msg void OnArticleOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetPrefsSheet property sheet, or 'tabbed dialog box'

class CInetPrefsSheet : public CPropertySheet
{
// Construction  
public:
	CInetPrefsSheet(CWnd* pParent = NULL);		// standard constructor

// Attributes
public: 	// property pages, or 'tabs' in this 'dialog box'

	CInetFingerPrefs m_inetFingerPrefs;  
	CInetWhoisPrefs  m_inetWhoisPrefs;                 
	CInetFTPPrefs	 m_inetFTPPrefs;
	CInetChatPrefs	 m_inetChatPrefs;   
	CInetMailPrefs	 m_inetMailPrefs; 
	CInetNewsPrefs	 m_inetNewsPrefs;
	CInetGopherPrefs m_inetGopherPrefs;

// Operations
public:	// function overrides
	BOOL CreateStandardButtons();  // this function creates the buttons
	void RecalcLayout();  // this function puts the buttons 
							// where they're supposed to be in the window
	
	void Serialize(BOOL IsStoring); // this function is responsible for
										// transferring page data to the
										// app and app's INI file
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInetPrefsSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFtpPropsGeneral property page

class CFtpPropsGeneral : public CPropertyPage
{
// Construction
public:
	CFtpPropsGeneral();	// standard constructor

// Property Page Data
	//{{AFX_DATA(CFtpPropsGeneral)
	enum { IDD = IDD_FTP_PROPS_GENERAL };
	CString	m_strPassword;
	CString	m_strSiteAddress;
	CString	m_strUserID;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA  

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFtpPropsGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFtpFilesProps dialog

class CFtpFilesProps : public CPropertyPage
{
// Construction
public:
	CFtpFilesProps();	// standard constructor

// Property Page Data
	//{{AFX_DATA(CFtpFilesProps)
	enum { IDD = IDD_FTP_PROPS_FILES };
	CString	m_strFileName;
	CString	m_strFilePath;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFtpFilesProps)
	afx_msg void OnSetDownloadDir();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFtpPropsSheet property sheet, or 'tabbed dialog box'

class CFtpPropsSheet : public CPropertySheet
{
// Construction  
public:
	CFtpPropsSheet(CWnd* pParentWnd = NULL);		// standard constructor

// Attributes
public: 	// property pages, or 'tabs' in this 'dialog box'

	CFtpPropsGeneral m_pgFtpPropsGeneral;
	CFtpFilesProps	 m_pgFtpPropsFiles;
	// We don't want all of the usual buttons which are in a modal property
	// sheet, and in order to do that we would have to make it modeless and
	// create and manage the windows for the buttons ourselves. But
	// the overriden functions below give us a modal 'tabbed dialog' with 
	// only the OK, Cancel, and Help buttons instead of the OK, Cancel,
	// Help, and Apply Now buttons.

// Operations
public:	// function overrides
	BOOL CreateStandardButtons();  // this function creates the buttons
	void RecalcLayout();  // this function puts the buttons 
							// where they're supposed to be in the window

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFtpPropsSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPageSetupSheet property sheet, or 'tabbed dialog box'

class CPageSetupSheet : public CPropertySheet
{
// Construction  
public:
	CPageSetupSheet(CWnd* pParentWnd = NULL);		// standard constructor

// Attributes
public: 	// property pages, or 'tabs' in this 'dialog box'

	CPageSetupPage m_pgHeaderFooter;
	CSetTabsPage   m_pgTabStops;
	// We don't want all of the usual buttons which are in a modal property
	// sheet, and in order to do that we would have to make it modeless and
	// create and manage the windows for the buttons ourselves. But
	// the overriden functions below give us a modal 'tabbed dialog' with 
	// only the OK, Cancel, and Help buttons instead of the OK, Cancel,
	// Help, and Apply Now buttons.

// Operations
public:	// function overrides
	BOOL CreateStandardButtons();  // this function creates the buttons
	void RecalcLayout();  // this function puts the buttons 
							// where they're supposed to be in the window

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSetupSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__PROPSHTS_H__