//
//  Copyright (C) 1994 by
//  Todd Osborne, St. Louis, MO, USA
//  All Rights Reserved.
//
//  This software is furnished under the Public Domain.
//	You may freely copy, modify and distribute any or all portions of this
// 	code.
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
//  Version Date        By      Reason
//

#ifndef __TOOLTIPB_H
#define __TOOLTIPB_H

#include "stdafx.h"
#include <afxpriv.h>	// MFC Private include. For WM_IDLEUPDATECMDUI 
#include "statbar.h"	// For pointer to status bar

// The class CToolTipPopup represents the small yellow popup window that
// displays the ToolTip. It is created when the CToolTipBar object is
// instantiated and is not destroyed until CToolTipBar is. Rather is is
// hidden. This increases speed by not having to create the popup window
// each time the user moves over a tool. When a tip is shown, this popup
// window will resize itself to fit around the text with spacing on all 4
// side equal to the size of the text + m_nClearance. This class is not set
// up to be a base class for anything else. Note the private declaration of
// all members with a friend class association with CTooTipBar.
class CToolTipPopup : public CWnd
{
friend class CToolTipBar;

// Construction/Destruction
private:
	CToolTipPopup(CWnd* pParentWnd);		
    ~CToolTipPopup();

// Attributes
private:   
  	HBRUSH		m_hBrush;				// Background brush (yellow)
	CToolBar*	m_pParentWnd;	// Pointer to owner window (the toolbar)
	CString		m_strWndClass;	// Custom window class for the popup tip
	CString 	m_strText;		// Text to be displayed when popup is shown
	CFont*		m_pFont;		// Font used to display tool tip text
	UINT		m_nShowTip;		// Tip that is currently showing (Equals menu item ID or 0 for none)
	UINT 		m_nClearance;	// Number of pixels to leave between text and edges of popup window

// Operations
private:
	void ShowTip(UINT nID, UINT x);			// Show a tool tip window
    void HideCurrentTip();					// Hide the current tip (if any)
    BOOL SetFont(const LOGFONT* pLogFont);	// Set the font for tips

// Implementation
protected: 
	virtual void PostNcDestroy(); // for cleanup of the this pointer
	// Generated message map functions
	//{{AFX_MSG(CToolTipPopup)
	afx_msg int 	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void 	OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// This class is the actual CToolBar derived class the you derive or use to
// instantiate your toolbar. You can use this object exactly the same way
// you would use CToolBar, plus it has extension that allow you to modify
// its behaviour. You can show enable/disable ToolTips and FlyBy's at run-time,
// as well as change the font that is displayed as the tooltip. By default, you
// cannot change the colors used for tooltips. By default, if you change your
// mainframe header from CToolBar m_wndToolBar to CToolTipBar m_wndToolBar, you
// will enable all features, including ToolTips and FlyBy's. The initial font will
// be a small Arial. If the CreateFontIndirect() call fails, the default system
// font will be used. If the popup window that displays the tooltips fails to
// create itself, ToolTips will be disabled, but FlyBy's will continue to work.
// The interface to CToolTipBar is commented below. No changes should be made
// the to CToolTipPopup class, but customization of CToolTipBar is easily made.
class CToolTipBar : public CToolBar
{
	DECLARE_DYNCREATE(CToolTipBar)

private:
    // Control memebers
    CToolTipPopup*	m_pToolTipPopup;	// Pointer to the popup window that displays the tip
    UINT			m_nShowTip;			// ID to show when ShowTip() is called
	UINT			m_nFlyBy;			// Last FlyBy menu ID sent to parent window
    UINT			m_nWait;			// Number of milliseconds to wait before showing tip (After mouse has stopped)
    UINT			m_nShowX;			// X pizel location to show the popup tip
    CPoint			m_ptCurPos;			// Cursor position when we decided to show a tool tip. Will be checked in the timer event
    BOOL			m_bMouseDown;		// Is the mouse left button down?
    BOOL			m_bEnableFlyBy;		// Should WM_MENUSELECT messages be sent for toolbar items?
    BOOL			m_bEnableToolTips;	// Should ToolTips be shown? 
    CInternetNavApp* m_pApp;
    CInetnavStatusBar* m_pStatusBar;

	UINT			GetMouseOverTip();	// Get the tip the mouse is over, if any (0 if none)
public:
	// Construction / Destruction
	CToolTipBar();				// Standard constructor
	~CToolTipBar();				// Standard destructor (Performs cleanup of this object and CToolTipPopup object)
    
    // Override default CToolBar::Create() to initialize this class. Will call base class
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,	UINT nID = AFX_IDW_TOOLBAR);
	        
    // Enable/Disable and Set/Get private members
    void EnableToolTips(BOOL bEnable=TRUE)	{ m_bEnableToolTips = bEnable; }
    BOOL GetEnableToolTips()			{ return m_bEnableToolTips; }
    void EnableFlyBy(BOOL bEnable=TRUE)		{ m_bEnableFlyBy = bEnable; }
    BOOL GetEnableFlyBy()				{ return m_bEnableFlyBy; }
    void SetClearance(UINT nClearance) 	{ if ( m_pToolTipPopup ) m_pToolTipPopup->m_nClearance = nClearance; }
    UINT GetClearance()					{ return (m_pToolTipPopup) ? m_pToolTipPopup->m_nClearance : 0; }
    void SetWait(UINT nWait)			{ m_nWait = nWait; }
	UINT GetWait()						{ return m_nWait; }
	
	// Set font in CToolTipPopup object
    BOOL SetFont(const LOGFONT* pLogFont) { return (m_pToolTipPopup) ? m_pToolTipPopup->SetFont(pLogFont) : FALSE; }
    
    // member variables for size
    // overriden from CToolBar
    CSize m_sizeButton;     // "size of button"	
    CSize m_sizeImage; 		// "size of glyph"

protected: 
	virtual void DoPaint(CDC* pDC); // called for drawing the toolbar
	// Generated message map functions
	//{{AFX_MSG(CToolTipBar)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif	// __TOOLTIPB_H
