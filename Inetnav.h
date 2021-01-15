// inetnav.h : main header file for the INETNAV application
//

#ifndef __INETNAV_H__
#define __INETNAV_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "splash.h"
#include "mainfrm.h"

// Forward declaration of application class
class CInternetNavApp;

/////////////////////////////////////////////////////////////////////////////
// CUserData -- information about the user.  This data structure class is to
// be used with the User Information Wizard

class CUserData
{
// Construction/Destruction
public:
	CUserData();
	~CUserData();

// Attributes
public:
	CString m_strUserName, m_strUserCompany;

public:
	CString& GetUserName() { return m_strUserName; }
	CString& GetUserCompany() { return m_strUserCompany; }

	CInternetNavApp* m_pApp;
		
// Implementation
public:
	void Attach(CInternetNavApp* pApp) { m_pApp = pApp; }
	void Detach() { m_pApp = NULL; }
	
	void Delete(CString& strDelete) { strDelete.Empty(); }
	void DeleteContents();     
	
	void OnWizardInit();
	void OnWizardCancel();
	void OnWizardFinish();

// Operations
public:
	void Serialize(BOOL IsStoring);

};                  

/////////////////////////////////////////////////////////////////////////////
// CInetnavPreferences data structure class
// This class contains variables for preferences in Internet Navigator
// and manages serialization of preferences to and from INI file

class CInetnavPreferences
{
friend class CInternetNavApp;
// Construction
	CInetnavPreferences();	// default constructor

	void Initialize(CInternetNavApp* pApp = NULL);
	
// Attributes
protected:
	CInternetNavApp*	m_pApp;		// pointer to application class

	BOOL			m_bDisplayGopherDocs;
	BOOL            m_bShowMainMenu, m_bIRCIsConnected;
	BOOL            m_bStartOnline, m_bFTPOpenASCII;
	BOOL			m_bFTPOpenBin, m_bAskFingerAddr;
	BOOL			m_bWarnFTPOpenBin, m_bConfirmExit, m_bShowTOD;  
	BOOL			m_bConfirmSignOff;  
	BOOL			m_bFingerRQP, m_bMailSendConfirm;
	BOOL   			m_bStartCheckMail, m_bMailSetupNew, m_bMailSendClose;
	
	BOOL			m_bChatNotifyArrive, m_bChatNotifyLeave;
	CString			m_strLocalHost, m_strOnlineHost;
	
	CString         m_strGopherDocDir; // Directory to store Gopher docs
	CString         m_strIRCServerName, m_strNickname, m_strInitialChannel;
	CString			m_strInitialChannelPref;
	CString			m_strFTPDownloadDir, m_strFTPUploadDir, m_strVirusPath;
	CString			m_strIRCBackupServerName;
	CString			m_strFilesDirectory;
	UINT            m_nIRCPort;
	int				m_nShowTips, m_nArticleHours, m_nArticleMinutes; 
	int		        m_nArticleSeconds;
	LONG    		m_lArticleDays;
	
	BOOL			m_bAutoFingerPrint, m_bAutoFingerSave;
	BOOL			m_bWhoisRQP, m_bAutoWhoisPrint, m_bAutoWhoisSave;
	
	BOOL			m_bGopherShowSaveAs;    
	int				m_nNNTPArticleDisplay;  
	UINT			m_nInetTimeOut;
	    				
// Operations
public:		// Helper functions for accessing and setting preferences

	int GetNNTPArticleDisplay() { return m_nNNTPArticleDisplay; }
	void SetNNTPArticleDisplay(int nDisp = 1)
		{ m_nNNTPArticleDisplay = nDisp; }
	
	BOOL GetCheckMailStartup() { return m_bStartCheckMail; }
	void SetCheckMailStartup(BOOL b = TRUE)
		{ m_bStartCheckMail = b; }
		
	CString& GetGopherDocDir() { return m_strGopherDocDir; }
	void SetGopherDocDir(const CString& strDir)
		{ m_strGopherDocDir = strDir;}    

	BOOL IsGopherShowSaveAs() { return m_bGopherShowSaveAs; }
	void SetGopherShowSaveAs(BOOL b = TRUE)
		{ m_bGopherShowSaveAs = b; }
		
	CString& GetFTPDownloadDir() { return m_strFTPDownloadDir; }
	void SetFTPDownloadDir(const CString& strDir)
		{ m_strFTPDownloadDir = strDir; }  
		
	CString& GetFTPUploadDir() { return m_strFTPUploadDir; }
	void SetFTPUploadDir(const CString& strDir)
		{ m_strFTPUploadDir = strDir; }  
		
