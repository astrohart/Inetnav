// inetnav.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "inetnav.h" 
#include "dialogs.h"
#include "propshts.h"

#include "mainfrm.h"

#include "finger.h"    
#include "ftp.h"
#include "text.h"
#include "gopher.h"
#include "mail.h"
#include "news.h"

#include "ircdoc.h"
#include "ircview.h"
#include "ircwnd.h"

#include "whoisdoc.h"
#include "whoiswnd.h"
#include "whoisvw.h"       

#include "mainmenu.h" // for Main Menu area
#include "helpinfo.h" // for Help & Information area

#include "prefview.h" // for Preferences area
#include "prefsdoc.h" // for Preferences area  

#include "tipofday.h"

#include "askstaff.h"  

#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp

BEGIN_MESSAGE_MAP(CInternetNavApp, CWinApp)
	//{{AFX_MSG_MAP(CInternetNavApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, DisplayMainMenu)
	ON_COMMAND(ID_NEW_TEXT, OnNewTextFile)
	ON_COMMAND(ID_PLACES_FINGER, OnPlacesFinger)
	ON_COMMAND(ID_PLACES_WHOIS, OnPlacesWhois)
	ON_COMMAND(ID_PLACES_FTP_CLIENT, OnPlacesFTPClient)
	ON_COMMAND(ID_PLACES_POST_OFFICE, OnPlacesPostOffice)
	ON_COMMAND(ID_PLACES_PREFERENCES, OnPlacesPreferences)
	ON_COMMAND(ID_PLACES_HELPINFO, OnPlacesHelpAndInfo)
	ON_COMMAND(ID_PLACES_CHAT_CENTER, OnPlacesChatCenter)
	ON_COMMAND(ID_PLACES_NEWSGROUPS, OnPlacesNewsgroups)
	ON_COMMAND(ID_PLACES_GOPHER_CENTER, OnPlacesGopherCenter)
	ON_COMMAND(ID_PLACES_SIGN_ON, OnPlacesSignOn)
	ON_UPDATE_COMMAND_UI(ID_PLACES_SIGN_ON, OnUpdatePlacesSignOn)
	ON_COMMAND(ID_PLACES_SIGN_OFF, OnPlacesSignOff)
	ON_UPDATE_COMMAND_UI(ID_PLACES_SIGN_OFF, OnUpdatePlacesSignOff)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateDisplayMainMenu)
	ON_UPDATE_COMMAND_UI(ID_PLACES_CHAT_CENTER, OnUpdatePlacesChatCenter)
	ON_UPDATE_COMMAND_UI(ID_PLACES_FINGER, OnUpdatePlacesFinger)
	ON_UPDATE_COMMAND_UI(ID_PLACES_FTP_CLIENT, OnUpdatePlacesFTPClient)
	ON_UPDATE_COMMAND_UI(ID_PLACES_GOPHER_CENTER, OnUpdatePlacesGopherCenter)
	ON_UPDATE_COMMAND_UI(ID_PLACES_NEWSGROUPS, OnUpdatePlacesNewsgroups)
	ON_UPDATE_COMMAND_UI(ID_PLACES_WHOIS, OnUpdatePlacesWhois)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_CHAT_JOIN_GROUP, OnChatJoinGroup)
	ON_UPDATE_COMMAND_UI(ID_CHAT_JOIN_GROUP, OnUpdateChatJoinGroup)
	ON_COMMAND(ID_CHAT_SEND_COMMAND, OnChatSendCommand)
	ON_COMMAND(ID_TIP_OF_DAY, OnTipOfDay)
	ON_UPDATE_COMMAND_UI(ID_PLACES_POST_OFFICE, OnUpdatePlacesPostOffice)
	ON_COMMAND(ID_MAILBOX_COMPOSE_OFFLINE, OnMailboxCompose)
	ON_COMMAND(ID_ARTICLE_ADD_OFFLINE, OnArticleAdd)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_MAILBOX_CHECK, OnMailboxCheck)
	ON_COMMAND(ID_CANCEL_CALL, OnCancelCall)
	ON_COMMAND(ID_INTERNET_MAILBOXES, OnInternetMailboxes)
	ON_COMMAND(ID_GOPHER_SITE, OnPlacesGopherCenter)
	//}}AFX_MSG_MAP
	// Standard ID_HELP command
	ON_COMMAND(ID_HELP,	CWinApp::OnHelp)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	// Default Help command -- shows Help & Info area
	ON_COMMAND(ID_DEFAULT_HELP, OnPlacesHelpAndInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp construction

CInternetNavApp::CInternetNavApp()
{
	// TODO: add construction code here
	m_pStatusBar = NULL;  
	
	m_bTopLevelMenu = TRUE;
}                        

/////////////////////////////////////////////////////////////////////////////
// The one and only CInternetNavApp object

CInternetNavApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp initialization

BOOL CInternetNavApp::InitInstance()
{
	CWaitCursor wait;
	
	// Initialize data structures
	m_userData.Attach(this);
	m_prefs.Initialize(this);
	
	m_bToolTipsOff = FALSE;
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetDialogBkColor();
	CInternetNavApp::Serialize(FALSE);  // Load information from INI
 
	UseCTL3D(Ctl3dRegister(AfxGetInstanceHandle()));
    
    if (IsUseCTL3D())
    	Ctl3dAutoSubclass(AfxGetInstanceHandle());
				      
	if (m_lpCmdLine[0] != '/' && m_lpCmdLine[1] != 'S')
	{  
		if (m_wndSplash.Create())
		{
			wait.Restore();
			m_wndSplash.ShowWindow(SW_SHOW);
			m_wndSplash.CenterWindow(CWnd::GetDesktopWindow());
			m_wndSplash.UpdateWindow();  
		}
	}

    wait.Restore();
	// create main MDI Frame window

	TRY
	{
		m_pMainFrame = new CMainFrame;
	}
	CATCH(CMemoryException, pMemoryException)
	{
		AfxMessageBox("Insufficient system resources to create main window.");
		return FALSE;
	}   
	END_CATCH
		
	if (m_lpCmdLine[0] == '/' && m_lpCmdLine[1] == 'S')
		m_pMainFrame->m_bCreateToolBar = m_pMainFrame->m_bCreateStatBar = FALSE;
	else
		m_pMainFrame->m_bCreateToolBar = m_pMainFrame->m_bCreateStatBar = TRUE;
	
	if (!m_pMainFrame->LoadFrame(IDR_MAINFRAME,
	WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL))
	{
		AfxMessageBox("Insufficient system resources to create main window.");
		return FALSE;
	}
	
	m_pMainWnd = m_pMainFrame;
	
	m_pStatusBar = GetMainFrame()->GetStatusBar();

    // If we made it this far,
	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.
	// In Internet Navigator, each document type can retrieve and display 
	// different types of information from the Internet.  
	
	m_pTextTXT = new CMultiDocTemplate(
		IDR_TEXTTYPE,
		RUNTIME_CLASS(CTextDoc),
		RUNTIME_CLASS(CTextWnd),     // custom MDI child frame
		RUNTIME_CLASS(CTextView));
	AddDocTemplate(m_pTextTXT);

	m_pFinger = new CMultiDocTemplate(
		IDR_FINGERTYPE,
		RUNTIME_CLASS(CFingerDoc),
		RUNTIME_CLASS(CFingerWnd),     // custom MDI child frame
		RUNTIME_CLASS(CFingerView));
	AddDocTemplate(m_pFinger);
	
	m_pWhois = new CMultiDocTemplate(
		IDR_WHOISTYPE,
		RUNTIME_CLASS(CWhoisDoc),
		RUNTIME_CLASS(CWhoisWnd),     // custom MDI child frame
		RUNTIME_CLASS(CWhoisView));
	AddDocTemplate(m_pWhois);

	m_pFTP = new CMultiDocTemplate(
		IDR_FTPTYPE,
		RUNTIME_CLASS(CFtpDoc),
		RUNTIME_CLASS(CFtpWnd),     // custom MDI child frame
		RUNTIME_CLASS(CFtpView));
	AddDocTemplate(m_pFTP);        

	m_pMainMenu = new CMultiDocTemplate(
		IDR_MAINMENUTYPE,
		RUNTIME_CLASS(CMainMenuDoc),
		RUNTIME_CLASS(CMainMenuFrm),     // custom MDI child frame
		RUNTIME_CLASS(CMainMenuView));
	AddDocTemplate(m_pMainMenu);

	// The Help & Information area
	m_pHelpInfoArea = new CMultiDocTemplate(
		IDR_HELPINFO,
		RUNTIME_CLASS(CHelpInfoDoc),
		RUNTIME_CLASS(CHelpInfoWnd),
		RUNTIME_CLASS(CHelpInfoView));
	AddDocTemplate(m_pHelpInfoArea);

	m_pIRCConverse = new CMultiDocTemplate(
		IDR_IRCTYPE,
		RUNTIME_CLASS(CIRClientDoc),
		RUNTIME_CLASS(CIRClientWnd),
		RUNTIME_CLASS(CIRClientView));
	AddDocTemplate(m_pIRCConverse);

    m_pGopherMenu = new CMultiDocTemplate(
	IDR_GOPHERMENU,
	RUNTIME_CLASS(CGopherDoc),
	RUNTIME_CLASS(CGopherWnd),
	RUNTIME_CLASS(CGopherMenu));
    AddDocTemplate(m_pGopherMenu);
    
    m_pGopherDocument = new CMultiDocTemplate(
	IDR_GOPHEROUTPUT,
	RUNTIME_CLASS(COutputDoc),
	RUNTIME_CLASS(CTextWnd),
	RUNTIME_CLASS(COutputView));
    AddDocTemplate(m_pGopherDocument);
    
    m_pGopherCenter = new CMultiDocTemplate(
	IDR_GOPHCENTERTYPE,
	RUNTIME_CLASS(CGophCenterDoc),
	RUNTIME_CLASS(CGophCenterWnd),
	RUNTIME_CLASS(CGopherCenter));
    AddDocTemplate(m_pGopherCenter);
        
    m_pPrefs = new CMultiDocTemplate(
    IDR_PREFERENCES,
    RUNTIME_CLASS(CPrefsDoc),
    RUNTIME_CLASS(CPrefsWnd),
    RUNTIME_CLASS(CPrefsView));
    AddDocTemplate(m_pPrefs);
	
	m_pMailBox = new CMultiDocTemplate(
	IDR_MAILBOXTYPE,
	RUNTIME_CLASS(CMailBoxDoc),
	RUNTIME_CLASS(CMailBoxWnd),
	RUNTIME_CLASS(CMailBoxView));
	AddDocTemplate(m_pMailBox);    

	m_pInbox = new CMultiDocTemplate(
	IDR_NEWMAILTYPE,
	RUNTIME_CLASS(CMailBoxDoc),
	RUNTIME_CLASS(CNewMailWnd),
	RUNTIME_CLASS(CNewMailView));
	AddDocTemplate(m_pInbox);
	
	m_pMailMessage = new CMultiDocTemplate(
	IDR_MESSAGETYPE,
	RUNTIME_CLASS(CMailBoxDoc),
	RUNTIME_CLASS(CMessageWnd),
	RUNTIME_CLASS(CMessageView));
	AddDocTemplate(m_pMailMessage);
	
	m_pMailCompose = new CMultiDocTemplate(
	IDR_MAILCOMPOSETYPE,
	RUNTIME_CLASS(CMailBoxDoc),
	RUNTIME_CLASS(CComposeWnd),
	RUNTIME_CLASS(CComposeView));
	AddDocTemplate(m_pMailCompose);  
	
	m_pInternetNewsgroups = new CMultiDocTemplate(
	IDR_NEWSAREATYPE,
	RUNTIME_CLASS(CNewsDoc),
	RUNTIME_CLASS(CNewsWnd),
	RUNTIME_CLASS(CNewsView));
	AddDocTemplate(m_pInternetNewsgroups);
	
	m_pNewsgroupsManager = new CMultiDocTemplate(
	IDR_GROUPMAN,
	RUNTIME_CLASS(CNewsDoc),
	RUNTIME_CLASS(CNewsgroupsManagerWnd),
	RUNTIME_CLASS(CNewsgroupsManager));
	AddDocTemplate(m_pNewsgroupsManager); 
	
	m_pNewsgroup = new CMultiDocTemplate(
	IDR_ARTSELTYPE,
	RUNTIME_CLASS(CNewsDoc),
	RUNTIME_CLASS(CArticleSelectionWnd),
	RUNTIME_CLASS(CArticleSelectionView));
	AddDocTemplate(m_pNewsgroup); 
	
	m_pNewsArticle = new CMultiDocTemplate(
	IDR_ARTICLETYPE,
	RUNTIME_CLASS(CNewsDoc),
	RUNTIME_CLASS(CArticleWnd),
	RUNTIME_CLASS(CArticleView));
	AddDocTemplate(m_pNewsArticle);  
	
	m_pComposeNewsArticle = new CMultiDocTemplate(
	IDR_NEWSCOMPOSETYPE,
	RUNTIME_CLASS(CNewsDoc),
	RUNTIME_CLASS(CArticleComposeWnd),
	RUNTIME_CLASS(CArticleComposeView));
	AddDocTemplate(m_pComposeNewsArticle); 
	
	m_pAskStaff = new CMultiDocTemplate(
	IDR_ASKSTAFF,
	RUNTIME_CLASS(CAskStaffDoc),
	RUNTIME_CLASS(CAskStaffWnd),
	RUNTIME_CLASS(CAskStaffView));
	AddDocTemplate(m_pAskStaff);
    
	if (!IsWindows95())
	{
		// enable file manager drag/drop and DDE Execute open 
		TRACE0("INF: Enabling DDE Execute Open for File Manager...\r\n");    
		EnableShellOpen();

		TRACE0("INF: Registering file types with Windows Shell...\r\n");
		RegisterShellFileTypes();
        
        TRACE0("INF: Enabling drag-and-drop for files...\r\n");
		m_pMainWnd->DragAcceptFiles();
    }
    
	// simple command line parsing     

	if (m_lpCmdLine[0] == '\0')
	{
		// do nothing -- the Sign On function called above shows the 
		// main menu to the user if the signon was successfull.
		// Otherwise, we can't show any windows or areas yet because
		// the user is not yet online 
		
		// We set m_nCmdShow to SW_SHOWMAXIMIZED here to make sure the
		// main window is always shown maximized on startup 
		m_nCmdShow = SW_SHOWMAXIMIZED;
	}                   
	else if (m_lpCmdLine[0] == '/' && m_lpCmdLine[1] == 'S')
	{
		// first time setup -- guide the user through setting up preferences
		// and the connection
        
        CWnd* pParent = NULL;
        
		// TODO: Add code here to guide the user through setup
		m_nCmdShow = SW_SHOWMINNOACTIVE;                             
		GetMainFrame()->ShowWindow(m_nCmdShow);
		GetMainFrame()->UpdateWindow();

		pParent = CWnd::GetActiveWindow();
			
        CInetnavWelcomeDlg theWelcome(pParent);
        
        if (theWelcome.DoModal() == IDCANCEL)
        	return FALSE;     // exit
        
		CInternetNavApp::Serialize(FALSE);		
		CUserInformation userInfo(pParent);
		
		userInfo.DoModal();

		// Now ask the user to set Application Preferences
		CAppPrefsSheet appSheet(pParent);
		appSheet.Serialize(FALSE);
		
		if (appSheet.DoModal() == IDOK)
			appSheet.Serialize(TRUE);

		// Now ask the user to choose Internet Preferences
		CInetPrefsSheet inetSheet(pParent); 
		inetSheet.Serialize(FALSE);
		
		if (inetSheet.DoModal() == IDOK)
			inetSheet.Serialize(TRUE);
        
        CFtpViruses theDialog(pParent);
		theDialog.DoModal();
		
		CWaitCursor wait;

		// Set the user up with a new mailbox             
		// using CInternetNavApp::AddNewMailbox()
		AddNewMailbox();
		
		// Have the user set up the Internet Newsgroups area
		// using CNewsDoc::OnNewDocument()'s setup procedure
		m_pMainFrame->ShowWindow(SW_SHOWMINNOACTIVE);
		m_pMainFrame->UpdateWindow();
		m_pInternetNewsgroups->OpenDocumentFile(NULL);
		m_pMainFrame->ShowWindow(SW_SHOWMINNOACTIVE);
		m_pMainFrame->UpdateWindow();
		m_pInternetNewsgroups->CloseAllDocuments(TRUE);
		m_pMainFrame->ShowWindow(SW_SHOWMINNOACTIVE);
		m_pMainFrame->UpdateWindow();
		
		CInternetNavApp::Serialize(TRUE);
		
		// Save the mailbox the user created
		SaveMailboxes();
		
		return FALSE;
	} 
	else if (m_lpCmdLine[0] == '/' && m_lpCmdLine[1] == 'M' &&
		m_lpCmdLine[2] == 'A' && m_lpCmdLine[3] == 'I')
	{ 
		// Create a new Mailbox
		
		AddNewMailbox();

		return FALSE;
	}
	else if (m_lpCmdLine[0] == '/' && m_lpCmdLine[1] == 'N' &&
		m_lpCmdLine[2] == 'W' && m_lpCmdLine[3] == 'S')
	{
		// Create a new Newsgroups Settings file
		
		// Have the user set up the Internet Newsgroups area
		// using CNewsDoc::OnNewDocument()'s setup procedure
		m_pMainFrame->ShowWindow(SW_SHOWMINNOACTIVE);
		m_pMainFrame->UpdateWindow();
		m_pInternetNewsgroups->OpenDocumentFile(NULL);
		m_pMainFrame->ShowWindow(SW_SHOWMINNOACTIVE);
		m_pMainFrame->UpdateWindow();
		m_pInternetNewsgroups->CloseAllDocuments(TRUE);
		m_pMainFrame->ShowWindow(SW_SHOWMINNOACTIVE);
		m_pMainFrame->UpdateWindow();
		
		return FALSE;
	}
	else
	{
		// open an existing document
		Serialize(TRUE);     

		CString strLine = m_lpCmdLine;
				
		if (!IsOnline() && strLine.Right(3) == "FNG" ||
			strLine.Right(3) == "FTP" || strLine.Right(3) == "WHO" ||
				strLine.Right(3) == "IRC")
		{
			if (!SignOn())    // connect to Internet
				return FALSE;
		}

		if (strLine.Right(4) == ".MAI")
		{
			OnMailboxCompose();
		}
		else if (strLine.Right(4) == ".NWS")
		{
			OnArticleAdd();
		}
		else
		{
			OpenDocumentFile(strLine);
		}

		m_nCmdShow = SW_SHOWMAXIMIZED;
	}
    
    if (m_lpCmdLine[0] != '/' && m_lpCmdLine[1] != 'S')
	{    
		// The main window has been initialized, so show and update it.
		m_pMainFrame->ShowWindow(m_nCmdShow);
		m_pMainFrame->UpdateWindow();
	}
                 
	BOOL bDontRemoveAll = FALSE;
	
	TRY
	{
		m_pIRCChanList = new CMapStringToOb(10);
	}
	CATCH(CMemoryException, e)
	{
		m_pIRCChanList = NULL;
		bDontRemoveAll = TRUE;
		
		SetIRCConnected(FALSE);
		
		m_pIRCSocket = NULL;
		return TRUE;
	}
	END_CATCH
	
	if (!bDontRemoveAll) m_pIRCChanList->RemoveAll();
	
	SetIRCConnected(FALSE);
	
	m_pIRCSocket = NULL;
	
	m_bSaveUserData = TRUE; 

	LoadMailboxes();
	
	return TRUE;
}

BOOL CInternetNavApp::OnIdle(LONG lCount)
{    
	return CWinApp::OnIdle(lCount);
}

BOOL CInternetNavApp::PreTranslateMessage(MSG* pMsg)
{
	BOOL bResult = CWinApp::PreTranslateMessage(pMsg);
	
	if (m_wndSplash.m_hWnd != NULL &&
		(pMsg->message == WM_KEYDOWN ||
		 pMsg->message == WM_SYSKEYDOWN ||
		 pMsg->message == WM_LBUTTONDOWN ||
		 pMsg->message == WM_RBUTTONDOWN ||
		 pMsg->message == WM_MBUTTONDOWN ||
		 pMsg->message == WM_NCLBUTTONDOWN ||
		 pMsg->message == WM_NCRBUTTONDOWN ||
		 pMsg->message == WM_NCMBUTTONDOWN))
	{  
		if (m_lpCmdLine[0] == '/'&& m_lpCmdLine[1] == 'S')
			return bResult;
			 
		m_wndSplash.ShowWindow(SW_HIDE);
		m_wndSplash.UpdateWindow();
		m_wndSplash.DestroyWindow();
		m_pMainFrame->UpdateWindow();	  
		
		if (m_lpCmdLine[0] != '\0')
			return bResult;			
		
		// Show the user the Tip Of The Day, if they said they want to see it
		// in Preferences
		if (IsShowTOD() && GetShowTips() == 0)  
		{
			m_bPseudoShowTOD = TRUE;
			OnTipOfDay();           
		}
		else
			m_bPseudoShowTOD = FALSE;      		

		if (StartOnline() && !IsOnline())
		{
			m_bPseudoOnline = TRUE;
			OnPlacesSignOn();      
		}
		
		return bResult;
	}                                                  

	return bResult;  
}     

int CInternetNavApp::ExitInstance()
{      
	// Cleanup application and system resources
	
	CWaitCursor wait; 
	
	// Save mailboxes
	SaveMailboxes();
	
	if (IsUseCTL3D())
		Ctl3dUnregister(AfxGetInstanceHandle());
	
	CInternetNavApp::Serialize(TRUE);         
	
	if (m_pIRCSocket)
		delete m_pIRCSocket;
	m_pIRCSocket = NULL;
	
	m_pStatusBar = NULL;
	
	if (m_pIRCChanList)
	{
		m_pIRCChanList->RemoveAll();
		delete m_pIRCChanList;
	}
	m_pIRCChanList = NULL;
	
	m_userData.Detach();
	
	return CWinApp::ExitInstance();
}  

// AddToRecentFileList
// Our implementation of this function first calls the base class version
// of this function and then calls 
// CInternetNavApp::Serialize(IsStoring=TRUE) to save the MRU file list to
// the INI file.
void CInternetNavApp::AddToRecentFileList(const char* pszPathName)
{
	CWaitCursor wait;
	// Check pszPathName.  Only proceed if there isn't a '.MAI' or a '.NWS'
	// on the end of the filename.  Otherwise, prevent the filename
	// from being added to the MRU list
	
	// So we can deal with pszPathName better, save it in a CString
	CString strPathName = pszPathName;
	int nEndPosition = strPathName.GetLength() - 1;
	
	if (strPathName.Right(4) == ".MAI" ||
			strPathName.Right(4) == ".NWS")
	{
		return;
	} 
	else if (strPathName[nEndPosition - 3] != '.')
	{
		// If the path name has no file extension, which would mean
		// we weren't even passed a valid filename path name in the first
		// place, just ignore it and return
		return;
	}
	
	// Call the base class version of this function first
	CWinApp::AddToRecentFileList(pszPathName);
	
	// Now save the most-recently-used-files list to the INI file and
	// return
	CInternetNavApp::Serialize(TRUE);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp INI file information helper functions

void CInternetNavApp::Serialize(BOOL IsStoring /*=TRUE*/)
{       
	// Call CInternetNavApp::SaveStdProfileSettings() to save settings
	// or CInternetNavApp::LoadStdProfileSettings() to load settings
	// depending on whether or not bSave is TRUE or FALSE.
	
	// Call Serialize functions of data structures so they too can
	// read/write their settings to/from the INI file.
	GetUserData()->Serialize(IsStoring);
	GetPrefs()->Serialize(IsStoring);
		
	if (IsStoring) 
	{
		CInternetNavApp::SaveStdProfileSettings();
	}
	else 
	{                                             
		CInternetNavApp::LoadStdProfileSettings();   
	}

	return;
}

void CInternetNavApp::LoadStdProfileSettings()
{ 
	CWaitCursor wait;
	CWinApp::LoadStdProfileSettings();

    CString strGeneralSec = "Global Settings";
    m_bWindows95 = GetProfileInt(strGeneralSec, "IsWindows95", TRUE);

    // TODO: Add calls here to GetProfileString() and GetProfileInt() to load
    // additional global data and preferences from the INI file
    
	return;
}

void CInternetNavApp::SaveStdProfileSettings()
{
	CWaitCursor wait;
	CWinApp::SaveStdProfileSettings();
    
    // save global settings to INI
    CString strGeneralSec = "Global Settings";
    WriteProfileInt(strGeneralSec, "IsWindows95", IsWindows95());

	// TODO: Add calls to WriteProfileString() and WriteProfileInt() to save
	// additional data and preferences to the INI file
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp sign on/sign off functions

BOOL  CInternetNavApp::SignOn()
{
	CWaitCursor wait;
	if (!m_bOnline)
		OnPlacesSignOn();
		
	wait.Restore();  
	return IsOnline();
} 

BOOL CInternetNavApp::SignOff()
{
	CWaitCursor wait;
	if (m_bOnline)
		OnPlacesSignOff();

	wait.Restore();
	return !IsOnline();
}

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp FlashConnect helper functions

BOOL CInternetNavApp::SetFlashConnect(BOOL bFlashConnect /*=TRUE*/)
{           
	CWaitCursor wait;
	if (bFlashConnect) // begin FlashConnect
	{     
		CWaitCursor wait;
		// Show the 'Begin FlashConnect' message -- if the user presses
		// cancel then cancel the FlashConnect  
		
		CFlashConnectBegin theMessage(GetMainFrame());
		
		if (theMessage.DoModal() == IDCANCEL) return FALSE;
		
		m_bFlashConnect = TRUE;
		
		// Attempt to connect to the user's Internet provider and begin
		// the FlashConnect
		if (!SignOn())
			return FALSE;
		return TRUE;
	}	
	else // end FlashConnect
	{    
		CWaitCursor wait;
		// The SetFlashConnect function must always be successful here -- 
		// OnPlacesSignOff() forces the user to disconnect, so SignOff() will
		// always return TRUE.
		
		// Show the user the 'End FlashConnect' message
		if (!m_bFlashConnectError)
		{
			CFlashConnectEnd theMessage(GetMainFrame());
			theMessage.DoModal();
		}              
		else
		{
			MessageBeep(-1);
			CFlashConnectAbort theMessage(GetMainFrame());
			theMessage.DoModal();
			
			m_bFlashConnectError = FALSE;
		}
		
		m_pStatusBar->SetText("Ending FlashConnect...");
		
		// Disconnect from the Internet
		SignOff();
		
		// set the m_bFlashConnect variable to FALSE
		m_bFlashConnect = FALSE;
		return TRUE;	
	}		
}

BOOL CInternetNavApp::AbortFlashConnect()
{
	// A failed FlashConnect must be aborted -- a fatal repception,
	// transmission, or other communication error has occurred
	m_bFlashConnectError = TRUE;              
	SetFlashConnect(FALSE);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp Help & Information helper functions

void CInternetNavApp::EnterHelpArea()
{
	CWaitCursor wait;
	OnPlacesHelpAndInfo();	
}

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp socket helper functions

QSocket* CInternetNavApp::GetNewConnection(BOOL bCreateSocket /*= TRUE*/)
{   
	CWaitCursor wait;
	
	QSocket* pResult = NULL;
	
	// Try to allocate memory for a new socket
	TRY
	{
		pResult = new QSocket(bCreateSocket);
	}
	CATCH(CMemoryException, pMemoryException) // a memory allocation error
	{
		// We were unable to allocate memory for a new connection --
		// tell the user that
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_MEMORY_SOCKET_ALLOC, MB_ICONSTOP);
		return NULL;
	}
	END_CATCH
	
	if (pResult)
		return pResult;
	return NULL;		
}

/////////////////////////////////////////////////////////////////////////////
// Directory Chooser common dialog helper

CString CInternetNavApp::DoDirectoryDialog(CWnd* pParent,
	CString& strInitialDir)
{
	CString strReturn = ""; // return string is chosen directory name
	strReturn.Empty();
	
	if (strInitialDir.IsEmpty()) 
	{
		strInitialDir = "C:\\";
	}
		
	_chdir(strInitialDir);
	_chdir(strInitialDir);
	_chdir(strInitialDir);
	                
   	CDirectoryDialog  cfdlg(FALSE, NULL, "Junk", OFN_SHOWHELP | 
   		OFN_HIDEREADONLY | OFN_ENABLETEMPLATE, 
   			NULL, pParent);  
	
	cfdlg.m_ofn.hwndOwner = pParent->GetSafeHwnd();
	
    cfdlg.m_ofn.hInstance = AfxGetInstanceHandle();
    cfdlg.m_ofn.lpTemplateName = MAKEINTRESOURCE(FILEOPENORD);
	
	cfdlg.m_ofn.lpstrTitle = "Select Directory";
	cfdlg.m_ofn.lpstrInitialDir = strInitialDir; 
	
    if (cfdlg.DoModal() == IDOK)
    {
		strReturn = cfdlg.GetPathName();
		
		// Find the 'Junk' and chop it off
		int nJunk = strReturn.ReverseFind('J');
		strReturn = strReturn.Left(nJunk); // since nJunk is 0-based, we
											// just chop off the Junk!
											
		// now return the directory name to the caller!
		return strReturn;
    }  
    else
    {
    	strReturn = "";
    }
    
    return strReturn;
}
	
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/) 
	: CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetNavApp commands

// App command to run the About dialog box
void CInternetNavApp::OnAppAbout()
{
	CWaitCursor wait;
	CAboutDlg aboutDlg(m_pMainFrame);
	aboutDlg.DoModal();
	return;
}

void CInternetNavApp::DisplayMainMenu()
{
	CWaitCursor wait;
	// Show the Main Menu to the user.  Since the Main Menu window
	// is a form view, we use a document template.  Displaying the
	// window is like opening a new copy of the document in the
	// template, so we call CDocTemplate::OpenDocumentFile with
	// no file name parameter (we don't want to open a file; we
	// want a new one 
	if (IsOnline()) 
	{
		if (m_pWndMainMenu != NULL &&
			::IsWindow(m_pWndMainMenu->GetSafeHwnd()))
		{
			m_pWndMainMenu->ActivateFrame(SW_RESTORE);    
			m_pWndMainMenu->BringWindowToTop();
			return;
		}
		m_pMainMenu->OpenDocumentFile(NULL);
		return;
	}
	else        
	{
		UINT nType = MB_OK|MB_ICONSTOP;
		MessageBeep(-1);
		AfxMessageBox(IDP_INETNAV_SIGN_ON, nType);
		return;
	}
		
	return; 
}

void CInternetNavApp::OnNewTextFile()
{  
	CWaitCursor wait;
	// Make a new document of the Text File template with OpenDocumentFile
	// passed a NULL parameter    
	// Soon, we will ask the user whether or not they want to open
	// a TXT file or a HTM file with a dialog box.
	// The same template but different file extension will be used for HTM
	// files    
	m_pTextTXT->OpenDocumentFile(NULL);
}

void CInternetNavApp::OnPlacesFinger()
{                                           
	CWaitCursor wait;
	// Open the Internet Users Information Browser      
	if (IsOnline())
		m_pFinger->OpenDocumentFile(NULL);              
	else        
	{
		UINT nType = MB_OK|MB_ICONSTOP;
		MessageBeep(-1);
		AfxMessageBox(IDP_INETNAV_SIGN_ON, nType);
		return;
	}
		
	return; 
	
	// TODO: Add a preferences option here to let the user decide on whether
	// or not to have the File Open dialog box be displayed here instead of
	// a blank file; this way the user can open a file they've already
	// made
}

void CInternetNavApp::OnPlacesWhois()
{
	CWaitCursor wait;
	// Open the InterNIC Site Information Browser 
	if (IsOnline())
		m_pWhois->OpenDocumentFile(NULL);       
	else        
	{
		UINT nType = MB_OK|MB_ICONSTOP;
		MessageBeep(-1);
		AfxMessageBox(IDP_INETNAV_SIGN_ON, nType);
		return;
	}
		
	return; 
	
	// TODO: Add a preferences option here to let the user decide on whether
	// or not to have the File Open dialog box be displayed here instead of
	// a blank file; this way the user can open a file they've already
	// made
}

void CInternetNavApp::OnPlacesFTPClient()
{
	CWaitCursor wait;
	// Open the FTP Client
	if (IsOnline())
		m_pFTP->OpenDocumentFile(NULL); 
	else        
	{
		UINT nType = MB_OK|MB_ICONSTOP;
		MessageBeep(-1);
		AfxMessageBox(IDP_INETNAV_SIGN_ON, nType);
		return;
	}
		
	return; 
	// TODO: Add a preferences option here to let the user decide on whether
	// or not to have the File Open dialog box be displayed here instead of
	// a blank file; this way the user can open a file they've already
	// made
}

void CInternetNavApp::OnPlacesPostOffice()
{
	CWaitCursor wait; 
	
	// We allow the user to access electronic mail functions while offline.
    // as well as offline
    
	CString strPath = GetProfileString("Default Mailbox", "MailboxPath");
	
	if (!strPath.IsEmpty())
	{
		// open the mail box
		GetMailBoxType()->OpenDocumentFile(strPath);
		return;
	}
	else
		GetMailBoxType()->OpenDocumentFile(NULL);
	
	return;
}

void CInternetNavApp::OnPlacesPreferences()
{
	CWaitCursor wait; 

	if (m_pWndPrefs != NULL &&
		::IsWindow(m_pWndPrefs->GetSafeHwnd()))
	{
		wait.Restore();
		m_pWndPrefs->ActivateFrame(SW_RESTORE);
		m_pWndPrefs->BringWindowToTop();
		return;
	}          
	m_pPrefs->OpenDocumentFile(NULL);
	return;
}

void CInternetNavApp::OnPlacesHelpAndInfo()
{
	CWaitCursor wait;    
	
	wait.Restore();    
	if (m_pWndHelpInfo != NULL &&
		::IsWindow(m_pWndHelpInfo->GetSafeHwnd()))
	{  
		wait.Restore();
		m_pWndHelpInfo->ActivateFrame(SW_RESTORE);
		m_pWndHelpInfo->BringWindowToTop();
		return;
	}
	m_pHelpInfoArea->OpenDocumentFile(NULL); 
	return;
}

void CInternetNavApp::OnPlacesChatCenter()
{
	CWaitCursor wait; 
	if (IsOnline())
	{
		m_pStatusBar->SetText("Welcome to the Chat Center!");
		
		if (!IRCIsConnected())
		{
			CIRConnectDlg theDialog(GetMainFrame()); 
			
			// fill default data values  
			wait.Restore();
			theDialog.m_nPortNum = GetIRCPort();
			theDialog.m_strServer = GetIRCServerName();
			theDialog.m_strNickname = GetNickname();
			theDialog.m_strInitialChannel = GetInitialChannel();
			
			if (theDialog.DoModal() == IDOK)
			{ 
				CString strText = "Connecting to " + theDialog.m_strServer;
				strText += "...  Please wait.";  
				m_pStatusBar->SetText(strText);
				
				wait.Restore();
				// get information from dialog box
				SetIRCPort(theDialog.m_nPortNum);
				SetIRCServerName(theDialog.m_strServer);
				SetNickname(theDialog.m_strNickname);  
				
				if (!theDialog.m_strInitialChannel.IsEmpty())
				{
					SetInitialChannel(theDialog.m_strInitialChannel);
					SetInitialChannelPref(GetInitialChannel());
					CInternetNavApp::Serialize(TRUE);
				}
								
				QSocket* pIRCSocket = GetNewConnection();
				
				if (pIRCSocket == NULL)
					return;
				
				SetIRCSocket(pIRCSocket);
				m_pIRCSocket->SetReceiveTarget(GetMainFrame(), 
										WM_IRC_SOCKET_RESPONSE);
				
				wait.Restore();  
				
				m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
				if (m_pIRCSocket->Connect(GetIRCServerName(), GetIRCPort()))
				{   
					wait.Restore();
					strText = "Connected to " + GetIRCServerName() + "."; 
					m_pStatusBar = GetMainFrame()->GetStatusBar();
					m_pStatusBar->SetText(strText);
					
					SetIRCConnected(TRUE); 
					
					wait.Restore();	
					// save IRC settings to INI file with Serialize()
					CInternetNavApp::Serialize(TRUE);
					
					// Open the IRC template on a new document
					GetIRCConversation()->OpenDocumentFile(NULL);	
				}
				else	// connect failed
				{ 
					SetIRCConnected(FALSE);  
					strText = "Attempt to connect to " + GetIRCServerName();
					strText += " has failed."; 
					m_pStatusBar->SetText(strText);
					
					wait.Restore();
    				// Tell the user we were'nt able to connect
    				MessageBeep(-1);
    				AfxMessageBox(IDP_NOCONNECT_HOST);
    				delete m_pIRCSocket;
    				m_pIRCSocket = NULL;      
    				m_pStatusBar->ShowIdleMessage();
				} // if (m_pIRCSocket->Connect(m_strIRCServerName, m_nPort))						
			} // if (theDialog.DoModal() == IDOK)
			else
				return;
		} // if (!IRCIsConnected())
	}
	else
	{
		UINT nType = MB_OK|MB_ICONSTOP;
		MessageBeep(-1);
		AfxMessageBox(IDP_INETNAV_SIGN_ON, nType);
		return;
	} // if (IsOnline())
}

void CInternetNavApp::OnPlacesNewsgroups()
{
	CWaitCursor wait; 
	if (IsOnline())
	{ 
		wait.Restore(); 
		CString strPath = CWinApp::GetProfileString("Internet Newsgroups", "AreaFile");
		
		if (!strPath.IsEmpty())
		{ 
			wait.Restore();
			GetInternetNews()->OpenDocumentFile(strPath);
			return;
		}
		else 
		{
			wait.Restore();
			GetInternetNews()->OpenDocumentFile(NULL);
		}
		
		wait.Restore();	
		return;		
	}
	else
	{
		UINT nType = MB_OK|MB_ICONSTOP;
		MessageBeep(-1);
		AfxMessageBox(IDP_INETNAV_SIGN_ON, nType);
		return;
	}
}

void CInternetNavApp::OnPlacesGopherCenter()
{
	CWaitCursor wait;
	
	wait.Restore();    
	if (m_pWndGophCenter != NULL &&
		::IsWindow(m_pWndGophCenter->GetSafeHwnd()))
	{  
		wait.Restore();
		m_pWndGophCenter->ActivateFrame(SW_RESTORE);
		m_pWndGophCenter->BringWindowToTop();
		return;
	}   
	
	// Otherwise open the Gopher Center window from scratch
	GetGopherCenter()->OpenDocumentFile(NULL); 

	return;
}

void CInternetNavApp::OpenGopherMenu(const char* pszSelector)
{
	m_pGopherMenu->OpenDocumentFile(pszSelector);
}

void CInternetNavApp::OpenGopherDocument(const char* pszSelector)
{
	m_pGopherDocument->OpenDocumentFile(pszSelector);
}

void CInternetNavApp::OpenGopherSearch(const char* pszSelector)
{
	m_pStatusBar->SetText("Resolving search information...  Please wait.");
	
	#ifdef _DEBUG
		afxDump << pszSelector;
	#endif
	
	if (pszSelector == NULL)
	{
		AfxMessageBox(IDP_GOPHER_NOSEARCH, MB_ICONSTOP|MB_OK);
		return;                                               
	}
	
	TRACE0("INF: Now duplicating selector string...\r\n");
		  
	char* s = (char*)pszSelector; 
	
	// Duplicate the passed selector string because it is changed when it is
	// parsed and we need to pass an unchanged version to OpenGopherMenu()
	// below.   
    
    if (s == NULL)
    {
    	AfxMessageBox(IDP_GOPHER_NOSEARCH, MB_ICONSTOP|MB_OK);
    	return;
    }

	m_strSearchTitle = strtok(&s[1], "\t");
	
	if (m_strSearchTitle.IsEmpty())
	{
	   	TRACE0("ERR: No search service title provided.\r\n");
    	AfxMessageBox(IDP_GOPHER_NOSEARCH, MB_ICONSTOP|MB_OK);
		return;
	}
			
	char* pszSearchSel = strtok(NULL, "\t"); 

	if (pszSearchSel == NULL)
	{
	   	TRACE0("ERR: No search service selector provided.\r\n"); 
	   	AfxMessageBox(IDP_GOPHER_NOSEARCH, MB_ICONSTOP|MB_OK);
	   	return;
	}
	
	m_strSearchSelector = pszSearchSel;
			
	if (m_strSearchSelector.IsEmpty())
	{
	   	TRACE0("ERR: No search service selector provided.\r\n");
    	AfxMessageBox(IDP_GOPHER_NOSEARCH, MB_ICONSTOP|MB_OK);
    	return;
	}
	
	m_strSearchServer = strtok(NULL, "\t");
		
	if (m_strSearchServer.IsEmpty())
	{
	   	TRACE0("ERR: No search service server address provided.\r\n");
    	AfxMessageBox(IDP_GOPHER_NOSEARCH, MB_ICONSTOP|MB_OK);
		return;
	}

	char* pszSearchPort = strtok(NULL, "\t");
	
	if (pszSearchPort == NULL) // beyond end of selector string
	{ 
		// Port number is actually m_strSearchServer,
		// Server Address is actually m_strSearchSelector,
		// and the selector itself is empty.
		// Voila! A Veronica search!
		m_nSearchPort = (UINT)atoi(m_strSearchServer);
		m_strSearchServer = m_strSearchSelector;
		m_strSearchSelector = "";   
		m_pStatusBar->SetText("Searching using the Veronica search service.");
	}
	else if (pszSearchPort != NULL)
	{
		m_nSearchPort = (UINT)atoi(pszSearchPort); 
	}

	// now show the search dialog box to get the query string to use from
	// the user

	CVeronicaSearchDlg theSearch(GetMainFrame());
	if (theSearch.DoModal() == IDOK)
	{
    	if (theSearch.m_strQuery.IsEmpty())
		{
			AfxMessageBox(IDP_SEARCH_INFO_NEEDED);
			return;                               
		}
		else
			m_strSearchQuery = theSearch.m_strQuery;
		
		SetGopherSearch(TRUE);
		m_pStatusBar->SetText("Searching...  Please wait.");
		OpenGopherMenu(s);
		return;							
	}	
	else // user pressed cancel
	{       
		// make sure future Gopher menus won't think we're searching
		// call SetGopherSearch() with bSearch set to FALSE.
		SetGopherSearch(FALSE); 
		m_pStatusBar->SetText("Search aborted.");
		AfxMessageBox("Search aborted by user.", MB_ICONASTERISK|MB_OK);
		m_pStatusBar->ShowIdleMessage();
		return;
	}
}

void CInternetNavApp::OnPlacesSignOn()
{
    CWaitCursor wait;
    
    wait.Restore(); 
    CString strText = "";
    
    if (!IsFlashConnect())
    {
    	strText = "Connecting to the Internet...";
    	strText += "  Please wait.";
    }
    else
    {
     	strText = "Starting FlashConnect...  Please wait.";
    }	
    
    if ((m_pStatusBar != NULL) && (::IsWindow(m_pStatusBar->GetSafeHwnd())))
    	m_pStatusBar->SetText(strText);  
    
    if (!IsFlashConnect() && IsWindows95())
    {
    	// Show a reminder to the user about connecting their computer
    	// to their Internet Provider
    	CConnectMsg theMsg(GetMainFrame());
    	
    	MessageBeep(-1);
    	if (theMsg.DoModal() == IDCANCEL)
    		return;
    }
    
	if (!AfxSocketInit(GetWSADATA()))
	{
		// Socket init failed
		MessageBeep(-1);
		AfxMessageBox(IDP_SIGNON_FAILED, MB_ICONSTOP|MB_OK);
		TRACE0("ERR: Failed to connect to the Internet.");
		if (IsFlashConnect())
		{
			AbortFlashConnect(); 
			return;
		}
		if (m_pStatusBar != NULL && ::IsWindow(m_pStatusBar->m_hWnd))
			m_pStatusBar->ShowIdleMessage();
		m_bOnline = FALSE;
		return;
	}

	wait.Restore();
	wait.Restore();
    
    m_bOnline = TRUE; 
    OnIdle(0);
    
	// Show the user the Tip Of The Day, if they said they want to see it
	// in Preferences
	if (IsShowTOD() && GetShowTips() == 1 && !IsFlashConnect())
		OnTipOfDay();

	if (IsFlashConnect())
	{
		#ifdef _DEBUG
			afxDump << "INF: FlashConnect in progress!\r\n\r\n";
		#endif
		m_pStatusBar->SetText("FlashConnect in progress.");   
		OnIdle(0); // update user interface
	}                                       
	
	m_pStatusBar->ShowIdleMessage();
	
	wait.Restore(); 
	  
	if (IsShowMainMenu() && IsOnline() && !IsFlashConnect())
		m_pMainMenu->OpenDocumentFile(NULL);     
	
	if (IsCheckMailStartup() && IsOnline() && !IsFlashConnect())
		OnMailboxCheck();
			
	wait.Restore();             
	
	m_bPseudoOnline = FALSE;
	return;
}

void CInternetNavApp::OnUpdatePlacesSignOn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!IsOnline());
}

