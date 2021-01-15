// askstaff.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"   

#include "resource.h"
#include "mail.h"
#include "news.h"

#include "mainfrm.h"

#include "askstaff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAskStaffView

IMPLEMENT_DYNCREATE(CAskStaffView, CFormView)

CAskStaffView::CAskStaffView()
	: CFormView(CAskStaffView::IDD)
{
	//{{AFX_DATA_INIT(CAskStaffView)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT          '
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
}

CAskStaffView::~CAskStaffView()
{
}

void CAskStaffView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAskStaffView)
	DDX_Control(pDX, IDC_TECH_SUPPORT, m_btnTechSupport);
	DDX_Control(pDX, IDC_SUGGESTION_BOX, m_btnSuggestionBox);
	DDX_Control(pDX, IDC_ISP_TECH_SUPPORT, m_btnISPSupport);
	DDX_Control(pDX, IDC_FEEDBACK_WIZARD, m_btnFeedback);
	DDX_Control(pDX, IDC_ASKSTAFF, m_btnAskStaff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAskStaffView, CFormView)
	//{{AFX_MSG_MAP(CAskStaffView)
	ON_BN_CLICKED(IDC_TECH_SUPPORT, OnTechSupport)
	ON_BN_CLICKED(IDC_SUGGESTION_BOX, OnSuggestionBox)
	ON_BN_CLICKED(IDC_ISP_TECH_SUPPORT, OnISPTechSupport)
	ON_BN_CLICKED(IDC_FEEDBACK_WIZARD, OnFeedbackWizard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAskStaffView initial update

void CAskStaffView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
    CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
    
	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
	
	m_btnTechSupport.AutoLoad(IDC_TECH_SUPPORT, this);
    m_btnSuggestionBox.AutoLoad(IDC_SUGGESTION_BOX, this);
	m_btnFeedback.AutoLoad(IDC_FEEDBACK_WIZARD, this);
	m_btnAskStaff.AutoLoad(IDC_ASKSTAFF, this);
	m_btnISPSupport.AutoLoad(IDC_ISP_TECH_SUPPORT, this);

	UpdateData(FALSE);

	GetDocument()->SetTitle("Ask the Staff");
	GetParentFrame()->SetWindowText("Ask the Staff"); 
	
	// Now, we're done being initialized, so show ourselves to the user
	GetParentFrame()->ActivateFrame(SW_SHOW);
	GetParentFrame()->UpdateWindow();
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CAskStaffView message handlers


void CAskStaffView::OnTechSupport()
{
	CWaitCursor wait;
	CMultiDocTemplate* pTempl = m_pApp->GetComposeMailType();
	
	// For now, we will bring up a Compose Mail window with the title
	// "Ask the Staff"
	CString strMailPath = m_pApp->m_strDefMailboxPath;
	

	CMailBoxDoc* pDocument = (CMailBoxDoc*)pTempl->CreateNewDocument();
	if (pDocument == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CComposeWnd* pFrame = 
		(CComposeWnd*)pTempl->CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);    
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	if (strMailPath.IsEmpty())
	{
		if (!m_pApp->IsMailSetupNew())
		{
			pFrame->DestroyWindow();
			return;                 
		}
		
		switch (AfxMessageBox(IDP_SETUP_NEW_MAILBOX,
					MB_ICONQUESTION|MB_YESNO))
		{
			case IDYES:
				if (!pDocument->OnNewDocument())
				{
					// user has be alerted to what failed in OnNewDocument
					TRACE0("CDocument::OnNewDocument returned FALSE\n");
					pFrame->DestroyWindow();
				}
				break;
			
			case IDNO:
				m_pStatusBar->ShowIdleMessage(); 
				pFrame->DestroyWindow();
				break;
		}
		return;
	}
	else
	{
		// open an existing document
		BeginWaitCursor();
		if (!pDocument->OnOpenDocument(strMailPath))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			pFrame->DestroyWindow();
			EndWaitCursor();
			return;
		}
		pDocument->SetPathName(strMailPath);
		EndWaitCursor();
	}

	pFrame->m_bIsUseCompWizard = FALSE;
	pFrame->m_bIsUseFeedbackWizard = FALSE;

	pFrame->m_bIsInetnavTechSupport = TRUE;
	pFrame->m_bIsISPTechSupport = FALSE; 
	pFrame->m_bIsUseCompWizard = FALSE;     
	pFrame->m_bIsUseFeedbackWizard = FALSE;  
	pFrame->OnUpdateFrameTitle(FALSE); 

	pFrame->GetWnDBSoftHelpBar().ShowWindow(SW_SHOW);
	pFrame->GetWnDBSoftHelpBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	pFrame->GetRulerBar().ShowWindow(SW_SHOW);
	pFrame->GetRulerBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	pFrame->GetInfoBar().SetSubject("Internet Navigator "
		"Technical Support Request");
	pFrame->GetInfoBar().SetTO(
		m_pApp->GetProfileString("Ask the Staff", "WnDBSoft Address",
			"WnDBSoft@aol.com")); 
	pFrame->GetInfoBar().UpdateData(FALSE);
			
	pTempl->InitialUpdateFrame(pFrame, pDocument, TRUE);

	CComposeView* pView = (CComposeView*)pFrame->GetActiveView();

	return;
}

