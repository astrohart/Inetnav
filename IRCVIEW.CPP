// textview.cpp : implementation of the CIRClientView class
//

#include "stdafx.h"
#include "inetnav.h"

#include "text.h"
#include "ircview.h"
#include "propshts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIRClientView

IMPLEMENT_DYNCREATE(CIRClientView, CEditView)

BEGIN_MESSAGE_MAP(CIRClientView, CEditView)
	//{{AFX_MSG_MAP(CIRClientView)
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
	ON_WM_CHAR()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

UINT CIRClientView::m_nDefTabStops;
UINT CIRClientView::m_nDefTabStopsOld;
BOOL CIRClientView::m_bDefWordWrap;
BOOL CIRClientView::m_bDefWordWrapOld;
LOGFONT NEAR CIRClientView::m_lfDefFont;
LOGFONT NEAR CIRClientView::m_lfDefFontOld;
LOGFONT NEAR CIRClientView::m_lfDefPrintFont;
LOGFONT NEAR CIRClientView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

static char BASED_CODE szSettings[] = "Chat Center Conversation: Settings";
static char BASED_CODE szTabStops[] = "TabStops";
static char BASED_CODE szFont[] = "Chat Center Conversation: Font";
static char BASED_CODE szPrintFont[] = "Chat Center Conversation: PrintFont";
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

void CIRClientView::Initialize()
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

void CIRClientView::Terminate()
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
// CIRClientView construction/destruction

CIRClientView::CIRClientView()
{ 
	TRACE0("CIRClientView::CIRClientView()\r\n");
	// Load in settings from INI file
	Initialize();
		
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

BOOL CIRClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

CIRClientView::~CIRClientView()
{ 
	TRACE0("CIRClientView::~CIRClientView()\r\n");
	m_pApp = NULL;
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CIRClientView Word Wrap support

BOOL CIRClientView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL CIRClientView::SetWordWrap(BOOL bWordWrap)
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
// CIRClientView drawing

void CIRClientView::OnDraw(CDC* pDC)
{
	CIRClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CIRClientView printing

BOOL CIRClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void CIRClientView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
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

void CIRClientView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
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
	CString strFooter = "";
	
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

void CIRClientView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void CIRClientView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CIRClientView diagnostics

#ifdef _DEBUG
void CIRClientView::AssertValid() const
{
	CEditView::AssertValid();
}

void CIRClientView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CIRClientDoc* CIRClientView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIRClientDoc)));
	return (CIRClientDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIRClientView message handlers

static void ScaleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member from one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}
 
int CIRClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CIRClientView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing commands
	CEditView::OnRButtonDown(nFlags, point);
}

void CIRClientView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void CIRClientView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void CIRClientView::OnWordWrap()
{
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void CIRClientView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void CIRClientView::OnChoosePrintFont()
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

void CIRClientView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void CIRClientView::OnChooseFont()
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

void CIRClientView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());
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

void CIRClientView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void CIRClientView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 13)
	{
		CString strNewText = "PRIVMSG ";
		CDocument *pDoc = GetDocument();
		CString strEmpty = "";

		strNewText += ((CIRClientDoc *)pDoc)->GetChannelName() + " :";
		((CWnd *)this)->GetWindowText(strEmpty);
		strNewText += strEmpty;
		strEmpty = "";
		((CWnd *)this)->SetWindowText(strEmpty);

		strNewText += "\r\n";
		m_pApp->GetIRCSocket()->Send(strNewText);
	}
	else
		CEditView::OnChar(nChar, nRepCnt, nFlags);
}

void CIRClientView::OnFilePrint()
{
	CIRClientDoc* pDoc = GetDocument();
	
	pDoc->PrintChatTranscript();	// will call server view OnPrintTranscript
}

void CIRClientView::OnPrintTranscript()
{
	CEditView::OnFilePrint();
}