 //
//  Copyright (C) 1994 by
//  Todd Osborne, St. Louis, MO, USA
//  All Rights Reserved.
//
//  This software is furnished under the Public Domain.
//      You may freely copy, modify and distribute any or all portions of this
//      code.
//
//  Todd Osborne assumes no responsibility for
//  the use or reliability of this software.
//
//  FACILITY: ToolTips and FlyBy Support for MFC CToolBar derived class
//
//  ABSTRACT:
//
//     The CToolTipBar is a direct replacement for classes derived or                  
//     instantiated from the MFC class CToolBar. To implement all                       
//     functionality, simply change your derivation or instantiation from              
//     CToolBar to CToolTipBar. For extended usage, see header file.                    
//
//  AUTHOR(S): Todd Osborne INTERNET: toddos@aol.com
//
//  CREATED: 06/27/94
//
//  MODIFICATION HISTORY:
//
//  Version Date        By              Reason
//      09/27/94        Brian Hart      Needed to include more header files from
//                                      project in order for this file to
//                                      be successfully built.
//		04/19/97		Brian Hart		Added code to turn off tool tips when
//										needed
//

#include "stdafx.h"
#include "resource.h"
#include "inetnav.h"
#include "tooltipb.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////

CToolTipPopup::CToolTipPopup(CWnd* pParentWnd)
{
	// Initialize member variables
	m_nShowTip =    0;
	m_strText =     "";
	m_pParentWnd = (CToolBar*)pParentWnd;
	m_pFont =               NULL;
	m_nClearance =  2;  
	m_hBrush =              ::CreateSolidBrush(RGB(255, 255, 128));
	
	// Create the popup-window
	CreateEx(0, AfxRegisterWndClass(0, 0, m_hBrush), NULL, WS_POPUP | WS_CLIPSIBLINGS | WS_BORDER, 0, 0, 0, 0, pParentWnd->m_hWnd, 0);
}

CToolTipPopup::~CToolTipPopup()
{
	// Clean up
	if ( m_pFont )
	{
		m_pFont->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}
	
	if ( m_hBrush )
	{
		::DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}
}

int CToolTipPopup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 )
		return -1;
		
	// Create the font to use in popup windows. Initially a 9 point Arial
	LOGFONT         lf;
	CClientDC       dc(this);
	int             nFontSize = -1 * (::GetDeviceCaps(dc.m_hDC, LOGPIXELSY) * 9/*9 is the initial point size*/ / 72);
			
	m_pFont = new CFont();
	
	lf.lfHeight = nFontSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_REGULAR;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;                 
	lf.lfCharSet = ANSI_CHARSET;                              
	lf.lfOutPrecision = OUT_TT_PRECIS;
	lf.lfClipPrecision = CLIP_TT_ALWAYS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = FF_MODERN;
	lstrcpy(lf.lfFaceName, "Arial");
	
	CToolTipPopup::SetFont(&lf);
						
	return 0;       
}
						