void CInternetNavApp::OnPlacesSignOff()
{ 
	CWaitCursor wait;
	
	Serialize(FALSE);
	Serialize(TRUE);
	    
    if (IsConfirmSignOff() && !IsFlashConnect()) 
    	MessageBeep(-1);
	
    CDisconnectDialog theDialog(GetMainFrame());
    theDialog.m_bConfirmSignOff = !IsConfirmSignOff();
    
    if (IsConfirmSignOff() && !IsFlashConnect())
    {
    	if (theDialog.DoModal() == IDCANCEL)
    	{
    		return;		
    	}
    	else
    	{
    		SetConfirmSignOff(!theDialog.m_bConfirmSignOff);
    		Serialize(TRUE);
    		Serialize(FALSE);
    	}
    }
	
	wait.Restore();
	m_pStatusBar->SetText("Disconnecting from the Internet...");

	// Close the Main Menu 
	if (!IsFlashConnect())   // we're not in FlashConnect
	{
		GetMainMenu()->CloseAllDocuments(FALSE);
        
    	wait.Restore();
		// Close all visible windows and documents of each of the Internet
		// document types
		if (!SaveAllModified())
			return;
		
		GetFingerType()->CloseAllDocuments(FALSE);
		GetFtpType()->CloseAllDocuments(FALSE);                 
		GetIRCConversation()->CloseAllDocuments(FALSE); 
		GetGopherCenter()->CloseAllDocuments(FALSE);
		GetGopherMenu()->CloseAllDocuments(FALSE);
		GetGopherDocument()->CloseAllDocuments(FALSE);
		GetWhoisType()->CloseAllDocuments(FALSE);  
		GetInboxType()->CloseAllDocuments(FALSE);
		GetMailBoxType()->CloseAllDocuments(FALSE);
		GetMessageType()->CloseAllDocuments(FALSE);
		GetInternetNews()->CloseAllDocuments(FALSE);
		GetNewsgroupsManager()->CloseAllDocuments(FALSE);
		GetNewsgroup()->CloseAllDocuments(FALSE);
		GetNewsArticle()->CloseAllDocuments(FALSE);
		GetCNA()->CloseAllDocuments(FALSE);
	}

	// All offline document types - text, graphics, ..., any kind of
	// data files - will remain open, because only the document templates
	// which manage the Internet browsers and information viewers will
	// be closed	
		
	wait.Restore();		
	m_bOnline = m_bPseudoOnline = FALSE;
	SetIRCConnected(FALSE);
    
    // update UI -- now that we're offline, make sure commands for
    // online Places are disabled
    OnIdle(0);
    OnIdle(1);
    
	m_pStatusBar->ShowIdleMessage();
	
	// Actually disconnect ousrselves from the Internet and clean up
	// resources needed by the system as specified in the Windows Sockets
	// version 1.1 specifications
	AfxSocketTerm(); 
				
	return;   
}

