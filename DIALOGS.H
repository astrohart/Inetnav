// dialogs.h : header file for classes which implement the dialog boxes 
// used in Internet Navigator for getting or displaying information from
// or to the user.
//

#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include "socket.h"  

#include "mail.h"
#include "news.h"

/////////////////////////////////////////////////////////////////////////////
// CExitDlg dialog: a class to implement an "Are you sure?" message box
// for confirming the user's decision to exit this application. 

class CExitDlg : public CDialog
{
// Construction
public:
	CExitDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExitDlg)
	enum { IDD = IDD_EXIT_APPLICATION };
	BOOL	m_bConfirmExit;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CExitDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog : used to gather the password and user ID of the user
// when they want to connect to a FTP site

class CConnectDlg : public CDialog
{
// Construction
public:
	CConnectDlg(CWnd* pParent = NULL, CString strSiteName = "");

// Dialog Data
	//{{AFX_DATA(CConnectDlg)
	enum { IDD = IDD_FTP_CONNECT };
	CString	m_strUserID;
	CString	m_strPassword;
	//}}AFX_DATA
	CString m_strSiteName;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CConnectDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CWelcomeDlg dialog

class CWelcomeDlg : public CDialog
{
// Construction
public:
	CWelcomeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWelcomeDlg)
	enum { IDD = IDD_FTP_WELCOME };
	CEdit	m_ctlWelcome;
	CString	m_strWelcome;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CWelcomeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CIRConnectDlg dialog

class CIRConnectDlg : public CDialog
{
// Construction
public:
	CIRConnectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIRConnectDlg)
	enum { IDD = IDD_IRC_CONNECT };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnHelp;
	CString	m_strServer;
	int		m_nPortNum;
	CString	m_strNickname;
	CString	m_strInitialChannel;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CIRConnectDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGopherConnect dialog

class CGopherConnect : public CDialog
{
// Construction
public:
	CGopherConnect(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGopherConnect)
	enum { IDD = IDD_GOPHER_CONNECT };
	CString	m_strMenuTitle;
	CString	m_strSiteAddress;
	CString	m_strSelector;
	UINT	m_nPortNum;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CGopherConnect)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CVeronicaSearchDlg dialog

class CVeronicaSearchDlg : public CDialog
{
// Construction
public:
	CVeronicaSearchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVeronicaSearchDlg)
	enum { IDD = IDD_GOPHER_SEARCH };
	CString	m_strQuery;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CVeronicaSearchDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAOLDirDlg dialog

class CAOLDirDlg : public CDialog
{
// Construction
public:
	CAOLDirDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAOLDirDlg)
	enum { IDD = IDD_FTP_AOL_DIRECTORY };
	CString	m_strDirectory;
	//}}AFX_DATA

// Implementation
protected: 
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CAOLDirDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CWarnDlg dialog

class CWarnDlg : public CDialog
{
// Construction
public:
	CWarnDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWarnDlg)
	enum { IDD = IDD_FTP_OPEN_WARNING };
	int		m_nType;
	//}}AFX_DATA

// Implementation
protected:  
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CWarnDlg)
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CIRCJoinDlg dialog

class CIRCJoinDlg : public CDialog
{
// Construction
public:
	CIRCJoinDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIRCJoinDlg)
	enum { IDD = IDD_IRC_JOIN_CHANNEL };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CString	m_strChannel;
	//}}AFX_DATA

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CIRCJoinDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFingerAddr dialog

class CFingerAddr : public CDialog
{
// Construction
public:
	CFingerAddr(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFingerAddr)
	enum { IDD = IDD_FINGER_ADDR };
	CString	m_strAddr;
	BOOL	m_bDontShowAgain;
	//}}AFX_DATA

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFingerAddr)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CIRCMessageDialog dialog

class CIRCMessageDialog : public CDialog
{
// Construction
public:
	CIRCMessageDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIRCMessageDialog)
	enum { IDD = IDD_IRC_SEND_PRIVMSG };
	CString	m_strMessage;
	CString	m_strRecipient;
	//}}AFX_DATA

// Implementation
protected:   
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CIRCMessageDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CIRCSendCmdDialog dialog

class CIRCSendCmdDialog : public CDialog
{
// Construction
public:
	CIRCSendCmdDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIRCSendCmdDialog)
	enum { IDD = IDD_IRC_SEND_COMMAND };
	CString	m_strCommand;
	//}}AFX_DATA

