// ftp.cpp: implementation of the CFtpDoc, CFtpView, and CFtpWnd classes
//

#include "stdafx.h"
#include "inetnav.h"  
#include "dialogs.h"

#include "ftp.h"
#include <stdio.h>  

#include "propshts.h"

#include "mainfrm.h"
#include "statbar.h"

#include <shellapi.h>
#include <math.h>
#include <stdlib.h>
#include "appd.h" 

#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFtpDoc

IMPLEMENT_DYNCREATE(CFtpDoc, CDocument)

BEGIN_MESSAGE_MAP(CFtpDoc, CDocument)
	//{{AFX_MSG_MAP(CFtpDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpDoc construction/destruction

CFtpDoc::CFtpDoc()
{
	//{{AFX_DATA_INIT(CFtpDoc)
	m_strSiteName = "";
	m_strUserID = "";
	m_strPassword = "";
	//}}AFX_DATA_INIT 
	m_bEmergencyClose = FALSE;
	m_pApp = (CInternetNavApp*)AfxGetApp(); 
	m_bFTPAutoConnect = FALSE;
}

/*
//{{AFX_DATA_MAP(CFtpDoc)
//}}AFX_DATA_MAP
*/
CFtpDoc::~CFtpDoc()
{ 
	m_strSiteName = m_strUserID = m_strPassword = "";
}

BOOL CFtpDoc::OnNewDocument()
{
	CInternetNavApp*  pApp = (CInternetNavApp*)AfxGetApp(); 
	if (!pApp->IsOnline())
	{
		AfxMessageBox(IDP_INETNAV_SIGN_ON);
		return FALSE;
	}

	if (!CDocument::OnNewDocument())
		return FALSE;

	// Set this document's title to "FTP Client", so that
	// something better than "Download Files1" appears on the frame
	// window's title bar
	SetTitle("FTP Client");
	
	m_strSiteName = m_strUserID = m_strPassword = "";
	
	// Get the site address to open and whether or not to use anonymous
	// FTP from the user with a dialog
	COpenFtpSite theDialog(AfxGetMainWnd());
	
	if (theDialog.DoModal() == IDOK)
	{
		if (theDialog.m_bUseAnonFTP)
		{
			if (theDialog.m_strSite != "pweb.netcom.com")
			{
				m_strUserID = "anonymous";
				m_strPassword = m_pApp->GetProfileString("Default Mailbox", 
						"MailboxAddress");
			}
			else
			{ 
				// Anonymous access to pweb.netcom.com is not allowed
				m_strUserID = "";
				m_strPassword = "";
			}
		}
		
		m_strSiteName = theDialog.m_strSite;        
		m_bFTPAutoConnect = TRUE;
		
		SetModifiedFlag(TRUE);
	}
	else  // user pressed the Cancel button in the dialog.
	{  
		// Don't go any further -- the user doesn't want to enter the
		// FTP Browser area 
		m_bFTPAutoConnect = FALSE;
		return FALSE;
	}
	
	SetPathName("somesite.ftp", FALSE); // set path name so file save works
	m_strPathTitle = "FTP Browser"; 

	return TRUE;
}

BOOL CFtpDoc::OnOpenDocument(const char* pszPathName)
{
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();          
	if (!pApp->IsOnline())
	{       
		MessageBeep(-1);
		AfxMessageBox(IDP_INETNAV_SIGN_ON, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	
	if (!CDocument::OnOpenDocument(pszPathName)) 
	{
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;                             
	}  

	return TRUE;
}

BOOL CFtpDoc::OnSaveDocument(const char* pszPathName)
{
	return CDocument::OnSaveDocument(pszPathName);
}

BOOL CFtpDoc::SaveModified()
{
	if (!IsModified())
		return TRUE;        // ok to continue

	CString strName = "the FTP connection information";

	CString prompt = "";
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, strName); 
	if (!m_bEmergencyClose)
	{
		switch (AfxMessageBox(prompt, MB_YESNOCANCEL|MB_ICONQUESTION, 
					AFX_IDP_ASK_TO_SAVE))
		{
			case IDCANCEL:
				return FALSE;       // don't continue
	
			case IDYES:
				// If so, either Save or Update, as appropriate
				CDocument::OnFileSave();
				break;
	
			case IDNO:
				// If not saving changes, revert the document
				break;

			default:
				ASSERT(FALSE);
				break;
		}
	}
	else if (m_bEmergencyClose)
	{
		m_bEmergencyClose = FALSE;
		switch (AfxMessageBox(prompt, MB_YESNO|MB_ICONQUESTION, 
					AFX_IDP_ASK_TO_SAVE))
		{
			case IDYES:
				// If so, either Save or Update, as appropriate
				CDocument::OnFileSave();
				break;
	
			case IDNO:
				// If not saving changes, revert the document
				break;

			default:
				ASSERT(FALSE);
				break;
		}     
	}
	return TRUE;    // keep going
}

void CFtpDoc::SetPathName(const char* pszPathName, BOOL bAddToMRU)
{
	m_strPathName = pszPathName;
	ASSERT(!m_strPathName.IsEmpty());       // must be set to something

	if (bAddToMRU)
		AfxGetApp()->AddToRecentFileList(pszPathName);
	
	return;
}

void CFtpDoc::SetTitle(const char* pszTitle, BOOL bFTPDir /*= FALSE*/)
{
	CString strTitle = "FTP Browser";
	if (m_strSiteName.IsEmpty())
	{       
		m_strTitle = strTitle;
		UpdateFrameCounts();
		return;
	}
	else if (!m_strSiteName.IsEmpty() && bFTPDir == FALSE)
	{
		m_strTitle = GetSiteName();
		m_strTitle = GetSiteName();
		m_strTitle = m_strSiteName;
		m_strTitle = m_strSiteName;
		UpdateFrameCounts();
		return;
	}          
	else if (!m_strSiteName.IsEmpty() && bFTPDir == TRUE)
	{
		// directory name is in pszTitle
		m_strTitle = m_strSiteName + ":";
		m_strTitle += pszTitle;
		UpdateFrameCounts();
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFtpDoc data handling: serialization to/from a file, and deleting data

void CFtpDoc::Serialize(CArchive& ar)
{
	// This function is called to store and load to/from a disk file the
	// document's data, which are the site name, the user's ID, and the
	// user's password.
	if (ar.IsStoring())
	{  
		// save the data to the archive with the << operator
		ar << m_strSiteName;
		ar << m_strUserID;
		ar << m_strPassword;
	}
	else
	{           
		// load the data from the archive with the >> operator
		ar >> m_strSiteName;
		ar >> m_strUserID;
		ar >> m_strPassword;
	}
}

void CFtpDoc::DeleteContents()
{   
	// This function is called to delete the document data, which are the
	// site name, the user's ID, and the user's password
	
	m_strSiteName.Empty();
	m_strUserID.Empty();
	m_strPassword.Empty();         
	
	m_bEmergencyClose = FALSE;
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFtpDoc diagnostics

#ifdef _DEBUG
void CFtpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFtpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFtpDoc commands

void CFtpDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// Enable the File Save command only if our data has been changed
	// by the user
	pCmdUI->Enable(IsModified());
}

void CFtpDoc::OnToolsFileManager()
{
	// Run the Windows File Manager, WINFILE.EXE, in a maximized window
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
    
    if (!pApp->IsWindows95())
		pApp->m_pStatusBar->SetText("Opening File Manager...  Please wait.");
	else
		pApp->m_pStatusBar->SetText("Opening Windows Explorer...  Please wait.");
	
	if (!pApp->IsWindows95())
		::WinExec("WINFILE.EXE", SW_SHOWMAXIMIZED);
	else // open the Windows Explorer
		::WinExec("EXPLORE.EXE", SW_SHOWMAXIMIZED);
	
	pApp->m_pStatusBar->ShowIdleMessage();
	return;                                                                    
}

/////////////////////////////////////////////////////////////////////////////
// CFtpView

IMPLEMENT_DYNCREATE(CFtpView, CFormView)

BEGIN_MESSAGE_MAP(CFtpView, CFormView)
	//{{AFX_MSG_MAP(CFtpView)
	ON_LBN_DBLCLK(IDC_FILES, OnDblclkFiles)
	ON_LBN_SELCHANGE(IDC_FILES, OnSelchangeFiles)
	ON_BN_CLICKED(IDC_HELP, OnClickedHelp)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
	ON_BN_CLICKED(IDC_GET, OnGet)
	ON_BN_CLICKED(IDC_PUT, OnPut)
	ON_BN_CLICKED(IDC_CHANGE_DIRECTORY_UP, OnChangeDirectoryUp)
	ON_BN_CLICKED(IDC_CHANGE_REMOTE_DIRECTORY, OnChangeRemoteDirectory)
	ON_EN_CHANGE(IDC_SITE_NAME, OnEditChange)
	ON_COMMAND(ID_WINDOW_REFRESH, OnWindowRefresh)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_REFRESH, OnUpdateWindowRefresh)
	ON_UPDATE_COMMAND_UI(ID_SITE_CHG_DIR, OnUpdateSiteChgDir)
	ON_UPDATE_COMMAND_UI(ID_SITE_CHG_DIR_UP, OnUpdateSiteChgDirUp)
	ON_UPDATE_COMMAND_UI(ID_SITE_CONNECT, OnUpdateConnect)
	ON_UPDATE_COMMAND_UI(ID_SITE_DISCONNECT, OnUpdateDisconnect)
	ON_UPDATE_COMMAND_UI(ID_TRANSFERS_DOWNLOAD, OnUpdateDownload)
	ON_UPDATE_COMMAND_UI(ID_TRANSFERS_UPLOAD, OnUpdateUpload)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	ON_COMMAND(ID_TRANSFERS_LINK, OnTransfersLink)
	ON_UPDATE_COMMAND_UI(ID_TRANSFERS_LINK, OnUpdateTransfersLink)
	ON_COMMAND(ID_CANCEL_CALL, OnCancelCall)
	ON_COMMAND(ID_FTP_VIEW_PROPERTIES, OnViewFtpProperties)
	ON_COMMAND(ID_SITE_CHG_USER_DIR, OnSiteChgUserDir)
	ON_COMMAND(ID_SITE_DOWNLOAD_FILE, OnDownloadFile)
	ON_COMMAND(ID_SITE_CHG_DIR, OnChangeRemoteDirectory)
	ON_COMMAND(ID_SITE_CHG_DIR_UP, OnChangeDirectoryUp)
	ON_COMMAND(ID_SITE_CONNECT, OnConnect)
	ON_COMMAND(ID_SITE_DISCONNECT, OnDisconnect)
	ON_COMMAND(ID_TRANSFERS_DOWNLOAD, OnGet)
	ON_COMMAND(ID_TRANSFERS_UPLOAD, OnPut)
	ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SOCKET_RESPONSE, OnControlSocket)
	ON_MESSAGE(WM_SOCKET_LISTEN, OnListeningSocket)
	ON_MESSAGE(WM_SOCKET_DATA, OnDataSocket)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

static UINT BASED_CODE ftpButtons[] = 
{
	ID_NEW_TEXT,
	ID_FILE_OPEN,
	ID_FILE_SAVE, 
		ID_SEPARATOR,
	ID_FILE_NEW,
	ID_PLACES_PREFERENCES,
	ID_PLACES_HELPINFO,
		ID_SEPARATOR,
	ID_SITE_CHG_DIR_UP,
		ID_SEPARATOR,
	ID_TRANSFERS_DOWNLOAD,
	ID_SITE_CHG_DIR,
	ID_TRANSFERS_LINK,
		ID_SEPARATOR,
	ID_FILE_PRINT_SETUP,
		ID_SEPARATOR,
	ID_HELP_INDEX,
	ID_CONTEXT_HELP,
		ID_SEPARATOR,
	ID_TIP_OF_DAY,
};

/////////////////////////////////////////////////////////////////////////////
// CFtpView construction/destruction

CFtpView::CFtpView()
	: CFormView(CFtpView::IDD)
{
	//{{AFX_DATA_INIT(CFtpView)
	m_pDoc = NULL;
	m_strSelection = "";
	//}}AFX_DATA_INIT   
	m_strFileName = m_strFilePath = m_strDirectory = m_strFileExt = "";
	
	m_pControlSocket = NULL;
	m_pListeningSocket = NULL;
	m_pDataSocket = NULL;  
	
	m_pUserFile = NULL;
	m_bMarkDirty = FALSE;
	m_bEnableUp = FALSE; 
	
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_nFileSize = m_nBytes = 0;               
	m_pPercentDialog = NULL; 

	m_bConnected = FALSE;
	m_bNoWelcome = TRUE;
	m_bAppClosing = FALSE;
}

CFtpView::~CFtpView()
{
	if (m_bConnected)
	{
		m_bAppClosing = TRUE;
		OnDisconnect();
	}
	
	// If necessary, destroy the percent gauge dialog box     
	if (m_pPercentDialog && IsWindow(m_pPercentDialog->GetSafeHwnd()))
	{
		m_pPercentDialog->ShowWindow(SW_HIDE);
		m_pPercentDialog->UpdateWindow();
		
		m_pPercentDialog->DestroyWindow();
	}
}

void CFtpView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpView)
	DDX_Control(pDX, IDC_SITE_NAME, m_ctlSiteName);
	DDX_Control(pDX, IDC_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_DISCONNECT, m_btnDisconnect);
	DDX_Control(pDX, IDC_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_PUT, m_btnPut);
	DDX_Control(pDX, IDC_GET, m_btnGet);
	DDX_Control(pDX, IDC_FILES, m_ctlFiles);
	DDX_Control(pDX, IDC_CHANGE_REMOTE_DIRECTORY, m_btnChgDir);
	DDX_Control(pDX, IDC_CHANGE_DIRECTORY_UP, m_btnUp);
	DDX_LBString(pDX, IDC_FILES, m_strSelection);
	DDX_Text(pDX, IDC_SITE_NAME, m_pDoc->m_strSiteName);
	DDV_MaxChars(pDX, m_pDoc->m_strSiteName, 255);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		m_pDoc->UpdateAllViews(this, 0L, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CFtpView update helper functions

void CFtpView::OnInitialUpdate()
{
	// Fill in the pointer which we have to the document
	// so we can access the document's data in order to change and 
	// display it      
	m_pDoc = GetDocument();  

 	// call base class OnInitialUpdate so we get correct initialization
	CFormView::OnInitialUpdate();
    
   if (!m_pDoc->GetSiteName().IsEmpty())
    	m_pDoc->SetTitle(m_pDoc->m_strPathTitle + ": " 
    		+ m_pDoc->GetSiteName());
    else
    	m_pDoc->SetTitle(m_pDoc->m_strPathTitle);
    	
	// Subclass this view's controls with CTL3D.DLL to make it 
	// look nice
	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);   
	
	// Disable all buttons on this form except Connect and Help and Upload
	m_btnUp.EnableWindow(FALSE);
	m_btnChgDir.EnableWindow(FALSE);
	m_btnConnect.EnableWindow(TRUE);
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnGet.EnableWindow(FALSE);
	m_btnPut.EnableWindow(TRUE);
	m_btnHelp.EnableWindow(TRUE);    
	
	m_pDoc->UpdateAllViews(NULL, 0L, NULL);
    
    // If the user used the Open FTP Site dialog box to specify the address
    // of a site to connect to, then connect to that site!
    if (m_pDoc->m_bFTPAutoConnect)
    {
    	// Connect to the FTP site for the user
    	/*wait.Restore();*/
    	m_pStatusBar->SetText("Connecting to FTP site...  Please wait.");
    	OnConnect();                                                     
    }
    	
	return;
}

