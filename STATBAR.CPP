// statbar.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"
#include "statbar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInetnavStatusBar

CInetnavStatusBar::CInetnavStatusBar()
{
	// No contstruction code is needed here
}

CInetnavStatusBar::~CInetnavStatusBar()
{    
	// No destruction code is needed here
}

BEGIN_MESSAGE_MAP(CInetnavStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CInetnavStatusBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInetnavStatusBar message handlers

BOOL CInetnavStatusBar::SetText(CString strText)
{    
	// Set the status bar text using the CString strText.
	
	// first, test to make sure this object is valid, and test to make
	// sure the status bar window handle is valid.
	
	if (this == NULL)
	{
		TRACE0("ERR: CIntetnavStatusBar this pointer equal to NULL.\n");
		TRACE0("Warning/Debug-Aid: STACK OVERFLOW IMMINENT\r\n");
		return FALSE;                                                       
	}

	/*#ifdef _DEBUG
		afxDump << strText << "\r\n";
	#endif*/

	BOOL bResult = SetPaneText(0, strText, TRUE);

	return bResult; 
}

BOOL CInetnavStatusBar::SetText(UINT nIDResource)
{      
	// Set the text for the status bar using the nIDResource string resource.
	
	// first, test to make sure this object is valid and test to make
	// sure the status bar window handle is valid
	 
	if (this == NULL)
	{
		TRACE0("ERR: CIntetnavStatusBar this pointer equal to NULL.\n");
		TRACE0("Warning/Debug-Aid: STACK OVERFLOW IMMINENT\r\n");
		
		#ifdef _DEBUG                                                        
			MessageBeep(-1);
			AfxMessageBox("The pointer to the status bar is equal to NULL. "
				" A stack overflow is imminent.");
		#endif
		
		return FALSE;                                                       
	}

	CString strText = "";
	strText.LoadString(nIDResource);
	                      
	/*#ifdef _DEBUG
		afxDump << strText << "\n";
	#endif     */
	
	if (this == NULL)
		return FALSE;
	
	// this version of SetText takes the ID of a string resource to use
	// for the text
	
	BOOL bResult;
	bResult = SetPaneText(0, strText, TRUE);
	
	return bResult;
}