// Implementation
protected:  
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CIRCSendCmdDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CIRCNicknameDialog dialog

class CIRCNicknameDialog : public CDialog
{
// Construction
public:
	CIRCNicknameDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIRCNicknameDialog)
	enum { IDD = IDD_IRC_CHANGE_NICKNAME };
	CString	m_strNewNickname;
	//}}AFX_DATA

// Implementation
protected: 
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CIRCNicknameDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDisconnectDialog dialog

class CDisconnectDialog : public CDialog
{
// Construction
public:
	CDisconnectDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDisconnectDialog)
	enum { IDD = IDD_DISCONNECT };
	BOOL	m_bConfirmSignOff;
	//}}AFX_DATA  
	CInternetNavApp* m_pApp;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDisconnectDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailBoxTitleDlg dialog

class CMailBoxTitleDlg : public CDialog
{
// Construction
public:
	CMailBoxTitleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMailBoxTitleDlg)
	enum { IDD = IDD_NEW_MAILBOX };
	CString	m_strMailBoxTitle;
	CString	m_strFileName;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailBoxTitleDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPOP3Dialog dialog

class CPOP3Dialog : public CDialog
{
// Construction
public:
	CPOP3Dialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPOP3Dialog)
	enum { IDD = IDD_MAIL_LOGON };
	CEdit	m_ctlPassword;
	CButton	m_btnOK;
	CString	m_strUserID;
	CString	m_strPassword;
	//}}AFX_DATA

public:	// helper functions for retrieving dialog data
	CString GetPassword() { return m_strPassword; }
	CString GetUserID() { return m_strUserID; }
	
// Implementation
protected:     
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CPOP3Dialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePassword();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CUserInformation dialog

class CUserInformation : public CDialog
{
// Construction
public:
	CUserInformation(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUserInformation)
	enum { IDD = IDD_USER_INFORMATION };
	CInternetNavApp*	m_pApp;
	CString	m_strUser;
	CString	m_strCompany;
	//}}AFX_DATA

// Implementation
protected:      
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CUserInformation)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsSetup dialog

class CNewsgroupsSetup : public CDialog
{
// Construction
public:
	CNewsgroupsSetup(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNewsgroupsSetup)
	enum { IDD = IDD_NEWSGROUPS_SETUP };
	CString	m_strNNTPHost;
	//}}AFX_DATA

// Implementation
protected:  
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CNewsgroupsSetup)
	afx_msg void OnArticleAge();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailSetup dialog

class CMailSetup : public CDialog
{
// Construction
public:
	CMailSetup(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMailSetup)
	enum { IDD = IDD_EMAIL_SETTINGS };
	CString	m_strEmailAddress;
	CString	m_strUserID;
	CString	m_strPassword;
	CString	m_strPOP3Address;
	CString	m_strSMTPAddress;
	//}}AFX_DATA

// Implementation
protected: 
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailSetup)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGroupSubscribe dialog

class CGroupSubscribe : public CDialog
{
// Construction
public:
	CGroupSubscribe(CWnd* pParent, QSocket* pSocket);

// Dialog Data
	//{{AFX_DATA(CGroupSubscribe)
	enum { IDD = IDD_ADD_NEWSGROUP };
	CNewsgroupsListBox	m_ctlGroupList;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnHelp;
	CButton	m_btnGetList;
	CString	m_strNewsgroup;
	CString	m_strSelection;
	//}}AFX_DATA       
	BOOL m_bKeepAdding;
	CNewsDoc* m_pDoc;

// Attributes
public:
	QSocket* m_pSocket; // our network connection
	CInternetNavApp* m_pApp;
	CInetnavStatusBar* m_pStatusBar;
	CString m_strModStatus;

// Operations
public:
	void DoFillListBox();
	
// Implementation
public:
	virtual ~CGroupSubscribe();
protected:  
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CGroupSubscribe)
	afx_msg void OnDownloadGroups();
	virtual void OnOK();
	afx_msg void OnSelect();
	virtual void OnCancel();
	afx_msg void OnDblclkNewsgroupsList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAddressBook dialog

