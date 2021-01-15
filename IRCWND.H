// ircwnd.h : header file
//

#ifndef __IRCWND_H__
#define __IRCWND_H__
#include "ircview.h"
#include "statbar.h"

/////////////////////////////////////////////////////////////////////////////
// CIRClientWnd frame

class CIRClientWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CIRClientWnd)
protected:
	CIRClientWnd();			// protected constructor used by dynamic creation

// Attributes
public:  
	CIRClientView *m_pServerView, *m_pClientView;
	CInternetNavApp* m_pApp;
protected:
	CSplitterWnd m_wndSplitter;
	CBitmapButton m_btnRuler;
	CDialogBar m_wndRulerBar; 
	CInetnavStatusBar* m_pStatusBar;

// Operations
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext);
		
// Implementation
protected:
	virtual ~CIRClientWnd();

	// Generated message map functions
	//{{AFX_MSG(CIRClientWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__IRCWND_H__