	CString& GetVirusScannerPath() { return m_strVirusPath; }
	void SetVirusScannerPath(const CString& strPath)
		{ m_strVirusPath = strPath; }
    	
    CString& GetFilesDirectory()
    	{ return m_strFilesDirectory; }
    void SetFilesDirectory(const CString& strDir) 
    	{ m_strFilesDirectory = strDir; }

	BOOL IRCIsConnected() { return m_bIRCIsConnected; }
	void SetIRCConnected(const BOOL bConnected) 
		{ m_bIRCIsConnected = bConnected; }
	
	CString& GetIRCServerName() { return m_strIRCServerName; }
	CString& GetNickname() { return m_strNickname; }
	CString& GetInitialChannel() { return m_strInitialChannel; }
	CString& GetIRCBackupServerName() { return m_strIRCBackupServerName; }
	CString& GetLocalHostID() { return m_strLocalHost; }
	CString& GetOnlineHostID() { return m_strOnlineHost; }
	
	BOOL IsChatNotifyArrive() { return m_bChatNotifyArrive; }
	void SetChatNotifyArrive(BOOL b = TRUE)
		{ m_bChatNotifyArrive = b; }
		
	BOOL IsChatNotifyLeave() { return m_bChatNotifyLeave; }
	void SetChatNotifyLeave(BOOL b = TRUE) { m_bChatNotifyLeave = b; }
	
	void SetIRCServerName(const CString& strIRCServerName) 
		{ m_strIRCServerName = strIRCServerName; } 
	void SetNickname(const CString& strNickname) 
		{ m_strNickname = strNickname; }
	void SetInitialChannel(const CString& strChannel)
		{ m_strInitialChannel = strChannel; } 
	void SetIRCBackupServerName(const CString& strIRCBackupServer)
		{ m_strIRCBackupServerName = strIRCBackupServer; }         
	void SetLocalHostID(const CString& strID)
		{ m_strLocalHost = strID; }  
	void SetOnlineHostID(const CString& strID)
		{ m_strOnlineHost = strID; }
		          
	UINT GetIRCPort() { return m_nIRCPort; }
	void SetIRCPort(const UINT nPort) { m_nIRCPort = nPort; }
    
    int GetShowTips() { return m_nShowTips; }
    void SetShowTips(int nShowTips = 0)
    	{ m_nShowTips = nShowTips; }
	
	BOOL StartOnline() { return m_bStartOnline; } 
	void SetStartOnline(const BOOL b = TRUE)
		{ m_bStartOnline = b; }

	BOOL IsAskFingerAddr() { return m_bAskFingerAddr; }     
	void SetAskFingerAddr(const BOOL bAsk) { m_bAskFingerAddr = bAsk; } 
	
	BOOL IsAutoFingerPrint() { return m_bAutoFingerPrint; }
	void SetAutoFingerPrint(const BOOL b) { m_bAutoFingerPrint = b; }
	
	BOOL IsAutoFingerSave() { return m_bAutoFingerSave; }
	void SetAutoFingerSave(const BOOL b = TRUE)
		{ m_bAutoFingerSave = b; }
		   
    BOOL IsFingerReportQueryProgress() { return m_bFingerRQP; }
    void SetFingerReportQueryProgress(BOOL bReport = TRUE)
    	{ m_bFingerRQP = bReport; }      	
	
	BOOL IsWhoisRQP() { return m_bWhoisRQP; }
	void SetWhoisRQP(const BOOL bReport = TRUE) { m_bWhoisRQP = bReport; } 
	
	BOOL IsAutoWhoisSave() { return m_bAutoWhoisSave; }
	void SetAutoWhoisSave(const BOOL bSave = TRUE) 
		{ m_bAutoWhoisSave = bSave; }
	
	BOOL IsAutoWhoisPrint() { return m_bAutoWhoisPrint; }
	void SetAutoWhoisPrint(const BOOL b = TRUE) { m_bAutoWhoisPrint = b; }   
	
	BOOL IsFTPOpenASCII() { return m_bFTPOpenASCII; }
	void SetFTPOpenASCII(const BOOL bOpen = TRUE) 
		{ m_bFTPOpenASCII = bOpen; }
	
	BOOL IsFTPOpenBin() { return m_bFTPOpenBin; }
	void SetFTPOpenBin(const BOOL bOpen = TRUE) 
		{ m_bFTPOpenBin = bOpen; }

