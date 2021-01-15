// gopher.cpp - contains classes to implement the Internet Gopher protocol,
// named after Minnesota's own Golden Gophers!

#include "stdafx.h"
#include "inetnav.h"

#include "gopher.h"
#include "propshts.h" 
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGopherDoc                      

IMPLEMENT_DYNCREATE(CGopherDoc, CDocument)

CGopherDoc::CGopherDoc()
{                                                  
	m_pApp = (CInternetNavApp*)AfxGetApp();
	
	m_bTopLevelMenu = FALSE;
	m_bNewMenu = FALSE;
}

BOOL CGopherDoc::OnNewDocument()
{ 
	m_bNewMenu = TRUE;
	
	if (m_pApp->IsTopLevelMenu())   
	{
		m_bTopLevelMenu = TRUE;
		m_pApp->m_bTopLevelMenu = FALSE;
	}
	
	if (!m_pApp->IsOnline())
	{
		AfxMessageBox(IDP_INETNAV_SIGN_ON, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	
	if (!CDocument::OnNewDocument())
		return FALSE;

	CGopherConnect theDialog(m_pApp->GetMainFrame());
	if (theDialog.DoModal() == IDOK)
	{
		if (theDialog.m_strSiteAddress.IsEmpty())
		{
			AfxMessageBox("Please completely fill out the Connect to Gopher Site dialog box.");
			return FALSE;
		}
	
		m_strSiteAddress = theDialog.m_strSiteAddress;
		m_strSelector = theDialog.m_strSelector;
		m_strMenuName = theDialog.m_strMenuTitle;
		m_nPortNum = theDialog.m_nPortNum;
		m_bTopLevelMenu = m_bNewMenu;		
	}
	else
		return FALSE;
		
	return TRUE;
}

BOOL CGopherDoc::OnOpenDocument(const char* pszSelector)
{      
	m_bNewMenu = FALSE;
	
	if (m_pApp->IsTopLevelMenu())
	{
		m_bTopLevelMenu = TRUE;
		m_pApp->m_bTopLevelMenu = FALSE;
	}
	
	if (!m_pApp->IsOnline())
	{
		AfxMessageBox(IDP_INETNAV_SIGN_ON, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	
	if (pszSelector)
		return ProcessSelector(pszSelector, m_pApp->IsGopherSearch());
	
	return FALSE;
}

void CGopherDoc::OnCloseDocument()
{
	m_strQuery.Empty();
	m_pApp->SetGopherSearch(FALSE);
	CDocument::OnCloseDocument();
}

CGopherDoc::~CGopherDoc()
{                                         
	m_pApp = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// File/Path commands

void CGopherDoc::SetPathName(const char* pszPathName, BOOL bAddToMRU)
{
	m_strPathName = pszPathName;
	ASSERT(!m_strPathName.IsEmpty());       // must be set to something

	// Set the document title based on menu name
	SetTitle(m_strMenuName);
	
	if (bAddToMRU)
	{
		// don't do anything -- the MRU list entry for a Gopher menu listing
		// would look too weird.
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGopherDoc selector helper

BOOL CGopherDoc::ProcessSelector(const char* pszSelector, BOOL bSearch)
{
	CWaitCursor wait;
	
	char* s = (char*)pszSelector;
	
	if (!bSearch)
	{
		m_strMenuName = strtok(&s[1], "\t");	// title of menu
		m_strSelector = strtok(NULL, "\t");		// selector for menu
		m_strSiteAddress = strtok(NULL, "\t");  // address of server 
		m_nPortNum = atoi(strtok(NULL, "\t"));  // port number
		
		return TRUE;
	}
	
	// If we made it this far, this is a search
	m_strMenuName = "Search Results";
		
	if (!m_pApp->GetSearchSelector().IsEmpty())
	m_strSelector = m_pApp->GetSearchSelector();
	else
	m_strSelector = "";
		
	if (!m_pApp->GetSearchServer().IsEmpty())
	m_strSiteAddress = m_pApp->GetSearchServer();
	else
	return FALSE;
		
	if (!m_pApp->GetSearchQuery().IsEmpty())
	m_strQuery = m_pApp->GetSearchQuery();
	else
	return FALSE;
		
	m_nPortNum = m_pApp->GetSearchPort();
	
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGopherDoc, CDocument)
	//{{AFX_MSG_MAP(CGopherDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_GOPHER_SITE_PROPERTIES, OnGopherSiteProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGopherDoc serialization

void CGopherDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// No storing code -- this is a file-less area
	}
	else
	{
		// No loading code -- this is a file-less area
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGopherDoc commands

void CGopherDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// Disable this menu item; we don't need to save Gopher menu lists
	pCmdUI->Enable(FALSE);
}

void CGopherDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	// Disable this menu item; we don't need to save Gopher menu lists
	pCmdUI->Enable(FALSE);
} 

void CGopherDoc::OnGopherSiteProperties()
{
	CInetnavStatusBar* pBar = m_pApp->GetMainFrame()->GetStatusBar();
	
	// Show the Gopher Site Properties dialog box.
	// First, fill the dialog box with the information we have on the current
	// site.
	pBar->SetText("Please wait a few seconds while properties are loaded...");
	CGopherSiteProperties theDialog(AfxGetMainWnd());
	theDialog.m_strServer = m_strSiteAddress;
	theDialog.m_strName = m_strMenuName;
	theDialog.m_strPath = m_strSelector;
	theDialog.m_nPort = m_nPortNum; 
	pBar->ShowIdleMessage();
	
	theDialog.DoModal();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CGopherMenu Gopher menu listing

IMPLEMENT_DYNCREATE(CGopherMenu, CFormView)

BEGIN_MESSAGE_MAP(CGopherMenu, CFormView)
	//{{AFX_MSG_MAP(CGopherMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, OnUpdateFilePrintSetup)
	ON_BN_CLICKED(IDC_GOPHER, OnView)
	ON_BN_CLICKED(IDC_HELP, OnClickedHelp)
	ON_LBN_DBLCLK(IDC_MENU_LIST, OnDblclkMenuList)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_PAGE_SETUP, OnUpdateFilePageSetup)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	ON_LBN_SELCHANGE(IDC_MENU_LIST, OnSelchangeMenuList)
	ON_UPDATE_COMMAND_UI(ID_GOPHER_SEARCH, OnUpdateGopherSearch)
	ON_COMMAND(ID_CANCEL_CALL, OnCancelCall)
	ON_COMMAND(ID_GOPHER_SEARCH, OnSearch)
	ON_COMMAND(ID_GOPHER_VIEW, OnView)
	ON_COMMAND(ID_GOPHER_HELP, OnClickedHelp)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GOPHER_GET_MENU, OnGetGopherMenu)
END_MESSAGE_MAP()

CGopherMenu::CGopherMenu()
	: CFormView(CGopherMenu::IDD)
{
	//{{AFX_DATA_INIT(CGopherMenu)
	m_strSelection = "";
	//}}AFX_DATA_INIT
	m_pApp = (CInternetNavApp*)AfxGetApp(); 
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_pSocket = NULL;
}

CGopherMenu::~CGopherMenu()
{
	delete m_pSocket;
	m_pSocket = NULL;
	m_pStatusBar = NULL;
	m_pApp = NULL;
}

void CGopherMenu::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGopherMenu)
	DDX_Control(pDX, IDC_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_GOPHER, m_btnView);
	DDX_Control(pDX, IDC_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_MENU_LIST, m_menuList);
	DDX_LBString(pDX, IDC_MENU_LIST, m_strSelection);     
	DDX_Control(pDX, ID_FILE_CLOSE, m_btnBack);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CGopherMenu update handlers

void CGopherMenu::OnInitialUpdate()
{ 
	// load bitmaps for window buttons	
	m_btnView.AutoLoad(IDC_GOPHER, this);
	
	m_btnSearch.AutoLoad(IDC_SEARCH, this);
	
	m_btnHelp.AutoLoad(IDC_HELP, this);
	
	m_btnBack.AutoLoad(ID_FILE_CLOSE, this);
	
	CFormView::OnInitialUpdate(); 
	
	// size buttons to match the sizes of their bitmaps
	m_btnView.SizeToContent();
	m_btnSearch.SizeToContent();
	m_btnHelp.SizeToContent();                                  
	m_btnBack.SizeToContent();
	
	CGopherDoc* pDoc = (CGopherDoc*)GetDocument();
	
	// Dump information for this menu to the debug terminal
	#ifdef _DEBUG
		Dump(afxDump);
	#endif
	
	pDoc->SetModifiedFlag(FALSE);
	CString strText = "", strSiteAddr = "";
	strSiteAddr = pDoc->m_strSiteAddress;
	
	strText = "Retreiving information from ";
	strText += strSiteAddr;                  
	strText += ".  Please wait.";
	
	m_pStatusBar->SetText(strText);
	
	pDoc->SetTitle(pDoc->m_strMenuName);
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	if (!pDoc->m_strSiteAddress.IsEmpty())
	{
		if (m_pSocket) delete m_pSocket;
		m_pSocket = new QSocket(TRUE);
		m_pStatusBar->SetText("Connecting to Gopher site...");
		if (!m_pSocket->Connect(pDoc->m_strSiteAddress, pDoc->m_nPortNum))
		{
			m_pStatusBar->ShowIdleMessage();
			AfxMessageBox(IDP_NOCONNECT_HOST);
			delete m_pSocket;
			m_pSocket = NULL;  
			m_pStatusBar->ShowIdleMessage();
			return;
		}
		else
		{
			AfxGetApp()->BeginWaitCursor();
			PostMessage(WM_GOPHER_GET_MENU);
			AfxGetApp()->RestoreWaitCursor();
		}
	}
	else
	{
		AfxMessageBox("Please completely fill out the Connect to Gopher Site dialog box.");
		return;
	}

	m_btnView.EnableWindow(FALSE);
	m_btnSearch.EnableWindow(FALSE);
	m_btnHelp.EnableWindow(TRUE);     
	m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
	
	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CGopherMenu diagnostics

#ifdef _DEBUG
CGopherDoc* CGopherMenu::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGopherDoc)));
	return (CGopherDoc*)m_pDocument;
}           
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGopherMenu communications

