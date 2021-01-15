// wizards.h : header file for the pages (steps) of Internet Navigator's
// Wizards
//

#ifndef __WIZARDS_H__
#define __WIZARDS_H__
#include "mail.h"
/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz1 dialog

class CMailBoxSetupWiz1 : public CDialog
{
// Construction
public:
	CMailBoxSetupWiz1(CWnd* pParent=NULL, CMailBoxDoc* pDoc=NULL);

// Dialog Data
	//{{AFX_DATA(CMailBoxSetupWiz1)
	enum { IDD = IDD_MAI_SETUP_1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	CMailData* m_pData; // pointer to data class for this wizard

// Implementation
protected:            
	/*virtual void OnSetFont(CFont* pFont);*/	// sets font for this dialog box
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailBoxSetupWiz1)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz2 dialog

class CMailBoxSetupWiz2 : public CDialog
{
// Construction
public:
	CMailBoxSetupWiz2(CWnd* pParent = NULL, CMailBoxDoc* pDoc = NULL);

// Dialog Data
	//{{AFX_DATA(CMailBoxSetupWiz2)
	enum { IDD = IDD_MAI_SETUP_2 };
	int		m_nWalkThrough;
	//}}AFX_DATA 
	CMailData* m_pData; // pointer to data class for this wizard

// Implementation
protected:                  
	/*virtual void OnSetFont(CFont* pFont);*/	// sets font for this dialog box
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailBoxSetupWiz2)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz3 dialog

class CMailBoxSetupWiz3 : public CDialog
{
// Construction
public:
	CMailBoxSetupWiz3(CWnd* pParent=NULL, CMailBoxDoc* pDoc=NULL);
// Dialog Data
	//{{AFX_DATA(CMailBoxSetupWiz3)
	enum { IDD = IDD_MAI_SETUP_3 };
	CString	m_strSMTPAddress;
	//}}AFX_DATA

protected:
	CMailData* m_pData; // pointer to data class for this wizard

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/	// sets font for this dialog box
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailBoxSetupWiz3)
	afx_msg void OnBack();
	afx_msg void OnEditChange();
	virtual void OnCancel();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz4 dialog

class CMailBoxSetupWiz4 : public CDialog
{
// Construction
public:
	CMailBoxSetupWiz4(CWnd* pParent=NULL, CMailBoxDoc* pDoc=NULL);	
	
// Dialog Data
	//{{AFX_DATA(CMailBoxSetupWiz4)
	enum { IDD = IDD_MAI_SETUP_4 };
	CString	m_strPOP3Host;
	//}}AFX_DATA

protected:
	CMailData* m_pData; // pointer to data class for this wizard

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/	// sets font for this dialog box
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailBoxSetupWiz4)
	afx_msg void OnEditChange();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz5 dialog

class CMailBoxSetupWiz5 : public CDialog
{
// Construction
public:
	CMailBoxSetupWiz5(CWnd* pParent, CMailBoxDoc* pDoc);

// Dialog Data
	//{{AFX_DATA(CMailBoxSetupWiz5)
	enum { IDD = IDD_MAI_SETUP_5 };
	CString	m_strPassword;
	CString	m_strPOP3Logon;
	//}}AFX_DATA   
	