void CInternetNavApp::OnUpdatePlacesSignOff(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline());
}

void CInternetNavApp::OnUpdateDisplayMainMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline());
}

void CInternetNavApp::OnUpdatePlacesChatCenter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((IsOnline() && !IRCIsConnected()));
}

void CInternetNavApp::OnUpdatePlacesFinger(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline());
}

void CInternetNavApp::OnUpdatePlacesFTPClient(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline());
}

void CInternetNavApp::OnUpdatePlacesGopherCenter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline());
}

void CInternetNavApp::OnUpdatePlacesNewsgroups(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline());
}

void CInternetNavApp::OnUpdatePlacesWhois(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline());
}

void CInternetNavApp::OnAppExit()
{  
	// Thank the user for using Internet Navigator and make sure they
	// really want to exit
	CWaitCursor wait;
    
    Serialize(FALSE);
    
	if (IsOnline())                                  
	{
		m_pStatusBar->SetText("Disconnecting from Internet...  Please wait.");
		
		BOOL bConfirm = IsConfirmSignOff();
		
		if (bConfirm)
		{
			SetConfirmSignOff(FALSE);
		}
		
		SignOff(); // sign off before exiting
		
		if (bConfirm)
		{
			SetConfirmSignOff(bConfirm);
		}
    	
    	Serialize(TRUE);
    	Serialize(FALSE);
    	
		m_pStatusBar->ShowIdleMessage();
	}                                                 

    if (!IsOnline() && IsConfirmExit())
    {
		CExitDlg dlg;
		Serialize(FALSE);    
		dlg.m_bConfirmExit = !IsConfirmExit();
		
		if (dlg.DoModal() == IDOK)
		{
			wait.Restore();
			SetConfirmExit(!dlg.m_bConfirmExit);
			Serialize(TRUE); // save data one last time before exiting
			
			CWinApp::OnAppExit();
			return; 
		} 
		else // user pressed the No button (cancel)
		{
			m_pStatusBar->ShowIdleMessage();
			return;
		}
	}                      
	else if (!IsOnline() && !IsConfirmExit())
	{
		CWinApp::OnAppExit();
	}
	return;
}

