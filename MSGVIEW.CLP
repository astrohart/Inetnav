P�  �L  �   &Text ��$G�L  W% �L    W%�E�&��D�&GV%  �E  E0O      �D�G �L  �M  &OEM Text �$G�L   �L    �E�&��D�&G  �E  E0O      �D�G/////////////////////////////////////////////////////////////////////////////
// CMessageView -- displays electronic mail messages

IMPLEMENT_DYNCREATE(CMessageView, CEditView)

BEGIN_MESSAGE_MAP(CMessageView, CEditView)
	//{{AFX_MSG_MAP(CMessageView)
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
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

UINT CMessageView::m_nDefTabStops;
UINT CMessageView::m_nDefTabStopsOld;
BOOL CMessageView::m_bDefWordWrap;
BOOL CMessageView::m_bDefWordWrapOld;
LOGFONT NEAR CMessageView::m_lfDefFont;
LOGFONT NEAR CMessageView::m_lfDefFontOld;
LOGFONT NEAR CMessageView::m_lfDefPrintFont;
LOGFONT NEAR CMessageView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

// toolbar buttons array
static UINT BASED_CODE msgButtons[] =
{
	ID_NEW_TEXT,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_FILE_NEW,
	ID_PLACES_PREFERENCES,
	ID_PLACES_HELPINFO,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_EDIT_UNDO,
	ID_EDIT_REDO,
		ID_SEPARATOR,
	ID_PLACES_AOL,
	ID_PLACES_CIS,
	ID_PLACES_PRODIGY,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
	ID_PAGE_SETUP,
	ID_FILE_PRINT_SETUP,
		ID_SEPARATOR,
	ID_HELP_INDEX,
	ID_CONTEXT_HELP,
};

static char BASED_CODE szSettings[] = "E-Mail Message: Settings";
static char BASED_CODE szTabStops[] = "TabStops";
static char BASED_CODE szFont[] = "E-Mail Message: Font";
static char BASED_CODE szPrintFont[] = "E-Mail Message: PrintFont";
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

void CMessageView::Initialize()
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

void CMessageView::Terminate()
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
// CMessageView construction/destruction

CMessageView::CMessageView()
{
	// Load in settings from INI file
	Initialize();
	
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_bUndo = FALSE; // start out unable to undo or redo anything
}

BOOL CMessageView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

CMessageView::~CMessageView()
{ 
	m_pApp = NULL;
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView Word Wrap support

BOOL CMessageView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL CMessageView::SetWordWrap(BOOL bWordWrap)
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
// CMessageView drawing

void CMessageView::OnDraw(CDC* pDC)
{
	CTextDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView printing

BOOL CMessageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void CMessageView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
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
    
	CWaitCursor wait;
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
	pInfo->SetMaxPage(m_aPageStart.GetSize());
	m_nPreviewPage = nCurPage;
}

void CMessageView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// get string to show as "filename" in header/footer
	CPageSetupSheet shtPageSetup;
	CString strFileName = GetDocument()->GetPathName();
	if (strFileName.IsEmpty())
		strFileName = GetDocument()->GetTitle();

	// go thru CPageSetupSheet::CPageSetupPage to format the header 
	// and footer
	CString strHeader;
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.FormatHeader(strHeader, m_timeHeader, strFileName,
		pInfo->m_nCurPage);
	CString strFooter;        
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

void CMessageView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void CMessageView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView diagnostics

#ifdef _DEBUG
void CMessageView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMessageView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMailBoxDoc* CMessageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMailBoxDoc*)));
	return (CMailBoxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMessageView message handlers

static void ScaleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member from one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}
 
int CMessageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CMessageView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing commands
	CEditView::OnRButtonDown(nFlags, point);
}

