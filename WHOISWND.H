// whoiswnd.h : header file
//

#ifndef __WHOISWND_H__
#define __WHOISWND_H__

/////////////////////////////////////////////////////////////////////////////
// CWhoisWnd frame

class CWhoisWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CWhoisWnd)
protected:
	CWhoisWnd();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child
// Implementation
protected:
	virtual ~CWhoisWnd();

	// Generated message map functions
	//{{AFX_MSG(CWhoisWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__WHOISWND_H__