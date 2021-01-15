// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "inetnav.h"

#include "dialogs.h"

#include "mainfrm.h" 

#include "ircerrs.h" 
#include "ircview.h"  
#include "ircwnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OFFLINE, OnUpdateIndicatorOffline)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_HELP_SEARCH, OnHelpSearch)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_INDEX, CMDIFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CMDIFrameWnd::OnHelpUsing)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, OnDefaultHelp)
	// IRC Socket Response message -- for communications
	ON_MESSAGE(WM_IRC_SOCKET_RESPONSE, OnIRCSocketResponse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_NEW_TEXT,    // creates a new text file document
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR, 
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_EDIT_UNDO,
	ID_EDIT_REDO,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
		ID_SEPARATOR, 
	ID_FILE_NEW,
	ID_PLACES_PREFERENCES,
	ID_PLACES_POST_OFFICE,
		ID_SEPARATOR,
	ID_PLACES_FINGER,
	ID_PLACES_WHOIS,
	ID_PLACES_FTP_CLIENT,
		ID_SEPARATOR,
	ID_PLACES_CHAT_CENTER,
	ID_PLACES_NEWSGROUPS,
	ID_PLACES_GOPHER_CENTER,
		ID_SEPARATOR,
	ID_HELP_INDEX,
	ID_CONTEXT_HELP,  
		ID_SEPARATOR,  
	ID_TIP_OF_DAY,
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator  
	ID_INDICATOR_OFFLINE,   // tells user whether they're offline or online
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

CMainFrame::~CMainFrame()
{   
	m_pApp = NULL;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    /*wait.Restore();*/
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons, sizeof(buttons)/sizeof(UINT)))
	{   
		#ifdef _DEBUG
		TRACE0("CMainFrame::OnCreate() failed to create Places toolbar\r\n");
		#endif
	
		#ifdef NDEBUG
			AfxMessageBox("Failed to initialize Places toolbar.",
				MB_ICONINFORMATION|MB_OK);
		#endif
		
		return -1;      // fail to create
	}

	/*wait.Restore();*/ 
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
	  	sizeof(indicators)/sizeof(UINT)))
	{      
		#ifdef _DEBUG
		TRACE0("CMainFrame::OnCrate() failed to create status bar.\r\n");
		#endif
		
		#ifdef NDEBUG
			AfxMessageBox("Failed to initialize status bar.",
				MB_ICONINFORMATION|MB_OK);
		#endif
		
		return -1;      // fail to create
	}
			
	/*wait.Restore();*/

	UINT nID, nStyle;
	int cxWidth;
		
	m_wndStatusBar.ShowIdleMessage();
	/*wait.Restore();*/
	m_wndStatusBar.GetPaneInfo(0, nID, nStyle, cxWidth);
	nStyle = SBPS_STRETCH|SBPS_NORMAL;
	m_wndStatusBar.SetPaneInfo(0, nID, nStyle, cxWidth);
	/*wait.Restore();*/    
		
	RecalcLayout();
	
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		#ifdef _DEBUG
			TRACE0("Failed to create main window.\r\n");
		#endif
		
		#ifdef NDEBUG
			AfxMessageBox("Failed to initialize main window.",
				MB_ICONINFORMATION|MB_OK);
		#endif
		
		return -1;
	}
    
	m_pApp = (CInternetNavApp*)AfxGetApp();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnSysColorChange()
{
	CMDIFrameWnd::OnSysColorChange();
	
	if (m_pApp->IsUseCTL3D())
		Ctl3dColorChange();     
}

void CMainFrame::OnClose()
{                    
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnWindowCloseAll()
{ 
	// Close all visible windows -- this can be done with a call to
	// CWinApp::CloseAllDocuments(), since each of the windows
	// in Internet Navigator that are nonmodal are frame windows
	// attached to views and documents
	
	// Basically, this function is a duplicate of the CFrameWnd::OnClose()
	// function (which handles the WM_CLOSE message) with one exception --
	// this function DOES NOT close the frame window
	
	// Save any modified documents first
	if (!m_pApp->SaveAllModified())
		return;
	
	/*wait.Restore();*/
	
	// Now close each document-view-frame combination
	m_pApp->CloseAllDocuments(FALSE/*(we don't want to exit)*/);
	/*wait.Restore();*/
	return; 
}

void CMainFrame::OnUpdateIndicatorOffline(CCmdUI* pCmdUI)
{
	if (m_pApp->IsOnline())
	{
		// set this indicator's text to say, "Online"
		pCmdUI->SetText("Online");
		pCmdUI->Enable(TRUE);
		return;
	}
	else if (!m_pApp->IsOnline())
	{
		// set this indicator's text to say, "Offline"
		pCmdUI->SetText("Offline"); 
		pCmdUI->Enable(TRUE);
		return;
	}
	else
		return;
}    

void CMainFrame::ResetToolBar()
{
	if (!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons, sizeof(buttons)/sizeof(UINT)))
	{
		#ifdef _DEBUG
		TRACE0("CMainFrame::ResetToolBar failed to display Places toolbar\n");
		#endif
		
		#ifdef NDEBUG
			AfxMessageBox("Unable to display main toolbar.");
		#endif
	
		return;
	}
	
	return;
}