LRESULT CGopherMenu::OnGetGopherMenu(WPARAM wParam, LPARAM lParam)
{
	LONG lCount = 0;
	// Wait until we're connected
	for (;;)
	{
		if (m_pSocket->IsConnected())
		{         
			CString strText = "Connected to ";
			strText += GetDocument()->m_strSiteAddress;
			strText += ".  Now receiving menu.";
			m_pStatusBar->SetText(strText);    
			/*wait.Restore();*/              
			m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
			break;
		}
		else if (!m_pSocket->IsConnecting())
		{
			MessageBeep(-1);
			AfxMessageBox(IDP_NOCONNECT_HOST); 
			
			delete m_pSocket;
			m_pSocket = NULL;
			
			m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
			
			return NULL;
		}
		else // m_pSocket->IsConnecting()
		{   
			if (!m_pApp->IsGopherSearch())
			{
				m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
			}
			else if (m_pApp->IsGopherSearch())
			{
				CString strText = "Searching for ";
				strText += m_pApp->GetSearchQuery() + "...   Please wait.";
				m_pStatusBar->SetText(strText);
			}
			m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
			
			// Not connected yet, so process events to allow the socket
			// events to occur 
			
			// Set up a message pump so all Windows messages continue to
			// be sent so we don't hang up the user's system
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
	
	BOOL bSearch = m_pApp->IsGopherSearch();
	// Send the Gopher menu request -- if the document's search query string
	// is empty then don't ask for search results from the server; but, if
	// we do have a query string send it to the server
	// as "<selector>\t<query string>\t";
	if (!bSearch) 
	{ 
		m_pStatusBar->SetText("Asking server for menu...  Please wait...");
		m_pSocket->Send(GetDocument()->m_strSelector + "\r\n"); 
		m_pStatusBar->SetText("Receiving menu items...  Please wait."); 
	}
	else
	{   
		m_pApp->SetGopherSearch(FALSE);
		CString strQuery = GetDocument()->m_strQuery;
		GetDocument()->m_strQuery.Empty();    
		CString strSend = "";
		if (GetDocument()->m_strSelector.IsEmpty())
		{ 
			// Veronica search
			strSend = strQuery + "\r\n";
		}
		else
		{
			strSend = GetDocument()->m_strSelector; 
			strSend += "\t";
			strSend += strQuery;
			strSend += "\r\n";
		}
		CString strText = "Searching for " + m_pApp->GetSearchQuery(); 
		strText += "...   Please wait.";
		m_pStatusBar->SetText(strText);
		m_pSocket->Send(strSend);   
		m_pStatusBar->SetText("Receiving search results... Please wait.");
	}

	for (;;)
	{ 
		if (m_pApp->IsGopherSearch())
			m_pStatusBar->SetText("Receiving search results...  Please wait.");
		else
			m_pStatusBar->SetText("Receiving Gopher menu...  Please wait.");
			
		CString str = m_pSocket->GetLine();
		CString strTitle = "";
		if (str.IsEmpty())
		{
			m_pStatusBar->ShowIdleMessage();
			break;
		}
		
		if (str[0] == '.' && str[1] == '\r')
		{
			continue;
		}
		
		// Get title to appear in the menu and assign the selector to the
		// listbox item
		char* pszString = new char[str.GetLength() + 1];
		strcpy(pszString, str);
		
		// don't put a "NULL pointer" or "-1==" entry in the listbox --
		// it looks weird
		char* pszTitle = strtok(&pszString[1], "\t");   
		strTitle = pszTitle;
		if ((strstr(pszTitle, "-1") != "-1") || 
			(strstr(pszTitle, "(null pointer)") != "(null pointer)") ||
				(strstr(pszTitle, "NULL") != "NULL") ||
			(strstr(pszTitle, "Null") != "Null") ||
			(strstr(pszTitle, "NULL POINTER") != "NULL POINTER") ||
			(strstr(pszTitle, "(NULL POINTER)") != "(NULL POINTER)")
			|| strstr(pszTitle, "(NULL)") != "(NULL)")
		{
			int nItem = m_menuList.AddString(pszTitle);
			delete pszString;
			m_menuList.SetItemDataPtr(nItem, strdup(str)); 
		}   
		else
			continue;
	}
	UpdateData(TRUE);

	// select first list item
	if (m_menuList.GetCount() != 0)  
	{
		m_menuList.SetCurSel(0);
		OnSelchangeMenuList();
		m_pStatusBar->ShowIdleMessage(); 
		return 0;
	}
	else
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_GOPHER_FAILED_FIND_INFO, 
			MB_ICONASTERISK|MB_OK, NULL);
		return 0;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CGopherMenu diagnostics

#ifdef _DEBUG
void CGopherMenu::AssertValid() const
{
	CFormView::AssertValid();
}

void CGopherMenu::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGopherMenu message handlers

void CGopherMenu::OnUpdateFilePrint(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CGopherMenu::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CGopherMenu::OnUpdateFilePageSetup(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CGopherMenu::OnUpdateFilePrintSetup(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CGopherMenu::OnView()
{
	OnDblclkMenuList();
}

void CGopherMenu::OnClickedHelp()
{
	CWnd::OnHelp();
	return;
}

void CGopherMenu::OnDblclkMenuList()
{
	char* s = (char*)m_menuList.GetItemDataPtr(m_menuList.GetCurSel());
	
	if (s == NULL) 
	{
		MessageBeep(-1);
		AfxMessageBox("Unable to determine destination of Gopher link.",
						MB_ICONSTOP|MB_OK);
		return;                                                          
	}
	
	if (strstr(s, "\t") == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox("Unable to determine destination of Gopher link.");
		return;
	}
		
	if (s == (char*)-1)
	{
		MessageBeep(-1);
		AfxMessageBox("Invalid Gopher link.", MB_ICONSTOP|MB_OK);
		return;
	}
    
    CString strMessage = "";
	switch (*s)
	{
		case '0':       // link to a document
			m_pApp->OpenGopherDocument(s);
			break;
		
		case '1':       // link to another menu
			m_pApp->OpenGopherMenu(s);
			break;
		
		case '3':		// error message
			strMessage = strtok(&s[1], "\t");
			MessageBeep(-1);
			AfxMessageBox(strMessage, MB_ICONEXCLAMATION);
			break;
			
		case '7':       // search item
			m_pApp->OpenGopherSearch(s); 
			
		default:   // unsupported link type     
			MessageBeep(-1);
			AfxMessageBox(IDP_GOPHER_NODOC, MB_ICONASTERISK|MB_OK);
			break;
	}
}

void CGopherMenu::OnSearch()
{
	// the handler function for a mouse double-click in the menu listbox
	// handles opening a search
	OnDblclkMenuList();
}

void CGopherMenu::OnClose()
{
	// If the socket is trying to connect to a server, cancel
	if (m_pSocket->IsConnecting())
	{
		delete m_pSocket;
		m_pSocket = NULL;       
		m_pStatusBar->ShowIdleMessage();
	}
	
	// Free the memory we've allocated for each menu item  
	if (!(m_menuList.GetCount() == 0))
	{
		void* pVoid;
			for (int i = 0; i < m_menuList.GetCount(); i++)
				if ((pVoid = m_menuList.GetItemDataPtr(i)) != (void*)-1)
					free(pVoid);
		CFormView::OnClose();
		return;
	}
	
	// If we made it this far, just call the default function
	// CFormView::OnClose();
	CFormView::OnClose();
	return;
}

void CGopherMenu::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strGopherSect = "Gopher";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strGopherSect, "GopherLeft", rect.left);
	pApp->WriteProfileInt(strGopherSect, "GopherRight", rect.right);
	pApp->WriteProfileInt(strGopherSect, "GopherTop", rect.top);
	pApp->WriteProfileInt(strGopherSect, "GopherBottom", rect.bottom);
	pApp->WriteProfileInt(strGopherSect, "GopherCX", rect.Width());
	pApp->WriteProfileInt(strGopherSect, "GopherCY", rect.Height());        
}

void CGopherMenu::OnSelchangeMenuList()
{
	char* s = (char*)m_menuList.GetItemDataPtr(m_menuList.GetCurSel());
	
	if (s == NULL)
		return;
		
	if (s == (char*)-1)
		return;
	
	// Check the type of gopher item
	switch (*s)
	{
		case '0':       // link to a document
			m_btnView.EnableWindow(TRUE);
			m_btnSearch.EnableWindow(FALSE);
			m_btnHelp.EnableWindow(TRUE);
			m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
			break;
		
		case '1':       // link to another menu          
			m_btnView.EnableWindow(TRUE);
			m_btnSearch.EnableWindow(FALSE);
			m_btnHelp.EnableWindow(TRUE);
			m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
			break;
		
		case '3':		// error message
			m_btnView.EnableWindow(TRUE);
			m_btnSearch.EnableWindow(FALSE);
			m_btnHelp.EnableWindow(TRUE);
			m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
			break;
					
		case '7':       // search item
			m_btnView.EnableWindow(FALSE);
			m_btnSearch.EnableWindow(TRUE);
			m_btnHelp.EnableWindow(TRUE);
			m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
			break;
		
		default:        // unsupported link type
			m_btnView.EnableWindow(FALSE);
			m_btnSearch.EnableWindow(FALSE);
			m_btnHelp.EnableWindow(TRUE);
			m_btnBack.EnableWindow(!GetDocument()->IsTopLevelMenu());
			break;
	}
}

void CGopherMenu::OnUpdateGopherSearch(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnSearch.IsWindowEnabled());
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CGopherListBox list box

void CGopherListBox::DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap, 
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

void CGopherListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	COLORREF clrText, clrBk, clrOldText, clrOldBk;
	int nMode = 0;
	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	// Draw a focus rectangle if nothing is selected and we have the focus
	if (lpDrawItemStruct->itemID == -1)
	{
		// draw the focus rect at the top, though
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		return;   
	}
	
	// Initialize colors		
	if (lpDrawItemStruct->itemAction & ODA_SELECT)	// sel status changed 
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED) // item is selected
		{
			// The current item is selected -- get the system's color
			// for highlighting and text
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		} 
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
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
		lpDrawItemStruct->itemAction |= ODA_DRAWENTIRE;
	}
	else if (lpDrawItemStruct->itemAction & ODA_FOCUS)
	{   
		// Current item may or may not have input focus
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// The current item is selected -- get the system's color
			// for highlighting and text
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
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
		lpDrawItemStruct->itemAction |= ODA_DRAWENTIRE;
	}				
	else
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// highlight and highlighttext colors
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
			
		clrText = ::GetSysColor(COLOR_WINDOWTEXT);
		clrBk = ::GetSysColor(COLOR_WINDOW);
	}

	// Get the first field of the item selector to draw to determine whether
	// to draw a FOLDER bitmap (for a gopher directory), a FILE bitmap
	// (for a gopher document), a BLANK FILE (for a unsupported gopher
	// document type), a MAGNIFYING GLASS AND FOLDER icon (for a
	// Gopher Index-Search server link), or a ERROR EXCLAMATION POINT icon
	// (for a Gopher error message).
	
	CBitmap bmIcon;
	CString strItem = "";
	char* s = (char*)GetItemDataPtr(lpDrawItemStruct->itemID);
	
	if (s == NULL)
		return;
		
	if (s == (char*)-1)
		return;
		
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) // draw whole control
	{
		switch (*s)
		{	
			case '0': // Gopher document
				// draw the file icon
				bmIcon.LoadBitmap(IDB_FILE); 
				break;
				
			case '1': // Gopher directory
				// draw the folder icon 
				if ((lpDrawItemStruct->itemAction & ODA_SELECT)
					&& (lpDrawItemStruct->itemState & ODS_SELECTED))
				{
					bmIcon.LoadBitmap(IDB_OPEN_FOLDER);
				}
				else if ((lpDrawItemStruct->itemAction & ODA_FOCUS)
						&& (lpDrawItemStruct->itemState & ODS_SELECTED))
				{
					bmIcon.LoadBitmap(IDB_OPEN_FOLDER);
				}
				else
				{
					bmIcon.LoadBitmap(IDB_FOLDER);
				}
				break;
			
			case '3': // Gopher error message
				// draw the little exclamation-point icon; the same one
				// Windows uses for error message boxes
				bmIcon.LoadBitmap(IDB_ERROR_ALERT);
					
			case '7': // Gopher Index-Search Server      
				// draw the hand-lens-over-folder icon
				bmIcon.LoadBitmap(IDB_SEARCH);
				break;   
				
			default: // unsupported gopher link type
				// draw the blank file icon
				bmIcon.LoadBitmap(IDB_BLANK_FILE);
		}
		
		// Initialize text and background colors, and the background mode
		clrOldText = pDC->SetTextColor(clrText);
		clrOldBk = pDC->SetBkColor(clrBk);
		
		nMode = pDC->SetBkMode(TRANSPARENT);
		
		// Fill item rect with highlight (or non-highlight) color
		CBrush br(clrBk);
		pDC->FillRect(&lpDrawItemStruct->rcItem, &br);   
        
        // Now draw the item bitmap transparent show the highlight looks right
        CBitmap* pBitmap = &bmIcon;
		BITMAP bitmap;
		DrawTransparentBitmap(pDC, pBitmap, bitmap,
			lpDrawItemStruct->rcItem.left + 5,
			lpDrawItemStruct->rcItem.top, RGB(0,128,128));
		
		// Now draw the text associated with the item 	
		GetText(lpDrawItemStruct->itemID, strItem);
		
		pDC->TextOut(lpDrawItemStruct->rcItem.left + bitmap.bmWidth + 10,
					lpDrawItemStruct->rcItem.top, strItem);  
		
		// Restore the device context's colors and modes to their 
		// previous values			
		pDC->SetTextColor(clrOldText);
		pDC->SetBkColor(clrOldBk);
		
		pDC->SetBkMode(nMode);
			
		if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
				(lpDrawItemStruct->itemState & ODS_SELECTED))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);    
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
					(lpDrawItemStruct->itemState & ODS_SELECTED))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		else if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
					(lpDrawItemStruct->itemState & ODS_FOCUS))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
					(lpDrawItemStruct->itemState & ODS_FOCUS))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
		
		return;     
	}         
		
	if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);    
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
				(lpDrawItemStruct->itemState & ODS_SELECTED))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	else if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
				(lpDrawItemStruct->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
				(lpDrawItemStruct->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
}

void CGopherListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CBitmap bmIcon;

	bmIcon.LoadBitmap(IDB_FOLDER);
	BITMAP bitmap;
	bmIcon.GetObject(sizeof(BITMAP), &bitmap);
	
	lpMeasureItemStruct->itemHeight = bitmap.bmHeight;
	return;
}

void CGopherListBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (GetCount() > 0)
	{
		// Show the CGopherSite properties dialog filled in with the
		// properties of the selected item
		if (GetCurSel() >= 0)
		{
			int nIndex = GetCurSel();
			char* s = 
				(char*)GetItemDataPtr(nIndex);	
			
			if (s == NULL)
				return;
		
			if (s == (char*)-1)
				return; 
				
				CGopherSiteProperties theDialog(AfxGetMainWnd());
				
				theDialog.m_strName = strtok(&s[1], "\t");
				theDialog.m_strPath = strtok(NULL, "\t");
				theDialog.m_strServer = strtok(NULL, "\t");
				theDialog.m_nPort = atoi(strtok(NULL, "\t"));
				
				theDialog.DoModal();
		}	
	}
}

BEGIN_MESSAGE_MAP(CGopherListBox, CListBox)
	//{{AFX_MSG_MAP(CGopherListBox)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CGopherMenu::OnCancelCall()
{                       
	// The user can use this function to cancel a connection attempt
	// which is taking too long
	if (m_pSocket->IsConnecting())
	{
		AfxMessageBox("Connection attempt cancelled.", MB_ICONASTERISK);
		delete m_pSocket;
		m_pSocket = NULL;       
		m_pStatusBar->ShowIdleMessage();
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CGopherWnd

IMPLEMENT_DYNCREATE(CGopherWnd, CMDIChildWnd)

CGopherWnd::CGopherWnd()
{
}

CGopherWnd::~CGopherWnd()
{
}


BEGIN_MESSAGE_MAP(CGopherWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CGopherWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGopherWnd message handlers

BOOL CGopherWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 80;
	windowRect.right = 562;
	windowRect.top = 10;
	windowRect.bottom = 383;
	 
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

/////////////////////////////////////////////////////////////////////////////
// COutputDoc

IMPLEMENT_DYNCREATE(COutputDoc, CDocument)

BEGIN_MESSAGE_MAP(COutputDoc, CDocument)
	//{{AFX_MSG_MAP(COutputDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputDoc construction/destruction

COutputDoc::COutputDoc()
{
	// TODO: add one-time construction code here
}

COutputDoc::~COutputDoc()
{
}

BOOL COutputDoc::OnNewDocument()
{  
	SetModifiedFlag(FALSE);
	CDocument::OnNewDocument();
	SetPathName(NULL);
	return FALSE;
}

BOOL COutputDoc::OnOpenDocument(const char* pszSelector)
{
	SetModifiedFlag(FALSE);
	char* pszStr = (char*)pszSelector;
	if (pszStr == NULL)
		return FALSE;
	else
	{
		m_strDocTitle = strtok(&pszStr[1], "\t");	// doc title
		m_strDocSelector = strtok(NULL, "\t");		// doc selector
		m_strDocHost = strtok(NULL, "\t");			// doc host
		m_nDocPort = atoi(strtok(NULL, "\t"));		// doc host's port 
		m_strPathName.Empty();
	}
	return TRUE;
}

BOOL COutputDoc::OnSaveDocument(const char* pszPathName)
{
	// call base-class version in order to save document, then
	// restore the document's title so the filename is not displayed in
	// the title bar of the document window, rather, the document's
	// title is displayed.
	
	if (!CDocument::OnSaveDocument(pszPathName))
		return FALSE;
	
	// if CDocument::OnSaveDocument() returned TRUE, set this document's
	// title equal to m_strDocTitle
	SetTitle(m_strDocTitle);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// File/Path commands

void COutputDoc::SetPathName(const char* pszPathName, BOOL bAddToMRU)
{
	m_strPathName = pszPathName;
	ASSERT(!m_strPathName.IsEmpty());       // must be set to something

	// Set the document title based on menu name
	SetTitle(m_strDocTitle);
	
	if (bAddToMRU)
	{
		// don't do anything -- the MRU list entry for a Gopher menu listing
		// or document would look too weird.
	}
}

/////////////////////////////////////////////////////////////////////////////	
BOOL COutputDoc::SaveModified()
{
	if (!IsModified())
		return TRUE;        // ok to continue

	CString strName = GetTitle();

	CString prompt = "";
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, strName);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (_access(m_strPathName, 6) != 0)
		{
			if (!CDocument::DoSave(NULL))
				return FALSE;   // don't continue
		}
		else
		{
			if (!CDocument::DoSave(m_strPathName))
				return FALSE;   // don't continue
		}
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}

/////////////////////////////////////////////////////////////////////////////
// COutputDoc serialization

void COutputDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}    
	
	// Serialize the text this document contains to and from disk
	// with the help of the view attached to this document
	
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL) return;
	
	while (pos != NULL)
	{
		// tell each view to serialize (load or save) its contents to or
		// from disk
		CEditView* pEditView = (CEditView*)GetNextView(pos);
		pEditView->SerializeRaw(ar);
	}
	
	return;	
}

