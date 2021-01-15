// newsdoc.cpp: implementation file
//

#include "stdafx.h"
#include "inetnav.h"
#include "percent.h" 
#include "dialogs.h"   

#include <stdio.h>

#include "resource.h"
#include "propshts.h" 

#include "mail.h" 
#include "news.h"  

#include "wizards.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewsDoc

IMPLEMENT_DYNCREATE(CNewsDoc, CDocument)

BEGIN_MESSAGE_MAP(CNewsDoc, CDocument)
	//{{AFX_MSG_MAP(CNewsDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_NEWSGROUPS_SETUP, OnNewsgroupsSetup)
	ON_COMMAND(ID_ARTICLE_ADD, OnArticleAdd)
	ON_UPDATE_COMMAND_UI(ID_ARTICLE_ADD, OnUpdateArticleAdd)
	ON_COMMAND(ID_ARTICLE_SIGNATURE, OnArticleSignature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewsDoc construction/destruction

CNewsDoc::CNewsDoc()
{    
	m_bInitialized = FALSE;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pSocket = NULL; 
	
	m_bCanPost = TRUE;    
	m_bArticleFlashConnect = FALSE;

	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		m_pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'N'
		&& m_pApp->m_lpCmdLine[2] == 'W' && m_pApp->m_lpCmdLine[3] == 'S')
	{
		if (m_pApp->IsWindows95())
			m_pParent = CWnd::GetDesktopWindow();
		else
			m_pParent = CWnd::GetActiveWindow();
	}
	else
		m_pParent = AfxGetMainWnd();
}

CNewsDoc::~CNewsDoc()
{  
	CInetnavStatusBar* pSB = m_pApp->GetMainFrame()->GetStatusBar();
	
	if (m_pSocket != NULL && !m_bArticleFlashConnect)
	{
		if (IsSocketAvailable() && m_pSocket->IsConnected()
				&& m_pApp->IsOnline() && !m_bArticleFlashConnect)
		{ 
			if (m_pSocket->IsBlocking())
			{
				// A blocking call, like possibly GetLine(), is in progress
				// so let's cancel it
				m_pSocket->CancelBlockingCall();
			}
			
			pSB->SetText("Disconnecting from newsgroups server...");
			m_pSocket->Send("QUIT\r\n");
			pSB->ShowIdleMessage(); 
        }

        
        if (m_pSocket->IsConnected())	
			m_pSocket->Disconnect();

		delete m_pSocket;
		m_pSocket = NULL;
	}
	
	DeleteContents();
}

BOOL CNewsDoc::OnNewDocument()
{
	m_pApp->Serialize(FALSE);

	if (!CDocument::OnNewDocument())
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC, MB_ICONSTOP|MB_OK);
		return FALSE;                                                  
	}
    
    m_pApp->Serialize(FALSE);
    m_pApp->Serialize(TRUE); 
    
    m_pApp->LoadMailboxes();
    
	m_strNNTPHost = m_pApp->m_strNNTPHost; 
	m_strOldNNTPHost = m_strNNTPHost;
	m_strEmailAddress = m_pApp->m_strDefAddress;
	
	if (m_strEmailAddress.IsEmpty())
	{
		MessageBeep(-1);
		AfxMessageBox("Unable to set up Internet Newsgroups.");
		return FALSE;
	}
                                  
	m_strRevNNTPHost = m_strNNTPHost;
	                                 
	DoNewsgroupsSetup();	// Starts the Newsgroups Setup Wizard
	
	if (m_bInitialized)
	{
		// Set the path name and save this file
		m_strPathName = m_pApp->GetFilesDirectory() + "INTERNET.NWS";
		if (!CDocument::DoSave(m_strPathName, TRUE))
		{
			MessageBeep(-1);
			AfxMessageBox(AFX_IDP_FAILED_TO_SAVE_DOC, MB_ICONSTOP);
			return FALSE;
		}
		
		// Now let the user set Article Options
		m_pApp->Serialize(FALSE);
		CArticleOptions theDialog(m_pParent);	
		theDialog.m_nDisplay = m_pApp->GetNNTPArticleDisplay();
		
		if (theDialog.DoModal() == IDOK)
			m_pApp->SetNNTPArticleDisplay(theDialog.m_nDisplay);
		m_pApp->Serialize(TRUE);
	}
    else               
    {
    	/*wait.Restore();*/
    	return FALSE;  
    }
	
	SetTitle("Internet Newsgroups");
	
	// Now prompt the user to save their changes to a file  
	/*wait.Restore();*/

	return ConnectToNewsServer();
}

