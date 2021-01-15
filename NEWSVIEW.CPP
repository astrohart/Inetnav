// newsview.cpp: implementation file
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

#include "wizards.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewsView

IMPLEMENT_DYNCREATE(CNewsView, CFormView)

BEGIN_MESSAGE_MAP(CNewsView, CFormView)
	//{{AFX_MSG_MAP(CNewsView)
	ON_BN_CLICKED(IDC_MAIL_MANAGER, OnMailManager)
	ON_BN_CLICKED(IDC_HELPINFO, OnHelpinfo)
	ON_BN_CLICKED(IDC_NEWSGROUPS_SETUP, OnNewsgroupsSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_BN_CLICKED(IDC_GROUP_MANAGER, OnGroupManager)
	ON_COMMAND(ID_ARTICLE_ADD, OnArticleAdd)
	ON_BN_CLICKED(IDC_COMPOSE_ARTICLE, OnArticleAdd)
	//}}AFX_MSG_MAP   
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewsView construction/destruction

CNewsView::CNewsView()
	: CFormView(CNewsView::IDD)
{
	//{{AFX_DATA_INIT(CNewsView)
	m_pDoc = NULL;
	//}}AFX_DATA_INIT
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

CNewsView::~CNewsView()
{
}

void CNewsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsView)
	DDX_Control(pDX, IDC_MAIL_MANAGER, m_btnPostOffice);
	DDX_Control(pDX, IDC_COMPOSE_ARTICLE, m_btnCompose);
	DDX_Control(pDX, IDC_GROUP_MANAGER, m_btnGroupMan);
	DDX_Control(pDX, IDC_HELPINFO, m_btnHelp);
	DDX_Control(pDX, IDC_NEWSGROUPS_SETUP, m_btnSetup);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)                 
		UpdateData(FALSE);
}

void CNewsView::OnInitialUpdate()
{
	m_pDoc = GetDocument();    // pointer to document data, including
								// server addresses  
	
	((CNewsWnd*)GetParentFrame())->OnUpdateFrameTitle(TRUE);
	
	// We will set the bitmaps for the buttons in this area.
	m_btnPostOffice.LoadBitmaps("POFFICEU", "POFFICED", "POFFICEF", "POFFICEX");
	m_btnCompose.LoadBitmaps("NEWSU", "NEWSD", "NEWSF", "NEWSX");
	m_btnSetup.LoadBitmaps("NEWSU", "NEWSD", "NEWSF", "NEWSX");
	m_btnGroupMan.LoadBitmaps("GROUPMANU", "GROUPMAND", "GROUPMANF", "GROUPMANX");
	m_btnHelp.LoadBitmaps("HELPINFOU", "HELPINFOD", "HELPINFOF",
		"HELPINFOX");
		
	CFormView::OnInitialUpdate();

	// Now resize each button so that its size matches its bitmap image.
	m_btnPostOffice.SizeToContent();
	m_btnCompose.SizeToContent();
	m_btnSetup.SizeToContent();
	m_btnGroupMan.SizeToContent();
	m_btnHelp.SizeToContent();

	// Now subclass with CTL3D so this area looks good
	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
	
	m_btnCompose.EnableWindow(m_pDoc->CanPost());
	
	// Tell the caller of this function that we're done.
	#ifdef _DEBUG
		afxDump << "Internet Newsgroups area is coming online." << "\r\n";
	#endif
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CNewsView diagnostics

#ifdef _DEBUG
void CNewsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNewsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNewsDoc* CNewsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewsDoc)));
	return (CNewsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNewsView message handlers

void CNewsView::OnNewsgroupsSetup()
{
	// Show the Newsgroups Settings dialog box
	m_pDoc->OnNewsgroupsSetup();
}

void CNewsView::OnMailManager()
{
	m_pApp->OnPlacesPostOffice();
	return;
}

void CNewsView::OnHelpinfo()
{	
	// Show help on this window
	CWnd::OnHelp();
}

void CNewsView::OnGroupManager()
{ 
	CWaitCursor wait;
	// Show the m_pApp->m_pNewsgroupsManager template using the same document
	CNewsgroupsManagerWnd* pFrame;
	CMultiDocTemplate* pTempl = m_pApp->GetNewsgroupsManager();
	
	pFrame = (CNewsgroupsManagerWnd*)pTempl->CreateNewFrame(m_pDoc, NULL);
	if (pFrame == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	}
	
	// We've now successfully created the frame window for the Newsgroups
	// Manager, so let's now initialize it.  The below function call also
	// calls CNewsgroupsManager::OnInitialUpdate()
	pTempl->InitialUpdateFrame(pFrame, m_pDoc, TRUE);
}

void CNewsView::OnUpdateFilePrint(CCmdUI* pCmdUI)
{
	// we don't want the user to be able to print anything -- to them, this
	// is just another area of Internet Navigator.
	pCmdUI->Enable(FALSE);
}

void CNewsView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	// we don't want the user to be able to print anything -- to them, this
	// is just another area of Internet Navigator.
	pCmdUI->Enable(FALSE);
}

void CNewsView::OnArticleAdd()
{
	CArticleComposeWnd* pFrame = 
		(CArticleComposeWnd*)m_pApp->GetCNA()->CreateNewFrame(m_pDoc, NULL);
	if (pFrame == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;                  
	}
	m_pApp->GetCNA()->InitialUpdateFrame(pFrame, m_pDoc, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManager

IMPLEMENT_DYNCREATE(CNewsgroupsManager, CFormView)

CNewsgroupsManager::CNewsgroupsManager()
	: CFormView(CNewsgroupsManager::IDD)
{
	//{{AFX_DATA_INIT(CNewsgroupsManager)
	m_pDoc = NULL;
	//}}AFX_DATA_INIT
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_pPercentDialog = NULL;	
}                                                           

CNewsgroupsManager::~CNewsgroupsManager()
{ 
	// If necessary, delete the percent gauge dialog box     
	if (m_pPercentDialog && IsWindow(m_pPercentDialog->GetSafeHwnd()))
	{
		m_pPercentDialog->ShowWindow(SW_HIDE);
		m_pPercentDialog->UpdateWindow();
		
		m_pPercentDialog->DestroyWindow();
	}
}

void CNewsgroupsManager::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsgroupsManager)
	DDX_Control(pDX, IDC_NEWSGROUPS_LIST, m_ctlGroupList);
	DDX_LBString(pDX, IDC_NEWSGROUPS_LIST, m_strSelection);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CNewsgroupsManager, CFormView)
	//{{AFX_MSG_MAP(CNewsgroupsManager)
	ON_COMMAND(ID_GROUP_ADD, OnGroupAdd)
	ON_LBN_ERRSPACE(IDC_NEWSGROUPS_LIST, OnListOutOfSpace)
	ON_COMMAND(ID_GROUP_REMOVE, OnGroupRemove)
	ON_UPDATE_COMMAND_UI(ID_GROUP_REMOVE, OnUpdateGroupRemove)
	ON_COMMAND(ID_GROUP_OPEN, OnGroupOpen)
	ON_UPDATE_COMMAND_UI(ID_GROUP_OPEN, OnUpdateGroupOpen)
	ON_LBN_DBLCLK(IDC_NEWSGROUPS_LIST, OnGroupOpen)
	ON_COMMAND(ID_ARTICLE_AGE, OnArticleAge)
	//}}AFX_MSG_MAP
	// Standard Help command
	ON_COMMAND(ID_HELP, CWnd::OnHelp)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Idle ON_UPDATE_COMMAND_UI invoke message
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManager update handlers

void CNewsgroupsManager::OnInitialUpdate()
{
	m_pDoc = GetDocument();
	
	((CNewsgroupsManagerWnd*)GetParentFrame())->OnUpdateFrameTitle(TRUE);
			
	CFormView::OnInitialUpdate();

	if (m_pApp->IsUseCTL3D())
		Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);

	// We need to load the subscribed newsgroup list in from the document
	// here.   
	if (m_pDoc->m_strGroupsArray.GetSize() == 0) return;

	for (int i = 0;i < m_pDoc->m_strGroupsArray.GetSize();i++)
	{
		m_ctlGroupList.AddString(m_pDoc->m_strGroupsArray[i]);
	}	

	return; // return for now
}

void CNewsgroupsManager::OnUpdate(CView*, LPARAM, CObject*)
{
	// Just update the data in this view
	if (!UpdateData(FALSE))
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP|MB_OK);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManager printing
// Here's how we're going to implement printing for this view:
// What to print: The newsgroup list
// How to do it: Iterate through the array in the document
// and insert each newsgroup name into a mega string which
// we draw on the printer with CDC::DrawText().  Between each
// of the newsgroup names we insert a CRLF pair so they are
// broken into lines.

BOOL CNewsgroupsManager::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->m_nNumPreviewPages = 2;  
	pInfo->SetMaxPage(0xFFFF);
	return DoPreparePrinting(pInfo);
}

void CNewsgroupsManager::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFormView::OnBeginPrinting(pDC, pInfo);
}

void CNewsgroupsManager::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pDoc->m_strGroupsArray.GetSize() == 0)
		return;
		
	// Form and print an informational header first
	CString strHeader = "Internet Newsgroups: Subscribed Newsgroups List";
	PrintPageHeader(pDC, pInfo, strHeader);
	
	// Now prepare the mega-string
	CString strList = m_pDoc->m_strGroupsArray[0] + "\r\n";
	if (m_pDoc->m_strGroupsArray.GetSize() > 1)
	{
		for (int i = 0;i < m_pDoc->m_strGroupsArray.GetSize();i++)
		{
			strList += m_pDoc->m_strGroupsArray[i] + "\r\n";	
		}
	}	
	
	// Now draw the list on the printer device context with
	// CDC::DrawText()
	pDC->DrawText(strList, -1, pInfo->m_rectDraw,
		DT_LEFT|DT_WORDBREAK);
	
	// we're done with printing!
	return;	
}

void CNewsgroupsManager::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFormView::OnEndPrinting(pDC, pInfo);
}

void CNewsgroupsManager::PrintPageHeader(CDC* pDC, CPrintInfo* pInfo,
			CString& strHeader)
{
	// Print a page header consisting of the name of
	// the document and a horizontal line
	pDC->TextOut(0, 25, strHeader);  // 1/4 inch down

	// Draw a line across the page, below the header
	TEXTMETRIC textMetric;
	pDC->GetTextMetrics(&textMetric);
	int y = 35 + textMetric.tmHeight;          // line 1/10th inch below text
	pDC->MoveTo(0, y);                          // from left margin
	pDC->LineTo(pInfo->m_rectDraw.right, y);    // to right margin
            
	// Subtract out from the drawing rectange the space used by the header.
	y += 25;    // space 1/4 inch below (top of) line
	pInfo->m_rectDraw.top += y; 
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManager diagnostics
#ifdef _DEBUG

CNewsDoc* CNewsgroupsManager::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewsDoc)));
	return (CNewsDoc*)m_pDocument;
}

#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManager idle button update

LRESULT CNewsgroupsManager::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{   
	// Update controls and buttons in this window
	UpdateDialogControls(this, TRUE);
	return 0;	
}

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManager message handlers

void CNewsgroupsManager::OnGroupAdd()
{  
	QSocket* pSocket = m_pDoc->GetSocket();
	
	if (pSocket == NULL)
	{
		AfxMessageBox(IDP_CONNECTION_BUSY,
			MB_ICONASTERISK|MB_OK);
		return;
	}
	
	CGroupSubscribe theDialog(AfxGetMainWnd(), pSocket); 
	theDialog.m_pDoc = m_pDoc;
	
	if (theDialog.DoModal() == IDOK)
	{
		m_pDoc->m_strGroupsArray.Add(theDialog.m_strNewsgroup);
		
		// We've changed the document's data, so mark the document as
		// modified
		m_pDoc->SetModifiedFlag();
		m_ctlGroupList.AddString(theDialog.m_strNewsgroup);
		m_pDoc->ReleaseSocket(); // make our socket available again
		return;
	} 

	if (!m_pDoc->DoSave(m_pDoc->m_strPathName, TRUE))
	{
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_FAILED_TO_SAVE_DOC, MB_ICONSTOP);
		m_pDoc->ReleaseSocket();
		return;
	}
			
	m_pDoc->ReleaseSocket(); // make our socket available again  
	return;
}  

void CNewsgroupsManager::OnGroupRemove()
{
	//if (m_pApp->m_bNewsAskBeforeRemove)     (a future Preference)
	CString strMsg = "Are you sure you want to remove this group from the ";
	strMsg += "list?";
	if (AfxMessageBox(strMsg, MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	// We want to remove the selected group from the list box and
	// the m_sttrGroupsArray array.
	UpdateData(TRUE);
	int nCurSel = m_ctlGroupList.GetCurSel();
	m_pStatusBar->SetText("Removing " + m_strSelection + "...");
	// Use nCurSel as the zero-based index into the array, since the indexing
	// systems of the list box and the array are the same
	
	// First, remove the newsgroup name from the newsgroups array
	m_pDoc->m_strGroupsArray.RemoveAt(nCurSel);
	m_pDoc->SetModifiedFlag();
	
	// Next, remove the group name from the list
	m_ctlGroupList.DeleteString(nCurSel);
	m_ctlGroupList.UpdateWindow();
	m_pStatusBar->ShowIdleMessage();
	return;
}

void CNewsgroupsManager::OnArticleAge()
{
	CArticleAge theDlg(m_pApp->GetMainFrame());
	m_pApp->Serialize(FALSE);
	theDlg.m_lDays = m_pApp->GetArticleDays();
	theDlg.m_nHours = m_pApp->GetArticleHours();
	theDlg.m_nMinutes = m_pApp->GetArticleMinutes();
	theDlg.m_nSeconds = m_pApp->GetArticleSeconds();
	
	if (theDlg.DoModal() == IDOK)
	{
		m_pApp->SetArticleDays(theDlg.m_lDays);
		m_pApp->SetArticleHours(theDlg.m_nHours);
		m_pApp->SetArticleMinutes(theDlg.m_nMinutes);
		m_pApp->SetArticleSeconds(theDlg.m_nSeconds);
		m_pApp->Serialize(TRUE);
	}
	return;
}

void CNewsgroupsManager::OnUpdateGroupRemove(CCmdUI* pCmdUI)
{
	// Enable this command only if the list box has more than 0 items
	// and one of the items is selected.
	pCmdUI->Enable((m_pDoc->m_strGroupsArray.GetSize() > 0) &&
				(m_ctlGroupList.GetCurSel() >= 0));
}

void CNewsgroupsManager::OnGroupOpen()
{
	if (!m_pApp->IsOnline())
	{
		AfxMessageBox(IDP_INETNAV_SIGN_ON, MB_ICONSTOP|MB_OK);
		return;                                               
	}
	
	if (!UpdateData(TRUE))
	{
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP|MB_OK);
		return;
	}
		
	// Open a window which will have the same title as the name of the
	// selected group, plus a list of the group's articles
	if (!m_pDoc->IsSocketAvailable())
	{
		AfxMessageBox(IDP_CONNECTION_BUSY,
			MB_ICONASTERISK|MB_OK);
		return;
	}
	
	int nIndex = m_ctlGroupList.GetCurSel();
	if (nIndex < 0)
	{
		// User hasn't selected a group -- use the first one.
		m_ctlGroupList.SetCurSel(0);
		nIndex = 0;
		UpdateData(TRUE);
	}
	
	CString strText = "Opening " + m_strSelection + "...  Please wait.";
	m_pStatusBar->SetText(strText);
	
	CArticleSelectionWnd* pFrame =
	(CArticleSelectionWnd*)m_pApp->GetNewsgroup()->CreateNewFrame(m_pDoc, NULL);
	if (pFrame == NULL)   
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;             
	}  
	
	m_pStatusBar->ShowIdleMessage();
	pFrame->m_strGroup = m_strSelection;
	m_pApp->GetNewsgroup()->InitialUpdateFrame(pFrame, m_pDoc, TRUE); 
	
	pFrame->ActivateFrame(SW_RESTORE);
			 
	return;
}

void CNewsgroupsManager::OnUpdateGroupOpen(CCmdUI* pCmdUI)
{
	// Enable this command only if the list box has more than 0 items
	// and one of the items is selected.
	pCmdUI->Enable((m_pDoc->m_strGroupsArray.GetSize() > 0) &&
			(m_ctlGroupList.GetCurSel() >= 0) &&
				m_pApp->IsOnline());
}

void CNewsgroupsManager::OnListOutOfSpace()
{
	// Tell the user there's not enough space left in the listbox for any
	// more group names
	AfxMessageBox("Can't add more newsgroup names, no space left in listbox.",
		MB_ICONSTOP|MB_OK, AFX_IDP_FAILED_MEMORY_ALLOC);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionView

IMPLEMENT_DYNCREATE(CArticleSelectionView, CFormView)

BEGIN_MESSAGE_MAP(CArticleSelectionView, CFormView)
	//{{AFX_MSG_MAP(CArticleSelectionView)
	ON_COMMAND(ID_WINDOW_ARTICLE_REFRESH, OnWindowRefresh)
	ON_COMMAND(ID_ARTICLE_OPEN, OnArticleOpen)
	ON_UPDATE_COMMAND_UI(ID_ARTICLE_OPEN, OnUpdateArticleOpen)
	ON_COMMAND(ID_ARTICLE_AGE, OnArticleAge)
	ON_BN_CLICKED(IDC_WINDOW_REFRESH, OnWindowRefresh)
	ON_LBN_DBLCLK(IDC_ARTICLES_LIST, OnArticleOpen)  
	ON_BN_CLICKED(ID_ARTICLE_ADD, OnArticleAdd)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWnd::OnHelp)          
	ON_COMMAND(ID_ARTICLE_ADD, OnArticleAdd)
	// Idle Button Update
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionView construction/destruction