void COutputDoc::DeleteContents()
{
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL) return;
	
	while (pos != NULL)
	{
		CEditView* pEditView = 
			(CEditView*)GetNextView(pos);
		pEditView->DeleteContents();
	}
}

/////////////////////////////////////////////////////////////////////////////
// COutputDoc diagnostics

#ifdef _DEBUG
void COutputDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COutputDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COutputDoc commands

void COutputDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// Only enable the File, Save menu item if the document data has
	// changed
	pCmdUI->Enable(IsModified());
	return;
}

/////////////////////////////////////////////////////////////////////////////
// COutputView

IMPLEMENT_DYNCREATE(COutputView, CEditView)

BEGIN_MESSAGE_MAP(COutputView, CEditView)
	//{{AFX_MSG_MAP(COutputView)
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
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Socket response
	ON_MESSAGE(WM_SOCKET_RESPONSE, OnSocketResponse)
END_MESSAGE_MAP()

UINT COutputView::m_nDefTabStops;
UINT COutputView::m_nDefTabStopsOld;
BOOL COutputView::m_bDefWordWrap;
BOOL COutputView::m_bDefWordWrapOld;
LOGFONT NEAR COutputView::m_lfDefFont;
LOGFONT NEAR COutputView::m_lfDefFontOld;
LOGFONT NEAR COutputView::m_lfDefPrintFont;
LOGFONT NEAR COutputView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

