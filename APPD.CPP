// appd.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"
#include "appd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAppFileDialog dialog

CAppFileDialog::CAppFileDialog(BOOL bOpenFileDialog, LPCSTR lpszDefExt,
	LPCSTR lpszFileName, DWORD dwFlags, LPCSTR lpszFilter, CWnd* pParentWnd)
: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	//{{AFX_DATA_INIT(CAppFileDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CAppFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppFileDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP  
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CAppFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CAppFileDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAppFileDialog message handlers