CArticleSelectionView::CArticleSelectionView()
	: CFormView(CArticleSelectionView::IDD)
{
	//{{AFX_DATA_INIT(CArticleSelectionView)
	m_strSelection = "";
	m_strGroupLabel = "";
	//}}AFX_DATA_INIT
	m_pPercentDialog = NULL;
	m_pDoc = NULL;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
}

CArticleSelectionView::~CArticleSelectionView()
{
	// If necessary, delete the percent gauge dialog box     
	if (m_pPercentDialog && IsWindow(m_pPercentDialog->GetSafeHwnd()))
	{
		m_pPercentDialog->ShowWindow(SW_HIDE);
		m_pPercentDialog->UpdateWindow();
		
		m_pPercentDialog->DestroyWindow();
	}
}

void CArticleSelectionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArticleSelectionView)
	DDX_Control(pDX, IDC_ARTICLES_LIST, m_ctlArticleList);
	DDX_LBString(pDX, IDC_ARTICLES_LIST, m_strSelection);
	DDX_Text(pDX, IDC_ARTICLES_LABEL, m_strGroupLabel);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionView update

void CArticleSelectionView::OnInitialUpdate()
{
	m_pDoc = GetDocument();
	
	m_strGroup = ((CArticleSelectionWnd*)GetParentFrame())->m_strGroup;
	((CArticleSelectionWnd*)GetParentFrame())->OnUpdateFrameTitle(TRUE);
	
	// Set the title of this window to be the group name
	GetParentFrame()->SetWindowText(m_strGroup);
	
	m_strGroupLabel = "&Articles in " + m_strGroup + ":";
	
	CFormView::OnInitialUpdate();
	
	if (m_pApp->IsUseCTL3D())
		 Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
			
	m_ctlArticleList.SetTabStops(160); // dialog units
	m_ctlArticleList.SetHorizontalExtent(LOWORD(GetDialogBaseUnits())*80);
	
	DoFillListBox();

	return;
}

void CArticleSelectionView::OnUpdate(CView*, LPARAM, CObject*)
{
	if (!UpdateData(FALSE))
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP|MB_OK);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionView operations

void CArticleSelectionView::DoFillListBox()
{    
	CWaitCursor wait;

	QSocket* pSocket = m_pDoc->GetSocket();
	CString strResponse = "";
	int nLength;
	
	if (pSocket == NULL)
	{
		// Socket is busy
		MessageBeep(-1);
		AfxMessageBox(IDP_CONNECTION_BUSY,
			MB_ICONINFORMATION|MB_OK);
		
		return;
	}
			
	// make sure the newsgroups server knows which group we're interested
	// in
		
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	pSocket->SetReceiveTarget(NULL, 0);
		
	CString strText = "Please wait while we attempt to open the ";
	strText += "selected group on the server.";
	m_pStatusBar->SetText(strText);
		
	// send the GROUP command
	pSocket->Send("GROUP " + m_strGroup + "\r\n");
        
    m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
	strResponse = pSocket->GetLine();
	#ifdef _DEBUG
		afxDump << strResponse;
	#endif
		
	int nArticles, nMaxArticles;
	int nResponseCode = atoi(strResponse.Left(3));
		
	if (nResponseCode == 500)
	{
		// the news server doesn't understand or implement the
		// GROUP command; that's OK, we can still handle it
		nMaxArticles = 0;
	}
	else if (nResponseCode != 211)
	{
		// invalid group -- stop right here
		m_pStatusBar->ShowIdleMessage();   
			
		m_pDoc->DisplayNNTPError(nResponseCode);
			
		MessageBeep(-1);
		AfxMessageBox(IDP_NEWSGROUP_INVALID, MB_ICONSTOP);
			
		// Release the connection we have with the server back to the
		// area
		m_pDoc->ReleaseSocket();
			
		return;
	} 
	else
	{
		// valid group -- get number of articles
			
		// Chop off '211 '
		strResponse = strResponse.Right(strResponse.GetLength() - 4);
			
		// Get the first number right after the '211 ' -- according to
		// the RFC, this should be the number of articles in this group
		int nSpace = strResponse.Find(' ');
		strResponse = strResponse.Left(nSpace);
			
		nArticles = atoi(strResponse);
	}
		
	if (nArticles == 0)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_NEWSGROUP_EMPTY, MB_ICONSTOP);
			
		m_pStatusBar->ShowIdleMessage();
		m_pDoc->ReleaseSocket();
		return;
	}
		
	if (nArticles < 100 && nMaxArticles == 0)
	{
		nMaxArticles = 0;
		nArticles = 0;     
	}
	else if (nArticles < 100)
	{
		nMaxArticles = nArticles;
	}
	else if (nArticles >= 100)
	{
		nMaxArticles = 100;
	}
		
	m_pStatusBar->ShowIdleMessage();
		
	m_pStatusBar->SetText("Clearing old article lists...  Please wait.");
	m_pDoc->DeleteArrays();  
	m_pStatusBar->ShowIdleMessage();
		
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
		
	CTime curTime = CTime::GetCurrentTime() - 
	CTimeSpan(m_pApp->GetArticleDays(),
		  m_pApp->GetArticleHours(),
		  m_pApp->GetArticleMinutes(),
		  m_pApp->GetArticleSeconds());    

	CString strCommand = "";			  
	strCommand= "NEWNEWS ";
	strCommand += m_strGroup;
	strCommand += " ";
	strCommand += curTime.Format("%y%m%d %H%M%S");
	strCommand += "\r\n";    

	CString strMessageID = "";

	m_pApp->OnIdle(0);
	m_pApp->OnIdle(1);
		
	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	pSocket->Send(strCommand);
		
	m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
		
	strResponse = pSocket->GetLine();
	#ifdef _DEBUG
		afxDump << strResponse;
	#endif  
		
	strText = "Article request has been received and acknowledged.";
	m_pStatusBar->SetText(strText);
	nResponseCode = atoi(strResponse.Left(3));
		
	if (nResponseCode == 230)
	{
		strText = "Now receiving information on new articles in ";
		strText += m_strGroup + "...  Please wait.";
		m_pStatusBar->SetText(strText);
			
		int nMessageIDs = 0;
		// message IDs follow 
			
		char chText[30];

		m_pDoc->DeleteArrays();
			
		if (nArticles > 0 && nMaxArticles > 0)
		{
			for (int i = 1;((strResponse = pSocket->GetLine()) != ".\r\n") &&
					(pSocket->IsConnected()) && i <= nMaxArticles;i++)
			{
				/*wait.Restore();*/
				nLength = strResponse.GetLength();
				if (nLength >= 2 
					   && strResponse[nLength-2] == '\r'
					   && strResponse[nLength-1] == '\n')
				{
					strMessageID = strResponse.Left(nLength-2);
					m_pDoc->m_strMessageIDs.Add(strMessageID);	
						
					#ifdef _DEBUG
						afxDump << strMessageID << "\r\n";
					#endif                                    
					strMessageID.Empty();
						
					nMessageIDs = m_pDoc->m_strMessageIDs.GetSize();

					// pump waiting Windows messages
					for (;;)
					{
						MSG msg; 
						/*wait.Restore();*/
						if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
						{ 
							if (!m_pApp->PreTranslateMessage(&msg))
							{
								::TranslateMessage(&msg);
								::DispatchMessage(&msg);
							}  
					
							m_pApp->OnIdle(0);
							m_pApp->OnIdle(1);
						}
						else
						{    
							// to correctly handle idle processing, call
							// CWinApp::OnIdle()
							m_pApp->OnIdle(0); // update UI
							m_pApp->OnIdle(1); // delete temp objects
						
							break; // no more messages to pump
						}
					}	
				}
			}                                             
		}
		else
		{       
			for (int i = 1;((strResponse = pSocket->GetLine()) != ".\r\n")
					&& pSocket->IsConnected();i++)
			{
				/*wait.Restore();*/
				nLength = strResponse.GetLength();
				if (nLength >= 2 
					   && strResponse[nLength-2] == '\r'
					   && strResponse[nLength-1] == '\n')
				{
					strMessageID = strResponse.Left(nLength-2);
					m_pDoc->m_strMessageIDs.Add(strMessageID);	
						
					#ifdef _DEBUG
						afxDump << strMessageID << "\r\n";
					#endif                                    
					strMessageID.Empty();
						
					nMessageIDs = m_pDoc->m_strMessageIDs.GetSize();

					// pump waiting Windows messages
					for (;;)
					{
						MSG msg; 
						/*wait.Restore();*/
						if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
						{ 
							if (!m_pApp->PreTranslateMessage(&msg))
							{
								::TranslateMessage(&msg);
								::DispatchMessage(&msg);
							}  
					
							m_pApp->OnIdle(0);
							m_pApp->OnIdle(1);
						}
						else
						{    
							// to correctly handle idle processing, call
							// CWinApp::OnIdle()
							m_pApp->OnIdle(0); // update UI
							m_pApp->OnIdle(1); // delete temp objects
						
							break; // no more messages to pump
						}
					}	
				}
			}  
		}

		if (m_pDoc->m_strMessageIDs.GetSize() == 0)
		{
			MessageBeep(-1);
			AfxMessageBox(IDP_NEWSGROUP_EMPTY, MB_ICONSTOP|MB_OK);
			
			if (!m_pDoc->IsSocketAvailable()) 
				m_pDoc->ReleaseSocket();
			return;	
		}
			
		m_pStatusBar->ShowIdleMessage();
	}
	else
	{ 
		m_pDoc->DisplayNNTPError(nResponseCode);

		m_pStatusBar->ShowIdleMessage();
		m_pDoc->ReleaseSocket();
		return;
	}

	CString strLine = "", strSubject = "";
	CString strFrom = "", strTitle = "", text = "";
		
	m_pPercentDialog = new CPercentDialog(AfxGetMainWnd());
	strTitle = "Article Retrieval Progress";
	text = "Please wait while we retrieve and process the headers for ";
	text += "the articles of " + m_strGroup + ".\r\n";
	text += "Thank you for your patience!"; 
	m_pPercentDialog->Initialize(strTitle, text);
	m_pPercentDialog->ShowWindow(SW_SHOW);
	m_pPercentDialog->UpdateWindow();
		
	text.Empty();
	text = "Receiving and processing headers...  Please wait.";
	m_pStatusBar->SetText(text);  
        
    BOOL bIncremI = FALSE;
        
    int iSav = 0;
	for (int i = 0; i < m_pDoc->m_strMessageIDs.GetSize(); i++)
	{
		if (bIncremI)
		{
			i = iSav;
			bIncremI = FALSE;
		}
			
		/*wait.Restore();*/
		// build the string to send -- the HEAD command
		strCommand = "HEAD ";
		strCommand += m_pDoc->m_strMessageIDs[i];
		strCommand += "\r\n"; 

		m_pPercentDialog->CalculatePercent(i + 1, m_pDoc->m_strMessageIDs.GetSize());  
		m_pPercentDialog->UpdateWindow();
			
		pSocket->Send(strCommand);

		strResponse = pSocket->GetLine();                                          
			
		#ifdef _DEBUG
			afxDump << strResponse;
		#endif //_DEBUG                                
			
		nResponseCode = atoi(strResponse.Left(3));
			
		if (nResponseCode == 221)
		{
			strSubject = "";
			strFrom = "";
			while ( (strResponse = pSocket->GetLine()) != ".\r\n"
			       && pSocket->IsConnected())
			{  
				strText = "Receiving and processing headers...  ";
				strText += "Please wait.";
				m_pStatusBar->SetText(strText);
					
				/*wait.Restore();*/
				nLength = strResponse.GetLength();
				if (strResponse.Left(9) == "Subject: ")
				{                              
					strSubject = strResponse.Right(nLength - 9);
					nLength = strSubject.GetLength();
					if (nLength >= 2 
						   && strSubject[nLength-2] == '\r'
						   && strSubject[nLength-1] == '\n')
					{
						strSubject = strSubject.Left(nLength-2);

						m_pDoc->m_strSubjectList.Add(strSubject);
					}
				}
				if (strResponse.Left(6) == "From: ")
				{
					strFrom = strResponse.Right(nLength - 6);
					nLength = strFrom.GetLength();
					if (nLength >= 2 
						   && strFrom[nLength-2] == '\r'
						   && strFrom[nLength-1] == '\n')
					{
						strFrom = strFrom.Left(nLength-2);
						m_pDoc->m_strSenderList.Add(strFrom);
					}
				}
					
				// Call the app idle handler so the UI is updated.
				// This is important because this updates the status
				// bar's clock -- and gives the user an idea of
				// how long this is taking
				m_pApp->OnIdle(0);  
				m_pApp->OnIdle(1);
			}
				
			if (strFrom.GetLength() <= 40)
				strLine = strFrom;
			else if (strFrom.GetLength() > 40)
			{
				strLine = strFrom.Left(37); //so as not to overshoot tabstop
				strLine += "...";
			}
			strLine += '\t';  //tab -- see Create
			strLine += strSubject;     
			#ifdef _DEBUG
				afxDump << strLine << "\r\n";
			#endif //_DEBUG
				
			m_ctlArticleList.AddString(strLine);
		} 
		else // if strResponse.Left(3) == 221
		{   
			/*m_strMessageIDs.RemoveAt(i);
				
			bIncremI = TRUE;
			iSav = i;        */
				
			// diagnostics
			#ifdef _DEBUG
				afxDump << "ERR: " << m_pDoc->m_strMessageIDs[i] << " ";
				afxDump << "was not found.  Ignoring..." << "\r\n";
			#endif //_DEBUG
				
		} // if strResponse = 221 
			
		if (m_pDoc->m_strMessageIDs.GetSize() == 0)
			break;
				
	} // for loop going through m_pDoc->m_strMessageIDs
		
	m_pStatusBar->ShowIdleMessage();
		
	if (m_ctlArticleList.GetCount() == 0)
	{
		m_pDoc->DeleteArrays();
			
		MessageBeep(-1);
		AfxMessageBox(IDP_NEWSGROUP_EMPTY, MB_ICONSTOP|MB_OK);
	}
			
	m_pPercentDialog->SetPercent(100);
	m_pPercentDialog->UpdateWindow();
	m_pPercentDialog->DestroyWindow();
			
	if (!m_pDoc->IsSocketAvailable())
		m_pDoc->ReleaseSocket();
		
	// If there aren't any newsgroup articles in this newsgroup, then
	// tell the user so
	if (m_pDoc->m_strMessageIDs.GetSize() == 0)
	{  
		MessageBeep(-1);
		AfxMessageBox(IDP_NEWSGROUP_EMPTY, MB_ICONSTOP|MB_OK);
		if (!m_pDoc->IsSocketAvailable()) 
			m_pDoc->ReleaseSocket();
		return;
	}
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionView diagnostics

#ifdef _DEBUG
void CArticleSelectionView::AssertValid() const
{
	CFormView::AssertValid();
}

void CArticleSelectionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNewsDoc* CArticleSelectionView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewsDoc)));
	return (CNewsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAricleSelectionView message handlers

LRESULT CArticleSelectionView::OnIdleUpdateCmdUI(WPARAM, LPARAM)
{
	UpdateDialogControls(this, TRUE);
	return 0;
}

void CArticleSelectionView::OnWindowRefresh()
{ 
	// DoFillListBox() handles filling the listbox, so first, wipe out the
	// current contents of the listbox, then call
	// CArticleSelectionView::DoFillListBox()
	m_ctlArticleList.ResetContent();
	m_ctlArticleList.UpdateWindow();
	UpdateData(TRUE);
	
	DoFillListBox();
	return;
} 

void CArticleSelectionView::OnArticleAge()
{
	CArticleAge theDlg(m_pApp->GetMainFrame());
	m_pApp->Serialize(FALSE);
	theDlg.m_lDays = m_pApp->GetArticleDays();
	theDlg.m_nHours = m_pApp->GetArticleHours();
	theDlg.m_nMinutes = m_pApp->GetArticleMinutes();
	theDlg.m_nSeconds = m_pApp->GetArticleSeconds();
	
	if (theDlg.DoModal() == IDOK)
	{
		m_pApp->SetArticleDays(theDlg.m_lDays);
		m_pApp->SetArticleHours(theDlg.m_nHours);
		m_pApp->SetArticleMinutes(theDlg.m_nMinutes);
		m_pApp->SetArticleSeconds(theDlg.m_nSeconds);
		m_pApp->Serialize(TRUE);
	}
	return;
}

void CArticleSelectionView::OnArticleOpen()
{      
	m_pDoc = GetDocument();
	
	if (m_ctlArticleList.GetCount() == 0)
		return;
			
	if (!TestLists())
	{
		AfxMessageBox(IDP_FAILED_RETRIEVE_ARTICLE, MB_ICONSTOP);
		return; 
	}
		          
	if (!m_pDoc->IsSocketAvailable())
	{     
		// Another part of the Internet Newsgroups area is using the
		// area socket.  We'll inform the user, then skip the rest of
		// this function for now.
		AfxMessageBox(IDP_CONNECTION_BUSY,
			MB_ICONINFORMATION|MB_OK);
		return;   
	}
	
	// get the index of the article selected.
	int nArticle = m_ctlArticleList.GetCurSel();
	if (nArticle < 0)
	{
		// User hasn't selected an article -- use first article
		nArticle = 0;                                          
	}

	CString strAuthor = m_pDoc->m_strSenderList[nArticle];
	CString strSubject = m_pDoc->m_strSubjectList[nArticle];
	CString strGroup = m_strGroup;
	
	CArticleWnd* pFrame = 
		(CArticleWnd*)m_pApp->GetNewsArticle()->CreateNewFrame(m_pDoc, NULL);
	if (pFrame == NULL)
	{ 
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	}
	
	pFrame->SetGroupName(strGroup);
	pFrame->SetAuthor(strAuthor);
	pFrame->SetMessageIDIndex(nArticle);
	pFrame->SetSubject(strSubject);
	pFrame->SetNumArticles(m_pDoc->m_strMessageIDs.GetSize());	
	
	// Initial update the frame, but keep it in hiding until its view has
	// the article     
	m_pStatusBar->SetText("Retrieving article...  Please wait.");
	m_pApp->GetNewsArticle()->InitialUpdateFrame(pFrame, m_pDoc);	
	return;
}

void CArticleSelectionView::OnUpdateArticleOpen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TestLists());	
}                               

