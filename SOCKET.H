//*****************************************************************************
//* (C) Copyright 1995 Gregory Consulting Limited
//*
//* This file is Copyrighted and is subject to the rules outlined in the
//* software agreement accompanying this CDROM and book. You are free to
//* modify this code as you see fit according to those rules.
//*
//* THIS IS SAMPLE CODE! No warranties are expressed or implied, you're
//* using it at your own risk. In addition, the resulting program may
//* not be fully functional. Support for some features are left out so
//* that they can be added by you, the reader.
//*
//*****************************************************************************

// socket.h : header file
//
#ifndef _SOCKET_H
#define _SOCKET_H

#include "winsock.h"

#define WS_VERSION_REQUIRED 0x0101
#define WS_VERSION_MAJOR 1
#define WS_VERSION_MINOR 1
#define MIN_SOCKETS_REQUIRED 10
#define ID_CONNECT_TIMER 1
#define ID_RECEIVE_TIMER 2                                         

#define WM_SOCKETS_MESSAGE 				WM_USER+1
#define WM_CONNECT_GET_HOST_MESSAGE 	WM_USER+2
#define WM_GET_SERVER_PORT_MESSAGE 		WM_USER+3
                               
// at least as big as the largest possible line.(currently 512 bytes)                               
const int SOCK_BLOCK_SIZE = 520;

/////////////////////////////////////////////////////////////////////////////
// QSocket socket       

enum SocketStatus {UNINITIALIZED, DISCONNECTED, CONNECTING, LISTENING, DISCONNECTING, CONNECTED, 
			ERRORSTATE, TIMEDOUT } ;

enum SocketReceiveCmd { SocketStatusChanged = -100, NewSocketAccepted = -101 };
                        
class QSocket : public CSocket
{   
friend class CInternetNavApp;
// Construction
public:
	QSocket(BOOL create_socket = TRUE);

// Attributes
private:
	int m_nLastError;
	                  
	SocketStatus CurrentStatus;
	CString m_strError;
	
	CStringList ReceiveLines;
	CString RemainingReceive;
	CStringList SendLines;
	char *RawSendData;
	int RawSendDataLength;
	
	CWnd *ReceiveWindow;
	UINT ReceiveMessage;         
	
	CString m_strHostAddress;
	UINT m_nHostPort;
	
	CInternetNavApp* m_pApp;     
	BOOL m_bGetLine;  
	
	UINT m_nTimerID;
	
public: // Socket state helper functions   
	BOOL IsConnecting() { return (CurrentStatus == CONNECTING); }
	BOOL IsConnected() { return (CurrentStatus == CONNECTED); }
	BOOL IsDisconnected() { return (CurrentStatus == DISCONNECTED); }
	BOOL IsListening() { return (CurrentStatus == LISTENING); }
	BOOL IsTimedOut() { return (CurrentStatus == TIMEDOUT); }
	BOOL IsUnintialized() { return (CurrentStatus == UNINITIALIZED); }

// Operations
public:   
	// Notification of status -- only use on an accepted socket
	// to force the sending of a "socket status changed CONNECTED"
	// message
	void NotifyStatus();
	
	virtual BOOL OnMessagePending();

// Implementation
public:
	virtual ~QSocket(); 
	// Diagnostics	
	BOOL Disconnect(void);
	                             
	BOOL SetReceiveTarget(CWnd *window, UINT message);                             
	void Send(const CString& data);
	void SendRaw(const void *data, const int dataLen);
	CString GetLine(void);
	BOOL Listen(int back_log = 5);
	QSocket *Accept();
	void WaitUntilSendBufferEmpty();
   	void Linger(void);

	SocketStatus GetStatus(void) { return CurrentStatus; }
	CString GetErrorString(void) { return m_strError; }
	
	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	
protected:
	// helper functions
 	int SetErrorVars();
	void DisplayError(UINT nIDP = -1);
 	BOOL Start(void);
	void AddToReceive(const char *buffer, int amt);
	virtual void OnReceive(int error);
	virtual void OnWrite(int error);
	virtual void OnAccept(int error);
	virtual void OnConnect(int error);
	virtual void OnClose(int error);
	BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);
};

/////////////////////////////////////////////////////////////////////////////
#endif