	CMailBoxDoc* m_pDoc;
	CMailData* m_pData;

// Implementation
protected:
	/*virtual void OnSetFont(CFont* pFont);*/	// sets font for this dialog box
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailBoxSetupWiz5)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz6 dialog

class CMailBoxSetupWiz6 : public CDialog
{
// Construction
public:
	CMailBoxSetupWiz6(CWnd* pParent=NULL, CMailBoxDoc* pDoc=NULL);
// Dialog Data
	//{{AFX_DATA(CMailBoxSetupWiz6)
	enum { IDD = IDD_MAI_SETUP_6 };
	CString	m_strAddress;
	//}}AFX_DATA

protected:
	CMailData* m_pData; // pointer to data class for this wizard

// Implementation
protected: 
	/*virtual void OnSetFont(CFont* pFont);*/	// sets font for this dialog box
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailBoxSetupWiz6)
	afx_msg void OnBack();
	afx_msg void OnNext();
	virtual void OnCancel();
	afx_msg void OnEditChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz7 dialog

class CMailBoxSetupWiz7 : public CDialog
{
// Construction
public:
	CMailBoxSetupWiz7(CWnd* pParent=NULL, CMailBoxDoc* pDoc=NULL);

// Dialog Data
	//{{AFX_DATA(CMailBoxSetupWiz7)
	enum { IDD = IDD_MAI_SETUP_7 };
	//}}AFX_DATA

protected:
	CMailData* m_pData;
	
// Implementation
protected: 
	/*virtual void OnSetFont(CFont* pFont);*/	// sets font for this dialog box
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMailBoxSetupWiz7)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};    

/////////////////////////////////////////////////////////////////////////////
// CCompWiz1 dialog