// for Internet Relay Chat
void CMainFrame::AddToReceive(const char* pszBuffer, int nAmt)
{
	char temp[SOCK_BLOCK_SIZE+1];
	
	int cur_pos = 0;
	while (cur_pos < nAmt)
	{
		char *lf = strchr(pszBuffer+cur_pos, '\n');
		int length;
					
		if (lf)
		{
			length = lf-pszBuffer-cur_pos+1;
		}
		else
		{
			length = nAmt - cur_pos;
		}
					
		strncpy(temp, pszBuffer+cur_pos, length);
		temp[length] = 0;            
		
		if (lf)
		{				                  
			// we have a linefeed so add string
			if (m_strRemainingReceive.IsEmpty())
			{
				m_ReceiveLines.AddTail(temp);
				IRCParseResponse();
			}                            
			else
			{
				m_ReceiveLines.AddTail(m_strRemainingReceive+temp);
				m_strRemainingReceive.Empty();
			}
 		}
 		else
 		{
 			// we don't have a linefeed so add to m_strRemainingReceive
 			// for future use
 			m_strRemainingReceive += temp;
 		}
 		cur_pos += length;
	}
}

LRESULT CMainFrame::OnIRCSocketResponse(WPARAM amount, LPARAM buffer)
{
	if ((int)amount > 0)
	{
		char* p = (char*)buffer;
		
		p[amount] = '\0';
		AddToReceive(p, amount);
		IRCParseResponse();	
	}
	else                          
	{
		// if amount < 0 it is a receive command
		switch ( (SocketReceiveCmd)amount )
		{
			case SocketStatusChanged:
				switch (m_pApp->GetIRCSocket()->GetStatus())
				{  
					case CONNECTING:
						break;		// we ignore status CONNECTING
											
					case CONNECTED:
						IRCBeginChat();	// begin this chat 
						break;
						
					case DISCONNECTED:
						m_wndStatusBar.ShowIdleMessage();
						break;
					
					case TIMEDOUT:
						m_wndStatusBar.ShowIdleMessage();
						delete m_pApp->GetIRCSocket();
						break;
							
					default: 
						m_wndStatusBar.ShowIdleMessage(); 
						// all other status states represent an error
						break;
				}
				break;
		}
	}
	return 0;
}

void CMainFrame::IRCBeginChat()
{
	m_wndStatusBar.SetText("Connected to chat server.");
	CString tempCmd = "";
	char hostname[128];
	int hSocket = 0;
	
	m_wndStatusBar.SetText("Signing on to EFNet...  Please wait.");
	
	tempCmd = "NICK " + m_pApp->GetNickname() + "\n\r";
	m_pApp->GetIRCSocket()->Send(tempCmd);
	tempCmd = "";
							
	tempCmd = "USER pcuser ";
	
	hSocket = gethostname(&hostname[0], 128);
	if (hSocket == SOCKET_ERROR)
	{
		AfxMessageBox("Sorry, unable to register you with the chat server!",
				MB_ICONSTOP|MB_OK);       
		m_wndStatusBar.ShowIdleMessage();
		
		m_pApp->GetIRCSocket()->Disconnect();
		return;
	}
		
	tempCmd += hostname;
	tempCmd += " ignore :" + m_pApp->GetUserName() + "\n\r";
	m_pApp->GetIRCSocket()->Send(tempCmd); 
	tempCmd = "";
							
	tempCmd = "JOIN " + m_pApp->GetInitialChannel();
	tempCmd += "\n\r";
	m_pApp->GetIRCSocket()->Send(tempCmd);
	tempCmd = "";
							
	tempCmd = "OnlineHost: ";
	tempCmd += "Welcome to " + m_pApp->GetInitialChannel();
	tempCmd += "!";
	PutInActive(tempCmd);
	tempCmd = "OnlineHost: ";
	tempCmd += m_pApp->GetNickname() + " has joined ";
	tempCmd += "channel " + m_pApp->GetInitialChannel();
	tempCmd += ".";
	PutInActive(tempCmd);
	tempCmd = "";
							
	m_pApp->GetInitialChannel().Empty();
	m_pApp->Serialize(TRUE); 
	return;
}

