// newswnd.cpp: implementation file
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

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewsWnd

IMPLEMENT_DYNCREATE(CNewsWnd, CMDIChildWnd)

CNewsWnd::CNewsWnd()
{
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

CNewsWnd::~CNewsWnd()
{
	m_pApp = NULL;
}


BEGIN_MESSAGE_MAP(CNewsWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CNewsWnd)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewsWnd message handlers

BOOL CNewsWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
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
    // give the application a pointer to this window so other functions
    // can access this window
	m_pApp->m_pNewsWnd = this;
    
	return TRUE;
}

BOOL CNewsWnd::PreCreateWindow(CREATESTRUCT& cs)
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

void CNewsWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL)
	{
		// Since the Internet Newsgroups window is going to have the label,
		// "Internet Navigator", then set this window's text to that
		CString strText = "Internet Newsgroups";
		SetWindowText(strText);
		return;
	}
	return;
}

void CNewsWnd::OnClose()
{
	m_pApp->m_pNewsWnd = NULL;
	CMDIChildWnd::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManagerWnd

IMPLEMENT_DYNCREATE(CNewsgroupsManagerWnd, CMDIChildWnd)

CNewsgroupsManagerWnd::CNewsgroupsManagerWnd()
{
}

CNewsgroupsManagerWnd::~CNewsgroupsManagerWnd()
{
}


BEGIN_MESSAGE_MAP(CNewsgroupsManagerWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CNewsgroupsManagerWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManagerWnd message handlers

void CNewsgroupsManagerWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// Update your parent window first.
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
	
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CNewsDoc* pDocument = (CNewsDoc*)GetActiveDocument();
	if (pDocument != NULL && bAddToTitle) 
	{
		CString strText = "Newsgroups Manager";
		SetWindowText(strText);		
	}
	
	return;
}

BOOL CNewsgroupsManagerWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 47;
	windowRect.right = 598;
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
	  "Newsgroups Manager", dwDefaultStyle, windowRect,
	  (CMDIFrameWnd*)pParentWnd, pContext))
	{
		return FALSE;	// will self destruct on failure normally
	}
    
    // it worked !
 	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionWnd

IMPLEMENT_DYNCREATE(CArticleSelectionWnd, CMDIChildWnd)

CArticleSelectionWnd::CArticleSelectionWnd()
{
}

CArticleSelectionWnd::~CArticleSelectionWnd()
{
}

BEGIN_MESSAGE_MAP(CArticleSelectionWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CArticleSelectionWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionWnd message handlers

BOOL CArticleSelectionWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 47;
	windowRect.right = 598;
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

BOOL CArticleSelectionWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_CHILD|WS_OVERLAPPEDWINDOW;
	
	return CMDIChildWnd::PreCreateWindow(cs);
}

void CArticleSelectionWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// Update your parent window first.
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
	
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CNewsDoc* pDocument = (CNewsDoc*)GetActiveDocument();
	if (pDocument != NULL && bAddToTitle) 
	{
		CString strText = m_strGroup;
		SetWindowText(strText);		
	}
	
	return;
}	

/////////////////////////////////////////////////////////////////////////////
// CArticleWnd

IMPLEMENT_DYNCREATE(CArticleWnd, CMDIChildWnd)

CArticleWnd::CArticleWnd()
{
}

CArticleWnd::~CArticleWnd()
{
}

BEGIN_MESSAGE_MAP(CArticleWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CArticleWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArticleWnd message handlers

BOOL CArticleWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
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

int CArticleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndBtnBar.Create(this, IDD_NNTPARTICLE_DIALOG_BAR,
							CBRS_TOP,
							IDW_NNTPARTICLE_COMMAND_BAR))
	{
		MessageBeep(-1);
		AfxMessageBox("Unable to create Article Navigation Bar.",
				MB_ICONSTOP);
		return -1;
	}
		
	// Create and display the ruler along the top edge of the window
	if (!m_wndRulerBar.Create(this, IDD_RULERBAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_LAST))
	{
		TRACE0("Failed to create ruler.\r\n");
		return -1;      // failed to create ruler                               
	}
	
	// Load bitmap button for ruler
	if (!m_btnRuler.AutoLoad(IDC_RULER, &m_wndRulerBar))
	{
		TRACE0("Failed to create ruler.\r\n");
		return -1;  // failed to create ruler bitmap
	}
	
	return 0;
}