BOOL CNewsDoc::OnOpenDocument(const char* pszPathName)
{
	if (!CDocument::OnOpenDocument(pszPathName))
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC, MB_ICONSTOP|MB_OK);
		return FALSE;
	}
	
	m_pApp->Serialize(FALSE);
	m_strNNTPHost = m_pApp->m_strNNTPHost;
	m_strOldNNTPHost = m_strNNTPHost;

	// If the application's default mail address has changed, let's make
	// sure we have the current copy	
	m_strEmailAddress = m_pApp->m_strDefAddress;
			
	m_pApp->Serialize(TRUE);
	
	/*wait.Restore();*/
	SetTitle("Internet Newsgroups");
	/*wait.Restore();*/
	
	return ConnectToNewsServer();
}

BOOL CNewsDoc::OnSaveDocument(const char* pszPathName)
{
	// First, call the base class CDocument::OnSaveDocument().
	if (!CDocument::OnSaveDocument(pszPathName))
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	
	/*wait.Restore();*/
	SetTitle("Internet Newsgroups");
	/*wait.Restore();*/
	
	// If the news server the user set for this area has changed, then
	// set the app's copy to the new setting so that this area loads the
	// right address the next time the user goes to the Newsgroups area
	m_pApp->m_strNNTPHost = m_strNNTPHost;
	
	// Now save the application's new m_strNNTPHost setting to the INI file
	m_pApp->Serialize(TRUE);
	
	// Now, write the path name to the Internet Navigator INI file
	/*wait.Restore();*/
	m_pApp->WriteProfileString("Internet Newsgroups", "AreaFile", pszPathName);
	/*wait.Restore();*/
	
	// we're done and we made it this far, so we can tell the caller of this
	// function that we were successful by returning TRUE.
	return TRUE;
}
	
/////////////////////////////////////////////////////////////////////////////
// CNewsDoc serialization
               
void CNewsDoc::Serialize(CArchive& ar)
{
	// Serialize the list of the user's subscribed groups
	m_strGroupsArray.Serialize(ar);
	
	if (ar.IsStoring())
	{ 
		ar << m_strNNTPHost; 
		ar << m_strOldNNTPHost;
		ar << m_strEmailAddress;
		
		ar << (BYTE)m_bCanPost;
	}
	else
	{
		ar >> m_strNNTPHost;    
		ar >> m_strOldNNTPHost;
		ar >> m_strEmailAddress;
		
		BYTE byte;
		ar >> byte;
		m_bCanPost = byte;
	}
}

void CNewsDoc::DeleteContents()
{
	m_strNNTPHost = m_strOldNNTPHost = m_strEmailAddress = "";

	// Remove all elements of the subscribed groups array -- if there
	// are any
	if (m_strGroupsArray.GetSize() != 0)
	{
		m_strGroupsArray.FreeExtra(); 
		m_strGroupsArray.RemoveAll();
	}
	
	// Remove all elements, if necessary, of the other arrays in this 
	// document
	DeleteArrays();
	
	return;
}

void CNewsDoc::DoNewsgroupsSetup()
{
	CNewsSetupWiz1 wiz(m_pParent);
	
	switch (wiz.DoModal())
	{
		case IDOK:
			DoNewsWiz2();
			break;
		
		case IDCANCEL:
			m_bInitialized = FALSE;
			break;
	}
}