void CAskStaffView::OnSuggestionBox()
{
	CWaitCursor wait;
	CMultiDocTemplate* pTempl = m_pApp->GetComposeMailType();
	
	// For now, we will bring up a Compose Mail window with the title
	// "Suggestion Box"
	CString strMailPath = m_pApp->m_strDefMailboxPath;
	

	CMailBoxDoc* pDocument = (CMailBoxDoc*)pTempl->CreateNewDocument();
	if (pDocument == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CComposeWnd* pFrame = 
		(CComposeWnd*)pTempl->CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);    
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	if (strMailPath.IsEmpty())
	{
		if (!m_pApp->IsMailSetupNew())
		{
			pFrame->DestroyWindow();
			return;                 
		}
		
		switch (AfxMessageBox(IDP_SETUP_NEW_MAILBOX,
					MB_ICONQUESTION|MB_YESNO))
		{
			case IDYES:
				if (!pDocument->OnNewDocument())
				{
					// user has be alerted to what failed in OnNewDocument
					TRACE0("CDocument::OnNewDocument returned FALSE\n");
					pFrame->DestroyWindow();
				}
				break;
			
			case IDNO:
				m_pStatusBar->ShowIdleMessage(); 
				pFrame->DestroyWindow();
				break;
		}
		return;
	}
	else
	{
		// open an existing document
		BeginWaitCursor();
		if (!pDocument->OnOpenDocument(strMailPath))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			pFrame->DestroyWindow();
			EndWaitCursor();
			return;
		}
		pDocument->SetPathName(strMailPath);
		EndWaitCursor();
	}

	pFrame->m_bIsUseCompWizard = FALSE; 
	pFrame->m_bIsUseFeedbackWizard = FALSE;

	pFrame->m_bIsInetnavTechSupport = FALSE;
	pFrame->m_bIsISPTechSupport = FALSE;
	pFrame->m_bIsSuggestionBox = TRUE;
	pFrame->OnUpdateFrameTitle(FALSE); 
		
	pFrame->GetSuggBar().ShowWindow(SW_SHOW);
	pFrame->GetSuggBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	pFrame->GetRulerBar().ShowWindow(SW_SHOW);
	pFrame->GetRulerBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	pFrame->GetInfoBar().SetSubject("Internet Navigator Suggestion");
	pFrame->GetInfoBar().SetTO(
		m_pApp->GetProfileString("Ask the Staff", "WnDBSoft Address")); 
	pFrame->GetInfoBar().UpdateData(FALSE);
		
	pTempl->InitialUpdateFrame(pFrame, pDocument, TRUE);

	return;
}