void CArticleWnd::OnViewRuler()
{
	BOOL bToggleVisible = !m_wndRulerBar.IsWindowVisible();
	
	// If IsWindowVisible() == TRUE, we should hide the
	// ruler bar. If IsWindowVisible() == FALSE, then we
	// should show the ruler bar.
	m_wndRulerBar.ShowWindow((bToggleVisible)?SW_SHOW:SW_HIDE);
	RecalcLayout(); // adjust positions of various child windows
	return;
}

void CArticleWnd::OnUpdateViewRuler(CCmdUI* pCmdUI)
{   
	// If the ruler is visible, we want to put a check mark
	// next to the Ruler menu command.  
	// CDialogBar::IsWindowVisible() tells us whether or not
	// the ruler is visible
	BOOL bCheck = m_wndRulerBar.IsWindowVisible();
	pCmdUI->SetCheck(bCheck);
}

void CArticleWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// Update your parent window first.
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
	
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CNewsDoc* pDocument = (CNewsDoc*)GetActiveDocument();
	if (pDocument != NULL && bAddToTitle) 
	{
		// We want the window text to say the following:
		// Article Subject  
		
		// TODO: Add a preferences setting here to enable the user to
		// configure the newsgroup name away
		m_strText = m_strSubject;
		SetWindowText(m_strText);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeWnd

IMPLEMENT_DYNCREATE(CArticleComposeWnd, CMDIChildWnd)

CArticleComposeWnd::CArticleComposeWnd()
{
}

CArticleComposeWnd::~CArticleComposeWnd()
{
}


BEGIN_MESSAGE_MAP(CArticleComposeWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CArticleComposeWnd)
		ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArticleComposeWnd message handlers

BOOL CArticleComposeWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
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

BOOL CArticleComposeWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	return CMDIChildWnd::PreCreateWindow(cs);
}

int CArticleComposeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create and display the ruler along the top edge of the window 
	m_wndComposeBar.m_pDoc = (CNewsDoc*)GetActiveDocument(); 
	
	if (!m_wndComposeBar.Create(this, IDD_NEWS_COMPOSE_DIALOG_BAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_FIRST+23))
	{
		AfxMessageBox("Failed to create Compose News Article headers bar.");
		return -1;      // failed to create Compose Mail addresses bar                               
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

void CArticleComposeWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// Update your parent window first
    GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
	
	CNewsDoc* pDocument = (CNewsDoc*)GetActiveDocument(); 
	if (pDocument != NULL && bAddToTitle) 
	{
		SetWindowText("Compose News Article");
		return;
	}
	
	return;	
}

void CArticleComposeWnd::OnViewRuler()
{
	BOOL bToggleVisible = !m_wndRulerBar.IsWindowVisible();
	
	// If IsWindowVisible() == TRUE, we should hide the
	// ruler bar. If IsWindowVisible() == FALSE, then we
	// should show the ruler bar.
	m_wndRulerBar.ShowWindow((bToggleVisible)?SW_SHOW:SW_HIDE);
	RecalcLayout(); // adjust positions of various child windows
	return;
}

