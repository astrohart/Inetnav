// mailwnd.cpp: implementation file for mailbox windows
//

#include "stdafx.h"
#include "inetnav.h" 
#include <stdio.h>

#include "dialogs.h" 
#include "wizards.h"
#include "propshts.h" 

#include "mail.h" 
#include "news.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailBoxWnd

IMPLEMENT_DYNCREATE(CMailBoxWnd, CMDIChildWnd)

CMailBoxWnd::CMailBoxWnd()
{
}

CMailBoxWnd::~CMailBoxWnd()
{
}


BEGIN_MESSAGE_MAP(CMailBoxWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMailBoxWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxWnd message handlers

BOOL CMailBoxWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 80;
	windowRect.right = 562;
	windowRect.top = 17;
	windowRect.bottom = 375;
	 
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

BOOL CMailBoxWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// change the window style -- we aren't using the WS_VISIBLE style here
	// because the view needs to form itself and fill in bitmap buttons
	// The view then tells this window to show itself when it is finished
	// initializing
	cs.style = WS_CHILD|WS_BORDER|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU;
	cs.style |= FWS_ADDTOTITLE;
	
	// return return value of base-class version of this function
	return CMDIChildWnd::PreCreateWindow(cs);
} 

// OnUpdateFrameTitle -- maintains the window title we want, no matter what
// the document class tries to change this window's title to!
void CMailBoxWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// Update your parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	CMailBoxDoc* pDocument = (CMailBoxDoc*)GetActiveDocument();
	if (pDocument != NULL && bAddToTitle)
	{
		CString strText = pDocument->GetMailBoxTitle();
		SetWindowText(strText);
		return;
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CNewMailWnd

IMPLEMENT_DYNCREATE(CNewMailWnd, CMDIChildWnd)

CNewMailWnd::CNewMailWnd()
{
}

CNewMailWnd::~CNewMailWnd()
{
}


BEGIN_MESSAGE_MAP(CNewMailWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CNewMailWnd)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewMailWnd message handlers

void CNewMailWnd::OnWindowRemember()
{
	// In this function, we gather data about this window's size and
	// position, then write those values to the INI file.    
	// we need these values to position the window in LoadFrame() later
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	
	CRect wndRect;
	GetWindowRect(wndRect);
	
	// now write size and position to the INI file
	int nHeight, nWidth, nTop, nBottom, nLeft, nRight;
	
	nHeight = wndRect.Height();
	nWidth = wndRect.Width();
	nTop = wndRect.top;
	nBottom = wndRect.bottom;
	nLeft = wndRect.left;
	nRight = wndRect.right;
	
	// now write the above values to the INI file
	
	CString strNewMailSect = "NewMailView";	
	pApp->WriteProfileInt(strNewMailSect, "NewMailLeft", nLeft);
	pApp->WriteProfileInt(strNewMailSect, "NewMailRight", nRight);
	pApp->WriteProfileInt(strNewMailSect, "NewMailTop", nTop);
	pApp->WriteProfileInt(strNewMailSect, "NewMailBottom", nBottom);
	pApp->WriteProfileInt(strNewMailSect, "NewMailCX", nWidth);
	pApp->WriteProfileInt(strNewMailSect, "NewMailCY", nHeight); 
	return;
}

void CNewMailWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// Update your parent window first.
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	CMailBoxDoc* pDocument = (CMailBoxDoc*)GetActiveDocument();
	if (pDocument != NULL && bAddToTitle) 
	{
		CString strText = "New Mail";
		SetWindowText(strText);		
	}
	
	return;
}

BOOL CNewMailWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 47;
	windowRect.right = 596;
	windowRect.top = 60;
	windowRect.bottom = 350;
	 
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

BOOL CNewMailWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// change the window style -- we aren't using the WS_VISIBLE style here
	// because the view needs to form itself and fill in bitmap buttons
	// The view then tells this window to show itself when it is finished
	// initializing
	cs.style = WS_CHILD|WS_OVERLAPPEDWINDOW|WS_VISIBLE|FWS_ADDTOTITLE;

	// return return value of base-class version of this function
	return CMDIChildWnd::PreCreateWindow(cs);
} 
/////////////////////////////////////////////////////////////////////////////
// CMessageWnd

