// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

// Main MFC Classes headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions (including VB)
#include <afxsock.h>		// MFC Windows Sockets extensions 
#include <afxdlgs.h>		// MFC Common and other Dialog Boxes

// Other classes and system headers
#include "waitcur.h"		// MFC CWaitCursor wait cursor manager
#include <ctl3d.h>			// for 3D dialog box effects                 
#include <io.h>
#include <errno.h>

// Application-Specific headers
#include "socket.h"  		// for QSocket class      

// Message definitions
#define WM_SOCKET_RESPONSE WM_USER+201 // communications control socket 
#define WM_SOCKET_LISTEN   WM_USER+202 // socket for locating data socket
#define WM_SOCKET_DATA	   WM_USER+203 // socket for managing data  
#define WM_GOPHER_GET_MENU WM_USER+204 // fetch a Gopher menu!
#define WM_IRC_SOCKET_RESPONSE WM_USER+205 // response from IRC Socket
#define WM_FEEDBACK_WIZARD WM_USER+206	// start up the Feedback Wizard!
#define WM_COMPOSE_WIZARD	WM_USER+207	// start up the Compose Mail Wizard!