void CAskStaffView::OnISPTechSupport()
{
	CWaitCursor wait;
	CMultiDocTemplate* pTempl = m_pApp->GetComposeMailType();
	
	// For now, we will bring up a Compose Mail window with the title
	// "Ask the Staff"
	CString strMailPath = m_pApp->m_strDefMailboxPath;
	

	CMailBoxDoc* pDocument = (CMailBoxDoc*)pTempl->CreateNewDocument();
	if (pDocument == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CComposeWnd* pFrame = 
		(CComposeWnd*)pTempl->CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);    
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	if (strMailPath.IsEmpty())
	{
		if (!m_pApp->IsMailSetupNew())
		{
			pFrame->DestroyWindow();
			return;                 
		}
		
		switch (AfxMessageBox(IDP_SETUP_NEW_MAILBOX,
					MB_ICONQUESTION|MB_YESNO))
		{
			case IDYES:
				if (!pDocument->OnNewDocument())
				{
					// user has be alerted to what failed in OnNewDocument
					TRACE0("CDocument::OnNewDocument returned FALSE\n");
					pFrame->DestroyWindow();
				}
				break;
			
			case IDNO:
				m_pStatusBar->ShowIdleMessage(); 
				pFrame->DestroyWindow();
				break;
		}
		return;
	}
	else
	{
		// open an existing document
		BeginWaitCursor();
		if (!pDocument->OnOpenDocument(strMailPath))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			pFrame->DestroyWindow();
			EndWaitCursor();
			return;
		}
		pDocument->SetPathName(strMailPath);
		EndWaitCursor();
	}

	pFrame->m_bIsUseCompWizard = FALSE;

	pFrame->m_bIsInetnavTechSupport = FALSE;
	pFrame->m_bIsSuggestionBox = FALSE;
	pFrame->m_bIsISPTechSupport = TRUE; 
	pFrame->m_bIsUseCompWizard = FALSE;
	pFrame->m_bIsUseFeedbackWizard = FALSE;
	pFrame->OnUpdateFrameTitle(FALSE);

	pFrame->GetISPHelpBar().ShowWindow(SW_SHOW);
	pFrame->GetISPHelpBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	pFrame->GetRulerBar().ShowWindow(SW_SHOW);
	pFrame->GetRulerBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	pFrame->GetInfoBar().SetSubject("Internet Provider Support Request");
	pFrame->GetInfoBar().SetTO(
		m_pApp->GetProfileString("Ask the Staff", "ISP Address")); 
	pFrame->GetInfoBar().SetBCC(
		m_pApp->GetProfileString("Ask the Staff", "WnDBSoft Address")
			+ ", hartgc@ix.netcom.com");
	pFrame->GetInfoBar().UpdateData(FALSE);
		
	pTempl->InitialUpdateFrame(pFrame, pDocument, TRUE);
	return;
}

