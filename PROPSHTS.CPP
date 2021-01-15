// propshts.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"

#include "dialogs.h"
#include "propshts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static UINT optionsButtons[3] = { IDOK, IDCANCEL, ID_HELP, };

/////////////////////////////////////////////////////////////////////////////
//  CPageSetupSheet property sheet                          

BEGIN_MESSAGE_MAP(CPageSetupSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPageSetupSheet)
	ON_WM_CREATE()  
	ON_BN_CLICKED(ID_HELP, CWnd::OnHelp)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPageSetupSheet::CPageSetupSheet(CWnd* pParentWnd /*=NULL*/)
	: CPropertySheet("Page Setup", pParentWnd, 0)
{      
	AddPage(&m_pgHeaderFooter);
	AddPage(&m_pgTabStops);
}

BOOL CPageSetupSheet::CreateStandardButtons()
{
	for (int i = 0; 
		i < sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
	{
		// Note: If one of these _AfxLoadString calls fail, it is likely that
		//  _AFX_NO_PROPERTY_RESOURCES is defined in your .RC file.
		// To correct the situation, remove the following line from your
		//  resource script:
		//      #define _AFX_NO_PROPERTY_RESOURCES
		// This should be done using the Resource.Set Includes... command.

		// load the caption
		CString strCaption = "";
		VERIFY(strCaption.LoadString(AFX_IDS_PS_OK+i) != 0);
		char* lpsz = strstr(strCaption, "\n");                    
		if (lpsz != NULL)
			*lpsz = '\0';

		// create the control
		HWND hWnd = ::CreateWindow(_T("button"), strCaption,
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP|BS_PUSHBUTTON,
			0, 0, 0, 0, m_hWnd, (HMENU)optionsButtons[i],
			AfxGetInstanceHandle(), NULL);
		if (hWnd == NULL)
		{
			TRACE0("Warning: failed to create options buttons\n");
			return FALSE;
		}

		// set the font
		if (m_hFont != NULL)
			::SendMessage(hWnd, WM_SETFONT, (WPARAM)m_hFont, 0);
	}
 
 	UpdateDialogControls(this, FALSE);

	return TRUE;
}

int CPageSetupSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{  
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    
    // subclass this window with CTL3D
    if (((CInternetNavApp*)AfxGetApp())->IsUseCTL3D())
    	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);  
    
    // Initialize values for Header/Footer page from INI file
    m_pgHeaderFooter.Initialize();
    
	// fix-up the system menu so this looks like a dialog box
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	ASSERT(pSysMenu != NULL);
	int i, nCount = pSysMenu->GetMenuItemCount();
	for (i = 0; i < nCount; i++)
	{
		UINT nID = pSysMenu->GetMenuItemID(i);
		if (nID != SC_MOVE && nID != SC_CLOSE)
		{
			pSysMenu->DeleteMenu(i, MF_BYPOSITION);
			i--;
			nCount--;
		}
	}

	// set active page and active tab
	SetActivePage(m_nCurPage);

	// initialize font used for buttons
	ASSERT(m_hFont == NULL);
	CPropertyPage* pPage = GetActivePage();
	ASSERT_VALID(pPage);

	HFONT hFont = (HFONT)pPage->SendMessage(WM_GETFONT);
	if (hFont != NULL)
	{
		LOGFONT logFont;
		VERIFY(::GetObject(hFont, sizeof(LOGFONT), &logFont));
		m_hFont = CreateFontIndirect(&logFont);
	}

	// create the tab control itself
	CRect rect(0, 0, 100, 10);
	if (!m_tabRow.Create(WS_GROUP|WS_TABSTOP|WS_CHILD|WS_VISIBLE|
		(m_bStacked ? TCS_MULTILINE : 0), rect, this, AFX_IDC_TAB_CONTROL))
	{
		return -1;
	}

	// set page's z-order correctly (side effect of SetActivePage)
	SetActivePage(m_nCurPage);

	// calculate button sizes and separator
	rect.right = 50;    // slightly smaller buttons than normal
	rect.bottom = 14;
	rect.left = 4;      // button gap is 4 dialog units
	pPage->MapDialogRect(rect);
	m_sizeButton.cx = rect.right;
	m_sizeButton.cy = rect.bottom;
	m_cxButtonGap = rect.left;

	// calculate tab margin area
	rect.bottom = rect.right = 4;   // std dialog margin is 6 dialog units
	pPage->MapDialogRect(rect);
	m_sizeTabMargin.cx = rect.right;
	m_sizeTabMargin.cy = rect.bottom;

	// create standard buttons
	if (!CPageSetupSheet::CreateStandardButtons())
		return -1;

	CPageSetupSheet::RecalcLayout();
	m_tabRow.SetFocus();
	m_tabRow.SetCurSel(m_nCurPage);
	CenterWindow(m_pParentWnd);

	return 0;   // success
}

void CPageSetupSheet::OnClose()
{
	// Save Header/Footer tab's settings to INI file
	m_pgHeaderFooter.Terminate();
	
	CPropertySheet::OnClose();
}