void CMainFrame::PutInChanWindow(const CString& strChannel,
								CString& strMessage)				
{
	// This function puts the specified message in the window for the
	// specified channel

	char nl = '\n';
	CIRClientView *pView = NULL;
	CString tempR = strMessage;
	if (tempR.Find(nl) == -1)
		tempR += "\r\n";

	if (m_pApp->m_pIRCChanList->Lookup((LPCTSTR)strChannel, (CObject*&)pView))
	{
		int tLen = pView->GetEditCtrl().GetWindowTextLength();
		pView->GetEditCtrl().SetSel(tLen, tLen);
	 	pView->GetEditCtrl().ReplaceSel(tempR);
	 	return;
	}
}

void CMainFrame::PutInActive(CString& strResponse)
{
	char nl = '\n';
	CIRClientWnd* pChild = NULL;
	CString tempR = strResponse;
	 
	pChild = (CIRClientWnd*)MDIGetActive(); 
	if (pChild == NULL)
		return;
	int tLen = pChild->m_pServerView->GetEditCtrl().GetWindowTextLength();
	pChild->m_pServerView->GetEditCtrl().SetSel(tLen, tLen);
	if (tempR.Find(nl) == -1)
		tempR += "\r\n";
	pChild->m_pServerView->GetEditCtrl().ReplaceSel(tempR);
}