void CAskStaffView::OnFeedbackWizard()
{
	CWaitCursor wait;
	CMultiDocTemplate* pTempl = m_pApp->GetComposeMailType();
	
	// For now, we will bring up a Compose Mail window with the title
	// "Suggestion Box"
	CString strMailPath = m_pApp->m_strDefMailboxPath;
	

	CMailBoxDoc* pDocument = (CMailBoxDoc*)pTempl->CreateNewDocument();
	if (pDocument == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CComposeWnd* pFrame = 
		(CComposeWnd*)pTempl->CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);    
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	if (strMailPath.IsEmpty())
	{
		if (!m_pApp->IsMailSetupNew())
		{
			pFrame->DestroyWindow();
			return;                 
		}
		
		switch (AfxMessageBox(IDP_SETUP_NEW_MAILBOX,
					MB_ICONQUESTION|MB_YESNO))
		{
			case IDYES:
				if (!pDocument->OnNewDocument())
				{
					// user has be alerted to what failed in OnNewDocument
					TRACE0("CDocument::OnNewDocument returned FALSE\n");
					pFrame->DestroyWindow();
				}
				break;
			
			case IDNO:
				m_pStatusBar->ShowIdleMessage(); 
				pFrame->DestroyWindow();
				break;
		}
		return;
	}
	else
	{
		// open an existing document
		BeginWaitCursor();
		if (!pDocument->OnOpenDocument(strMailPath))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			pFrame->DestroyWindow();
			EndWaitCursor();
			return;
		}
		pDocument->SetPathName(strMailPath);
		EndWaitCursor();
	}

	pFrame->m_bIsUseCompWizard = FALSE; 
	pFrame->m_bIsUseFeedbackWizard = TRUE;

	pFrame->m_bIsInetnavTechSupport = FALSE;
	pFrame->m_bIsISPTechSupport = FALSE;
	pFrame->m_bIsSuggestionBox = TRUE; 
	pFrame->m_bIsUseCompWizard = FALSE;
	pFrame->m_bIsUseFeedbackWizard = TRUE;  
	pFrame->OnUpdateFrameTitle(FALSE);

	pFrame->GetSuggBar().ShowWindow(SW_SHOW);
	pFrame->GetSuggBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	pFrame->GetRulerBar().ShowWindow(SW_SHOW);
	pFrame->GetRulerBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	pFrame->GetInfoBar().SetSubject("Internet Navigator Feedback");
	pFrame->GetInfoBar().SetTO(
		m_pApp->GetProfileString("Ask the Staff", "WnDBSoft Address"));
	pFrame->GetInfoBar().SetBCC(
		m_pApp->GetProfileString("Ask the Staff", "WnDBSoft Address")
			+ ", hartgc@ix.netcom.com"); 
	pFrame->GetInfoBar().UpdateData(FALSE);
		
	pTempl->InitialUpdateFrame(pFrame, pDocument, TRUE);

	return;
}               

/////////////////////////////////////////////////////////////////////////////
// CAskStaffDoc dummy document

IMPLEMENT_SERIAL(CAskStaffDoc, CDocument, 0 /* schema number*/ )

CAskStaffDoc::CAskStaffDoc()
{
}

BOOL CAskStaffDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	SetTitle("Ask the Staff");    
	return TRUE;
}

CAskStaffDoc::~CAskStaffDoc()
{
}


BEGIN_MESSAGE_MAP(CAskStaffDoc, CDocument)
	//{{AFX_MSG_MAP(CAskStaffDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAskStaffDoc serialization

void CAskStaffDoc::Serialize(CArchive& ar)
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
// CAskStaffDoc overrides

void CAskStaffDoc::SetTitle(const char* pszTitle)
{
	// We'll ignore pszTitle and set our own instead
	m_strTitle = "Ask the Staff";
	UpdateFrameCounts();
}

/////////////////////////////////////////////////////////////////////////////
// CAskStaffDoc message handlers

void CAskStaffDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// Always disable Save menu item and button
	pCmdUI->Enable(FALSE);
}

void CAskStaffDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	// Always disable Save As... menu item and button	
	pCmdUI->Enable(FALSE);	
}

/////////////////////////////////////////////////////////////////////////////
// CAskStaffWnd

IMPLEMENT_DYNCREATE(CAskStaffWnd, CMDIChildWnd)

CAskStaffWnd::CAskStaffWnd()
{
}

CAskStaffWnd::~CAskStaffWnd()
{
}


BEGIN_MESSAGE_MAP(CAskStaffWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAskStaffWnd)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAskStaffWnd overrides

BOOL CAskStaffWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
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

BOOL CAskStaffWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// change the window style -- we aren't using the WS_VISIBLE style here
	// because the view needs to form itself and fill in bitmap buttons
	// The view then tells this window to show itself when it is finished
	// initializing
	cs.style = WS_CHILD|WS_BORDER|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU;
	
	// return return value of base-class version of this function
	return CMDIChildWnd::PreCreateWindow(cs);
}               

/////////////////////////////////////////////////////////////////////////////
// CAskStaffWnd message handlers

int CAskStaffWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CAskStaffWnd::OnClose()
{
	CMDIChildWnd::OnClose();
}