IMPLEMENT_DYNCREATE(CMessageWnd, CMDIChildWnd)

CMessageWnd::CMessageWnd()
{      
	m_strSubject = "";
}

CMessageWnd::~CMessageWnd()
{  
	m_strSubject = "";
}

BEGIN_MESSAGE_MAP(CMessageWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMessageWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageWnd message handlers

BOOL CMessageWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
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

int CMessageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create and display the ruler along the top edge of the window
	if (!m_wndMailBar.Create(this, IDD_EMAIL_DIALOG_BAR,
							CBRS_TOP,
							IDW_EMAIL_COMMAND_BAR))
	{
		AfxMessageBox("Failed to create e-mail command button bar.");
		return -1;      // failed to create e-mail command button bar                               
	}
	
	// Create and display the ruler along the top edge of the window
	if (!m_wndRulerBar.Create(this, IDD_RULERBAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_LAST))
	{
		AfxMessageBox("Failed to create ruler.");
		return -1;      // failed to create ruler                               
	}
	
	// Load bitmap buttons
	if (!m_btnRuler.AutoLoad(IDC_RULER, &m_wndRulerBar))
	{
		AfxMessageBox("Failed to create ruler.");
		return -1;  // failed to create ruler bitmap
	}

	return 0;
}

void CMessageWnd::OnViewRuler()
{
	BOOL bToggleVisible = !m_wndRulerBar.IsWindowVisible();
	
	// If IsWindowVisible() == TRUE, we should hide the
	// ruler bar. If IsWindowVisible() == FALSE, then we
	// should show the ruler bar.
	m_wndRulerBar.ShowWindow((bToggleVisible)?SW_SHOW:SW_HIDE);
	RecalcLayout(); // adjust positions of various child windows
	return;
}

void CMessageWnd::OnUpdateViewRuler(CCmdUI* pCmdUI)
{   
	// If the ruler is visible, we want to put a check mark
	// next to the Ruler menu command.  
	// CDialogBar::IsWindowVisible() tells us whether or not
	// the ruler is visible
	BOOL bCheck = m_wndRulerBar.IsWindowVisible();
	pCmdUI->SetCheck(bCheck);
}

void CMessageWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	if (!bActivate)
	{
		// If this window is not activated, then get the view attached to
		// this frame window to tell it to turn off its special toolbar
		// and to change the main toolbar back to the Places toolbar
		CMessageView* pView = (CMessageView*)GetActiveView();
		pView->OnActivateView(FALSE, NULL, pView);
	}
	else
	{
		CMessageView* pView = (CMessageView*)GetActiveView();
		pView->OnActivateView(TRUE, pView, NULL);
	}
	return;
}

void CMessageWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// Update your parent window first.
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	CMailBoxDoc* pDocument = (CMailBoxDoc*)GetActiveDocument();
	if (pDocument != NULL && bAddToTitle) 
	{
		CString strText = GetMessageSubject();
		
		if (strText.IsEmpty())
		{
			strText = "Mail Message (No Subject)";
		}
		
		SetWindowText(strText);		
		return;
	}
	
	return;
}                  

/////////////////////////////////////////////////////////////////////////////
// CComposeWnd

IMPLEMENT_DYNCREATE(CComposeWnd, CMDIChildWnd)

CComposeWnd::CComposeWnd()
{                        
	m_bIsInetnavTechSupport = FALSE;
	m_bIsISPTechSupport = FALSE;
	m_bIsSuggestionBox = FALSE;
	m_bIsUseCompWizard = FALSE;
	m_bIsUseFeedbackWizard = FALSE;
}

CComposeWnd::~CComposeWnd()
{
}


