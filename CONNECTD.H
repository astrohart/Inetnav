//*****************************************************************************
//* (C) Copyright 1995 Gregory Consulting Limited
//*
//* This file is Copyrighted and is subject to the rules outlined in the
//* software agreement accompanying this CDROM and book. You are free to
//* modify this code as you see fit according to those rules.
//*
//* THIS IS SAMPLE CODE! No warranties are expressed or implied, you're
//* using it at your own risk. In addition, the resulting program may
//* not be fully functional. Support for some features are left out so
//* that they can be added by you, the reader.
//*
//*****************************************************************************
// connectd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConnectDialog dialog

class CConnectDialog : public CDialog
{
// Construction
public:
	CConnectDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CConnectDialog)
	enum { IDD = IDD_CONNECT };
	CString	m_user;
	CString	m_password;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CConnectDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