void CFtpView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// the document is telling us its data was changed by the user
	// we can display the new values of the data on the screen with
	// CWnd::UpdateData(bSave=FALSE);
	
	if (!UpdateData(FALSE))
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFtpView communications

LRESULT CFtpView::OnControlSocket(WPARAM wAmount, LPARAM lBuffer)
{ 
	if ((int)wAmount > 0) // for replies from socket
	{
		// we don't expect any big deal replies from the control socket,
		// so we will ignore them
	}
	else
	{
		// If wAmount < 0 it is a receive command
		CString strMsg = "", strCmd = "";
		switch ((SocketReceiveCmd)wAmount)
		{
			case SocketStatusChanged:
				switch (m_pControlSocket->GetStatus())
				{        
						case CONNECTING:
							break;
							
						case CONNECTED:  
						m_bConnected = TRUE;
						// switch to getline mode
												
						m_pControlSocket->SetReceiveTarget(NULL, 0);
						HandleResponse();
						
						if (!GetUserIDAndPassword())
						{
							delete m_pControlSocket;
							m_pControlSocket = NULL;
							
							m_btnConnect.EnableWindow(TRUE);
							m_btnDisconnect.EnableWindow(FALSE);
							m_pStatusBar->SetText("Disconnected from FTP site.");
							return 0;
						}

						m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
												
						strCmd = "USER " + m_pDoc->GetUserID() + "\r\n";
						m_pControlSocket->Send(strCmd);
						
						#ifdef _DEBUG
							ReportFTPCommand("USER");
						#endif
						
						if (!HandleResponse())
						{ 
							delete m_pControlSocket;
							m_pControlSocket = NULL;
							m_btnConnect.EnableWindow(TRUE);
							m_btnDisconnect.EnableWindow(FALSE);
							m_pStatusBar->SetText("Disconnected from FTP site.");
							return 0;
						} 

						m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
						/*wait.Restore();*/
						
						strCmd = "PASS " + m_pDoc->GetPassword() + "\r\n";
						m_pControlSocket->Send(strCmd);
						
						#ifdef _DEBUG
							ReportFTPCommand("PASS");
						#endif

						// The FTP server, its requests for info satisfied,
						// probably has a big welcome message it wants
						// the user to see, so show the user the message
						// in a dialog box using HandleWelcomeMsg()
						strMsg = "Connected to ";
						strMsg += m_pDoc->GetSiteName();
						strMsg += ".  For Help, press F1.";
						m_pStatusBar->SetText(strMsg);

						if (!HandleWelcomeMsg()) 
						{  
							m_pStatusBar->ShowIdleMessage();
							
							State = ReceivingWelcome;
							OnDisconnect();
							return 0; // get out of this message
						}

						/*UpdateData(FALSE); */
						m_btnUp.EnableWindow(FALSE);
						
						m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
						
						if (m_pDoc->GetSiteName() == "members.aol.com" ||
							m_pDoc->GetSiteName() == "users.aol.com" ||
							m_pDoc->GetSiteName() == "members.gnn.com")
						{
							CAOLDirDlg aolDir(AfxGetMainWnd()); 
							aolDir.m_strDirectory = "/"; // default is root
							
							if (aolDir.DoModal() == IDOK)
							{
								m_pStatusBar->ShowIdleMessage();
								m_strDirectory = aolDir.m_strDirectory;
								if (!m_strDirectory.IsEmpty())
								{ 
									strMsg = "Changing directories on ";
									strMsg += m_pDoc->GetSiteName();
									strMsg += " to ";
									strMsg += m_strDirectory + "... ";
									strMsg += " Please wait.";
									m_pStatusBar->SetText(strMsg);
									strMsg.Empty();
									
								    strCmd = "CWD ";
								    strCmd += m_strDirectory;
								    strCmd += "\r\n";
								    m_pControlSocket->Send(strCmd);
								    
								    #ifdef _DEBUG
								    	ReportFTPCommand("CWD");
								    #endif
								    
								    strCmd.Empty();
									if (!HandleResponse())
									{
										m_pStatusBar->ShowIdleMessage();
										m_pDoc->m_bEmergencyClose = TRUE;
										OnDisconnect();
										break;
									}
									
									// Ask the server for its working
									// directory so we can inform the
									// user
									m_pControlSocket->Send("PWD\r\n");
									
									if (!HandleResponse())
									{
										m_pStatusBar->ShowIdleMessage();
										m_pDoc->m_bEmergencyClose = TRUE;
										OnDisconnect();
										break;
									}
									
									m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
									ListFiles();
									break;								    
								}
							}
						}

						m_pStatusBar->SetText(IDS_REQUESTING_DIRECTORY);
						
						m_pControlSocket->Send("PWD\r\n");
						if (!HandleResponse())
						{ 
							MessageBeep(-1);
							AfxMessageBox(IDP_ERROR_DISCONNECT, MB_ICONSTOP);
							
							m_pDoc->m_bEmergencyClose = TRUE;
							OnDisconnect();
							break;
						}

                        m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
						ListFiles();
						break; 
						
					case ERRORSTATE: // socket error
						m_pStatusBar->ShowIdleMessage();
						// serious socket error occurred; disconnect
						AfxMessageBox(IDP_ERROR_DISCONNECT, 
										MB_ICONSTOP|MB_OK);
						
						m_pDoc->m_bEmergencyClose = TRUE;
						SendMessage(WM_COMMAND, ID_SITE_DISCONNECT);
						break;
					
					case TIMEDOUT:
						m_pStatusBar->ShowIdleMessage();
						break;
							
				}
				break;
		}
	}	
	return 0;
}

LRESULT CFtpView::OnListeningSocket(WPARAM wAmount, LPARAM lBuffer)
{ 
	if ((int)wAmount > 0)
	{
		// we don't expect any big deal replies from the listening socket,
		// so we will ignore them     
	}
	else                               
	{
		// If wAmount < 0 it is a receive command.
		switch ((SocketReceiveCmd)wAmount)
		{
			case SocketStatusChanged:
				break;
			case NewSocketAccepted:
				// The only function of this listening
				// socket is to tell you where to find the
				// data socket that the server connected to.
				// After that, you work with the data socket
				m_pDataSocket = (QSocket*)lBuffer; 
				m_pDataSocket->SetReceiveTarget(this,
					WM_SOCKET_DATA); 

				// Force this socket to receive notification of its
				// CONNECTED status
				m_pDataSocket->NotifyStatus(); 
				
				delete m_pListeningSocket;
				m_pListeningSocket = NULL;
				break;
		}
	}
	return 0;                            
}

