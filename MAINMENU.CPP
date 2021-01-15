// mainmenu.cpp : implementation of the document, frame, and view classes for
// the Main Menu of Internet Navigator
//

#include "stdafx.h"
#include "inetnav.h"
#include "mainmenu.h"

#include <ctl3d.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainMenuView

IMPLEMENT_DYNCREATE(CMainMenuView, CFormView)

CMainMenuView::CMainMenuView()
	: CFormView(CMainMenuView::IDD)
{
	//{{AFX_DATA_INIT(CMainMenuView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT         
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

CMainMenuView::~CMainMenuView()
{
}

void CMainMenuView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainMenuView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainMenuView, CFormView)
	//{{AFX_MSG_MAP(CMainMenuView)
	ON_BN_CLICKED(IDC_FINGER, OnFinger)
	ON_BN_CLICKED(IDC_FTP, OnFtp)
	ON_BN_CLICKED(IDC_GOPHER, OnGopher)
	ON_BN_CLICKED(IDC_HELPINFO, OnHelpAndInfo)
	ON_BN_CLICKED(IDC_IRC_CHAT, OnIRC)
	ON_BN_CLICKED(IDC_MAIL_MANAGER, OnMailManager)
	ON_BN_CLICKED(IDC_USENET_NEWS, OnUsenetNews)
	ON_BN_CLICKED(IDC_WHOIS, OnWhois)
	ON_BN_CLICKED(IDC_PREFS, OnPrefs)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainMenuView update handlers

void CMainMenuView::OnInitialUpdate()
{   
	// call base class version of this function next
	CFormView::OnInitialUpdate();

	// now do our work
	m_btnPOFFICE.AutoLoad(IDC_MAIL_MANAGER, this);
	m_btnHelpInfo.AutoLoad(IDC_HELPINFO, this);
	m_btnFinger.AutoLoad(IDC_FINGER, this);
	m_btnFTP.AutoLoad(IDC_FTP, this);
	m_btnChat.AutoLoad(IDC_IRC_CHAT, this);
	m_btnNews.AutoLoad(IDC_USENET_NEWS, this);
	m_btnGopher.AutoLoad(IDC_GOPHER, this);
	m_btnPrefs.AutoLoad(IDC_PREFS, this);
	m_btnWhois.AutoLoad(IDC_WHOIS, this);

    if (m_pApp->IsUseCTL3D())  // be sure to give this form a 3D look!
    	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
    
    // Show and activate this window
	GetParentFrame()->ActivateFrame(SW_RESTORE);   
	GetParentFrame()->UpdateWindow();

	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainMenuView message handlers

void CMainMenuView::OnFinger()
{
	// TODO: Add a 'Minimize on Use' preferences option here
	GetParentFrame()->ActivateFrame(SW_SHOWMINIMIZED);
	
	// make a new document from the application's 
	// Internet Users Information document type
	
	m_pApp->GetFingerType()->OpenDocumentFile(NULL);
	return;	
}

void CMainMenuView::OnFtp()
{
	// TODO: Add a 'Minimize on Use' preferences option here
	GetParentFrame()->ActivateFrame(SW_SHOWMINIMIZED); 
	GetParentFrame()->UpdateWindow();
	
	// make a new document from the application's
	// FTP Browser document type

	m_pApp->GetFtpType()->OpenDocumentFile(NULL);
}

void CMainMenuView::OnGopher()
{
	// TODO: Add a 'Minimize on Use' preferences option here
	GetParentFrame()->ActivateFrame(SW_SHOWMINIMIZED); 
	GetParentFrame()->UpdateWindow();
	
    // Open the Gopher Center area.
	m_pApp->OnPlacesGopherCenter();  
}

void CMainMenuView::OnHelpAndInfo()
{
	m_pApp->OnPlacesHelpAndInfo();
	return;
}

void CMainMenuView::OnIRC()
{
	// TODO: Add a 'Minimize on Use' preferences option here
	GetParentFrame()->ActivateFrame(SW_SHOWMINIMIZED); 
	GetParentFrame()->UpdateWindow();
	
	m_pApp->OnPlacesChatCenter();
	return;
}

void CMainMenuView::OnMailManager()
{
	// TODO: Add a 'Minimize on Use' preferences option here
	GetParentFrame()->ActivateFrame(SW_SHOWMINIMIZED); 
	GetParentFrame()->UpdateWindow();
	
	m_pApp->OnPlacesPostOffice();
	return;
}

void CMainMenuView::OnUsenetNews()
{
	CWaitCursor wait; 
	
	// TODO: Add a 'Minimize on Use' preferences option here
	GetParentFrame()->ActivateFrame(SW_SHOWMINIMIZED); 
	GetParentFrame()->UpdateWindow();
	
	m_pApp->OnPlacesNewsgroups();
	return;	
}

void CMainMenuView::OnWhois()
{
	// TODO: Add a 'Minimize on Use' preferences option here
	GetParentFrame()->ActivateFrame(SW_SHOWMINIMIZED); 
	GetParentFrame()->UpdateWindow();
	
	// make a new document from the application's
	// InterNIC Site Information document type

	m_pApp->GetWhoisType()->OpenDocumentFile(NULL);
	return;
}      

/////////////////////////////////////////////////////////////////////////////
// CMainMenuDoc

IMPLEMENT_SERIAL(CMainMenuDoc, CDocument, 0 /* schema number*/ )

CMainMenuDoc::CMainMenuDoc()
{
}

BOOL CMainMenuDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC, MB_ICONSTOP|MB_OK);
		return FALSE;                                                  
	}
	
	SetTitle("Main Menu");
	SetModifiedFlag(FALSE);
	
	return TRUE;
}