void CMessageView::OnSize(UINT nType, int cx, int cy)
{
	CWaitCursor wait;
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void CMessageView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void CMessageView::OnWordWrap()
{
	CWaitCursor wait;
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void CMessageView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void CMessageView::OnChoosePrintFont()
{
	CWaitCursor wait;
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
	wait.Restore();
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

void CMessageView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void CMessageView::OnChooseFont()
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

void CMessageView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());      
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.m_bOutputView = FALSE;
	shtPageSetup.m_pgTabStops.m_nTabStops = m_nTabStops/4;
	if (shtPageSetup.DoModal() == IDOK)
	{
		// get values entered into this tabbed dialog box 
		CWaitCursor wait;
		shtPageSetup.m_pgHeaderFooter.Terminate();
		
		m_nHeaderTime = shtPageSetup.m_pgHeaderFooter.m_nHeaderTime;
		m_nFooterTime = shtPageSetup.m_pgHeaderFooter.m_nFooterTime;
		SetTabStops(shtPageSetup.m_pgTabStops.m_nTabStops*4);
		m_nDefTabStops = m_nTabStops;
     		
		return;
	}  
	return;
}

void CMessageView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void CMessageView::OnWindowRemember()
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

void CMessageView::OnEditUndo()
{
	CWaitCursor wait;
	CEditView::OnEditUndo();
	
	// change the "redo flag" to TRUE so the undone action can be redone
	m_bUndo = TRUE;
	return;
}

void CMessageView::OnEditRedo()
{
	CWaitCursor wait;
	if (m_bUndo)
	{
		CEditView::OnEditUndo();
		m_bUndo = FALSE;	// can't redo anymore -- not until another undo
	}
	return;	// we're done
}	

void CMessageView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bUndo && GetEditCtrl().CanUndo());
	return;
} 

void CMessageView::OnActivateView(BOOL bActivate, CView* pActivateView, 
									CView* pDeactiveView)
{
	// call base class version of this function first so we can
	// process this function properly
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	
	// change the main toolbar above so that it has editing, printing,
	// and saving commands which the user can select to use this view
	// and its attached document
	
	// First, get a pointer to the main window, courtesy of the application
	// class
	CMainFrame* pFrame = m_pApp->GetMainFrame();
	
	// now, if we are being activated, tell the toolbar (via the main window)
	// to change its buttons to those we specify.  If we are being
	// deactivated, tell the main window to reset the toolbar to the
	// Places buttons.
	
	if (bActivate)
	{
		pFrame->GetToolBar()->LoadBitmap(IDR_MESSAGETYPE);
		pFrame->GetToolBar()->SetButtons(msgButtons, 26);
		
	}
	else
		pFrame->ResetToolBar();
	
	// Force updating of the toolbar
	pFrame->GetToolBar()->Invalidate();
	pFrame->GetToolBar()->UpdateWindow();
	
	return;
}	

 ��v��x�F��^
&�G$&G&�^&;G/////////////////////////////////////////////////////////////////////////////
// CMessageView -- displays electronic mail messages

IMPLEMENT_DYNCREATE(CMessageView, CEditView)

BEGIN_MESSAGE_MAP(CMessageView, CEditView)
	//{{AFX_MSG_MAP(CMessageView)
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
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

UINT CMessageView::m_nDefTabStops;
UINT CMessageView::m_nDefTabStopsOld;
BOOL CMessageView::m_bDefWordWrap;
BOOL CMessageView::m_bDefWordWrapOld;
LOGFONT NEAR CMessageView::m_lfDefFont;
LOGFONT NEAR CMessageView::m_lfDefFontOld;
LOGFONT NEAR CMessageView::m_lfDefPrintFont;
LOGFONT NEAR CMessageView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

// toolbar buttons array
static UINT BASED_CODE msgButtons[] =
{
	ID_NEW_TEXT,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_FILE_NEW,
	ID_PLACES_PREFERENCES,
	ID_PLACES_HELPINFO,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_EDIT_UNDO,
	ID_EDIT_REDO,
		ID_SEPARATOR,
	ID_PLACES_AOL,
	ID_PLACES_CIS,
	ID_PLACES_PRODIGY,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
	ID_PAGE_SETUP,
	ID_FILE_PRINT_SETUP,
		ID_SEPARATOR,
	ID_HELP_INDEX,
	ID_CONTEXT_HELP,
};

static char BASED_CODE szSettings[] = "E-Mail Message: Settings";
static char BASED_CODE szTabStops[] = "TabStops";
static char BASED_CODE szFont[] = "E-Mail Message: Font";
static char BASED_CODE szPrintFont[] = "E-Mail Message: PrintFont";
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

void CMessageView::Initialize()
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

void CMessageView::Terminate()
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
// CMessageView construction/destruction

CMessageView::CMessageView()
{
	// Load in settings from INI file
	Initialize();
	
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_bUndo = FALSE; // start out unable to undo or redo anything
}

