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
// connectd.cpp : implementation file
//

#include "stdafx.h"
#include "ftp.h"
#include "connectd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectDialog dialog


CConnectDialog::CConnectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectDialog)
	m_user = "";
	m_password = "";
	//}}AFX_DATA_INIT
}

void CConnectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDialog)
	DDX_Text(pDX, IDC_USER, m_user);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConnectDialog, CDialog)
	//{{AFX_MSG_MAP(CConnectDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConnectDialog message handlers