void CToolTipPopup::OnPaint()
{
	CPaintDC dc(this);
	
	// Set font
	CFont* pOldFont = dc.SelectObject(m_pFont);
	
	// Get rect
	RECT rect;
	GetClientRect(&rect);
	
	// Draw the text in black
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 0, 0));
	dc.DrawText(m_strText, m_strText.GetLength(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	
	// Clean-up
	dc.SelectObject(pOldFont);
}

BOOL CToolTipPopup::SetFont(const LOGFONT* pLogFont)
{
	// Set m_pFont to pLogFont (passed in from CToolTipBar member function)
	
	// If this font exists, delete it first
	if ( m_pFont )
		m_pFont->DeleteObject();
		
	BOOL bCreated;
	
	if ( !(bCreated = m_pFont->CreateFontIndirect(pLogFont)) )
		m_pFont = NULL;
		
	return bCreated;        
}

void CToolTipPopup::ShowTip(UINT nID, UINT x)
{
	// Show the tool tip window, displaying the text from the menu identified by nID
	// post WM_INITMENUPOPUP message to main frame window so menu items
	// get updated
							     
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	// If this tip is already showing, exit now
	if ( nID == m_nShowTip )
		return;
		
	m_nShowTip = nID;
	
	// Attempt to get menu item text (This will become the popup ToolTip text)
	char szMenuText[50];
	CMenu* pMenu = pApp->GetMainFrame()->GetMenu();
	if ( pMenu != NULL && pMenu->GetMenuString(m_nShowTip, szMenuText, sizeof(szMenuText), MF_BYCOMMAND) > 0 )
	{
		// We have the menu text. "Fix" string. NULL terminate, remove & and tabs and ... chars
		szMenuText[sizeof(szMenuText) - 1] = '\0';
		
		// Remove ellipses (...)
		char* psz = strstr(szMenuText, "...");
		if ( psz != NULL )
			*psz = '\0';
			
		// Remove tabs and tab escape sequences ('\t')
		psz = strchr(szMenuText, '\t');
		if ( psz != NULL )
			*psz = '\0';
			
		// and finally remove ampersands (&)
		char* ptr1 = szMenuText;
		char* ptr2 = szMenuText;
		while ( *ptr1 != '\0' )
		{
			if ( *ptr1 != '&' )
			{
				*ptr2 = *ptr1;
				++ptr2;
			}
			++ptr1;
		}
		*ptr2 = '\0';
						
		// Copy to CString object, if not one of the tips for the menu
		// commands whose UPDATE_COMMAND_UI handlers change their text.
		// For those menu commands, switch() on m_nShowTip below
		// and case on each <menuID>:, initializing m_strText to whatever the
		// UPDATE_COMMAND_UI handler would change the menu text to under the
		// same conditions.
		
		switch (m_nShowTip)
		{
			case ID_EDIT_CUT:
				m_strText = "Cut";
				break;
				
			case ID_EDIT_COPY:
				m_strText = "Copy";
				break;
			
			case ID_EDIT_PASTE:
				m_strText = "Paste";
				break;
				
			case ID_EDIT_UNDO:
				m_strText = "Undo";
				break;
			
			case ID_EDIT_REDO:
				m_strText = "Redo";
				break;
			
			case ID_FILE_PRINT:
				m_strText = "Print";
				break;
			
			case ID_FILE_PRINT_PREVIEW:
				m_strText = "Print Preview";
				break;
			
			default:
				m_strText = szMenuText;
				break;
		}
		
		// Determine size of popup window based on size of text
		CClientDC dc(this);
		CFont* pOldFont = dc.SelectObject(m_pFont);
		CSize size = dc.GetTextExtent(m_strText, lstrlen(m_strText));
			
		// Set tip window postion and make the window slightly larger than this font ( m_nClearance pixels in each direction )
		RECT rToolBar;
		m_pParentWnd->GetWindowRect(&rToolBar);
		SetWindowPos(&wndNoTopMost, x, rToolBar.top + (rToolBar.bottom - rToolBar.top), size.cx + (2 * m_nClearance), size.cy + (2 * m_nClearance), SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_SHOWWINDOW);
						
		// Clean-Up
		dc.SelectObject(pOldFont);
				
		// Invalidate this window and re-paint
		InvalidateRect(NULL);
		UpdateWindow();
	}
}
			  
void CToolTipPopup::HideCurrentTip()
{
	if ( IsWindowVisible() )
	{
		ShowWindow(SW_HIDE);
		m_nShowTip = 0;
	}
}

void CToolTipPopup::PostNcDestroy()
{
	if (this != NULL)
	{
		delete this;  // cleanup memory used by this window
	}
	return;
}

BEGIN_MESSAGE_MAP(CToolTipPopup, CWnd)
	//{{AFX_MSG_MAP(CToolTipPopup)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTipBar class: A toolbar with ToolTips

IMPLEMENT_DYNCREATE(CToolTipBar, CToolBar)

CToolTipBar::CToolTipBar()
	 : CToolBar()                  
{
	// Initialize member variables
	m_pToolTipPopup =       NULL;   // Popup not yet created
	m_bEnableToolTips =     FALSE;  // ToolTips are not enabled (until popup window is created)
    m_bEnableFlyBy =    TRUE;   // FlyBy's are enabled by default
    m_ptCurPos =                0, 0;   // No last cursor position
    m_nShowTip =                0;              // No current tip to be shown
    m_nWait =                   050;    // 50 milliseconds before ToolTip is shown
    m_bMouseDown =              FALSE;  // Left mouse button is not down to start with
    m_nShowX =                  0;              // No X pixel to show tip at
    m_pApp = (CInternetNavApp*)AfxGetApp();
    m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
}

CToolTipBar::~CToolTipBar()
{
	// Clean up
	/*if ( m_pToolTipPopup )
	{
		m_pToolTipPopup->DestroyWindow();
		m_pToolTipPopup = NULL;
	}*/
}

BOOL CToolTipBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	// Call base class first
	BOOL bRetVal = CToolBar::Create(pParentWnd, dwStyle, nID);
	
	// Create the popup tool tip window. It is not initially visible
	m_pToolTipPopup = new CToolTipPopup(this);
	
	// Verify memory was allocated and window was created
	if ( m_pToolTipPopup )
		m_bEnableToolTips = TRUE;
	
	Invalidate();
		
	// Return the return value for CToolBar::Create()
	return bRetVal;
}

void CToolTipBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Call base class first
	CToolBar::OnLButtonDown(nFlags, point);
	
	if (m_pApp->m_bToolTipsOff)
		return;
	
	// Always remove any ToolTips when a mouse button is pressed
	m_bMouseDown = TRUE;                          
	
	// Once the mouse button is pressed, the ToolTip is removed. Therefore
	// we can remove the command discription associated with the ToolTip
	// because once the tip is gone, it doesn't make sense to leave the command
	// discription in the status bar, so show the AFX_IDS_IDLEMESSAGE string.
	// The PostMessage() function below posts a special message to the main
	// window which tells it do display the AFX_IDS_IDLEMESSAGE string in
	// the status bar's first pane.
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_pStatusBar->SetPaneText(0, "For Help, press F1.", TRUE);
		
	if ( m_pToolTipPopup )
		m_pToolTipPopup->HideCurrentTip();
}

void CToolTipBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Call base class first
	CToolBar::OnLButtonUp(nFlags, point);
	
	if (m_pApp->m_bToolTipsOff)
		return;
		
	// Always remove any ToolTips when a mouse button is released
	m_bMouseDown = FALSE;
	if ( m_pToolTipPopup )
		m_pToolTipPopup->HideCurrentTip();
}

void CToolTipBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// Call base class first
	CToolBar::OnMouseMove(nFlags, point);
	
	if (m_pApp->m_bToolTipsOff)
		return;
		
	if (nFlags == MK_LBUTTON)
	{
		// Left button down -- button is pressed -- no ToolTip and no flyby
		m_bMouseDown = TRUE;   
		m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
		m_pStatusBar->SetPaneText(0, "For Help, press F1.", TRUE);
		if (m_pToolTipPopup)
			m_pToolTipPopup->HideCurrentTip();
		return;
	}
		
	
	// Get current button mouse is over. 0 if none
	UINT nID = GetMouseOverTip();
		
	if ( nID )
	{
		// Post WM_MENUSELECT to parent window. This allows updating of status bar
		if ( m_bEnableFlyBy )
		{       
			m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
			CString text = "";
			text.LoadString(nID);
			m_pStatusBar->SetPaneText(0, text, TRUE);
		}
		
		// Hide the current tip NOW because it is not the one the mouse is over
		if ( nID != m_nShowTip && m_pToolTipPopup )
		{
			m_pToolTipPopup->HideCurrentTip();
		}
			
		// Set m_nShowTip, x Pixel to display and timer
		if ( m_bEnableToolTips )
		{
			m_ptCurPos = point;
			ClientToScreen(&point);
			m_nShowTip = nID;
			m_nShowX =       point.x;
			// Set timer to fire in m_nWait milliseconds
			SetTimer(1, m_nWait, NULL);
			return;
		} 
	}
	else if (GetMouseOverTip() == 0)
	{
		// the mouse is NOT over a toolbar button, so just display the IDLE
		// message on the message line of the status bar.         
		m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
		m_pStatusBar->SetPaneText(0, "For Help, press F1.", TRUE);
	}
	
	// If we made it this far, we need to hide the current tip (if any)
	if ( m_pToolTipPopup )
		m_pToolTipPopup->HideCurrentTip();
}