BOOL CMessageView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

CMessageView::~CMessageView()
{ 
	m_pApp = NULL;
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView Word Wrap support

BOOL CMessageView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL CMessageView::SetWordWrap(BOOL bWordWrap)
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
// CMessageView drawing

void CMessageView::OnDraw(CDC* pDC)
{
	CTextDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView printing

BOOL CMessageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void CMessageView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
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
    
	CWaitCursor wait;
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
	pInfo->SetMaxPage(m_aPageStart.GetSize());
	m_nPreviewPage = nCurPage;
}

void CMessageView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// get string to show as "filename" in header/footer
	CPageSetupSheet shtPageSetup;
	CString strFileName = GetDocument()->GetPathName();
	if (strFileName.IsEmpty())
		strFileName = GetDocument()->GetTitle();

	// go thru CPageSetupSheet::CPageSetupPage to format the header 
	// and footer
	CString strHeader;
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.FormatHeader(strHeader, m_timeHeader, strFileName,
		pInfo->m_nCurPage);
	CString strFooter;        
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

void CMessageView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void CMessageView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView diagnostics

#ifdef _DEBUG
void CMessageView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMessageView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMailBoxDoc* CMessageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMailBoxDoc*)));
	return (CMailBoxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMessageView message handlers

static void ScaleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member from one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}
 
int CMessageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CMessageView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing commands
	CEditView::OnRButtonDown(nFlags, point);
}

void CMessageView::OnSize(UINT nType, int cx, int cy)
{
	CWaitCursor wait;
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void CMessageView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void CMessageView::OnWordWrap()
{
	CWaitCursor wait;
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void CMessageView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void CMessageView::OnChoosePrintFont()
{
	CWaitCursor wait;
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
	wait.Restore();
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

void CMessageView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void CMessageView::OnChooseFont()
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

void CMessageView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());      
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.m_bOutputView = FALSE;
	shtPageSetup.m_pgTabStops.m_nTabStops = m_nTabStops/4;
	if (shtPageSetup.DoModal() == IDOK)
	{
		// get values entered into this tabbed dialog box 
		CWaitCursor wait;
		shtPageSetup.m_pgHeaderFooter.Terminate();
		
		m_nHeaderTime = shtPageSetup.m_pgHeaderFooter.m_nHeaderTime;
		m_nFooterTime = shtPageSetup.m_pgHeaderFooter.m_nFooterTime;
		SetTabStops(shtPageSetup.m_pgTabStops.m_nTabStops*4);
		m_nDefTabStops = m_nTabStops;
     		
		return;
	}  
	return;
}

void CMessageView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void CMessageView::OnWindowRemember()
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

void CMessageView::OnEditUndo()
{
	CWaitCursor wait;
	CEditView::OnEditUndo();
	
	// change the "redo flag" to TRUE so the undone action can be redone
	m_bUndo = TRUE;
	return;
}

void CMessageView::OnEditRedo()
{
	CWaitCursor wait;
	if (m_bUndo)
	{
		CEditView::OnEditUndo();
		m_bUndo = FALSE;	// can't redo anymore -- not until another undo
	}
	return;	// we're done
}	

void CMessageView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bUndo && GetEditCtrl().CanUndo());
	return;
} 

void CMessageView::OnActivateView(BOOL bActivate, CView* pActivateView, 
									CView* pDeactiveView)
{
	// call base class version of this function first so we can
	// process this function properly
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	
	// change the main toolbar above so that it has editing, printing,
	// and saving commands which the user can select to use this view
	// and its attached document
	
	// First, get a pointer to the main window, courtesy of the application
	// class
	CMainFrame* pFrame = m_pApp->GetMainFrame();
	
	// now, if we are being activated, tell the toolbar (via the main window)
	// to change its buttons to those we specify.  If we are being
	// deactivated, tell the main window to reset the toolbar to the
	// Places buttons.
	
	if (bActivate)
	{
		pFrame->GetToolBar()->LoadBitmap(IDR_MESSAGETYPE);
		pFrame->GetToolBar()->SetButtons(msgButtons, 26);
		
	}
	else
		pFrame->ResetToolBar();
	
	// Force updating of the toolbar
	pFrame->GetToolBar()->Invalidate();
	pFrame->GetToolBar()->UpdateWindow();
	
	return;
}	

                             