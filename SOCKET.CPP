// socket.cpp : implementation file of a Windows Sockets-based C++ 'QSocket'
// class which is the basis for communications over the Internet carried out
// by Internet Navigator.
//                       

#include "stdafx.h" 
#include "inetnav.h"

#include "resource.h"
#include "socket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define SET_ERROR_VARS() SetErrorVars()
/////////////////////////////////////////////////////////////////////////////
// QSocket

QSocket::QSocket(BOOL create_socket /*=TRUE*/)
  : CurrentStatus(UNINITIALIZED), ReceiveWindow(0), 
   ReceiveMessage(0), RawSendData(0), RawSendDataLength(0)
{ 
	m_pApp = (CInternetNavApp*)AfxGetApp();
	  
	if (create_socket)
	{
		if (Start())
		{
	    	CurrentStatus = DISCONNECTED;
		}
	}
	
	m_nTimeOut = m_pApp->GetInetTimeOut();
}

QSocket::~QSocket()
{
	// Disconnect the socket (if we aren't already) so we can cleanup
	// resources and tell the remote host we're not interested.
	Disconnect();                                              
	
	// Cleanup GetLine() buffer and empty send buffer
	CInetnavStatusBar* p = m_pApp->GetMainFrame()->GetStatusBar();
	p->SetText("Emptying transmission/receive buffers...");
	m_bGetLine = FALSE;
	
	if (!ReceiveLines.IsEmpty())
		ReceiveLines.RemoveAll();
	
	if (!SendLines.IsEmpty())
		SendLines.RemoveAll();
		
	// Free memory allocated for RawSendData
	if (RawSendData)
		delete[] RawSendData;
	
	p->ShowIdleMessage();
}

/////////////////////////////////////////////////////////////////////////////
// QSocket message handlers

BOOL QSocket::Start()
{                        
	// Call CAsyncSocket::Socket() with the default paramaters in order to
	// create this socket.                     
   	if (!Socket(/*0, SOCK_STREAM, FD_READ | FD_WRITE 
   			| FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, NULL*/))
    { 
		m_nLastError = GetLastError();
		
		// We have error description strings in the string table with the
		// same numeric values as WinSock error codes; so just pass the
		// error code to QSocket::DisplayError().
		DisplayError(m_nLastError);
		
   		CurrentStatus = ERRORSTATE;
   		return FALSE;
    }                     

	return TRUE;
}               

int QSocket::SetErrorVars() 
{
	// This function displays the error code and the line number and
	// filename that the error occurred on
	
	// However, if the error code is WSAEWOULDBLOCK or WSAEINPROGRESS,
	// then return immediately -- they are non-fatal errors
	           
	m_nLastError = GetLastError();
		
	switch (m_nLastError)
	{
		case WSAEWOULDBLOCK: // non-fatal warning
			return m_nLastError;
			
		case WSAEINPROGRESS: // non-fatal warning
			return m_nLastError;
			
		case WSAEINTR:		 // non-fatal information
			return m_nLastError; 
			
		case WSAENOTSOCK:	// non-fatal information
			return m_nLastError; 
			
		case WSAENOBUFS:	// non-fatal information
			return m_nLastError;
	}	
	
	// Get the appropriate error string from the String Table and load it
	// into m_strError
	m_strError.LoadString(m_nLastError);
	
	CurrentStatus = ERRORSTATE; 

    switch (m_nLastError)
    {
    	case WSAETIMEDOUT:
    		CurrentStatus = TIMEDOUT;
    		break;

    	default:  
    		CurrentStatus = ERRORSTATE;
    		break;
    }
    
	// our status just changed
	// send message if we are sending to another window
	if (CurrentStatus == ERRORSTATE)
	{
		switch (m_nLastError)
		{
	    	case WSAECONNREFUSED:
    			DisplayError(IDP_SOCKET_WSAECONNREFUSED); 
    			if (!IsDisconnected())
    				Disconnect(); // disconnect this socket -- connect was
    								// refused
    			break;
    		
    		case WSAECONNRESET:
    			DisplayError(IDP_SOCKET_WSAECONNRESET);
    			break;
    		
    		case WSAECONNABORTED:
    			DisplayError(IDP_SOCKET_WSAECONNABORTED);
    			break;  
    		
    		case WSAENETRESET:
    			DisplayError(IDP_SOCKET_WSAENETRESET);
    			break;
    			
    		case WSAENETDOWN:
    			DisplayError(IDP_SOCKET_WSAENETDOWN);
    			break; 
    			
			case WSAEADDRINUSE:
            	DisplayError(IDP_SOCKET_WSAEADDRINUSE);
            	break;
            
            case WSAEDESTADDRREQ:
            	DisplayError(IDP_SOCKET_WSAEDESTADDRREQ);
            	break;

    		default:
    			DisplayError();
    			break;
    	}
    }
	
	switch (m_nLastError)
	{
		case WSAETIMEDOUT:
			DisplayError(IDP_SOCKET_WSAETIMEDOUT);
			CurrentStatus = TIMEDOUT;
			break;
		
		default:
			break;
	}
	
	if (ReceiveWindow)
	{
		ReceiveWindow->SendMessage(ReceiveMessage, 
			(WPARAM)SocketStatusChanged, (LPARAM)0);
	}
	
    return m_nLastError;
}               
 