void CInternetNavApp::AddChanWindow(const CView* pNewView, 
						const CString& strChannel)
{
	if (m_pIRCChanList == NULL)
		return;
	               
	CString strText = "Adding window for " + strChannel + "to list of ";
	strText += "channel windows...  Please wait.";     
	m_pStatusBar->SetText(strText);
	
	TRY
	{
		m_pIRCChanList->SetAt((const char*)strChannel, (CObject*&)pNewView);
	}
	CATCH(CMemoryException, pME)
	{   
		m_pStatusBar->ShowIdleMessage();
		strText = "Not enough memory to display window for channel ";
		strText += strChannel + ".  Please close other programs which are ";
		strText += "running and try again.";
		MessageBeep(-1); 
		AfxMessageBox(strText, MB_ICONASTERISK|MB_OK);
		return;
	}
	END_CATCH
	
	m_pStatusBar->ShowIdleMessage();	
	return;
}

BOOL CInternetNavApp::RemoveChanWindow(const CString& strChannel)
{
	if (m_pIRCChanList == NULL)
		return FALSE;
	
	BOOL bResult = FALSE;
	bResult = m_pIRCChanList->RemoveKey((const char*)strChannel);
	
	if (bResult && m_pIRCChanList->IsEmpty())
		SetIRCConnected(FALSE);

	m_pStatusBar->ShowIdleMessage();
	return bResult;
}