LRESULT CFtpView::OnDataSocket(WPARAM wAmount, LPARAM lBuffer)
{   
	CString strItem = "";
	CString strMsg = "", strCmd = "";  

	if ((int)wAmount > 0)
	{
		if (State == ReceivingList) // receiving a list of files
		{
			m_pStatusBar->SetText(IDS_RECEIVING_LIST);
			/*wait.Restore();*/

			unsigned int nLeftoverLength = m_strLeftovers.GetLength();
			char* pszFullBuffer = new char[wAmount+nLeftoverLength+1];
			
			strcpy(pszFullBuffer,m_strLeftovers);
			strcat(pszFullBuffer, (char*) lBuffer); 
			
			char* pszCur = pszFullBuffer;
			char* pszCRLF = strstr(pszCur, "\r\n"); //location of the CR LF pair
			while (pszCRLF)
			{                             
				*pszCRLF = 0;
				#ifdef _DEBUG
					afxDump << pszCur << "\r\n";
				#endif   
				
				// Add the string to this list box, after doing any special
				// processing 
				strItem = pszCur;
				if (OnAddToFilesList(strItem))
					m_ctlFiles.AddString(strItem);//will go from pszCur to the null termination we just put in
				pszCur = pszCRLF + 2;         //skip over the null
				pszCRLF = strstr(pszCur, "\r\n");   // search again
			}
			m_strLeftovers = pszCur;
			
			delete[] pszFullBuffer; // clean-up memory 
			
			m_pStatusBar->ShowIdleMessage();
		}
		
		if (State == ReceivingFile)
			DoDownload((char*)lBuffer, (ULONG)wAmount);
	}
	else
	{ 
	    /*wait.Restore();*/
		// If wAmount < 0 it is a receive command
		switch ((SocketReceiveCmd)wAmount)
		{
			case SocketStatusChanged:
			{
				switch(m_pDataSocket->GetStatus())
				{
					case CONNECTING:
						if (State == SendingFile)
							m_pStatusBar->SetText(IDS_WAITING_DATA_CONNECTION);
						break;	
									
					case CONNECTED:		// This is important; we may be uploading
						if (State == SendingFile)
							DoUpload();
						break;
				 
					case DISCONNECTED: 
						HandleResponse();
						if (State == ReceivingList)	// 226 Transfer Complete
						{
							OnEndList();
							m_pStatusBar->ShowIdleMessage();			
						}
						else if (State == ReceivingFile) 
						{						
							OnEndDownload();
							m_pStatusBar->ShowIdleMessage();
						}
						m_btnDisconnect.EnableWindow(TRUE);
						m_btnPut.EnableWindow(TRUE);	
						State = Idle;
						break;	
				}	
			}				
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CFtpView upload and download

void CFtpView::DoDownload(char* pszBuffer, unsigned long int ulBuffer)
{
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
	if (m_nFileType == TYPE_BINARY)
	{
		// If the file we're downloading is a binary data file,
		// then we can write it to the disk as is 
		TRY
		{
			m_pUserFile->Write((char*)pszBuffer, ulBuffer);				
		}
		CATCH(CFileException, pFileException)
		{
			EmitFileException(pFileException);
					
			// Cancel this download
			PostMessage(WM_COMMAND, ID_CANCEL_CALL, 0L);
			return;
		}
		END_CATCH
	
		// Calculate percentage of file downloaded
		// and display it in the percent-gauge dialog box

		// NOTE: The below technique has been perfected and it
		// WORKS!
				
		m_nBytes = (ULONG)m_pUserFile->GetLength();
					
		m_pPercentDialog->CalculatePercent(m_nBytes, 
						m_nFileSize);
	}
	else if (m_nFileType == TYPE_ASCII)
	{
		// we're getting an ASCII text file; strip out any
		// carriage-return ('\r') characters so Write(), when
		// expanding CR-LF pairs, doesn't mess up the file 
				
		char* pszCur = pszBuffer; // holds our place in lBuffer
        char* pszCR = strchr(pszCur, '\r'); // finds a carriage 
											// return
		while(pszCR)
		{
			m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
			*pszCR++ = 0;   
					
			TRY
			{
				m_pUserFile->Write((char*)pszCur, strlen(pszCur));
			}
			CATCH(CFileException, pFileException)
			{
				EmitFileException(pFileException);
						
				// Cancel this download
				PostMessage(WM_COMMAND, ID_CANCEL_CALL, 0L);
				return;
			}
			END_CATCH
				
			// Calculate percentage of file downloaded
			// and display it in the percent-gauge dialog box

			m_nBytes = (ULONG)m_pUserFile->GetLength();
						
			m_pPercentDialog->CalculatePercent(m_nBytes, 
							m_nFileSize);

			// Will go from pszCur to the NULL we put in the
			// lBuffer.
			pszCur = pszCR;					// Skip over the NULL.
			pszCR = strchr(pszCur, '\r');	// Search again.
		}
				 
		m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
				
		// Write any leftovers to the file, and calculate the
		// percentage downloaded
		TRY
		{
			m_pUserFile->Write((char*)pszCur, strlen(pszCur));
		}
		CATCH(CFileException, pFileException)
		{
			EmitFileException(pFileException);
					
			// Cancel this download
			PostMessage(WM_COMMAND, ID_CANCEL_CALL, 0L);
			return;
		}
		END_CATCH
				
		// Calculate percentage of file downloaded
		// and display it in the percent-gauge dialog box

		m_nBytes = (ULONG)m_pUserFile->GetLength();
						
		m_pPercentDialog->CalculatePercent(m_nBytes, 
						m_nFileSize);
	}
}

void CFtpView::DoUpload()
{    
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
	// The upload process is different then downloading.
	// Here, for simplicity, we read and feed the server 1K chunks of file.
	
	const UINT nPacketSize = 1024;	// 1K is our size of chunks	
	UINT nBytesRead = 0;			// 0 bytes read from the file so far
	m_nBytes = 0;					// 0 bytes sent so far 
	
	// Allocate the lBuffer which is going to be our 'byte bucket'
	m_pStatusBar->SetText("Allocating buffers...  Please wait.");
	char* pszBuffer = new char[nPacketSize];

	// If we're this far, start reading and sending
	           
	// Start the ball rolling
	m_pStatusBar->SetText("Reading data from file...  Please wait.");
	TRY
	{
		nBytesRead = m_pUserFile->Read((char*)pszBuffer, nPacketSize);
	}                                                     
	CATCH(CFileException, pFileException)
	{  
		m_pPercentDialog->DestroyWindow();
		EmitFileException(pFileException);
		
		State = Idle;
		
		// Remember to remove our buffer from memory!
		delete[] pszBuffer;   
		pszBuffer = NULL;
		return;
					
		m_pControlSocket->Send("ABOR\r\n");
		
		delete m_pDataSocket;
		m_pDataSocket = NULL;  
		
		HandleResponse();
		return;
	}
	END_CATCH		
	
	m_nBytes = nBytesRead;

	// Now use SendRaw() on the data socket to blast the
	// chunk of raw file data out into cyberspace          
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
	m_pDataSocket->SendRaw((char*)pszBuffer, nBytesRead);
	
	m_pPercentDialog->CalculatePercent(m_nBytes, m_nFileSize);
	
	if (nBytesRead < nPacketSize && m_nBytes == m_nFileSize)
	{
		// Remember to remove our buffer from memory!
		delete[] pszBuffer;
		pszBuffer = NULL;
		
		// End this upload
		OnEndUpload();
		return;
	}
	
	
	// If we're still here, there's more file to send
	while (nBytesRead == nPacketSize && m_nBytes < m_nFileSize)
	{
		// Read the next 1K chunk of file, then position the file pointer
		// to skip over the read section so we can read more file 
		m_pStatusBar->SetText("Reading data from file...  Please wait.");
		TRY
		{
			nBytesRead = m_pUserFile->Read((char*)pszBuffer, nPacketSize);		
		}
		CATCH(CFileException, pFileException)
		{  
			m_pPercentDialog->DestroyWindow();
			EmitFileException(pFileException);
			
			State = Idle;
			
			// Remember to remove our buffer from memory!
			delete[] pszBuffer;   
			pszBuffer = NULL;
			return;
						
			m_pControlSocket->Send("ABOR\r\n");
			
			delete m_pDataSocket;
			m_pDataSocket = NULL;  
			
			HandleResponse();
			return;
		}
		END_CATCH
		
		m_nBytes += nBytesRead; 
			
		// Now use SendRaw() on the data socket to blast the
		// chunk of raw file data out into cyberspace          
		m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
		m_pDataSocket->SendRaw((char*)pszBuffer, nBytesRead);
		
		m_pPercentDialog->CalculatePercent(m_nBytes, m_nFileSize);
		
	}
	
	delete[] pszBuffer;
	pszBuffer = NULL;
	
	// Cleanup - end transfer by deleting and closing the data connection
	OnEndUpload();
	m_pStatusBar->ShowIdleMessage();
	
	return;

}
			
/////////////////////////////////////////////////////////////////////////////
// CFtpView file-processing helpers

BOOL CFtpView::IsProgramFile()
{
	if (m_strFileExt.IsEmpty())
	{
		// We can't tell whether or not the file is a program file, so return
		// FALSE
		return FALSE;
	}
	
	if ((m_strFileExt.CompareNoCase("EXE") == 0)
		|| (m_strFileExt.CompareNoCase("BAT") == 0)
		|| (m_strFileExt.CompareNoCase("COM") == 0)
		|| (m_strFileExt.CompareNoCase("PIF") == 0))
	{
		// We have a program file!
		return TRUE;
	}
	
	// If we made it this far, the file has some other extension
	return FALSE;	
}                

BOOL CFtpView::IsHelpFile()
{
	if (m_strFileExt.IsEmpty())
	{
		// We can't tell whether or not the file is a help file, so return
		// FALSE
		return FALSE;
	}
	
	if ((m_strFileExt.CompareNoCase("HLP")) == 0)
	{
		// We have a Help file!
		return TRUE;
	}
	
	// If we made it this far, the file has some other extension
	return FALSE;	
}
	
/////////////////////////////////////////////////////////////////////////////
// CFtpView diagnostics

#ifdef _DEBUG
void CFtpView::AssertValid() const
{
	CFormView::AssertValid();
	return;
}

void CFtpView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);      
	
	dc << "Site Name: " << m_pDoc->GetSiteName() << "\n";
	dc << "UserID: " << m_pDoc->GetUserID() << "\n";
	dc << "Password: " << m_pDoc->GetPassword() << "\n";
	
	dc << "File Type: " << m_nFileType << "\n";
	dc << "File Name: " << m_strFileName << "\n";
	dc << "File Path: " << m_strFilePath << "\n";
	dc << "File Ext.: " << m_strFileExt << "\n";
	
	int nDepth = dc.GetDepth();
	dc.SetDepth(10);
	
	if (m_pControlSocket != NULL) dc << "Control Socket:\n" << m_pControlSocket << "\n";
	if (m_pListeningSocket != NULL) dc << "Listening Socket:\n" << m_pListeningSocket << "\n";
	if (m_pDataSocket != NULL) dc << "Data Socket:\n" << m_pDataSocket << "\n";	
	
	dc.SetDepth(nDepth);
	return;
}

CFtpDoc* CFtpView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFtpDoc)));
	return (CFtpDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFtpView message handlers

void CFtpView::OnDblclkFiles()
{
	m_bMarkDirty = FALSE;
	UpdateData(TRUE);  
	
	m_nSelection = m_ctlFiles.GetCurSel();
	
	CString dest = "", link = ""; // destination of a FTP symbolic link
	int nLinkStart; // start of the symbolic link label
	
	int nMaxIndex;
	
	char type = m_strSelection[0];
	
	switch (type)
	{
		case 'd':
			OnChangeRemoteDirectory(); // change to double-clicked directory
			break;
		
		case '-':
			OnGet(); // download double-clicked filename
			break;
		
		case 'l':  // change directories to link destination  or download
						// file at link destination
			link = m_strSelection.Right(m_strSelection.GetLength() -
								m_strSelection.ReverseFind(' ') - 1);
								
			// Lookup dest using our link-dest map
			if (!m_ftpLinkDestMap.Lookup(link, dest))
			{
				MessageBeep(-1);
				AfxMessageBox("Unable to determine symbolic link destination.");
				break;
			}
			
			nMaxIndex = dest.GetLength() - 1;
			
			if (dest[nMaxIndex - 2] == '.') // file 
			{         
				m_strDirectory.Empty();
				m_strFileName = dest;
                
                // get file type from extension
				nLinkStart = dest.ReverseFind('.');
				m_strFileExt = dest.Right(dest.GetLength()
							- nLinkStart - 1);
								
				OnGet(); // download the link's destination file                             
				m_strFileName.Empty();                 
			}
			else if (dest[nMaxIndex - 3] == '.') // file
			{
				m_strDirectory.Empty();
				m_strFileName = dest;
                
                // get file type from extension
				nLinkStart = dest.ReverseFind('.');
				m_strFileExt = dest.Right(dest.GetLength()
							- nLinkStart - 1);
								
				OnGet();                              
				m_strFileName.Empty();     			
			}
			else if ((dest.Left(2) == "..")	&& ((dest[nMaxIndex - 2] != '.')
						|| (dest[nMaxIndex - 3] != '.')))
			{
				m_strFileName.Empty();
				m_pControlSocket->Send("CWD " + dest + "\r\n");
				
				#ifdef _DEBUG
					ReportFTPCommand("CWD");
				#endif
				
				HandleResponse();     
				
				m_pControlSocket->Send("PWD\r\n");
				
				#ifdef _DEBUG
					ReportFTPCommand("PWD");
				#endif
				
				HandleResponse();
				
				m_ctlFiles.ResetContent(); 
				m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
				ListFiles();
			}
			else if (dest.Find('.') == -1)	// directory
			{
				m_strFileName.Empty();
				m_pControlSocket->Send("CWD " + dest + "\r\n");
				
				#ifdef _DEBUG
					ReportFTPCommand("CWD");
				#endif
				
				HandleResponse();
				
				m_pControlSocket->Send("PWD\r\n");
				
				#ifdef _DEBUG
					ReportFTPCommand("PWD");
				#endif
				
				HandleResponse();
				
				m_ctlFiles.ResetContent();
				m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
				ListFiles();
			}
			else if (dest.Find('.') != -1)	// file
			{
				m_strDirectory.Empty();
				m_strFileName = dest;
                
                // get file type from extension
				nLinkStart = dest.ReverseFind('.');
				m_strFileExt = dest.Right(dest.GetLength()
							- nLinkStart - 1);
								
				OnGet(); // download the link's destination file                             
				m_strFileName.Empty();            
			}							
			break;
			
		default:
			AfxMessageBox("Unable to determine selection type.", 
					MB_ICONSTOP|MB_OK);
			break;
	}
	return;
}

void CFtpView::OnSelchangeFiles()
{
	m_bMarkDirty = FALSE; // don't mark document as dirty; the user
							// hasn't changed its data   
	
	m_btnPut.EnableWindow(TRUE);
	
	if (!UpdateData(TRUE))
		return;
	
	m_nSelection = m_ctlFiles.GetCurSel();
	
	if (m_strSelection.Left(1) == "d")  // directory
	{
		m_btnGet.EnableWindow(FALSE);
		m_btnChgDir.EnableWindow(TRUE);
		m_btnPut.EnableWindow(TRUE);
		m_btnUp.EnableWindow(!m_strPWDirectory.IsEmpty() &&
					m_strPWDirectory != "/");
		return;
	}
	else if (m_strSelection.Left(1) == "-")	// file'
	{	
		m_btnGet.EnableWindow(TRUE);
		m_btnChgDir.EnableWindow(FALSE);
		m_btnPut.EnableWindow(TRUE);
		m_btnUp.EnableWindow(!m_strPWDirectory.IsEmpty() &&
					m_strPWDirectory != "/");
		return;
	}
	else if (m_strSelection.Left(1) == "l")	// symbolic link
	{
		m_btnGet.EnableWindow(FALSE);
		m_btnChgDir.EnableWindow(FALSE);
		m_btnPut.EnableWindow(TRUE);
		m_btnUp.EnableWindow(!m_strPWDirectory.IsEmpty() &&
					m_strPWDirectory != "/");
		return;                     
	}
	else      
	{
		m_btnGet.EnableWindow(FALSE);
		m_btnChgDir.EnableWindow(FALSE);
		m_btnPut.EnableWindow(TRUE);
		m_btnUp.EnableWindow(!m_strPWDirectory.IsEmpty() &&
					m_strPWDirectory != "/");
		return;                     
	}
}

void CFtpView::OnClickedHelp()
{ 
	OnHelp(); 
	return;	
}

void CFtpView::OnEditChange()
{
	return;
}

void CFtpView::OnConnect()
{
	m_btnConnect.EnableWindow(FALSE);
	m_btnDisconnect.EnableWindow(TRUE); 
	
	CString strMsg = "";
	// We want to set up the control socket and use it to connect
	// to the specified FTP site.
	
	delete m_pControlSocket; // gets rid of any existing socket
	m_pControlSocket = new QSocket(TRUE); 
	m_pControlSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);

	// Get the host name the user specified from the edit box.
	// We use CWnd::UpdateData(bSave=TRUE)
	if (!UpdateData(TRUE))
	{
		AfxMessageBox("Unable to retrieve server name.",
			MB_ICONSTOP|MB_OK);
		
		delete m_pControlSocket;
		m_pControlSocket = NULL;  
		
		return;
	}
		
	// Make sure the files box doesn't have anything in it (we're not 
	// connected yet)
	m_ctlFiles.ResetContent();
	UpdateData(FALSE);
	/*wait.Restore();*/
	
	m_pDoc->SetTitle(m_pDoc->m_strPathTitle + ": " + m_pDoc->GetSiteName());
	strMsg = "Connecting to " + m_pDoc->GetSiteName() + "...  Please wait.";
	m_pStatusBar->SetText(strMsg);
	
	if (!m_pControlSocket->Connect(m_pDoc->GetSiteName(), 21))
	{
		/*wait.Restore();*/
		strMsg = "Unable to connect to " + m_pDoc->GetSiteName()  + ".";
		m_pStatusBar->SetText(strMsg);
		// Control connection failed
		MessageBeep(-1);
		AfxMessageBox(IDP_NOCONNECT_HOST);

		m_btnConnect.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(FALSE);
		
		SendMessage(WM_COMMAND, ID_SITE_DISCONNECT);
		m_pStatusBar->ShowIdleMessage();
		return;
	} 
	
	m_bConnected = TRUE;
	/*wait.Restore();*/
	// for the rest of the code where we deal with control socket
	// communications, see CFtpView::OnControlSocket().
		
	return;
}

void CFtpView::OnDisconnect()
{   
	CString strSiteName = m_pDoc->GetSiteName();
	CString strMsg = "";    
	
	m_bConnected = FALSE;
	
	if (State == ReceivingWelcome) // OK! That's it! That's the straw!
   	{
		// Disable all buttons on this form except Connect and Help
		m_btnUp.EnableWindow(FALSE);
		m_btnChgDir.EnableWindow(FALSE);
		m_btnConnect.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(FALSE);
		m_btnGet.EnableWindow(FALSE);
		m_btnPut.EnableWindow(TRUE);       
		m_btnHelp.EnableWindow(TRUE);		
		
		m_pDoc->SetTitle("FTP Browser");
		m_pDoc->m_bEmergencyClose = TRUE;
		
		m_pDoc->SaveModified();
		
		m_pDoc->GetSiteName().Empty();
		m_pDoc->GetUserID().Empty();
		m_pDoc->GetPassword().Empty();
		m_pDoc->SetModifiedFlag(FALSE);
		
		m_bConnected = FALSE;		
		
		m_pControlSocket->Disconnect();
   		
   		delete m_pControlSocket;
   		m_pControlSocket = NULL;
   		
		AfxFormatString1(strMsg, IDS_DISCONNECTED_TEXT, strSiteName);
		if (!m_bAppClosing)
		{		
			CFtpDisconnected theDialog(m_pApp->GetMainFrame());
			theDialog.m_strSiteName = strSiteName;
			theDialog.m_strMsg = strMsg;
			
			MessageBeep(-1);
			theDialog.DoModal();
		} 

		// TODO: Show the Open FTP Site dialog box here, if the user
		// so chooses in Preferences		
		
		m_bConnected = FALSE;
		return;
	}
   	
	// Clear the files list box.
    m_ctlFiles.ResetContent();
    m_ctlFiles.UpdateWindow();

    // Clear the document's data, but let's let the user have a chance
    // to save it first
	if (!m_pDoc->SaveModified())
		return;		// user doesn't want to disconnect
    
    m_pDoc->GetUserID().Empty();
    m_pDoc->GetPassword().Empty(); 
    m_pDoc->GetSiteName().Empty();
    m_pDoc->SetModifiedFlag(FALSE);
    
    if (!m_bAppClosing)
   		m_pDoc->UpdateAllViews(NULL);
    
    m_pDoc->SetTitle("FTP Browser");
    
    m_bConnected = FALSE;  
    
	// Disable all buttons on this form except Connect and Help
	m_btnUp.EnableWindow(FALSE);
	m_btnChgDir.EnableWindow(FALSE);
	m_btnConnect.EnableWindow(TRUE);
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnGet.EnableWindow(FALSE);
	m_btnPut.EnableWindow(FALSE);
	m_btnHelp.EnableWindow(TRUE);

	// Wipe out any ongoing work  
	if (m_pControlSocket)
	{
		delete m_pControlSocket;
		m_pControlSocket = NULL;
	}
	if (m_pListeningSocket)
	{
		delete m_pListeningSocket;
		m_pListeningSocket = NULL;
	}
	if (m_pDataSocket)
	{
		delete m_pDataSocket;
		m_pDataSocket = NULL;
	}
	if (m_pUserFile)
	{
		delete m_pUserFile;
		m_pUserFile = NULL; 
	}

	AfxFormatString1(strMsg, IDS_DISCONNECTED_TEXT, strSiteName);
	
	if (!m_bAppClosing)
	{
		CFtpDisconnected theDialog(m_pApp->GetMainFrame());
		theDialog.m_strSiteName = strSiteName;
		theDialog.m_strMsg = strMsg;
	
		MessageBeep(-1);
		theDialog.DoModal();
		m_pStatusBar->ShowIdleMessage();
	} 

	// TODO: Show the Open FTP Site dialog box here, if the user
	// so chooses in Preferences
	
	m_bConnected = FALSE;		
	return;
}