BEGIN_MESSAGE_MAP(CComposeWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CComposeWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	ON_UPDATE_COMMAND_UI(ID_HELP, OnUpdateHelp)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_MAIL_ADDRESS_BOOK, OnMailAddressBook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CComposeWnd message handlers

BOOL CComposeWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
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

int CComposeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create and display the ruler along the top edge of the window 
	m_wndInfoBar.m_pDoc = (CMailBoxDoc*)GetActiveDocument(); 

	if (!m_wndInfoBar.Create(this, IDD_MAIL_COMPOSE_DIALOG_BAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_FIRST+22))
	{
		AfxMessageBox("Failed to create Compose Mail addresses bar.");
		return -1;      // failed to create Compose Mail addresses bar                               
	}
	m_wndInfoBar.ShowWindow(SW_HIDE);
	m_wndInfoBar.UpdateWindow();
	
	if (!m_wndDlgBar.Create(this, IDD_TECHSUPPORT_DIALOG_BAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_FIRST+24))
	{
		AfxMessageBox("Failed to create part of the Ask the Staff window.");
		return -1;
	}
	m_wndDlgBar.ShowWindow(SW_HIDE);
	m_wndDlgBar.UpdateWindow();
		
	if (!m_wndISPBar.Create(this, IDD_ISP_TECHSUPPORT_DIALOG_BAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_FIRST+26))
	{
		AfxMessageBox("Failed to create part of the Ask the Staff window.");
		return -1;
	}   
	m_wndISPBar.ShowWindow(SW_HIDE);
	m_wndISPBar.UpdateWindow();
		
	if (!m_wndSuggBar.Create(this, IDD_SUGGESTION_BOX_BAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_FIRST+28))
	{
		AfxMessageBox("Failed to create part of the Ask the Staff window.");
		return -1;
	}
	m_wndSuggBar.ShowWindow(SW_HIDE);
	m_wndSuggBar.UpdateWindow();
	
	// Create and display the ruler along the top edge of the window
	if (!m_wndRulerBar.Create(this, IDD_RULERBAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_LAST))
	{
		AfxMessageBox("Failed to create ruler.");
		return -1;      // failed to create ruler                               
	}                   
	m_wndRulerBar.ShowWindow(SW_HIDE);
	m_wndRulerBar.UpdateWindow();
	
	// Load bitmap buttons
	if (!m_btnRuler.AutoLoad(IDC_RULER, &m_wndRulerBar))
	{
		AfxMessageBox("Failed to create ruler.");
		return -1;  // failed to create ruler bitmap
	}
    
    m_bIsInetnavTechSupport = FALSE;
    m_bIsISPTechSupport = FALSE;
    
	return 0;
}

void CComposeWnd::OnViewRuler()
{
	BOOL bToggleVisible = !m_wndRulerBar.IsWindowVisible();
	
	// If IsWindowVisible() == TRUE, we should hide the
	// ruler bar. If IsWindowVisible() == FALSE, then we
	// should show the ruler bar.
	m_wndRulerBar.ShowWindow((bToggleVisible)?SW_SHOW:SW_HIDE);
	RecalcLayout(); // adjust positions of various child windows
	return;
}

void CComposeWnd::OnUpdateViewRuler(CCmdUI* pCmdUI)
{   
	// If the ruler is visible, we want to put a check mark
	// next to the Ruler menu command.  
	// CDialogBar::IsWindowVisible() tells us whether or not
	// the ruler is visible
	BOOL bCheck = m_wndRulerBar.IsWindowVisible();
	pCmdUI->SetCheck(bCheck);
}

void CComposeWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{   
	// Update your parent window first
    GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
	
	CMailBoxDoc* pDocument = (CMailBoxDoc*)GetActiveDocument(); 
	if (pDocument != NULL && bAddToTitle) 
	{
		if (!m_bIsInetnavTechSupport && !m_bIsISPTechSupport
			&& !m_bIsSuggestionBox && !m_bIsUseFeedbackWizard)
			SetWindowText("Compose Mail");
		else if (m_bIsInetnavTechSupport && !m_bIsISPTechSupport
			&& !m_bIsSuggestionBox && !m_bIsUseFeedbackWizard)
			SetWindowText("Technical Support For Internet Navigator");
		else if (!m_bIsInetnavTechSupport && m_bIsISPTechSupport
			&& !m_bIsSuggestionBox && !m_bIsUseFeedbackWizard)
			SetWindowText("Ask Your Internet Provider");       
		else if (!m_bIsInetnavTechSupport && !m_bIsISPTechSupport
			&& m_bIsSuggestionBox && !m_bIsUseFeedbackWizard) 
			SetWindowText("Suggestion Box");
		else if (!m_bIsInetnavTechSupport && !m_bIsISPTechSupport
			&& m_bIsSuggestionBox && m_bIsUseFeedbackWizard) 		
			SetWindowText("Internet Navigator Feedback");
		return;
	}
	
	return;
}  

void CComposeWnd::OnHelp()
{
	CWnd::OnHelp();	
}

void CComposeWnd::OnUpdateHelp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CComposeWnd::OnMailAddressBook()
{
	// This function shows the user's Address Book -- the interface
	// to the document's address list, which helps the user fill in
	// the fields of this window's dialog bar with addresses
	
	// First, make sure this window is visible in the normal show state
	ActivateFrame(SW_RESTORE);
	UpdateWindow();
	
	CAddressBook theDialog(AfxGetMainWnd());
	theDialog.m_pComposeWnd = this;
	theDialog.m_pDoc = (CMailBoxDoc*)GetActiveDocument();
	
	theDialog.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CComposeBar dialog bar

CComposeBar::CComposeBar() : CDialogBar()
{
	//{{AFX_DATA_INIT(CComposeBar)
	m_strSubject = "";
	//}}AFX_DATA_INIT
	m_pParentWnd = NULL;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pDoc = NULL;
}

void CComposeBar::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CComposeBar)
	DDX_Text(pDX, IDC_MESSAGE_TO, m_strTo);
	DDX_Text(pDX, IDC_MESSAGE_CC, m_strCC);
	DDX_Text(pDX, IDC_MESSAGE_BCC, m_strBCC);
	DDX_Text(pDX, IDC_MESSAGE_SUBJECT, m_strSubject);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CComposeBar, CDialogBar)
	//{{AFX_MSG_MAP(CComposeBar)
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP, OnHelp)
	ON_UPDATE_COMMAND_UI(ID_HELP, OnUpdateHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CComposeBar message handlers

BOOL CComposeBar::Create(CWnd* pParentWnd, LPCSTR lpszTemplateName,	
	UINT nStyle, UINT nID)
{
	ASSERT(pParentWnd);
	
	BOOL bResult = CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID);
	
	// store the pParentWnd pointer
	if (pParentWnd)
		m_pParentWnd = pParentWnd;
	
	// now subcalss this dialog bar with CTL3D so it looks like the
	// other dialog-based parts of the program interface.
	if (m_pApp->IsUseCTL3D())
		Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);		
	
	return bResult;
}                  

BOOL CComposeBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, 
		UINT nID)
{
	ASSERT(pParentWnd);

	BOOL bResult = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);

	// store the pParentWnd pointer
	m_pParentWnd = pParentWnd;
	
	// now subcalss this dialog bar with CTL3D so it looks like the
	// other dialog-based parts of the program interface.
	if (m_pApp->IsUseCTL3D())
		Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);		
	
	return bResult;
}                  

int CComposeBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{   
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CComposeWnd* pParent = (CComposeWnd*)GetParent();
	m_pDoc = (CMailBoxDoc*)pParent->GetActiveDocument();

	UpdateDialogControls(this, FALSE);

	return 0;
}

void CComposeBar::OnHelp()
{
	m_pParentWnd->SendMessage(WM_COMMAND, ID_HELP);
	return;
}

void CComposeBar::OnUpdateHelp(CCmdUI* pCmdUI)
{
	// since we support context-sensitive help, let's enable this command
	// so the user can choose it.
	pCmdUI->Enable(TRUE);        
	return;
}