BOOL CInternetNavApp::ChangeChanWindow(const CString& strOldChannel,
						const CString& strNewChannel)
{   
	if (m_pIRCChanList == NULL)
		return FALSE;
	
	CIRClientView* pChild = NULL;      
	BOOL bResult = TRUE;
	
	if (m_pIRCChanList->Lookup(strOldChannel, (CObject*&)pChild))
	{
		bResult = m_pIRCChanList->RemoveKey((const char*)strOldChannel);
		AddChanWindow(pChild, strNewChannel);
		return bResult;
	}  
	else
	{
		bResult = FALSE;                                        
	}
	return bResult;
}

void CInternetNavApp::OnChatJoinGroup()
{
	CWaitCursor wait; // show a hourglass cursor

	// To join a chat group, we just create a new document from the
	// m_pIRCConverse template.  The CIRClientDoc class handles asking the
	// user for the name of the chat group or channel they want to join.
	GetIRCConversation()->OpenDocumentFile(NULL);
	return;
}

void CInternetNavApp::OnUpdateChatJoinGroup(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline() && IRCIsConnected());
}

/////////////////////////////////////////////////////////////////////////////
// CUserData data structure class -- use with CUserInformation

CUserData::CUserData()
{
	m_pApp = NULL;
	DeleteContents();
}

CUserData::~CUserData()
{
	DeleteContents();
}

void CUserData::OnWizardInit()
{
	// prep the Online Registration/Tech Support Wizard
	Serialize(FALSE);
	return;
}

void CUserData::OnWizardFinish()
{
	m_pApp->m_bSaveUserData = TRUE; 
	Serialize(TRUE);

	return;
}

void CUserData::OnWizardCancel()
{
	DeleteContents();
	m_pApp->m_bSaveUserData = FALSE;
	return;
}

void CUserData::Serialize(BOOL IsStoring)
{    
	CWaitCursor wait;
	if (IsStoring)
	{  
		if (m_pApp->m_bSaveUserData == TRUE)
		{
			m_pApp->WriteProfileString("User Information", "UserName", m_strUserName);
			m_pApp->WriteProfileString("User Information", "UserCompany", m_strUserCompany);
		 	return;
		}          
		else
			return;
		return;
	} 
	else
	{
		m_strUserName = m_pApp->GetProfileString("User Information", "UserName");
		m_strUserCompany = m_pApp->GetProfileString("User Information", "UserCompany");
		return;
	}
}