void QSocket::DisplayError(UINT nIDP /*= -1*/)
{
	// nIDP is the string resource ID of a message box prompt to display
	// if nIDP is -1, then display m_strError.
	
	if (nIDP != -1)
		m_strError.LoadString(nIDP);
		
	// diagnostics
	#ifdef _DEBUG
		afxDump << "ERR: " << m_strError << "\r\n";
	#endif
	
	// Display the error in a message box
	MessageBeep(-1);
	AfxMessageBox(m_strError, MB_ICONSTOP|MB_OK, nIDP);
	return;
}

BOOL QSocket::Disconnect(void)
{
	// Set up a message pump so all Windows messages continue to
	// be sent; this way we don't hang up the user's system
	
	m_bGetLine = FALSE;

	for (;;)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) && msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}                         
		else
		{
			break;
		}
	}

	// We don't just call CAsyncSocket::Close() because we want to 
	// disable non-blocking mode before the closesocket() is 
	// called in the CAsyncSocket::Close().
	// We disable the non-blocking mode so that when we have
	// linger on we will block rather than return a WSAEWOULDBLOCK.
	unsigned long nbIO = 0;
	CancelBlockingCall();
	AsyncSelect(0);
	
	// disable non-blocking mode
	ioctlsocket(m_hSocket, FIONBIO, &nbIO);   

	Close();	// close this socket
	m_hSocket = INVALID_SOCKET;

 	return TRUE;
}

BOOL QSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	BOOL bResult = TRUE;
	
	m_strHostAddress = lpszHostAddress;
	m_nHostPort = nHostPort;
	
	bResult = CAsyncSocket::Connect(lpszHostAddress, nHostPort);
	return bResult;
}

void QSocket::Linger(void)
{
	int what = 0;
	struct linger sL;
	sL.l_onoff = 1;
	sL.l_linger = 30;

	what = SetSockOpt(SO_LINGER, (char *)&sL, sizeof(sL));

	if (what == SOCKET_ERROR)
	{
		SetErrorVars();
	}
}

BOOL QSocket::OnMessagePending()
{
	// Even if the socket is blocking, empty the message queue
	
	// We want to turn tool tips off when in OnMessagePending so the user's
	// system isn't tied up if an unsuspecting user moves the mouse over
	// the toolbar and the computer freezes
	m_pApp->m_bToolTipsOff = TRUE;
		    
	MSG msg;

	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (!m_pApp->PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			
			// Always return TRUE  
			m_pApp->m_bToolTipsOff = FALSE;
			return TRUE;
		}
		else        
		{
			m_pApp->m_bToolTipsOff = FALSE;
			return TRUE;                   
		}
	}
	
	m_pApp->m_bToolTipsOff = FALSE;
	// We always want the idle messages sent because this takes care
	// of keeping memory free and updating the status bar's clock
	return TRUE;
}		