void CFtpView::OnGet()
{      
	CFileException e;

	CString str = "Internet Navigator attempts to offer you some protection";
	str += " against Internet viruses by allowing you to run a virus scan ";
	str += "program before opening a file you have downloaded with FTP. ";
	str += " WnDBSoft Software International will not be held ";
	str += "liable for damage caused to your system by viruses.  ";
	str += "Remember, Internet Navigator does not force you to download ";
	str += "any file from the Internet; you have the freedom to choose ";
	str += "which files are sent to your computer on the Internet.";      
	
	MessageBeep(-1);
	MessageBox(str, "Disclaimer", MB_ICONASTERISK);
		
	if (!m_strDLFileName.IsEmpty())
		m_strDLFileName.Empty();
	
	m_nBytes = m_nFileSize = 0;

	m_btnGet.EnableWindow(FALSE);
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnUp.EnableWindow(FALSE);
	m_btnChgDir.EnableWindow(FALSE);
	m_btnPut.EnableWindow(FALSE);
	m_bMarkDirty = FALSE;         
	
	m_nSelection = m_ctlFiles.GetCurSel();
	
	UpdateData(TRUE);            

	m_pStatusBar->SetText("Resolving filename...  Please wait.");
	if (m_strSelection[0] != 'l' && m_strFileName.IsEmpty())
	{	
		// get file name and extension (to determine its type)
		int nNameStart = m_strSelection.ReverseFind(' ');
		m_strFileName = m_strSelection.Right(m_strSelection.GetLength()
							- nNameStart - 1);
					
		m_strDLFileName = m_strFileName;  
		if (((m_strFileName.Right(4).CompareNoCase(".htm")) == 0) ||
				((m_strFileName.Right(5).CompareNoCase(".html")) == 0))
		{
			if (m_strFileName.Find('.') > 8 && m_strFileName.Find('.') != -1)
				m_strFileName = m_strFileName.Left(8) + ".htm";
			else  if (m_strFileName.Find('.') != -1)
				m_strFileName = m_strFileName.Left(m_strFileName.Find('.')) + ".htm";	
		}
		else
		{  
			CString name = "";
			
			if (m_strFileName.Find('.') != -1)
				name = m_strFileName;
			else if (m_strFileName.GetLength() < 8)  
				name = m_strFileName + ".txt";
			else if (m_strFileName.GetLength() >= 8 && m_strFileName.Find('.') == -1)
				name = m_strFileName.Left(8) + ".txt";
			
		
			if (name.Left(1) == ".")
			{
				// do nothing
			}
			else if ((name.Left(6).CompareNoCase("README")) == 0)
			{
				// a readme file -- just download without prompting the user
				m_strFileName = "readme.txt";	
			}
			else if ((name.Left(4).CompareNoCase("READ")) == 0)
			{
				// a readme file -- just download without prompting the user
				m_strFileName = "readme.txt";
			}
			else if ((name.Left(5).CompareNoCase("index")) == 0)
			{
				m_strFileName = "index.txt";
			}
			else if ((name.Left(7).CompareNoCase("00index")) == 0)
			{
				m_strFileName = "index.txt";
			}
			else if ((name.Left(8).CompareNoCase("00readme")) == 0)
			{
				m_strFileName = "readme.txt";
			}
			else
			{
				// take name above (which was initialized with the
				// extensionless filename) and put it in m_strFileName
				m_strFileName = name;
			}
		}				
	}
	else
	{
		// File name already requested
		m_strDLFileName = m_strFileName;
	}
	
	m_pStatusBar->SetText("Resolving file name...  Please wait.");
	// if there is a period as the first character of the file name, then
	// erase the period and change the filename to a valid MS-DOS text
	// file name
	if (m_strFileName[0] == '.')
	{  
		m_strDLFileName = m_strFileName; // so the FTP server gets the right file for us
		m_strFileName.MakeLower(); // convert to lowercase
		
		// erase the period
		m_strFileName = m_strFileName.Right(m_strFileName.GetLength() - 1);
		
		// now take the first 8 characters of the name
		if (m_strFileName.GetLength() > 8)
		{
			m_strFileName = m_strFileName.Left(8);
		}
		
		// now put a 'txt' extension onto the filename
		m_strFileName += ".txt";
		
		// bing! we're done.
	}   

	if (!GetDownloadInfo()) // gets file type and destination path
	{
		m_pStatusBar->ShowIdleMessage();
		
		// user wants to cancel  
		m_strFileName.Empty();
		m_strFilePath.Empty();
		m_strFileExt.Empty();

		m_btnGet.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(TRUE); 
		OnSelchangeFiles();
		return;
	}
	
	/*wait.Restore();*/
	// Get the correct type of file transfer to use from the file extension
	GetFileType();

	if (m_nFileType == TYPE_BINARY) // we are downloading a binary file
	{ 
		TRY
		{                                  
			m_pUserFile = new CFile();
		}
		CATCH(CMemoryException, e)
		{
			AfxMessageBox("There is insufficient memory to open the file "
				"you have requested to download.");
			
			return;
		}
		END_CATCH
		
		if (!m_pUserFile->Open(m_strFilePath, CFile::modeWrite|
				CFile::modeCreate|CFile::typeBinary, &e)) 
		{
			EmitFileException(&e);                
			
			m_pStatusBar->ShowIdleMessage();
			AfxMessageBox("Unable to complete download.");
			
			delete m_pUserFile;
			m_pUserFile = NULL;
			
			m_strFileName.Empty();
			m_strFilePath.Empty();
			m_strFileExt.Empty();
		
			m_btnGet.EnableWindow(TRUE);
			m_btnDisconnect.EnableWindow(TRUE); 
		
			m_ctlFiles.SetCurSel(0);
			OnSelchangeFiles();
			return;
		}
	}  
	else  if (m_nFileType == TYPE_ASCII) // we are downloading a text file
	{  
		TRY
		{
			m_pUserFile = new CStdioFile();
		}
		CATCH(CMemoryException, e)
		{
			AfxMessageBox("There is insufficient memory to open the file "
				"you have requested to download.");
			
			return;
		}
		END_CATCH
		
		if (!m_pUserFile->Open(m_strFilePath, CFile::modeWrite|
				CFile::typeText|CFile::modeCreate, &e))
		{
			EmitFileException(&e);
			
			m_pStatusBar->ShowIdleMessage();
			AfxMessageBox("Unable to complete download.");
			
			delete m_pUserFile;
			m_pUserFile = NULL;
			
			m_strFileName.Empty();
			m_strFilePath.Empty();
			m_strFileExt.Empty();
		
			m_btnGet.EnableWindow(TRUE);
			m_btnDisconnect.EnableWindow(TRUE); 
		
			m_ctlFiles.SetCurSel(0);
			OnSelchangeFiles();
			return;
		}
	}
	
	// Inform the FTP site of our port number and address so it knows
	// where to send the file the user wants.
	
	// Also, this sets up a 'listening socket'; this socket waits for the
	// FTP site to connect, receives information on where the connection
	// took place, and gives us a socket which represents the connection
	// on which the data is delivered by the FTP site.
    
    if (!m_strDLFileName.IsEmpty())
    	m_strFileName = m_strDLFileName;

	if (!OpenDataConnection())
	{     
		m_pStatusBar->ShowIdleMessage();
		AfxMessageBox("Unable to complete download.");
		
		delete m_pUserFile;
		m_pUserFile = NULL;
		
		m_strFileName.Empty();
		m_strFilePath.Empty();
		m_strFileExt.Empty();
		
		m_btnGet.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(TRUE); 
		
		m_ctlFiles.SetCurSel(0);
		OnSelchangeFiles();
		return;
	}
	
	CString strCmd = "";
    m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
    
	if (m_nFileType == TYPE_BINARY)
	{
		strCmd = "TYPE I"; // Binary or Image file type
		strCmd += "\r\n";
		m_pControlSocket->Send(strCmd);   
		HandleResponse();
		
		#ifdef _DEBUG
			ReportFTPCommand("TYPE");
		#endif
	}
	else if (m_nFileType == TYPE_ASCII)
	{
		strCmd = "TYPE A"; // ASCII (Text) file type
		strCmd += "\r\n";         
		m_pControlSocket->Send(strCmd);
		HandleResponse();
		
		#ifdef _DEBUG
			ReportFTPCommand("TYPE");
		#endif
	}

	State = ReceivingFile;     
	
	// now ask the FTP site for the file the user wants 
	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	
	m_pControlSocket->Send("RETR " + m_strFileName + "\r\n");
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
	
	#ifdef _DEBUG
		ReportFTPCommand("RETR");
	#endif
	
	if (!HandleResponse(TRUE))
	{
		m_pStatusBar->ShowIdleMessage();
		AfxMessageBox("Unable to complete download.");
		
		delete m_pUserFile;
		m_pUserFile = NULL;
		
		m_strFileName.Empty();
		m_strFilePath.Empty();
		m_strFileExt.Empty();
		
		m_btnGet.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(TRUE);
		OnSelchangeFiles();
		
		return;
	}	
	
	if (m_nFileSize == 0)
		m_nFileSize = 200;
	
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");	
	
	TRY
	{
		m_pPercentDialog = new CFTPercentDialog(m_pApp->GetMainFrame());
	}
	CATCH(CException, pException)
	{
		m_pStatusBar->ShowIdleMessage();
		AfxMessageBox("Unable to complete download.");
		
		delete m_pUserFile;
		m_pUserFile = NULL;
		
		m_strFileName.Empty();
		m_strFilePath.Empty();
		m_strFileExt.Empty();
		
		m_btnGet.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(TRUE);
		OnSelchangeFiles();
		
		return;
	}
	END_CATCH		

	m_pPercentDialog->Initialize(m_strFilePath, m_strFileName, this, FALSE);  
	m_pPercentDialog->UpdateData(FALSE);
	
	m_pPercentDialog->SetPercent(0);
	
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
	
	return;	
}

void CFtpView::OnPut()
{ 
	CFileException e;
	
	State = SendingFile;  
	
	m_strFileName.Empty();
	m_strFilePath.Empty();
	m_strFileExt.Empty();
	
	// Upload a file to the FTP Site
	m_btnPut.EnableWindow(FALSE);
	m_btnDisconnect.EnableWindow(FALSE);
	
	m_bMarkDirty = FALSE;
	UpdateData(TRUE);
	
	if (!GetUploadInfo()) // user wants to cancel
	{  
		m_btnPut.EnableWindow(TRUE);   
		m_btnDisconnect.EnableWindow(TRUE); 
		OnSelchangeFiles();                                      
		State = Idle;
		return;
	} 
	
	if (m_strFileName.IsEmpty() && m_strFilePath.IsEmpty()
		&& m_strFileExt.IsEmpty())
	{
		// return control to caller
		MessageBeep(-1);
		AfxMessageBox("Unable to complete upload!  Try again later.");
		
		return;
	}
	
	GetFileType(); // determines file type using file extension
	
	if (m_nFileType == TYPE_BINARY) // we're uploading a binary data file
	{
		m_pUserFile = new CFile();	// guaranteed to work
		
		if (!m_pUserFile->Open(m_strFilePath, CFile::modeRead|
							CFile::typeBinary, &e))
		{
			EmitFileException(&e);                
			
			m_pStatusBar->ShowIdleMessage();
			AfxMessageBox("Unable to complete upload.");
			
			delete m_pUserFile;
			m_pUserFile = NULL;
			
			m_strFileName.Empty();
			m_strFilePath.Empty();
			m_strFileExt.Empty();
		
			m_btnPut.EnableWindow(TRUE);
			m_btnDisconnect.EnableWindow(TRUE); 
		
			m_ctlFiles.SetCurSel(0);
			OnSelchangeFiles();
			return;
		}	
	}
	else if (m_nFileType == TYPE_ASCII) // we're uploading ASCII text
	{
		m_pUserFile = new CStdioFile(); // guaranteed to work
		
		if (!m_pUserFile->Open(m_strFilePath, CFile::modeRead|
					CFile::typeText, &e))
		{
			EmitFileException(&e);                
			
			m_pStatusBar->ShowIdleMessage();
			AfxMessageBox("Unable to complete upload.");
			
			delete m_pUserFile;
			m_pUserFile = NULL;
			
			m_strFileName.Empty();
			m_strFilePath.Empty();
			m_strFileExt.Empty();
		
			m_btnPut.EnableWindow(TRUE);
			m_btnDisconnect.EnableWindow(TRUE); 
		
			m_ctlFiles.SetCurSel(0);
			OnSelchangeFiles();
			return;
		}	
	}

	if (!m_pUserFile->GetStatus(m_userStatus))
	{
		e.m_cause = CFileException::badPath;
		
		EmitFileException(&e);                
			
		m_pStatusBar->ShowIdleMessage();
		AfxMessageBox("Unable to complete upload.");
			
		delete m_pUserFile;
		m_pUserFile = NULL;
			
		m_strFileName.Empty();
		m_strFilePath.Empty();
		m_strFileExt.Empty();
		
		m_btnPut.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(TRUE); 
		
		m_ctlFiles.SetCurSel(0);
		OnSelchangeFiles();
		return;
	}
		
	// Inform the FTP site of our port number and address so it knows
	// where to send the file the user wants.

	if (!OpenDataConnection())
	{
		m_pStatusBar->ShowIdleMessage();
		AfxMessageBox("Unable to complete upload.");
		
		delete m_pUserFile;
		m_pUserFile = NULL;
		
		m_strFileName.Empty();
		m_strFilePath.Empty(); 
		m_strFileExt.Empty();
		
		m_btnPut.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(TRUE);
		
		m_ctlFiles.SetCurSel(0);
		OnSelchangeFiles();                
		return;
	}
	
	CString strCmd = "";
	if (m_nFileType == TYPE_BINARY)
	{
		strCmd = "TYPE I";
		strCmd += "\r\n";
		m_pControlSocket->Send(strCmd);
		HandleResponse();        
		
		#ifdef _DEBUG
			ReportFTPCommand("TYPE");
		#endif
	}
	else if (m_nFileType == TYPE_ASCII)
	{  
		strCmd = "TYPE A";
		strCmd += "\r\n";
		m_pControlSocket->Send(strCmd); 
		HandleResponse();
		
		#ifdef _DEBUG
			ReportFTPCommand("TYPE");
		#endif
	}
    
	m_pControlSocket->Send("STOR " + m_strFileName + "\r\n"); 
	
	#ifdef _DEBUG
		ReportFTPCommand("STOR");
	#endif

	if (!HandleResponse())
	{
		m_pStatusBar->ShowIdleMessage();
		AfxMessageBox("Unable to complete upload.");
		
		delete m_pUserFile;
		m_pUserFile = NULL;
		
		m_strFileName.Empty();
		m_strFilePath.Empty(); 
		m_strFileExt.Empty();
		
		m_btnPut.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(TRUE);
		
		m_ctlFiles.SetCurSel(0);
		OnSelchangeFiles();                
		return;
	}  
	
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");
	
	m_nFileSize = (ULONG)m_userStatus.m_size; 
	m_nBytes = 0;
	
	// From now, when the listening socket accepts the data connection
	// from the server and we can start the upload in CFtpView::DoUpload
	
	State = SendingFile;
	
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");	
	m_pPercentDialog = new CFTPercentDialog(m_pApp->GetMainFrame());

	m_pPercentDialog->Initialize(m_strFilePath, m_strFileName, this, TRUE);  
	m_pPercentDialog->UpdateData(FALSE);
	
	m_pPercentDialog->CalculatePercent(0, m_nFileSize);
	
	m_pStatusBar->SetText("Copying " + m_strFileName + "...  Please wait.");	
	return;
}

void CFtpView::OnChangeDirectoryUp()
{
	m_btnDisconnect.EnableWindow(FALSE);
	// Tell the server to move to the parent directory of the current
	// directory
	CString strCmd = "CDUP";
	strCmd += "\r\n";
	m_pControlSocket->Send(strCmd);

	#ifdef _DEBUG
		ReportFTPCommand("CDUP");
	#endif
    
    HandleResponse();
    
	m_pControlSocket->Send("PWD\r\n");
	
	#ifdef _DEBUG
		ReportFTPCommand("PWD");
	#endif //_DEBUG
	
	if (!HandleResponse())
	{
		m_btnDisconnect.EnableWindow(TRUE);
		return;
	}
	
	// Refresh the file list  
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	ListFiles();	
}

void CFtpView::OnChangeRemoteDirectory()
{
	m_btnDisconnect.EnableWindow(FALSE);
    
    CString strSite = m_pDoc->GetSiteName(), strMsg = "";
	UpdateData(TRUE);

	int namestart = m_strSelection.ReverseFind(' ');
	CString strDirectory = m_strSelection.Right(m_strSelection.GetLength()
			 - namestart - 1);
	// don't want the space at position namestart included in directory
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	m_pControlSocket->Send("CWD " + strDirectory + "\r\n"); 
	
	#ifdef _DEBUG
		ReportFTPCommand("CWD");
	#endif
	
	if (!HandleResponse())
	{ 
		// Since the HandleResponse() function received an error from
		// the server that the user was told about, just refresh the files
		// list
		ListFiles(); 
		return;
	}
    
    m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
    m_pControlSocket->Send("PWD\r\n");
    
    #ifdef _DEBUG
    	ReportFTPCommand("PWD");
    #endif
    
    // The HandleResponse() call is non-fatal for the PWD command, so we'll
    // ignore its return value 
    m_pStatusBar->ShowIdleMessage();
    HandleResponse();
    
    m_ctlFiles.ResetContent();

	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	
	ListFiles(); 
	return;
}

