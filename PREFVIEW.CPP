// prefview.cpp : implementation of the CPrefsView class
//

#include "stdafx.h"
#include "inetnav.h" 
#include "dialogs.h" 

#include "prefsdoc.h"
#include "prefview.h"
#include "propshts.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefsView

IMPLEMENT_DYNCREATE(CPrefsView, CFormView)

BEGIN_MESSAGE_MAP(CPrefsView, CFormView)
	//{{AFX_MSG_MAP(CPrefsView)
	ON_BN_CLICKED(IDC_USER_INF, OnUserInf)
	ON_BN_CLICKED(IDC_APP_PREFS, OnAppPrefs)
	ON_BN_CLICKED(IDC_INET_PREFS, OnInetPrefs)
	ON_BN_CLICKED(IDC_HELP, OnClickedHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefsView construction/destruction

CPrefsView::CPrefsView()
	: CFormView(CPrefsView::IDD)
{
	//{{AFX_DATA_INIT(CPrefsView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
}

CPrefsView::~CPrefsView()
{
}

void CPrefsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefsView)
	DDX_Control(pDX, IDC_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_APP_PREFS, m_btnAppPrefs);
	DDX_Control(pDX, IDC_USER_INF, m_btnUserInf);
	DDX_Control(pDX, IDC_INET_PREFS, m_btnInetPrefs);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPrefsView diagnostics

#ifdef _DEBUG
void CPrefsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPrefsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CPrefsDoc* CPrefsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPrefsDoc)));
	return (CPrefsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrefsView message handlers

void CPrefsView::OnInitialUpdate()
{
	CPrefsDoc* pDoc = GetDocument();
	
	pDoc->SetTitle("Preferences"); 

	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
	
	m_btnAppPrefs.LoadBitmaps("PREFSU", "PREFSD", "PREFSF", "PREFSX");
	m_btnInetPrefs.LoadBitmaps("PREFSU", "PREFSD", "PREFSF", "PREFSX");
	m_btnUserInf.LoadBitmaps("PREFSU", "PREFSD", "PREFSF", "PREFSX");
    m_btnHelp.LoadBitmaps("HELPINFOU", "HELPINFOD", "HELPINFOF", "HELPINFOX");
    
  	CFormView::OnInitialUpdate();
  	
  	m_btnAppPrefs.SizeToContent();
  	m_btnInetPrefs.SizeToContent();
  	m_btnUserInf.SizeToContent();
  	m_btnHelp.SizeToContent();
	
	pDoc->SetTitle("Preferences");       
	GetParentFrame()->SetWindowText("Preferences");
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CPrefsWnd

IMPLEMENT_DYNCREATE(CPrefsWnd, CMDIChildWnd)

CPrefsWnd::CPrefsWnd()
{ 
	// Initialize our pointer to CInternetNavApp
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

CPrefsWnd::~CPrefsWnd()
{                                          
	// Make our pointer to CInternetNavApp invalid
	m_pApp = NULL;
}


BEGIN_MESSAGE_MAP(CPrefsWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CPrefsWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPrefsWnd message handlers

BOOL CPrefsWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 108;
	windowRect.right = 532;
	windowRect.top = 50;
	windowRect.bottom = 349;
	 
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

BOOL CPrefsWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// change the window style -- we aren't using the WS_VISIBLE style here
	// because the view needs to form itself and fill in bitmap buttons
	// The view then tells this window to show itself when it is finished
	// initializing
	cs.style = WS_CHILD|WS_BORDER|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU;
	
	// return return value of base-class version of this function
	return CMDIChildWnd::PreCreateWindow(cs);
} 

int CPrefsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Give the application class a pointer to this window
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	
	pApp->m_pWndPrefs = this;
	
	return 0;
}

void CPrefsWnd::OnClose()
{   
	// tell the application class this window is no longer available --
	// we set it's pointer to us to NULL
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	
	m_pApp->m_pWndPrefs = NULL;

	CMDIChildWnd::OnClose();
}

void CPrefsView::OnUserInf()
{
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	
	// Show the user the CUserInformation dialog declared in DIALOGS.H
	// and implemented in DIALOGS.CPP
	CUserInformation theInfo(pApp->GetMainFrame());
	
	if (theInfo.DoModal() == IDOK)
		return;
}

void CPrefsView::OnAppPrefs()
{
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	CAppPrefsSheet appSheet(pApp->GetMainFrame());
	appSheet.Serialize(FALSE); // load current Preferences
	
	if (appSheet.DoModal() == IDOK)
		appSheet.Serialize(TRUE); // save changed Preferences
	return;
}

void CPrefsView::OnInetPrefs()
{
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	CInetPrefsSheet inetSheet(pApp->GetMainFrame());
	inetSheet.Serialize(FALSE); // load current Preferences into prop sheet
	
	if (inetSheet.DoModal() == IDOK) 
		inetSheet.Serialize(TRUE);  // save changed Preferences to app
	return;
}

void CPrefsView::OnClickedHelp()
{
	// Since we haven't authored help for the Preferences window yet,
	// just go to the default help, which is the Help Contents
	((CInternetNavApp*)AfxGetApp())->WinHelp(0L, HELP_CONTENTS);
}

void CPrefsWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
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
			if (!AfxGetMainWnd()->SendMessage(WM_COMMANDHELP, 0,
				HID_BASE_COMMAND+ID_COMMAND_FROM_SC(nID & 0xFFF0))) 
			{
				if (!SendMessage(WM_COMMAND, ID_DEFAULT_HELP))
					AfxMessageBox(AFX_IDP_FAILED_TO_LAUNCH_HELP);
			}
			break;
		
			default:
			// don't interfere with system commands we don't know about
			CMDIChildWnd::OnSysCommand(nID, lParam);
			break;
		}
	}
	else
	{
		switch (nID & 0xFFF0)
		{
		    case SC_CLOSE:
		    	CMDIChildWnd::OnSysCommand(nID, lParam);
		    	break;
			case SC_SIZE:
				break; // do nothing
			case SC_MOVE:
				CMDIChildWnd::OnSysCommand(nID, lParam);
				break;
			case SC_MINIMIZE:
				CMDIChildWnd::OnSysCommand(nID, lParam);
				break;
			case SC_MAXIMIZE:
				break; // do nothing -- this window can't be sized or maximized
			case SC_NEXTWINDOW:
			case SC_PREVWINDOW:
			case SC_RESTORE:
			case SC_TASKLIST:
			CMDIChildWnd::OnSysCommand(nID, lParam);
			break;
			
			default:
			CMDIChildWnd::OnSysCommand(nID, lParam);
			break;
		}
	}
}