	BOOL IsWarnFTPOpenBin() { return m_bWarnFTPOpenBin; }
	void SetWarnFTPOpenBin(BOOL bWarn = TRUE) { m_bWarnFTPOpenBin = bWarn; }
	
	BOOL IsDisplayGopherDocs() { return m_bDisplayGopherDocs; }
	void SetDisplayGopherDocs(BOOL b = TRUE) { m_bDisplayGopherDocs = b; } 

	BOOL IsConfirmSignOff() { return m_bConfirmSignOff; }
	void SetConfirmSignOff(BOOL b = TRUE) { m_bConfirmSignOff = b; }
    
    BOOL IsConfirmExit() { return m_bConfirmExit; }
    void SetConfirmExit(BOOL b = TRUE) { m_bConfirmExit = b; }

    BOOL IsShowTOD() { return m_bShowTOD; }
    void SetShowTOD(BOOL bShowTOD = TRUE) { m_bShowTOD = bShowTOD; }
    
    BOOL IsMailSendConfirm() { return m_bMailSendConfirm; }
    void SetMailSendConfirm(const BOOL b = TRUE)
    	{	m_bMailSendConfirm = b; }
    
    BOOL IsMailSendClose() { return m_bMailSendClose; } 
    void SetMailSendClose(const BOOL b = TRUE)
    	{	m_bMailSendClose = b; }
    	
    BOOL IsMailSetupNew() { return m_bMailSetupNew; }
    void SetMailSetupNew(const BOOL b = TRUE)
    	{	m_bMailSetupNew = b; }
    
	int GetArticleSeconds() { return m_nArticleSeconds; }
	int GetArticleMinutes() { return m_nArticleMinutes; }
	int GetArticleHours() 	{ return m_nArticleHours; }
	LONG GetArticleDays() 	{ return m_lArticleDays; }
	
	void SetArticleSeconds(const int nSecs) { m_nArticleSeconds = nSecs; } 
	void SetArticleMinutes(const int nMins) { m_nArticleMinutes = nMins; }
	void SetArticleHours(const int nHours)  { m_nArticleHours = nHours; }
	void SetArticleDays(const LONG lDays) { m_lArticleDays = lDays; } 
	
	BOOL IsShowMainMenu() { return m_bShowMainMenu; }
	void SetShowMainMenu(const BOOL b = TRUE)
		{ m_bShowMainMenu = b; }   
	
	CString& GetInitialChannelPref() { return m_strInitialChannelPref; }
	void SetInitialChannelPref(const CString& strChannel)
		{ m_strInitialChannelPref = strChannel; }
	
	UINT GetInetTimeOut() { return m_nInetTimeOut; }
	void SetInetTimeOut(UINT nTimeOut = 20)
		{	m_nInetTimeOut = nTimeOut;	}

// Implementation
public:		// Serialization
	void Serialize(BOOL IsStoring = TRUE);
	
	// Serialize() helpers
	void SaveProfileSettings();
	void LoadProfileSettings();
};

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp:
// See inetnav.cpp for the implementation of this class
//

class CInternetNavApp : public CWinApp
{             
friend class CUserData;
friend class CMainFrame;
friend class CMainMenuView;
friend class CMainMenuFrm; 
friend class CHelpInfoView;
friend class CHelpInfoWnd;           
friend class CPrefsWnd;
friend class CSplashWnd;   
friend class CAppGeneralPrefs;

friend class CAskStaffDoc;
friend class CAskStaffView;
friend class CAskStaffWnd;

// we want the Internet area classes to have access to the protected member
// variables of this class
friend class CFingerView;
friend class CFingerWnd;
friend class CFingerDoc;

friend class CNewsWnd;
friend class CNewsView;
friend class CNewsDoc;

friend class CFtpView;
friend class CFtpWnd;
friend class CFtpDoc;

friend class CWhoisView;
friend class CWhoisDoc;
friend class CWhoisWnd;    

friend class CGopherCenter;
friend class CGophCenterWnd;
friend class CGopherDoc;

// Construction             
public:
	CInternetNavApp();

// Overrides
public:
	virtual BOOL InitInstance(); 
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	virtual int ExitInstance();
	
	// We are overriding DoPromptFileName here so we can make sure that
	// the Open or Save As dialog box displayed is switched over to 
	// the C:\INETNAV\FILES\ directory and centered on the screen
	BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
			
	// We are overriding AddToRecentFileList here so we can make sure the
	// list is written out to the INI file each time a file is added to the
	// list.
	virtual void AddToRecentFileList(const char* pszPathName);

// Operations
public:   // Public Gopher document/menu/search helpers  
	void OpenGopherMenu(const char* pszSelector);
	void OpenGopherDocument(const char* pszSelector);
	void OpenGopherSearch(const char* pszSelector);    
	