void CUserData::DeleteContents()
{
	m_strUserName = m_strUserCompany = "";
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CInetnavPreferences data structure class
// This class contains variables for every preference in Internet Navigator
// and mananges serialization of preferences to and from the INI file

CInetnavPreferences::CInetnavPreferences()
{
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

void CInetnavPreferences::Initialize(CInternetNavApp* pApp /*=NULL*/)
{
	if (pApp)
		m_pApp = pApp;
	else
		m_pApp = (CInternetNavApp*)AfxGetApp();
		
	return;	
}

void CInetnavPreferences::Serialize(BOOL IsStoring /*TRUE*/)
{
	if (IsStoring) // we're not loading, we're saving
	{
		// Save settings to INI
		SaveProfileSettings(); 
	}
	else // we're not storing, we're loading
	{
		// Load settings from INI
		LoadProfileSettings();
	}
}

void CInetnavPreferences::SaveProfileSettings()
{
	CWaitCursor wait;
	CString strSect = "";
	
	// save general preferences for Application category
	strSect = "Application Preferences: General";
	m_pApp->WriteProfileInt(strSect, "ShowMainMenu", m_bShowMainMenu);
	m_pApp->WriteProfileInt(strSect, "StartOnline", m_bStartOnline);
	m_pApp->WriteProfileInt(strSect, "ConfirmExit", m_bConfirmExit);
	m_pApp->WriteProfileInt(strSect, "ShowTipsOnStartup", m_bShowTOD);     
	m_pApp->WriteProfileInt(strSect, "WhenToShowTips", m_nShowTips); 
	m_pApp->WriteProfileInt(strSect, "ConfirmSignOff", m_bConfirmSignOff);
	
	strSect = "Application Preferences: Areas";
	m_pApp->WriteProfileInt(strSect, "CheckMailOnStartup", m_bStartCheckMail);
	m_pApp->WriteProfileInt(strSect, "InetTimeOut", m_nInetTimeOut);
	
	// save Internet Users Information Area settings
	strSect = "Internet Preferences: Internet Users Information";
	m_pApp->WriteProfileInt(strSect, "AskFingerAddr", m_bAskFingerAddr);
	m_pApp->WriteProfileInt(strSect, "ReportQueryProgress", m_bFingerRQP);
	m_pApp->WriteProfileInt(strSect, "AutoSave", m_bAutoFingerSave);
	m_pApp->WriteProfileInt(strSect, "AutoPrint", m_bAutoFingerPrint);
	
	// save InterNIC Site Information area settings
	strSect = "Internet Preferences: InterNIC Site Information";
	m_pApp->WriteProfileInt(strSect, "ReportQueryProgress", m_bWhoisRQP);
	m_pApp->WriteProfileInt(strSect, "AutoSave", m_bAutoWhoisSave);
	m_pApp->WriteProfileInt(strSect, "AutoPrint", m_bAutoWhoisPrint);

	// save settings for FTP   
	strSect = "Internet Preferences: FTP";
	m_pApp->WriteProfileString(strSect, "DownloadDir", m_strFTPDownloadDir);
	m_pApp->WriteProfileString(strSect, "UploadDir", m_strFTPUploadDir);
	m_pApp->WriteProfileInt(strSect, "OpenTextOnTransfer", m_bFTPOpenASCII);
	m_pApp->WriteProfileInt(strSect, "OpenBinaryOnTransfer", m_bFTPOpenBin);
	m_pApp->WriteProfileInt(strSect, "WarnOpenBinary", m_bWarnFTPOpenBin); 
	m_pApp->WriteProfileString(strSect, "VirusScannerPath", m_strVirusPath);
	
	// save Gopher Center preferences
	strSect = "Internet Preferences: Gopher Center";
	m_pApp->WriteProfileString(strSect, "GopherDocumentDirectory",
			 m_strGopherDocDir);     
	m_pApp->WriteProfileInt(strSect, "DisplayGopherDocuments", 
			 m_bDisplayGopherDocs);   
	m_pApp->WriteProfileInt(strSect, "ShowSaveAs", m_bGopherShowSaveAs);

	// save Chat Center preferences  
	strSect = "Internet Preferences: Chat Center";
	m_pApp->WriteProfileString(strSect, "IRCServer", m_strIRCServerName);
	m_pApp->WriteProfileString(strSect, "IRCBackupServer", m_strIRCBackupServerName);
	m_pApp->WriteProfileString(strSect, "IRCName", m_strNickname);
	m_pApp->WriteProfileString(strSect, "ChatGroup", m_strInitialChannelPref);
	m_pApp->WriteProfileInt(strSect, "IRCServerPort", m_nIRCPort);
	m_pApp->WriteProfileInt(strSect, "ChatNotifyArrive", m_bChatNotifyArrive);
	m_pApp->WriteProfileInt(strSect, "ChatNotifyLeave", m_bChatNotifyLeave);
	m_pApp->WriteProfileString(strSect, "LocalHostID", m_strLocalHost);
	m_pApp->WriteProfileString(strSect, "OnlineHostID", m_strOnlineHost);
    
	// save settings for the Internet Newsgroups area 
	strSect = "Internet Preferences: Newsgroups";
	m_pApp->WriteProfileString(strSect, "NewsgroupsServer", m_pApp->m_strNNTPHost);    
	m_pApp->WriteProfileInt(strSect, "ArticleDisplayOption", m_nNNTPArticleDisplay);

    // save settings for Internet Newsgroups Articles
    strSect = "Internet Preferences: Newsgroups";
    m_pApp->WriteProfileInt(strSect, "Days", m_lArticleDays);
    m_pApp->WriteProfileInt(strSect, "Hours", m_nArticleHours);
    m_pApp->WriteProfileInt(strSect, "Minutes", m_nArticleMinutes);
    m_pApp->WriteProfileInt(strSect, "Seconds", m_nArticleSeconds);
	
	// save settings for Internet Mailboxes
	strSect = "Internet Preferences: Mail";
	m_pApp->WriteProfileInt(strSect, "ConfirmOnSend", m_bMailSendConfirm);
	m_pApp->WriteProfileInt(strSect, "CloseOnSend", m_bMailSendClose);
	m_pApp->WriteProfileInt(strSect, "AutoSetupNew", m_bMailSetupNew);
	    
    // Save Help & Information - Ask the Staff technical support addresses
    
    // TODO: Transfer the code for writing this information to the Internet
    // Provider Information data structure when it is written
    m_pApp->WriteProfileString("Ask the Staff", "WnDBSoft Address", 
    	"WnDBSoft@aol.com");
    
	// Save settings for the user's directories
	m_pApp->WriteProfileString("Directories", "Files", m_strFilesDirectory);
	                            
	// TODO: Add calls to WriteProfileString() and WriteProfileInt() to save
	// additional data and preferences to the INI file
		
	return;	
}

void CInetnavPreferences::LoadProfileSettings()
{
	CWaitCursor wait;
	
	CString strSect = "";
	
    // get general preferences for Application category
    strSect = "Application Preferences: General";
    m_bShowMainMenu = m_pApp->GetProfileInt(strSect, "ShowMainMenu", TRUE);
    m_bStartOnline = m_pApp->GetProfileInt(strSect, "StartOnline", TRUE);   
	m_bConfirmExit = m_pApp->GetProfileInt(strSect, "ConfirmExit", TRUE);
	m_bShowTOD = m_pApp->GetProfileInt(strSect, "ShowTipsOnStartup", TRUE);
	m_nShowTips = m_pApp->GetProfileInt(strSect, "WhenToShowTips", 1);  
	m_bConfirmSignOff = m_pApp->GetProfileInt(strSect, "ConfirmSignOff", TRUE);
	
	strSect = "Application Preferences: Areas";
	m_bStartCheckMail = m_pApp->GetProfileInt(strSect, "CheckMailOnStartup", TRUE);
	m_nInetTimeOut = m_pApp->GetProfileInt(strSect, "InetTimeOut", 20);
    
    // get the Internet Users Information preferences
	strSect = "Internet Preferences: Internet Users Information";
	m_bAskFingerAddr = m_pApp->GetProfileInt(strSect, "AskFingerAddr", TRUE);
	m_bFingerRQP = m_pApp->GetProfileInt(strSect, "ReportQueryProgress", TRUE);
	m_bAutoFingerSave = m_pApp->GetProfileInt(strSect, "AutoSave", TRUE);
	m_bAutoFingerPrint = m_pApp->GetProfileInt(strSect, "AutoPrint", TRUE);
		
	// get the InterNIC Site Information preferences
	strSect = "Internet Preferences: InterNIC Site Information";
	m_bWhoisRQP = m_pApp->GetProfileInt(strSect, "ReportQueryProgress", TRUE);
	m_bAutoWhoisSave = m_pApp->GetProfileInt(strSect, "AutoSave", TRUE);
	m_bAutoWhoisPrint = m_pApp->GetProfileInt(strSect, "AutoPrint", TRUE);
    
    // get settings for FTP
    strSect = "Internet Preferences: FTP";
    m_strFTPDownloadDir = m_pApp->GetProfileString(strSect, "DownloadDir");
    m_strFTPUploadDir = m_pApp->GetProfileString(strSect, "UploadDir");
    m_bFTPOpenASCII = m_pApp->GetProfileInt(strSect, "OpenTextOnTransfer", TRUE);
    m_bFTPOpenBin = m_pApp->GetProfileInt(strSect, "OpenBinaryOnTransfer", TRUE);
    m_bWarnFTPOpenBin = m_pApp->GetProfileInt(strSect, "WarnOpenBinary", TRUE);
    m_strVirusPath = m_pApp->GetProfileString(strSect, "VirusScannerPath");
		
    // get the Gopher Center preferences
    strSect = "Internet Preferences: Gopher Center";
    m_strGopherDocDir = 
    	m_pApp->GetProfileString(strSect, "GopherDocumentDirectory");
    m_bDisplayGopherDocs =
    	m_pApp->GetProfileInt(strSect, "DisplayGopherDocuments", TRUE);
    m_bGopherShowSaveAs =    
	m_pApp->GetProfileInt(strSect, "ShowSaveAs", FALSE);
    
    // get the Chat Center preferences
    strSect = "Internet Preferences: Chat Center";
    m_strIRCServerName = m_pApp->GetProfileString(strSect, "IRCServer");
    m_strIRCBackupServerName = m_pApp->GetProfileString(strSect, "IRCBackupServer");
    m_strNickname = m_pApp->GetProfileString(strSect, "IRCName");
    m_strInitialChannelPref = m_pApp->GetProfileString(strSect, "ChatGroup");
    m_strInitialChannel = m_strInitialChannelPref;
    m_nIRCPort = m_pApp->GetProfileInt(strSect, "IRCServerPort", 6667); 
    m_bChatNotifyArrive = m_pApp->GetProfileInt(strSect, "ChatNotifyArrive", TRUE);
    m_bChatNotifyLeave = m_pApp->GetProfileInt(strSect, "ChatNotifyLeave", TRUE);
    m_strLocalHost = m_pApp->GetProfileString(strSect, "LocalHostID", "LocalHost");
    m_strOnlineHost = m_pApp->GetProfileString(strSect, "OnlineHostID", "OnlineHost");
    

	// load settings for the Internet Newsgroups area as preferences
    strSect = "Internet Preferences: Newsgroups";
    m_pApp->m_strNNTPHost = 
    	m_pApp->GetProfileString(strSect, "NewsgroupsServer"); 
    m_nNNTPArticleDisplay = m_pApp->GetProfileInt(strSect,
    	"ArticleDisplayOption", 1);

	// load settings for Internet Newsgroups articles
    strSect = "Internet Preferences: Newsgroups";
    m_lArticleDays = (LONG)m_pApp->GetProfileInt(strSect, "Days", 14);
    m_nArticleHours = m_pApp->GetProfileInt(strSect, "Hours", 0);
    m_nArticleMinutes = m_pApp->GetProfileInt(strSect, "Minutes", 0);
    m_nArticleSeconds = m_pApp->GetProfileInt(strSect, "Seconds", 0);

	// save settings for Internet Mailboxes
	strSect = "Internet Preferences: Mail";
	m_bMailSendConfirm = m_pApp->GetProfileInt(strSect, "ConfirmOnSend", TRUE);
	m_bMailSendClose = m_pApp->GetProfileInt(strSect, "CloseOnSend", TRUE);
	m_bMailSetupNew = m_pApp->GetProfileInt(strSect, "AutoSetupNew", TRUE);
	    
    // Get settings for the user's directories
    m_strFilesDirectory = m_pApp->GetProfileString("Directories", "Files");
    
    // TODO: Add calls here to GetProfileString() and GetProfileInt() to load
    // additional data and preferences from the INI file      
    
	return;
}
		
/////////////////////////////////////////////////////////////////////////////
// Mailbox helpers

BOOL CInternetNavApp::AddNewMailbox()
{  
	CDocument* pMailbox = NULL;
	
	if (m_lpCmdLine[0] == '/' && m_lpCmdLine[1] == 'S')
	{
		// Set the user up with a new mailbox             
		// using CMailBoxDoc::OnNewDocument()'s setup procedure
		pMailbox = m_pMailBox->OpenDocumentFile(NULL);
		GetMainFrame()->ShowWindow(SW_SHOWMINNOACTIVE);
		m_pMailBox->CloseAllDocuments(TRUE);
		GetMainFrame()->ShowWindow(SW_SHOWMINNOACTIVE);
		GetMainFrame()->UpdateWindow();
		
		return (pMailbox == NULL)?FALSE:TRUE;
	}
	else if (m_lpCmdLine[0] == '/' && m_lpCmdLine[1] == 'M'
			&& m_lpCmdLine[2] == 'A' && m_lpCmdLine[3] == 'I')
	{
		// Set the user up with a new mailbox             
		// using CMailBoxDoc::OnNewDocument()'s setup procedure
		pMailbox = m_pMailBox->OpenDocumentFile(NULL);
		GetMainFrame()->ShowWindow(SW_SHOWMINNOACTIVE);
		m_pMailBox->CloseAllDocuments(TRUE);
		GetMainFrame()->ShowWindow(SW_SHOWMINNOACTIVE);
		GetMainFrame()->UpdateWindow();
		
		return (pMailbox == NULL)?FALSE:TRUE;
	}
	else
	{
		pMailbox = m_pMailBox->OpenDocumentFile(NULL);
		m_pMailBox->CloseAllDocuments(TRUE);
	}
		
	return (pMailbox == NULL)?FALSE:TRUE;
}

BOOL CInternetNavApp::RegisterMailbox(const CString& strName,
	const CString& strPath, const CString& strAddr, const CString& strUser,
		const CString& strOrg)
{
	BOOL bResult = TRUE;
	
	for (int i = 0;i < m_strMailboxPathList.GetSize();i++)
	{
		if (m_strMailboxPathList[i] == strPath)
		{
			AfxMessageBox(IDP_MAILBOX_EXISTS, MB_ICONSTOP);
			bResult = FALSE;
			break;
		}
	}
	
	for (int c = 0;c < m_strMailboxNameList.GetSize();c++)
	{
		if (m_strMailboxNameList[c] == strName)
		{
			AfxMessageBox(IDP_MAILBOX_EXISTS, MB_ICONSTOP);
			bResult = FALSE;
			break;
		}
	}
	
	if (!bResult)
		return bResult;
	
	m_mapMailboxNameToPath.SetAt(strName, strPath);
	m_mapMailboxPathToName.SetAt(strPath, strName);
	m_mapMailboxNameToAddress.SetAt(strName, strAddr);
	m_mapMailboxNameToUserName.SetAt(strName, strUser);
	m_mapMailboxNameToOrg.SetAt(strName, strOrg);
	
	m_strMailboxPathList.Add(strPath);
	m_strMailboxNameList.Add(strName);                   
	m_strMailboxAddressList.Add(strAddr);  
	m_strMailboxUserNameList.Add(strUser);
	m_strMailboxOrgList.Add(strOrg);
	
	return bResult;
}

BOOL CInternetNavApp::RemoveMailboxName(const CString& strName)
{
	CString strPath = GetMailboxPath(strName);
	if (strPath.IsEmpty())
	{
		AfxMessageBox("Unable to find mailbox.");
		return FALSE;
	}
	
	CString strAddr = GetMailboxAddress(strName);
	CString strUser = GetMailboxUser(strName);  
	CString strOrg = GetMailboxOrg(strName);
	
	RemoveMailbox(strName, strPath, strAddr, strUser, strOrg);
	return TRUE;
}                            

BOOL CInternetNavApp::RemoveMailboxPath(const CString& strPath)
{
	CString strName = GetMailboxName(strPath);
	if (strName.IsEmpty())
	{
		AfxMessageBox("Unable to find mailbox.");
		return FALSE;
	}
	
	CString strAddr = GetMailboxAddress(strName);   
	CString strUser = GetMailboxUser(strName);  
	CString strOrg = GetMailboxOrg(strName);	
	
	RemoveMailbox(strName, strPath, strAddr, strUser, strOrg);
	return TRUE;    
}

BOOL CInternetNavApp::RemoveMailbox(const CString& strName, 
	const CString& strPath, const CString& strAddr, const CString& strUser,
		const CString& strOrg)
{                                
	if (!m_mapMailboxNameToPath.RemoveKey(strName) 
			|| !m_mapMailboxPathToName.RemoveKey(strPath)
		|| !m_mapMailboxNameToAddress.RemoveKey(strName) ||
			!m_mapMailboxNameToUserName.RemoveKey(strName) ||
		!m_mapMailboxNameToOrg.RemoveKey(strName))
	{
		AfxMessageBox("Unable to find mailbox.");
		return FALSE;
	}	   
	
	int i = 0;
	for (i = 0;i < m_strMailboxPathList.GetSize();i++)
	{
		if (m_strMailboxPathList[i] == strPath)
		{
			m_strMailboxPathList.RemoveAt(i);
			break;
		}
	}
	
	for (i = 0;i < m_strMailboxNameList.GetSize();i++)
	{
		if (m_strMailboxNameList[i] == strName)
		{
			m_strMailboxNameList.RemoveAt(i);
			break;
		}
	}
	
	for (i = 0;i < m_strMailboxAddressList.GetSize();i++)
	{
		if (m_strMailboxAddressList[i] == strAddr)
		{
			m_strMailboxAddressList.RemoveAt(i);
			break;
		}
	}                
	
	for (i = 0;i < m_strMailboxUserNameList.GetSize();i++)
	{
		if (m_strMailboxUserNameList[i] == strUser)
		{
			m_strMailboxUserNameList.RemoveAt(i);
			break;
		}
	}

	for (i = 0;i < m_strMailboxOrgList.GetSize();i++)
	{
		if (m_strMailboxOrgList[i] == strOrg)
		{
			m_strMailboxOrgList.RemoveAt(i);
			break;
		}
	}
		
	if (IsDefaultMailbox(strName, strPath))
	{
		SetDefaultMailbox("", "", "", "", ""); 
	}     
	
	// Now delete the mailbox file
	CFile::Remove(strPath);
	return TRUE;
}                       

BOOL CInternetNavApp::OpenDefaultMailbox(BOOL bCompose)
{
	if (m_strDefMailboxPath.IsEmpty())
		return FALSE;
	
	CDocument* pDoc = NULL;	
	if (bCompose)
		pDoc = m_pMailCompose->OpenDocumentFile(m_strDefMailboxPath);
	else
		pDoc = m_pMailBox->OpenDocumentFile(m_strDefMailboxPath);
	
	if (pDoc == NULL)
		return FALSE;
	return TRUE;
}
	
BOOL CInternetNavApp::OpenMailboxName(const CString& strName)
{
	CString strPath = GetMailboxPath(strName);
	if (strPath.IsEmpty())
	{
		AfxMessageBox("Unable to find mailbox.");
		return FALSE;
	}
	
	CMailBoxDoc* pDoc = (CMailBoxDoc*)m_pMailBox->OpenDocumentFile(strPath);
	
	if (pDoc && !IsDefaultMailbox(strName, strPath))
	{
		switch(AfxMessageBox(IDP_MAKE_MAILBOX_DEFAULT,
					MB_YESNO|MB_ICONQUESTION))
		{
			case IDYES:
				SetDefaultMailbox(strName, strPath, pDoc->m_strAddress,
					pDoc->m_strUserName, pDoc->m_strOrg);
				AfxMessageBox("\'" + strName + "\' is now the default"
					" mailbox.", MB_ICONASTERISK);
				break;
			
			case IDNO:
				break;
		}
		return TRUE;
	}
	
	if (pDoc)	
		return TRUE;
	else	
		return FALSE;
}			

BOOL CInternetNavApp::OpenMailboxPath(const CString& strPath)
{
	CString strName = GetMailboxName(strPath);
	if (strName.IsEmpty())
	{
		AfxMessageBox("Unable to find mailbox.");
		return FALSE;
	}
	
	CMailBoxDoc* pDoc = (CMailBoxDoc*)m_pMailBox->OpenDocumentFile(strPath);
	
	if (pDoc && !IsDefaultMailbox(strName, strPath))
	{
		switch(AfxMessageBox(IDP_MAKE_MAILBOX_DEFAULT,
					MB_YESNO|MB_ICONQUESTION))
		{
			case IDYES:
				SetDefaultMailbox(strName, strPath, pDoc->m_strAddress,
					pDoc->m_strUserName, pDoc->m_strOrg);
				AfxMessageBox("\'" + strName + "\' is now the default"
					" mailbox.", MB_ICONASTERISK);
				break;
			
			case IDNO:
				break;
		}
		return TRUE;
	}
	
	if (pDoc)	
		return TRUE;
	else	
		return FALSE;
}	

CString CInternetNavApp::GetMailboxPath(const CString& strName)
{
	CString strResult;
	strResult.Empty();
	
	if (!m_mapMailboxNameToPath.Lookup(strName, strResult))
	{  
		strResult.Empty();
		return strResult;
	}
	
	return strResult;
}
	
CString CInternetNavApp::GetMailboxName(const CString& strPath)
{
	CString strResult;
	strResult.Empty();
	
	if (!m_mapMailboxPathToName.Lookup(strPath, strResult))
	{  
		strResult.Empty();
		return strResult;
	}
	
	return strResult;
}

CString CInternetNavApp::GetMailboxAddress(const CString& strName)
{
	CString strResult;
	strResult.Empty();
	
	if (!m_mapMailboxNameToAddress.Lookup(strName, strResult))
	{
		strResult.Empty();
		return strResult;
	}
	
	return strResult;
}

CString CInternetNavApp::GetMailboxUser(const CString& strName)
{
	CString strResult;
	strResult.Empty();
	
	if (!m_mapMailboxNameToUserName.Lookup(strName, strResult))
	{
		strResult.Empty();
		return strResult;
	}
	
	return strResult;
}

CString CInternetNavApp::GetMailboxOrg(const CString& strName)
{
	CString strResult;
	strResult.Empty();
	
	if (!m_mapMailboxNameToOrg.Lookup(strName, strResult))
	{
		strResult.Empty();
		return strResult;
	}
	
	return strResult;
}
	
BOOL CInternetNavApp::IsDefaultMailbox(const CString& strName,
	const CString& strPath)
{     
	LoadMailboxes();
	
	BOOL bResult = FALSE;
	
	if ((m_strDefMailboxName.IsEmpty()) && (m_strDefMailboxPath.IsEmpty()))
		bResult = FALSE;
		
	if ((m_strDefMailboxName == strName) && (m_strDefMailboxPath == strPath))
		bResult = TRUE;
	return bResult;
}
				
void CInternetNavApp::SetDefaultMailbox(const CString& strName,
	const CString& strPath, const CString& strAddress, const CString& strUser,
		const CString& strOrg)
{ 
	CWaitCursor wait;       
	// All we need to do for the default mailbox is set variables
	// and write information to INI
	
	m_strDefMailboxName = strName;
	m_strDefMailboxPath = strPath;
	m_strDefAddress = strAddress;    
	m_strDefMailboxUser = strUser;
	m_strDefMailboxOrg = strOrg;
	
	WriteProfileString("Default Mailbox", "MailboxName", strName);
	WriteProfileString("Default Mailbox", "MailboxPath", strPath);
	WriteProfileString("Default Mailbox", "MailboxAddress", strAddress);   
	WriteProfileString("Default Mailbox", "MailboxUser", strUser);
	WriteProfileString("Default Mailbox", "MailboxOrg", strOrg);
}

void CInternetNavApp::LoadMailboxes()
{  
	char szPathEntry[100] = "MailboxPath%d";
	char szNameEntry[100] = "MailboxName%d"; 
	char szAddrEntry[100] = "MailboxAddr%d";  
	char szUserEntry[100] = "MailboxUser%d";
	char szOrgEntry[100] = "MailboxOrg%d";
	char szEntry[100];
	
	CString title = "Loading Mailboxes...";
	CString text = "Please wait while we load your mailboxes.  Thank you ";
	text += "for your patience!";

	CWaitCursor wait;
	
	m_strDefMailboxName = GetProfileString("Default Mailbox", "MailboxName");
	m_strDefMailboxPath = GetProfileString("Default Mailbox", "MailboxPath");
	m_strDefAddress = GetProfileString("Default Mailbox", "MailboxAddress");
	m_strDefMailboxUser = GetProfileString("Default Mailbox", "MailboxUser");
	m_strDefMailboxOrg = GetProfileString("Default Mailbox", "MailboxOrg");	
	
	int nSize = GetProfileInt("Mailboxes", "Quantity", 0);
	if (nSize == 0)
		return;
	
	if (m_strMailboxPathList.GetSize() > 0)
		m_strMailboxPathList.RemoveAll();
		
	if (m_strMailboxNameList.GetSize() > 0)
		m_strMailboxNameList.RemoveAll();    
	
	if (m_strMailboxAddressList.GetSize() > 0)
		m_strMailboxAddressList.RemoveAll();  
	
	if (m_strMailboxUserNameList.GetSize() > 0)
		m_strMailboxUserNameList.RemoveAll();
	
	if (m_strMailboxOrgList.GetSize() > 0)
		m_strMailboxOrgList.RemoveAll();
	
	// Load in mailbox paths & names
	for (int i = 0; i < nSize; i++)
	{
		wsprintf(szEntry, szNameEntry, i + 1);
		m_strMailboxNameList.Add(GetProfileString("Mailboxes", szEntry));
		
		wsprintf(szEntry, szPathEntry, i + 1);
		m_strMailboxPathList.Add(GetProfileString("Mailboxes", szEntry));
		
		wsprintf(szEntry, szAddrEntry, i + 1);
		m_strMailboxAddressList.Add(GetProfileString("Mailboxes", szEntry));
		
		wsprintf(szEntry, szUserEntry, i + 1);
		m_strMailboxUserNameList.Add(GetProfileString("Mailboxes", szEntry));

		wsprintf(szEntry, szOrgEntry, i + 1);
		m_strMailboxOrgList.Add(GetProfileString("Mailboxes", szEntry));			
	}		                                                             
	
	// Add paths and names to the maps
	if (!m_mapMailboxPathToName.IsEmpty())
		m_mapMailboxPathToName.RemoveAll(); 
	
	if (!m_mapMailboxNameToPath.IsEmpty())
		m_mapMailboxNameToPath.RemoveAll();
	
	if (!m_mapMailboxNameToAddress.IsEmpty())
		m_mapMailboxNameToAddress.RemoveAll(); 
	
	if (!m_mapMailboxNameToUserName.IsEmpty())
		m_mapMailboxNameToUserName.RemoveAll(); 

	if (!m_mapMailboxNameToOrg.IsEmpty())
		m_mapMailboxNameToOrg.RemoveAll(); 
	
	for (int c = 0;c < nSize;c++)
	{
		m_mapMailboxPathToName.SetAt(m_strMailboxPathList[c],
					m_strMailboxNameList[c]);
		m_mapMailboxNameToPath.SetAt(m_strMailboxNameList[c],
					m_strMailboxPathList[c]); 
		m_mapMailboxNameToAddress.SetAt(m_strMailboxNameList[c],
					m_strMailboxAddressList[c]);     
		m_mapMailboxNameToUserName.SetAt(m_strMailboxNameList[c],
					m_strMailboxUserNameList[c]);
		m_mapMailboxNameToOrg.SetAt(m_strMailboxNameList[c],
					m_strMailboxOrgList[c]);
	}
}

void CInternetNavApp::SaveMailboxes()
{
	CWaitCursor wait;
	
	char szPathEntry[100] = "MailboxPath%d";
	char szNameEntry[100] = "MailboxName%d";
	char szAddrEntry[100] = "MailboxAddr%d";   
	char szUserEntry[100] = "MailboxUser%d";  
	char szOrgEntry[100] = "MailboxOrg%d";

	WriteProfileString("Default Mailbox", "MailboxName", m_strDefMailboxName);
	WriteProfileString("Default Mailbox", "MailboxPath", m_strDefMailboxPath);
	WriteProfileString("Default Mailbox", "MailboxAddress", m_strDefAddress);   
	WriteProfileString("Default Mailbox", "MailboxUser", m_strDefMailboxUser);
	WriteProfileString("Default Mailbox", "MailboxOrg", m_strDefMailboxOrg);
	
	WriteProfileInt("Mailboxes", "Quantity", m_strMailboxPathList.GetSize());
	
	int nSize = m_strMailboxPathList.GetSize();
	
	for (int i = 0; i < nSize; i++)
	{
		char szEntry[100];
		wsprintf(szEntry, szNameEntry, i + 1);
		WriteProfileString("Mailboxes", szEntry, m_strMailboxNameList[i]);
		
		wsprintf(szEntry, szPathEntry, i + 1);
		WriteProfileString("Mailboxes", szEntry, m_strMailboxPathList[i]);
		
		wsprintf(szEntry, szAddrEntry, i + 1);
		WriteProfileString("Mailboxes", szEntry, m_strMailboxAddressList[i]);
		
		wsprintf(szEntry, szUserEntry, i + 1);
		WriteProfileString("Mailboxes", szEntry, m_strMailboxUserNameList[i]);
		
		wsprintf(szEntry, szOrgEntry, i + 1);
		WriteProfileString("Mailboxes", szEntry, m_strMailboxOrgList[i]);			
	} 

	return;
}
		            	
/////////////////////////////////////////////////////////////////////////////
// Now back to CInternetNavApp message handlers...

void CInternetNavApp::OnChatSendCommand()
{       
	if (IRCIsConnected())
	{
		CWaitCursor wait;
		CIRCSendCmdDialog theDialog(GetMainFrame());
		if (theDialog.DoModal() == IDOK)
		{   
			wait.Restore();
			if (!theDialog.m_strCommand.IsEmpty())
			{
				GetIRCSocket()->Send(theDialog.m_strCommand);
				CString strMsg = "The command you entered has been sent to ";
				strMsg += GetIRCServerName() + '.';
				m_pStatusBar->SetText(strMsg);    
				strMsg.Empty();
				strMsg = "OnlineHost: Command " + theDialog.m_strCommand;
				strMsg += " has been received from Internet Navigator.";
				strMsg += " Please wait, a response will be shown shortly...";
				strMsg += "  Thank you!";
				GetMainFrame()->PutInActive(strMsg);
				strMsg.Empty();
			}
		}    
		m_pStatusBar->ShowIdleMessage();
		return;
	}
}

void CInternetNavApp::OnTipOfDay()
{   
  CWaitCursor wait;                
  Serialize(FALSE);
  int nIndex = GetProfileInt("Application Preferences: General","TipOfDayIndex", 28);
  
  if (IsWindow(m_wndSplash.m_hWnd)) m_wndSplash.DestroyWindow();
  
  // create the T.O.D. dialog box; pass variables from profile  
  CTipOfDay csTip(nIndex, IsShowTOD(), GetMainFrame());        
  
  // run the dialog box
  wait.Restore();
  if (csTip.DoModal() == IDOK)
  {
     SetShowTOD(csTip.m_bShowTips);
  
  	// save T.O.D.-related variables back to profile  
	
  	WriteProfileInt("Application Preferences: General", "TipOfDayIndex", csTip.GetIndex());
  	Serialize(TRUE);

  	return;
  }                

  return;
}

void CInternetNavApp::OnUpdatePlacesPostOffice(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsOnline());
}

void CInternetNavApp::OnMailboxCompose()
{
	CWaitCursor wait;
	CMultiDocTemplate* pTempl = GetComposeMailType();
	
	// For now, we will bring up a Compose Mail window with the title
	// "Technical Support"
	CString strMailPath = m_strDefMailboxPath;
	

	CMailBoxDoc* pDocument = (CMailBoxDoc*)pTempl->CreateNewDocument();
	if (pDocument == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CComposeWnd* pFrame = 
		(CComposeWnd*)pTempl->CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);    
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	if (strMailPath.IsEmpty())
	{
		if (!IsMailSetupNew())
		{
			pFrame->DestroyWindow();
			return;                 
		}
		
		switch (AfxMessageBox(IDP_SETUP_NEW_MAILBOX,
					MB_ICONQUESTION|MB_YESNO))
		{
			case IDYES:
				if (!pDocument->OnNewDocument())
				{
					// user has be alerted to what failed in OnNewDocument
					TRACE0("CDocument::OnNewDocument returned FALSE\n");
					pFrame->DestroyWindow();
				}
				break;
			
			case IDNO:
				m_pStatusBar->ShowIdleMessage(); 
				pFrame->DestroyWindow();
				break;
		}
		return;
	}
	else
	{
		// open an existing document
		BeginWaitCursor();
		if (!pDocument->OnOpenDocument(strMailPath))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			pFrame->DestroyWindow();
			EndWaitCursor();
			return;
		}
		pDocument->SetPathName(strMailPath);
		EndWaitCursor();
	}
	
	pFrame->m_bIsUseCompWizard = TRUE; 
	pFrame->m_bIsUseFeedbackWizard = FALSE;

	pFrame->SetWindowText("Compose Mail"); 
	pFrame->m_bIsInetnavTechSupport = FALSE;
	pFrame->m_bIsISPTechSupport = FALSE;
	pFrame->m_bIsSuggestionBox = FALSE;      
	pFrame->m_bIsUseCompWizard = TRUE; 
	pFrame->m_bIsUseFeedbackWizard = FALSE;

	pFrame->GetInfoBar().ShowWindow(SW_SHOW);
	pFrame->GetInfoBar().UpdateWindow();
	pFrame->RecalcLayout();
		
	pFrame->GetRulerBar().ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	pFrame->RecalcLayout();
		
	pTempl->InitialUpdateFrame(pFrame, pDocument, TRUE);

	return;
}

