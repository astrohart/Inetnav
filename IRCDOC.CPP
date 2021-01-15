// ircdoc.cpp : implementation of the CIRClientDoc class
//

#include "stdafx.h"
#include "inetnav.h"
#include "mainfrm.h"

#include "ircdoc.h"
#include "ircview.h" 
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIRClientDoc

IMPLEMENT_DYNCREATE(CIRClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CIRClientDoc, CDocument)
	//{{AFX_MSG_MAP(CIRClientDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_CHAT_CHANGE_GROUP, OnChatChangeGroup)
	ON_COMMAND(ID_CHAT_SEND_PRIVMSG, OnChatPrivateMessage)
	ON_COMMAND(ID_CHAT_CHANGE_NICKNAME, OnChatChangeNickname)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_CHAT_CHANGE_NICKNAME, OnUpdateChatChangeNickname)
	ON_UPDATE_COMMAND_UI(ID_CHAT_CHANGE_GROUP, OnUpdateChatChangeGroup)
	ON_UPDATE_COMMAND_UI(ID_CHAT_SEND_PRIVMSG, OnUpdateChatPrivateMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIRClientDoc construction/destruction

CIRClientDoc::CIRClientDoc()
{ 
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_strChannel ="";

	if (!m_pApp->GetInitialChannel().IsEmpty()) 
	{
		SetChannelName(m_pApp->GetInitialChannel());
	}
	else
		m_strChannel.Empty();

	// Initialize our pointer to the status bar
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_bChangeNick = FALSE;
}

CIRClientDoc::~CIRClientDoc()
{
	TRACE0("CIRClientDoc::~CIRClientDoc()\r\n");
	m_pApp = NULL;
	m_strChannel = "";
	m_pStatusBar = NULL;
	
	m_pServerView = NULL;
}

BOOL CIRClientDoc::OnNewDocument()
{ 
	if (!m_pApp->IsOnline())
	{
		AfxMessageBox(IDP_INETNAV_SIGN_ON, MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	if (m_pApp->IRCIsConnected())
	{
		CIRCJoinDlg theDialog(AfxGetMainWnd());
		if (m_strChannel.IsEmpty())
		{
			if (theDialog.DoModal() == IDOK)
			{
				if (theDialog.m_strChannel.IsEmpty()) 
				{
					return FALSE;
				}
				SetChannelName(theDialog.m_strChannel);		
			}
			else	// user pressed cancel
			{
				return FALSE;
			}
		}

		if (!CDocument::OnNewDocument()) 
		{
			AfxMessageBox("Unable to join IRC channel.");
			m_pStatusBar->SetText("Unable to join IRC channel.");
			delete m_pApp->GetIRCSocket();
			m_pApp->SetIRCSocket(NULL);        
			m_pStatusBar->ShowIdleMessage();
			return FALSE;              
		}
		
		TRACE0("CDocument::OnNewDocument() successful.\r\n");	
		if (m_pApp->GetIRCSocket()->IsConnected())
		{     
			CString strJoinCmd = "JOIN " + GetChannelName();
			strJoinCmd += "\r\n";
			m_pApp->GetIRCSocket()->Send(strJoinCmd);
		}
			
		{
			m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
			m_pStatusBar->SetText("Setting title for channel window...");
			
			m_bChangeNick = FALSE;
			char szTitle[128];
			sprintf(&szTitle[0], "%s on %s (%s)",
				(const char*)m_pApp->GetNickname(),
				(const char*)GetChannelName(),
				(const char*)m_pApp->GetIRCServerName());
			
			#ifdef _DEBUG
				afxDump << "Calling CIRClientDoc::SetTitle()...\r\n";
			#endif 
			SetTitle(&szTitle[0]); 
			
			m_pApp->SetIRCConnected(TRUE);
		}
		
		return TRUE;
	}    
	else // we're not yet connected
	{
		CString strMsg = "Internet Navigator is unable to connect to ";
		strMsg += m_pApp->GetIRCServerName() + ".  Please choose the ";
		strMsg += "Chat Center command from the Places menu again to ";
		strMsg += "attempt to re-connect to " + m_pApp->GetIRCServerName();
		strMsg += "."; 

		MessageBeep(-1);
		AfxMessageBox(strMsg);
		
		m_pApp->SetIRCConnected(FALSE);
		return FALSE;
	}
}

void CIRClientDoc::OnCloseDocument()
{
	// the user is closing the channel window they're using; so close the
	// whole document and tell the server we want to leave the channel we're
	// joined to
	CString strPartCmd = "", strText = "";
	strText = "Now leaving channel " + GetChannelName() + ".";
	m_pStatusBar->SetText(strText);
	
	strPartCmd = "PART ";
	strPartCmd += GetChannelName() + "\r\n";
	m_pApp->GetIRCSocket()->Send(strPartCmd);
	m_pApp->RemoveChanWindow(GetChannelName()); 
	
	if (m_pApp->GetIRCChanList()->IsEmpty())
	{
		m_pApp->SetIRCConnected(FALSE);
		m_pApp->GetIRCSocket()->Send("QUIT\r\n");
		m_pApp->GetIRCSocket()->Disconnect();
	}
	
	CString title = "Private Message From OnlineHost";
	CString text = "Thank you for chatting on channel " + GetChannelName();
	text += "!\r\n\r\nI will inform everybody that you have left this ";
	text += "channel.  Please come back again soon and thank you for ";
	text += "using IRC and EFnet!";   
	
	// Now show the user this goodbye-from-IRC 'psuedo private message' from
	// the pseudo-user OnlineHost (which represents the server). 
	m_pStatusBar->SetText("A private message has arrived for you!");
	
	MessageBeep(-1);
	AfxGetMainWnd()->MessageBox(text, title, MB_ICONASTERISK|MB_OK);
	
	m_pStatusBar->ShowIdleMessage();
	
	CDocument::OnCloseDocument();    
	
	return;
}

BOOL CIRClientDoc::SaveModified()
{
	if (!IsModified())
		return TRUE;        // ok to continue

	CString strName = "the transcript of your IRC conversation";

	CString strPrompt = "";
	AfxFormatString1(strPrompt, AFX_IDP_ASK_TO_SAVE, strName);
	switch (AfxMessageBox(strPrompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (_access(m_strPathName, 6) != 0)
		{
			if (!CDocument::DoSave(NULL))
				return FALSE;   // don't continue
		}
		else
		{
			if (!CDocument::DoSave(m_strPathName))
				return FALSE;   // don't continue
		}
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		// don't do nothin' here
		break;
	}
	return TRUE;    // keep going
}

/////////////////////////////////////////////////////////////////////////////
// CIRClientDoc serialization

void CIRClientDoc::Serialize(CArchive& ar)
{ 
	m_pServerView->SerializeRaw(ar);
	
	if (ar.IsStoring())
	{
		// serialize the title of this document into the *.IRC file
		ar << m_strWindowTitle;
	}
	else // ar.IsLoading()
	{   
		// deserialize the title of this document from the *.IRC file
		ar >> m_strWindowTitle;
	}

	return;	
}

void CIRClientDoc::DeleteContents()
{
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL) return;
	
	while (pos != NULL)
	{
		CEditView* pEditView = 
			(CEditView*)GetNextView(pos);
		pEditView->DeleteContents();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIRClientDoc diagnostics

#ifdef _DEBUG
void CIRClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIRClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIRClientDoc commands

void CIRClientDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// Only enable the File, Save menu item if the document data has
	// changed
	pCmdUI->Enable(IsModified());
	return;
}

void CIRClientDoc::SetTitle(const char* pszTitle)
{                               
	CString strTitle = pszTitle;
	CString strTitleTranscript = strTitle;
	
	if ((m_pServerView != NULL) && (!m_bChangeNick))
		m_pApp->AddChanWindow(m_pServerView, GetChannelName());
		
	m_strTitle = strTitleTranscript;
	UpdateFrameCounts();
	m_strWindowTitle = strTitleTranscript;
	
	return;
} 

void CIRClientDoc::OnChangedViewList()
{
	CDocument::OnChangedViewList();
	static BOOL bFirst = TRUE;
	
	if (bFirst)
	{
		POSITION pos = GetFirstViewPosition();
		m_pServerView = (CIRClientView*)GetNextView(pos);
		bFirst = FALSE;
	}
	return;
}

void CIRClientDoc::OnChatChangeGroup()
{
	CIRCJoinDlg theDialog(m_pApp->GetMainFrame());
	theDialog.m_strChannel = GetChannelName();
	
	if (theDialog.DoModal() == IDOK)
	{
		/*wait.Restore();*/
		CString strJoinCmd = "";
		strJoinCmd = "PART " + GetChannelName() + "\r\n";
		m_pApp->GetIRCSocket()->Send(strJoinCmd);
		
		// we've PARTed with the old channel; now let's JOIN the new one
		strJoinCmd = "JOIN " + theDialog.m_strChannel + "\r\n";
		m_pApp->GetIRCSocket()->Send(strJoinCmd);
		
		// make sure the window displaying the old channel gets changed
		// around so the app knows its now going to display the new
		// channel
		if (!m_pApp->ChangeChanWindow(GetChannelName(),
								theDialog.m_strChannel))
		{
			AfxMessageBox("Failed to change channel.");
			return;
		}
		
		m_bChangeNick = TRUE;
		SetChannelName(theDialog.m_strChannel);
		{
			char szTitle[128];
			sprintf(&szTitle[0], "%s on %s (%s)",
				(const char*)m_pApp->GetNickname(),
				(const char*)GetChannelName(),
				(const char*)m_pApp->GetIRCServerName());
			SetTitle(&szTitle[0]);
		}
	}
}

void CIRClientDoc::OnChatPrivateMessage()
{
	// Show a dialog box to allow the user to type who they want to send
	// a private message to, then send the message to the recipient
	CIRCMessageDialog theDialog(m_pApp->GetMainFrame());
	
	// make sure the user is connected
	if (!m_pApp->IRCIsConnected())
	{
		#ifdef _DEBUG
			afxDump << "Can't send private message -- user isn't ";
			afxDump << "connected to " << m_pApp->GetIRCServerName();
			afxDump << "\r\n";
		#endif
		return;
	}
	
	if (theDialog.DoModal() == IDOK)	// user pressed OK
	{
		/*wait.Restore();*/
		if (!theDialog.m_strRecipient.IsEmpty())
		{
			CString strMsg = "PRIVMSG " + theDialog.m_strRecipient;
			strMsg += " :" + theDialog.m_strMessage + "\r\n";
			m_pApp->GetIRCSocket()->Send(strMsg);
		}
		return;
	}
	else	// user pressed Cancel
		return;
	return;	
}

void CIRClientDoc::OnChatChangeNickname()
{
	// Show a dialog box asking the user for the new nickname they want to
	// use, then ask the server to use the new nickname to represent the
	// user
	
	if (!m_pApp->IRCIsConnected())
		return;
	
	CIRCNicknameDialog theDialog(m_pApp->GetMainFrame());
	theDialog.m_strNewNickname = m_pApp->GetNickname();
	
	if (theDialog.DoModal() == IDOK) // user pressed OK button
	{
		m_pApp->SetNickname(theDialog.m_strNewNickname);
		
		CString strNickCmd = "NICK " + m_pApp->GetNickname() + "\r\n";
		
		m_bChangeNick = TRUE;
		m_pApp->GetIRCSocket()->Send(strNickCmd);  
		{
			char szTitle[128];
			sprintf(&szTitle[0], "%s on %s (%s)",
				m_pApp->GetNickname(),
				GetChannelName(),
				m_pApp->GetIRCServerName());
			SetTitle(&szTitle[0]);
		}
		return;
	}
	else // user pressed Cancel
		return; // don't do anything if the user doesn't want to change
				// his/her nickname
}

void CIRClientDoc::OnFileClose()
{
	CString strText = "", strMsg = "", strCaption = "";
	
	strCaption = "Chat Center";
	strMsg = "Disconnected from " + m_pApp->GetIRCServerName() + ".";
	
	::MessageBox(m_pApp->GetMainFrame()->GetSafeHwnd(), strMsg, strCaption, 
			MB_ICONASTERISK|MB_OK);    
			
	strText = strMsg;
	m_pStatusBar->SetText(strText);
	
	m_pApp->SetIRCConnected(FALSE);
	m_strChannel = "";
	
	// call base class handler
	CDocument::OnFileClose();
}

void CIRClientDoc::PrintChatTranscript()
{	
	m_pServerView->OnPrintTranscript();
}

void CIRClientDoc::OnUpdateChatChangeNickname(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pApp->IRCIsConnected());
}

void CIRClientDoc::OnUpdateChatChangeGroup(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pApp->IRCIsConnected());
}

void CIRClientDoc::OnUpdateChatPrivateMessage(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pApp->IRCIsConnected());
}