void CArticleComposeWnd::OnUpdateViewRuler(CCmdUI* pCmdUI)
{
	// If the ruler is visible, we want to put a check mark
	// next to the Ruler menu command.  
	// CDialogBar::IsWindowVisible() tells us whether or not
	// the ruler is visible
	BOOL bCheck = m_wndRulerBar.IsWindowVisible();
	pCmdUI->SetCheck(bCheck);
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeBar dialog bar

CArticleComposeBar::CArticleComposeBar() : CDialogBar()
{
	//{{AFX_DATA_INIT(CArticleComposeBar)
	//}}AFX_DATA_INIT
	m_pParentWnd = NULL;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pDoc = NULL;
}

void CArticleComposeBar::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CArticleComposeBar)
	DDX_Text(pDX, IDC_ARTICLE_SUBJECT, m_strSubject);
	DDX_Text(pDX, IDC_ARTICLE_NEWSGROUPS, m_strNewsgroups);
	DDX_Text(pDX, IDC_ARTICLE_DISTRIBUTION, m_strDistribution);
	DDX_Text(pDX, IDC_ARTICLE_FOLLOWUP_TO, m_strFollowupTo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CArticleComposeBar, CDialogBar)
	//{{AFX_MSG_MAP(CArticleComposeBar)
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP, OnHelp)
	ON_UPDATE_COMMAND_UI(ID_HELP, OnUpdateHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArticleComposeBar message handlers

BOOL CArticleComposeBar::DoFollowup(CString& strNewsgroups,
	 CString& strSubject, CString& /*strAuthor*/, CString& strDistribution)
{
	SetFollowup();
	
	SetNewsgroups(strNewsgroups);
	SetSubject(strSubject);
	SetDistribution(strDistribution);

	return UpdateData(FALSE);
}

BOOL CArticleComposeBar::UpdateData(BOOL bSaveAndValidate)
{    
	#ifdef _DEBUG
		afxDump << "Doing CArticleComposeBar::UpdateData(";
		bSaveAndValidate ? afxDump << "TRUE" : afxDump << "FALSE";
		afxDump << ").\r\n";
	#endif
	
	TRACE0("INF: Allocating CDataExchange object.\r\n");
	CDataExchange dx(this, bSaveAndValidate);
 
	BOOL bOK = FALSE;       // assume failure
	TRY
	{ 
		TRACE0("INF: Calling CArticleComposeBar::DoDataExchange().\r\n");
		DoDataExchange(&dx);                                             
		TRACE0("INF: CArticleComposeBar::DoDataExchange() successful.\r\n");
		bOK = TRUE;         // it worked
	}
	CATCH(CUserException, e)
	{ 
		TRACE0("ERR: CArticleComposeBar::DoDataExchange(): Validation failed.\r\n");
		// validation failed - user already alerted, fall through
		ASSERT(bOK == FALSE);
	}
	AND_CATCH_ALL(e)
	{ 
		TRACE0("ERR: Validation failed due to OOM or other resource failure.\r\n");
		// validation failed due to OOM or other resource failure
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP);
		ASSERT(bOK == FALSE);
	}
	END_CATCH_ALL
    
    #ifdef _DEBUG
    	afxDump << "INF: CArticleComposeBar::UpdateData() returning\r\n";
    	afxDump << bOK << "to caller.";
    #endif
	return bOK;
}

BOOL CArticleComposeBar::Create(CWnd* pParentWnd, LPCSTR lpszTemplateName,	
	UINT nStyle, UINT nID)
{
	ASSERT(pParentWnd);
	
	BOOL bResult = CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID);
	
	// store the pParentWnd pointer
	m_pParentWnd = pParentWnd;
	
	// now subcalss this dialog bar with CTL3D so it looks like the
	// other dialog-based parts of the program interface.
	if (m_pApp->IsUseCTL3D())
		Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);		
	
	return bResult;
}                  

BOOL CArticleComposeBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, 
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

int CArticleComposeBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{   
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CArticleComposeWnd* pParent = (CArticleComposeWnd*)GetParent();
	m_pDoc = (CNewsDoc*)pParent->GetActiveDocument();

	UpdateDialogControls(this, FALSE);
	
	return 0;
}

void CArticleComposeBar::OnHelp()
{
	m_pParentWnd->SendMessage(WM_COMMAND, ID_HELP);
	return;
}

void CArticleComposeBar::OnUpdateHelp(CCmdUI* pCmdUI)
{
	// since we support context-sensitive help, let's enable this command
	// button so the user can choose it.
	pCmdUI->Enable(TRUE);        
	return;
}