	// Public socket helpers
	QSocket* GetNewConnection(BOOL bCreateSocket = TRUE);    
	
	// for keeping track of the IRC channels the user has open.
	void AddChanWindow(const CView* pNewView, const CString& strChannel);
	BOOL RemoveChanWindow(const CString& strChannel);
	BOOL ChangeChanWindow(const CString& strOldChannel,
				const CString& strNewChannel);	
				
	// Mailbox management
	BOOL AddNewMailbox();	// creates a new mailbox
	BOOL RegisterMailbox(const CString& strName, const CString& strPath,
						const CString& strAddr, const CString& strUser,
					const CString& strOrg);
	
	BOOL RemoveMailboxName(const CString& strName);
	BOOL RemoveMailboxPath(const CString& strPath);
	BOOL RemoveMailbox(const CString& strName, const CString& strPath,
							const CString& strAddr, const CString& strUser,
						const CString& strOrg);
	
	BOOL OpenDefaultMailbox(BOOL bCompose = FALSE);
	
	BOOL OpenMailboxName(const CString& strName);
	BOOL OpenMailboxPath(const CString& strPath);
	
	void SetDefaultMailbox(const CString& strName, const CString& strPath,
		const CString& strAddress, const CString& strUser,
			const CString& strOrg);
	
	BOOL IsDefaultMailbox(const CString& strName, const CString& strPath);

	CString GetMailboxName(const CString& strPath);
	CString GetMailboxPath(const CString& strName);
	CString GetMailboxAddress(const CString& strName); 
	CString GetMailboxUser(const CString& strName);
	CString GetMailboxOrg(const CString& strOrg);

	void LoadMailboxes();
	void SaveMailboxes();

// Attributes     
public:
	CMapStringToOb* m_pIRCChanList;    
	CInetnavStatusBar* m_pStatusBar;                 
	CPrefsWnd* m_pWndPrefs;
	CAskStaffWnd* m_pWndAskStaff;  
	CUserData m_userData;    
	BOOL m_bSaveUserData;  
	CNewsWnd* m_pNewsWnd; 
	
	BOOL m_bToolTipsOff;           
	
	// Map of mailboxes -- maps mailbox names to pathnames and back
	CMapStringToString m_mapMailboxNameToPath;
	CMapStringToString m_mapMailboxPathToName;
	CMapStringToString m_mapMailboxNameToAddress; 
	CMapStringToString m_mapMailboxNameToUserName;
	CMapStringToString m_mapMailboxNameToOrg;  
	
	CStringArray 	m_strMailboxPathList;
	CStringArray	m_strMailboxNameList;
	CStringArray	m_strMailboxAddressList;
	CStringArray	m_strMailboxUserNameList;
	CStringArray	m_strMailboxOrgList;
	
	// Other mailbox variables
	CString m_strDefMailboxName, m_strDefMailboxPath, m_strDefAddress;
	CString m_strDefMailboxUser, m_strDefMailboxOrg;
	
	// Variable for whether or not we're on Windows 95
	BOOL m_bWindows95;
	
	// Name of newsgroups server for Internet Newsgroups area to use
	CString m_strNNTPHost; 
	
	// Internet Host Name of this computer
	CString m_strHostName; 

protected:	// Data structures and application component pointers, and prefs
	CMainFrame* m_pMainFrame; // pointer to main window
	WSADATA     m_WSADATA; // socket initialization structure
	CSplashWnd  m_wndSplash; // window which is displayed on startup
	
	CInetnavPreferences m_prefs; // Preferences Manager class

	// Pointers to document types
	CMultiDocTemplate *m_pFinger, *m_pWhois, *m_pFTP, *m_pMainMenu;
	CMultiDocTemplate *m_pTextTXT, *m_pIRCConverse, *m_pGopherMenu;
	CMultiDocTemplate *m_pGopherDocument, *m_pGopherCenter, *m_pHelpInfoArea;
	CMultiDocTemplate *m_pPrefs, *m_pMailBox, *m_pInbox, *m_pMailMessage;
	CMultiDocTemplate *m_pMailCompose, *m_pInternetNewsgroups;
	CMultiDocTemplate *m_pNewsgroupsManager, *m_pNewsgroup, *m_pNewsArticle;
	CMultiDocTemplate *m_pComposeNewsArticle, *m_pAskStaff;           
	
