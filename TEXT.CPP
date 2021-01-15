// text.cpp: implementation file for classes for Internet Navigator's built-in
// text editor

#include "stdafx.h"
#include "inetnav.h"

#include "text.h"  
#include "propshts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextDoc document

IMPLEMENT_DYNCREATE(CTextDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextDoc, CDocument)
	//{{AFX_MSG_MAP(CTextDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextDoc construction/destruction

CTextDoc::CTextDoc()
{
	// TODO: add one-time construction code here
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

CTextDoc::~CTextDoc()
{
}

BOOL CTextDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
 	
 	SetTitle("Untitled");
 	
 	return TRUE;
}

BOOL CTextDoc::SaveModified()
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
// CTextDoc serialization

void CTextDoc::Serialize(CArchive& ar)
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

void CTextDoc::DeleteContents()
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
// CTextDoc diagnostics

#ifdef _DEBUG
void CTextDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextDoc commands

void CTextDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// Only enable the File, Save menu item if the document data has
	// changed
	pCmdUI->Enable(IsModified());
	return;
}

void CTextDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CTextView text view

IMPLEMENT_DYNCREATE(CTextView, CEditView)

BEGIN_MESSAGE_MAP(CTextView, CEditView)
	//{{AFX_MSG_MAP(CTextView)
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
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

UINT CTextView::m_nDefTabStops;
UINT CTextView::m_nDefTabStopsOld;
BOOL CTextView::m_bDefWordWrap;
BOOL CTextView::m_bDefWordWrapOld;
LOGFONT NEAR CTextView::m_lfDefFont;
LOGFONT NEAR CTextView::m_lfDefFontOld;
LOGFONT NEAR CTextView::m_lfDefPrintFont;
LOGFONT NEAR CTextView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

static char BASED_CODE szSettings[] = "Text Processor: Settings";
static char BASED_CODE szTabStops[] = "TabStops";
static char BASED_CODE szFont[] = "Text Processor: Font";
static char BASED_CODE szPrintFont[] = "Text Processor: PrintFont";
static char BASED_CODE szHeight[] = "Height";
static char BASED_CODE szWeight[] = "Weight";
static char BASED_CODE szItalic[] = "Italic";
static char BASED_CODE szUnderline[] = "Underline";
static char BASED_CODE szPitchAndFamily[] = "PitchAndFamily";
static char BASED_CODE szFaceName[] = "FaceName";
static char BASED_CODE szSystem[] = "Arial";
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

void CTextView::Initialize()
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

void CTextView::Terminate()
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
// CTextView construction/destruction

CTextView::CTextView()
{
	// Load in settings from INI file
	Initialize();
	
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_bUndo = FALSE; // start out unable to undo or redo anything
}

BOOL CTextView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

CTextView::~CTextView()
{ 
	m_pApp = NULL;
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CTextView Word Wrap support

BOOL CTextView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL CTextView::SetWordWrap(BOOL bWordWrap)
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
// CTextView drawing

void CTextView::OnDraw(CDC* pDC)
{
	CTextDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTextView printing

BOOL CTextView::OnPreparePrinting(CPrintInfo* pInfo)
{ 
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void CTextView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
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

void CTextView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// get string to show as "filename" in header/footer
	CPageSetupSheet shtPageSetup;
	CString strFileName = GetDocument()->GetPathName();
	if (strFileName.IsEmpty())
		strFileName = GetDocument()->GetTitle();

	// go thru CPageSetupSheet::CPageSetupPage to format the header 
	// and footer
	CString strHeader = "";
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.FormatHeader(strHeader, m_timeHeader, strFileName,
		pInfo->m_nCurPage);
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

void CTextView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void CTextView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CTextView diagnostics

#ifdef _DEBUG
void CTextView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CTextDoc* CTextView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextDoc)));
	return (CTextDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextView message handlers

static void ScaleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member from one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}
 
int CTextView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CTextView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing commands
	CEditView::OnRButtonDown(nFlags, point);
}

void CTextView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void CTextView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void CTextView::OnWordWrap()
{
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void CTextView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void CTextView::OnChoosePrintFont()
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

void CTextView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void CTextView::OnChooseFont()
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

void CTextView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());      
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.m_bOutputView = FALSE;
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

void CTextView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void CTextView::OnWindowRemember()
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

void CTextView::OnEditUndo()
{
	CEditView::OnEditUndo();
	
	// change the "redo flag" to TRUE so the undone action can be redone
	m_bUndo = TRUE;
	return;
}

void CTextView::OnEditRedo()
{
	if (m_bUndo)
	{
		CEditView::OnEditUndo();
		m_bUndo = FALSE;	// can't redo anymore -- not until another undo
	}
	return;	// we're done
}	

void CTextView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bUndo && GetEditCtrl().CanUndo());
	return;
} 

/////////////////////////////////////////////////////////////////////////////
// CTextWnd

IMPLEMENT_DYNCREATE(CTextWnd, CMDIChildWnd)

CTextWnd::CTextWnd()
{
}

CTextWnd::~CTextWnd()
{
}

BEGIN_MESSAGE_MAP(CTextWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CTextWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextWnd message handlers

BOOL CTextWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
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

int CTextWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
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

void CTextWnd::OnViewRuler()
{
	BOOL bToggleVisible = !m_wndRulerBar.IsWindowVisible();
	
	// If IsWindowVisible() == TRUE, we should hide the
	// ruler bar. If IsWindowVisible() == FALSE, then we
	// should show the ruler bar.
	m_wndRulerBar.ShowWindow((bToggleVisible)?SW_SHOW:SW_HIDE);
	RecalcLayout(); // adjust positions of various child windows
	return;
}

void CTextWnd::OnUpdateViewRuler(CCmdUI* pCmdUI)
{   
	// If the ruler is visible, we want to put a check mark
	// next to the Ruler menu command.  
	// CDialogBar::IsWindowVisible() tells us whether or not
	// the ruler is visible
	BOOL bCheck = m_wndRulerBar.IsWindowVisible();
	pCmdUI->SetCheck(bCheck);
}