static char BASED_CODE szSettings[] = "Gopher Document: Settings";
static char BASED_CODE szTabStops[] = "TabStops";
static char BASED_CODE szFont[] = "Gopher Document: Font";
static char BASED_CODE szPrintFont[] = "GopherDocument: PrintFont";
static char BASED_CODE szHeight[] = "Height";
static char BASED_CODE szWeight[] = "Weight";
static char BASED_CODE szItalic[] = "Italic";
static char BASED_CODE szUnderline[] = "Underline";
static char BASED_CODE szPitchAndFamily[] = "PitchAndFamily";
static char BASED_CODE szFaceName[] = "FaceName";
static char BASED_CODE szSystem[] = "System";
static char BASED_CODE szWordWrap[] = "WordWrap";

static void GetProfileFont(LPCSTR szSec, LOGFONT* plf)
{
	CWinApp* pApp = AfxGetApp();
	plf->lfHeight = pApp->GetProfileInt(szSec, szHeight, 0);
	if (plf->lfHeight != 0)
	{
		plf->lfWeight = pApp->GetProfileInt(szSec, szWeight, 0);
		plf->lfItalic = (BYTE)pApp->GetProfileInt(szSec, szItalic, 0);
		plf->lfUnderline = (BYTE)pApp->GetProfileInt(szSec, szUnderline, 0);
		plf->lfPitchAndFamily = (BYTE)pApp->GetProfileInt(szSec, szPitchAndFamily, 0);
		CString strFont = pApp->GetProfileString(szSec, szFaceName, szSystem);
		strncpy((char*)plf->lfFaceName, strFont, sizeof plf->lfFaceName);
		plf->lfFaceName[sizeof plf->lfFaceName-1] = 0;
	}
}

static void WriteProfileFont(LPCSTR szSec, const LOGFONT* plf, LOGFONT* plfOld)
{
	CWinApp* pApp = AfxGetApp(); 
	if (plf->lfHeight != plfOld->lfHeight)
		pApp->WriteProfileInt(szSec, szHeight, plf->lfHeight);
	if (plf->lfHeight != 0)
	{
		if (plf->lfHeight != plfOld->lfHeight)
			pApp->WriteProfileInt(szSec, szHeight, plf->lfHeight);
		if (plf->lfWeight != plfOld->lfWeight)
			pApp->WriteProfileInt(szSec, szWeight, plf->lfWeight);
		if (plf->lfItalic != plfOld->lfItalic)
			pApp->WriteProfileInt(szSec, szItalic, plf->lfItalic);
		if (plf->lfUnderline != plfOld->lfUnderline)
			pApp->WriteProfileInt(szSec, szUnderline, plf->lfUnderline);
		if (plf->lfPitchAndFamily != plfOld->lfPitchAndFamily)
			pApp->WriteProfileInt(szSec, szPitchAndFamily, plf->lfPitchAndFamily);
		if (strcmp(plf->lfFaceName, plfOld->lfFaceName) != 0)
			pApp->WriteProfileString(szSec, szFaceName, (LPCSTR)plf->lfFaceName);
	}
	*plfOld = *plf;
}