	CMainMenuFrm*   m_pWndMainMenu;
	CGophCenterWnd* m_pWndGophCenter;
	CHelpInfoWnd*   m_pWndHelpInfo; 
	QSocket*        m_pIRCSocket;   
	
	BOOL			m_bFlashConnect, m_bOnline, m_bPseudoOnline;       
	BOOL			m_bFlashConnectError;
	BOOL			m_bContinueCursor, m_bPseudoContinueCursor;
	BOOL			m_bGopherSearch, m_bUseCTL3D, m_bPseudoShowTOD;

	// Preferences and other settings are in the class, defined above, called
	// CInetnavPreferences.  CInetnavPreferences is a class which manages
	// Internet Navigator's preferences and serialization of preferences
	// to and from the INI file

	// Gopher search information
	CString m_strSearchTitle, m_strSearchSelector, m_strSearchServer;
	CString m_strSearchQuery;
	UINT m_nSearchPort;

public: // Helper functions to allow access to protected members from other     
			// classes     
	
	CString GetDefaultAddress() { return m_strDefAddress; }
		
	CMainFrame* GetMainFrame() { return m_pMainFrame; }
	  
	CUserData* GetUserData() { return &m_userData; }
	CInetnavPreferences* GetPrefs() { return &m_prefs; }
	
	WSADATA* GetWSADATA() { return &m_WSADATA; }  

	// Allow other classes to access each document type
	CMultiDocTemplate* GetFingerType() { return m_pFinger; }
	CMultiDocTemplate* GetWhoisType() { return m_pWhois; }
	CMultiDocTemplate* GetFtpType() { return m_pFTP; } 
	CMultiDocTemplate* GetMainMenu() { return m_pMainMenu; } 
	CMultiDocTemplate* GetTXTType() { return m_pTextTXT; }
	CMultiDocTemplate* GetIRCConversation() { return m_pIRCConverse; } 
	CMultiDocTemplate* GetGopherMenu() { return m_pGopherMenu; }
	CMultiDocTemplate* GetGopherDocument() { return m_pGopherDocument; }
	CMultiDocTemplate* GetGopherCenter() { return m_pGopherCenter; }
	CMultiDocTemplate* GetHelpAndInfoArea() { return m_pHelpInfoArea; }   
	CMultiDocTemplate* GetPrefsType() { return m_pPrefs; }
	CMultiDocTemplate* GetMailBoxType() { return m_pMailBox; }
	CMultiDocTemplate* GetInboxType() { return m_pInbox; }
	CMultiDocTemplate* GetMessageType() { return m_pMailMessage; }
	CMultiDocTemplate* GetComposeMailType() { return m_pMailCompose; }
	CMultiDocTemplate* GetInternetNews() { return m_pInternetNewsgroups; } 
	CMultiDocTemplate* GetNewsgroupsManager() { return m_pNewsgroupsManager; }
	CMultiDocTemplate* GetNewsgroup() { return m_pNewsgroup; }   
	CMultiDocTemplate* GetNewsArticle() { return m_pNewsArticle; }
	CMultiDocTemplate* GetCNA() { return m_pComposeNewsArticle; }
	CMultiDocTemplate* GetAskStaff() { return m_pAskStaff; }
	
	// Allow splash window to get user information and display it
	CString& GetUserName() { return m_userData.m_strUserName; }
	CString& GetUserCompany() { return m_userData.m_strUserCompany; }
	
	CStringArray& GetMailboxNames() { return m_strMailboxNameList; }
	CStringArray& GetMailboxPaths() { return m_strMailboxPathList; } 
	
	QSocket* GetIRCSocket() { return m_pIRCSocket; } 
	void SetIRCSocket(QSocket* pIRCSocket)
		{ m_pIRCSocket = pIRCSocket; }
    
    BOOL IsWindows95() { return m_bWindows95; }
    void SetWindows95(BOOL bWindows95 = TRUE) 
    	{ m_bWindows95 = bWindows95; }

	BOOL IsOnline() { return m_bOnline; } 
	    	
	BOOL IsGopherSearch() { return m_bGopherSearch; }
	void SetGopherSearch(const BOOL bSearch) { m_bGopherSearch = bSearch; }

	int GetNNTPArticleDisplay() { return GetPrefs()->GetNNTPArticleDisplay(); }
	void SetNNTPArticleDisplay(int nDisp = 1)
		{ GetPrefs()->SetNNTPArticleDisplay(nDisp); }
		