CMainMenuDoc::~CMainMenuDoc()
{
}


BEGIN_MESSAGE_MAP(CMainMenuDoc, CDocument)
	//{{AFX_MSG_MAP(CMainMenuDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainMenuDoc serialization

void CMainMenuDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainMenuDoc::SetTitle -- keeps this window's title at "Main Menu"

void CMainMenuDoc::SetTitle(const char* pszTitle)
{
	m_strTitle = "Main Menu";
	UpdateFrameCounts();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainMenuFrm

IMPLEMENT_DYNCREATE(CMainMenuFrm, CMDIChildWnd)

CMainMenuFrm::CMainMenuFrm()
{
}

CMainMenuFrm::~CMainMenuFrm()
{
}


BEGIN_MESSAGE_MAP(CMainMenuFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMainMenuFrm)
	ON_WM_MDIACTIVATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainMenuFrm message handlers

BOOL CMainMenuFrm::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
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
    CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
    
    pApp->m_pWndMainMenu = this;
    
	return TRUE;
}

BOOL CMainMenuFrm::PreCreateWindow(CREATESTRUCT& cs)
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

void CMainMenuFrm::OnClose()
{
	// tell the application class we are no longer available -- set its
	// pointer to us to NULL
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	
	pApp->m_pWndMainMenu = NULL;
	
	CMDIChildWnd::OnClose();
}

void CMainMenuDoc::OnUpdateFileNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainMenuDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainMenuDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainMenuFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add code here to correctly call
	// CMainMenuView::OnActivateView() to correctly change toolbars
	// around to the one we set up to change to when this window appears.
	// In the meantime, this message handler is fine just the way it is
}

void CMainMenuView::OnPrefs()
{
	CWaitCursor wait;
	m_pApp->GetPrefsType()->OpenDocumentFile(NULL);
	return;
}

void CMainMenuView::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strMainMenuSect = "MainMenu";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strMainMenuSect, "MainMenuLeft", rect.left);
	pApp->WriteProfileInt(strMainMenuSect, "MainMenuRight", rect.right);
	pApp->WriteProfileInt(strMainMenuSect, "MainMenuTop", rect.top);
	pApp->WriteProfileInt(strMainMenuSect, "MainMenuBottom", rect.bottom);
	pApp->WriteProfileInt(strMainMenuSect, "MainMenuCX", rect.Width());
	pApp->WriteProfileInt(strMainMenuSect, "MainMenuCY", rect.Height());	
}
