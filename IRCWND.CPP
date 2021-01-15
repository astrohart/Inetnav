// ircwnd.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"
#include "mainfrm.h"

#include "ircwnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIRClientWnd

IMPLEMENT_DYNCREATE(CIRClientWnd, CMDIChildWnd)

CIRClientWnd::CIRClientWnd()
{     
	m_pServerView = m_pClientView = NULL;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
}

BOOL CIRClientWnd::OnCreateClient(LPCREATESTRUCT lpcs,
					  CCreateContext* pContext)
{
	BOOL bStatus = FALSE;
	SIZE aSize;

	CSplitterWnd* pSplitWnd = &m_wndSplitter;
	pSplitWnd->CreateStatic(this, 2, 1, WS_CHILD); //|WS_HSCROLL|WS_VSCROLL

	// Pane #1 is our text-from-server
	aSize.cx=80; aSize.cy=200;
	bStatus = pSplitWnd->CreateView(0, 0, RUNTIME_CLASS(CIRClientView), 
									aSize, pContext); // ); 
	m_pServerView = (CIRClientView *)pSplitWnd->GetPane(0, 0);
	m_pServerView->GetEditCtrl().SetReadOnly(TRUE);

	// Pane #2 is where we type
	aSize.cx=80; aSize.cy=20;
	bStatus = pSplitWnd->CreateView(1, 0, 
									RUNTIME_CLASS(CIRClientView),
									 aSize, pContext); // ); 
 	m_pClientView = (CIRClientView *)pSplitWnd->GetPane(1, 0);
 	SetActiveView(m_pClientView, TRUE);

	pSplitWnd->ShowWindow(SW_SHOWNORMAL);
	pSplitWnd->UpdateWindow();
                  
    #ifdef _DEBUG
    	if (bStatus == FALSE)
    	{
    		CString strMsg = "CIRClientWnd::OnCreateClient() is returning";
    		strMsg += " FALSE to its caller.\r\n\r\nReason: Unable to ";
    		strMsg += "create splitter bar for IRC channel window.";
    		AfxMessageBox(strMsg);
    		
    		// Dump same message to debug output
    		afxDump << "CIRClientWnd::OnCreateClient() is returning FALSE ";
    		afxDump << "\r\n";
    		afxDump << "to its caller.\r\nReason: Unable to create ";
    		afxDump << "splitter bar for IRC channel window.";
    		afxDump << "\r\n";
    	} 
    	else
    	{
    		afxDump << "CIRClientWnd::OnCreateClient() is returning TRUE ";
    		afxDump << "\r\n";
    		afxDump << "to its caller.  Creation of the client area of ";
    		afxDump << "\r\n";
    		afxDump << "the client area of the IRC child window was ";
    		afxDump << "\r\n";
    		afxDump << "successfull." << "\r\n";
    	}
    #endif //_DEBUG  

	return bStatus;
}

CIRClientWnd::~CIRClientWnd()
{  
	#ifdef _DEBUG
	TRACE0("CIRClientWnd::~CIRClientWnd()\r\n");
	#endif
	
	delete m_pServerView;
	m_pServerView = NULL;
	
	delete m_pClientView;
	m_pClientView = NULL;
	
	m_pApp = NULL;
	m_pStatusBar = NULL;
}


BEGIN_MESSAGE_MAP(CIRClientWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CIRClientWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIRClientWnd message handlers

BOOL CIRClientWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{ 
	#ifdef _DEBUG
	TRACE0("Entering CIRClientWnd::LoadFrame()\r\n");
	#endif
	
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 9; 
	windowRect.right = 627;
	windowRect.top = 50;
	windowRect.bottom = 342;
	 
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
		return FALSE;   // will self destruct on failure normally
	}

	// it worked !
	return TRUE;
}

int CIRClientWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{   
	#ifdef _DEBUG
	TRACE0("Entering CIRClientWnd::OnCreate()\r\n");
	
	TRACE0("Now calling CMDIChildWnd::OnCreate()\r\n");
	#endif

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create and display the ruler along the top edge of the window
	if (!m_wndRulerBar.Create(this, IDD_RULERBAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_LAST))
	{
		AfxMessageBox("Failed to create ruler.");
		TRACE0("Failed to create ruler.\r\n");
		return -1;	// failed to create ruler                               
	}
	
	// Load bitmap buttons
	if (!m_btnRuler.AutoLoad(IDC_RULER, &m_wndRulerBar))
	{
		AfxMessageBox("Failed to create ruler.");     
		TRACE0("Failed to create ruler -- failed to load bitmap for ruler.\r\n");
		return -1;  // failed to create ruler bitmap
	}
	
	return 0;
}

void CIRClientWnd::OnViewRuler()
{
	TRACE0("CIRClientWnd::OnViewRuler()\r\n");
	BOOL bToggleVisible = !m_wndRulerBar.IsWindowVisible();
	
	// If IsWindowVisible() == TRUE, we should hide the
	// ruler bar. If IsWindowVisible() == FALSE, then we
	// should show the ruler bar.
	m_wndRulerBar.ShowWindow((bToggleVisible)?SW_SHOW:SW_HIDE);
	RecalcLayout(); // adjust positions of various child windows
	return;
}

void CIRClientWnd::OnUpdateViewRuler(CCmdUI* pCmdUI)
{
	// If the ruler is visible, we want to put a check mark
	// next to the Ruler menu command.  
	// CDialogBar::IsWindowVisible() tells us whether or not
	// the ruler is visible
	BOOL bCheck = m_wndRulerBar.IsWindowVisible();
	pCmdUI->SetCheck(bCheck);
}

void CIRClientWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	if (!bActivate)
	{
		// Deactivate both the client and server views
		m_pServerView->OnActivateView(FALSE, NULL, m_pServerView);
		m_pClientView->OnActivateView(FALSE, NULL, m_pClientView);
	}
	else
	{
		// Activate both the client and server views	
		m_pServerView->OnActivateView(TRUE, m_pServerView, NULL);
		m_pClientView->OnActivateView(TRUE, m_pClientView, NULL);
	} 
	return;
}