	BOOL IsCheckMailStartup() { return GetPrefs()->GetCheckMailStartup(); }
	void SetCheckMailStartup(BOOL b = TRUE)
		{ GetPrefs()->SetCheckMailStartup(b); }	
		
	CString& GetGopherDocDir() { return GetPrefs()->GetGopherDocDir(); }
	void SetGopherDocDir(const CString& strDir)
		{ GetPrefs()->SetGopherDocDir(strDir);}  

	BOOL IsGopherShowSaveAs() { return GetPrefs()->IsGopherShowSaveAs(); }
	void SetGopherShowSaveAs(BOOL b = TRUE)
		{ GetPrefs()->SetGopherShowSaveAs(b); }
			
	CString& GetFTPDownloadDir() { return GetPrefs()->GetFTPDownloadDir(); }
	void SetFTPDownloadDir(const CString& strDir)
		{ GetPrefs()->SetFTPDownloadDir(strDir); }
		
	CString& GetFTPUploadDir() { return GetPrefs()->GetFTPUploadDir(); }
	void SetFTPUploadDir(const CString& strDir)
		{ GetPrefs()->SetFTPUploadDir(strDir); }       
		
	CString& GetVirusScannerPath() { return GetPrefs()->GetVirusScannerPath(); }
	void SetVirusScannerPath(const CString& s)
		{	GetPrefs()->SetVirusScannerPath(s); }
    	
    CString& GetFilesDirectory()
    	{ return GetPrefs()->GetFilesDirectory(); }
    void SetFilesDirectory(const CString& strDir) 
    	{ GetPrefs()->SetFilesDirectory(strDir); }

	BOOL IRCIsConnected() { return GetPrefs()->IRCIsConnected(); }
	void SetIRCConnected(const BOOL bConnected = TRUE) 
		{ GetPrefs()->SetIRCConnected(bConnected); }
	
	CString& GetIRCServerName() { return GetPrefs()->GetIRCServerName(); }
	CString& GetNickname() { return GetPrefs()->GetNickname(); }
	CString& GetInitialChannel() { return GetPrefs()->GetInitialChannel(); }
	CString& GetIRCBackupServerName() 
		{ return GetPrefs()->GetIRCBackupServerName(); }    
	CString& GetLocalHostID() { return GetPrefs()->GetLocalHostID(); }
	CString& GetOnlineHostID() { return GetPrefs()->GetOnlineHostID(); }
	
	BOOL IsChatNotifyArrive() { return GetPrefs()->IsChatNotifyArrive(); }
	void SetChatNotifyArrive(BOOL b = TRUE)
		{ GetPrefs()->SetChatNotifyArrive(b); }
		
	BOOL IsChatNotifyLeave() { return GetPrefs()->IsChatNotifyLeave(); }
	void SetChatNotifyLeave(BOOL b = TRUE) 
		{ GetPrefs()->SetChatNotifyLeave(b); }
	
	void SetIRCServerName(const CString& strIRCServerName) 
		{ GetPrefs()->SetIRCServerName(strIRCServerName); } 
	void SetNickname(const CString& strNickname) 
		{ GetPrefs()->SetNickname(strNickname); }
	void SetInitialChannel(const CString& strChannel)
		{ GetPrefs()->SetInitialChannel(strChannel); } 
	void SetIRCBackupServerName(const CString& strIRCBackupServer)
		{ GetPrefs()->SetIRCBackupServerName(strIRCBackupServer); }
	void SetLocalHostID(const CString& strID)
		{ GetPrefs()->SetLocalHostID(strID); }  
	void SetOnlineHostID(const CString& strID)
		{ GetPrefs()->SetOnlineHostID(strID); }        
		          
	UINT GetIRCPort() { return GetPrefs()->GetIRCPort(); }
	void SetIRCPort(const UINT nPort = 6667) 
		{ GetPrefs()->SetIRCPort(nPort); }
    
    int GetShowTips() { return GetPrefs()->GetShowTips(); }
    void SetShowTips(int nShowTips = 0)
    	{ GetPrefs()->SetShowTips(nShowTips); }
	
	BOOL StartOnline() { return GetPrefs()->StartOnline(); } 
	void SetStartOnline(const BOOL b = TRUE)
		{ GetPrefs()->SetStartOnline(b); } 
		
	BOOL IsShowMainMenu() { return GetPrefs()->IsShowMainMenu(); }
	void SetShowMainMenu(const BOOL b = TRUE)
		{ GetPrefs()->SetShowMainMenu(b); }
	