void COutputView::Initialize()
{ 
	CWinApp* pApp = AfxGetApp();
	// Enable word wrap by default
	m_bDefWordWrap = pApp->GetProfileInt(szSettings, szWordWrap, TRUE);
	m_bDefWordWrapOld = m_bDefWordWrap;
	m_nDefTabStops = pApp->GetProfileInt(szSettings, szTabStops, 8*4);
	m_nDefTabStopsOld = m_nDefTabStops;
	GetProfileFont(szFont, &m_lfDefFont);
	m_lfDefFontOld = m_lfDefFont;
	GetProfileFont(szPrintFont, &m_lfDefPrintFont);
	m_lfDefPrintFontOld = m_lfDefPrintFont;       
	
}

void COutputView::Terminate()
{
	CWinApp* pApp = AfxGetApp();     
	// Save settings if they changed
	if (m_nDefTabStops != m_nDefTabStopsOld)
		pApp->WriteProfileInt(szSettings, szTabStops, m_nDefTabStops);
	if (m_bDefWordWrap != m_bDefWordWrapOld)
		pApp->WriteProfileInt(szSettings, szWordWrap, m_bDefWordWrap);
	WriteProfileFont(szFont, &m_lfDefFont, &m_lfDefFontOld);
	WriteProfileFont(szPrintFont, &m_lfDefPrintFont, &m_lfDefPrintFontOld);
	
}

/////////////////////////////////////////////////////////////////////////////
// COutputView update

void COutputView::OnInitialUpdate()
{
	Initialize();

	COutputDoc* pDoc = GetDocument();
	CDocTemplate* pTemplate = pDoc->GetDocTemplate();
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	pDoc->SetModifiedFlag(FALSE);
	
	m_pStatusBar->SetText("Retrieving Gopher information... Please wait...");
	pDoc->SetTitle(pDoc->m_strDocTitle);   
	
	if (!pApp->IsGopherShowSaveAs())
	{
		// try to make a filename for this document based on its title
		CString	newName = pDoc->m_strTitle;
		if (newName.GetLength() > 8)
			newName.ReleaseBuffer(8);
		int iBad = newName.FindOneOf(" #%;/\\");    // dubious filename
		if (iBad != -1)
			newName.ReleaseBuffer(iBad);
	
		// append the default suffix if there is one
		CString strExt = ".txt";
		if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
		  !strExt.IsEmpty())
		{
			ASSERT(strExt[0] == '.');
			newName += strExt;
		}
		
		if (pApp->GetGopherDocDir().IsEmpty())
			pDoc->SetPathName(newName);
		else
		{
			CString strPathName = pApp->GetGopherDocDir();
			strPathName += newName;
			pDoc->SetPathName(strPathName);
		}			
		pDoc->DoSave(pDoc->GetPathName(), TRUE);
	}
	else
	{        
		CString strFilter = "Text Files (*.txt)|*.txt||All Files (*.*";
		strFilter += ")|*.*||";
		// Show a 'Save As' dialog box, and let the user pick a filename
		CFileDialog dlg(FALSE, NULL, NULL, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_SHOWHELP|OFN_NOCHANGEDIR,
			 strFilter, NULL); 
		
		if (dlg.DoModal() == IDOK)
		{
			pDoc->SetPathName(dlg.GetPathName());
		}  
		return; 
	}      
	pDoc->SetTitle(pDoc->m_strDocTitle);
	
	m_pStatusBar->SetText("Contacting site...  Please wait.");
	if (!pDoc->m_strDocHost.IsEmpty())	
	{
		delete m_pSocket;
		m_pSocket = new QSocket(TRUE);
		m_pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);
		if (!m_pSocket->Connect(pDoc->m_strDocHost, pDoc->m_nDocPort))
		{
			m_pStatusBar->SetText("Connection attempt failed.");
			MessageBeep(-1);
			AfxMessageBox(IDP_NOCONNECT_HOST);
			delete m_pSocket;
			m_pSocket = NULL;
			m_pStatusBar->SetText(AFX_IDS_IDLEMESSAGE);
			pDoc->OnFileClose();
			return;
		}
	}
	else
	{
		CString strMsg = "The location of the document titled ";
		strMsg += "'" + pDoc->m_strDocTitle + "'";
		strMsg += " is unknown.  The Gopher server has not provided enough ";
		strMsg += "information on the document's location.";
		CString strCaption = "Location of Document Unknown";
		CWnd::MessageBox(strMsg, strCaption, MB_ICONASTERISK|MB_OK);               

		return;
	}
	
	CEditView::OnInitialUpdate();  
	pDoc->SetModifiedFlag(FALSE);               
	
	if (GetParentFrame()->IsWindowVisible())
	{
		GetParentFrame()->ShowWindow(SW_HIDE);
		GetParentFrame()->UpdateWindow();   
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// COutputView construction/destruction

COutputView::COutputView()
{
	// Load in settings from INI file
	Initialize();
	
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_hCursorOld = NULL;   
	m_pSocket = NULL;
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
}

BOOL COutputView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

COutputView::~COutputView()
{ 
	m_pApp = NULL; 
	m_hCursorOld = NULL;
	
	delete m_pSocket;
	
	m_pStatusBar = NULL;
	m_pSocket = NULL;
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// COutputView Word Wrap support

BOOL COutputView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL COutputView::SetWordWrap(BOOL bWordWrap)
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
// COutputView socket communications

LRESULT COutputView::OnSocketResponse(WPARAM amount, LPARAM buffer)
{
	COutputDoc* pDoc = GetDocument();
	
	if ((int)amount > 0)
	{ 
		m_pStatusBar->SetText("Receiving document text... Please wait...");
		// NULL-terminate the buffer
		((char*)buffer)[amount] = '\0';
		int nLength = GetEditCtrl().GetWindowTextLength();
		GetEditCtrl().SetSel(nLength, nLength, TRUE);
		GetEditCtrl().ReplaceSel((char*)buffer);	
		return 0;
	}
	else
	{
		// If amount < 0 it is a receive command    
		CString strText = "Connected to gopher server, now requesting ";
		strText += "document information.";
		switch ((SocketReceiveCmd)amount)
		{
			case SocketStatusChanged:
				switch (m_pSocket->GetStatus())
				{           
					case CONNECTING:
						break;
						
					case CONNECTED:
						// send the Gopher request  
						m_pStatusBar->SetText(strText);
						m_pSocket->Send(pDoc->m_strDocSelector + "\r\n");
						break;			
					
					case DISCONNECTED: 
						// connection complete
						strText = "Information request completed.";
						m_pStatusBar->SetText(strText);	 
						pDoc->DoSave(pDoc->GetPathName(), TRUE);
						pDoc->SetTitle(pDoc->m_strDocTitle);
						pDoc->SetModifiedFlag(FALSE);
						
						GetParentFrame()->ShowWindow(SW_SHOW);
						GetParentFrame()->UpdateWindow();
						break;
					
					default:
						// All other status states
						// represent an error
						break;
				}
		}
	}
	return 0;
}		 		
/////////////////////////////////////////////////////////////////////////////
// COutputView drawing

void COutputView::OnDraw(CDC* pDC)
{
	COutputDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// COutputView printing

BOOL COutputView::OnPreparePrinting(CPrintInfo* pInfo)
{  
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void COutputView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
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

void COutputView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// get string to show as "filename" in header/footer
	CPageSetupSheet shtPageSetup;
	CString strFileName = GetDocument()->GetPathName();
	if (strFileName.IsEmpty())
		strFileName = GetDocument()->GetTitle();

	// go thru CPageSetupSheet::CPageSetupPage to format the footer
	// we use the title of the Gopher document being displayed in this
	// view to form the header
	CString strHeader = "";
	strHeader = GetDocument()->GetTitle();
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

void COutputView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void COutputView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// COutputView diagnostics

#ifdef _DEBUG
void COutputView::AssertValid() const
{
	CEditView::AssertValid();
}

void COutputView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

COutputDoc* COutputView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COutputDoc)));
	return (COutputDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COutputView message handlers

static void ScaleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member from one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}
 
int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void COutputView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing commands
	CEditView::OnRButtonDown(nFlags, point);
}