void CPageSetupSheet::RecalcLayout()
{
	// determine size of the active page (active page determines initial size)
	CRect rectPage;
	GetActivePage()->GetWindowRect(rectPage);
	int nWidth = 2 * m_sizeTabMargin.cx + rectPage.Width() + 3;

	// determine total size of the buttons
	int cxButtons[sizeof(optionsButtons)/sizeof(optionsButtons[0])];
	int cxButtonTotal = 0;
	int cxButtonGap = 0;
	if (!m_bModeless)
	{
		for (int i = 0; 
			i < sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
		{
			cxButtons[i] = m_sizeButton.cx;

			// load the button caption information (may contain button size info)
			CString strTemp = "";
			VERIFY(strTemp.LoadString(AFX_IDS_PS_OK+i) != 0);

			// format is Apply\n50 (ie. text\nCX)
			char* lpsz = strstr(strTemp, "\n");
			if (lpsz != NULL)
			{
				// convert CX fields from text dialog units to binary pixels
				CRect rect(0, 0, 0, 0);
				rect.right = _ttoi(lpsz+1);
				GetActivePage()->MapDialogRect(&rect);
				cxButtons[i] = rect.Width();
			}
			if (::GetDlgItem(m_hWnd, optionsButtons[i]) != NULL)
			{
				cxButtonTotal += cxButtons[i];
				cxButtonGap += m_cxButtonGap;
			}
		}
	}
	if (cxButtonGap != 0)
		cxButtonGap -= m_cxButtonGap;

	// margin OK buttonGap Cancel buttonGap Help margin
	// margin is same as tab margin
	// button sizes are totaled in cxButtonTotal + cxButtonGap
	nWidth = max(nWidth, 2*m_sizeTabMargin.cx + cxButtonTotal + cxButtonGap);

	m_tabRow.SetWindowPos(NULL, m_sizeTabMargin.cx, m_sizeTabMargin.cy,
		nWidth - m_sizeTabMargin.cx*2, 0, SWP_NOACTIVATE|SWP_NOZORDER);
	CRect rectTabRow;
	m_tabRow.GetWindowRect(&rectTabRow);
	int nTabHeight = rectTabRow.Height();

	int nHeight = 2 * m_sizeTabMargin.cy + rectPage.Height() + nTabHeight + 4
		+ m_sizeTabMargin.cy + m_sizeButton.cy; // leave room for buttons

	CRect rectSheet(0, 0, nWidth, nHeight);
	CRect rectClient = rectSheet;
	::AdjustWindowRectEx(rectSheet, GetStyle(), FALSE, GetExStyle());

	SetWindowPos(NULL, 0, 0, rectSheet.Width(), rectSheet.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	CenterWindow();

	GetActivePage()->SetWindowPos(NULL,
		m_sizeTabMargin.cx+1, m_sizeTabMargin.cy + nTabHeight,
		nWidth - m_sizeTabMargin.cx*2 - 3, rectPage.Height(),
		SWP_NOACTIVATE | SWP_NOZORDER);

	if (!m_bModeless)
	{
		int x = nWidth - m_sizeTabMargin.cx - cxButtonTotal - cxButtonGap;
		int y = (nHeight - m_sizeTabMargin.cy) - m_sizeButton.cy;
		for (int i = 0; i < 
			sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
		{
			HWND hWnd = ::GetDlgItem(m_hWnd, optionsButtons[i]);
			if (hWnd != NULL)
			{
				::MoveWindow(hWnd, x, y, cxButtons[i], m_sizeButton.cy, TRUE);
				x += cxButtons[i] + m_cxButtonGap;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//  CAppPrefsSheet property sheet                          

BEGIN_MESSAGE_MAP(CAppPrefsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CAppPrefsSheet)
	ON_WM_CREATE()  
	ON_BN_CLICKED(ID_HELP, CWnd::OnHelp)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAppPrefsSheet::CAppPrefsSheet(CWnd* pParentWnd /*=NULL*/)
	: CPropertySheet("Application Preferences", pParentWnd, 0)
{      
	AddPage(&m_appGeneralPrefs);
	AddPage(&m_appAreasPrefs);
}

BOOL CAppPrefsSheet::CreateStandardButtons()
{
	for (int i = 0; 
		i < sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
	{
		// Note: If one of these _AfxLoadString calls fail, it is likely that
		//  _AFX_NO_PROPERTY_RESOURCES is defined in your .RC file.
		// To correct the situation, remove the following line from your
		//  resource script:
		//      #define _AFX_NO_PROPERTY_RESOURCES
		// This should be done using the Resource.Set Includes... command.

		// load the caption
		CString strCaption = "";
		VERIFY(strCaption.LoadString(AFX_IDS_PS_OK+i) != 0);
		char* lpsz = strstr(strCaption, "\n");                    
		if (lpsz != NULL)
			*lpsz = '\0';

		// create the control
		HWND hWnd = ::CreateWindow(_T("button"), strCaption,
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP|BS_PUSHBUTTON,
			0, 0, 0, 0, m_hWnd, (HMENU)optionsButtons[i],
			AfxGetInstanceHandle(), NULL);
		if (hWnd == NULL)
		{
			TRACE0("Warning: failed to create options buttons\n");
			return FALSE;
		}

		// set the font
		if (m_hFont != NULL)
			::SendMessage(hWnd, WM_SETFONT, (WPARAM)m_hFont, 0);
	}
 
 	UpdateDialogControls(this, FALSE);

	return TRUE;
}

int CAppPrefsSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    
    // subclass this window with CTL3D
    if (((CInternetNavApp*)AfxGetApp())->IsUseCTL3D())
    	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);  
 
	// fix-up the system menu so this looks like a dialog box
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	ASSERT(pSysMenu != NULL);
	int i, nCount = pSysMenu->GetMenuItemCount();
	for (i = 0; i < nCount; i++)
	{
		UINT nID = pSysMenu->GetMenuItemID(i);
		if (nID != SC_MOVE && nID != SC_CLOSE)
		{
			pSysMenu->DeleteMenu(i, MF_BYPOSITION);
			i--;
			nCount--;
		}
	}

	// set active page and active tab
	SetActivePage(m_nCurPage);

	// initialize font used for buttons
	ASSERT(m_hFont == NULL);
	CPropertyPage* pPage = GetActivePage();
	ASSERT_VALID(pPage);

	HFONT hFont = (HFONT)pPage->SendMessage(WM_GETFONT);
	if (hFont != NULL)
	{
		LOGFONT logFont;
		VERIFY(::GetObject(hFont, sizeof(LOGFONT), &logFont));
		m_hFont = CreateFontIndirect(&logFont);
	}

	// create the tab control itself
	CRect rect(0, 0, 100, 10);
	if (!m_tabRow.Create(WS_GROUP|WS_TABSTOP|WS_CHILD|WS_VISIBLE|
		(m_bStacked ? TCS_MULTILINE : 0), rect, this, AFX_IDC_TAB_CONTROL))
	{
		return -1;
	}

	// set page's z-order correctly (side effect of SetActivePage)
	SetActivePage(m_nCurPage);

	// calculate button sizes and separator
	rect.right = 50;    // slightly smaller buttons than normal
	rect.bottom = 14;
	rect.left = 4;      // button gap is 4 dialog units
	pPage->MapDialogRect(rect);
	m_sizeButton.cx = rect.right;
	m_sizeButton.cy = rect.bottom;
	m_cxButtonGap = rect.left;

	// calculate tab margin area
	rect.bottom = rect.right = 4;   // std dialog margin is 6 dialog units
	pPage->MapDialogRect(rect);
	m_sizeTabMargin.cx = rect.right;
	m_sizeTabMargin.cy = rect.bottom;

	// create standard buttons
	if (!CAppPrefsSheet::CreateStandardButtons())
		return -1;

	CAppPrefsSheet::RecalcLayout();
	m_tabRow.SetFocus();
	m_tabRow.SetCurSel(m_nCurPage);
	CenterWindow(m_pParentWnd);

	return 0;   // success
}

void CAppPrefsSheet::OnClose()
{
	CPropertySheet::OnClose();
}

void CAppPrefsSheet::RecalcLayout()
{
	// determine size of the active page (active page determines initial size)
	CRect rectPage;
	GetActivePage()->GetWindowRect(rectPage);
	int nWidth = 2 * m_sizeTabMargin.cx + rectPage.Width() + 3;

	// determine total size of the buttons
	int cxButtons[sizeof(optionsButtons)/sizeof(optionsButtons[0])];
	int cxButtonTotal = 0;
	int cxButtonGap = 0;
	if (!m_bModeless)
	{
		for (int i = 0; 
			i < sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
		{
			cxButtons[i] = m_sizeButton.cx;

			// load the button caption information (may contain button size info)
			CString strTemp = "";
			VERIFY(strTemp.LoadString(AFX_IDS_PS_OK+i) != 0);

			// format is Apply\n50 (ie. text\nCX)
			char* lpsz = strstr(strTemp, "\n");
			if (lpsz != NULL)
			{
				// convert CX fields from text dialog units to binary pixels
				CRect rect(0, 0, 0, 0);
				rect.right = _ttoi(lpsz+1);
				GetActivePage()->MapDialogRect(&rect);
				cxButtons[i] = rect.Width();
			}
			if (::GetDlgItem(m_hWnd, optionsButtons[i]) != NULL)
			{
				cxButtonTotal += cxButtons[i];
				cxButtonGap += m_cxButtonGap;
			}
		}
	}
	if (cxButtonGap != 0)
		cxButtonGap -= m_cxButtonGap;

	// margin OK buttonGap Cancel buttonGap Help margin
	// margin is same as tab margin
	// button sizes are totaled in cxButtonTotal + cxButtonGap
	nWidth = max(nWidth, 2*m_sizeTabMargin.cx + cxButtonTotal + cxButtonGap);

	m_tabRow.SetWindowPos(NULL, m_sizeTabMargin.cx, m_sizeTabMargin.cy,
		nWidth - m_sizeTabMargin.cx*2, 0, SWP_NOACTIVATE|SWP_NOZORDER);
	CRect rectTabRow;
	m_tabRow.GetWindowRect(&rectTabRow);
	int nTabHeight = rectTabRow.Height();

	int nHeight = 2 * m_sizeTabMargin.cy + rectPage.Height() + nTabHeight + 4
		+ m_sizeTabMargin.cy + m_sizeButton.cy; // leave room for buttons

	CRect rectSheet(0, 0, nWidth, nHeight);
	CRect rectClient = rectSheet;
	::AdjustWindowRectEx(rectSheet, GetStyle(), FALSE, GetExStyle());

	SetWindowPos(NULL, 0, 0, rectSheet.Width(), rectSheet.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	CenterWindow();

	GetActivePage()->SetWindowPos(NULL,
		m_sizeTabMargin.cx+1, m_sizeTabMargin.cy + nTabHeight,
		nWidth - m_sizeTabMargin.cx*2 - 3, rectPage.Height(),
		SWP_NOACTIVATE | SWP_NOZORDER);

	if (!m_bModeless)
	{
		int x = nWidth - m_sizeTabMargin.cx - cxButtonTotal - cxButtonGap;
		int y = (nHeight - m_sizeTabMargin.cy) - m_sizeButton.cy;
		for (int i = 0; i < 
			sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
		{
			HWND hWnd = ::GetDlgItem(m_hWnd, optionsButtons[i]);
			if (hWnd != NULL)
			{
				::MoveWindow(hWnd, x, y, cxButtons[i], m_sizeButton.cy, TRUE);
				x += cxButtons[i] + m_cxButtonGap;
			}
		}
	}
}

void CAppPrefsSheet::Serialize(BOOL IsStoring)
{
	// call the serialize functions of the property pages
	m_appGeneralPrefs.Serialize(IsStoring); 
	m_appAreasPrefs.Serialize(IsStoring);
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
//  CInetPrefsSheet property sheet                          

BEGIN_MESSAGE_MAP(CInetPrefsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CInetPrefsSheet)
	ON_WM_CREATE()  
	ON_BN_CLICKED(ID_HELP, CWnd::OnHelp)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CInetPrefsSheet::CInetPrefsSheet(CWnd* pParentWnd /*=NULL*/)
	: CPropertySheet("Internet Preferences", pParentWnd, 0)
{      
	AddPage(&m_inetFingerPrefs); 
	AddPage(&m_inetWhoisPrefs);    
	AddPage(&m_inetFTPPrefs);
	AddPage(&m_inetChatPrefs);
	AddPage(&m_inetMailPrefs);   
	AddPage(&m_inetNewsPrefs); 
	AddPage(&m_inetGopherPrefs);  
}

BOOL CInetPrefsSheet::CreateStandardButtons()
{
	for (int i = 0; 
		i < sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
	{
		// Note: If one of these _AfxLoadString calls fail, it is likely that
		//  _AFX_NO_PROPERTY_RESOURCES is defined in your .RC file.
		// To correct the situation, remove the following line from your
		//  resource script:
		//      #define _AFX_NO_PROPERTY_RESOURCES
		// This should be done using the Resource.Set Includes... command.

		// load the caption
		CString strCaption = "";
		VERIFY(strCaption.LoadString(AFX_IDS_PS_OK+i) != 0);
		char* lpsz = strstr(strCaption, "\n");                    
		if (lpsz != NULL)
			*lpsz = '\0';

		// create the control
		HWND hWnd = ::CreateWindow(_T("button"), strCaption,
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP|BS_PUSHBUTTON,
			0, 0, 0, 0, m_hWnd, (HMENU)optionsButtons[i],
			AfxGetInstanceHandle(), NULL);
		if (hWnd == NULL)
		{
			TRACE0("Warning: failed to create options buttons\n");
			return FALSE;
		}

		// set the font
		if (m_hFont != NULL)
			::SendMessage(hWnd, WM_SETFONT, (WPARAM)m_hFont, 0);
	}
 
 	UpdateDialogControls(this, FALSE);

	return TRUE;
}

int CInetPrefsSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    
    // subclass this window with CTL3D
    if (((CInternetNavApp*)AfxGetApp())->IsUseCTL3D())
    	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);  
 
	// fix-up the system menu so this looks like a dialog box
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	ASSERT(pSysMenu != NULL);
	int i, nCount = pSysMenu->GetMenuItemCount();
	for (i = 0; i < nCount; i++)
	{
		UINT nID = pSysMenu->GetMenuItemID(i);
		if (nID != SC_MOVE && nID != SC_CLOSE)
		{
			pSysMenu->DeleteMenu(i, MF_BYPOSITION);
			i--;
			nCount--;
		}
	}

	// set active page and active tab
	SetActivePage(m_nCurPage);

	// initialize font used for buttons
	ASSERT(m_hFont == NULL);
	CPropertyPage* pPage = GetActivePage();
	ASSERT_VALID(pPage);

	HFONT hFont = (HFONT)pPage->SendMessage(WM_GETFONT);
	if (hFont != NULL)
	{
		LOGFONT logFont;
		VERIFY(::GetObject(hFont, sizeof(LOGFONT), &logFont));
		m_hFont = CreateFontIndirect(&logFont);
	}

	// create the tab control itself
	CRect rect(0, 0, 100, 10);
	if (!m_tabRow.Create(WS_GROUP|WS_TABSTOP|WS_CHILD|WS_VISIBLE|
		(m_bStacked ? TCS_MULTILINE : 0), rect, this, AFX_IDC_TAB_CONTROL))
	{
		return -1;
	}

	// set page's z-order correctly (side effect of SetActivePage)
	SetActivePage(m_nCurPage);

	// calculate button sizes and separator
	rect.right = 50;    // slightly smaller buttons than normal
	rect.bottom = 14;
	rect.left = 4;      // button gap is 4 dialog units
	pPage->MapDialogRect(rect);
	m_sizeButton.cx = rect.right;
	m_sizeButton.cy = rect.bottom;
	m_cxButtonGap = rect.left;

	// calculate tab margin area
	rect.bottom = rect.right = 4;   // std dialog margin is 6 dialog units
	pPage->MapDialogRect(rect);
	m_sizeTabMargin.cx = rect.right;
	m_sizeTabMargin.cy = rect.bottom;

	// create standard buttons
	if (!CInetPrefsSheet::CreateStandardButtons())
		return -1;

	CInetPrefsSheet::RecalcLayout();
	m_tabRow.SetFocus();
	m_tabRow.SetCurSel(m_nCurPage);
	CenterWindow(m_pParentWnd); 
	
	Serialize(FALSE);

	return 0;   // success
}

void CInetPrefsSheet::OnClose()
{
	CPropertySheet::OnClose();
}

void CInetPrefsSheet::RecalcLayout()
{ 
	// determine size of the active page (active page determines initial size)
	CRect rectPage;
	GetActivePage()->GetWindowRect(rectPage);
	int nWidth = 2 * m_sizeTabMargin.cx + rectPage.Width() + 3;

	// determine total size of the buttons
	int cxButtons[sizeof(optionsButtons)/sizeof(optionsButtons[0])];
	int cxButtonTotal = 0;
	int cxButtonGap = 0;
	if (!m_bModeless)
	{
		for (int i = 0; 
			i < sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
		{
			cxButtons[i] = m_sizeButton.cx;

			// load the button caption information (may contain button size info)
			CString strTemp = "";
			VERIFY(strTemp.LoadString(AFX_IDS_PS_OK+i) != 0);

			// format is Apply\n50 (ie. text\nCX)
			char* lpsz = strstr(strTemp, "\n");
			if (lpsz != NULL)
			{
				// convert CX fields from text dialog units to binary pixels
				CRect rect(0, 0, 0, 0);
				rect.right = _ttoi(lpsz+1);
				GetActivePage()->MapDialogRect(&rect);
				cxButtons[i] = rect.Width();
			}
			if (::GetDlgItem(m_hWnd, optionsButtons[i]) != NULL)
			{
				cxButtonTotal += cxButtons[i];
				cxButtonGap += m_cxButtonGap;
			}
		}
	}
	if (cxButtonGap != 0)
		cxButtonGap -= m_cxButtonGap;

	// margin OK buttonGap Cancel buttonGap Help margin
	// margin is same as tab margin
	// button sizes are totaled in cxButtonTotal + cxButtonGap
	nWidth = max(nWidth, 2*m_sizeTabMargin.cx + cxButtonTotal + cxButtonGap);

	m_tabRow.SetWindowPos(NULL, m_sizeTabMargin.cx, m_sizeTabMargin.cy,
		nWidth - m_sizeTabMargin.cx*2, 0, SWP_NOACTIVATE|SWP_NOZORDER);
	CRect rectTabRow;
	m_tabRow.GetWindowRect(&rectTabRow);
	int nTabHeight = rectTabRow.Height();

	int nHeight = 2 * m_sizeTabMargin.cy + rectPage.Height() + nTabHeight + 4
		+ m_sizeTabMargin.cy + m_sizeButton.cy; // leave room for buttons

	CRect rectSheet(0, 0, nWidth, nHeight);
	CRect rectClient = rectSheet;
	::AdjustWindowRectEx(rectSheet, GetStyle(), FALSE, GetExStyle());

	SetWindowPos(NULL, 0, 0, rectSheet.Width(), rectSheet.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	CenterWindow();

	GetActivePage()->SetWindowPos(NULL,
		m_sizeTabMargin.cx+1, m_sizeTabMargin.cy + nTabHeight,
		nWidth - m_sizeTabMargin.cx*2 - 3, rectPage.Height(),
		SWP_NOACTIVATE | SWP_NOZORDER);

	if (!m_bModeless)
	{
		int x = nWidth - m_sizeTabMargin.cx - cxButtonTotal - cxButtonGap;
		int y = (nHeight - m_sizeTabMargin.cy) - m_sizeButton.cy;
		for (int i = 0; i < 
			sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
		{
			HWND hWnd = ::GetDlgItem(m_hWnd, optionsButtons[i]);
			if (hWnd != NULL)
			{
				::MoveWindow(hWnd, x, y, cxButtons[i], m_sizeButton.cy, TRUE);
				x += cxButtons[i] + m_cxButtonGap;
			}
		}
	}
}

void CInetPrefsSheet::Serialize(BOOL IsStoring)
{
	// call the serialize functions of the property pages
	m_inetFingerPrefs.Serialize(IsStoring); 
	m_inetWhoisPrefs.Serialize(IsStoring);      
	m_inetFTPPrefs.Serialize(IsStoring);  
	m_inetChatPrefs.Serialize(IsStoring);
	m_inetMailPrefs.Serialize(IsStoring);
	m_inetGopherPrefs.Serialize(IsStoring);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CPageSetupPage property page

CPageSetupPage::CPageSetupPage()
	: CPropertyPage(CPageSetupPage::IDD)
{
	//{{AFX_DATA_INIT(CPageSetupPage)
	m_nFooterTime = 0;
	m_nHeaderTime = 0;
	//}}AFX_DATA_INIT  
	m_pApp = (CInternetNavApp*)AfxGetApp(); 
	Initialize();
}

void CPageSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSetupPage)
	DDX_Control(pDX, IDC_HEADER_FILE, m_btnHeaderFile);    
	DDX_Control(pDX, IDC_HEADER_SYSTEM, m_btnHeaderSystem);
	DDX_Control(pDX, IDC_HEADER_LABEL, m_lblHeader);
	DDX_Control(pDX, IDC_HEADER, m_ctlHeader);
	DDX_Text(pDX, IDC_HEADER, m_strHeader);
	DDV_MaxChars(pDX, m_strHeader, 255);
	DDX_Text(pDX, IDC_FOOTER, m_strFooter);
	DDV_MaxChars(pDX, m_strFooter, 255);
	DDX_Radio(pDX, IDC_FOOTER_FILE, m_nFooterTime);
	DDX_Radio(pDX, IDC_HEADER_FILE, m_nHeaderTime);
	//}}AFX_DATA_MAP 
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
	Terminate();  // write data to INI file as it is changed
}

BEGIN_MESSAGE_MAP(CPageSetupPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSetupPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSetupPage member functions

static void Replace(CString& strModify, const char* pszWhat, const char* pszWith)
	// replace pszWhat with pwzWith in string strModify
{
	int i;
	if ((i = strModify.Find(pszWhat)) >= 0)
		strModify = strModify.Left(i) + pszWith + strModify.Mid(i+strlen(pszWhat));
}

void CPageSetupPage::FormatFilePage(CString& strFormat,
	CString strFileName, UINT nPage)
{
	char sz[32];
	wsprintf(sz, "%d", nPage);
	::Replace(strFormat, "&p", sz);
	if (strFileName.IsEmpty())
	{
		strFileName = sz;
		sz[0] = 0;
	}
	::Replace(strFormat, "&f", strFileName);
}  

void CPageSetupPage::FormatHeader(CString& strHeader, CTime& time,
	CString strFileName, UINT nPage)
{
	CString strFormat = m_strHeader;
	FormatFilePage(strFormat, strFileName, nPage);
	CString strPageHeader = strFormat; // save what header we have before
											// adding the time
#ifndef _AFXDLL                                           
	// Include time with time zone in header
	strHeader = strFormat + " " + time.Format("%I:%M %p %Z");
#else
	// formatted time option not supported
	(void)&time;        // not used
	strHeader = strFormat;
#endif
}

void CPageSetupPage::FormatFooter(CString& strFooter, CTime& time,
	CString strFileName, UINT nPage)
{
	CString strFormat = m_strFooter;
	FormatFilePage(strFormat, strFileName, nPage);
	CString strPageFooter = strFormat; // save what footer we have before
											// adding the time
#ifndef _AFXDLL                                              
	// Include time with time zone in footer
   	strFooter = strFormat + " " + time.Format("%I:%M %p %Z");
#else
	// formatted time option not supported
	(void)&time;        // not used
	strFooter = strFormat;
#endif
}


static char BASED_CODE szPageSetup[] = "PageSetup";
static char BASED_CODE szHeader[] = "Header";
static char BASED_CODE szFooter[] = "Footer";
static char BASED_CODE szHeaderTime[] = "HeaderTime";
static char BASED_CODE szFooterTime[] = "FooterTime";

void CPageSetupPage::Initialize()
{
	m_strHeader = m_pApp->GetProfileString(szPageSetup, szHeader, "File: &f");
	m_strFooter = m_pApp->GetProfileString(szPageSetup, szFooter, "Page: &p");
	m_nHeaderTime = m_pApp->GetProfileInt(szPageSetup, szHeaderTime, 0);
	m_nFooterTime = m_pApp->GetProfileInt(szPageSetup, szFooterTime, 0);
	m_strHeaderOld = m_strHeader;
	m_strFooterOld = m_strFooter;
	m_nHeaderTimeOld = m_nHeaderTime;
	m_nFooterTimeOld = m_nFooterTime;
}

void CPageSetupPage::Terminate()
{
	if (m_strHeader != m_strHeaderOld)
		m_pApp->WriteProfileString(szPageSetup, szHeader, m_strHeader);
	if (m_strFooter != m_strFooterOld)
		m_pApp->WriteProfileString(szPageSetup, szFooter, m_strFooter);
	if (m_nHeaderTime != m_nHeaderTimeOld)
		m_pApp->WriteProfileInt(szPageSetup, szHeaderTime, m_nHeaderTime);
	if (m_nFooterTime != m_nFooterTimeOld)
		m_pApp->WriteProfileInt(szPageSetup, szFooterTime, m_nFooterTime);
}

BOOL CPageSetupPage::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}

	// If the Page Setup dialog box is being shown to configure the
	// COutputView, we want to hide the Header edit box and label.
	// COutputView doesn't use a user-defined header.
	Initialize();
	
	if (m_bOutputView)
	{
		m_lblHeader.ShowWindow(SW_HIDE);
		
		m_ctlHeader.ShowWindow(SW_HIDE);
		m_btnHeaderFile.ShowWindow(SW_HIDE);
		m_btnHeaderSystem.ShowWindow(SW_HIDE);
	}
	else
	{
		m_lblHeader.ShowWindow(SW_SHOW);
		
		m_ctlHeader.ShowWindow(SW_SHOW);
		m_btnHeaderFile.ShowWindow(SW_SHOW);
		m_btnHeaderSystem.ShowWindow(SW_SHOW);
	}		                          
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

////////////////////////////////////////////////////////////////////////
// CSetTabsPage property page

CSetTabsPage::CSetTabsPage() : CPropertyPage(CSetTabsPage::IDD)
{
	//{{AFX_DATA_INIT(CSetTabsPage)
	//}}AFX_DATA_INIT
}

void CSetTabsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTabsPage)
	DDX_Text(pDX, IDC_EDIT_TABS, m_nTabStops);
	DDV_MinMaxInt(pDX, m_nTabStops, 1, 16);
	//}}AFX_DATA_MAP  
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CSetTabsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSetTabsPage)
	ON_EN_CHANGE(IDC_EDIT_TABS, OnChangeEditTabs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSetTabsPage message handlers

void CSetTabsPage::OnChangeEditTabs()
{
	UpdateData(TRUE);
	return;
}    

BOOL CSetTabsPage::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CAppGeneralPrefs property page


CAppGeneralPrefs::CAppGeneralPrefs() : CPropertyPage(CAppGeneralPrefs::IDD)
{
	//{{AFX_DATA_INIT(CAppGeneralPrefs)
	m_pApp = (CInternetNavApp*)AfxGetApp();  
	m_nShowTips = 0;
	//}}AFX_DATA_INIT
}

void CAppGeneralPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppGeneralPrefs)
	DDX_Control(pDX, IDC_SHOW_TIPS_INET, m_btnShowTipsInet);
	DDX_Control(pDX, IDC_SHOW_TIPS, m_btnShowTipsApp);
	DDX_Check(pDX, IDC_START_ONLINE, m_bStartOnline);
	DDX_Check(pDX, IDC_SHOW_MAIN_MENU, m_bShowMainMenu);
	DDX_Check(pDX, IDC_CONFIRM_EXIT, m_bConfirmExit);
	DDX_Check(pDX, IDC_SHOW_TIP_AT_STARTUP, m_bShowTipOfDay);
	DDX_Radio(pDX, IDC_SHOW_TIPS, m_nShowTips);
	DDX_Check(pDX, IDC_CONFIRM_DISCONNECT, m_bConfirmSignOff);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CAppGeneralPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CAppGeneralPrefs)
	ON_BN_CLICKED(IDC_START_ONLINE, OnBtnClick)
	ON_BN_CLICKED(IDC_SHOW_MAIN_MENU, OnBtnClick)
	ON_BN_CLICKED(IDC_SHOW_TIP_AT_STARTUP, OnBtnClick)
	ON_BN_CLICKED(IDC_CONFIRM_EXIT, OnBtnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAppGeneralPrefs message handlers

void CAppGeneralPrefs::Serialize(BOOL IsStoring)
{		
	if (IsStoring)
	{
		m_pApp = (CInternetNavApp*)AfxGetApp();
		m_pApp->SetStartOnline(m_bStartOnline);
		m_pApp->SetShowMainMenu(m_bShowMainMenu);
	    m_pApp->SetConfirmExit(m_bConfirmExit);
	    m_pApp->SetShowTOD(m_bShowTipOfDay);
	    m_pApp->SetShowTips(m_nShowTips); 
	    m_pApp->SetConfirmSignOff(m_bConfirmSignOff);
		m_pApp->Serialize(TRUE);
	}
	else if (!IsStoring)
	{                
		m_pApp->Serialize(FALSE);
		m_bStartOnline = m_pApp->StartOnline();
		m_bShowMainMenu = m_pApp->IsShowMainMenu();
        m_bConfirmExit = m_pApp->IsConfirmExit();   
        m_nShowTips = m_pApp->GetShowTips();
		m_bShowTipOfDay = m_pApp->IsShowTOD();         
		m_bConfirmSignOff = m_pApp->IsConfirmSignOff();
	}	
}                                    

void CAppGeneralPrefs::OnBtnClick()
{ 
	if (!UpdateData(TRUE))
		return;

	m_btnShowTipsApp.EnableWindow(m_bShowTipOfDay);
	m_btnShowTipsInet.EnableWindow(m_bShowTipOfDay);
}

BOOL CAppGeneralPrefs::OnSetActive()
{
	// This function is called when this page is created and also when
	// this page is the one the user has selected.       
	
	if (!CPropertyPage::OnSetActive())
	{
		AfxMessageBox("Unable to move to or open the selected tab.");
		return FALSE;                                                
	}

	return TRUE;
}

BOOL CAppGeneralPrefs::OnKillActive()
{
	// This function is called when this page is hidden because another
	// page has been selected by the user, or if the OK or Apply Now buttons
	// are pressed.
	
	if (!CPropertyPage::OnKillActive())
		return FALSE;

	return TRUE;
}

BOOL CAppGeneralPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}
    
    OnBtnClick();
    
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CInetFingerPrefs property sheet


CInetFingerPrefs::CInetFingerPrefs()
	: CPropertyPage(CInetFingerPrefs::IDD)
{
	//{{AFX_DATA_INIT(CInetFingerPrefs)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CInetFingerPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetFingerPrefs)
	DDX_Check(pDX, IDC_ASK_FINGER_ADDR, m_bAskFingerAddr);
	DDX_Check(pDX, IDC_AUTO_SAVE, m_bAutoFingerSave);
	DDX_Check(pDX, IDC_AUTO_PRINT, m_bAutoFingerPrint);
	DDX_Check(pDX, IDC_REPORT_QUERY_PROGRESS, m_bFingerRQP);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CInetFingerPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CInetFingerPrefs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInetFingerPrefs message handlers

BOOL CInetFingerPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CInetFingerPrefs::OnSetActive()
{
	// This function is called when this page is created and also when
	// this page is the one the user has selected.       
	
	if (!CPropertyPage::OnSetActive())
	{
		AfxMessageBox("Unable to move to or open the selected tab.");
		return FALSE;                                                
	}

	return TRUE;
}

BOOL CInetFingerPrefs::OnKillActive()
{
	// This function is called when this page is hidden because another
	// page has been selected by the user, or if the OK or Apply Now buttons
	// are pressed.
	
	if (!CPropertyPage::OnKillActive())
		return FALSE;

	return TRUE;
}

void CInetFingerPrefs::Serialize(BOOL IsStoring)
{
	if (IsStoring)
	{
		m_pApp->SetAskFingerAddr(m_bAskFingerAddr);
		m_pApp->SetFingerReportQueryProgress(m_bFingerRQP);
		m_pApp->SetAutoFingerSave(m_bAutoFingerSave);
		m_pApp->SetAutoFingerPrint(m_bAutoFingerPrint);
		m_pApp->Serialize(TRUE);	// write app's new info to INI file	
	}
	else
	{                
		m_pApp->Serialize(FALSE);	// refresh app's data from INI file
		m_bAskFingerAddr = m_pApp->IsAskFingerAddr(); 
		m_bFingerRQP = m_pApp->IsFingerReportQueryProgress();
		m_bAutoFingerSave = m_pApp->IsAutoFingerSave();
		m_bAutoFingerPrint = m_pApp->IsAutoFingerPrint();
	}	
} 
                                   
/////////////////////////////////////////////////////////////////////////////
// CInetWhoisPrefs property page


CInetWhoisPrefs::CInetWhoisPrefs() : CPropertyPage(CInetWhoisPrefs::IDD)
{
	//{{AFX_DATA_INIT(CInetWhoisPrefs)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CInetWhoisPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetWhoisPrefs)
	DDX_Check(pDX, IDC_REPORT_QUERY_PROGRESS, m_bReportQueryProgress);
	DDX_Check(pDX, IDC_AUTO_SAVE, m_bAutoSave);
	DDX_Check(pDX, IDC_AUTO_PRINT, m_bAutoPrint);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInetWhoisPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CInetWhoisPrefs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInetWhoisPrefs message handlers

BOOL CInetWhoisPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CInetWhoisPrefs::OnSetActive()
{
	// This function is called when this page is created and also when
	// this page is the one the user has selected.       
	
	if (!CPropertyPage::OnSetActive())
	{
		AfxMessageBox("Unable to move to or open the selected tab.");
		return FALSE;                                                
	}

	return TRUE;
}

BOOL CInetWhoisPrefs::OnKillActive()
{
	// This function is called when this page is hidden because another
	// page has been selected by the user, or if the OK or Apply Now buttons
	// are pressed.
	
	if (!CPropertyPage::OnKillActive())
		return FALSE;

	return TRUE;
}

void CInetWhoisPrefs::Serialize(BOOL IsStoring)
{
	if (IsStoring)
	{
		m_pApp->SetWhoisRQP(m_bReportQueryProgress);
		m_pApp->SetAutoWhoisSave(m_bAutoSave);
		m_pApp->SetAutoWhoisPrint(m_bAutoPrint);
		m_pApp->Serialize(TRUE);
	}
	else
	{                
		m_pApp->Serialize(FALSE);
		m_bReportQueryProgress = m_pApp->IsWhoisRQP();
		m_bAutoSave = m_pApp->IsAutoWhoisSave();
		m_bAutoPrint = m_pApp->IsAutoWhoisPrint();
	}	
}      

/////////////////////////////////////////////////////////////////////////////
//  CFtpPropsSheet property sheet                          

BEGIN_MESSAGE_MAP(CFtpPropsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CFtpPropsSheet)
	ON_WM_CREATE()  
	ON_BN_CLICKED(ID_HELP, CWnd::OnHelp)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFtpPropsSheet::CFtpPropsSheet(CWnd* pParentWnd /*=NULL*/)
	: CPropertySheet("FTP Site Properties", pParentWnd, 0)
{      
	AddPage(&m_pgFtpPropsGeneral); 
	AddPage(&m_pgFtpPropsFiles);
}

BOOL CFtpPropsSheet::CreateStandardButtons()
{
	for (int i = 0; 
		i < sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
	{
		// Note: If one of these _AfxLoadString calls fail, it is likely that
		//  _AFX_NO_PROPERTY_RESOURCES is defined in your .RC file.
		// To correct the situation, remove the following line from your
		//  resource script:
		//      #define _AFX_NO_PROPERTY_RESOURCES
		// This should be done using the Resource.Set Includes... command.

		// load the caption
		CString strCaption = "";
		VERIFY(strCaption.LoadString(AFX_IDS_PS_OK+i) != 0);
		char* lpsz = strstr(strCaption, "\n");                    
		if (lpsz != NULL)
			*lpsz = '\0';

		// create the control
		HWND hWnd = ::CreateWindow(_T("button"), strCaption,
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP|BS_PUSHBUTTON,
			0, 0, 0, 0, m_hWnd, (HMENU)optionsButtons[i],
			AfxGetInstanceHandle(), NULL);
		if (hWnd == NULL)
		{
			TRACE0("Warning: failed to create options buttons\n");
			return FALSE;
		}

		// set the font
		if (m_hFont != NULL)
			::SendMessage(hWnd, WM_SETFONT, (WPARAM)m_hFont, 0);
	}
 
 	UpdateDialogControls(this, FALSE);

	return TRUE;
}

int CFtpPropsSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    
    // subclass this window with CTL3D
    if (((CInternetNavApp*)AfxGetApp())->IsUseCTL3D())
    	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);  
 
	// fix-up the system menu so this looks like a dialog box
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	ASSERT(pSysMenu != NULL);
	int i, nCount = pSysMenu->GetMenuItemCount();
	for (i = 0; i < nCount; i++)
	{
		UINT nID = pSysMenu->GetMenuItemID(i);
		if (nID != SC_MOVE && nID != SC_CLOSE)
		{
			pSysMenu->DeleteMenu(i, MF_BYPOSITION);
			i--;
			nCount--;
		}
	}

	// set active page and active tab
	SetActivePage(m_nCurPage);

	// initialize font used for buttons
	ASSERT(m_hFont == NULL);
	CPropertyPage* pPage = GetActivePage();
	ASSERT_VALID(pPage);

	HFONT hFont = (HFONT)pPage->SendMessage(WM_GETFONT);
	if (hFont != NULL)
	{
		LOGFONT logFont;
		VERIFY(::GetObject(hFont, sizeof(LOGFONT), &logFont));
		m_hFont = CreateFontIndirect(&logFont);
	}

	// create the tab control itself
	CRect rect(0, 0, 100, 10);
	if (!m_tabRow.Create(WS_GROUP|WS_TABSTOP|WS_CHILD|WS_VISIBLE|
		(m_bStacked ? TCS_MULTILINE : 0), rect, this, AFX_IDC_TAB_CONTROL))
	{
		return -1;
	}

	// set page's z-order correctly (side effect of SetActivePage)
	SetActivePage(m_nCurPage);

	// calculate button sizes and separator
	rect.right = 50;    // slightly smaller buttons than normal
	rect.bottom = 14;
	rect.left = 4;      // button gap is 4 dialog units
	pPage->MapDialogRect(rect);
	m_sizeButton.cx = rect.right;
	m_sizeButton.cy = rect.bottom;
	m_cxButtonGap = rect.left;

	// calculate tab margin area
	rect.bottom = rect.right = 4;   // std dialog margin is 6 dialog units
	pPage->MapDialogRect(rect);
	m_sizeTabMargin.cx = rect.right;
	m_sizeTabMargin.cy = rect.bottom;

	// create standard buttons
	if (!CFtpPropsSheet::CreateStandardButtons())
		return -1;

	CFtpPropsSheet::RecalcLayout();
	m_tabRow.SetFocus();
	m_tabRow.SetCurSel(m_nCurPage);
	CenterWindow(m_pParentWnd); 

	return 0;   // success
}

void CFtpPropsSheet::OnClose()
{
	CPropertySheet::OnClose();
}

void CFtpPropsSheet::RecalcLayout()
{
	// determine size of the active page (active page determines initial size)
	CRect rectPage;
	GetActivePage()->GetWindowRect(rectPage);
	int nWidth = 2 * m_sizeTabMargin.cx + rectPage.Width() + 3;

	// determine total size of the buttons
	int cxButtons[sizeof(optionsButtons)/sizeof(optionsButtons[0])];
	int cxButtonTotal = 0;
	int cxButtonGap = 0;
	if (!m_bModeless)
	{
		for (int i = 0; 
			i < sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
		{
			cxButtons[i] = m_sizeButton.cx;

			// load the button caption information (may contain button size info)
			CString strTemp = "";
			VERIFY(strTemp.LoadString(AFX_IDS_PS_OK+i) != 0);

			// format is Apply\n50 (ie. text\nCX)
			char* lpsz = strstr(strTemp, "\n");
			if (lpsz != NULL)
			{
				// convert CX fields from text dialog units to binary pixels
				CRect rect(0, 0, 0, 0);
				rect.right = _ttoi(lpsz+1);
				GetActivePage()->MapDialogRect(&rect);
				cxButtons[i] = rect.Width();
			}
			if (::GetDlgItem(m_hWnd, optionsButtons[i]) != NULL)
			{
				cxButtonTotal += cxButtons[i];
				cxButtonGap += m_cxButtonGap;
			}
		}
	}
	if (cxButtonGap != 0)
		cxButtonGap -= m_cxButtonGap;

	// margin OK buttonGap Cancel buttonGap Help margin
	// margin is same as tab margin
	// button sizes are totaled in cxButtonTotal + cxButtonGap
	nWidth = max(nWidth, 2*m_sizeTabMargin.cx + cxButtonTotal + cxButtonGap);

	m_tabRow.SetWindowPos(NULL, m_sizeTabMargin.cx, m_sizeTabMargin.cy,
		nWidth - m_sizeTabMargin.cx*2, 0, SWP_NOACTIVATE|SWP_NOZORDER);
	CRect rectTabRow;
	m_tabRow.GetWindowRect(&rectTabRow);
	int nTabHeight = rectTabRow.Height();

	int nHeight = 2 * m_sizeTabMargin.cy + rectPage.Height() + nTabHeight + 4
		+ m_sizeTabMargin.cy + m_sizeButton.cy; // leave room for buttons

	CRect rectSheet(0, 0, nWidth, nHeight);
	CRect rectClient = rectSheet;
	::AdjustWindowRectEx(rectSheet, GetStyle(), FALSE, GetExStyle());

	SetWindowPos(NULL, 0, 0, rectSheet.Width(), rectSheet.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	CenterWindow();

	GetActivePage()->SetWindowPos(NULL,
		m_sizeTabMargin.cx+1, m_sizeTabMargin.cy + nTabHeight,
		nWidth - m_sizeTabMargin.cx*2 - 3, rectPage.Height(),
		SWP_NOACTIVATE | SWP_NOZORDER);

	if (!m_bModeless)
	{
		int x = nWidth - m_sizeTabMargin.cx - cxButtonTotal - cxButtonGap;
		int y = (nHeight - m_sizeTabMargin.cy) - m_sizeButton.cy;
		for (int i = 0; i < 
			sizeof(optionsButtons)/sizeof(optionsButtons[0]); i++)
		{
			HWND hWnd = ::GetDlgItem(m_hWnd, optionsButtons[i]);
			if (hWnd != NULL)
			{
				::MoveWindow(hWnd, x, y, cxButtons[i], m_sizeButton.cy, TRUE);
				x += cxButtons[i] + m_cxButtonGap;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFtpPropsGeneral property page


CFtpPropsGeneral::CFtpPropsGeneral() : CPropertyPage(CFtpPropsGeneral::IDD)
{
	//{{AFX_DATA_INIT(CFtpPropsGeneral)
	m_strPassword = "";
	m_strSiteAddress = "";
	m_strUserID = "";
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CFtpPropsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpPropsGeneral)
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_SITE_NAME, m_strSiteAddress);
	DDX_Text(pDX, IDC_USER_ID, m_strUserID);
	//}}AFX_DATA_MAP    
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CFtpPropsGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CFtpPropsGeneral)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFtpPropsGeneral message handlers

BOOL CFtpPropsGeneral::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}
/////////////////////////////////////////////////////////////////////////////
// CFtpFilesProps property page


CFtpFilesProps::CFtpFilesProps() : CPropertyPage(CFtpFilesProps::IDD)
{
	//{{AFX_DATA_INIT(CFtpFilesProps)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CFtpFilesProps::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpFilesProps)
	DDX_Text(pDX, IDC_FILE_NAME, m_strFileName);
	DDX_Text(pDX, IDC_FILE_PATH, m_strFilePath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFtpFilesProps, CPropertyPage)
	//{{AFX_MSG_MAP(CFtpFilesProps)
	ON_BN_CLICKED(IDC_SET_DOWNLOAD_DIR, OnSetDownloadDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFtpFilesProps message handlers

void CFtpFilesProps::OnSetDownloadDir()
{   
	CString strDir1 = m_pApp->GetFTPDownloadDir();
	CString strDir2 = m_pApp->DoDirectoryDialog(this, strDir1);
	m_pApp->SetFTPDownloadDir(strDir2);
	return;
}

BOOL CFtpFilesProps::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
/////////////////////////////////////////////////////////////////////////////
// CInetFTPPrefs dialog


CInetFTPPrefs::CInetFTPPrefs() : CPropertyPage(CInetFTPPrefs::IDD)
{
	//{{AFX_DATA_INIT(CInetFTPPrefs)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CInetFTPPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetFTPPrefs)
	DDX_Check(pDX, IDC_OPEN_ASCII, m_bOpenASCII);
	DDX_Check(pDX, IDC_OPEN_BIN, m_bOpenBin);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInetFTPPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CInetFTPPrefs)
	ON_BN_CLICKED(IDC_ADVANCED, OnAdvanced)
	ON_BN_CLICKED(IDC_VIRUSES, OnViruses)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInetFTPPrefs message handlers

BOOL CInetFTPPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CInetFTPPrefs::Serialize(BOOL IsStoring)
{
	if (IsStoring)
	{
		m_pApp->SetFTPOpenASCII(m_bOpenASCII);
		m_pApp->SetFTPOpenBin(m_bOpenBin);
		m_pApp->SetWarnFTPOpenBin(TRUE);
		m_pApp->Serialize(IsStoring);
	}
	else
	{                
		m_pApp->Serialize(IsStoring);
		m_bOpenASCII = m_pApp->IsFTPOpenASCII();
		m_bOpenBin = m_pApp->IsFTPOpenBin();
	}	
}      

void CInetFTPPrefs::OnAdvanced()
{
	CFtpPrefsAdvanced theDialog((CPropertySheet*)m_pParentWnd);
	theDialog.DoModal();
}

void CInetFTPPrefs::OnViruses()
{
	CFtpViruses theDialog((CPropertySheet*)m_pParentWnd);
	theDialog.m_strPath = m_pApp->GetVirusScannerPath();
	
	theDialog.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAppAreasPrefs dialog


CAppAreasPrefs::CAppAreasPrefs() : CPropertyPage(CAppAreasPrefs::IDD)
{
	//{{AFX_DATA_INIT(CAppAreasPrefs)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CAppAreasPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppAreasPrefs)
	DDX_Check(pDX, IDC_STARTUP_CHECK_MAIL, m_bStartCheckMail);
	DDX_Text(pDX, IDC_TIME_OUT, m_nTimeOut);
	DDV_MinMaxUInt(pDX, m_nTimeOut, 20, 32676);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAppAreasPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CAppAreasPrefs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAppAreasPrefs message handlers

BOOL CAppAreasPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAppAreasPrefs::Serialize(BOOL IsStoring)
{
	if (IsStoring)
	{
		m_pApp->SetCheckMailStartup(m_bStartCheckMail);
		m_pApp->SetInetTimeOut(m_nTimeOut);
		m_pApp->Serialize(TRUE); 
	}                           
	else
	{         
		m_pApp->Serialize(FALSE);
		m_bStartCheckMail = m_pApp->IsCheckMailStartup();
		m_nTimeOut = m_pApp->GetInetTimeOut();
	}  
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CInetMailPrefs dialog


CInetMailPrefs::CInetMailPrefs()
	: CPropertyPage(CInetMailPrefs::IDD)
{
	//{{AFX_DATA_INIT(CInetMailPrefs)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CInetMailPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetMailPrefs)
	DDX_Check(pDX, IDC_SEND_CLOSE, m_bSendClose);
	DDX_Check(pDX, IDC_SEND_CONFIRM, m_bSendConfirm);
	DDX_Check(pDX, IDC_SETUP_NEW_MAILBOX, m_bSetupNew);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInetMailPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CInetMailPrefs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInetMailPrefs message handlers

BOOL CInetMailPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}  

void CInetMailPrefs::Serialize(BOOL IsStoring)
{
	if (IsStoring)
	{
		m_pApp->SetMailSetupNew(m_bSetupNew);
		m_pApp->SetMailSendConfirm(m_bSendConfirm);
		m_pApp->SetMailSendClose(m_bSendClose);
		m_pApp->Serialize(IsStoring);
	}
	else
	{
		m_pApp->Serialize(IsStoring);
		m_bSetupNew = m_pApp->IsMailSetupNew();
		m_bSendConfirm = m_pApp->IsMailSendConfirm();
		m_bSendClose = m_pApp->IsMailSendClose();
	}
}                               

/////////////////////////////////////////////////////////////////////////////
// CInetGopherPrefs property page


CInetGopherPrefs::CInetGopherPrefs() : CPropertyPage(CInetGopherPrefs::IDD)
{
	//{{AFX_DATA_INIT(CInetGopherPrefs)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CInetGopherPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetGopherPrefs)
	DDX_Text(pDX, IDC_GOPHER_DOC_DIR, m_strGopherDocDir);
	DDX_Check(pDX, IDC_OPEN_ON_DOWNLOAD, m_bAutoOpenFile);
	DDX_Check(pDX, IDC_SHOW_SAVE_AS, m_bShowSaveAs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInetGopherPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CInetGopherPrefs)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInetGopherPrefs message handlers

void CInetGopherPrefs::OnBrowse()
{ 
	CString strInputDir, strOutputDir;
	
	if (!UpdateData(TRUE))
		return;
	
	if (!m_strGopherDocDir.IsEmpty())
	{
		// Use the directory string to initialize dir dlg 
		strInputDir = m_strGopherDocDir;
	}
	else
	{
		// Use application class's default value to initialize dir dlg
		strInputDir = m_pApp->GetGopherDocDir();
	}
	
	strOutputDir = m_pApp->DoDirectoryDialog(this, strInputDir);
	
	m_strGopherDocDir = strOutputDir;
	UpdateData(FALSE);
}

BOOL CInetGopherPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInetGopherPrefs::Serialize(BOOL IsStoring)
{
	if (IsStoring)
	{
		m_pApp->SetGopherShowSaveAs(m_bShowSaveAs);
		m_pApp->SetDisplayGopherDocs(m_bAutoOpenFile);
		m_pApp->SetGopherDocDir(m_strGopherDocDir);
		m_pApp->Serialize(IsStoring);
	}
	else
	{
		m_pApp->Serialize(IsStoring);
		m_bShowSaveAs = m_pApp->IsGopherShowSaveAs();
		m_bAutoOpenFile = m_pApp->IsDisplayGopherDocs();
		m_strGopherDocDir = m_pApp->GetGopherDocDir();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInetChatPrefs property page


CInetChatPrefs::CInetChatPrefs() : CPropertyPage(CInetChatPrefs::IDD)
{
	//{{AFX_DATA_INIT(CInetChatPrefs)    
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CInetChatPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetChatPrefs)
	DDX_CBString(pDX, IDC_IRC_SERVER, m_strServer);
	DDX_CBString(pDX, IDC_LOCAL_HOST, m_strLocal);
	DDX_Check(pDX, IDC_NOTIFY_ARRIVE, m_bNotifyA);
	DDX_Check(pDX, IDC_NOTIFY_LEAVE, m_bNotifyL);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInetChatPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CInetChatPrefs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInetChatPrefs message handlers


BOOL CInetChatPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInetChatPrefs::Serialize(BOOL IsStoring)
{
	if (IsStoring)
	{
		m_pApp->SetChatNotifyArrive(m_bNotifyA);
		m_pApp->SetChatNotifyLeave(m_bNotifyL);
		m_pApp->SetLocalHostID(m_strLocal);
		m_pApp->SetOnlineHostID(m_strServer);
		m_pApp->Serialize(IsStoring);
	}
	else
	{
		m_pApp->Serialize(IsStoring);
		m_bNotifyA = m_pApp->IsChatNotifyArrive();
		m_bNotifyL = m_pApp->IsChatNotifyLeave();
		m_strLocal = m_pApp->GetLocalHostID();
		m_strServer = m_pApp->GetOnlineHostID();
	}
}
/////////////////////////////////////////////////////////////////////////////
// CInetNewsPrefs property page


CInetNewsPrefs::CInetNewsPrefs() : CPropertyPage(CInetNewsPrefs::IDD)
{
	//{{AFX_DATA_INIT(CInetNewsPrefs)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CInetNewsPrefs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetNewsPrefs)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInetNewsPrefs, CPropertyPage)
	//{{AFX_MSG_MAP(CInetNewsPrefs)
	ON_BN_CLICKED(ID_ARTICLE_AGE, OnArticleAge)
	ON_BN_CLICKED(ID_ARTICLE_OPTIONS, OnArticleOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInetNewsPrefs message handlers

BOOL CInetNewsPrefs::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInetNewsPrefs::OnArticleAge()
{
	CArticleAge theDlg((CPropertySheet*)m_pParentWnd);
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

void CInetNewsPrefs::OnArticleOptions()
{
	m_pApp->Serialize(FALSE);
	CArticleOptions theDialog((CPropertySheet*)m_pParentWnd);
	theDialog.m_nDisplay = m_pApp->GetNNTPArticleDisplay();
	
	if (theDialog.DoModal() == IDOK)
		m_pApp->SetNNTPArticleDisplay(theDialog.m_nDisplay);
	m_pApp->Serialize(TRUE);
}