void CInternetNavApp::OnArticleAdd()
{
	CWaitCursor wait;
	CString strPath = GetProfileString("Internet Newsgroups", 
			"AreaFile");
	
	if (!strPath.IsEmpty())  
	{
		GetCNA()->OpenDocumentFile(strPath);
		return;
	}
	else
	{
		// The user needs to set up a mailbox if they already haven't
		// done so, in order to use it to compose a mail message.
		// We will tell the user this, and in so doing, also will
		// ask the user if they want to set up a new mailbox now.
		MessageBeep(-1);
		switch (AfxMessageBox(IDP_SETUP_NEWSGROUPS,
					MB_ICONQUESTION|MB_YESNOCANCEL))
		{
			case IDYES:
				GetCNA()->OpenDocumentFile(NULL);
				break;
			
			case IDNO:
				AfxMessageBox("Unable to open Compose News Article window."
						"  Choose the Newsgroups command from "
						"the Places menu when you want to access newsgroups"
							" later.");
				break;
			
			case IDCANCEL:
				break;
		}
		return;
	}
	return;
}

void CInternetNavApp::OnFileOpen()
{   
	CWaitCursor wait;
	// prompt the user (with all document templates)
	CString newName = "";
	if (!CInternetNavApp::DoPromptFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

	OpenDocumentFile(newName);
		// if returns NULL, the user has already been alerted
}

static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt, BOOL bOpen)
{
	ASSERT_VALID(pTemplate);
	ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));

	CString strFilterExt = "", strFilterName = "";
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((const char*)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPSTR)(const char*)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (char)'\0';       // next string please
		filter += "*" + strFilterExt;
		filter += (char)'\0';       // next string please
		ofn.nMaxCustFilter++;
	}
}