void COutputView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void COutputView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void COutputView::OnWordWrap()
{
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void COutputView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void COutputView::OnChoosePrintFont()
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
	/*wait.Restore();*/
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

void COutputView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void COutputView::OnChooseFont()
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

void COutputView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());
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

void COutputView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void COutputView::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strGopherSect = "Gopher Document";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strGopherSect, "GopherLeft", rect.left);
	pApp->WriteProfileInt(strGopherSect, "GopherRight", rect.right);
	pApp->WriteProfileInt(strGopherSect, "GopherTop", rect.top);
	pApp->WriteProfileInt(strGopherSect, "GopherBottom", rect.bottom);
	pApp->WriteProfileInt(strGopherSect, "GopherCX", rect.Width());
	pApp->WriteProfileInt(strGopherSect, "GopherCY", rect.Height());
}

/////////////////////////////////////////////////////////////////////////////
// COutputWnd

IMPLEMENT_DYNCREATE(COutputWnd, CMDIChildWnd)

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}


BEGIN_MESSAGE_MAP(COutputWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(COutputWnd)
		ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COutputWnd message handlers

BOOL COutputWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 56;
	windowRect.right = 588;
	windowRect.top = 112;
	windowRect.bottom = 404;
	 
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

BOOL COutputWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_CHILD|WS_OVERLAPPEDWINDOW;
	
	// we have changed this window's styles so it will be invisible
	// at first -- this is OK because we only want the user to see this
	// window when the whole Gopher document has been downloaded by
	// COutputView
	
	// call the base-class version so this function will work
	return CMDIChildWnd::PreCreateWindow(cs);
}

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create and display the ruler along the top edge of the window
	if (!m_wndRulerBar.Create(this, IDD_RULERBAR,
							CBRS_TOP,
							AFX_IDW_CONTROLBAR_LAST))
	{
		AfxMessageBox("Failed to create ruler.");
		return -1;	// failed to create ruler                               
	}
	
	// Load bitmap buttons
	if (!m_btnRuler.AutoLoad(IDC_RULER, &m_wndRulerBar))
	{
		AfxMessageBox("Failed to create ruler.");
		return -1;  // failed to create ruler bitmap
	}
	
	return 0;
}

void COutputWnd::OnViewRuler()
{
	BOOL bToggleVisible = !m_wndRulerBar.IsWindowVisible();
	
	// If IsWindowVisible() == TRUE, we should hide the
	// ruler bar. If IsWindowVisible() == FALSE, then we
	// should show the ruler bar.
	m_wndRulerBar.ShowWindow((bToggleVisible)?SW_SHOW:SW_HIDE);
	RecalcLayout(); // adjust positions of various child windows
	return;
}

void COutputWnd::OnUpdateViewRuler(CCmdUI* pCmdUI)
{
	// If the ruler is visible, we want to put a check mark
	// next to the Ruler menu command.  
	// CDialogBar::IsWindowVisible() tells us whether or not
	// the ruler is visible
	BOOL bCheck = m_wndRulerBar.IsWindowVisible();
	pCmdUI->SetCheck(bCheck);
	return;
}		

/////////////////////////////////////////////////////////////////////////////
// CGopherSiteProperties dialog


CGopherSiteProperties::CGopherSiteProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CGopherSiteProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGopherSiteProperties)
	//}}AFX_DATA_INIT
}

void CGopherSiteProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGopherSiteProperties)
	DDX_Text(pDX, IDC_SITE_SERVER, m_strServer);
	DDX_Text(pDX, IDC_SITE_PATH, m_strPath);
	DDX_Text(pDX, IDC_SITE_NAME, m_strName);
	DDX_Text(pDX, IDC_SITE_PORT, m_nPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGopherSiteProperties, CDialog)
	//{{AFX_MSG_MAP(CGopherSiteProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGopherSiteProperties message handlers

BOOL CGopherSiteProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CGopherCenter -- this view encapsulates the Gopher Center area of
// Internet Navigator

IMPLEMENT_DYNCREATE(CGopherCenter, CFormView)

CGopherCenter::CGopherCenter()
	: CFormView(CGopherCenter::IDD)
{
	//{{AFX_DATA_INIT(CGopherCenter)
		m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT      
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
}

CGopherCenter::~CGopherCenter()
{
}