BOOL CArticleSelectionView::TestLists()
{
	// This function tests the article count and the
	// size of the CStringArrays to make sure that they're all
	// filled with items.
	
	if (m_ctlArticleList.GetCount() == 0 ||
			m_pDoc->m_strMessageIDs.GetSize() == 0 ||
		m_pDoc->m_strSubjectList.GetSize() == 0 ||
			m_pDoc->m_strSenderList.GetSize == 0)
	{
		return FALSE;
	}
	
	return TRUE;
}

void CArticleSelectionView::OnArticleAdd()
{
	CArticleComposeWnd* pFrame = 
		(CArticleComposeWnd*)m_pApp->GetCNA()->CreateNewFrame(m_pDoc, NULL);
	if (pFrame == NULL)
	{ 
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	}

	m_pApp->GetCNA()->InitialUpdateFrame(pFrame, m_pDoc, TRUE);
	
	pFrame->GetArticleComposeBar().SetNewsgroups(m_strGroup);
	pFrame->GetArticleComposeBar().UpdateData(FALSE);
	
}  

/////////////////////////////////////////////////////////////////////////////
// CArticleView

IMPLEMENT_DYNCREATE(CArticleView, CEditView)

BEGIN_MESSAGE_MAP(CArticleView, CEditView)
	//{{AFX_MSG_MAP(CArticleView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_MIRROR_DISPLAY_FONT, OnMirrorDisplayFont)
	ON_COMMAND(ID_WORD_WRAP, OnWordWrap)
	ON_UPDATE_COMMAND_UI(ID_WORD_WRAP, OnUpdateWordWrap)
	ON_COMMAND(ID_CHOOSE_PRINT_FONT, OnChoosePrintFont)
	ON_UPDATE_COMMAND_UI(ID_CHOOSE_PRINT_FONT, OnUpdateChoosePrintFont)
	ON_COMMAND(ID_CHOOSE_FONT, OnChooseFont)
	ON_COMMAND(ID_PAGE_SETUP, OnPageSetup)
	ON_UPDATE_COMMAND_UI(ID_MIRROR_DISPLAY_FONT, OnUpdateMirrorDisplayFont)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_ARTICLE_FIRST, OnArticleFirst)
	ON_UPDATE_COMMAND_UI(ID_ARTICLE_FIRST, OnUpdateArticleFirst)
	ON_COMMAND(ID_ARTICLE_PREVIOUS, OnArticlePrevious)
	ON_UPDATE_COMMAND_UI(ID_ARTICLE_PREVIOUS, OnUpdateArticlePrevious)
	ON_COMMAND(ID_ARTICLE_NEXT, OnArticleNext)
	ON_UPDATE_COMMAND_UI(ID_ARTICLE_NEXT, OnUpdateArticleNext)
	ON_COMMAND(ID_ARTICLE_LAST, OnArticleLast)
	ON_UPDATE_COMMAND_UI(ID_ARTICLE_LAST, OnUpdateArticleLast)
	ON_COMMAND(ID_ARTICLE_FOLLOWUP, OnArticleFollowup)
	ON_COMMAND(ID_ARTICLE_REPLY_EMAIL, OnArticleReply)
	ON_COMMAND(ID_ARTICLE_ADD, OnArticleAdd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_BN_CLICKED(ID_ARTICLE_FOLLOWUP, OnArticleFollowup)
	ON_COMMAND(ID_ARTICLE_OPTIONS, OnArticleOptions)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview) 
	// Internet communications
	ON_MESSAGE(WM_SOCKET_RESPONSE, OnSocketResponse)
	// Modification messsage -- we use this to prevent modifications to
	// this view's contents from making changes in our document
	ON_EN_CHANGE(AFX_IDW_PANE_FIRST, OnEditChange)
END_MESSAGE_MAP()

UINT CArticleView::m_nDefTabStops;
UINT CArticleView::m_nDefTabStopsOld;
BOOL CArticleView::m_bDefWordWrap;
BOOL CArticleView::m_bDefWordWrapOld;
LOGFONT NEAR CArticleView::m_lfDefFont;
LOGFONT NEAR CArticleView::m_lfDefFontOld;
LOGFONT NEAR CArticleView::m_lfDefPrintFont;
LOGFONT NEAR CArticleView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

static void GetProfileFont(LPCSTR szSec, LOGFONT* plf)
{
	CWinApp* pApp = AfxGetApp();
	plf->lfHeight = pApp->GetProfileInt(szSec, "Height", 0);
	if (plf->lfHeight != 0)
	{
		plf->lfWeight = pApp->GetProfileInt(szSec, "Weight", 400);
		plf->lfItalic = (BYTE)pApp->GetProfileInt(szSec, "Italic", 0);
		plf->lfUnderline = (BYTE)pApp->GetProfileInt(szSec, "Underline", 0);
		plf->lfPitchAndFamily = (BYTE)pApp->GetProfileInt(szSec, "PitchAndFamily", 0);
		CString strFont = pApp->GetProfileString(szSec, "FaceName", "Arial");
		strncpy((char*)plf->lfFaceName, strFont, sizeof plf->lfFaceName);
		plf->lfFaceName[sizeof plf->lfFaceName-1] = 0;
	}
}

static void WriteProfileFont(LPCSTR szSec, const LOGFONT* plf, LOGFONT* plfOld)
{
	CWinApp* pApp = AfxGetApp(); 
	if (plf->lfHeight != plfOld->lfHeight)
		pApp->WriteProfileInt(szSec, "Height", plf->lfHeight);
	if (plf->lfHeight != 0)
	{
		if (plf->lfHeight != plfOld->lfHeight)
			pApp->WriteProfileInt(szSec, "Height", plf->lfHeight);
		if (plf->lfWeight != plfOld->lfWeight)
			pApp->WriteProfileInt(szSec, "Weight", plf->lfWeight);
		if (plf->lfItalic != plfOld->lfItalic)
			pApp->WriteProfileInt(szSec, "Italic", plf->lfItalic);
		if (plf->lfUnderline != plfOld->lfUnderline)
			pApp->WriteProfileInt(szSec, "Underline", plf->lfUnderline);
		if (plf->lfPitchAndFamily != plfOld->lfPitchAndFamily)
			pApp->WriteProfileInt(szSec, "PitchAndFamily", plf->lfPitchAndFamily);
		if (strcmp(plf->lfFaceName, plfOld->lfFaceName) != 0)
			pApp->WriteProfileString(szSec, "FaceName", (LPCSTR)plf->lfFaceName);
	}
	*plfOld = *plf;
}

void CArticleView::Initialize()
{ 
	CWinApp* pApp = AfxGetApp();
	// Enable word wrap by default
	m_bDefWordWrap = pApp->GetProfileInt("Newsgroup Article: Settings", "WordWrap", TRUE);
	m_bDefWordWrapOld = m_bDefWordWrap;
	m_nDefTabStops = pApp->GetProfileInt("Newsgroup Article: Settings", "TabStops", 8*4);
	m_nDefTabStopsOld = m_nDefTabStops;
	GetProfileFont("Newsgroup Article: Font", &m_lfDefFont);
	m_lfDefFontOld = m_lfDefFont;
	GetProfileFont("Newsgroup Article: PrintFont", &m_lfDefPrintFont);
	m_lfDefPrintFontOld = m_lfDefPrintFont;   	
}

void CArticleView::Terminate()
{
	CWinApp* pApp = AfxGetApp();     
	// Save settings if they changed
	if (m_nDefTabStops != m_nDefTabStopsOld)
		pApp->WriteProfileInt("Newsgroup Article: Settings", "TabStops", m_nDefTabStops);
	if (m_bDefWordWrap != m_bDefWordWrapOld)
		pApp->WriteProfileInt("Newsgroup Article: Settings", "WordWrap", m_bDefWordWrap);
	WriteProfileFont("Newsgroup Article: Font", &m_lfDefFont, &m_lfDefFontOld);
	WriteProfileFont("Newsgroup Article: PrintFont", &m_lfDefPrintFont, &m_lfDefPrintFontOld);
	
}

/////////////////////////////////////////////////////////////////////////////
// CArticleView construction/destruction

CArticleView::CArticleView()
{
	// Load in settings from INI file
	Initialize();
	
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_bUndo = FALSE; // start out unable to undo or redo anything
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_bFirstCall = TRUE;
}

BOOL CArticleView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

CArticleView::~CArticleView()
{ 
	m_pApp = NULL;
	
	if (!m_pDoc->IsSocketAvailable())
		m_pDoc->ReleaseSocket();
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CArticleView initial update

void CArticleView::OnInitialUpdate()
{
	m_pDoc = GetDocument();
	m_pParentFrame = (CArticleWnd*)GetParentFrame();
	if (m_pParentFrame == NULL)
		return;

	CString strText = "Loading article information...  Please wait.";
	m_pStatusBar->SetText(strText);
	strText.Empty();
	
	SetGroupName(m_pParentFrame->GetGroupName());
	SetSubject(m_pParentFrame->GetSubject());
	SetMessageIDIndex(m_pParentFrame->GetMessageIDIndex());
	SetAuthor(m_pParentFrame->GetAuthor());
	
	SetNumArticles(m_pDoc->m_strMessageIDs.GetSize());

    strText = "Requesting server attention...  Please wait.";
    m_pStatusBar->SetText(strText);
    
    m_pParentFrame->OnUpdateFrameTitle(FALSE);
  	
  	m_pApp->Serialize(FALSE);
  	switch (m_pApp->GetNNTPArticleDisplay())
  	{
  		case 0:
  			Type = Headers;
  			break;
  		
  		case 1:
  			Type = Article;
  			break;
  		
  		case 2:
  			Type = Body;
  			break;
  	}
    m_bFirstCall = TRUE; 
	
	switch (Type)
	{
		case Headers:
			GetArticleHeaders(m_pDoc->m_strMessageIDs[GetMessageIDIndex()]);
			break;
		
		case Article:
			GetArticle(m_pDoc->m_strMessageIDs[GetMessageIDIndex()]);
			break;
		
		case Body:
			GetArticleBody(m_pDoc->m_strMessageIDs[GetMessageIDIndex()]);
			break;
	}
	
	m_pApp->Serialize(TRUE);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleView article retrieval and communications

void CArticleView::GetArticle(CString& strMessageID)
{
	if (strMessageID.IsEmpty())
		return; // we can't tell the newsgroups server which article we want
					// unless we have a message ID
	
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	
	QSocket* pSocket = m_pDoc->GetSocket();
	if (pSocket == NULL)
	{	
		AfxMessageBox(IDP_CONNECTION_BUSY,
				MB_ICONINFORMATION|MB_OK);
		m_pStatusBar->ShowIdleMessage();
		return;
	}                                   
	
	Type = Article;
	pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);

	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	
	// Now, send the ARTICLE command.  This command asks the server to
	// send us the article whose message ID is in strMessageID.
	CString strCommand = "ARTICLE " + strMessageID + "\r\n";
	pSocket->Send(strCommand);
	
	// For code which receives the article's text, see
	// CArticleView::OnSocketResponse
	
	// Set m_bFirstCall to TRUE so OnSocketResponse() will filter the
	// response line out of the main article text
	m_bFirstCall = TRUE;            
	
	m_strArticle.Empty();
	
	return;
}

// A function which asks for the article headers
void CArticleView::GetArticleHeaders(CString& strMessageID)
{
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);           
	
	if (strMessageID.IsEmpty())
		return; // we can't tell the newsgroups server which article we want
					// unless we have a message ID
	
	QSocket* pSocket = m_pDoc->GetSocket();
	if (pSocket == NULL)
	{	
		AfxMessageBox(IDP_CONNECTION_BUSY,
				MB_ICONINFORMATION|MB_OK);
		m_pStatusBar->ShowIdleMessage();
		return;
	}                                   
	
	Type = Headers;
	pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);
	
	m_pStatusBar->SetText("Asking for headers...  Please wait.");
	
	// Now, send the HEAD command.  This command asks the server to
	// send us the headers of the article whose message ID is in 
	// strMessageID.
	CString strCommand = "HEAD " + strMessageID + "\r\n";
	pSocket->Send(strCommand);
	
	// For code which receives the article's text, see
	// CArticleView::OnSocketResponse
	
	// Set m_bFirstCall to TRUE so OnSocketResponse() will filter the
	// response line out of the main article text
	m_bFirstCall = TRUE;                         
	
	m_strArticle.Empty();
	
	return;		
}	   

void CArticleView::GetArticleBody(CString& strMessageID)
{
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);     
	
	if (strMessageID.IsEmpty())
		return; // we can't tell the newsgroups server which article we want
					// unless we have a message ID
	
	QSocket* pSocket = m_pDoc->GetSocket();
	if (pSocket == NULL)
	{	
		AfxMessageBox(IDP_CONNECTION_BUSY,
				MB_ICONINFORMATION|MB_OK);
				
		m_pDoc->ReleaseSocket();
		
		m_pStatusBar->ShowIdleMessage();
		return;
	}                                   
	
	Type = Body;
	pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);

	m_pStatusBar->SetText("Asking for article...  Please wait.");
	
	// Now, send the BODY command.  This command asks the server to
	// send us the body of the article whose message ID is in strMessageID.
	CString strCommand = "BODY " + strMessageID + "\r\n";
	pSocket->Send(strCommand); 
	
	// Set m_bFirstCall to TRUE so OnSocketResponse() will filter the
	// response line out of the main article text
	m_bFirstCall = TRUE;            
	
	// For code which receives the body's text, see
	// CArticleView::OnSocketResponse
	
	m_strArticle.Empty();
	
	return;
}

LRESULT CArticleView::OnSocketResponse(WPARAM amount, LPARAM buffer)
{
	CString strText = "";  // text which we will put on the status bar
	
	if (Type == IDLE)
		return 0;
		
	m_strArticle += (char*)buffer;
	
	// Announce to the user (via the status bar) that we are receiving
	// article text.
	m_pStatusBar->SetText(IDS_RECEIVING_ARTICLE);
	
	strText.Empty();
	if (m_bFirstCall) // if this is the first call of this function...
	{
		// First line of response is status -- strip it out!
		int nPos;		                   
		if ((nPos = m_strArticle.Find("\r\n")) >= 0)
		{
			CString strLine = m_strArticle.Left(nPos);
			int nResponseCode = atoi(strLine.Left(3));
			int nFirstDigit = atoi(strLine.Left(1));
			
			if (nFirstDigit == 2)
			{
				// Article (body, headers, or both)
				// follows -- strip out this status response line
				m_strArticle = m_strArticle.Right(m_strArticle.GetLength() - nPos - 2);

				m_bFirstCall = FALSE;				
			}
			else
			{
				m_pDoc->DisplayNNTPError(nResponseCode);
							
				// Close this view
				Type = IDLE;

				m_pDoc->ReleaseSocket(); 
				GetParentFrame()->ShowWindow(SW_HIDE);
				GetParentFrame()->DestroyWindow();  
				m_pStatusBar->ShowIdleMessage();
				return 0;
			}
		} 
		else
		{
			// No processing until we have the first line
			strText = "Waiting for first line of article...  Please wait.";
			m_pStatusBar->SetText(strText);
			/*wait.Restore();*/
			return 0;
		}
	}
	if (m_strArticle.Right(5) == "\r\n.\r\n")
	{   
		// Article transfer is complete -- inform the user via the
		// status bar.
		if (Type == Article)
			m_pStatusBar->SetText("Article transfer complete.");
		else if (Type == Headers)
			m_pStatusBar->SetText("Header transfer complete.");
		else if (Type == Body)
			m_pStatusBar->SetText("Article body transfer complete.");
		
		// Remove the trailing "\r\n.\r\n" at the end -- it really isn't a
		// part of this article.
		m_strArticle = m_strArticle.Left(m_strArticle.GetLength() - 5);   

		// Find .. at the start of lines, and drop one .
		// First, handle special case if first line starts ..
		if (m_strArticle.Left(2) == "..")
		{
			m_strArticle = m_strArticle.Right(m_strArticle.GetLength() - 1);
		}
		char *pszPosition, *pszWindowBuffer;
		pszWindowBuffer = m_strArticle.GetBuffer(0);
		pszPosition = pszWindowBuffer;
		while (pszPosition = strstr(pszPosition, "\n.."))
		{
			int nLength = m_strArticle.GetLength();
			strcpy(pszPosition+1, pszPosition+2);
			pszPosition += 2;  // Get past the \n. left behind
		}

		m_pDoc->ReleaseSocket();  
		GetEditCtrl().SetWindowText(m_strArticle); 
		GetEditCtrl().UpdateWindow();
		GetEditCtrl().SetReadOnly();
		Type = IDLE; // We're done! 
		
		// Next, extract the contents of the subject header and 
		// put it in this window's title bar
		UINT nLines, nLineLength, i;
		// Buffer of 5004 chars -- 4 extra for \r,\n, zero term, and a ..
		char szBuffer[5004];                                             
		nLines = GetEditCtrl().GetLineCount();
		
		CString title = "Article Processing Progress";
		CString text = "Please wait while we search for the subject of this";
		text += " article.  Thank you for your patience!";
		
		CPercentDialog* pDialog = new CPercentDialog(AfxGetMainWnd());
		pDialog->Initialize(title, text);
		pDialog->SetPercent(0);
		for (i = 0;i < nLines;i++)
		{ 
			/*wait.Restore();*/
			nLineLength = GetEditCtrl().GetLine(i, szBuffer, 5000);
			if (strnicmp(szBuffer, "Subject: ", 9) == 0)
			{
				szBuffer[nLineLength] = 0;
				m_pParentFrame = (CArticleWnd*)GetParentFrame();
				m_pParentFrame->SetSubject(szBuffer + 9);
				
				// Inform ourselves of the subject also
				SetSubject(m_pParentFrame->GetSubject());
				
				// We're 100% done analyzing this article
				pDialog->SetPercent(100);                
				
				break;
			}

			pDialog->CalculatePercent(i + 1, nLines);
			pDialog->UpdateWindow();
		}
		
		// dismiss the percent dialog created earlier
		pDialog->SetPercent(100);
		pDialog->DestroyWindow();
		
		// remember to call operator delete on pDialog, since it was
		// allocated with operator new
		delete pDialog;
		pDialog = NULL;
		
		// now tell the parent window to update its title bar
		if (!m_pParentFrame->m_strSubject.IsEmpty())
			m_pParentFrame->OnUpdateFrameTitle(TRUE); 		
	}	

	return 0;	
}	
/////////////////////////////////////////////////////////////////////////////
// CArticleView Word Wrap support