class CCompWiz1 : public CDialog
{
// Construction
public:
	CCompWiz1(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CCompWiz1)
	enum { IDD = IDD_COMPMAIL_WIZ1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompWiz1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCompWiz2 dialog

class CCompWiz2 : public CDialog
{
// Construction
public:
	CCompWiz2(CWnd* pParent, CComposeView* pView);

// Dialog Data
	//{{AFX_DATA(CCompWiz2)
	enum { IDD = IDD_COMPMAIL_WIZ2 };
	CString	m_strSubject;
	//}}AFX_DATA 
	CComposeView* m_pComposeView;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompWiz2)
	afx_msg void OnBack();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCompWiz3 dialog

class CCompWiz3 : public CDialog
{
// Construction
public:
	CCompWiz3(CWnd* pParent, CComposeView* pView);

// Dialog Data
	//{{AFX_DATA(CCompWiz3)
	enum { IDD = IDD_COMPMAIL_WIZ3 };
	CString	m_strTo;
	//}}AFX_DATA
	CComposeView* m_pComposeView;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompWiz3)
	afx_msg void OnChoose();
	afx_msg void OnBack();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCompWiz4 dialog

class CCompWiz4 : public CDialog
{
// Construction
public:
	CCompWiz4(CWnd* pParent, CComposeView* pView);

// Dialog Data
	//{{AFX_DATA(CCompWiz4)
	enum { IDD = IDD_COMPMAIL_WIZ4 };
	CString	m_strCC;
	//}}AFX_DATA   
	CComposeView* m_pComposeView;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompWiz4)
	afx_msg void OnChoose();
	virtual void OnOK();
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCompWiz5 dialog

class CCompWiz5 : public CDialog
{
// Construction
public:
	CCompWiz5(CWnd* pParent, CComposeView* pView);	

// Dialog Data
	//{{AFX_DATA(CCompWiz5)
	enum { IDD = IDD_COMPMAIL_WIZ5 };
	CString	m_strBCC;
	//}}AFX_DATA    
	CComposeView* m_pComposeView;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompWiz5)
	virtual void OnOK();
	afx_msg void OnBack();
	afx_msg void OnChoose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CCompWizFinish dialog

class CCompWizFinish : public CDialog
{
// Construction
public:
	CCompWizFinish(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCompWizFinish)
	enum { IDD = IDD_COMPMAIL_WIZ6 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompWizFinish)
	afx_msg void OnBack();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz1 dialog

class CFeedBackWiz1 : public CDialog
{
// Construction
public:
	CFeedBackWiz1(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFeedBackWiz1)
	enum { IDD = IDD_FEEDBACK_WIZ1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFeedBackWiz1)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz2 dialog

class CFeedBackWiz2 : public CDialog
{
// Construction
public:
	CFeedBackWiz2(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFeedBackWiz2)
	enum { IDD = IDD_FEEDBACK_WIZ2 };
	int		m_nShowComments;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFeedBackWiz2)
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz3 dialog

class CFeedBackWiz3 : public CDialog
{
// Construction
public:
	CFeedBackWiz3(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFeedBackWiz3)
	enum { IDD = IDD_FEEDBACK_WIZ3 };
	int		m_nEase;
	int		m_nNav;
	int		m_nQuality;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFeedBackWiz3)
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz4 dialog

class CFeedBackWiz4 : public CDialog
{
// Construction
public:
	CFeedBackWiz4(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFeedBackWiz4)
	enum { IDD = IDD_FEEDBACK_WIZ4 };
	CString	m_strAdd;
	CString	m_strNix;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFeedBackWiz4)
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz5 dialog

class CFeedBackWiz5 : public CDialog
{
// Construction
public:
	CFeedBackWiz5(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFeedBackWiz5)
	enum { IDD = IDD_FEEDBACK_WIZ5 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFeedBackWiz5)
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz1 dialog

class CNewsSetupWiz1 : public CDialog
{
// Construction
public:
	CNewsSetupWiz1(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNewsSetupWiz1)
	enum { IDD = IDD_NWS_SETUP_1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CNewsSetupWiz1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz2 dialog

class CNewsSetupWiz2 : public CDialog
{
// Construction
public:
	CNewsSetupWiz2(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNewsSetupWiz2)
	enum { IDD = IDD_NWS_SETUP_2 };
	int		m_nChoice;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CNewsSetupWiz2)
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz3 dialog

class CNewsSetupWiz3 : public CDialog
{
// Construction
public:
	CNewsSetupWiz3(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNewsSetupWiz3)
	enum { IDD = IDD_NWS_SETUP_3 };
	CString	m_strServer;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CNewsSetupWiz3)
	afx_msg void OnBack();
	afx_msg void OnArticleAge();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz4 dialog

class CNewsSetupWiz4 : public CDialog
{
// Construction
public:
	CNewsSetupWiz4(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNewsSetupWiz4)
	enum { IDD = IDD_NWS_SETUP_4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CNewsSetupWiz4)
	afx_msg void OnBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz1 dialog

class CCompNewsWiz1 : public CDialog
{
// Construction
public:
	CCompNewsWiz1(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCompNewsWiz1)
	enum { IDD = IDD_COMPNEWS_WIZ1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompNewsWiz1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz2 dialog

class CCompNewsWiz2 : public CDialog
{
// Construction
public:
	CCompNewsWiz2(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCompNewsWiz2)
	enum { IDD = IDD_COMPNEWS_WIZ2 };
	CString	m_strSubject;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompNewsWiz2)
	afx_msg void OnBack();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; 

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz3 dialog

class CCompNewsWiz3 : public CDialog
{
// Construction
public:
	CCompNewsWiz3(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCompNewsWiz3)
	enum { IDD = IDD_COMPNEWS_WIZ3 };
	CString	m_strNewsgroups;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompNewsWiz3)
	afx_msg void OnBack();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; 

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz4 dialog

class CCompNewsWiz4 : public CDialog
{
// Construction
public:
	CCompNewsWiz4(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCompNewsWiz4)
	enum { IDD = IDD_COMPNEWS_WIZ4 };
	CString	m_strDistribution;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompNewsWiz4)
	afx_msg void OnBack();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz5 dialog

class CCompNewsWiz5 : public CDialog
{
// Construction
public:
	CCompNewsWiz5(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCompNewsWiz5)
	enum { IDD = IDD_COMPNEWS_WIZ5 };
	CString	m_strFollowupTo;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompNewsWiz5)
	afx_msg void OnBack();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; 

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz6 dialog

class CCompNewsWiz6 : public CDialog
{
// Construction
public:
	CCompNewsWiz6(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCompNewsWiz6)
	enum { IDD = IDD_COMPNEWS_WIZ6 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCompNewsWiz6)
	afx_msg void OnBack();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
 
/////////////////////////////////////////////////////////////////////////////
#endif //__WIZARDS_H__