BOOL QSocket::SetReceiveTarget(CWnd *window, UINT message)
{                      
	// sets who to send all received data to
	ReceiveWindow = window;
	ReceiveMessage = message; 
	
	if (window && message)
		m_bGetLine = FALSE;              
	else
		m_bGetLine = TRUE;
	
	// clear receive buffer as the data must be left over
	ReceiveLines.RemoveAll();
	RemainingReceive = "";

	// setup AsyncSelect
	switch (CurrentStatus)
	{
		case CONNECTED:
		    if (!AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE ))
    		{       
   				SetErrorVars();
	   			return FALSE; 	
		    }
			break;

		case LISTENING:
		    if (!AsyncSelect(FD_ACCEPT ))
    		{       
   				SetErrorVars();
	   			return FALSE; 	
		    }
			break;

		default:
		    if (!AsyncSelect(FD_CONNECT | FD_CLOSE ))
    		{       
   				SetErrorVars();
	   			return FALSE; 	
		    }
			break;
	}

	return TRUE;
}

void QSocket::Send(const CString& data)
{
	// diagnostic code
	#ifdef _DEBUG
		if (data[data.GetLength() - 2] == '\r'
			&& data[data.GetLength() - 1] == '\n')
		{
			afxDump << data; // dump without adding CRLF
		}
		else
		{
			afxDump << data << "\r\n";
		}
	#endif //_DEBUG
	
	if (data.GetLength() == 0)
	{   
		#ifdef _DEBUG
			afxDump << "Unable to send data -- data is zero-length.\r\n";
		#endif //_DEBUG
		
		return;
	}

	BOOL send_buffer_empty = SendLines.IsEmpty();

	SendLines.AddTail(data);
	
	if (send_buffer_empty)
	{
		int amt = SendLines.GetHead().GetLength();
 
 		amt = CAsyncSocket::Send(SendLines.GetHead(), amt, 0);	
		if (amt == SOCKET_ERROR)
		{   
			int error = GetLastError();
			if (error != WSAEWOULDBLOCK
				&& error != WSAEINPROGRESS)
			{
				WSASetLastError(error);
				SetErrorVars();
			}
		}                      
		else
		{  
			if (amt == SendLines.GetHead().GetLength())
			{
				SendLines.RemoveHead();
			}
			else
			{
				// only part of line was sent leave rest for later
				const char *head = SendLines.GetHead();
				strcpy((char *)head, head+amt);
			}
		}
	}
}