void CFtpView::OnFileDelete()
{
	m_btnDisconnect.EnableWindow(FALSE);
    
    CString strSite = m_pDoc->GetSiteName(), strMsg = "";
	UpdateData(TRUE);
	
	if (m_strSelection.Left(1) == 'd')
		return;
	else if (m_strSelection.Left(1) == 'l')
		return;
	else if (m_strSelection.Left(1) != '-')
		return;

	int namestart = m_strSelection.ReverseFind(' ');
	CString strFile = m_strSelection.Right(m_strSelection.GetLength()
			 - namestart - 1);
	// don't want the space at position namestart included in directory
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	m_pControlSocket->Send("DELE " + strFile + "\r\n"); 
	
	#ifdef _DEBUG
		ReportFTPCommand("DELE");
	#endif
	
	if (!HandleResponse())
	{ 
		// Since the HandleResponse() function received an error from
		// the server that the user was told about, just refresh the files
		// list
		ListFiles(); 
		return;
	}
	
	ListFiles();
	return;
}

BOOL CFtpView::OpenDataConnection()
{ 	
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	
	delete m_pListeningSocket;
	
	TRY
	{
		m_pListeningSocket = new QSocket(); 
	}
	CATCH(CMemoryException, e)
	{
		MessageBeep(-1);
		AfxMessageBox("Cannot open data connection.", MB_ICONSTOP);
		
		return FALSE;
	}
	END_CATCH
	
	if (m_pListeningSocket == NULL)
		return FALSE;
		
	m_pListeningSocket->SetReceiveTarget(this, WM_SOCKET_LISTEN);
	SOCKADDR_IN sock_address;           
	SOCKADDR_IN cont_address;
	int addr_size = sizeof(sock_address);
	sock_address.sin_family = AF_INET;
	sock_address.sin_addr.s_addr = htonl(INADDR_ANY);                 
	sock_address.sin_port = 0;
	m_pListeningSocket->Bind((SOCKADDR*)&sock_address, addr_size);
	m_pListeningSocket->Listen();
								
	m_pListeningSocket->GetSockName((SOCKADDR*)&sock_address, &addr_size);
	m_pControlSocket->GetSockName((SOCKADDR*)&cont_address, &addr_size);
	
	unsigned char *port = (unsigned char *)&(sock_address.sin_port);
	unsigned char *host = (unsigned char *)&(cont_address.sin_addr);
	
	char command[512];
	sprintf(command, "PORT %i,%i,%i,%i,%i,%i\r\n", host[0], host[1], 
						host[2], host[3], port[0], port[1]);

	#ifdef _DEBUG // diagnostics
		char dumpInfo[512];
		
		sprintf(dumpInfo, "IP Address: %i.%i.%i.%i on port %i%i.",
			host[0], host[1], host[2], host[3], port[0], port[1]); 

	    afxDump << dumpInfo << "\r\n";
	#endif //_DEBUG
	
	// Send the PORT command	
	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	m_pControlSocket->Send(command);       
	
	// diagnostics
	#ifdef _DEBUG
		ReportFTPCommand("PORT");
	#endif //_DEBUG
	
	return HandleResponse();
}
		
void CFtpView::OnWindowRefresh()
{
	// List available files
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	ListFiles();
    return;
}

void CFtpView::OnUpdateWindowRefresh(CCmdUI* pCmdUI)
{
	// we have to be connected to the FTP site in order to be refreshing
	// any directory listings
	pCmdUI->Enable(!m_btnConnect.IsWindowEnabled());
	return;
}

void CFtpView::OnUpdateSiteChgDir(CCmdUI* pCmdUI)
{
	// If the Change Remote Directory button is grayed, then gray this
	// command also; if the Change Remote Directory button is not grayed,
	// then this command can be enabled
	pCmdUI->Enable(m_btnChgDir.IsWindowEnabled());
	return;
}

void CFtpView::OnUpdateSiteChgDirUp(CCmdUI* pCmdUI)
{   
	// Disable if we're at the FTP site's root directory
	m_btnUp.EnableWindow(!m_strPWDirectory.IsEmpty() && 
			m_strPWDirectory != "/" && m_bConnected);
		
	pCmdUI->Enable(m_btnUp.IsWindowEnabled());	 
	return;
}

void CFtpView::OnUpdateConnect(CCmdUI* pCmdUI)
{
	
	pCmdUI->Enable(m_btnConnect.IsWindowEnabled());
	return;	
}

void CFtpView::OnUpdateDisconnect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnDisconnect.IsWindowEnabled());
	return;
}

void CFtpView::OnUpdateDownload(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnGet.IsWindowEnabled());
	return;	
}

void CFtpView::OnUpdateUpload(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnPut.IsWindowEnabled());
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFtpView client/server interactions helper functions

BOOL CFtpView::GetUserIDAndPassword()
{
	CString strText = ""; 
	
	CString strSiteName = m_pDoc->GetSiteName();
	CString strMsg = "";
	BOOL bResult = FALSE;
	
	if (!m_pDoc->GetUserID().IsEmpty() && !m_pDoc->GetPassword().IsEmpty()) 
	{       
		// If we are connecting and getting the password and user ID from
		// the document file, we don't need to modify the document here
	
		m_pDoc->SetModifiedFlag(FALSE);
		return TRUE; // don't need new userID and password from user
	}

	BOOL bMarkDirty = m_bMarkDirty;
	m_bMarkDirty = TRUE;
	UpdateData(TRUE);
	m_bMarkDirty = bMarkDirty; 
	    
	CConnectDlg dlg(this, m_pDoc->GetSiteName());
	
	if (dlg.DoModal() == IDOK)
	{ 
		bResult = TRUE;
		// get the UserID and Password the user provided.
		m_pDoc->m_strUserID = dlg.m_strUserID;
		m_pDoc->m_strPassword = dlg.m_strPassword; 
		m_pDoc->SetModifiedFlag(TRUE);
		m_pDoc->SaveModified(); // save the document file 
		
		// Set directory to root directory
		m_strPWDirectory = "/";
		m_pDoc->SetTitle(m_strPWDirectory, TRUE);
		
		return bResult; 
	}
	else  // user pressed cancel
	{
		bResult = FALSE;
		// Disable all buttons on this form except Connect and Help
		m_btnUp.EnableWindow(FALSE);
		m_btnChgDir.EnableWindow(FALSE);
		m_btnConnect.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(FALSE);
		m_btnGet.EnableWindow(FALSE);
		m_btnPut.EnableWindow(FALSE);
		m_btnHelp.EnableWindow(TRUE);
		
		CString strCmd = "QUIT";
		strCmd += "\r\n";
		m_pControlSocket->Send(strCmd); // Sends QUIT command to server	
		
		AfxFormatString1(strMsg, IDS_DISCONNECTED_TEXT, strSiteName);
	
		CFtpDisconnected theDialog(m_pApp->GetMainFrame());
		theDialog.m_strSiteName = strSiteName;
		theDialog.m_strMsg = strMsg;
	
		MessageBeep(-1);
		theDialog.DoModal();
	
		m_pStatusBar->ShowIdleMessage(); 
	
		return bResult;
	}
	return bResult;
}

BOOL CFtpView::HandleWelcomeMsg()
{
	CString strWelcome = "";
	m_bNoWelcome = TRUE;
	State = ReceivingWelcome;
	
	// Get the welcome message from the server
	CString strResponse = m_pControlSocket->GetLine();    
	CString strResponseCode = ""; 
	m_bNoWelcome = strResponse.IsEmpty();
	
	if (strResponse.GetAt(3) == '-')
	{
		strResponseCode = strResponse.Left(3);  
		
		// Chop off the response code from this line of text 
		if (strResponse.Left(3) == "230")
			strWelcome = strResponse.Right(strResponse.GetLength() - 4); 
		else
			strWelcome = strResponse;
	}
	else
	{
		strResponseCode = strResponse.Left(3);   
		
		// Chop off the response code from this line of the 
		// welcome message 
		if (strResponseCode == "230")
			strWelcome = strResponse.Right(strResponse.GetLength() - 3);
		else
			strWelcome = strResponse;
	}
	
	if (strResponse.GetAt(3) == '-' || strResponse[0] == ' ')
	{ 
		// Multiline response
		strResponse = m_pControlSocket->GetLine();
		m_bNoWelcome = strResponse.IsEmpty();
		
		// Chop off the response code from this line of the 
		// welcome message
		if (strResponse.Left(3) == "230")
			strWelcome += strResponse.Right(strResponse.GetLength() - 4);         
		else
			strWelcome += strResponse;
					
		m_bNoWelcome = strWelcome.IsEmpty();
		
		while ((strResponse.Left(3) != strResponseCode
				|| strResponse[3] == '-' || strResponse[0] == ' ')
			 && m_pControlSocket->IsConnected())
		{
			strResponse = m_pControlSocket->GetLine();
			strResponseCode = strResponse.Left(3);     
			
			// Chop off the response code and the following space from
			// this line of the welcome message
			if (strResponse.GetAt(3) == '-')
				strWelcome += strResponse.Right(strResponse.GetLength() - 4);
			else if (strResponse.GetAt(3) == ' ')
				strWelcome += strResponse.Right(strResponse.GetLength() - 3);
			else if (strResponse.Left(3) != strResponseCode)
				strWelcome += strResponse;    
			else
				strWelcome += strResponse;
			
			m_bNoWelcome = strWelcome.IsEmpty();
		}
		
		if (!m_pControlSocket->IsConnected())
			return FALSE;
	}

	// If the server reports that we are denied access, then we will test
	// the first character of the response code against a '4' or a '5'; these
	// are FTP fatal error message numbers.
	
	if (atoi(strResponseCode.Left(1)) == 4 
			|| atoi(strResponseCode.Left(1)) == 5)
	{
		CString strStatus = "Access to " + m_pDoc->GetSiteName();
		strStatus += " is denied.";
		m_pStatusBar->SetText(strStatus);
		
		CString strMsg = m_pDoc->GetSiteName() + " has denied you access to";
		strMsg += " its services.  Internet Navigator will now disconnect ";
		strMsg += "from " + m_pDoc->GetSiteName() + ".\r\n\r\nPress 'OK' below to";
		strMsg += " acknowledge this message.  For more information, press F1.";
		
		AfxMessageBox(strMsg, MB_ICONSTOP|MB_OK, IDP_FTP_ACCESS_DENIED);
        
        ReportFTPError(strResponse);

		State = Idle;
		return FALSE;
	}
	
	// we have the Welcome message, so now show it to the user in a dialog
	// box -- if we have all of the Welcome message.  If not, just
	// return TRUE and wait till this function is called again to get the
	// rest of the message and display it all.
	CWelcomeDlg dlg(this);
	dlg.m_strWelcome = strWelcome;
	
	if (strWelcome.IsEmpty())
		return FALSE;
		   
	if (!m_bNoWelcome)
	{
		if (dlg.DoModal() == IDOK) 
		{
			State = Idle;
			return TRUE; // proceed with Connect operation
		}
		else
		{  
			if (!m_bNoWelcome)
				return FALSE;
		}      
		State = Idle;
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
	State = Idle;
	return FALSE;
}

BOOL CFtpView::HandleResponse(BOOL bRETR /*= FALSE*/)
{
	// First, get the response we have to handle from the server.
	CString strResponse = m_pControlSocket->GetLine(); // whole response
	CString strResponseCode = strResponse.Left(3); // response code
	int nResponseCode = atoi(strResponseCode);

	m_pStatusBar->ShowIdleMessage();
	
	// diagnostics - dump response  

	#ifdef _DEBUG
		afxDump << strResponse;
	#endif
	                                                             
	// If we're about to download a file, get the file size from the
	// response text the server sends.  Otherwise, just handle this response
	// as if it's any other response the FTP server gives us	                                                             
	if (bRETR)
	{
		// parse, from the reply of the server, the size of the file 
		// to download in bytes.
		// Response string for RETR:
		// 115 Opening ASCII mode data connection for INDEX.TXT (n bytes)
		// where n = number of bytes
		// We want to extract 'n' from the above string, so we will use
		// the CString::Mid() function
	    
	    if (atoi(strResponseCode.Left(1)) == 4 || atoi(strResponseCode.Left(1)) == 5)
	    {
	    	ReportFTPError(strResponse);
	    	return FALSE; // unsuccessful
	    }
	    
		int nSpace, nFirstParen;
		nSpace = strResponse.ReverseFind('b'); // the 'b' in 'bytes'
		
		if (nSpace == -1)
		{
			m_nFileSize = 200;
			return TRUE;
		}
		
		nSpace = nSpace - 1;
		nFirstParen = strResponse.ReverseFind('(');
		
		CString str1 = strResponse.Right(strResponse.GetLength() - nFirstParen - 1);
		str1 = str1.Left(nSpace);
		
		m_nFileSize = (ULONG)atol(str1);
		/*bRETR = FALSE;*/
		return TRUE;
	}
	else if (nResponseCode == 257)
	{
		// PWD response -- we use this for determining whether or not
		// we're in the root directory and for showing the directory name
		// on the title bar
				
		// Cut away response code, following space, and " which precedes
		// the directory name
		strResponse = strResponse.Right(strResponse.GetLength() - 5);
		
		// Now cut away all response to the right of the " which follows
		// the directory name
		int nSpaceIndex = strResponse.Find(' ');
		if (nSpaceIndex == -1)
		{
			strResponse = "/";
		}
		else
		{
			strResponse = strResponse.Left(nSpaceIndex - 1);
		}
		
		m_strPWDirectory = strResponse;
		
		// Now set the window title to say the name of this ftp site
		// and the directory name as follows:
		// "ftp.site.com:/dir/.../subdir" or "ftp.site.com:/"
		m_pDoc->SetTitle(m_strPWDirectory, TRUE);  
	}
		
	if (strResponse[3] == '-')
	{          
		// Multiline response           
		m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
		strResponse = m_pControlSocket->GetLine();      

		while ((strResponse.Left(3) != strResponseCode
				|| strResponse[3] == '-'))
		{
			m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
			strResponse = m_pControlSocket->GetLine();
			strResponseCode = strResponse.Left(3);
		}
	}

	if (atoi(strResponseCode.Left(1)) == 1 || atoi(strResponseCode.Left(1)) == 2
		|| atoi(strResponseCode.Left(1)) == 3)
	{
		return TRUE;
	}
	else
	{   
		ReportFTPError(strResponse);
		return FALSE;
	}
}		

void CFtpView::ListFiles()
{ 
	m_btnChgDir.EnableWindow(FALSE);
	m_btnGet.EnableWindow(FALSE);
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnPut.EnableWindow(FALSE);

	// Open the data connection for receiving the files list
	if (!OpenDataConnection())
	{  
		m_pStatusBar->ShowIdleMessage();
		
		m_pDoc->m_bEmergencyClose = TRUE;
		OnDisconnect();
		return;
	}

	m_pStatusBar->SetText(IDS_REQUESTING_DIRECTORY);
	
	m_ctlFiles.ResetContent();
	UpdateData(FALSE);
	State = ReceivingList;
	m_pControlSocket->Send("LIST\r\n");
	
	#ifdef _DEBUG
		ReportFTPCommand("LIST");
	#endif
	
	if (!HandleResponse())
	{  
		m_pStatusBar->ShowIdleMessage();
		
		MessageBeep(-1);
		AfxMessageBox("Unable to request file list from server.");
		
		m_pDoc->m_bEmergencyClose = TRUE;
		OnDisconnect();
		return;
	} 
	
	if (!m_ftpLinkDestMap.IsEmpty())
		m_ftpLinkDestMap.RemoveAll();
	
	m_pStatusBar->SetText(IDS_REQUESTING_DIRECTORY);
	return;
}

BOOL CFtpView::GetDownloadInfo()           
{
   	if (m_strFileName == "index.txt")
   	{
   		m_strFilePath = m_pApp->GetFTPDownloadDir() + "index.txt";
   		m_strFileExt = "txt";

   		return TRUE;
   	}
   	else if (m_strFileName == "readme.txt")
   	{
   		m_strFilePath = m_pApp->GetFTPDownloadDir() + "readme.txt";
   		m_strFileExt = "txt";

   		return TRUE;
   	}   
   	else if (m_strFileName.Right(4) == ".htm")
   	{
   		m_strFilePath = m_pApp->GetFTPDownloadDir() + m_strFileName;
   		m_strFileExt = "htm";

   		return TRUE;
   	}
   	
	// set the working directory to the FTP Download dir
	if (!m_pApp->GetFTPDownloadDir().IsEmpty())
	{
		_chdir(m_pApp->GetFTPDownloadDir());                
		_chdir(m_pApp->GetFTPDownloadDir());
		_chdir(m_pApp->GetFTPDownloadDir());
	}

	CString strFilter = "All Files (*.*)|*.*||"; 
	
	// Construct the C++ object for the file selector dialog box.
	
	// NOTE: Since the default download directory is shown to the user
	// in the dialog box, the user must always see that same directory
	// every time this box appears.  Therefore we use the OFN_NOCHANGEDIR
	// flag here so that the current directory, which is set to the
	// default download directory above, is not changed
	CFileDialog dlg(FALSE, NULL, m_strFileName, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_SHOWHELP|OFN_NOCHANGEDIR,
			 strFilter, NULL);            
	                  
	dlg.m_ofn.lpstrTitle = "Save As";                                 
	dlg.m_ofn.lpstrInitialDir = m_pApp->GetFTPDownloadDir();
	
	if (dlg.DoModal() == IDOK)
	{
		// get the path, and extension of the file as supplied by
		// the Save As dialog box
		m_strFilePath = dlg.GetPathName();
		m_strFileExt = dlg.GetFileExt();
		return TRUE;
	}
	else // user pressed cancel
	{
		m_strFileName.Empty();
		m_nFileType = -1; 
		if (m_pUserFile)
		{
			delete m_pUserFile;
			m_pUserFile = NULL;
		}   
		
		#ifdef _DEBUG
			afxDump << "INF: Download aborted by user." << "\r\n";
		#endif //_DEBUG
		
		return FALSE;
	}
	return FALSE;
}

BOOL CFtpView::GetUploadInfo()
{ 
	CString strFilter = "Web File (*.htm;*.asp)|*.htm;*.asp|";
	strFilter += "GIF File (*.gif)|*.gif|";
	strFilter += "JPEG File (*.jpe;*.jpg)|*.jpe;*.jpg|";
	strFilter += "Program File (*.exe)|*.exe|";
	strFilter += "Application Extension (*.dll)|*.dll|";
	strFilter += "MPEG File (*.mpe;*.mpg)|*.mpe;*.mpg|";
	strFilter += "All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, 
		OFN_HIDEREADONLY|OFN_NOCHANGEDIR, strFilter, NULL); 
	
	dlg.m_ofn.lpstrTitle = "Open";
	                                     
	if (dlg.DoModal() == IDOK)
	{
		// get the path, and extension of the file as supplied by
		// the Open dialog box   
		m_strFileName = dlg.GetFileTitle();
		/*if (m_pApp->IsFTPUploadLowerCase())*/ m_strFileName.MakeLower();
		m_strFilePath = dlg.GetPathName();
		m_strFileExt = dlg.GetFileExt();   
		return TRUE;
	}
	else // user pressed cancel
	{
		m_strFileName.Empty();
		m_strFilePath.Empty();
		m_strFileExt.Empty();
		
		m_nFileType = -1; 
		if (m_pUserFile)
		{
			delete m_pUserFile;
			m_pUserFile = NULL;
		}
		return FALSE;
	}
	return FALSE;
}     