	BOOL IsAskFingerAddr() { return GetPrefs()->IsAskFingerAddr(); }     
	void SetAskFingerAddr(const BOOL bAsk = TRUE) 
		{ GetPrefs()->SetAskFingerAddr(bAsk); }
	
	BOOL IsAutoFingerPrint() { return GetPrefs()->IsAutoFingerPrint(); }
	void SetAutoFingerPrint(const BOOL b) 
		{ GetPrefs()->SetAutoFingerPrint(b); }
	
	BOOL IsAutoFingerSave() { return GetPrefs()->IsAutoFingerSave(); }
	void SetAutoFingerSave(const BOOL b = TRUE)
		{ GetPrefs()->SetAutoFingerSave(b); }
		   	   
    BOOL IsFingerReportQueryProgress()
    	{ return GetPrefs()->IsFingerReportQueryProgress(); }
    void SetFingerReportQueryProgress(BOOL bReport = TRUE)
    	{ GetPrefs()->SetFingerReportQueryProgress(bReport); }      	
	
	BOOL IsWhoisRQP() { return GetPrefs()->IsWhoisRQP(); }
	void SetWhoisRQP(const BOOL bReport = TRUE) 
		{ GetPrefs()->SetWhoisRQP(bReport); } 
	
	BOOL IsAutoWhoisSave() { return GetPrefs()->IsAutoWhoisSave(); }
	void SetAutoWhoisSave(const BOOL bSave = TRUE) 
		{ GetPrefs()->SetAutoWhoisSave(bSave); }
	
	BOOL IsAutoWhoisPrint() { return GetPrefs()->IsAutoWhoisPrint(); }
	void SetAutoWhoisPrint(const BOOL b = TRUE) 
		{ GetPrefs()->SetAutoWhoisPrint(b); }   
		
	BOOL IsFTPOpenASCII() { return GetPrefs()->IsFTPOpenASCII(); }
	void SetFTPOpenASCII(const BOOL bOpen = TRUE) 
		{ GetPrefs()->SetFTPOpenASCII(bOpen); }
	
	BOOL IsFTPOpenBin() { return GetPrefs()->IsFTPOpenBin(); }
	void SetFTPOpenBin(const BOOL bOpen = TRUE) 
		{ GetPrefs()->SetFTPOpenBin(bOpen); } 
	
	BOOL IsDisplayGopherDocs() { return GetPrefs()->IsDisplayGopherDocs(); }
	void SetDisplayGopherDocs(BOOL b = TRUE)
		{ GetPrefs()->SetDisplayGopherDocs(b); }
	
	BOOL IsPseudoOnline() { return m_bPseudoOnline; }
	void SetPseudoOnline(const BOOL bOL = TRUE) { m_bPseudoOnline = bOL; }
	
	BOOL IsUseCTL3D() { return m_bUseCTL3D; }
	void UseCTL3D(const BOOL bUseCTL3D) 
		{ m_bUseCTL3D = bUseCTL3D; }
	
	BOOL IsWarnFTPOpenBin() { return GetPrefs()->IsWarnFTPOpenBin(); }
	void SetWarnFTPOpenBin(BOOL bWarn = TRUE) 
		{ GetPrefs()->SetWarnFTPOpenBin(bWarn); } 

	BOOL IsConfirmSignOff() { return GetPrefs()->IsConfirmSignOff(); }
	void SetConfirmSignOff(BOOL b = TRUE) 
		{ GetPrefs()->SetConfirmSignOff(b); }
    
    BOOL IsConfirmExit() { return GetPrefs()->IsConfirmExit(); }
    void SetConfirmExit(BOOL b = TRUE) 
    	{ GetPrefs()->SetConfirmExit(b); }

    BOOL IsShowTOD() { return GetPrefs()->IsShowTOD(); }
    void SetShowTOD(BOOL bShowTOD = TRUE) 
    	{ GetPrefs()->SetShowTOD(bShowTOD); } 
    	
    BOOL IsMailSetupNew() { return GetPrefs()->IsMailSetupNew(); }
    void SetMailSetupNew(const BOOL b = TRUE)
    	{ GetPrefs()->SetMailSetupNew(b); }
    	
    BOOL IsMailSendConfirm() { return GetPrefs()->IsMailSendConfirm(); }
    void SetMailSendConfirm(BOOL b = TRUE)
    	{ GetPrefs()->SetMailSendConfirm(b); }
    
    BOOL IsMailSendClose() { return GetPrefs()->IsMailSendClose(); }
    void SetMailSendClose(BOOL b = TRUE)
    	{ GetPrefs()->SetMailSendClose(b); }
    	