BOOL CArticleView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL CArticleView::SetWordWrap(BOOL bWordWrap)
{
	bWordWrap = !!bWordWrap;    // make sure ==TRUE || ==FALSE
	if (IsWordWrap() == bWordWrap)
		return FALSE;

	// preserve original control's state.
	CFont* pFont = GetFont();
	int nLen = GetBufferLength();
	char FAR* pSaveText = new far char[GetBufferLength()+1];
	GetWindowText(pSaveText, nLen+1);

	// create new edit control with appropriate style and size.
	DWORD dwStyle = dwStyleDefault & ~(ES_AUTOHSCROLL|WS_HSCROLL|WS_VISIBLE);
	if (!bWordWrap)
		dwStyle |= ES_AUTOHSCROLL|WS_HSCROLL;

	CWnd* pParent = GetParent();
	CRect rect;
	GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	CWnd* pFocus = GetFocus();

	UINT nID = GetDlgCtrlID();

	HWND hWnd = ::CreateWindow("edit", NULL, dwStyle,
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
		pParent->m_hWnd, (HMENU)nID,
		(HINSTANCE)m_segText, NULL);

	if (hWnd == NULL)
	{
		delete[] pSaveText;
		return FALSE;
	}

	// set the window text to nothing to make sure following set doesn't fail
	SetWindowText(NULL);

	// restore visual state
	::SetWindowText(hWnd, pSaveText);
	delete[] pSaveText;
	if (pFont != NULL)
	{
		ASSERT(pFont->m_hObject != NULL);
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)pFont->m_hObject, 0);
	}

	// detach old window, attach new
	HWND hWndOld = Detach();
	::SetWindowLong(hWndOld, GWL_WNDPROC, (LONG)*GetSuperWndProcAddr());
	::SetWindowWord(hWndOld, GWW_ID, nID+1);
	ASSERT(m_hWnd == NULL);
	SubclassWindow(hWnd);
	ASSERT(m_hWnd == hWnd);
	UINT nTabStops = m_nTabStops;
	GetParentFrame()->SendMessage(WM_RECALCPARENT);
	GetEditCtrl().SetTabStops(nTabStops);
	GetClientRect(&rect);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_DRAWFRAME);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	UpdateWindow();

	// destroy old
	::SetWindowPos(hWndOld, NULL, 0, 0, 0, 0,
		SWP_HIDEWINDOW|SWP_NOREDRAW|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|
		SWP_NOZORDER);
	::DestroyWindow(hWndOld);

	// restore rest of state...
	GetEditCtrl().LimitText(nMaxSize);
	if (pFocus == this)
		SetFocus();

	ASSERT_VALID(this);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleView drawing

void CArticleView::OnDraw(CDC* pDC)
{
	CNewsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CArticleView printing

BOOL CArticleView::OnPreparePrinting(CPrintInfo* pInfo)
{  
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void CArticleView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Begin print loop by calling default version -- let the
	// base class do its stuff first
	CEditView::OnBeginPrinting(pDC, pInfo);

	CString strFileName = GetDocument()->GetPathName(); 
	if (strFileName.IsEmpty()) strFileName = GetDocument()->GetTitle(); 
	BOOL bForceSysTime = strchr(strFileName, '.') == NULL;
	CTime timeSys = CTime::GetCurrentTime();
	CFileStatus status;
	CFile::GetStatus(strFileName, status);
	
	if (m_nHeaderTime != 0 || bForceSysTime)
		m_timeHeader = timeSys;
	else
		m_timeHeader = status.m_mtime;
    	
    if (m_nFooterTime != 0 || bForceSysTime)
		m_timeFooter = timeSys;
	else
		m_timeFooter = status.m_mtime;
    
    GetDocument()->m_strTitle = m_pParentFrame->GetSubject();
	if (!pInfo->m_bPreview)
		return;

	pInfo->m_nCurPage = 0xFFFF;
	OnPrepareDC(pDC, pInfo);

	UINT nIndex = LOWORD(GetEditCtrl().GetSel());
	UINT nCurPage = 1;
	while (nCurPage < (UINT)m_aPageStart.GetSize())
	{
		if (nIndex < m_aPageStart[nCurPage])
			break;
		nCurPage++;
	}
	pInfo->m_nCurPage = nCurPage;        
	
	// Fill the CPrintInfo struct with needed information
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(m_aPageStart.GetSize()); // number of pages of message
	pInfo->m_pPD->m_pd.nMinPage = 1;
	pInfo->m_pPD->m_pd.nMaxPage = m_aPageStart.GetSize();
	pInfo->m_pPD->m_pd.nFromPage = 1;
	pInfo->m_pPD->m_pd.nToPage = m_aPageStart.GetSize();

	m_nPreviewPage = nCurPage;
}

void CArticleView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{   
	GetDocument()->SetTitle(""); // automatically resets title
	// get string to show as "filename" in header/footer
	CPageSetupSheet shtPageSetup;
	CString strFileName = GetDocument()->GetPathName();
	if (strFileName.IsEmpty())
		strFileName = GetDocument()->GetTitle();

	// go thru CPageSetupSheet::CPageSetupPage to format the header 
	// and footer
	CString strHeader = "";
	shtPageSetup.m_pgHeaderFooter.Initialize();
	strHeader = m_pParentFrame->GetSubject() + " <";
	strHeader += m_pParentFrame->GetGroupName() + ">";
	
	CString strFooter = "";        
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.FormatFooter(strFooter, m_timeFooter, strFileName,
		pInfo->m_nCurPage);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cyChar = tm.tmHeight;
	CRect rectPage = pInfo->m_rectDraw;

	// draw and exclude space for header
	if (!strHeader.IsEmpty())
	{
		pDC->TextOut(rectPage.left, rectPage.top, strHeader);
		rectPage.top += cyChar + cyChar / 4;
		pDC->MoveTo(rectPage.left, rectPage.top);
		pDC->LineTo(rectPage.right, rectPage.top);
		rectPage.top += cyChar / 4;
	}

	// allow space for footer
	pInfo->m_rectDraw = rectPage;
	if (!strFooter.IsEmpty())
		pInfo->m_rectDraw.bottom -= cyChar + cyChar/4 + cyChar/4;

	// draw body text
	CEditView::OnPrint(pDC, pInfo);

	// draw footer
	if (!strFooter.IsEmpty())
	{
		rectPage.bottom -= cyChar;
		pDC->TextOut(rectPage.left, rectPage.bottom, strFooter);
		rectPage.bottom -= cyChar / 4;
		pDC->MoveTo(rectPage.left, rectPage.bottom);
		pDC->LineTo(rectPage.right, rectPage.bottom);
		rectPage.bottom -= cyChar / 4;
	}
}

void CArticleView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void CArticleView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CArticleView diagnostics

#ifdef _DEBUG
void CArticleView::AssertValid() const
{
	CEditView::AssertValid();
}

void CArticleView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CNewsDoc* CArticleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewsDoc)));
	return (CNewsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CArticleView message handlers

static void ScaleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member from one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}
 
int CArticleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;            
		
	if (m_lfDefFont.lfHeight != 0)
	{
		m_font.CreateFontIndirect(&m_lfDefFont);
		SetFont(&m_font);
	}
	if (m_lfDefPrintFont.lfHeight != 0)
	{
		m_fontPrint.CreateFontIndirect(&m_lfDefPrintFont);
		SetPrinterFont(&m_fontPrint);
	}
	return 0;
}

void CArticleView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing commands
	CEditView::OnRButtonDown(nFlags, point);
}

void CArticleView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void CArticleView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void CArticleView::OnWordWrap()
{
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void CArticleView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void CArticleView::OnChoosePrintFont()
{
	CFont* pFont = GetPrinterFont();
	LOGFONT lf;
	LPLOGFONT plf = NULL;
	if (pFont != NULL)
	{
		pFont->GetObject(sizeof(LOGFONT), &lf);
		plf = &lf;
	}

	// magic to get printer dialog that would be used if we were printing!
	CPrintDialog dlgPrint(FALSE);
	if (!AfxGetApp()->GetPrinterDeviceDefaults(&dlgPrint.m_pd))
	{
		AfxMessageBox(IDP_ERR_GET_DEVICE_DEFAULTS);
		return;
	}

	HDC hdcPrint = dlgPrint.CreatePrinterDC();
	if (hdcPrint == NULL)
	{
		AfxMessageBox(IDP_ERR_GET_PRINTER_DC);
		return;
	}

	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));
	CDC dcPrint;
	dcPrint.Attach(hdcPrint);

	if (plf != NULL)
	{
		// need to map initial logfont to screen metrics.
		::ScaleLogFont(plf, dcPrint, dcScreen);
	}

	// now bring up the dialog since we know the printer DC
	CFontDialog dlg(plf, CF_PRINTERFONTS, &dcPrint);
	if (dlg.DoModal() == IDOK)
	{
		// map the resulting logfont back to printer metrics.
		lf = dlg.m_lf;
		::ScaleLogFont(&lf, dcScreen, dcPrint);

		m_fontPrint.DeleteObject();
		if (m_fontPrint.CreateFontIndirect(&lf))
		{
			SetPrinterFont(&m_fontPrint);
			m_lfDefPrintFont = lf;
		}
	}
	//NOTE: destructor will call dcPrint.DeleteDC

	::ReleaseDC(NULL, dcScreen.Detach());
}

void CArticleView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void CArticleView::OnChooseFont()
{
   // get current font description
   CFont* pFont = GetFont();
   LOGFONT lf;
   if (pFont != NULL)
	   pFont->GetObject(sizeof(LOGFONT), &lf);
   else
	   ::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

	CFontDialog dlg(&lf, CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
	if (dlg.DoModal() == IDOK)
	{
		// switch to new font.
		m_font.DeleteObject();
		if (m_font.CreateFontIndirect(&lf))
		{
			SetFont(&m_font);
			m_lfDefFont = lf;
    	}
	}
}

void CArticleView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());      
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.m_bOutputView = TRUE;
	shtPageSetup.m_pgTabStops.m_nTabStops = m_nTabStops/4;
	if (shtPageSetup.DoModal() == IDOK)
	{
		// get values entered into this tabbed dialog box
		shtPageSetup.m_pgHeaderFooter.Terminate();
		
		m_nHeaderTime = shtPageSetup.m_pgHeaderFooter.m_nHeaderTime;
		m_nFooterTime = shtPageSetup.m_pgHeaderFooter.m_nFooterTime;
		SetTabStops(shtPageSetup.m_pgTabStops.m_nTabStops*4);
		m_nDefTabStops = m_nTabStops;
     		
		return;
	}  
	return;
}

void CArticleView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void CArticleView::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strTextSect = "Text";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strTextSect, "TextLeft", rect.left);
	pApp->WriteProfileInt(strTextSect, "TextRight", rect.right);
	pApp->WriteProfileInt(strTextSect, "TextTop", rect.top);
	pApp->WriteProfileInt(strTextSect, "TextBottom", rect.bottom);
	pApp->WriteProfileInt(strTextSect, "TextCX", rect.Width());
	pApp->WriteProfileInt(strTextSect, "TextCY", rect.Height());
}

void CArticleView::OnArticleOptions()
{
	m_pApp->Serialize(FALSE);
	CArticleOptions theDialog(m_pApp->GetMainFrame());
	theDialog.m_nDisplay = m_pApp->GetNNTPArticleDisplay();
	
	if (theDialog.DoModal() == IDOK)
		m_pApp->SetNNTPArticleDisplay(theDialog.m_nDisplay);
	m_pApp->Serialize(TRUE);
}

void CArticleView::OnEditUndo()
{
	CEditView::OnEditUndo();
	
	// change the "redo flag" to TRUE so the undone action can be redone
	m_bUndo = TRUE;
	return;
}

void CArticleView::OnEditRedo()
{
	if (m_bUndo)
	{
		CEditView::OnEditUndo();
		m_bUndo = FALSE;	// can't redo anymore -- not until another undo
	}
	return;	// we're done
}	

void CArticleView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bUndo && GetEditCtrl().CanUndo());
	return;
} 

void CArticleView::OnEditChange()
{
	m_pDoc->SetModifiedFlag(FALSE);
	return;
}
	
/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsListBox listbox