/////////////////////////////////////////////////////////////////////////////
// CFtpView diagnostic command reporting function

#ifdef _DEBUG

void CFtpView::ReportFTPCommand(CString strCmd)
{
	CString strDisplay = strCmd + " command sent to " + m_pDoc->GetSiteName();
	
	m_pStatusBar->SetText(strDisplay);  

	return;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFilesListBox list box

void CFilesListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// There's a HDC device context handle in the DRAWITEMSTRUCT that
	// we can use for drawing.  To make things easier on ourselves,
	// let's use CDC::FromHandle to get a CDC* pointer using the HDC handle
	// in the DRAWITEMSTRUCT structure
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);  

	COLORREF clrText, clrBk, clrOldText, clrOldBk;
	
	if (lpDrawItemStruct->itemID == -1) // nothing is selected 
	{
		// Draw the focus rectangle and return
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		return;                                     
	}	
	
	// Initialize colors		
	if (lpDrawItemStruct->itemAction & ODA_SELECT)	// sel status changed 
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED) // item is selected
		{
			// The current item is selected -- get the system's color
			// for highlighting and text
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		} 
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else // The current item isn't selected, but another item is
		{                                                           
			// Just get the system window and text color
			clrText = ::GetSysColor(COLOR_WINDOWTEXT);
			clrBk = ::GetSysColor(COLOR_WINDOW);
		}
		lpDrawItemStruct->itemAction |= ODA_DRAWENTIRE;
	}
	else if (lpDrawItemStruct->itemAction & ODA_FOCUS)
	{   
		// Current item may or may not have input focus
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// The current item is selected -- get the system's color
			// for highlighting and text
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			// The current item may be still selected let's take a gamble
			// here 
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else // The current item is not selected
		{
			clrText = ::GetSysColor(COLOR_WINDOWTEXT);
			clrBk = ::GetSysColor(COLOR_WINDOW);
		}
		lpDrawItemStruct->itemAction |= ODA_DRAWENTIRE;
	}				
	else
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// highlight and highlighttext colors
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
			
		clrText = ::GetSysColor(COLOR_WINDOWTEXT);
		clrBk = ::GetSysColor(COLOR_WINDOW);
	}

	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) // draw whole control
	{
		// Get the first line of the item to draw to determine whether
		// to draw a FOLDER bitmap (for a directory), a FILE bitmap
		// (for a file) or a LINK bitmap (for a FTP symbolic link)
		CBitmap bmIcon;
		CString strItem = "";
		
		GetText(lpDrawItemStruct->itemID, strItem);
		switch (strItem[0])
		{	
			case 'd': // directory
				// draw the folder icon -- an open folder if selected, closed
				// if not
				if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
					(lpDrawItemStruct->itemState & ODS_SELECTED))
					bmIcon.LoadBitmap(IDB_OPEN_FOLDER);
				else if ((lpDrawItemStruct->itemAction & ODA_FOCUS)
						&& (lpDrawItemStruct->itemState & ODS_SELECTED))
					bmIcon.LoadBitmap(IDB_OPEN_FOLDER);
				else
					bmIcon.LoadBitmap(IDB_FOLDER); 
				break;
				
			case '-': // file
				// draw the file icon
				bmIcon.LoadBitmap(IDB_FILE);
				break;
				
			case 'l': // link      
				// draw the link icon
				bmIcon.LoadBitmap(IDB_LINK);
				break;
		}
        
       	clrOldText = pDC->SetTextColor(clrText); 
        clrOldBk = pDC->SetBkColor(clrBk);
        
        int nMode = pDC->SetBkMode(TRANSPARENT);
            
		CBrush brBk(clrBk);
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brBk);
		
		CBitmap* pBitmap = &bmIcon;
		// Draw bmIcon transparently onto the listbox
		BITMAP bitmap; // bitmap info structure
		DrawTransparentBitmap(pDC, pBitmap, bitmap,
			lpDrawItemStruct->rcItem.left + 5,
			lpDrawItemStruct->rcItem.top, RGB(0,128,128));
		
		// Now draw the text associated with the item 	
		GetText(lpDrawItemStruct->itemID, strItem);
		
		pDC->TextOut(lpDrawItemStruct->rcItem.left + bitmap.bmWidth + 10,
					lpDrawItemStruct->rcItem.top, strItem);  
					
		pDC->SetTextColor(clrOldText);
		pDC->SetBkColor(clrOldBk);
		
		pDC->SetBkMode(nMode);
		
		if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
				(lpDrawItemStruct->itemState & ODS_SELECTED))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);    
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
					(lpDrawItemStruct->itemState & ODS_SELECTED))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		else if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
					(lpDrawItemStruct->itemState & ODS_FOCUS))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
					(lpDrawItemStruct->itemState & ODS_FOCUS))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		
		return;      
	}                               

	if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);    
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
				(lpDrawItemStruct->itemState & ODS_SELECTED))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	else if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
				(lpDrawItemStruct->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
				(lpDrawItemStruct->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);      
}