	int GetArticleSeconds() { return GetPrefs()->GetArticleSeconds(); }
	int GetArticleMinutes() { return GetPrefs()->GetArticleMinutes(); }
	int GetArticleHours() 	{ return GetPrefs()->GetArticleHours(); }
	LONG GetArticleDays() 	{ return GetPrefs()->GetArticleDays(); }
	
	void SetArticleSeconds(int nSecs) 
		{ GetPrefs()->SetArticleSeconds(nSecs); } 
	void SetArticleMinutes(int nMins) 
		{ GetPrefs()->SetArticleMinutes(nMins); }
	void SetArticleHours(int nHours)  
		{ GetPrefs()->SetArticleHours(nHours); }
	void SetArticleDays(LONG lDays) 
		{ GetPrefs()->SetArticleDays(lDays); }   
		
	CString& GetInitialChannelPref() 
		{ return GetPrefs()->GetInitialChannelPref(); }
	void SetInitialChannelPref(const CString& strChannel)
		{ GetPrefs()->SetInitialChannelPref(strChannel); }
		
	CMapStringToOb* GetIRCChanList() { return m_pIRCChanList; }    
	
	CInetnavStatusBar* GetStatusBar() { return m_pStatusBar; }
	
	// Gopher search information retrieval functions
	// used by CGopherDoc.   
	CString& GetSearchTitle() { return m_strSearchTitle; }
	CString& GetSearchSelector() { return m_strSearchSelector; }
	CString& GetSearchServer() { return m_strSearchServer; }
	CString& GetSearchQuery() { return m_strSearchQuery; }
	UINT GetSearchPort() { return m_nSearchPort; }   
	
	BOOL m_bTopLevelMenu;
	BOOL IsTopLevelMenu() { return m_bTopLevelMenu; } 

	
	UINT GetInetTimeOut() { return GetPrefs()->GetInetTimeOut(); }
	void SetInetTimeOut(UINT nTimeOut = 20)
		{	GetPrefs()->SetInetTimeOut(nTimeOut);	}
	
// Implementation 
public:  
	// INI file information helper functions
	void Serialize(BOOL IsStoring);
	void LoadStdProfileSettings();
	void SaveStdProfileSettings(); 
	
	// Signon/Signoff helper functions
	BOOL SignOn();
	BOOL SignOff();
	
	// FlashConnect helper functions 
	BOOL IsFlashConnect() { return m_bFlashConnect; }
	BOOL SetFlashConnect(BOOL bFlashConnect = TRUE);
	
	BOOL AbortFlashConnect();
	
	// Help & Information helper functions
	void EnterHelpArea();
	
	// Directory chooser common dialog box helper
	CString DoDirectoryDialog(CWnd* pParent, CString& strInitialDir);    

protected:
    // Generated message map functions
	//{{AFX_MSG(CInternetNavApp)
	afx_msg void OnAppAbout();
	afx_msg void DisplayMainMenu();
	afx_msg void OnNewTextFile();
	afx_msg void OnPlacesFinger();
	afx_msg void OnPlacesWhois();
	afx_msg void OnPlacesFTPClient();
	afx_msg void OnPlacesPostOffice();
	afx_msg void OnPlacesPreferences();
	afx_msg void OnPlacesHelpAndInfo();
	afx_msg void OnPlacesChatCenter();
	afx_msg void OnPlacesNewsgroups();
	afx_msg void OnPlacesGopherCenter();
	afx_msg void OnPlacesSignOn();
	afx_msg void OnUpdatePlacesSignOn(CCmdUI* pCmdUI);
	afx_msg void OnPlacesSignOff();
	afx_msg void OnUpdatePlacesSignOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisplayMainMenu(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlacesChatCenter(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlacesFinger(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlacesFTPClient(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlacesGopherCenter(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlacesNewsgroups(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlacesWhois(CCmdUI* pCmdUI);
	afx_msg void OnAppExit();
	afx_msg void OnChatJoinGroup();
	afx_msg void OnUpdateChatJoinGroup(CCmdUI* pCmdUI);
	afx_msg void OnChatSendCommand();
	afx_msg void OnTipOfDay();
	afx_msg void OnUpdatePlacesPostOffice(CCmdUI* pCmdUI);
	afx_msg void OnMailboxCompose();
	afx_msg void OnArticleAdd();
	afx_msg void OnFileOpen();
	afx_msg void OnMailboxCheck();
	afx_msg void OnCancelCall();
	afx_msg void OnInternetMailboxes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__INETNAV_H__