void CNewsDoc::DoNewsWiz2()
{
	CNewsSetupWiz2 wiz(m_pParent); 
	wiz.m_nChoice = 0;
	
	switch (wiz.DoModal())
	{
		case IDOK:		// Next
			switch (wiz.m_nChoice)
			{
				case 0:
					DoNewsWiz3();
					break;
				
				case 1:
					CNewsgroupsSetup theDialog(m_pParent);
					theDialog.m_strNNTPHost = m_strNNTPHost;
					m_strOldNNTPHost = theDialog.m_strNNTPHost;
					
					if (theDialog.DoModal() == IDOK)
					{
						// save settings from dialog box 
						m_bInitialized = TRUE;
						m_strNNTPHost = theDialog.m_strNNTPHost;
						m_pApp->m_strNNTPHost = m_strNNTPHost;
						m_pApp->Serialize(TRUE);
				
						if (m_strNNTPHost != m_strOldNNTPHost)
						{
							// set the modified flag
							SetModifiedFlag();
						}
						
						if (m_strNNTPHost != m_strOldNNTPHost)
						{          
							// The user wants to connect to a different news server --
							// make the attempt
							ChangeNewsServer();
						}
					}
					break;
			}
			break;     
		
		case IDRETRY:	// Back
			DoNewsgroupsSetup();
			break;
		
		case IDCANCEL:	// Cancel
			m_bInitialized = FALSE;
			break;
	}
}	 

void CNewsDoc::DoNewsWiz3()
{
	CNewsSetupWiz3 wiz(m_pParent);
	wiz.m_strServer = m_strRevNNTPHost;
	
	switch(wiz.DoModal())
	{
		case IDOK:		// Next
			m_strRevNNTPHost = wiz.m_strServer;
			DoNewsWiz4();
			break;
		
		case IDRETRY:	// Back
			m_strRevNNTPHost = wiz.m_strServer;
			DoNewsWiz2();
			break;  
		
		case IDCANCEL:	// Cancel
			m_bInitialized = FALSE;
			break;
	}
}

void CNewsDoc::DoNewsWiz4()
{
	CNewsSetupWiz4 wiz(m_pParent);
	
	switch (wiz.DoModal())
	{
		case IDOK:		// Finish
			m_strNNTPHost = m_strRevNNTPHost;
			if (m_strOldNNTPHost != m_strNNTPHost)
				SetModifiedFlag();
			if (m_strOldNNTPHost != m_strNNTPHost)
				ChangeNewsServer(); 
			m_pApp->m_strNNTPHost = m_strNNTPHost;
			m_pApp->Serialize(TRUE);
			m_bInitialized = TRUE;
			break;
		
		case IDRETRY:	// Back     
			DoNewsWiz3();
			break;
		
		case IDCANCEL:   
			m_bInitialized = FALSE;
			break;
	}
}
			
			
		