void CFilesListBox::DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap, 
	BITMAP& bitmap, short xStart, short yStart, COLORREF crTransparentColor)
{                                   
   COLORREF   crColor;
   CBitmap    bmAndBack, bmAndObject, bmAndMem, bmSave;
   CBitmap   *bmBackOld, *bmObjectOld, *bmMemOld, *bmSaveOld;
   CDC        memDC, backDC, objectDC, tempDC, saveDC;
   CPoint     ptSize;

   tempDC.CreateCompatibleDC(pDC);
   CBitmap* pOldTemp = tempDC.SelectObject(pBitmap);
   
   // Fill the BITMAP structure (which is a reference argument) with
   // bitmap information
   pBitmap->GetObject(sizeof(BITMAP), &bitmap);

   ptSize.x = bitmap.bmWidth;            // Get width of bitmap
   ptSize.y = bitmap.bmHeight;           // Get height of bitmap
   tempDC.DPtoLP(&ptSize, 1);      // Convert from device
                                     // to logical points

   // Create some DCs to hold temporary data.
   backDC.CreateCompatibleDC(pDC);
   objectDC.CreateCompatibleDC(pDC);
   memDC.CreateCompatibleDC(pDC);
   saveDC.CreateCompatibleDC(pDC);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC

   bmAndBack.CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject.CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem.CreateCompatibleBitmap(pDC, ptSize.x, ptSize.y);
   bmSave.CreateCompatibleBitmap(pDC, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = backDC.SelectObject(&bmAndBack);
   bmObjectOld = objectDC.SelectObject(&bmAndObject);
   bmMemOld    = memDC.SelectObject(&bmAndMem);
   bmSaveOld   = saveDC.SelectObject(&bmSave);

   // Set proper mapping mode.
   tempDC.SetMapMode(pDC->GetMapMode());

   // Save the bitmap sent here, because it will be overwritten.
   saveDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   crColor = tempDC.SetBkColor(crTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt()
   // from the source bitmap to a monochrome bitmap.
   objectDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0,
          SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   tempDC.SetBkColor(crColor);

   // Create the inverse of the object mask.
   backDC.BitBlt(0, 0, ptSize.x, ptSize.y, &objectDC, 0, 0,
          NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, pDC, xStart, yStart,
          SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, &objectDC, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   tempDC.BitBlt(0, 0, ptSize.x, ptSize.y, &backDC, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   pDC->BitBlt(xStart, yStart, ptSize.x, ptSize.y, &memDC, 0, 0,
          SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   tempDC.BitBlt(0, 0, ptSize.x, ptSize.y, &saveDC, 0, 0, SRCCOPY);
   
   CBitmap* pTemp = tempDC.SelectObject(pOldTemp);
   
   	// Delete the memory bitmaps.
   	pTemp = backDC.SelectObject(bmBackOld);
   	pTemp->DeleteObject();
   	
   	pTemp = objectDC.SelectObject(bmObjectOld);
   	pTemp->DeleteObject();
   	
   	pTemp = memDC.SelectObject(bmMemOld);
   	pTemp->DeleteObject();
    
    pTemp = saveDC.SelectObject(bmSaveOld);
    pTemp->DeleteObject();
    
   	// Delete the memory DCs.
   	memDC.DeleteDC();
   	backDC.DeleteDC();

   	objectDC.DeleteDC();
   	saveDC.DeleteDC();
	tempDC.DeleteDC();
}

void CFilesListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CBitmap bmIcon;

	bmIcon.LoadBitmap(IDB_FOLDER);
	BITMAP bitmap;
	bmIcon.GetObject(sizeof(BITMAP), &bitmap);
	
	lpMeasureItemStruct->itemHeight = bitmap.bmHeight;  
	return;
}
		
void CFtpView::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strFTPSect = "FTP";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strFTPSect, "FTPLeft", rect.left);
	pApp->WriteProfileInt(strFTPSect, "FTPRight", rect.right);
	pApp->WriteProfileInt(strFTPSect, "FTPTop", rect.top);
	pApp->WriteProfileInt(strFTPSect, "FTPBottom", rect.bottom);
	pApp->WriteProfileInt(strFTPSect, "FTPCX", rect.Width());
	pApp->WriteProfileInt(strFTPSect, "FTPCY", rect.Height());	
}

void CFtpView::ReportFTPError(CString& strResponse)
{
	CString strCaption = m_pDoc->GetSiteName(), strMsg = "";
	UINT nType = MB_OK|MB_ICONSTOP;
	
	int nLength;
	
	nLength = strResponse.GetLength();
	CString strResponsecode = strResponse.Left(4);
	if (strResponsecode.Find(' ') == -1)
	{
		// no space after response code -- message starts at strResponse[4]
		strMsg = strResponse.Right(nLength - 3);
	}
	else
		strMsg = strResponse.Right(nLength - 4); 
	
	MessageBeep(-1);
	CWnd::MessageBox(strMsg, strCaption, nType);
}
		
void CFtpView::OnTransfersLink()
{ 
	CWaitCursor wait;
	OnDblclkFiles(); 
	return;
}
		
void CFtpView::OnViewFtpProperties()
{ 
	CFtpPropsSheet theSheet(m_pApp->GetMainFrame());
	theSheet.m_pgFtpPropsGeneral.m_strUserID = m_pDoc->GetUserID();
	theSheet.m_pgFtpPropsGeneral.m_strPassword = m_pDoc->GetPassword();
	theSheet.m_pgFtpPropsGeneral.m_strSiteAddress = m_pDoc->GetSiteName();
	if (!m_strFileName.IsEmpty())
		theSheet.m_pgFtpPropsFiles.m_strFileName = m_strFileName;
	else
		theSheet.m_pgFtpPropsFiles.m_strFileName = "";
	if (!m_strFilePath.IsEmpty())
		theSheet.m_pgFtpPropsFiles.m_strFilePath = m_strFilePath;
	else
		theSheet.m_pgFtpPropsFiles.m_strFilePath = "";
	
	if (theSheet.DoModal() == IDOK)
	{
		m_pDoc->m_strUserID = theSheet.m_pgFtpPropsGeneral.m_strUserID;
		m_pDoc->m_strPassword = theSheet.m_pgFtpPropsGeneral.m_strPassword;
		m_pDoc->m_strSiteName = theSheet.m_pgFtpPropsGeneral.m_strSiteAddress;
		m_pDoc->SetModifiedFlag();
		
		// Since the user has changed the connection information of this
		// site, let's ask them if they want to disconnect
		if (AfxMessageBox(IDP_DISCONNECT_INFO_CHANGED,
				MB_ICONQUESTION|MB_YESNO) == IDYES)
		{
			OnDisconnect();
		}
	}
}

void CFtpView::OnUpdateTransfersLink(CCmdUI* pCmdUI)
{
	int nIndex = m_ctlFiles.GetCurSel();
	if (nIndex >= 0)
		m_ctlFiles.GetText(nIndex, m_strSelection); 
	else
	{
		pCmdUI->Enable(FALSE);
		return;                
	}
	
	pCmdUI->Enable(m_strSelection[0] == 'l');
}

void CFtpView::ResetToolBar()
{
	OnActivateView(TRUE, this, NULL);
	return;
}

void CFtpView::OnActivateView(BOOL bActivate, CView* pActivateView,
								CView* pDeactiveView)
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	
	CMainFrame* pFrame = m_pApp->GetMainFrame();
	
	if (bActivate)
	{
		pFrame->GetToolBar()->LoadBitmap(IDR_FTPTYPE);
		pFrame->GetToolBar()->SetButtons(ftpButtons, 20);
		
		// Also, if we are in the middle of a file download or upload,
		// keep the wait cursor visible and display a 'Please wait.'
		// message in the status bar
		
		if (State == ReceivingFile)
		{ 
			CString strDLMsg = "Copying " + m_strFileName + "...  Please";
			strDLMsg += " wait.";
			m_pStatusBar->SetText(strDLMsg);
		}
		else if (State == SendingFile)
		{
			CString strULMsg = "Copying " + m_strFileName + "...  Please ";
			strULMsg += "wait.";
			m_pStatusBar->SetText(strULMsg);    
		}
		else if (State == ReceivingList)
		{
			m_pStatusBar->SetText("Receiving directory list...  Please wait.");	
		} 
		else if (State == Idle)
		{
			m_pStatusBar->ShowIdleMessage();
		}
		
		pFrame->GetToolBar()->Invalidate();
		pFrame->GetToolBar()->UpdateWindow();
	
		pFrame->UpdateWindow();
		return;
	}                                   	
	else
	{
		pFrame->ResetToolBar();
		pFrame->GetToolBar()->Invalidate();
		pFrame->GetToolBar()->UpdateWindow();
		
		if (State == ReceivingFile)
		{ 
			CString strDLMsg = "Copying " + m_strFileName + "...  Please";
			strDLMsg += " wait.";
			m_pStatusBar->SetText(strDLMsg);
		}
		else if (State == SendingFile)
		{
			CString strULMsg = "Copying " + m_strFileName + "...  Please ";
			strULMsg += "wait.";
			m_pStatusBar->SetText(strULMsg);    
		} 
		else if (State == ReceivingList)
		{
			m_pStatusBar->SetText("Receiving directory list...  Please wait.");	
		} 
		else if (State == Idle)
		{
			m_pStatusBar->ShowIdleMessage();
		}
		
		pFrame->UpdateWindow();
		
		return;
	}
}

void CFtpView::OnCancelCall()
{ 
	if (m_pPercentDialog)
	{    
		m_pPercentDialog->ShowWindow(SW_HIDE);
		m_pPercentDialog->UpdateWindow();
		
		m_pPercentDialog->DestroyWindow();
	}
			
	// The user wants to cancel a blocking Windows Sockets call which
	// is making the computer hang, or a download which is
	// taking to long.  The control socket is the only socket
	// which could initiate such a call, so we can call its 
	// CancelBlockingCall function
	
	if (State == ReceivingFile) 
	{        
		State = Idle;  // stops action on the data socket  

		m_pDataSocket->CancelBlockingCall();
		
		// To get the FTP server's attention and cause it to 
		// stop its transmission of the file, send the ABOR command
		// over the control socket and disconnect and delete the
		// listening and data socket                 
		m_pStatusBar->SetText("Aborting download...  Please wait.");
		m_pControlSocket->Send("ABOR\r\n");
		
		// Disconnect and delete the listening and data socket
		m_pListeningSocket->Disconnect();
		delete m_pListeningSocket;
		m_pListeningSocket = NULL;
		
		m_pDataSocket->Disconnect();
		delete m_pDataSocket;
		m_pDataSocket = NULL;
		
		// Get the response to the ABOR command from the FTP server
		m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
		CString strResponse = m_pControlSocket->GetLine();
				
		m_pStatusBar->SetText("Download aborted.");
		MessageBeep(-1); 
		AfxMessageBox(IDP_DOWNLOAD_ABORTED,
			MB_ICONINFORMATION|MB_OK);
				
		m_pStatusBar->ShowIdleMessage();
	
		// Remove what data has already been downloaded 
		m_pUserFile->Close();
		delete m_pUserFile;
		m_pUserFile = NULL;

		TRY
		{
			m_pStatusBar->SetText("Deleting file...  Please wait.");
			CFile::Remove(m_strFilePath);
		}
		CATCH(CFileException, pFileException)
		{   
			// Tells the user the cause of the error
			EmitFileException(pFileException);
			
			m_strFileName.Empty();
			m_strFilePath.Empty();
			m_strFileExt.Empty();
			
			State = Idle;
			
			m_nBytes = m_nFileType = 0;
			
			m_btnConnect.EnableWindow(FALSE);
			m_btnDisconnect.EnableWindow(TRUE);
			
			m_ctlFiles.SetCurSel(0);
			
			return;
		}
		END_CATCH
		
		m_pStatusBar->ShowIdleMessage();
				
		m_strFileName.Empty();
		m_strFilePath.Empty();
		m_strFileExt.Empty();
        
        m_nBytes = m_nFileType = 0;

		// Finish by setting everything to idle and adjusting button graying
		// using OnSelchangeFiles();       
		State = Idle;     

		m_btnConnect.EnableWindow(FALSE);
		m_btnDisconnect.EnableWindow(TRUE);
		        
        m_ctlFiles.SetCurSel(0);
        m_nSelection = m_ctlFiles.GetCurSel();

		m_pStatusBar->ShowIdleMessage();  
		return;  // skip the rest of this function
	}
	else if (State == ReceivingList)
	{
		// If we're receiving a list and the user presses ESC, they've
		// already seen enough of the list they want to see displayed.
		State = Idle;   
		
		m_pDataSocket->CancelBlockingCall();
		
		m_pControlSocket->Send("ABOR\r\n");

		m_pDataSocket->Disconnect();
		m_pListeningSocket->Disconnect();
		
		delete m_pDataSocket;
		m_pDataSocket = NULL;
		
		delete m_pListeningSocket;
		m_pListeningSocket = NULL;
		
		CString response = m_pControlSocket->GetLine();
		
		// Show the user a dialog box asking them which file they would like
		// to download
		PostMessage(WM_COMMAND, ID_SITE_DOWNLOAD_FILE, 0L);
		
		m_ctlFiles.SetCurSel(0);
		OnSelchangeFiles();
		
		return;
	} 
	else if (State == SendingFile) // cancel a file upload
	{   
		State = Idle;
		
		m_pDataSocket->CancelBlockingCall();
		
		// If we're uploading a file, send the ABOR command to the server
		m_pControlSocket->Send("ABOR\r\n");
		if (!HandleResponse())
			return;                        
		
		// If the server has a copy of the file, have the server delete its
		// copy
		m_pControlSocket->Send("DELE " + m_strFileName + "\r\n");
		if (!HandleResponse())
			return;
		
		m_strFilePath = m_strFileName = m_strFileExt = "";
		m_nFileSize = 0;
		m_nBytes = 0;  
	
		// Turn off lingering on the socket		
		int what = 0;
		struct linger sL;
		sL.l_onoff = 0;
		sL.l_linger = 0;
	
		what = m_pDataSocket->SetSockOpt(SO_DONTLINGER, 
					(char *)&sL, sizeof(sL));
		what = m_pListeningSocket->SetSockOpt(SO_DONTLINGER, 
					(char *)&sL, sizeof(sL));

		m_pDataSocket->Disconnect();
		
		delete m_pDataSocket;       
		m_pDataSocket = NULL;
		
		m_pListeningSocket->Disconnect();
		delete m_pListeningSocket;
		m_pListeningSocket = NULL;
		
		AfxMessageBox("Upload aborted!", MB_ICONINFORMATION|MB_OK);
		
		State = Idle; 
		
		m_ctlFiles.ResetContent();
		ListFiles(); // refresh file list
		     
		if (m_ctlFiles.GetCount() > 0) 
		{
			m_ctlFiles.SetCurSel(0);
		}
		OnSelchangeFiles();
		
		m_btnConnect.EnableWindow(FALSE);
		m_btnDisconnect.EnableWindow(TRUE);
		
		return;
	}
	else if (State == ReceivingWelcome)
	{
		// Disconnecting from the FTP server is the thing to do!
		OnDisconnect();
		return;
	}
		
	if (m_pControlSocket->IsBlocking())
    {  
    	m_pStatusBar->ShowIdleMessage();
    	
    	m_pControlSocket->CancelBlockingCall();
		
		CString site = m_pDoc->GetSiteName();
		CString userID = m_pDoc->GetUserID();
		CString pass = m_pDoc->GetPassword();
		
		// Disconnect from the FTP site
		m_pDoc->m_bEmergencyClose = TRUE;
		OnDisconnect();
    	
    	m_bConnected = FALSE;
    	
    	if (AfxMessageBox("Connection to " + m_pDoc->GetSiteName() + " has been lost.",
    		MB_ICONEXCLAMATION|MB_RETRYCANCEL) == IDRETRY)
		{ 
			m_pDoc->m_strSiteName = site;
			m_pDoc->m_strPassword = pass;
			m_pDoc->m_strUserID = userID;
			m_pDoc->SetModifiedFlag();
			   		
    		m_pStatusBar->SetText("Re-connecting to " + m_pDoc->GetSiteName() + "...");
    		OnConnect();
    		return;
    	}
    }
    
	// Delete the sockets 
	m_pStatusBar->ShowIdleMessage();
	
	delete m_pListeningSocket;
	m_pListeningSocket = NULL;
		
	delete m_pDataSocket;
	m_pDataSocket = NULL;
	
	m_pStatusBar->ShowIdleMessage(); 
    
    return;
}  

void CFtpView::OnEndList()
{
	// Give the user visual cues that the download is complete
	// by re-enabling buttons
	
	// Enable Disconnect button, disable Connect	
	m_btnConnect.EnableWindow(FALSE);
	m_btnDisconnect.EnableWindow(); 
	m_btnPut.EnableWindow();

	// Last thing, show the 'idle' message on the status bar and call
	// OnIdle() to clear temporary objects
	AfxGetApp()->OnIdle(-1);
	m_pStatusBar->ShowIdleMessage();
	return;
}
	
void CFtpView::OnEndDownload()
{ 
	if (!m_strFilePath.IsEmpty()) // no errors
	{
		m_pStatusBar->SetText("File transfer complete.");           
		m_pPercentDialog->SetPercent(100);       
		m_pPercentDialog->SetPercent(100);
		m_pPercentDialog->SetPercent(100);
 
		// If necessary, destroy the percent gauge dialog box     
		if (m_pPercentDialog && IsWindow(m_pPercentDialog->GetSafeHwnd()))
		{
			m_pPercentDialog->ShowWindow(SW_HIDE);
			m_pPercentDialog->UpdateWindow();
			
			m_pPercentDialog->DestroyWindow();
		}
	    
		m_strFileName.MakeUpper();
	    
		AfxFormatString1(m_strMsg, IDS_DOWNLOAD_MESSAGE, m_strFileName);
		
		MessageBeep(-1);
		CDLComplete theDialog(m_pApp->GetMainFrame());
		theDialog.m_strFileDL = m_strMsg;
		
		theDialog.DoModal(); // dialog only has an OK button

		delete m_pUserFile;
		m_pUserFile = NULL; 

		m_btnGet.EnableWindow(TRUE); 
		m_btnPut.EnableWindow(TRUE);
		m_btnDisconnect.EnableWindow(TRUE);
		m_btnUp.EnableWindow(!m_strPWDirectory.IsEmpty() &&
					m_strPWDirectory != "/");

		m_pStatusBar->ShowIdleMessage();
		
		DoOpenDownloadedFile();
	  	
	  	m_strFilePath.Empty();
	  	m_strFileName.Empty();
	  	m_strFileExt.Empty();
	  	m_nFileType = -1;
	}
}

void CFtpView::OnEndUpload()
{    
	m_pStatusBar->SetText("File transfer complete.");           
	m_pPercentDialog->SetPercent(100);       
	m_pPercentDialog->SetPercent(100); 
	m_pPercentDialog->SetPercent(100);
	m_pPercentDialog->SetPercent(100);     
	
	// If necessary, destroy the percent gauge dialog box     
	if (m_pPercentDialog && IsWindow(m_pPercentDialog->GetSafeHwnd()))
	{
		m_pPercentDialog->ShowWindow(SW_HIDE);
		m_pPercentDialog->UpdateWindow();
		
		m_pPercentDialog->DestroyWindow();
	}
                                                                
	m_strFileName.MakeUpper();                                                                	
	AfxFormatString1(m_strMsg, IDS_UPLOAD_MESSAGE, m_strFileName);
		
	MessageBeep(-1);
	CDLComplete theDialog(m_pApp->GetMainFrame());
	theDialog.m_strFileDL = m_strMsg;
		
	theDialog.DoModal(); // dialog only has an OK button

	delete m_pUserFile;
	m_pUserFile = NULL;
	
	delete m_pDataSocket;
	m_pDataSocket = NULL;
	
	m_btnGet.EnableWindow(TRUE); 
	m_btnPut.EnableWindow(TRUE);
	m_btnDisconnect.EnableWindow(TRUE);
	m_btnUp.EnableWindow(!m_strPWDirectory.IsEmpty() &&
				m_strPWDirectory != "/");
	
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	ListFiles();
	
	// Clear out all our settings
	m_strFileName = m_strFilePath = m_strFileExt = "";
	m_nFileSize = m_nBytes = 0; 
	m_nFileType = -1;
	
	// bing! we're done!
	return;
}              

void CFtpView::DoOpenDownloadedFile()
{	
	if (m_nFileType == TYPE_ASCII && m_pApp->IsFTPOpenASCII() &&
			!m_strFilePath.IsEmpty())   
	{
		/*wait.Restore();*/   
			
		// If the file's extension is HTM, then open the user's
		// Web browser on the file via ::ShellExectue()
		if ((m_strFileExt.CompareNoCase("htm")) == 0)
		{
			HINSTANCE hInst = ::ShellExecute(AfxGetMainWnd()->GetSafeHwnd(),
					NULL,
					m_strFilePath,
					NULL,
					m_pApp->GetFTPDownloadDir(),
					SW_SHOWMAXIMIZED);
				
			if (hInst > (HINSTANCE)32)
				return;
			else // couldn't open file because of error
			{ 
				// TODO: Add better error-handling code here which tells
				// the user details about the error
					
				EmitShellExecuteError(hInst);
				return;
			}				 
		}
			
		// If we made it this far, open Internet Navigator's Text window
		// on the file
		m_pApp->GetTXTType()->OpenDocumentFile(m_strFilePath);
		m_strFilePath.Empty();
	} 
	else if (m_nFileType == TYPE_BINARY && m_pApp->IsFTPOpenBin() &&
			!m_strFilePath.IsEmpty())
	{
		CWarnDlg warn; // warns the user about possible viruses
            
        int result = IDOK;
            
        if (IsProgramFile() && m_pApp->IsWarnFTPOpenBin())
        {
        	MessageBeep(-1);
        	result = warn.DoModal();
        }
             
        if (result == IDOK)	// user wants to open file
        {      
        	if ((result = warn.m_nType) == 0)
        	{
				if (IsProgramFile())
				{
					// use WinExec to run an application file
	
					HINSTANCE hInst = (HINSTANCE)::WinExec(m_strFilePath, SW_SHOW);
						
					if (hInst <= (HINSTANCE)32)
					{
						EmitShellExecuteError(hInst);
					}	
					return;
				}
				else if (IsHelpFile())
				{
					// Run WinHelp() on the file
					COpenHelpDlg theDialog(m_pApp->GetMainFrame());
					int nResult = theDialog.DoModal();
						
					if (nResult == IDOK)
					{
						int nSel = theDialog.m_nSelection;
						switch (nSel)
						{
							case 0:	// open the Contents topic
								::WinHelp(GetSafeHwnd(), m_strFilePath,
									HELP_CONTENTS, 0L);
								break;
								
							case 1:   // search
								::WinHelp(GetSafeHwnd(), m_strFilePath,
									HELP_PARTIALKEY, (DWORD)(LPSTR)"");
								break;                        
						}
					}				
				} 
				else
				{  
					HINSTANCE hInst = ::ShellExecute(AfxGetMainWnd()->GetSafeHwnd(),
								NULL,
								m_strFilePath,
								NULL,
								m_pApp->GetFTPDownloadDir(),
								SW_SHOW);
						
					if (hInst > (HINSTANCE)32)
						return; // success!
					else  // an error occurred -- tell the user.
					{
						EmitShellExecuteError(hInst);
						return;
					}
				}         
			}
			else if ((result = warn.m_nType) == 1)
			{
				DoVirusScan();
			}
		}
  	}    
}	

void CFtpView::DoVirusScan()
{     
	// The dialog box shown below runs the user's virus scanner in its
	// OnInitDialog() and then lets the user choose to: open the file,
	// save the file, or delete the file.  The dialog box returns IDOK for
	// the first choice, IDCANCEL for the second choice, and IDABORT for
	// the third choice.
	
	CFtpVirusScan theDialog(AfxGetMainWnd());
	int nResult = theDialog.DoModal();	
	
	switch (nResult)
	{
		case IDOK:	// User wants to open file
			if (IsProgramFile())
			{
				// use WinExec to run an application file
	
				HINSTANCE hInst = (HINSTANCE)::WinExec(m_strFilePath, SW_SHOW);
						
				if (hInst <= (HINSTANCE)32)
				{
					EmitShellExecuteError(hInst);
				}	
				return;
			}
			else if (IsHelpFile())
			{
				// Run WinHelp() on the file
				COpenHelpDlg theDialog(m_pApp->GetMainFrame());
				int nResult = theDialog.DoModal();
						
				if (nResult == IDOK)
				{
					int nSel = theDialog.m_nSelection;
					switch (nSel)
					{
						case 0:	// open the Contents topic
							::WinHelp(GetSafeHwnd(), m_strFilePath,
								HELP_CONTENTS, 0L);
							break;
								
						case 1:   // search
							::WinHelp(GetSafeHwnd(), m_strFilePath,
								HELP_PARTIALKEY, (DWORD)(LPSTR)"");
							break;                        
					}
				}				
			} 
			else
			{  
				HINSTANCE hInst = ::ShellExecute(AfxGetMainWnd()->GetSafeHwnd(),
							NULL,
							m_strFilePath,
							NULL,
							m_pApp->GetFTPDownloadDir(),
							SW_SHOW);
						
				if (hInst > (HINSTANCE)32)
					return; // success!
				else  // an error occurred -- tell the user.
				{
					EmitShellExecuteError(hInst);
					return;
				}
			}   
			break;
			
		case IDCANCEL:
			break;       
			
		case IDABORT:
			CFile::Remove(m_strFilePath);
			AfxMessageBox(m_strFilePath + " has been removed from your computer.",
				MB_ICONASTERISK|MB_OK);
			break; 
	}
} 
							
void CFtpView::GetFileType()
{
	if (m_strFileExt.IsEmpty())
	{
		// assume file is a text file
		m_nFileType = TYPE_ASCII;
		return;                  
	}
	
	if (m_strFileExt == "TXT" || m_strFileExt == "txt" ||
		m_strFileExt == "HTM" || m_strFileExt == "htm" || 
		m_strFileExt == "INI" || m_strFileExt == "ini" ||
		m_strFileExt == "ME"  || m_strFileExt == "me"  ||
		m_strFileExt == "MSG" || m_strFileExt == "msg" ||
		m_strFileExt == "S"   || m_strFileExt == "s"   ||
		m_strFileExt == "sh"  || m_strFileExt == "sh"  ||
		m_strFileExt == "sha" || m_strFileExt == "sha" ||
		m_strFileExt == "RFC" || m_strFileExt == "rfc" ||
		m_strFileExt == "CPP" || m_strFileExt == "cpp" ||
		m_strFileExt == "C"   || m_strFileExt == "c"   ||
		m_strFileExt == "CXX" || m_strFileExt == "cxx" ||
		m_strFileExt == "HPP" || m_strFileExt == "hpp" ||
		m_strFileExt == "H"   || m_strFileExt == "h"   ||
		m_strFileExt == "HXX" || m_strFileExt == "hxx" ||
		m_strFileExt == "RC"  || m_strFileExt == "rc"  ||
		m_strFileExt == "APS" || m_strFileExt == "aps" ||
		m_strFileExt == "HQX" || m_strFileExt == "hqx" ||
		m_strFileExt == "DEF" || m_strFileExt == "def" ||
		m_strFileExt == "1ST" || m_strFileExt == "1st" /*all are ASCII text*/)
	{
		// set file type to text	
		m_nFileType = TYPE_ASCII;
		return;
	}
	else if (m_strFileExt == "OLD" || m_strFileExt == "old" ||
			 m_strFileExt == "BAC" || m_strFileExt == "bac" ||
			 m_strFileExt == "BAK" || m_strFileExt == "bak" ||
			 m_strFileExt == "REV" || m_strFileExt == "rev")
	{
		// older version of a revised file -- ask the user what kind of file
		// it is
		CString strMsg = m_strFileName + " may be an older version of";
		strMsg += " a revised or changed file.  Is this file a plain text ";
		strMsg += "file (which you could view in Notepad) or another kind";
		strMsg += " of file?\r\n\r\nPlease press the Yes button if the file";
		strMsg += " is a plain text file.  Otherwise, please press the No ";
		strMsg += "button.  Thank you!";
		
		if (AfxMessageBox(strMsg, MB_ICONQUESTION|MB_YESNO) == IDYES)
			m_nFileType = TYPE_ASCII;
		else
			m_nFileType = TYPE_BINARY;   
		strMsg.Empty();
		return;
	}                                                      
	else if (m_strFileExt.GetLength() == 1 ||
		m_strFileExt.GetLength() == 2 || m_strFileExt == "FYI" || 
		m_strFileExt == "fyi" || m_strFileExt == "STD" || 
		m_strFileExt == "std" || m_strFileExt == "FAQ" ||
		m_strFileExt == "faq")
	{
		m_nFileType = TYPE_BINARY;
	}    
	else if (IsProgramFile())
	{
		m_nFileType = TYPE_BINARY;
	} 
	else if (IsHelpFile())
	{
		m_nFileType = TYPE_BINARY;
	}
	else // all other kinds of files are binary files
	{
		// set file type to binary
		m_nFileType = TYPE_BINARY;
		return;
	}
}

void CFtpView::OnSiteChgUserDir()
{
	CFTPUserDir theDialog(m_pApp->GetMainFrame());
	CString strSite = m_pDoc->GetSiteName();
	CString strMsg = "";

	if (theDialog.DoModal() == IDOK)
	{
		m_pStatusBar->SetText("Contacting " + strSite + "...  Please wait.");
		m_pControlSocket->Send("CWD " + theDialog.m_strDirectory + "\r\n");
	    
	    if (!HandleResponse())
		{ 
			strMsg = "Unable to change to " + theDialog.m_strDirectory + ".";
			strMsg += "  Refresing files list...";
			m_pStatusBar->SetText(strMsg); 
			PostMessage(WM_COMMAND, ID_WINDOW_REFRESH, 0); 
			return;
		}
    
    	m_pControlSocket->Send("PWD\r\n");
        
        // The HandleResponse() call is non-fatal for the PWD command, so we'll
    	// ignore its return value
    	HandleResponse();
    
    	m_ctlFiles.ResetContent();
    
		strMsg = "Requesting files list for " + theDialog.m_strDirectory; 
		strMsg += "...  Please wait.";
		m_pStatusBar->SetText(strMsg);
	    
	    
		ListFiles(); 
		return;	
	}
}
  
// Files list filtering function
BOOL CFtpView::OnAddToFilesList(CString& strItem)
{  
	CString link = "", dest = "";
	
	if (strItem.IsEmpty())
		return FALSE;
		
	// If the item is a symbolic link, then parse out the name and
	// destination, enter them into a name-dest map, and then return only
	// the link name to show the user	
	if (strItem.Left(1) == 'l')
	{
		// symbolic link
		// format: "name -> dest"
		int nLinkStart = strItem.ReverseFind(' ');
		dest = strItem.Right(strItem.GetLength() - nLinkStart - 1);
		
		nLinkStart = strItem.Find("-> ");
		CString str = strItem;
		
		strItem = strItem.Left(nLinkStart);
		nLinkStart = strItem.ReverseFind(' ');
		strItem = strItem.Left(nLinkStart);		
		nLinkStart = strItem.ReverseFind(' ');
				
		link = strItem.Right(strItem.GetLength() - nLinkStart - 1);
		m_ftpLinkDestMap.SetAt(link, dest); // use link name to lookup dest
		
		strItem = str;
		nLinkStart = strItem.Find("-> ");
		
		strItem = strItem.Left(nLinkStart);
		nLinkStart = strItem.ReverseFind(' ');
		strItem = strItem.Left(nLinkStart);	
		nLinkStart = strItem.ReverseFind(' ');
		strItem = strItem.Left(nLinkStart + 1);
		
		strItem += link;				
	}
		
	if (strItem.Find("total") != -1)
		return FALSE;
	
	if (strItem.Find('.') != -1) // If the item contains a period, filename
		return TRUE;
	
	if (strItem.CompareNoCase(".unreadable") == 0)
		return FALSE;
	
	// TODO: Add more tests of strItem here
	
	// If we made it this far, return TRUE anyway (pszItem could contain
	// a folder name, but we can't be certain) for good measure.
	return TRUE;
}

void CFtpView::EmitShellExecuteError(HINSTANCE hInstance)
{
	switch ((UINT)hInstance) 
	{    
		case 2:
			MessageBeep(-1);
			AfxMessageBox(IDP_FILE_NOT_FOUND, MB_ICONSTOP);
			break;
		
		case 3:
			MessageBeep(-1);
			AfxMessageBox(IDP_PATH_NOT_FOUND, MB_ICONSTOP);
			break;
		
		case 5:
			MessageBeep(-1);
			AfxMessageBox(IDP_DYNLINKFAILED, MB_ICONSTOP);
			break;
		
		case 6:
			MessageBeep(-1);
			AfxMessageBox(IDP_SEPARATE_DATA_SEGS, MB_ICONSTOP);
			break;
		
		case 8:
			MessageBeep(-1);
			AfxMessageBox(IDP_INSUFFICIENT_MEMORY, MB_ICONSTOP);
			break;       
		
		default:
			MessageBeep(-1);
			AfxMessageBox("Unable to open or display file.", MB_ICONSTOP);
			break;
	}
	return;
}  

void CFtpView::EmitFileException(CFileException* pFileException)
{	
	CString msg = "";
	
	switch (pFileException->m_cause)
	{
		case CFileException::none:
			break;
		
		case CFileException::accessDenied:
			AfxFormatString1(msg, IDP_ACCESS_DENIED, m_strFilePath);
			MessageBeep(-1);
			AfxMessageBox(msg, MB_ICONSTOP|MB_OK, IDP_ACCESS_DENIED);
			break;
			
		case CFileException::badPath:
			AfxFormatString1(msg,
				AFX_IDP_FAILED_INVALID_PATH, m_strFilePath);
			MessageBeep(-1);
			AfxMessageBox(msg, 
				MB_ICONSTOP|MB_OK, AFX_IDP_FAILED_INVALID_PATH);
			break;
		
		case CFileException::diskFull:
			MessageBeep(-1);
			AfxMessageBox(AFX_IDP_FAILED_DISK_FULL);
			break;
		
		default:
			MessageBeep(-1);
			AfxMessageBox("An unspecified file I/O error occurred.",
				MB_OK|MB_ICONSTOP);
			break;
	}
	return;
}
	
void CFtpView::OnDownloadFile()
{
	CFileDownload theDialog(m_pApp->GetMainFrame());
	theDialog.m_strFileName = "";
	
	if (theDialog.DoModal() == IDOK)
	{
		m_strFileName = theDialog.m_strFileName;
		
		OnGet();
	}
}   

/////////////////////////////////////////////////////////////////////////////
// CFtpWnd

IMPLEMENT_DYNCREATE(CFtpWnd, CMDIChildWnd)

CFtpWnd::CFtpWnd()
{
}

CFtpWnd::~CFtpWnd()
{
}


BEGIN_MESSAGE_MAP(CFtpWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFtpWnd)
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFtpWnd message handlers

BOOL CFtpWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 60;
	windowRect.right = 583;
	windowRect.top = 57;
	windowRect.bottom = 345;
	 
	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);
	ASSERT(m_hMenuShared == NULL);      // only do once

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	// parent must be MDI Frame (or NULL for default)
	ASSERT(pParentWnd == NULL || pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	// will be a child of MDIClient
	ASSERT(!(dwDefaultStyle & WS_POPUP));
	dwDefaultStyle |= WS_CHILD;

	// if available - get MDI child menus from doc template
	ASSERT(m_hMenuShared == NULL);      // only do once
	CMultiDocTemplate* pTemplate;
	if (pContext != NULL &&
		(pTemplate = (CMultiDocTemplate*)pContext->m_pNewDocTemplate) != NULL)
	{
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)));
		// get shared menu from doc template
		m_hMenuShared = pTemplate->m_hMenuShared;
		m_hAccelTable = pTemplate->m_hAccelTable;
	}
	else
	{
		TRACE0("Warning: no shared menu/acceltable for MDI Child window\n");
			// if this happens, programmer must load these manually
	}

	CString strFullString = "", strTitle = "";
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(strTitle, strFullString, 0);    // first sub-string

	ASSERT(m_hWnd == NULL);
	if (!Create(GetIconWndClass(dwDefaultStyle, nIDResource),
	  strTitle, dwDefaultStyle, windowRect,
	  (CMDIFrameWnd*)pParentWnd, pContext))
	{
		return FALSE;	// will self destruct on failure normally
	}

	// it worked !
	return TRUE;
}

void CFtpWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	if (!bActivate)
	{
		CFtpView* pView = (CFtpView*)GetActiveView();
		pView->OnActivateView(FALSE, NULL, pView);
	}
	else
	{
		CFtpView* pView = (CFtpView*)GetActiveView();
		pView->OnActivateView(TRUE, pView, NULL);
	} 
	return;
}

BOOL CFtpWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	return CMDIChildWnd::PreCreateWindow(cs);
}