void CGopherCenter::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGopherCenter)
	DDX_Control(pDX, IDC_GOPHER_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_OPEN_SITE, m_btnOpenSite);
	DDX_Control(pDX, IDC_MESSAGE_BOARD, m_btnMsgBoard);
	DDX_Control(pDX, IDC_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_GOPHER_INFO, m_btnInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGopherCenter, CFormView)
	//{{AFX_MSG_MAP(CGopherCenter)
	ON_BN_CLICKED(IDC_GOPHER_INFO, OnInformationAboutGopher)
	ON_BN_CLICKED(IDC_OPEN_SITE, OnOpenSite)
	ON_BN_CLICKED(IDC_MESSAGE_BOARD, OnMessageBoard)
	ON_BN_CLICKED(IDC_HELP, OnClickedHelp)
	ON_BN_CLICKED(IDC_GOPHER_SEARCH, OnGopherSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGopherCenter diagnostics

#ifdef _DEBUG
void CGopherCenter::AssertValid() const
{
	CFormView::AssertValid();
}

void CGopherCenter::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGophCenterDoc* CGopherCenter::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGophCenterDoc)));
	return (CGophCenterDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGopherCenter update handlers

void CGopherCenter::OnInitialUpdate()
{
	// call base class version of this function first
	CFormView::OnInitialUpdate();

	// now do our work
	m_btnOpenSite.AutoLoad(IDC_OPEN_SITE, this);
	m_btnMsgBoard.AutoLoad(IDC_MESSAGE_BOARD, this);
	m_btnHelp.AutoLoad(IDC_HELP, this);
	m_btnInfo.AutoLoad(IDC_GOPHER_INFO, this);
	m_btnSearch.AutoLoad(IDC_GOPHER_SEARCH, this);

    if (m_pApp->IsUseCTL3D())  // be sure to give this form a 3D look!
    	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
    
    // Show and activate this window
	GetParentFrame()->ActivateFrame(SW_RESTORE);   
	GetParentFrame()->UpdateWindow();

	return;
}

/////////////////////////////////////////////////////////////////////////////
// CGopherCenter message handlers

void CGopherCenter::OnInformationAboutGopher()
{
	CWaitCursor wait;
	m_pStatusBar->SetText("Accessing information...  Please wait.");
	
	int nDocNum = 1;
	int nPortNum = 70;
	
	char szSelector[1024];
	sprintf(szSelector, "%i\tInformation About Gopher\t"
			"%i/Information About Gopher\tgopher.tc.umn.edu\t%i",
		nDocNum, nDocNum, nPortNum);
	
	CString strSelector = (char*)szSelector;
	
	m_pStatusBar->SetText("Opening Gopher menu...  Please wait.");
	m_pApp->m_bTopLevelMenu = TRUE;
	m_pApp->OpenGopherMenu(strSelector);	 
	return;
}

void CGopherCenter::OnOpenSite()
{	
	CWaitCursor wait;
	
	// Open a new Gopher menu and let the user fill in values
	m_pApp->OpenGopherMenu(NULL);     
	return;
}

void CGopherCenter::OnMessageBoard()
{    
	// This function opens an article selection window for the
	// 'comp.infosystems.gopher' newsgroup, which would be like a message
	// board for this area
	
	CWaitCursor wait;
	CMultiDocTemplate* pTempl = m_pApp->GetNewsgroup();
	
	CString strPath = m_pApp->GetProfileString("Internet Newsgroups", 
			"AreaFile");

	CNewsDoc* pDocument = (CNewsDoc*)pTempl->CreateNewDocument();
	if (pDocument == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CArticleSelectionWnd* pFrame = 
		(CArticleSelectionWnd*)pTempl->CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{     
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);    
		delete pDocument;       // explicit delete on error
		return;
	}
	ASSERT_VALID(pFrame);
		
	if (!strPath.IsEmpty())  
	{
		// open an existing document
		BeginWaitCursor();
		if (!pDocument->OnOpenDocument(strPath))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			pFrame->DestroyWindow();
			EndWaitCursor();
			return;
		}
		pDocument->SetPathName(strPath);
		EndWaitCursor();				
	}
	else
	{
		// The user needs to set up a mailbox if they already haven't
		// done so, in order to use it to compose a mail message.
		// We will tell the user this, and in so doing, also will
		// ask the user if they want to set up a new mailbox now.
		MessageBeep(-1);
		switch (AfxMessageBox(IDP_SETUP_NEWSGROUPS,
					MB_ICONQUESTION|MB_YESNOCANCEL))
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
				MessageBeep(-1);
				AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP);
				m_pStatusBar->ShowIdleMessage();
				return;
			
			case IDCANCEL:
				m_pStatusBar->ShowIdleMessage();
				return;
		}
	}
	
	m_pStatusBar->SetText("Opening comp.infosystems.gopher...  Please wait.");
	pFrame->m_strGroup = "comp.infosystems.gopher";
	
	pTempl->InitialUpdateFrame(pFrame, pDocument, TRUE);
}

void CGopherCenter::OnClickedHelp()
{
	CWaitCursor wait;
	CWnd::OnHelp();
	return;
} 

void CGopherCenter::OnGopherSearch()
{
	CWaitCursor wait;
	m_pStatusBar->SetText("Accessing information...  Please wait.");
	
	int nDocNum = 1;
	int nPortNum = 70;
	
	char szSelector[1024];
	sprintf(szSelector, "%i\tSearch Gopher With Veronica\t"
			"%i/Other Gopher and Information Servers/Veronica"
			"\tgopher.tc.umn.edu\t%i",
		nDocNum, nDocNum, nPortNum);
		
	
	CString strSelector = (char*)szSelector;
	
	m_pStatusBar->SetText("Opening Gopher menu...  Please wait.");
	m_pApp->m_bTopLevelMenu = TRUE;
	m_pApp->OpenGopherMenu(strSelector);	 
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CGophCenterWnd

IMPLEMENT_DYNCREATE(CGophCenterWnd, CMDIChildWnd)

CGophCenterWnd::CGophCenterWnd()
{
}

CGophCenterWnd::~CGophCenterWnd()
{
}


BEGIN_MESSAGE_MAP(CGophCenterWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CGophCenterWnd)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGophCenterWnd message handlers

BOOL CGophCenterWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
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

	CString strTitle = "Gopher Center";

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
    
    pApp->m_pWndGophCenter = this;
    
	return TRUE;
}

BOOL CGophCenterWnd::PreCreateWindow(CREATESTRUCT& cs)
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

void CGophCenterWnd::OnClose()
{
	// tell the application class we are no longer available -- set its
	// pointer to us to NULL
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	
	pApp->m_pWndGophCenter = NULL;
	
	CMDIChildWnd::OnClose();
}
/////////////////////////////////////////////////////////////////////////////
// CGophCenterDoc

IMPLEMENT_SERIAL(CGophCenterDoc, CDocument, 0 /* schema number*/ )

CGophCenterDoc::CGophCenterDoc()
{
}

BOOL CGophCenterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
		
	SetTitle("Gopher Center");
	SetModifiedFlag(FALSE);
	
	return TRUE;
}

CGophCenterDoc::~CGophCenterDoc()
{
}


BEGIN_MESSAGE_MAP(CGophCenterDoc, CDocument)
	//{{AFX_MSG_MAP(CGophCenterDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGophCenterDoc serialization

void CGophCenterDoc::Serialize(CArchive& ar)
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
// CGophCenterDoc::SetTitle -- keeps this window's title at "Gopher Center"

void CGophCenterDoc::SetTitle(const char* pszTitle)
{
	m_strTitle = "Gopher Center";
	UpdateFrameCounts();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CGophCenterDoc commands

void CGophCenterDoc::OnUpdateFileNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CGophCenterDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CGophCenterDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