void CToolTipBar::OnTimer(UINT nIDEvent)
{
	// We cannot show tips if any of the following are true:
	// 1) The ToolTipPopup window was not created
	// 2) The main window is not the currently enabled window
	// 3) The main window is not the active window
	// 4) The main window is an icon
	// 5) The left mouse button is down
	// Also, only show a ToolTip if the mouse has not moved since we set m_nShowTip in the
	// last WM_MOUSEMOVE event
	CMainFrame* pMain = m_pApp->GetMainFrame();
	if ( m_pToolTipPopup && pMain->IsWindowEnabled() && pMain->m_hWnd == ::GetActiveWindow() && !pMain->IsIconic() && !m_bMouseDown)
	{
		// Show tip if cursor has not moved since timer was set
		POINT ptCur;
		::GetCursorPos(&ptCur);
		ScreenToClient(&ptCur);
		if ( m_ptCurPos == ptCur )																										
			m_pToolTipPopup->ShowTip(m_nShowTip, m_nShowX);
	}
		
	// Always kill the timer
	KillTimer(nIDEvent);
}

LRESULT CToolTipBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	// This is our chance to hide a toolbar item when the mouse moves out of
	// the toolbar rect and we missed a WM_MOUSEMOVE message, or another app
	// was made active since we displayed a ToolTip
	
	if (m_pApp->m_bToolTipsOff)
		return CToolBar::OnIdleUpdateCmdUI(wParam, lParam);
		
	if ( m_pToolTipPopup )
	{
		POINT   ptCur;
		RECT    rToolBar;
		::GetCursorPos(&ptCur);
		GetWindowRect(&rToolBar);
		if ( !(::PtInRect(&rToolBar, ptCur)) || AfxGetMainWnd()->m_hWnd != ::GetActiveWindow() )
			m_pToolTipPopup->HideCurrentTip();
	}

	return CToolBar::OnIdleUpdateCmdUI(wParam, lParam);
}

BEGIN_MESSAGE_MAP(CToolTipBar, CToolBar)
	//{{AFX_MSG_MAP(CToolTipBar)
	ON_WM_TIMER()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***** SUPPORT ROUTINES ******

UINT CToolTipBar::GetMouseOverTip()
{
	// Return the menu ID the mouse is over (0 if none)
	RECT rItem;
	UINT nID = 0, nStyle = 0;
	int  nImage = 0;
	
	// Get the current mouse position
	POINT point;
	GetCursorPos(&point);
	
	// Get the toolbar rect
	RECT rToolBar;
	GetWindowRect(&rToolBar);
				
	for ( int i = 0; i < GetCount(); i++ )
	{
		GetItemRect(i, &rItem);
		ClientToScreen(&rItem);
		if ( ::PtInRect(&rItem, point) != 0 )
		{
			// We have found a item in the CToolBar that we are over
			// Is it a separator? (Values passed by reference)
			GetButtonInfo(i, nID, nStyle, nImage);
			if ( !(nStyle & TBBS_SEPARATOR) )
				return nID;
		}
	}
	
	// If we made it this far, we are not over a button
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// DoPaint() function - called for toolbar paint
// we want to make the toolbar look 3-D, like MS Visual Workbench's toolbar

void CToolTipBar::DoPaint(CDC* pDC)
{
	CPen penHilite, *pPenHilite, *pPenOld;

	// now do our own drawing
	
	penHilite.CreatePen(2, PS_SOLID, RGB(255,255,255));

	pPenHilite = &penHilite;
	
	pPenOld = pDC->SelectObject(pPenHilite);
	
	// draw with the pen
	CRect rect;
	GetClientRect(&rect);
	
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	
	pDC->SelectObject(pPenOld);

	CToolBar::DoPaint(pDC);
	
	return;
}


void CToolTipBar::OnSysColorChange()
{
	UpdateWindow();
	
	CToolBar::OnSysColorChange();
}
