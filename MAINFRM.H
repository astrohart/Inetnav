// mainfrm.h : interface of the CMainFrame class
//

#ifndef __MAINFRM_H__
#define __MAINFRM_H__ 
#include "statbar.h" 
#include "tooltipb.h"
/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CMDIFrameWnd
{    
friend class CInternetNavApp;
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CInternetNavApp* m_pApp;
	BOOL m_bCreateToolBar, m_bCreateStatBar;

// IRC Functions and Helpers:
private:
	void AddToReceive(const char* pszBuffer, int nAmt);
	CStringList m_ReceiveLines;
	CString m_strRemainingReceive;

// Operations
public:
	// for resetting the Places toolbar after documents and clients and
	// windows have changed the toolbar to suit their own needs
	void ResetToolBar();       
	
	// for putting IRC messages in the appropriate windows
	void PutInActive(CString& strResponse);
	void PutInChanWindow(const CString& strChannel, CString& strMessage);
	
	// Other IRC Operations
	void IRCBeginChat();

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif   
	void IRCParseResponse(); // parses responses from IRC server

protected:  // control bar embedded members
	CInetnavStatusBar  m_wndStatusBar;
	CToolTipBar    	   m_wndToolBar;
public:
	CInetnavStatusBar* GetStatusBar() { return &m_wndStatusBar; }
	CToolTipBar* GetToolBar() { return &m_wndToolBar; }

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange();
	afx_msg void OnClose();
	afx_msg void OnWindowCloseAll();
	afx_msg void OnUpdateIndicatorOffline(CCmdUI* pCmdUI); 
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnHelpSearch();
	//}}AFX_MSG
	afx_msg void OnDefaultHelp();
	afx_msg LRESULT OnIRCSocketResponse(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__MAINFRM_H__