void CNewsDoc::DeleteArrays()
{
	if (m_strMessageIDs.GetSize() > 0)
	{
		m_strMessageIDs.FreeExtra();
		m_strMessageIDs.RemoveAll();
	}
	
	if (m_strSubjectList.GetSize() > 0)
	{
		m_strSubjectList.FreeExtra();
		m_strSubjectList.RemoveAll();
	}
	
	if (m_strSenderList.GetSize() > 0)
	{
		m_strSenderList.FreeExtra();
		m_strSenderList.RemoveAll();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNewsDoc diagnostics

#ifdef _DEBUG
void CNewsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNewsDoc::Dump(CDumpContext& dc) const
{
	dc << "m_strNNTPHost: " << m_strNNTPHost << "\r\n";
	dc << "m_strOldNNTPHost: " << m_strOldNNTPHost << "\r\n";
	dc << "m_strEmailAddress: " << m_strEmailAddress << "\r\n";
	dc << "m_bCanPost: ";
	m_bCanPost ? dc << "TRUE" : dc << "FALSE";
	dc << "\r\n";
		
	CDocument::Dump(dc);
	dc << "\r\n"; // insert CRLF to be sure
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNewsDoc communications

QSocket* CNewsDoc::GetSocket()
{
	if (IsSocketAvailable())
	{
		// give the caller this document's socket!
		// And since someone now has the socket, set
		// m_bSocketAvailable to FALSE
		
		SetSocketAvailable(FALSE); 
		return m_pSocket;
	}
	else
	{
		// A function is already using this socket so deny
		// the caller's rights to the socket pointer
		return NULL;	
	}
	return NULL;	
} 

void CNewsDoc::ReleaseSocket()
{
	// Allow callers of GetSocket() access to the socket pointer
	SetSocketAvailable(TRUE);       
	
	// Undo any QSocket::SetReceiveTarget() calls which may have been made.
	// And put the socket back into QSocket::GetLine() mode
	m_pSocket->SetReceiveTarget(NULL, 0);
	return;
}

BOOL CNewsDoc::ConnectToNewsServer()
{
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
		return TRUE;
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'N'
		&& m_pApp->m_lpCmdLine[2] == 'W' && m_pApp->m_lpCmdLine[3] == 'S')
		return TRUE;

	// If the user is not online, let the document be opened -- news articles
	// can be composed offline and the communications are managed with
	// FlashConnects
	if (!m_pApp->IsOnline())
	{
		// Allow user offline access to this area.
		return TRUE;
	}
		
	// First, get a pointer to the status bar so we can inform the user
	// of our progress
	CInetnavStatusBar* pSB = m_pApp->GetMainFrame()->GetStatusBar();
	
	delete m_pSocket;
	SetSocketAvailable(FALSE); // socket's not avilable until we're connected

	m_pSocket = new QSocket(); 
	
	if (m_pSocket == NULL) 
	{
		delete m_pSocket;
		m_pSocket = NULL;
		
		return FALSE;
	}
	
	/*wait.Restore();*/
	CString strText = "Connecting to news server...  Please wait.";
	pSB->SetText(strText);
	/*wait.Restore();*/
	
	if (m_pSocket->Connect(m_strNNTPHost, 119))
	{
		/*wait.Restore();*/
		while (m_pSocket->IsConnecting())
		{
			// pump messages until we're connected
			for (;;)
			{
			 	/*wait.Restore();*/
			 	pSB->SetText(strText);
			 	MSG msg; // a message data structure
			 	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			 	{    
			 		if (!m_pApp->PreTranslateMessage(&msg))
			 		{   
			 			// message needs further processing
			 			::TranslateMessage(&msg);
			 			::DispatchMessage(&msg);
			 		}
			 	}
			 	else
			 	{
			 		m_pApp->OnIdle(0); // user interface update
			 		m_pApp->OnIdle(1); // frees temporary objects
			 		break;
			 	}
			 	/*wait.Restore();*/
			}	               
			/*wait.Restore();*/
		}
		
		if (m_pSocket->IsConnected())
		{ 
			pSB->SetText("Connected to news server.");  
			SetSocketAvailable(TRUE);
			CString strResponse = m_pSocket->GetLine();
			BOOL bCanPost = m_bCanPost;
		    
		    
			if (atoi(strResponse.Left(3)) == 200)
			{
				// A response code of 200 indicates that the user may
				// post to newsgroups on this server
				m_bCanPost = TRUE;
				if (bCanPost != m_bCanPost)  
				{
					// Set the document as changed and save it
					SetModifiedFlag();
				}
			}
			else if (atoi(strResponse.Left(3)) == 201)
			{   
				// A response code of 201 indicates that the user may
				// not post to newsgroups on this server
				m_bCanPost = FALSE;
				if (bCanPost != m_bCanPost)  
				{
					// Set the document as changed and save it
					SetModifiedFlag();
				}
			}			
		}
	}
	
	if (!IsSocketAvailable())
	{
		strText = "Connection attempt failed.";  
		pSB->SetText(strText);
		AfxMessageBox(IDP_NOCONNECT_NNTP_HOST, MB_ICONSTOP);
		pSB->ShowIdleMessage();
		return FALSE;	
	}
	else
	{
		// we've connected successfully!!
		pSB->ShowIdleMessage();
	    return TRUE;
	}
	
	return FALSE;			
}

// a function for changing the news server we're connected to if the user
// changes the m_strNNTPHost address.
void CNewsDoc::ChangeNewsServer()
{
	if (!m_pApp->IsOnline())
	{
		// Just return -- the user isn't online and therefore the program
		// will crash if we do sockets stuff now.
		return;
	}

	CInetnavStatusBar* pSB = m_pApp->GetMainFrame()->GetStatusBar();
	pSB->SetText("Changing to new news server...  Please wait.");

	// verify that the new server address IS different from the old
	if (m_strNNTPHost == m_strOldNNTPHost)
	{ 
		pSB->ShowIdleMessage();
		return;
	}
	
	// Now, disconnect from the current news server 
	pSB->SetText("Disconnecting from old news server...");
	m_pSocket->Send("QUIT\r\n");
	CString strResponse = m_pSocket->GetLine();
	pSB->SetText("Disconnected from " + m_strOldNNTPHost + ".");

	// connect to new news server with the CNewsDoc::ConnectToNewsServer()
	// function
	ReleaseSocket();	

	delete m_pSocket;
	m_pSocket = NULL;
	
	ConnectToNewsServer();
	
	return;
}

void CNewsDoc::DisconnectFromNewsServer()
{
	CInetnavStatusBar* pSB = m_pApp->GetMainFrame()->GetStatusBar();

	// Now, disconnect from the current news server 
	pSB->SetText("Disconnecting from news server...  Please wait.");
	m_pSocket->Send("QUIT\r\n");
	CString strResponse = m_pSocket->GetLine();
	pSB->SetText("Disconnected from newsgroups server.");

	delete m_pSocket;
	m_pSocket = NULL;
}	
			
/////////////////////////////////////////////////////////////////////////////
// CNewsDoc commands

void CNewsDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsModified());
}

void CNewsDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CNewsDoc::OnNewsgroupsSetup()
{
	DoNewsgroupsSetup();
	
	if (!CDocument::DoSave(m_strPathName, TRUE))
	{
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_FAILED_TO_SAVE_DOC, MB_ICONSTOP);
		return;
	}
	
	return;
}

void CNewsDoc::SetTitle(const char* pszTitle)
{
	m_strTitle = "Internet Newsgroups"; // <- title will ALWAYS be this
	UpdateFrameCounts();        // will cause name change in views
}	

void CNewsDoc::OnArticleAdd()
{
	CArticleComposeWnd* pFrame = 
		(CArticleComposeWnd*)m_pApp->GetCNA()->CreateNewFrame(this, NULL);
	if (pFrame == NULL)
	{
		AfxMessageBox("Unable to open the Compose News Article window.",
			MB_ICONSTOP|MB_OK);
		return;         
	}
	m_pApp->GetCNA()->InitialUpdateFrame(pFrame, this, TRUE);
}

void CNewsDoc::OnUpdateArticleAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanPost());
}

void CNewsDoc::DisplayNNTPError(int nResponseCode)
{
	// This function, when given an integer server response code, displays
	// the corresponding error message to the user.  With message boxes
	// which have prompt IDs, this also makes it easy for us to provide
	// the user with F1 help on these errors.
	
	switch (nResponseCode)
	{ 
		case 400:
			MessageBeep(-1);
			AfxMessageBox(IDP_NNTP_ERROR_400, MB_ICONSTOP);
			break;
			
		case 412:
			MessageBeep(-1);
			AfxMessageBox(IDP_NNTP_ERROR_412, MB_ICONSTOP);
			break;
	
		case 423:
			MessageBeep(-1);
			AfxMessageBox(IDP_NNTP_ERROR_423, MB_ICONSTOP);
			break;
					
		case 430:
			MessageBeep(-1);
			AfxMessageBox(IDP_NNTP_ERROR_430, MB_ICONSTOP);
			break;
        
        case 500:
        	MessageBeep(-1);
        	AfxMessageBox(IDP_NNTP_ERROR_500, MB_ICONSTOP); 
        	break;
        	
        case 501:
        	MessageBeep(-1);
        	AfxMessageBox(IDP_NNTP_ERROR_501, MB_ICONSTOP);
        	break;
        
        case 502:
        	MessageBeep(-1);
        	AfxMessageBox(IDP_NNTP_ERROR_502, MB_ICONSTOP);
        	break;
        
        case 503:
        	MessageBeep(-1);
        	AfxMessageBox(IDP_NNTP_ERROR_503, MB_ICONSTOP);
        	break;
	}   
}

void CNewsDoc::OnArticleSignature()
{
	return;
} 