class CAddressBook : public CDialog
{
// Construction
public:
	CAddressBook(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CAddressBook)
	enum { IDD = IDD_ADDRESS_BOOK };
	CButton	m_btnBCC;
	CButton	m_btnCC;
	CButton	m_btnTo;
	CListBox	m_ctlAddressList;
	CString	m_strSelection;
	//}}AFX_DATA
    CComposeWnd* m_pComposeWnd;
    CMailBoxDoc* m_pDoc;
    
// Implementation
protected:  
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CAddressBook)
	afx_msg void OnTo();
	afx_msg void OnCC();
	afx_msg void OnBCC();
	afx_msg void OnAddAddress();
	afx_msg void OnRemoveAddress();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAddAddress dialog

class CAddAddress : public CDialog
{
// Construction
public:
	CAddAddress(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAddAddress)
	enum { IDD = IDD_ADD_ADDRESS };
	CString	m_strAddress;
	//}}AFX_DATA

// Implementation
protected:                              
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CAddAddress)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailSignature dialog

class CMailSignature : public CDialog
{
// Construction
public:
	CMailSignature(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMailSignature)
	enum { IDD = IDD_MAIL_SIGNATURE };
	CEdit	m_ctlSignature;
	CString	m_strSignature;
	//}}AFX_DATA

// Implementation
protected: 
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailSignature)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CArticleAge dialog

class CArticleAge : public CDialog
{
// Construction
public:
	CArticleAge(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CArticleAge)
	enum { IDD = IDD_ARTICLE_AGE };
	long	m_lDays;
	int		m_nHours;
	int		m_nMinutes;
	int		m_nSeconds;
	//}}AFX_DATA

// Implementation
protected: 
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CArticleAge)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInetnavWelcomeDlg dialog

