// helpinfo.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h" 

#include "mainfrm.h"
#include "statbar.h"

#include "mail.h"

#include "helpinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpInfoView

IMPLEMENT_DYNCREATE(CHelpInfoView, CFormView)

CHelpInfoView::CHelpInfoView()
	: CFormView(CHelpInfoView::IDD)
{
	//{{AFX_DATA_INIT(CHelpInfoView)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT       
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();	
}

CHelpInfoView::~CHelpInfoView()
{
}

void CHelpInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpInfoView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpInfoView, CFormView)
	//{{AFX_MSG_MAP(CHelpInfoView)
	ON_BN_CLICKED(IDC_APP_ABOUT, OnAppAbout)
	ON_BN_CLICKED(IDC_HELP_CONTENTS, OnHelpContents)
	ON_BN_CLICKED(IDC_INET_CLIENTS_HELP, OnInetClientsHelp)
	ON_BN_CLICKED(IDC_SEARCH_HELP, OnSearchHelp)
	ON_BN_CLICKED(IDC_TECH_SUPPORT, OnTechSupport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CHelpInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
    CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
    
	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
	
	m_btnAbout.AutoLoad(IDC_APP_ABOUT, this);
    m_btnHelpContents.AutoLoad(IDC_HELP_CONTENTS, this);
	m_btnInet.AutoLoad(IDC_INET_CLIENTS_HELP, this);
	m_btnSearch.AutoLoad(IDC_SEARCH_HELP, this);
	m_btnTechHelp.AutoLoad(IDC_TECH_SUPPORT, this);
	m_btnLabel.AutoLoad(IDC_HELPINF, this);

	UpdateData(FALSE);

	GetDocument()->SetTitle("Help & Information");
	GetParentFrame()->SetWindowText("Help & Information"); 
	
	// Now, we're done being initialized, so show ourselves to the user
	GetParentFrame()->ActivateFrame(SW_SHOW);
	GetParentFrame()->UpdateWindow();
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CHelpInfoView message handlers


void CHelpInfoView::OnAppAbout()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_ABOUT);
}

void CHelpInfoView::OnHelpContents()
{
	WinHelp(0L, HELP_CONTENTS);
	return;
}

void CHelpInfoView::OnInetClientsHelp()
{
	AfxMessageBox("Sorry, this part of the Help & Information area is not available at this time.", MB_ICONINFORMATION|MB_OK);
	return;
}

void CHelpInfoView::OnSearchHelp()
{
	CWaitCursor wait;
	
	HWND hWnd = m_pApp->GetMainFrame()->GetSafeHwnd();
	if (hWnd == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_FAILED_TO_LAUNCH_HELP);
		return;
	}

	if (!::WinHelp(hWnd, m_pApp->m_pszHelpFilePath,
			HELP_PARTIALKEY, (DWORD)(LPSTR)""))
	{
		// Alert the user that we weren't able to open Help
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_FAILED_TO_LAUNCH_HELP);
		return;
	}
	return; 
}

void CHelpInfoView::OnTechSupport()
{
	CWaitCursor wait;
	m_pApp->GetAskStaff()->OpenDocumentFile(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CHelpInfoDoc

IMPLEMENT_SERIAL(CHelpInfoDoc, CDocument, 0 /* schema number*/ )

CHelpInfoDoc::CHelpInfoDoc()
{
}

BOOL CHelpInfoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CHelpInfoDoc::~CHelpInfoDoc()
{
}


BEGIN_MESSAGE_MAP(CHelpInfoDoc, CDocument)
	//{{AFX_MSG_MAP(CHelpInfoDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHelpInfoDoc serialization

void CHelpInfoDoc::Serialize(CArchive& ar)
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
// CHelpInfoWnd

IMPLEMENT_DYNCREATE(CHelpInfoWnd, CMDIChildWnd)

CHelpInfoWnd::CHelpInfoWnd()
{
}

CHelpInfoWnd::~CHelpInfoWnd()
{
}


BEGIN_MESSAGE_MAP(CHelpInfoWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CHelpInfoWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHelpInfoWnd message handlers

BOOL CHelpInfoWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
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

BOOL CHelpInfoWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// change the window style -- we aren't using the WS_VISIBLE style here
	// because the view needs to form itself and fill in bitmap buttons
	// The view then tells this window to show itself when it is finished
	// initializing
	cs.style = WS_CHILD|WS_BORDER|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU;
	
	// return return value of base-class version of this function
	return CMDIChildWnd::PreCreateWindow(cs);
} 

int CHelpInfoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Give the application class a pointer to this window
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	
	pApp->m_pWndHelpInfo = this;
	
	return 0;
}

void CHelpInfoWnd::OnClose()
{   
	// tell the application class this window is no longer available --
	// we set it's pointer to us to NULL
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	
	pApp->m_pWndHelpInfo = NULL;

	CMDIChildWnd::OnClose();
}

void CHelpInfoDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CHelpInfoDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}