BEGIN_MESSAGE_MAP(CNewsgroupsListBox, CListBox)
	//{{AFX_MSG_MAP(CNewsgroupsListBox)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CNewsgroupsListBox::DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap, 
	BITMAP& bitmap, short xStart, short yStart, COLORREF crTransparentColor)
{
   COLORREF   crColor;
   CBitmap    bmAndBack, bmAndObject, bmAndMem, bmSave;
   CBitmap   *bmBackOld, *bmObjectOld, *bmMemOld, *bmSaveOld;
   CDC        memDC, backDC, objectDC, tempDC, saveDC;
   CPoint     ptSize;

   tempDC.CreateCompatibleDC(pDC);
   CBitmap* pOldTemp = tempDC.SelectObject(pBitmap);
   
   // Fill the BITMAP structure (which is a reference argument) with
   // bitmap information
   pBitmap->GetObject(sizeof(BITMAP), &bitmap);

   ptSize.x = bitmap.bmWidth;            // Get width of bitmap
   ptSize.y = bitmap.bmHeight;           // Get height of bitmap
   tempDC.DPtoLP(&ptSize, 1);      // Convert from device
                                     // to logical points

   // Create some DCs to hold temporary data.
   backDC.CreateCompatibleDC(pDC);
   objectDC.CreateCompatibleDC(pDC);
   memDC.CreateCompatibleDC(pDC);
   saveDC.CreateCompatibleDC(pDC);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC

   bmAndBack.CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject.CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem.CreateCompatibleBitmap(pDC, ptSize.x, ptSize.y);
   bmSave.CreateCompatibleBitmap(pDC, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = backDC.SelectObject(&bmAndBack);
   bmObjectOld = objectDC.SelectObject(&bmAndObject);
   bmMemOld    = memDC.SelectObject(&bmAndMem);
   bmSaveOld   = saveDC.SelectObject(&bmSave);

   // Set proper mapping mode.
   tempDC.SetMapMode(pDC->GetMapMode());

   // Save the bitmap sent here, because it will be overwritten.
   saveDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   crColor = tempDC.SetBkColor(crTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt()
   // from the source bitmap to a monochrome bitmap.
   objectDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0,
          SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   tempDC.SetBkColor(crColor);

   // Create the inverse of the object mask.
   backDC.BitBlt(0, 0, ptSize.x, ptSize.y, &objectDC, 0, 0,
          NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, pDC, xStart, yStart,
          SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, &objectDC, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   tempDC.BitBlt(0, 0, ptSize.x, ptSize.y, &backDC, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   pDC->BitBlt(xStart, yStart, ptSize.x, ptSize.y, &memDC, 0, 0,
          SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   tempDC.BitBlt(0, 0, ptSize.x, ptSize.y, &saveDC, 0, 0, SRCCOPY);
   
   CBitmap* pTemp = tempDC.SelectObject(pOldTemp);
   
   	// Delete the memory bitmaps.
   	pTemp = backDC.SelectObject(bmBackOld);
   	pTemp->DeleteObject();
   	
   	pTemp = objectDC.SelectObject(bmObjectOld);
   	pTemp->DeleteObject();
   	
   	pTemp = memDC.SelectObject(bmMemOld);
   	pTemp->DeleteObject();
    
    pTemp = saveDC.SelectObject(bmSaveOld);
    pTemp->DeleteObject();
    
   	// Delete the memory DCs.
   	memDC.DeleteDC();
   	backDC.DeleteDC();

   	objectDC.DeleteDC();
   	saveDC.DeleteDC();
	tempDC.DeleteDC();
} 

void CNewsgroupsListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	
	COLORREF clrText, clrBk, clrOldText, clrOldBk;
	int nMode = 0;
	
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	
	// Draw a focus rectangle if nothing is selected and we have the focus
	if (lpDIS->itemID == -1)
	{
		// draw the focus rect at the top, though
		pDC->DrawFocusRect(&lpDIS->rcItem);
		return;   
	}
	
	// Initialize colors		
	if (lpDIS->itemAction & ODA_SELECT)	// sel status changed 
	{
		if (lpDIS->itemState & ODS_SELECTED) // item is selected
		{
			// The current item is selected -- get the system's color
			// for highlighting and text
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		} 
		else if (lpDIS->itemState & ODS_FOCUS)
		{
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else // The current item isn't selected, but another item is
		{                                                           
			// Just get the system window and text color
			clrText = ::GetSysColor(COLOR_WINDOWTEXT);
			clrBk = ::GetSysColor(COLOR_WINDOW);
		}
		lpDIS->itemAction |= ODA_DRAWENTIRE;
	}
	else if (lpDIS->itemAction & ODA_FOCUS)
	{   
		// Current item may or may not have input focus
		if (lpDIS->itemState & ODS_SELECTED)
		{
			// The current item is selected -- get the system's color
			// for highlighting and text
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (lpDIS->itemState & ODS_FOCUS)
		{
			// The current item may be still selected let's take a gamble
			// here 
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else // The current item is not selected
		{
			clrText = ::GetSysColor(COLOR_WINDOWTEXT);
			clrBk = ::GetSysColor(COLOR_WINDOW);
		}
		lpDIS->itemAction |= ODA_DRAWENTIRE;
	}				
	else
	{
		if (lpDIS->itemState & ODS_SELECTED)
		{
			// highlight and highlighttext colors
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (lpDIS->itemState & ODS_FOCUS)
		{
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
			
		clrText = ::GetSysColor(COLOR_WINDOWTEXT);
		clrBk = ::GetSysColor(COLOR_WINDOW);
	}

	if (lpDIS->itemAction & ODA_DRAWENTIRE) // draw whole control
	{
		CBitmap bmIcon;
		CString strItem = "";
		
		// Load Internet folder-and-earth icon
		bmIcon.LoadBitmap(IDB_INTERFOLDER);
		
		// Initialize text and background colors, and the background mode
		clrOldText = pDC->SetTextColor(clrText);
		clrOldBk = pDC->SetBkColor(clrBk);
		
		nMode = pDC->SetBkMode(TRANSPARENT);
		
		CBrush br(clrBk);
		pDC->FillRect(&lpDIS->rcItem, &br);   
        
        CBitmap* pBitmap = &bmIcon;
		BITMAP bitmap;
		DrawTransparentBitmap(pDC, pBitmap, bitmap,
			lpDIS->rcItem.left + 5,
			lpDIS->rcItem.top, RGB(255,0,255));
		
		// Now draw the text associated with the item 	
		GetText(lpDIS->itemID, strItem);
		
		pDC->TextOut(lpDIS->rcItem.left + bitmap.bmWidth + 10,
					lpDIS->rcItem.top, strItem);  
					
		pDC->SetTextColor(clrOldText);
		pDC->SetBkColor(clrOldBk);
		
		pDC->SetBkMode(nMode);
		
		if ((lpDIS->itemAction & ODA_SELECT) &&
				(lpDIS->itemState & ODS_SELECTED))
			pDC->DrawFocusRect(&lpDIS->rcItem);    
		else if ((lpDIS->itemAction & ODA_FOCUS) &&
					(lpDIS->itemState & ODS_SELECTED))
			pDC->DrawFocusRect(&lpDIS->rcItem);
		else if ((lpDIS->itemAction & ODA_SELECT) &&
					(lpDIS->itemState & ODS_FOCUS))
			pDC->DrawFocusRect(&lpDIS->rcItem);
		else if ((lpDIS->itemAction & ODA_FOCUS) &&
					(lpDIS->itemState & ODS_FOCUS))
			pDC->DrawFocusRect(&lpDIS->rcItem);   
		
		return;   
	}         
		
	if ((lpDIS->itemAction & ODA_SELECT) &&
			(lpDIS->itemState & ODS_SELECTED))
		pDC->DrawFocusRect(&lpDIS->rcItem);    
	else if ((lpDIS->itemAction & ODA_FOCUS) &&
				(lpDIS->itemState & ODS_SELECTED))
		pDC->DrawFocusRect(&lpDIS->rcItem);
	else if ((lpDIS->itemAction & ODA_SELECT) &&
				(lpDIS->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDIS->rcItem);
	else if ((lpDIS->itemAction & ODA_FOCUS) &&
				(lpDIS->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDIS->rcItem);      
}

void CNewsgroupsListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	CBitmap bmIcon;

	bmIcon.LoadBitmap(IDB_INTERFOLDER);
	BITMAP bitmap;
	bmIcon.GetObject(sizeof(BITMAP), &bitmap);
	
	lpMIS->itemHeight = bitmap.bmHeight;  
	return;
}

void CNewsgroupsListBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu popup;
	popup.CreatePopupMenu();
	if (GetCount() > 0)
	{
		popup.AppendMenu(MF_ENABLED|MF_STRING, ID_GROUP_OPEN,
			"&Open Group");
		popup.AppendMenu(MF_ENABLED|MF_STRING, ID_GROUP_REMOVE,
			"&Remove Group");
		popup.AppendMenu(MF_SEPARATOR);    
		popup.AppendMenu(MF_ENABLED|MF_STRING, ID_GROUP_ADD,
			"&Add Group...");			
	}
	else if (GetCount() == 0)
	{
		popup.AppendMenu(MF_DISABLED|MF_STRING, ID_GROUP_OPEN,
			"&Open Group");
		popup.AppendMenu(MF_DISABLED|MF_STRING, ID_GROUP_REMOVE,
			"&Remove Group");
		popup.AppendMenu(MF_SEPARATOR);	
		popup.AppendMenu(MF_ENABLED|MF_STRING, ID_GROUP_ADD,
			"&Add Group...");
	}
	ClientToScreen(&point);
	popup.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y,
				GetParent());
	return;
}

void CArticleView::OnArticleFirst()
{                
	CWaitCursor wait; 
	
	if (GetMessageIDIndex() == 0 && m_pDoc->m_strMessageIDs.GetSize() == 0)
		return;
	
	GetEditCtrl().SetReadOnly(FALSE);
	
	// First clear the text of this view
	CEditView::OnEditSelectAll();
	CEditView::OnEditClear();
	
	GetEditCtrl().EmptyUndoBuffer();
		
	// Next, empty out the member variable which stores the article text
	m_strArticle.Empty(); 
	
	SetMessageIDIndex(0);
	CString strMessageID = m_pDoc->m_strMessageIDs[GetMessageIDIndex()];
	
	Type = Article;
	
	// Now, get the first article
	GetArticle(strMessageID);
}

void CArticleView::OnUpdateArticleFirst(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetMessageIDIndex() > 0 && 
			m_pDoc->m_strMessageIDs.GetSize() > 1);
	return;
}

void CArticleView::OnArticlePrevious()
{ 
	GetEditCtrl().SetReadOnly(FALSE);
	
	if (GetMessageIDIndex() == 0 && m_pDoc->m_strMessageIDs.GetSize() == 1)
		return;
		
	// First clear the text of this view
	CEditView::OnEditSelectAll();
	CEditView::OnEditClear(); 
	
	GetEditCtrl().EmptyUndoBuffer();
		
	// Next, empty out the member variable which stores the article text
	m_strArticle.Empty(); 
		
	int nIndex = GetMessageIDIndex();
	
	SetMessageIDIndex(nIndex - 1);
	CString strMessageID = m_pDoc->m_strMessageIDs[GetMessageIDIndex()];
	
	Type = Article;
	
	// Now, get the previous article
	GetArticle(strMessageID);	
}

void CArticleView::OnUpdateArticlePrevious(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetMessageIDIndex() > 0 &&
			m_pDoc->m_strMessageIDs.GetSize() > 1);
	return;
}

void CArticleView::OnArticleNext()
{
	GetEditCtrl().SetReadOnly(FALSE);

	// First clear the text of this view
	CEditView::OnEditSelectAll();
	CEditView::OnEditClear(); 
	
	GetEditCtrl().EmptyUndoBuffer();
	
	int nArticles = m_pDoc->m_strMessageIDs.GetSize(); // # of articles
	int nMaxIndex = nArticles - 1; // highest possible index
	int nIndex = GetMessageIDIndex(); // current index
	
	// incase the ON_UPDATE_COMMAND_UI handler isn't called
	if (nIndex == nMaxIndex || nArticles == 1) return;
	
	// The index of the next article in the array is simply 1 more than
	// the current article's index
	SetMessageIDIndex(nIndex + 1);
	CString strMessageID = m_pDoc->m_strMessageIDs[GetMessageIDIndex()];
	
	// Next, empty out the member variable which stores the article text
	m_strArticle.Empty(); 
		
	Type = Article;
	
	// Now, get the previous article
	GetArticle(strMessageID);	
}

void CArticleView::OnUpdateArticleNext(CCmdUI* pCmdUI)
{
	int nArticles = m_pDoc->m_strMessageIDs.GetSize(); // # of articles
	int nMaxIndex = nArticles - 1; // highest possible index
	int nIndex = GetMessageIDIndex(); // current index
	
	// disable if nIndex == nMaxIndex or nArticles == 1
	pCmdUI->Enable(nIndex < nMaxIndex && nArticles > 1); 
}

void CArticleView::OnArticleLast()
{
	GetEditCtrl().SetReadOnly(FALSE);

	// First clear the text of this view
	CEditView::OnEditSelectAll();
	CEditView::OnEditClear(); 
	
	GetEditCtrl().EmptyUndoBuffer();

	// Next, empty out the member variable which stores the article text
	m_strArticle.Empty(); 
	
	int nArticles = m_pDoc->m_strMessageIDs.GetSize(); // # of articles
	int nMaxIndex = nArticles - 1; // highest possible index
	int nIndex = GetMessageIDIndex(); // current index
	
	// incase the ON_UPDATE_COMMAND_UI handler isn't called
	if (nIndex == nMaxIndex || nArticles == 1) return;
	
	// Last article is at index m_strMessageIDs.GetSize() - 1
	SetMessageIDIndex(nMaxIndex);
	CString strMessageID = m_pDoc->m_strMessageIDs[GetMessageIDIndex()];
	
	Type = Article;
	
	// Now, get the last article
	GetArticle(strMessageID);	
}

void CArticleView::OnUpdateArticleLast(CCmdUI* pCmdUI)
{
	int nArticles = m_pDoc->m_strMessageIDs.GetSize(); // # of articles
	int nMaxIndex = nArticles - 1; // highest possible index
	int nIndex = GetMessageIDIndex(); // current index
	
	// disable if nIndex == nMaxIndex or nArticles == 1
	pCmdUI->Enable(nIndex < nMaxIndex && nArticles > 1); 
}

void CArticleView::OnArticleFollowup()
{
	CMultiDocTemplate* pCNA = m_pApp->GetCNA();
	CArticleComposeWnd* pFrame;
	pFrame = (CArticleComposeWnd*)pCNA->CreateNewFrame(GetDocument(), NULL);
	
	if (pFrame == NULL) // an error occurred -- inform user and quit
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	}
	
	pFrame->GetArticleComposeBar().SetFollowup();
	pCNA->InitialUpdateFrame(pFrame, GetDocument());
	
	CString refs = "", subject = "", from = "", groups = "", distribution = "";
	CString messageID = "";
	
	// What we will do here is search for headers which we need to use to
	// prepare the followup for the user

	messageID = m_pDoc->m_strMessageIDs[GetMessageIDIndex()];
	
	// search for the references header
	UINT lines, linelength, i;
	lines = GetEditCtrl().GetLineCount();
	char buffer[5004];
	
	for (i = 0;i < lines;i++)
	{
		linelength = GetEditCtrl().GetLine(i, buffer, 5000);
		if (strnicmp(buffer, "References: ", 12) == 0)
		{
			buffer[linelength] = 0;
			refs = buffer+12;
		}
	}

	// search for Followup-To groups we should use instead of Newsgroups
	for (i = 0;i < lines;i++)
	{
		/*wait.Restore();*/
		linelength = GetEditCtrl().GetLine(i, buffer, 5000);
		if (strnicmp(buffer, "Followup-To: ", 13) == 0)
		{
			buffer[linelength] = 0;
			groups = buffer+13;
		}
	}
	
	if (!groups.IsEmpty())
	{
		if (groups.CompareNoCase("poster") == 0)
		{
			m_pStatusBar->ShowIdleMessage();
			pFrame->SendMessage(WM_CLOSE);  
		
			CArticleView::PostMessage(WM_COMMAND, ID_ARTICLE_REPLY_EMAIL);
			return;
		}
	}

	if (groups.IsEmpty()) // No Followup-To header -- search for newsgroups
	{					// header instead
		for (i = 0;i < lines;i++)
		{
			linelength = GetEditCtrl().GetLine(i, buffer, 5000);
			if (strnicmp(buffer, "Newsgroups: ", 12) == 0)
			{                   
				buffer[linelength] = 0;
				groups = buffer+12;
			}
		}
	}

	// search the article for the text of the Distribution header
	for (i = 0;i < lines;i++)
	{
		linelength = GetEditCtrl().GetLine(i, buffer, 5000);
		if (strnicmp(buffer, "Distribution: ", 14) == 0)
		{
			buffer[linelength] = 0;
			distribution = buffer+14;
		}
	}

	for (i = 0;i < lines;i++)
	{ 
		/*wait.Restore();*/
		linelength = GetEditCtrl().GetLine(i, buffer, 5000);
		if (strnicmp(buffer, "Reply-To: ", 10) == 0)
		{
			buffer[linelength] = 0;
			from = buffer + 6;
		}
	}

	if (from.IsEmpty())
	{	
		for (i = 0;i < lines;i++)
		{ 
			/*wait.Restore();*/
			linelength = GetEditCtrl().GetLine(i, buffer, 5000);
			if (strnicmp(buffer, "From: ", 6) == 0)
			{
				buffer[linelength] = 0;
				from = buffer + 6;
			}
		}
	}
	
	// search the article for the text of the Subject header
	for (i = 0;i < lines;i++)
	{
		/*wait.Restore();*/
		linelength = GetEditCtrl().GetLine(i, buffer, 5000);
		if (strnicmp(buffer, "Subject: ", 9) == 0)
		{
			buffer[linelength] = 0;
			subject = buffer+9;
		}
	}

	if (refs.IsEmpty())
		refs = messageID;
	else
		refs += messageID; // tack this article's MessageID onto end of refs
	
	// Process subject
	CString temp = subject;
	int nSpace;

	if (temp.Left(4) == "RE: ")
	{
		subject = temp.Right(temp.GetLength() - 4);
		temp = "Re: " + subject;
		
		// Now fill strSubject with temp, strSubject's changed self
		subject = temp;
	}
	else if (subject.Left(2) == "[R" &&
		(nSpace = temp.Find(' ')) != -1)
	{
		int c = nSpace + 1;
		subject = temp.Right(temp.GetLength() - c);   
		temp = "Re: " + subject;
		
		// Now fill strSubject with temp, strSubject's changed self
		subject = temp;
	}
	else if (subject.Left(4) != "Re: ")
	{
		// prepend "Re: " to subject
		temp = "Re: " + subject;
		
		// Now fill strSubject with temp, strSubject's changed self
		subject = temp;
	}  
	else if (subject.Left(4) == "Re: ")
	{
		subject = subject;
	}
				
	// now do the followup! -- DoFollowup() below generates the followup 
	// and fills in the information we just extracted
	
	CArticleComposeView* pView = 
		(CArticleComposeView*)pFrame->GetActiveView();
	
	if (!pView->DoFollowup(refs, groups, subject, messageID,
			from, distribution))
	{
		AfxMessageBox(IDP_FAILED_PREPARE_FOLLOWUP, 
			MB_ICONSTOP|MB_OK);
		return;
	}
	return;
}

void CArticleView::OnArticleReply()
{
	CMultiDocTemplate* pCompose = m_pApp->GetComposeMailType();
	
	CMailBoxDoc* pDocument = (CMailBoxDoc*)pCompose->CreateNewDocument();
	if (pDocument == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;
	}
    
    CString strMailPath = m_pApp->GetProfileString("Post Office", 
    							"DefaultMailBox");
	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CComposeWnd* pFrame = 
		(CComposeWnd*)pCompose->CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);    
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);

	if (strMailPath.IsEmpty() && m_pApp->IsMailSetupNew())
	{
		switch (AfxMessageBox(IDP_SETUP_NEW_MAILBOX,
					MB_ICONQUESTION|MB_YESNO))
		{
			case IDYES:
				if (!pDocument->OnNewDocument())
				{
					// user has be alerted to what failed in OnNewDocument
					TRACE0("CDocument::OnNewDocument returned FALSE\n");
					pFrame->DestroyWindow();
					return;
				}
				break;
			
			case IDNO:
				m_pStatusBar->ShowIdleMessage();  
				pFrame->DestroyWindow();
				break;
		}
		return;
	}
	else if (!m_pApp->IsMailSetupNew() && strMailPath.IsEmpty())
	{  
		pFrame->DestroyWindow();
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
	
	pCompose->InitialUpdateFrame(pFrame, pDocument, TRUE);
	CComposeView* pView = (CComposeView*)pFrame->GetActiveView();
	pView->m_pFrame = pFrame;                                 
	
	pFrame->GetInfoBar().ShowWindow(SW_SHOW);
	pFrame->GetInfoBar().UpdateWindow();
	pFrame->RecalcLayout();
		
	pFrame->GetRulerBar().ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	pFrame->RecalcLayout();

	pFrame->GetWnDBSoftHelpBar().ShowWindow(SW_HIDE);
	pFrame->GetWnDBSoftHelpBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	UINT nLineLength;
	CString strSubject = "", strSender = "";
	char buffer[5004];
	// For \r, \n, zero term, and perhaps an extra .
	UINT lines = GetEditCtrl().GetLineCount();
	UINT i = 0;

	for (i = 0;i < lines;i++)
	{
		/*wait.Restore();*/
		nLineLength = GetEditCtrl().GetLine(i, buffer, 5000);
		if (strnicmp(buffer, "Sender: ", 8) == 0)
		{
			buffer[nLineLength] = 0;
			strSender = buffer + 8;
			
			CString strSender2 = strSender; // in case we're wrong below
			// Chop off any parantheses, spaces, punctuation, names, etc.
			int nSpace = 0;     
			int nLength = strSender.GetLength();
			if (strSender.Right(1) == ">")
				nSpace = strSender.ReverseFind(' ');
			else
				nSpace = strSender.Find(' ');
				
			
			// Take string up to space
			if (nSpace == -1) 
			{
				break; // we have strSender -- bing! we're done.
			}
			
			// Take the strSender up to the space
			if (strSender.Right(1) == ">")
			{
				strSender = strSender.Right(nLength - nSpace - 1);
				strSender = strSender.Right(nLength - 1);  
				strSender = strSender.Left(nLength - 1);
				
				// bing! we now have strSender!
				break;
			}
					
			strSender = strSender.Left(nSpace);  
			
			if (strSender.Find('@') == -1)
			{
				// oops -- we guessed wrong -- other part of e-mail address
				// has the actual address
				strSender = strSender2; // get back original strSender
				strSender = strSender.Right(strSender.GetLength() 
						- nSpace - 1);   
			}  
			else
				break;

			if ((nSpace = strSender.Find('<')) != -1)
			{
				// e-mail address is enclosed in angle brackets -- remove
				// angle brackets
				strSender = strSender.Right(nLength - nSpace - 1);
				
				// initialize nLength with new length of strSender
				nLength = strSender.GetLength();
				
				strSender = strSender.Left(nLength - 1);
				break;
			} 
			else if ((nSpace = strSender.Find('\"')) != -1)
			{
				// e-mail address is enclosed in quotation marks -- remove
				strSender = strSender.Right(nLength - nSpace - 1);
				
				// initialize nLength with new length of strSender
				nLength = strSender.GetLength();
				
				strSender = strSender.Left(nLength - 1);
				break;
			} 
			else if ((nSpace = strSender.Find('(')) != -1)
			{
				// e-mail address is enclosed in parentheses -- remove
				strSender = strSender.Right(nLength - nSpace - 1);
				
				// initialize nLength with new length of strSender
				nLength = strSender.GetLength();
				
				strSender = strSender.Left(nLength - 1);
				break;
			} 
			else if ((nSpace = strSender.Find('[')) != -1)
			{
				// e-mail address is enclosed in square brackets -- remove
				strSender = strSender.Right(nLength - nSpace - 1);
				
				// initialize nLength with new length of strSender
				nLength = strSender.GetLength();
				
				strSender = strSender.Left(nLength - 1);

				break;
			} 			
		}
	}

	if (strSender.IsEmpty())
	{
		for (i = 0;i < lines;i++)
		{
			/*wait.Restore();*/
			nLineLength = GetEditCtrl().GetLine(i, buffer, 5000);
			if (strnicmp(buffer, "Reply-To: ", 10) == 0)
			{
				buffer[nLineLength] = 0;
				strSender = buffer + 10;
				
				CString strSender2 = strSender; // in case we're wrong below
				// Chop off any parantheses, spaces, punctuation, names, etc.
				int nSpace = 0;     
				int nLength = strSender.GetLength();
				if (strSender.Right(1) == ">")
					nSpace = strSender.ReverseFind(' ');
				else
					nSpace = strSender.Find(' ');
					
				
				// Take string up to space
				if (nSpace == -1) 
				{
					break; // we have strSender -- bing! we're done.
				}
				
				// Take the strSender up to the space
				if (strSender.Right(1) == ">")
				{
					strSender = strSender.Right(nLength - nSpace - 1);
					strSender = strSender.Right(nLength - 1);  
					strSender = strSender.Left(nLength - 1);
					
					// bing! we now have strSender!
					break;
				}
						
				strSender = strSender.Left(nSpace);  
				
				if (strSender.Find('@') == -1)
				{
					// oops -- we guessed wrong -- other part of e-mail address
					// has the actual address
					strSender = strSender2; // get back original strSender
					strSender = strSender.Right(strSender.GetLength() 
							- nSpace - 1);   
				}  
				else
					break;
	
				if ((nSpace = strSender.Find('<')) != -1)
				{
					// e-mail address is enclosed in angle brackets -- remove
					// angle brackets
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('\"')) != -1)
				{
					// e-mail address is enclosed in quotation marks -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('(')) != -1)
				{
					// e-mail address is enclosed in parentheses -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('[')) != -1)
				{
					// e-mail address is enclosed in square brackets -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
	
					break;
				} 			
			}
		}    
	}

	if (strSender.IsEmpty())
	{
		for (i = 0;i < lines;i++)
		{ 
			/*wait.Restore();*/
			nLineLength = GetEditCtrl().GetLine(i, buffer, 5000);
			if (strnicmp(buffer, "From: ", 6) == 0)
			{
				buffer[nLineLength] = 0;
				strSender = buffer + 6;
			
				CString strSender2 = strSender; // in case we're wrong below
				// Chop off any parantheses, spaces, punctuation, names, etc.
				int nSpace = 0;     
				int nLength = strSender.GetLength();
				if (strSender.Right(1) == ">")
					nSpace = strSender.ReverseFind(' ');
				else
					nSpace = strSender.Find(' ');
					
				
				// Take string up to space
				if (nSpace == -1) 
				{
					break; // we have strSender -- bing! we're done.
				}
				
				// Take the strSender up to the space
				if (strSender.Right(1) == ">")
				{
					strSender = strSender.Right(nLength - nSpace - 1);
					strSender = strSender.Right(nLength - 1);  
					strSender = strSender.Left(nLength - 1);
					
					// bing! we now have strSender!
					break;
				}
						
				strSender = strSender.Left(nSpace);  
				
				if (strSender.Find('@') == -1)
				{
					// oops -- we guessed wrong -- other part of e-mail address
					// has the actual address
					strSender = strSender2; // get back original strSender
					strSender = strSender.Right(strSender.GetLength() 
							- nSpace - 1);   
				}  
				else
					break;
	
				if ((nSpace = strSender.Find('<')) != -1)
				{
					// e-mail address is enclosed in angle brackets -- remove
					// angle brackets
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('\"')) != -1)
				{
					// e-mail address is enclosed in quotation marks -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('(')) != -1)
				{
					// e-mail address is enclosed in parentheses -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('[')) != -1)
				{
					// e-mail address is enclosed in square brackets -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
	
					break;
				} 				
			}
		}
	}

	for (i = 0;i < lines;i++)
	{  
		/*wait.Restore();*/
		nLineLength = GetEditCtrl().GetLine(i, buffer, 5000);
		if (strnicmp(buffer, "Subject: ", 9) == 0)
		{
			buffer[nLineLength] = 0;
			strSubject = buffer + 9;
			break;
		}
	}

	// Process subject
	CString temp = strSubject;
	int nSpace;

	if (temp.Left(4) == "RE: ")
	{
		strSubject = temp.Right(temp.GetLength() - 4);
		temp = "Re: " + strSubject;    
		
		// Now fill strSubject with temp, strSubject's changed self
		strSubject = temp;
	}
	else if (strSubject.Left(2) == "[R" &&
		(nSpace = temp.Find(' ')) != -1)
	{
		int c = nSpace + 1;
		strSubject = temp.Right(temp.GetLength() - c);   
		temp = "Re: " + strSubject;  
		
		// Now fill strSubject with temp, strSubject's changed self
		strSubject = temp;
	}
	else if (strSubject.Left(4) != "Re: ")
	{
		// prepend "Re: " to strSubject
		temp = "Re: " + strSubject;     
		
		// Now fill strSubject with temp, strSubject's changed self
		strSubject = temp;
	}  
	else if (strSubject.Left(4) == "Re: ")
	{
		strSubject = strSubject;
	}
				
	TRACE0("INF: Transferring information to Compose Mail window.\r\n");
	pView->m_pFrame->GetInfoBar().m_strTo = strSender;
	pView->m_pFrame->GetInfoBar().m_strSubject = strSubject;		
	pView->m_pFrame->GetInfoBar().UpdateData(FALSE);        
	pView->m_pFrame->SetWindowText(strSubject);    
	
	pFrame->ActivateFrame(SW_RESTORE);
	pFrame->UpdateWindow();

	// we're done!
	return;
}

void CArticleView::OnArticleAdd()
{
	CArticleComposeWnd* pFrame = 
		(CArticleComposeWnd*)m_pApp->GetCNA()->CreateNewFrame(m_pDoc, NULL);
	if (pFrame == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	}

	m_pApp->GetCNA()->InitialUpdateFrame(pFrame, m_pDoc, TRUE);
	
	pFrame->GetArticleComposeBar().SetNewsgroups(GetGroupName());
	pFrame->GetArticleComposeBar().UpdateData(FALSE);  	
}    


void CArticleView::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CArticleView::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CArticleView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CArticleView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CArticleView::OnUpdateEditReplace(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView

IMPLEMENT_DYNCREATE(CArticleComposeView, CEditView)

BEGIN_MESSAGE_MAP(CArticleComposeView, CEditView)
	//{{AFX_MSG_MAP(CArticleComposeView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_MIRROR_DISPLAY_FONT, OnMirrorDisplayFont)
	ON_COMMAND(ID_WORD_WRAP, OnWordWrap)
	ON_UPDATE_COMMAND_UI(ID_WORD_WRAP, OnUpdateWordWrap)
	ON_COMMAND(ID_CHOOSE_PRINT_FONT, OnChoosePrintFont)
	ON_UPDATE_COMMAND_UI(ID_CHOOSE_PRINT_FONT, OnUpdateChoosePrintFont)
	ON_COMMAND(ID_CHOOSE_FONT, OnChooseFont)
	ON_COMMAND(ID_PAGE_SETUP, OnPageSetup)
	ON_UPDATE_COMMAND_UI(ID_MIRROR_DISPLAY_FONT, OnUpdateMirrorDisplayFont)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_ARTICLE_SEND, OnArticleSend)
	ON_UPDATE_COMMAND_UI(ID_ARTICLE_SEND, OnUpdateArticleSend)
	ON_BN_CLICKED(ID_NEWSGROUPS_SETUP, OnNewsgroupsSetup)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Message handler to prevent doc modification when this view is changed
	ON_EN_CHANGE(AFX_IDW_PANE_FIRST, OnEditChange)
	ON_MESSAGE(WM_COMPOSE_WIZARD, OnComposeWizard)
END_MESSAGE_MAP()

UINT CArticleComposeView::m_nDefTabStops;
UINT CArticleComposeView::m_nDefTabStopsOld;
BOOL CArticleComposeView::m_bDefWordWrap;
BOOL CArticleComposeView::m_bDefWordWrapOld;
LOGFONT NEAR CArticleComposeView::m_lfDefFont;
LOGFONT NEAR CArticleComposeView::m_lfDefFontOld;
LOGFONT NEAR CArticleComposeView::m_lfDefPrintFont;
LOGFONT NEAR CArticleComposeView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

static void GetArticleProfileFont(LPCSTR szSec, LOGFONT* plf)
{
	CWinApp* pApp = AfxGetApp();
	plf->lfHeight = pApp->GetProfileInt(szSec, "Height", 0);
	if (plf->lfHeight != 0)
	{
		plf->lfWeight = pApp->GetProfileInt(szSec, "Weight", 0);
		plf->lfItalic = (BYTE)pApp->GetProfileInt(szSec, "Italic", 0);
		plf->lfUnderline = (BYTE)pApp->GetProfileInt(szSec, "Underline", 0);
		plf->lfPitchAndFamily = (BYTE)pApp->GetProfileInt(szSec, "PitchAndFamily", 0);
		CString strFont = pApp->GetProfileString(szSec, "FaceName", "Arial");
		strncpy((char*)plf->lfFaceName, strFont, sizeof plf->lfFaceName);
		plf->lfFaceName[sizeof plf->lfFaceName-1] = 0;
	}
}

static void WriteArticleProfileFont(LPCSTR szSec, const LOGFONT* plf, LOGFONT* plfOld)
{
	CWinApp* pApp = AfxGetApp(); 
	if (plf->lfHeight != plfOld->lfHeight)
		pApp->WriteProfileInt(szSec, "Height", plf->lfHeight);
	if (plf->lfHeight != 0)
	{
		if (plf->lfHeight != plfOld->lfHeight)
			pApp->WriteProfileInt(szSec, "Height", plf->lfHeight);
		if (plf->lfWeight != plfOld->lfWeight)
			pApp->WriteProfileInt(szSec, "Weight", plf->lfWeight);
		if (plf->lfItalic != plfOld->lfItalic)
			pApp->WriteProfileInt(szSec, "Italic", plf->lfItalic);
		if (plf->lfUnderline != plfOld->lfUnderline)
			pApp->WriteProfileInt(szSec, "Underline", plf->lfUnderline);
		if (plf->lfPitchAndFamily != plfOld->lfPitchAndFamily)
			pApp->WriteProfileInt(szSec, "PitchAndFamily", plf->lfPitchAndFamily);
		if (strcmp(plf->lfFaceName, plfOld->lfFaceName) != 0)
			pApp->WriteProfileString(szSec, "FaceName", (LPCSTR)plf->lfFaceName);
	}
	*plfOld = *plf;
}

void CArticleComposeView::Initialize()
{ 
	CWinApp* pApp = AfxGetApp();
	// Enable word wrap by default
	m_bDefWordWrap = pApp->GetProfileInt("Compose News Article: Settings", "WordWrap", TRUE);
	m_bDefWordWrapOld = m_bDefWordWrap;
	m_nDefTabStops = pApp->GetProfileInt("Compose News Article: Settings", "TabStops", 8*4);
	m_nDefTabStopsOld = m_nDefTabStops;
	GetArticleProfileFont("Compose News Article: Font", &m_lfDefFont);
	m_lfDefFontOld = m_lfDefFont;
	GetArticleProfileFont("Compose News Article: PrintFont", &m_lfDefPrintFont);
	m_lfDefPrintFontOld = m_lfDefPrintFont;       
	
}

void CArticleComposeView::Terminate()
{
	CWinApp* pApp = AfxGetApp();     
	// Save settings if they changed
	if (m_nDefTabStops != m_nDefTabStopsOld)
		pApp->WriteProfileInt("Compose News Article: Settings", "TabStops", m_nDefTabStops);
	if (m_bDefWordWrap != m_bDefWordWrapOld)
		pApp->WriteProfileInt("Compose News Article: Settings", "WordWrap", m_bDefWordWrap);
	WriteArticleProfileFont("Compose News Article: Font", &m_lfDefFont, &m_lfDefFontOld);
	WriteArticleProfileFont("Compose News Article: PrintFont", &m_lfDefPrintFont, &m_lfDefPrintFontOld);
	
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView construction/destruction

CArticleComposeView::CArticleComposeView()
{
	// Load in settings from INI file
	Initialize();
	
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp(); 
	m_pDoc = NULL;
	m_bUndo = FALSE; // start out unable to undo or redo anything   
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	
	m_bFollowup = m_bOriginalMaterial = FALSE;
}

BOOL CArticleComposeView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

CArticleComposeView::~CArticleComposeView()
{ 
	m_pApp = NULL;
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView initial update

void CArticleComposeView::OnInitialUpdate()
{
	// First, update the frame's title
	((CArticleComposeWnd*)GetParentFrame())->OnUpdateFrameTitle(TRUE);

	// Make sure the user can modify this view's contents
	GetEditCtrl().SetReadOnly(FALSE);

	// Make double-sure we have the status bar
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();  
    
    // Initialize pointer-to-document variable
  	m_pDoc = GetDocument();
  	
	// Now call the default base-class version of this function
	CEditView::OnInitialUpdate();     
	
	if (!m_bFollowup)
	{
		// Start the News Compose Wizard  
		GetParentFrame()->ActivateFrame(SW_HIDE);
		
		PostMessage(WM_COMPOSE_WIZARD);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView Article Compose Wizard

LRESULT CArticleComposeView::OnComposeWizard(WPARAM, LPARAM)
{
	DoComposeNewsWizard();
	return 0;
}

void CArticleComposeView::DoComposeNewsWizard()
{
	CCompNewsWiz1 wiz(m_pApp->GetMainFrame());
	
	switch (wiz.DoModal())
	{
		case IDOK:		// Next
			DoCompNewsWiz2();
			break;
		
		case IDCANCEL:	// Cancel
			((CArticleComposeWnd*)GetParentFrame())->DestroyWindow();
			break;
	}
}

void CArticleComposeView::DoCompNewsWiz2()
{
	// Show Step 2
	CCompNewsWiz2 wiz2(m_pApp->GetMainFrame());
	wiz2.m_strSubject = ((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().GetSubject();
	
	int nResult = wiz2.DoModal();
	CString strSubject = "";
	
	switch (nResult) 
	{
		case IDOK:		// Next button pressed
			// Go on to Step 3
			strSubject = wiz2.m_strSubject;
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().SetSubject(strSubject);
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().UpdateData(FALSE);
					
			DoCompNewsWiz3();
			break;
		
		case IDRETRY:	// Back button pressed
			// Go back to Step 1
			strSubject = wiz2.m_strSubject;
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().SetSubject(strSubject);
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().UpdateData(FALSE);
					
			DoComposeNewsWizard();			
			break;
		
		case IDCANCEL:	// Cancel button pressed   
			((CArticleComposeWnd*)GetParentFrame())->DestroyWindow();
			break;    			
	}
}

void CArticleComposeView::DoCompNewsWiz3()
{
	// Show Step 3
	CCompNewsWiz3 wiz3(m_pApp->GetMainFrame());
	wiz3.m_strNewsgroups = ((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().GetNewsgroups();
	
	int nResult = wiz3.DoModal();
	CString strNewsgroups = "";
	
	switch (nResult) 
	{
		case IDOK:		// Next button pressed
			// Go on to Step 4
			strNewsgroups = wiz3.m_strNewsgroups;
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().SetNewsgroups(strNewsgroups);
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().UpdateData(FALSE);
					
			DoCompNewsWiz4();
			break;
		
		case IDRETRY:	// Back button pressed
			// Go back to Step 2
			strNewsgroups = wiz3.m_strNewsgroups;
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().SetNewsgroups(strNewsgroups);
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().UpdateData(FALSE);
					
			DoCompNewsWiz2();			
			break;
		
		case IDCANCEL:	// Cancel button pressed   
			((CArticleComposeWnd*)GetParentFrame())->DestroyWindow();
			break;    			
	}
}

void CArticleComposeView::DoCompNewsWiz4()
{
	// Show Step 4
	CCompNewsWiz4 wiz4(m_pApp->GetMainFrame());
	wiz4.m_strDistribution = ((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().GetDistribution();
	
	int nResult = wiz4.DoModal();
	CString strDistribution = "";
	
	switch (nResult) 
	{
		case IDOK:		// Next button pressed
			// Go on to Step 5
			strDistribution = wiz4.m_strDistribution;
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().SetDistribution(strDistribution);
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().UpdateData(FALSE);
					
			DoCompNewsWiz5();
			break;
		
		case IDRETRY:	// Back button pressed
			// Go back to Step 3
			strDistribution = wiz4.m_strDistribution;
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().SetDistribution(strDistribution);
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().UpdateData(FALSE);
			
			DoCompNewsWiz3();			
			break;
		
		case IDCANCEL:	// Cancel button pressed   
			((CArticleComposeWnd*)GetParentFrame())->DestroyWindow();
			break;    			
	}
}

void CArticleComposeView::DoCompNewsWiz5()
{
	// Show Step 5
	CCompNewsWiz5 wiz5(m_pApp->GetMainFrame()); 
	wiz5.m_strFollowupTo = ((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().GetFollowupTo();
	
	int nResult = wiz5.DoModal();
	CString strFollowupTo = "";
	
	switch (nResult) 
	{
		case IDOK:		// Next button pressed
			// Go on to Finish
			strFollowupTo = wiz5.m_strFollowupTo;
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().SetFollowupTo(strFollowupTo);
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().UpdateData(FALSE);
					
			DoCompNewsWiz6();
			break;
		
		case IDRETRY:	// Back button pressed
			// Go back to Step 4
			strFollowupTo = wiz5.m_strFollowupTo;
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().SetFollowupTo(strFollowupTo);
			((CArticleComposeWnd*)GetParentFrame())->GetArticleComposeBar().UpdateData(FALSE);
			DoCompNewsWiz4();			
			break;
		
		case IDCANCEL:	// Cancel button pressed
			((CArticleComposeWnd*)GetParentFrame())->DestroyWindow();
			break;    		
	}
}

void CArticleComposeView::DoCompNewsWiz6()
{
	// Show the Finish -- Step 6
	CCompNewsWiz6 wizFinish(m_pApp->GetMainFrame());
	
	int nResult = wizFinish.DoModal();
	switch (nResult)
	{
		case IDOK:		// Finish button pressed
			// Make this view and frame window visible -- the user is
			// finished!
			((CArticleComposeWnd*)GetParentFrame())->ShowWindow(SW_SHOW);
			((CArticleComposeWnd*)GetParentFrame())->UpdateWindow();
			break;                    
			
		case IDRETRY:	// Back button pressed
			// Go back to Step 5
			DoCompNewsWiz5();
			break;
		
		case IDCANCEL:	// Cancel button pressed   
			((CArticleComposeWnd*)GetParentFrame())->DestroyWindow();
			break;    			
	}
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView date

// To be as accurate as we can about the date and time of posting for this
// article, the FillDate() function will be called just before the Date 
// header is sent to the server.

void CArticleComposeView::FillDate()
{
	CString strDateFormat = "";
	m_strDate.Empty(); // start with a blank date

	CTime curTime = CTime::GetCurrentTime();
	strDateFormat = curTime.FormatGmt("%a, %d %b %Y %H:%M:%S ");
	m_strDate = strDateFormat;
	
	// switch signs on the _timezone time zone difference variable
	if (_timezone > 0)
	{
		m_strDate += "-";
	}
	else if (_timezone < 0)
	{
		m_strDate += "+";
	} 
	CTimeSpan timeSpan((time_t)_timezone);
	m_strDate += timeSpan.Format("%H%M (");
	m_strDate += curTime.Format("%Z");
	m_strDate += ")";
	
	// bing! done.
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView followup

BOOL CArticleComposeView::DoFollowup(CString& strReferences,
	CString& strNewsgroups, CString& strSubject, CString& strMessageID,
		CString& strAuthor, CString& strDistribution)
{   
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	QSocket* pSocket = GetConnection();
	if (pSocket == NULL)
	{
		m_pStatusBar->ShowIdleMessage();
		return FALSE;                   
	}

	SetFollowup(); // this article is a followup
	
	CArticleComposeWnd* pFrame = (CArticleComposeWnd*)GetParentFrame();
	
	if (strMessageID.IsEmpty() || strNewsgroups.IsEmpty()
		|| strSubject.IsEmpty())
	{
		SetFollowup(FALSE);
		pFrame->SendMessage(WM_CLOSE);
		return FALSE;
	}
		
	// Inform the dialog bar, in which the user types header information,
	// that this is a followup
	if (!pFrame->m_wndComposeBar.DoFollowup(strNewsgroups,
			strSubject, strAuthor, strDistribution))
	{
		SetFollowup(FALSE);
		pFrame->SendMessage(WM_CLOSE);
		return FALSE;
	}

	if (!strReferences.IsEmpty())
		SetReferences(strReferences);
	else   // if empty References header, then use message ID
		SetReferences(strMessageID);
	
	SetMessageID(strMessageID); // message ID of article to quote
	SetNewsgroups(strNewsgroups); // Newsgroups to post this followup to
	SetSubject(strSubject); // Subject of this followup article
	
	pFrame->SetWindowText(strSubject);
	
	// Get the body of this article, quote it, and put it in this view
	CString strText = "Quoting body of article " + strMessageID + "...";
	strText += "  Please wait.";
	m_pStatusBar->SetText(strText);
	
	// If there's any text in this view, clear it
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	CEditView::OnEditSelectAll();
	CEditView::OnEditClear();    
	
	// Make sure the user can't undo the clear action
	GetEditCtrl().EmptyUndoBuffer();
	
	// Format of attribution line is:
	// In article <a>,\r\n\tuser@host.com (User Name) writes:
	// (where <a> is the MessageID of the inspiring article
		
	CString strAttribution = "In article " + strMessageID;
	strAttribution += ",\r\n\t" + strAuthor + " writes:\r\n\r\n";
	
	// Add attribution line to this view
	int nLen = GetEditCtrl().GetWindowTextLength();
	GetEditCtrl().SetSel(nLen, nLen, TRUE);
	GetEditCtrl().ReplaceSel(strAttribution);
	
	m_bOriginalMaterial = FALSE;
	
	// Send the BODY command to the NNTP server so we can
	// receive the text body of the article we need to quote
	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	pSocket->Send("BODY " + strMessageID + "\r\n");
	
	m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
	CString strResponse = pSocket->GetLine();

	if (atoi(strResponse.Left(3)) == 222) // body retrieved successfully
	{	
		CString strLine = "";
		int nLen; 
		m_pStatusBar->SetText(IDS_RECEIVING_ARTICLE);
		while ((strResponse = pSocket->GetLine()) != ".\r\n"
			&& pSocket->IsConnected())
		{
			strLine = ">" + strResponse;
			
			// Add strLine to the edit ctrl
			nLen = GetEditCtrl().GetWindowTextLength();
			GetEditCtrl().SetSel(nLen, nLen, TRUE);
			GetEditCtrl().ReplaceSel(strLine);
			
			m_bOriginalMaterial = FALSE;	
		}
		
		strLine = "\r\n\r\n";
		
		nLen = GetEditCtrl().GetWindowTextLength();
		GetEditCtrl().SetSel(nLen, nLen, TRUE);
		GetEditCtrl().ReplaceSel(strLine);

		m_bOriginalMaterial = FALSE;
		
		m_pStatusBar->SetText("Article transfer complete.");
		AfxGetMainWnd()->BringWindowToTop();  
		
		// Release our connection back to the area
		ReleaseConnection();			
	}
	else
	{
		// Display the error discription for the user
		int nResponseCode = atoi(strResponse.Left(3));
		m_pDoc->DisplayNNTPError(nResponseCode);
		
		ReleaseConnection();
		
		pFrame->SendMessage(WM_CLOSE);
		
		return FALSE;
	}
	
    // article was successfully prepared and quoted!
    // Show the window and put the idle message on the status bar
    m_pStatusBar->ShowIdleMessage();
    
    pFrame->ActivateFrame(SW_RESTORE);
    pFrame->UpdateWindow();
    
    m_bOriginalMaterial = FALSE;
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView Word Wrap support

BOOL CArticleComposeView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL CArticleComposeView::SetWordWrap(BOOL bWordWrap)
{
	bWordWrap = !!bWordWrap;    // make sure ==TRUE || ==FALSE
	if (IsWordWrap() == bWordWrap)
		return FALSE;

	// preserve original control's state.
	CFont* pFont = GetFont();
	int nLen = GetBufferLength();
	char FAR* pSaveText = new far char[GetBufferLength()+1];
	GetWindowText(pSaveText, nLen+1);

	// create new edit control with appropriate style and size.
	DWORD dwStyle = dwStyleDefault & ~(ES_AUTOHSCROLL|WS_HSCROLL|WS_VISIBLE);
	if (!bWordWrap)
		dwStyle |= ES_AUTOHSCROLL|WS_HSCROLL;

	CWnd* pParent = GetParent();
	CRect rect;
	GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	CWnd* pFocus = GetFocus();

	UINT nID = GetDlgCtrlID();

	HWND hWnd = ::CreateWindow("edit", NULL, dwStyle,
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
		pParent->m_hWnd, (HMENU)nID,
		(HINSTANCE)m_segText, NULL);

	if (hWnd == NULL)
	{
		delete[] pSaveText;
		return FALSE;
	}

	// set the window text to nothing to make sure following set doesn't fail
	SetWindowText(NULL);

	// restore visual state
	::SetWindowText(hWnd, pSaveText);
	delete[] pSaveText;
	if (pFont != NULL)
	{
		ASSERT(pFont->m_hObject != NULL);
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)pFont->m_hObject, 0);
	}

	// detach old window, attach new
	HWND hWndOld = Detach();
	::SetWindowLong(hWndOld, GWL_WNDPROC, (LONG)*GetSuperWndProcAddr());
	::SetWindowWord(hWndOld, GWW_ID, nID+1);
	ASSERT(m_hWnd == NULL);
	SubclassWindow(hWnd);
	ASSERT(m_hWnd == hWnd);
	UINT nTabStops = m_nTabStops;
	GetParentFrame()->SendMessage(WM_RECALCPARENT);
	GetEditCtrl().SetTabStops(nTabStops);
	GetClientRect(&rect);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_DRAWFRAME);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	UpdateWindow();

	// destroy old
	::SetWindowPos(hWndOld, NULL, 0, 0, 0, 0,
		SWP_HIDEWINDOW|SWP_NOREDRAW|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|
		SWP_NOZORDER);
	::DestroyWindow(hWndOld);

	// restore rest of state...
	GetEditCtrl().LimitText(nMaxSize);
	if (pFocus == this)
		SetFocus();

	ASSERT_VALID(this);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView drawing

void CArticleComposeView::OnDraw(CDC* pDC)
{
	CNewsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView printing

BOOL CArticleComposeView::OnPreparePrinting(CPrintInfo* pInfo)
{ 
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void CArticleComposeView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// First, let CEditView prepare to begin printing
	CEditView::OnBeginPrinting(pDC, pInfo);
	
	CString strFileName = GetDocument()->GetPathName(); 
	if (strFileName.IsEmpty()) strFileName = GetDocument()->GetTitle(); 
	BOOL bForceSysTime = strchr(strFileName, '.') == NULL;
	CTime timeSys = CTime::GetCurrentTime();
	CFileStatus status;
	CFile::GetStatus(strFileName, status);
	
	if (m_nHeaderTime != 0 || bForceSysTime)
		m_timeHeader = timeSys;
	else
		m_timeHeader = status.m_mtime;
    	
    if (m_nFooterTime != 0 || bForceSysTime)
		m_timeFooter = timeSys;
	else
		m_timeFooter = status.m_mtime;

	if (!pInfo->m_bPreview)
		return;

	pInfo->m_nCurPage = 0xFFFF;
	OnPrepareDC(pDC, pInfo);

	UINT nIndex = LOWORD(GetEditCtrl().GetSel());
	UINT nCurPage = 1;
	while (nCurPage < (UINT)m_aPageStart.GetSize())
	{
		if (nIndex < m_aPageStart[nCurPage])
			break;
		nCurPage++;
	}
	pInfo->m_nCurPage = nCurPage;        
	
	// Fill the CPrintInfo struct with needed information
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(m_aPageStart.GetSize()); // number of pages of message
	pInfo->m_pPD->m_pd.nMinPage = 1;
	pInfo->m_pPD->m_pd.nMaxPage = m_aPageStart.GetSize();
	pInfo->m_pPD->m_pd.nFromPage = 1;
	pInfo->m_pPD->m_pd.nToPage = m_aPageStart.GetSize();

	m_nPreviewPage = nCurPage;
}

void CArticleComposeView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// get string to show as "filename" in header/footer
	CPageSetupSheet shtPageSetup;
	CString strFileName = GetDocument()->GetPathName();
	if (strFileName.IsEmpty())
		strFileName = GetDocument()->GetTitle();

	// go thru CPageSetupSheet::CPageSetupPage to format the header 
	// and footer
	CString strHeader = "";
	strHeader = "New Newsgroup Article";
	CString strFooter = "";        
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.FormatFooter(strFooter, m_timeFooter, strFileName,
		pInfo->m_nCurPage);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cyChar = tm.tmHeight;
	CRect rectPage = pInfo->m_rectDraw;

	// draw and exclude space for header
	if (!strHeader.IsEmpty())
	{
		pDC->TextOut(rectPage.left, rectPage.top, strHeader);
		rectPage.top += cyChar + cyChar / 4;
		pDC->MoveTo(rectPage.left, rectPage.top);
		pDC->LineTo(rectPage.right, rectPage.top);
		rectPage.top += cyChar / 4;
	}

	// allow space for footer
	pInfo->m_rectDraw = rectPage;
	if (!strFooter.IsEmpty())
		pInfo->m_rectDraw.bottom -= cyChar + cyChar/4 + cyChar/4;

	// draw body text
	CEditView::OnPrint(pDC, pInfo);

	// draw footer
	if (!strFooter.IsEmpty())
	{
		rectPage.bottom -= cyChar;
		pDC->TextOut(rectPage.left, rectPage.bottom, strFooter);
		rectPage.bottom -= cyChar / 4;
		pDC->MoveTo(rectPage.left, rectPage.bottom);
		pDC->LineTo(rectPage.right, rectPage.bottom);
		rectPage.bottom -= cyChar / 4;
	}
}

void CArticleComposeView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void CArticleComposeView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView diagnostics

#ifdef _DEBUG
void CArticleComposeView::AssertValid() const
{
	CEditView::AssertValid();
}

void CArticleComposeView::Dump(CDumpContext& dc) const
{
	// Dump header strings
	
	dc << "m_strSubject: " << m_strSubject << "\r\n";
	dc << "m_strFrom: " << m_strFrom << "\r\n";
	dc << "m_strNewsgroups: " << m_strNewsgroups << "\r\n";
	dc << "m_strDate: " << m_strDate << "\r\n";
	dc << "m_strReplyTo: " << m_strReplyTo << "\r\n";
	dc << "m_strFollowupTo: " << m_strFollowupTo << "\r\n";
	dc << "m_strDistribution: " << m_strDistribution << "\r\n";
	dc << "m_strReferences: " << m_strReferences << "\r\n";
	dc << "m_strKeywords: " << m_strKeywords << "\r\n";
	
	CEditView::Dump(dc);
}

CNewsDoc* CArticleComposeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewsDoc)));
	return (CNewsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView message handlers

static void ScaleArticleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member from one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}
 
int CArticleComposeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;            
		
	if (m_lfDefFont.lfHeight != 0)
	{
		m_font.CreateFontIndirect(&m_lfDefFont);
		SetFont(&m_font);
	}
	if (m_lfDefPrintFont.lfHeight != 0)
	{
		m_fontPrint.CreateFontIndirect(&m_lfDefPrintFont);
		SetPrinterFont(&m_fontPrint);
	}
	return 0;
}

void CArticleComposeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing commands
	CEditView::OnRButtonDown(nFlags, point);
}

void CArticleComposeView::OnSize(UINT nType, int cx, int cy)
{
	CWaitCursor wait;
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void CArticleComposeView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void CArticleComposeView::OnWordWrap()
{
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void CArticleComposeView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void CArticleComposeView::OnChoosePrintFont()
{  
	CFont* pFont = GetPrinterFont();
	LOGFONT lf;
	LPLOGFONT plf = NULL;
	if (pFont != NULL)
	{
		pFont->GetObject(sizeof(LOGFONT), &lf);
		plf = &lf;
	}

	// magic to get printer dialog that would be used if we were printing!
	CPrintDialog dlgPrint(FALSE);
	if (!AfxGetApp()->GetPrinterDeviceDefaults(&dlgPrint.m_pd))
	{
		AfxMessageBox(IDP_ERR_GET_DEVICE_DEFAULTS);
		return;
	}
	
	HDC hdcPrint = dlgPrint.CreatePrinterDC();
	if (hdcPrint == NULL)
	{
		AfxMessageBox(IDP_ERR_GET_PRINTER_DC);
		return;
	}

	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));
	CDC dcPrint;
	dcPrint.Attach(hdcPrint);

	if (plf != NULL)
	{
		// need to map initial logfont to screen metrics.
		::ScaleArticleLogFont(plf, dcPrint, dcScreen);
	}

	// now bring up the dialog since we know the printer DC
	CFontDialog dlg(plf, CF_PRINTERFONTS, &dcPrint);
	if (dlg.DoModal() == IDOK)
	{
		// map the resulting logfont back to printer metrics.
		lf = dlg.m_lf;
		::ScaleArticleLogFont(&lf, dcScreen, dcPrint);

		m_fontPrint.DeleteObject();
		if (m_fontPrint.CreateFontIndirect(&lf))
		{
			SetPrinterFont(&m_fontPrint);
			m_lfDefPrintFont = lf;
		}
	}
	//NOTE: destructor will call dcPrint.DeleteDC

	::ReleaseDC(NULL, dcScreen.Detach());
}

void CArticleComposeView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void CArticleComposeView::OnChooseFont()
{
   // get current font description
   CFont* pFont = GetFont();
   LOGFONT lf;
   if (pFont != NULL)
	   pFont->GetObject(sizeof(LOGFONT), &lf);
   else
	   ::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

	CFontDialog dlg(&lf, CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
	if (dlg.DoModal() == IDOK)
	{
		// switch to new font.
		m_font.DeleteObject();
		if (m_font.CreateFontIndirect(&lf))
		{
			CWaitCursor wait;
			SetFont(&m_font);
			m_lfDefFont = lf;
    	}
	}
}

void CArticleComposeView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());      
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.m_bOutputView = TRUE;
	shtPageSetup.m_pgTabStops.m_nTabStops = m_nTabStops/4;
	if (shtPageSetup.DoModal() == IDOK)
	{
		// get values entered into this tabbed dialog box 
		shtPageSetup.m_pgHeaderFooter.Terminate();
		
		m_nHeaderTime = shtPageSetup.m_pgHeaderFooter.m_nHeaderTime;
		m_nFooterTime = shtPageSetup.m_pgHeaderFooter.m_nFooterTime;
		SetTabStops(shtPageSetup.m_pgTabStops.m_nTabStops*4);
		m_nDefTabStops = m_nTabStops;
     		
		return;
	}  
	return;
}

void CArticleComposeView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void CArticleComposeView::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strArtCompSect = "ArtComp";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strArtCompSect, "ArtCompLeft", rect.left);
	pApp->WriteProfileInt(strArtCompSect, "ArtCompRight", rect.right);
	pApp->WriteProfileInt(strArtCompSect, "ArtCompTop", rect.top);
	pApp->WriteProfileInt(strArtCompSect, "ArtCompBottom", rect.bottom);
	pApp->WriteProfileInt(strArtCompSect, "ArtCompCX", rect.Width());
	pApp->WriteProfileInt(strArtCompSect, "ArtCompCY", rect.Height());
}

void CArticleComposeView::OnEditUndo()                      
{ 
	CEditView::OnEditUndo();
	
	// change the "redo flag" to TRUE so the undone action can be redone
	m_bUndo = TRUE;
	return;
}

void CArticleComposeView::OnEditRedo()
{ 
	if (m_bUndo)
	{
		CEditView::OnEditUndo();
		m_bUndo = FALSE;	// can't redo anymore -- not until another undo
	}
	return;	// we're done
}	

void CArticleComposeView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bUndo && GetEditCtrl().CanUndo());
	return;
} 

void CArticleComposeView::OnEditChange()
{     
	m_bUndo = FALSE;
	
	if (IsFollowup() && !m_bOriginalMaterial)
		m_bOriginalMaterial = TRUE;
	else if (!IsFollowup())
		m_bOriginalMaterial = TRUE;
    
    CString str = "";
    GetEditCtrl().GetWindowText(str);
    m_bOriginalMaterial = !str.IsEmpty();
    
	GetDocument()->SetModifiedFlag(FALSE);
	return;
} 

void CArticleComposeView::OnArticleSend()
{
	// First, get the headers the user entered into the dialog bar
	// into the member variables of this view in a transmittable form
	
	CArticleComposeWnd* pFrame = (CArticleComposeWnd*)GetParentFrame();
	
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP|MB_OK); 
		return;
	}
	
	pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();
	m_pApp->GetMainFrame()->ResetToolBar();
	
	if (!pFrame->UpdateBarInfo())
	{
		MessageBeep(-1);
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP);
		pFrame->ActivateFrame(SW_RESTORE);
		pFrame->UpdateWindow();
		return;                
	}

	if (!PrepareHeaders())
	{
		AfxMessageBox(IDP_ARTICLE_INVALID_HEADERS);
		pFrame->ActivateFrame(SW_RESTORE);
		pFrame->UpdateWindow();
		return;
	}
	
	CString strResponse = "", strCommand = "";
	CPercentDialog* pDialog = NULL;
	
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	
	QSocket* pSocket = GetConnection();
	if (pSocket == NULL)
	{
		// since this window was hidden earlier, re-display it
		// and let user decide whether or not it is to be closed
		// leaving it hidden will confuse the user
		
		pFrame->ActivateFrame(SW_RESTORE);
		pFrame->UpdateWindow();
		
		m_pStatusBar->ShowIdleMessage();
		
		return;   // GetConnection() handles the error stuff
	} 
	
	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	
	// Send the POST command to the server
	pSocket->Send("POST\r\n");
	strResponse = pSocket->GetLine(); 
	
	// diagnostics
	#ifdef _DEBUG
		afxDump << strResponse;
	#endif
	
	char szBuffer[5004]; // for \r,\n, zero term, and perhaps an extra .
	UINT nLines, nLineLength;
	
	// First, insert soft line breaks
	GetEditCtrl().FmtLines(TRUE);
	
	nLines = GetEditCtrl().GetLineCount();
	
	// Get the response code from the server
	int nResponseCode = atoi(strResponse.Left(3));
	
	if (nResponseCode == 340) // Send article to be posted
	{
		// let's send stuff to the server -- it's ready
		m_pStatusBar->SetText(IDS_DELIVERING_NEWS);
		SendHeaders(pSocket);
		
		m_pStatusBar->SetText(IDS_DELIVERING_NEWS);
		// Send the article text
		/*CString strLeftovers = ""; */
		UINT i = 0;
		for (i = 0;i < nLines;i++)
		{
			m_pStatusBar->SetText(IDS_DELIVERING_NEWS);
			nLineLength = GetEditCtrl().GetLine(i, szBuffer, 5000);
			szBuffer[nLineLength] = 0;
			
			if (szBuffer[0] == '.')
				strCommand = '.' + szBuffer;
			else
				strCommand = szBuffer;
			
			strCommand += "\r\n";
			pSocket->Send(strCommand);
		}
		
		m_pStatusBar->ShowIdleMessage();
		// Send the user's signature
		if (!m_pApp->m_strDefMailboxUser.IsEmpty() && 
				!m_pApp->m_strDefMailboxOrg.IsEmpty())
		{
			m_pStatusBar->SetText("Sending signature...  Please wait.");
			
			pSocket->Send("\r\n");
			pSocket->Send("Sincerely,\r\n");
			pSocket->Send(m_pApp->m_strDefMailboxUser + "\r\n");
			pSocket->Send(m_pApp->m_strDefMailboxOrg + "\r\n");
			
			// A period on a line by itself tells the server we're done
			// sending this article    
			m_pStatusBar->ShowIdleMessage();
			pSocket->Send("\r\n.\r\n");
		}			
		else
		{
			// A period on a line by itself tells the server we're done
			// sending this article
			pSocket->Send("\r\n.\r\n");
		}
		
		m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
		strResponse = pSocket->GetLine();  
		
		// diagnostics
		#ifdef _DEBUG
			afxDump << strResponse;
		#endif
		
		int nResponseCode2 = atoi(strResponse.Left(3));
		if (nResponseCode2 == 240)  // Article posted ok
		{   
			// Article post was successful!  Tell the user, then
			// release our connection back to the newsgroups area
			AfxMessageBox(IDP_ARTICLE_POST_SUCCESS, MB_ICONASTERISK);
			ReleaseConnection();
            
            // show the idle message on the status bar
            m_pStatusBar->ShowIdleMessage();
            
            // Close the frame window; will close this view
			pFrame->SendMessage(WM_CLOSE);
			return;
		}
		else if (nResponseCode2 == 441)
		{  
			// Response code 441 means that the server didn't accept our
			// article for some reason   
			MessageBeep(-1);
			AfxMessageBox(IDP_ARTICLE_NOT_POSTED);
			ReleaseConnection();
			
			// show the idle message on the status bar
			m_pStatusBar->ShowIdleMessage();
			
			pFrame->ActivateFrame(SW_RESTORE);
			pFrame->UpdateWindow();
			return;
		}
	}
	else if (nResponseCode == 440)
	{   
		
		MessageBeep(-1);
		AfxMessageBox(IDP_ARTICLE_POST_DENIED, MB_ICONSTOP|MB_OK);
		
		m_pDoc->m_bCanPost = FALSE;
		m_pDoc->OnFileSave(); // Save the changed setting
		
		m_pApp->OnIdle(0);
		m_pApp->OnIdle(1);
		
		ReleaseConnection();
		
		// show the idle message on the status bar
		m_pStatusBar->ShowIdleMessage();
		
		pFrame->ShowWindow(SW_HIDE);
		pFrame->UpdateWindow();
		
		pFrame->SendMessage(WM_CLOSE);
		return;
	}		
}

QSocket* CArticleComposeView::GetConnection()
{
	if (!m_pApp->IsOnline())
	{  
		// If the user is not connected to the Internet, start a FlashConnect
		if (!m_pApp->SetFlashConnect(TRUE))
			return NULL; // user doesn't want to sign on right now
		
		m_pDoc->m_bArticleFlashConnect = m_pApp->IsFlashConnect();
		
		// If we made it this far, connect to the newsgroups server
		if (!m_pDoc->ConnectToNewsServer())
			return NULL; // error occurred while trying to connect
	}
	
	// Get the connection we have with the newsgroups server from the
	// document; if the connection is busy, we alert the user and return
	// faliure   
	
	QSocket* pResultSocket = m_pDoc->GetSocket();
	
	if (pResultSocket == NULL) // connection is busy -- alert user
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_CONNECTION_BUSY,
			MB_ICONINFORMATION);
		
		// abort the FlashConnect, if started
		if (m_pApp->IsFlashConnect())
		{  
			m_pStatusBar->SetText("The connection to the news server is busy.");

			if (m_pDoc->m_bArticleFlashConnect)
			{ 
				m_pDoc->ReleaseSocket();
				
				// Disconnect from the news server now or an error will
				// result when the FlashConnect ends, this document closes
				// and we try to disconnect a socket after the WSACleanup()
				// call
				m_pDoc->DisconnectFromNewsServer();  
			}
				
			m_pApp->AbortFlashConnect();
		}
		
		return NULL;
	}
	
	// If we made it this far, pResultSocket contains our connection --
	// return it to the caller of this function
	return pResultSocket;
}

void CArticleComposeView::ReleaseConnection()
{ 
	// Here's what we'll do -- first, we'll release the socket back to the
	// area.  That's all we'll do if this is not happening in a FlashConnect
	// (we'll do this also in a FlashConnect as well).  Next, if we're in
	// a FlashConnect, we'll tell the document to disconnect from the
	// newsgroups server and to throw away the connection; then, if we're
	// in a FlashConnect, we'll tell the application class to end the
	// FlashConnect.  Bing! Done.
	
	m_pDoc->ReleaseSocket(); // Releases socket back to the area
	
	if (m_pApp->IsFlashConnect())
	{
		// Close the connection
		m_pStatusBar->SetText("Disconnecting from newsgroups server..."); 

		m_pDoc->DisconnectFromNewsServer();
		
		m_pStatusBar->SetText("Ending FlashConnect...  Please wait.");
		m_pApp->SetFlashConnect(FALSE); 
		
		// Bing! Done.
		return;
	}
}		

BOOL CArticleComposeView::PrepareHeaders()
{
	CArticleComposeWnd* pFrame = (CArticleComposeWnd*)GetParentFrame();

	// Prepare the subject headers
	m_pStatusBar->SetText("Preparing article headers...  Please wait.");

	m_strSubject.Empty();
	m_strSubject = "Subject: ";
	m_strSubject += pFrame->m_wndComposeBar.m_strSubject;
	m_strSubject += "\r\n";
	
	if (!pFrame->m_wndComposeBar.m_strDistribution.IsEmpty())
	{  
		m_strDistribution.Empty();
		m_strDistribution = "Distribution: ";
		m_strDistribution += pFrame->m_wndComposeBar.m_strDistribution;
		m_strDistribution += "\r\n";
	}

	m_strNewsgroups.Empty();
	m_strNewsgroups = "Newsgroups: ";
	m_strNewsgroups += pFrame->m_wndComposeBar.m_strNewsgroups;
	m_strNewsgroups += "\r\n";

	if (!pFrame->m_wndComposeBar.m_strFollowupTo.IsEmpty())
	{
		m_strFollowupTo.Empty();
		m_strFollowupTo = "Followup-To: ";
		m_strFollowupTo += pFrame->m_wndComposeBar.m_strFollowupTo;
		m_strFollowupTo += "\r\n";
	}
	
	if (!m_strReferences.IsEmpty())
	{  
		CString strReferences = m_strReferences;
		m_strReferences = "References: ";
		m_strReferences += strReferences;
		m_strReferences += "\r\n";
	}
	
	if (m_pDoc->m_strEmailAddress.IsEmpty())
	{
		// E-Mail address must not be empty!
		AfxMessageBox("Need e-mail address; can't proceed.", 
			MB_ICONSTOP|MB_OK);
		return FALSE;
	}
		
	m_strFrom.Empty();
	m_strFrom = "From: " + m_pDoc->m_strEmailAddress + " (";
	m_strFrom += m_pApp->GetUserName();
	m_strFrom += ")\r\n";

	m_pStatusBar->ShowIdleMessage();
	return TRUE;
}

void CArticleComposeView::SendHeaders(QSocket* pSocket)
{
	m_pStatusBar->SetText("Sending article headers...  Please wait.");

	pSocket->Send(m_strFrom);
	pSocket->Send("Reply-To: " + m_pDoc->m_strEmailAddress + "\r\n");
	pSocket->Send(m_strNewsgroups);
	pSocket->Send("Organization: " + m_pApp->GetUserCompany() +"\r\n");

	if (!m_strFollowupTo.IsEmpty())
		pSocket->Send(m_strFollowupTo);

	pSocket->Send(m_strSubject);

	FillDate(); 
	pSocket->Send("Date: " + m_strDate + "\r\n");

	if (!m_strReferences.IsEmpty()) 
		pSocket->Send(m_strReferences);

	if (!m_strDistribution.IsEmpty())
		pSocket->Send(m_strDistribution);
    
    // Now give ourselves credit for this article
	pSocket->Send("X-Newsreader: Internet Navigator 2.0 for Windows\r\n");

	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	pSocket->Send("\r\n");
}		

void CArticleComposeView::OnUpdateArticleSend(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pDoc->CanPost() && m_bOriginalMaterial);
	return;
}

void CArticleComposeView::OnNewsgroupsSetup()
{
	m_pDoc->OnNewsgroupsSetup();
}