void QSocket::WaitUntilSendBufferEmpty()
{
	// Pump messages until all lines sent
	while(!SendLines.IsEmpty() && RawSendDataLength > 0)
	{
		// Set up a message pump so all Windows messages continue to
		// be sent so we don't hang up the user's system
		for (;;)
		{
			MSG msg;
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{ 
				if (!m_pApp->PreTranslateMessage(&msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}  
				
				m_pApp->OnIdle(0);
				m_pApp->OnIdle(1);
			}
			else
			{    
				// to correctly handle idle processing, call
				// CWinApp::OnIdle()
				m_pApp->OnIdle(0); // update UI
				m_pApp->OnIdle(1); // delete temp objects
					
				break; // no more messages to pump
			}
		}
	}
	
	return;
}

void QSocket::SendRaw(const void *data, const int dataLen)
{
	char *pszData = new char[RawSendDataLength + dataLen];
	memcpy(pszData, RawSendData, RawSendDataLength);
	memcpy(pszData+RawSendDataLength, data, dataLen);
	
	delete RawSendData;
	RawSendData = NULL;
	
	RawSendData = pszData;

	RawSendDataLength += dataLen;

	while(RawSendDataLength > 0)
	{
		int amt_sent = CAsyncSocket::Send((char *)RawSendData, RawSendDataLength, 0);
		if (amt_sent == SOCKET_ERROR)
		{
			int error = GetLastError();
			if (error != WSAEWOULDBLOCK && error != WSAEINPROGRESS)
			{
				WSASetLastError(error);
				SetErrorVars();
			}
			break;
		}
		else
		{
			RawSendDataLength -= amt_sent;
			memcpy(RawSendData, RawSendData+amt_sent, RawSendDataLength);
		}
	}
	return;
}

void QSocket::AddToReceive(const char *buffer, int amt)
{
	char temp[SOCK_BLOCK_SIZE+1];
	
	int cur_pos = 0;
	while (cur_pos < amt)
	{
		char *lf = strchr(buffer+cur_pos, '\n');
		int length;
					
		if (lf)
		{
			length = lf-buffer-cur_pos+1;
		}
		else
		{
			length = amt - cur_pos;
		}
					
		strncpy(temp, buffer+cur_pos, length);
		temp[length] = 0;            
		
		if (lf)
		{				                  
			// we have a linefeed so add string
			if (RemainingReceive.IsEmpty())
			{
				ReceiveLines.AddTail(temp);
			}                            
			else
			{
				ReceiveLines.AddTail(RemainingReceive+temp);
				RemainingReceive.Empty();
			}
 		}
 		else
 		{
 			// we don't have a linefeed so add to RemainingReceive
 			// for future use
 			RemainingReceive += temp;
 		}
 		cur_pos += length;
	}
}
				
CString QSocket::GetLine(void)
{                                      
	if (ReceiveWindow || !m_bGetLine)
	{
		// we are in the wrong mode to use GetLine()
		#ifdef _DEBUG
			afxDump << "ERR: GetLine() used in wrong mode." << "\r\n";
		#endif
		
		return "";
	}

	while(IsConnected() && ReceiveLines.IsEmpty())
	{
 		char temp[SOCK_BLOCK_SIZE+1];
	 	int amt = QSocket::Receive(temp, SOCK_BLOCK_SIZE, 0);
 		if (amt == SOCKET_ERROR)
	 	{                                              
 			SetErrorVars();
 			continue;
	 	}
		else if (amt == 0)
		{
			// Close this socket
			OnClose(0);
		}
 		else
	 	{   
			temp[amt] = 0; 
			// add to buffer we are using GetLine() mode
			AddToReceive(temp, amt);
  		}
	}

	if (!ReceiveLines.IsEmpty() && !IsDisconnected())
	{ 
		CString strResult = ReceiveLines.RemoveHead();
		return strResult;
	}
	else
	{  
		// diagnostics
		#ifdef _DEBUG
			afxDump << "QSocket::GetLine(): No text in socket receive buffer.";
			afxDump << "\r\n";
		#endif
		
		return "";
	}
} 

BOOL QSocket::Listen(int back_log /* = 5 */)
{
	if (ReceiveWindow != NULL)
	{
		// receiving callbacks when status changes so setup AsyncSelect
	    if (!AsyncSelect(FD_ACCEPT))
    	{       
   			SetErrorVars();
	   		return FALSE; 	
    	}
	}

	if (!CAsyncSocket::Listen(back_log))
	{                                          
		SetErrorVars();
		return FALSE;
	}
	
	CurrentStatus = LISTENING;
	
	if (ReceiveWindow)
	{
		// Since our status has changed to LISTENING, send our status
		// change Windows message to the receive target
		ReceiveWindow->SendMessage(ReceiveMessage, 
			(WPARAM)SocketStatusChanged, (LPARAM)0);
	}

	return TRUE;
}

QSocket* QSocket::Accept()
{   
	QSocket* pReturnSocket = new QSocket(FALSE);
	
	// do not get the address of remote end. 
	if (!CAsyncSocket::Accept(*pReturnSocket, NULL, NULL))
	{
		delete pReturnSocket;
		pReturnSocket = NULL;
		SetErrorVars();
	}

	pReturnSocket->CurrentStatus = CONNECTED;
	return pReturnSocket;
}

void QSocket::OnReceive(int error)
{
	// First, pump any waiting Windows messages
	for (;;)
	{
		MSG msg;
		/*wait.Restore();*/
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			if (!m_pApp->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		else
		{    
			// to correctly handle idle processing, call
			// CWinApp::OnIdle()
			m_pApp->OnIdle(0); // update UI
			m_pApp->OnIdle(1); // delete temp objects
					
			break; // no more messages to pump
		}
	}
	
 	// Ready to read
 	char temp[SOCK_BLOCK_SIZE+1];
 	int amt = CAsyncSocket::Receive(temp, SOCK_BLOCK_SIZE, 0);

 	if (amt == SOCKET_ERROR)
 	{                                              
		SetErrorVars();
 	}
 	else
 	{   
		temp[amt] = 0; 
 		if (ReceiveWindow)
 		{                                               
 			// send to window
 			ReceiveWindow->SendMessage(ReceiveMessage, (WPARAM)amt,
 								(LPARAM)(LPSTR)temp);
 		}
 		else
 		{
 			// add to buffer we are using with GetLine() mode
			AddToReceive(temp, amt);
		}
  	}
 	
}

void QSocket::OnWrite(int error)
{	
	// Ready to write
	if (RawSendDataLength)
	{
		while(RawSendDataLength > 0)
		{
			int amt_sent = CAsyncSocket::Send((char *)RawSendData, RawSendDataLength, 0);
			if (amt_sent == SOCKET_ERROR)
			{
				int error = GetLastError();
				if (error != WSAEWOULDBLOCK && error != WSAEINPROGRESS)
				{
					WSASetLastError(error);
					SetErrorVars();
				}
				break;
			}
			else
			{
				RawSendDataLength -= amt_sent;
				memcpy(RawSendData, RawSendData+amt_sent, RawSendDataLength);
			}
		}
	} 
	else if (!SendLines.IsEmpty())
	{
		int amt = SendLines.GetHead().GetLength();
		
		amt = CAsyncSocket::Send(SendLines.GetHead(), amt, 0);	
		if (amt == SOCKET_ERROR)
		{
			int error = GetLastError();
			if (error != WSAEWOULDBLOCK && error != WSAEINPROGRESS)
			{
				WSASetLastError(error);
				SetErrorVars();
			}
		}                      
		else
		{
			if (amt == SendLines.GetHead().GetLength())
			{
				SendLines.RemoveHead();
			}
			else
			{
				// only part of line was sent leave rest for later
				const char *head = SendLines.GetHead();
				strcpy((char *)head, head+amt);
			}
		}
	}
}

void QSocket::OnAccept(int error)
{  
	m_nLastError = error;

	// First, pump any waiting Windows messages
	for (;;)
	{
		MSG msg;
		/*wait.Restore();*/
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			if (!m_pApp->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		else
		{    
			// to correctly handle idle processing, call
			// CWinApp::OnIdle()
			m_pApp->OnIdle(0); // update UI
			m_pApp->OnIdle(1); // delete temp objects
					
			break; // no more messages to pump
		}
	}
	
	// Connection request has arrived
	if (!IsListening())
	{
		m_strError = "We can't accept the current connection because\r\n";
		m_strError += "we aren't listening for any right now.";
		
		CString strPrompt = m_strError + "\r\n\r\nPress 'OK' below to ";
		strPrompt += "acknowledge this message.";
		
		#ifdef _DEBUG
			afxDump << m_strError;
		#endif
		
		m_strError = strPrompt;
		DisplayError(-1); // display the prompt string above.    
		
		CurrentStatus = ERRORSTATE;
		
		if (ReceiveWindow)
		{
			ReceiveWindow->SendMessage(ReceiveMessage, 
				(WPARAM)SocketStatusChanged, (LPARAM)0);
		}
		
		ASSERT(FALSE); 				
	}   
	else
	{
		if (m_nLastError == 0)
		{
			QSocket *pNewSocket = Accept();

			if (pNewSocket)
			{
				if (ReceiveWindow)
				{
					ReceiveWindow->SendMessage(ReceiveMessage, 
						(WPARAM)NewSocketAccepted,	(LPARAM)pNewSocket);
				}
				else
				{
					m_strError = "Unable to accept new connection from host.";
					
										
					#ifdef _DEBUG  
						MessageBeep(-1);
						AfxGetMainWnd()->MessageBox("TODO: Call "
						"SetReceiveTarget() before calling "
						"Listen().  QSocket::Accept() is unable to send the "
						"connection to a NULL receive target.", "Ooops!",
							MB_ICONSTOP|MB_OK); 
						afxDump << m_strError;
					#endif //_DEBUG
					
					ASSERT(pNewSocket); // Is guaranteed to be FALSE
				}
			}
		}                             
		else
 		{
			switch (m_nLastError)
			{
				case WSAEWOULDBLOCK: // non-fatal warning
					break;		
				
				case WSAEINPROGRESS: // non-fatal warning
					break;
		
				case WSAEINTR:		 // non-fatal information
					break;
				
				case WSAENOTSOCK:	// non-fatal information
					break; 
		
				case WSAENOBUFS:	// non-fatal information
					break;
			}	
	
			// Get the appropriate error string from the String Table and load it
			// into m_strError
			m_strError.LoadString(m_nLastError);
	
			CurrentStatus = ERRORSTATE; 

    		switch (m_nLastError)
    		{
    			case WSAETIMEDOUT:
    				CurrentStatus = TIMEDOUT;
    				break;

		    	default:
    				break;
    		}
    
			// our status just changed
			// send message if we are sending to another window
			if (CurrentStatus == ERRORSTATE)
			{
				switch (m_nLastError)
				{
	    			case WSAECONNREFUSED:
    					DisplayError(IDP_SOCKET_WSAECONNREFUSED); 
    					
    					if (!IsDisconnected())
    						Disconnect(); // disconnect this socket -- connect was
    									// refused
	    				break;

	    			case WSAECONNRESET:
   						DisplayError(IDP_SOCKET_WSAECONNRESET);
    					break;
    			
	    			case WSAECONNABORTED:
    					DisplayError(IDP_SOCKET_WSAECONNABORTED);
    					break;  
    		
	    			case WSAENETRESET:
    					DisplayError(IDP_SOCKET_WSAENETRESET);
    					break;
    			
	    			case WSAENETDOWN:
    					DisplayError(IDP_SOCKET_WSAENETDOWN);
    					break; 
    			
					case WSAEADDRINUSE:
    		        	DisplayError(IDP_SOCKET_WSAEADDRINUSE);
        		    	break;
            
	            	case WSAEDESTADDRREQ:
		            	DisplayError(IDP_SOCKET_WSAEDESTADDRREQ);
    		        	break;

		    		default:
    					DisplayError();
    					break;
			    	
    			} 
    		}
	    	
			switch (m_nLastError)
			{
				case WSAETIMEDOUT:
					DisplayError(IDP_SOCKET_WSAETIMEDOUT);
					CurrentStatus = TIMEDOUT;
					break;
		        	
				default:
					break;
			}
	
			if (ReceiveWindow)
			{
				ReceiveWindow->SendMessage(ReceiveMessage, (WPARAM)SocketStatusChanged,
 									(LPARAM)0);
			}
		}
 	}
}

void QSocket::NotifyStatus()
{
	// Only use with Accept()ed sockets after calling SetReceiveTarget()
	if (ReceiveWindow == NULL)
	{
		return;
	}
	
	CurrentStatus = CONNECTED;
	
	if (ReceiveWindow)
	{
		ReceiveWindow->SendMessage(ReceiveMessage,
			(WPARAM)SocketStatusChanged, (LPARAM)0);
	}
	
	return;
}

void QSocket::OnConnect(int error)
{
    m_nLastError = error;
    
	// First, pump any waiting Windows messages
	for (;;)
	{
		MSG msg;
		/*wait.Restore();*/
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			if (!m_pApp->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		else
		{    
			// to correctly handle idle processing, call
			// CWinApp::OnIdle()
			m_pApp->OnIdle(0); // update UI
			m_pApp->OnIdle(1); // delete temp objects
					
			break; // no more messages to pump
		}
	}
	
 	// Completed connection
 	if (!IsConnecting())
 	{
 		m_strError = "FD_CONNECT received when not in status CONNECTING";
 		
 		#ifdef _DEBUG
 			afxDump << "ERR: " << m_strError << "\r\n";
 		#endif
 		
 		// go on silent running, do not alert the user but instead,
 		// just silently die 			 				
 	}   
 	else
 	{
 		if (m_nLastError == 0)
 		{
		    if (!AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE ))
			{       
				SetErrorVars();
				return; 	
			}
			CurrentStatus = CONNECTED; 
			
			// diagnostics
			#ifdef _DEBUG
				afxDump << "Connected to ";
				afxDump << m_strHostAddress;
				afxDump << " on port " << m_nHostPort << ".\r\n";
			#endif      
			
			// our status just changed
			// send message if we are sending to another window
			if (ReceiveWindow)
			{
				ReceiveWindow->SendMessage(ReceiveMessage, 
					(WPARAM)SocketStatusChanged, (LPARAM)0);
			}
		}                             
		else
		{
			CWaitCursor wait;
		
			switch (m_nLastError)
			{
				case WSAEWOULDBLOCK: // non-fatal warning
					break;		
				
				case WSAEINPROGRESS: // non-fatal warning
					break;
		
				case WSAEINTR:		 // non-fatal information
					break;
					      
				case WSAENOTSOCK:	 // non-fatal information
					break;
				
				case WSAENOBUFS:	 // non-fatal information
					break;				
			}	
	
			// Get the appropriate error string from the String Table and load it
			// into m_strError
			m_strError.LoadString(m_nLastError);
	
			CurrentStatus = ERRORSTATE; 

    		switch (m_nLastError)
    		{
    			case WSAETIMEDOUT:
    				CurrentStatus = TIMEDOUT;
    				break;

		    	default:
    				break;
    		}
    
			// our status just changed
			// send message if we are sending to another window
			if (CurrentStatus == ERRORSTATE)
			{
				switch (m_nLastError)
				{
	    			case WSAECONNREFUSED:
    					DisplayError(IDP_SOCKET_WSAECONNREFUSED); 
    					
    					if (!IsDisconnected())
    						Disconnect(); // disconnect this socket -- connect was
    									// refused
	    				break;

	    			case WSAECONNRESET:
   						DisplayError(IDP_SOCKET_WSAECONNRESET);
    					break;
    			
	    			case WSAECONNABORTED:
    					DisplayError(IDP_SOCKET_WSAECONNABORTED);
    					break;  
    		
	    			case WSAENETRESET:
    					DisplayError(IDP_SOCKET_WSAENETRESET);
    					break;
    			
	    			case WSAENETDOWN:
    					DisplayError(IDP_SOCKET_WSAENETDOWN);
    					break; 
    			
					case WSAEADDRINUSE:
    		        	DisplayError(IDP_SOCKET_WSAEADDRINUSE);
        		    	break;
            
	            	case WSAEDESTADDRREQ:
		            	DisplayError(IDP_SOCKET_WSAEDESTADDRREQ);
    		        	break;

		    		default:
    					DisplayError();
    					break;
			    	
    			} 
    		}
	    	
			switch (m_nLastError)
			{
				case WSAETIMEDOUT:
					DisplayError(IDP_SOCKET_WSAETIMEDOUT);
					CurrentStatus = TIMEDOUT;
					break;
		        	
				default:
					break;
			}
	
			if (ReceiveWindow)
			{
				ReceiveWindow->SendMessage(ReceiveMessage, 
					(WPARAM)SocketStatusChanged, (LPARAM)0);
			}
		}
	}
}
            
void QSocket::OnClose(int error)
{
	// Socket closed
	#ifdef _DEBUG
		afxDump << "Socket closed and disconnected.\r\n";
	#endif
	
	CurrentStatus = DISCONNECTED;
	// our status just changed
	// send message if we are sending to another window
	if (ReceiveWindow)
	{
		ReceiveWindow->SendMessage(ReceiveMessage, 
			(WPARAM)SocketStatusChanged, (LPARAM)0);
	}
}

BOOL QSocket::ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
  	CurrentStatus = CONNECTING;
  	if (ReceiveWindow)
  	{
  		// Send the receive target a Windows Message indicating that
  		// we're connecting to the server
		ReceiveWindow->SendMessage(ReceiveMessage, 
			(WPARAM)SocketStatusChanged, (LPARAM)0);
	}

    if (!AsyncSelect(FD_CONNECT))
    {       
   		SET_ERROR_VARS();
   		return FALSE; 	
    }
    
	if (connect(m_hSocket, lpSockAddr, nSockAddrLen) == SOCKET_ERROR)
	{
		int err = GetLastError();
		if (err != WSAEWOULDBLOCK)
		{
			SET_ERROR_VARS();
			return FALSE;
		}
	}                    
	else
	{
	    if (!AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE ))
    	{       
   			SET_ERROR_VARS();
   			return FALSE; 	
	    }
	    
		CurrentStatus = CONNECTED;
		if (ReceiveWindow)
		{
			ReceiveWindow->SendMessage(ReceiveMessage, (WPARAM)SocketStatusChanged,
										(LPARAM)0);
		}
 	}  
 	
 	// We're connected or waiting to connect
 	return TRUE;
}
                                                             