void CMainFrame::IRCParseResponse()
{
	CString responseOut = "";
	CStringList parsingList;
	int curPos = 0;
	char *sTemp = NULL;
	char *junkPtr = NULL;
	char *statNum = NULL;
	int nStatNum = 0;

	// state flags and stuff
	static BOOL fInNameList = FALSE;
	static CString theChan = "";
	char *responseCopy = NULL;
	const char kSpace=' ', kColon=':', kBang = '!', kPound = '#';
	LPCTSTR kExclude = "\n";


	for (curPos=0; curPos < m_ReceiveLines.GetCount();)
	{
		CString tempResponse = m_ReceiveLines.RemoveHead();
		tempResponse = tempResponse.SpanExcluding(kExclude);

		responseCopy = strdup(tempResponse);

		if (tempResponse[0] != ':')
		{
			sTemp = strtok(responseCopy, &kSpace); 
			if (stricmp(sTemp, "ping")==0)
			{
				responseOut = "PONG localhost\n\r";
				m_pApp->GetIRCSocket()->Send(responseOut);
				responseOut = "OnlineHost:  Connection with Internet ";
				responseOut += "Navigator is confirmed.";
				PutInActive(responseOut);
				responseOut = "";
			}
		}
		else
		{
			sTemp = strtok(responseCopy, &kSpace);
			statNum = strtok(NULL, &kSpace);
			if (atoi(statNum)==0)
			{
				if (stricmp(statNum, "nick")==0)
				{
					CString oldNick = "";
					const char *kNickMsg =" has changed nickname to ";
					responseOut = "OnlineHost: ";
					int bangPos = tempResponse.Find(kBang);
					oldNick = tempResponse.Mid(1, bangPos-1);
					responseOut += oldNick + kNickMsg;
					bangPos=tempResponse.ReverseFind(kColon)+1;
					oldNick = tempResponse.Mid(bangPos, tempResponse.GetLength());
					responseOut += oldNick;
					PutInActive(responseOut);
				}
				else if (stricmp(statNum, "join")==0)
				{
					CString who = "";
					int bangPos = tempResponse.Find(kBang);
					responseOut = "OnlineHost: ";
					who = tempResponse.Mid(1, bangPos-1);
					responseOut += who;	
					responseOut += " has joined channel ";
					junkPtr = strrchr(tempResponse, ':');
					responseOut += ++junkPtr;
					PutInChanWindow(junkPtr, responseOut);
				}
				else if (stricmp(statNum, "mode")==0)
				{
					responseOut = "OnlineHost: ";
					responseOut += strtok(NULL, &kSpace);
					responseOut += " set mode to ";
					responseOut += strtok(NULL, &kSpace);
					PutInActive(responseOut);
				}
				else if (stricmp(statNum, "kick")==0)
				{
					char *kicker = NULL;
					char *kickee = NULL;
					char *channel = NULL;
					kicker = strchr(tempResponse.Right(tempResponse.GetLength()-1), 
									(int)kBang);
					channel = strtok(NULL, &kSpace);
					kickee = strtok(NULL, &kSpace);
					responseOut = "OnlineHost: "; responseOut += kicker;
					responseOut += "kicked "; responseOut += kickee;
					responseOut += "off "; responseOut += channel; 
				}
				else if (stricmp(statNum, "privmsg")==0)
				{
					char *msg = NULL;
					char *chan = NULL;
					BOOL fPrivate = FALSE;
					int bangPos = tempResponse.Find(kBang);
					CString who = "";
					who = tempResponse.Mid(1, bangPos-1);

					chan = strtok(NULL, &kSpace); 
					fPrivate = (strchr(chan, kPound) == NULL);

					msg = tempResponse.GetBuffer(520);

					while (*++msg != ':')
						;
					msg++;

					sTemp = strtok(NULL, &kColon);
					if (fPrivate)
						responseOut = "* " + who + " *: ";
					else
						responseOut = who + ": ";
				
					responseOut += msg;
					if (fPrivate)
					{
						CString title = "Private Message From " + who;
						
						m_wndStatusBar.SetText(IDS_PRIVMSG_ARRIVED);	
						MessageBeep(-1);
						MessageBox(msg, title, MB_ICONASTERISK);  
						m_wndStatusBar.ShowIdleMessage();
					} 
					else
						PutInChanWindow(chan, responseOut);
				}  
				else if (stricmp(statNum, "notice") == 0)
				{
					char *msg = NULL;
					char *chan = NULL;
					int bangPos = tempResponse.Find(kBang);
					CString who = "";
					who = tempResponse.Mid(1, bangPos-1);

					chan = strtok(NULL, &kSpace); 

					msg = tempResponse.GetBuffer(520);

					while (*++msg != ':')
						;
					msg++;

					sTemp = strtok(NULL, &kColon);

					responseOut = "Notice From " + who + ": ";
				
					responseOut += msg;

					PutInChanWindow(chan, responseOut);
				}
				else if ((stricmp(statNum, "part")==0) || (stricmp(statNum, "quit")==0))
				{
					responseOut = "OnlineHost: ";
					responseOut += tempResponse.Left(tempResponse.Find(kBang));
					responseOut += " is leaving.";
					sTemp = strtok(NULL, &kSpace); // sTemp now has channel name
					responseOut = "";
					PutInChanWindow(sTemp, responseOut);
				}
			}
			else 
			{
				nStatNum = atoi(statNum);
				switch (nStatNum)
				{
					case RPL_START_MOTD:
						responseOut = "OnlineHost: Message of the Day follows.";
						PutInActive(responseOut);    
						responseOut = "\r\n\r\n";
						PutInActive(responseOut);
						responseOut = "";
						break;

					case RPL_MORE_MOTD:
						sTemp = strtok(NULL, &kColon);
						sTemp = strtok(NULL, &kColon);
						responseOut = sTemp;
						PutInActive(responseOut);
						break;

					case RPL_END_MOTD:
						responseOut = "\r\n\r\nOnlineHost: End of the Message of the Day.";
						PutInActive(responseOut);   
						responseOut = "OnlineHost: Go ahead and start chatting!";
						PutInActive(responseOut);
						break;

					case RPL_TOPIC:
						responseOut = "OnlineHost: Topic for channel ";
						sTemp = strtok(NULL, &kSpace);
						sTemp = strtok(NULL, &kSpace);
						responseOut += sTemp;
						responseOut += " is ";
						sTemp = strtok(NULL, &kColon);
						responseOut += sTemp;
						PutInActive(responseOut);
						break;
			
					case RPL_NOTOPIC:
						responseOut = "OnlineHost: There's no topic for channel ";
						sTemp = strtok(NULL, &kColon);
						responseOut += sTemp;
						PutInActive(responseOut);
						break;

					case RPL_NAMREPLY:     
						sTemp = strtok(NULL, &kSpace);
						sTemp = strtok(NULL, &kSpace);
						theChan = strtok(NULL, &kSpace);
						responseOut = "";
						if (!fInNameList)
						{
							fInNameList = TRUE;	
							responseOut = "OnlineHost: Users on channel	" + theChan;
						}
						sTemp = strtok(NULL, &kColon);
						responseOut += ":";
						PutInActive(responseOut);
						responseOut = sTemp;
						PutInActive(responseOut);
						break;

					case RPL_ENDOFNAMES:
						fInNameList = FALSE;
						break;

					case ERR_CHANNELISFULL:
						responseOut = "OnlineHost: You can't join that channel because it's full.";
						PutInActive(responseOut);
						break;

					case ERR_YOUREBANNEDCREEP:
					case ERR_BANNEDFROMCHAN:
						responseOut = "OnlineHost: You can't join that channel because you're banned.";
						PutInActive(responseOut);
						break;

					case ERR_INVITEONLYCHAN:
						responseOut = "OnlineHost: You can't join that channel because it's by invitation only.";
						PutInActive(responseOut);
						break;

					case ERR_NICKNAMEINUSE:
						responseOut = "Your nickname's already in use. Choose another.";
						MessageBeep(-1);
						AfxMessageBox(responseOut, MB_ICONSTOP);
						PutInActive("OnlineHost: " + responseOut);
						break;


					case IGN_STATUS1:
					case IGN_STATUS2:
					case IGN_STATUS3:
					case IGN_STATUS4:
						break;
				
					case RPL_LUSEROP:
		 			case RPL_LUSERUNKNOWN:
					case RPL_LUSERCHANNELS:
					case RPL_LUSERME:
						break;

					default:
						break;
				} // switch
			} // statnum != 0
		} // tempResponse == colon
		if (responseCopy)
			free(responseCopy);
	} // for
}
		
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// if SC_CLOSE is selected, step through app's Exit proc 
	if (m_pApp->m_bHelpMode)
	{
		// don't interfere with help mode
		switch (nID & 0xFFF0)
		{
		case SC_SIZE:
		case SC_MOVE:
		case SC_MINIMIZE:
		case SC_MAXIMIZE:
		case SC_NEXTWINDOW:
		case SC_PREVWINDOW:
		case SC_CLOSE:
		case SC_RESTORE:
		case SC_TASKLIST:
			if (!SendMessage(WM_COMMANDHELP, 0,
			HID_BASE_COMMAND+ID_COMMAND_FROM_SC(nID & 0xFFF0)))
			SendMessage(WM_COMMAND, ID_DEFAULT_HELP);
			break;
		
			default:
			// don't interfere with system commands we don't know about
			CWnd::OnSysCommand(nID, lParam);
			break;
		}
	}
	else
	{
		switch (nID & 0xFFF0)
		{
		    case SC_CLOSE:
			// don't bother with showing the user confirmation and all
			// that -- just send an WM_CLOSE message to ourselves after
			// signing off
			if (m_pApp->IsOnline())
			{
				BOOL b = m_pApp->IsConfirmSignOff();
				if (b)
				{
					m_pApp->SetConfirmSignOff(FALSE);
				}
				
				BOOL b2 = m_pApp->SignOff();
				
				if (b)
				{
					m_pApp->SetConfirmSignOff(b);
				}                                
				
				if (!b2)
				{
					return;
				}
			}

			m_pApp->CloseAllDocuments(TRUE);
			SendMessage(WM_CLOSE);
			break;

			case SC_SIZE:
			case SC_MOVE:
			case SC_MINIMIZE:
			case SC_MAXIMIZE:
			case SC_NEXTWINDOW:
			case SC_PREVWINDOW:
			case SC_RESTORE:
			case SC_TASKLIST:
			CWnd::OnSysCommand(nID, lParam);
			break;
			
			default:
			CWnd::OnSysCommand(nID, lParam);
			break;
		}
	}
}

void CMainFrame::OnDefaultHelp()
{
	m_pApp->EnterHelpArea();
}

void CMainFrame::OnHelpSearch()
{
	CWaitCursor wait;
	
	HWND hWnd = GetSafeHwnd();
	if (hWnd == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_FAILED_TO_LAUNCH_HELP);
		return;
	}
	
	CWnd* pSearchWnd = NULL;
	
	if (::WinHelp(hWnd, AfxGetApp()->m_pszHelpFilePath,
			HELP_PARTIALKEY, (DWORD)(LPSTR)""))
	{
		// Attempt to get a pointer to the Search dialog box
		// which appears and center it on this window
		pSearchWnd = CWnd::GetActiveWindow();
		
		if (((CMainFrame*)pSearchWnd) == this)
			return;
		
		pSearchWnd->CenterWindow(this);
	}
	else
	{
		// Alert the user that we weren't able to open Help
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_FAILED_TO_LAUNCH_HELP);
		return;
	}
}