class CInetnavWelcomeDlg : public CDialog
{
// Construction
public:
	CInetnavWelcomeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInetnavWelcomeDlg)
	enum { IDD = IDD_INETNAV_WELCOME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInetnavWelcomeDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFTPUserDir dialog

class CFTPUserDir : public CDialog
{
// Construction
public:
	CFTPUserDir(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFTPUserDir)
	enum { IDD = IDD_FTP_USER_DIR };
	CString	m_strDirectory;
	//}}AFX_DATA

// Implementation
protected:  
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFTPUserDir)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; 

/////////////////////////////////////////////////////////////////////////////
// CFlashConnectBegin dialog

class CFlashConnectBegin : public CDialog
{
// Construction
public:
	CFlashConnectBegin(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashConnectBegin)
	enum { IDD = IDD_FLASHCONNECT_BEGIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected: 
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFlashConnectBegin)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFlashConnectEnd dialog

class CFlashConnectEnd : public CDialog
{
// Construction
public:
	CFlashConnectEnd(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashConnectEnd)
	enum { IDD = IDD_FLASHCONNECT_END };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFlashConnectEnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COpenFtpSite dialog

class COpenFtpSite : public CDialog
{
// Construction
public:
	COpenFtpSite(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COpenFtpSite)
	enum { IDD = IDD_OPEN_FTP_SITE };
	CButton	m_ctlUseAnonFTP;
	CEdit	m_ctlSiteAddress;
	CString	m_strSite;
	BOOL	m_bUseAnonFTP;
	//}}AFX_DATA

// Implementation
protected:     
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(COpenFtpSite)
	afx_msg void OnChangeSiteAddress();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CArticleSignature dialog

class CArticleSignature : public CDialog
{
// Construction
public:
	CArticleSignature(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CArticleSignature)
	enum { IDD = IDD_NEWS_SIGNATURE };
	CEdit	m_ctlSignature;
	CString	m_strSignature;
	//}}AFX_DATA

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CArticleSignature)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};    

/////////////////////////////////////////////////////////////////////////////
// CDirectoryDialog dialog: a special file dialog which allows the user to
// choose a directory

class CDirectoryDialog : public CFileDialog
{
// Construction
public:
	CDirectoryDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, 
											// FALSE for FileSaveAs
               LPCSTR lpszDefExt = NULL,
               LPCSTR lpszFileName = NULL,
               DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
               LPCSTR lpszFilter = NULL,
               CWnd* pParentWnd = NULL);

// Dialog Data
	//{{AFX_DATA(CDirectoryDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
   BOOL m_bDlgJustCameUp;
   
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDirectoryDialog)
		virtual BOOL OnInitDialog(); 
		afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDLComplete dialog

class CDLComplete : public CDialog
{
// Construction
public:
	CDLComplete(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDLComplete)
	enum { IDD = IDD_DOWNLOAD_COMPLETE };
	CString	m_strFileDL;
	//}}AFX_DATA  
	

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDLComplete)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; 

/////////////////////////////////////////////////////////////////////////////
// CFtpDisconnected dialog

class CFtpDisconnected : public CDialog
{
// Construction
public:
	CFtpDisconnected(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFtpDisconnected)
	enum { IDD = IDD_DISCONNECTED };
	CString	m_strMsg;
	//}}AFX_DATA       
	CString m_strSiteName;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFtpDisconnected)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFileDownload dialog

class CFileDownload : public CDialog
{
// Construction
public:
	CFileDownload(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileDownload)
	enum { IDD = IDD_DOWNLOAD_FILE };
	CString	m_strFileName;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFileDownload)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CConnectMsg dialog

class CConnectMsg : public CDialog
{
// Construction
public:
	CConnectMsg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CConnectMsg)
	enum { IDD = IDD_CONNECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CConnectMsg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAddrChoose dialog -- allows the user to quickly pick an e-mail address
// from their address book when they use the Compose Mail Wizard.

class CAddrChoose : public CDialog
{
// Construction
public:
	CAddrChoose(CWnd* pParent, CComposeView* pView);

// Dialog Data
	//{{AFX_DATA(CAddrChoose)
	enum { IDD = IDD_CHOOSE_RECIPIENT };
	CListBox	m_ctlAddress;
	CString	m_strAddress;
	//}}AFX_DATA                                    
	CComposeView* m_pComposeView;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CAddrChoose)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkAddressList();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};   

/////////////////////////////////////////////////////////////////////////////
// COpenHelpDlg dialog

class COpenHelpDlg : public CDialog
{
// Construction
public:
	COpenHelpDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COpenHelpDlg)
	enum { IDD = IDD_OPEN_HELP_FILE };
	int		m_nSelection;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(COpenHelpDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFtpPrefsAdvanced dialog

class CFtpPrefsAdvanced : public CDialog
{
// Construction
public:
	CFtpPrefsAdvanced(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFtpPrefsAdvanced)
	enum { IDD = IDD_FTP_PREFS_ADVANCED };
	CString	m_strDLDir;
	CInternetNavApp*	m_pApp;
	CString	m_strULDir;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFtpPrefsAdvanced)
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnDirbrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFlashConnectAbort dialog

class CFlashConnectAbort : public CDialog
{
// Construction
public:
	CFlashConnectAbort(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashConnectAbort)
	enum { IDD = IDD_FLASHCONNECT_ABORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFlashConnectAbort)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInternetMailboxes dialog

class CInternetMailboxes : public CDialog
{
// Construction
public:
	CInternetMailboxes(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInternetMailboxes)
	enum { IDD = IDD_INTERNET_MAILBOXES };
	CListBox	m_ctlMailboxList;
	CButton	m_btnSetDefMailbox;
	CString	m_strSelection;
	CString	m_strDefMailbox;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CInternetMailboxes)
	afx_msg void OnAddMailbox();
	afx_msg void OnRemoveMailbox();
	afx_msg void OnSetDefaultMailbox();
	afx_msg void OnDblclkMailboxList();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailUserInfo dialog

class CMailUserInfo : public CDialog
{
// Construction
public:
	CMailUserInfo(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMailUserInfo)
	enum { IDD = IDD_MAIL_USER_INFORMATION };
	CString	m_strOrg;
	CString	m_strUser;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailUserInfo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFtpViruses dialog

class CFtpViruses : public CDialog
{
// Construction
public:
	CFtpViruses(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFtpViruses)
	enum { IDD = IDD_FTP_VIRUSES };
	CString	m_strPath;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFtpViruses)
	afx_msg void OnBrowse();
	afx_msg void OnDisclaimer();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};      

/////////////////////////////////////////////////////////////////////////////
// CFtpVirusScan dialog

class CFtpVirusScan : public CDialog
{
// Construction
public:
	CFtpVirusScan(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFtpVirusScan)
	enum { IDD = IDD_FTP_VIRUSCAN };
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFtpVirusScan)
	afx_msg void OnAbort();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CArticleOptions dialog

class CArticleOptions : public CDialog
{
// Construction
public:
	CArticleOptions(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CArticleOptions)
	enum { IDD = IDD_ARTICLE_OPTIONS };
	int		m_nDisplay;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CArticleOptions)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};  

/////////////////////////////////////////////////////////////////////////////
#endif //__DIALOGS_H__
