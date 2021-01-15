// statbar.h : header file
//

#ifndef __STATBAR_H__
#define __STATBAR_H__

/////////////////////////////////////////////////////////////////////////////
// CInetnavStatusBar window

class CInetnavStatusBar : public CStatusBar
{
// Construction
public:
	CInetnavStatusBar();

// Attributes
public:

// Operations
public:      
	BOOL SetText(CString strText);
	BOOL SetText(UINT nIDResource);  
	
	// for setting the IDLE message
	void ShowIdleMessage() { SetText(AFX_IDS_IDLEMESSAGE); }

// Implementation
public:
	virtual ~CInetnavStatusBar();

protected:
	// Generated message map functions
	//{{AFX_MSG(CInetnavStatusBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__STATBAR_H__