// prompt for file name - used for open and save as
BOOL CInternetNavApp::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
	BOOL bOpenFileDialog, CDocTemplate* pTemplate)
		// if pTemplate==NULL => all document templates
{  
	// CFileDialog -- a special file dialog box which centers itself on
	// the screen
	CFileDialog dlgFile(bOpenFileDialog, NULL, NULL, lFlags, NULL, GetMainFrame());
    
    // Change the working directory to the C:\INETNAV\FILES directory for the
    // user  
    if ((fileName.Find('.') <= 10) ||  // a directory name is not in front
    		(bOpenFileDialog == TRUE))	
    {
    	// Make really sure that the current working directory is the
    	// directory we have for data files, C:\INETNAV\FILES
    	_chdir(GetFilesDirectory());
    	_chdir(GetFilesDirectory());
    	_chdir(GetFilesDirectory());                         
    }
    
	CString title = "";
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;
    dlgFile.m_ofn.lpstrInitialDir = GetFilesDirectory();
    
	CString strFilter = "";
	CString strDefault = "";
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault, bOpenFileDialog);
	}
	else
	{
		// do for all doc templates
		POSITION pos = m_templateList.GetHeadPosition();
		while (pos != NULL)
		{
			AppendFilterSuffix(strFilter, dlgFile.m_ofn,
				(CDocTemplate*)m_templateList.GetNext(pos), NULL, bOpenFileDialog);
		}
	}

	// append the "*.*" all files filter
	CString allFilter = "";
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (char)'\0';        // next string please
	strFilter += "*.*";
	strFilter += (char)'\0';        // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	BOOL bRet = dlgFile.DoModal() == IDOK ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	return bRet;
}

void CInternetNavApp::OnMailboxCheck()
{
	CWaitCursor wait;     
	LoadMailboxes();
	
	// Check the user's mailbox by opening the user's mailbox and displaying
	// their mail
	CMultiDocTemplate* pTempl = GetInboxType();
	
	// Bring up the 'New Mail' window
	CString strMailPath = m_strDefMailboxPath;

	CMailBoxDoc* pDocument = (CMailBoxDoc*)pTempl->CreateNewDocument();
	if (pDocument == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CNewMailWnd* pFrame = 
		(CNewMailWnd*)pTempl->CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);    
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	if (strMailPath.IsEmpty())
	{
		if (!IsMailSetupNew())
		{
			pFrame->DestroyWindow();
			return;                 
		}
		
		switch (AfxMessageBox(IDP_SETUP_NEW_MAILBOX,
					MB_ICONQUESTION|MB_YESNO))
		{
			case IDYES:
				if (!pDocument->OnNewDocument())
				{
					// user has be alerted to what failed in OnNewDocument
					TRACE0("CDocument::OnNewDocument returned FALSE\n");
					pFrame->DestroyWindow();   
					return;
				}
				break;
			
			case IDNO:
				m_pStatusBar->ShowIdleMessage();  
				pFrame->DestroyWindow();
				return;
		}
	}
	else
	{
		// open an existing document
		BeginWaitCursor();
		if (!pDocument->OnOpenDocument(strMailPath))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			pFrame->DestroyWindow();
			EndWaitCursor();
			return;
		}
		pDocument->SetPathName(strMailPath);
		EndWaitCursor();
	}
    
	pTempl->InitialUpdateFrame(pFrame, pDocument, TRUE);

	CNewMailView* pView = (CNewMailView*)pFrame->GetActiveView();
	pView->SendMessage(WM_COMMAND, ID_MAILBOX_CHECK);
	return;
}

void CInternetNavApp::OnCancelCall()
{
/*	// If we're trying to connect to the Internet and the user gets
	// tired of waiting, then we should cancel the blocking call here
	// when they press ESC.
	if (m_bPseudoOnline && WSAIsBlocking())
	{
		// Cancel the outstanding blocking call.
		WSACancelBlockingCall();
		m_pStatusBar->ShowIdleMessage();
		
		// The rest happens in OnPlacesSignOn()
		return;
	}*/
	return;
}

void CInternetNavApp::OnInternetMailboxes()
{   
	LoadMailboxes();
	CInternetMailboxes theDialog(GetMainFrame());
	theDialog.m_strDefMailbox = m_strDefMailboxName;
	
	theDialog.DoModal();   
	SaveMailboxes();
}
