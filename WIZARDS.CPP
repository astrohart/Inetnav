// wizards.cpp : implementation file for Internet Navigator's wizards
//

#include "stdafx.h"
#include "inetnav.h"   

#include "mail.h"
#include "wizards.h"  
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailData data structure class -- use with CMailBoxSetupWiz

CMailData::CMailData()
{
	m_pApp = NULL;
	m_pDocument = NULL;
	DeleteContents();
}

CMailData::CMailData(CMailBoxDoc* pDoc, CInternetNavApp* pApp)
{
	Attach(pDoc, pApp);
	DeleteContents();
}

CMailData::~CMailData()
{
	DeleteContents();
	Detach();
}

void CMailData::Open()
{                                      
	// prep the Mailbox Setup Wizard
	Serialize(TRUE);
	m_pDocument->m_bInitialized = FALSE;
	return;
}

void CMailData::Close()
{
	Serialize(FALSE);       
	DeleteContents();
	return;
}

void CMailData::Serialize(BOOL IsLoading)
{
	if (IsLoading)
	{   
		if (!m_pDocument->m_strSMTPHost.IsEmpty())
			m_strSMTPHost = m_pDocument->m_strSMTPHost;
		if (!m_pDocument->m_strPOP3Host.IsEmpty())
			m_strPOP3Host = m_pDocument->m_strPOP3Host;
		if (!m_pDocument->m_strAddress.IsEmpty())
			m_strAddress  = m_pDocument->m_strAddress;
		if (!m_pDocument->m_strPassword.IsEmpty())
			m_strPassword = m_pDocument->m_strPassword;
		if (!m_pDocument->m_strPOP3Logon.IsEmpty())
			m_strPOP3Logon = m_pDocument->m_strPOP3Logon;
		return;
	}
	else
	{                   
		// save data to document class
		m_pDocument->m_strSMTPHost = m_strSMTPHost;
		m_pDocument->m_strPOP3Host = m_strPOP3Host;
		m_pDocument->m_strAddress = m_strAddress;  
		m_pDocument->m_strPassword = m_strPassword;
		m_pDocument->m_strPOP3Logon = m_strPOP3Logon;

		return;
	}
}                                           

void CMailData::DeleteContents()
{
	m_strSMTPHost = m_strPOP3Host = m_strAddress = m_strPassword = "";
	m_strPOP3Logon = "";
	return;
}

void CMailData::WizardCancel()
{
	DeleteContents();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz1 dialog

CMailBoxSetupWiz1::CMailBoxSetupWiz1(CWnd* pParent, CMailBoxDoc* pDoc)
	: CDialog(CMailBoxSetupWiz1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailBoxSetupWiz1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT     
	m_pData = pDoc->GetMailData();
}

void CMailBoxSetupWiz1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxSetupWiz1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP      
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMailBoxSetupWiz1, CDialog)
	//{{AFX_MSG_MAP(CMailBoxSetupWiz1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz1 message handlers

BOOL CMailBoxSetupWiz1::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_pData->Open(); // initializes information in data class for this wizard
						// using information in the document class
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMailBoxSetupWiz1::OnOK()
{
	CDialog::OnOK();              
	return;
}

void CMailBoxSetupWiz1::OnCancel()
{          
	m_pData->WizardCancel();
	CDialog::OnCancel();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz2 dialog


CMailBoxSetupWiz2::CMailBoxSetupWiz2(CWnd* pParent, CMailBoxDoc* pDoc)
	: CDialog(CMailBoxSetupWiz2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailBoxSetupWiz2)
	m_nWalkThrough = 0;
	//}}AFX_DATA_INIT 
	m_pData = pDoc->GetMailData();
}

void CMailBoxSetupWiz2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxSetupWiz2)
	DDX_Radio(pDX, IDC_WALKTHROUGH, m_nWalkThrough);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMailBoxSetupWiz2, CDialog)
	//{{AFX_MSG_MAP(CMailBoxSetupWiz2)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz2 message handlers

void CMailBoxSetupWiz2::OnCancel()
{
	m_pData->WizardCancel();
	
	CDialog::OnCancel();
}

void CMailBoxSetupWiz2::OnOK()
{
	if (UpdateData(TRUE))
	{
		m_pData->m_nWalkThrough = m_nWalkThrough;
		EndDialog(IDOK);
	}
}

void CMailBoxSetupWiz2::OnBack()
{
	EndDialog(IDRETRY);	
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz3 dialog


CMailBoxSetupWiz3::CMailBoxSetupWiz3(CWnd* pParent, CMailBoxDoc* pDoc)
	: CDialog(CMailBoxSetupWiz3::IDD, pParent)
{  
	m_pData = pDoc->GetMailData();
	//{{AFX_DATA_INIT(CMailBoxSetupWiz3)
	m_strSMTPAddress = m_pData->m_strSMTPHost;
	//}}AFX_DATA_INIT
}

void CMailBoxSetupWiz3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxSetupWiz3)
	DDX_Text(pDX, IDC_SMTP_ADDRESS, m_strSMTPAddress);
	DDV_MaxChars(pDX, m_strSMTPAddress, 255);
	//}}AFX_DATA_MAP                                
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMailBoxSetupWiz3, CDialog)
	//{{AFX_MSG_MAP(CMailBoxSetupWiz3)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDOK, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz3 message handlers

void CMailBoxSetupWiz3::OnBack()
{
	UpdateData(TRUE);
	m_pData->m_strSMTPHost = m_strSMTPAddress;
	EndDialog(IDRETRY); 
	return;
}

void CMailBoxSetupWiz3::OnCancel()
{
	m_pData->WizardCancel();
	CDialog::OnCancel();    
	return;
}

void CMailBoxSetupWiz3::OnNext()
{
	if (UpdateData(TRUE))
	{
		m_pData->m_strSMTPHost = m_strSMTPAddress;
		EndDialog(IDOK);
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz4 dialog


CMailBoxSetupWiz4::CMailBoxSetupWiz4(CWnd* pParent, CMailBoxDoc* pDoc)
	: CDialog(CMailBoxSetupWiz4::IDD, pParent)
{	
	m_pData = pDoc->GetMailData();
	//{{AFX_DATA_INIT(CMailBoxSetupWiz4)
	m_strPOP3Host = m_pData->m_strPOP3Host;
	//}}AFX_DATA_INIT
}

void CMailBoxSetupWiz4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxSetupWiz4)
	DDX_Text(pDX, IDC_POP3_ADDRESS, m_strPOP3Host);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMailBoxSetupWiz4, CDialog)
	//{{AFX_MSG_MAP(CMailBoxSetupWiz4)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz4 message handlers

void CMailBoxSetupWiz4::OnCancel()
{
	m_pData->WizardCancel();
	
	CDialog::OnCancel();
}

void CMailBoxSetupWiz4::OnOK()
{
	if (UpdateData(TRUE))
	{
		m_pData->m_strPOP3Host = m_strPOP3Host;
		EndDialog(IDOK);
	}	
}

void CMailBoxSetupWiz4::OnBack()
{
	if (UpdateData(TRUE))
	{
		m_pData->m_strPOP3Host = m_strPOP3Host;
		EndDialog(IDRETRY);
	}
}
               
/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz5 dialog


CMailBoxSetupWiz5::CMailBoxSetupWiz5(CWnd* pParent, CMailBoxDoc* pDoc)
	: CDialog(CMailBoxSetupWiz5::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pData = m_pDoc->GetMailData();
	//{{AFX_DATA_INIT(CMailBoxSetupWiz5)
	m_strPassword = m_pData->m_strPassword;
	m_strPOP3Logon = m_pData->m_strPOP3Logon;
	//}}AFX_DATA_INIT
}                  

void CMailBoxSetupWiz5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxSetupWiz5)
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_USER_ID, m_strPOP3Logon);
	//}}AFX_DATA_MAP     
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMailBoxSetupWiz5, CDialog)
	//{{AFX_MSG_MAP(CMailBoxSetupWiz5)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz5 message handlers

void CMailBoxSetupWiz5::OnOK()
{
	if (UpdateData(TRUE))
	{
		m_pData->m_strPassword = m_strPassword;
		m_pData->m_strPOP3Logon = m_strPOP3Logon;
		EndDialog(IDOK);	
	}
}

void CMailBoxSetupWiz5::OnCancel()
{
	m_pData->WizardCancel();
	
	CDialog::OnCancel();
}

void CMailBoxSetupWiz5::OnBack()
{
	if (UpdateData(TRUE))
	{
		m_pData->m_strPassword = m_strPassword;
		m_pData->m_strPOP3Logon = m_strPOP3Logon;
		EndDialog(IDRETRY);	
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz6 dialog


CMailBoxSetupWiz6::CMailBoxSetupWiz6(CWnd* pParent, CMailBoxDoc* pDoc)
	: CDialog(CMailBoxSetupWiz6::IDD, pParent)
{
	m_pData = pDoc->GetMailData();
	//{{AFX_DATA_INIT(CMailBoxSetupWiz6)
	m_strAddress = m_pData->m_strAddress;
	//}}AFX_DATA_INIT
}

void CMailBoxSetupWiz6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxSetupWiz6)
	DDX_Text(pDX, IDC_MAIL_ADDRESS, m_strAddress);
	DDV_MaxChars(pDX, m_strAddress, 255);
	//}}AFX_DATA_MAP  
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMailBoxSetupWiz6, CDialog)
	//{{AFX_MSG_MAP(CMailBoxSetupWiz6)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDOK, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz6 message handlers

void CMailBoxSetupWiz6::OnBack()
{
	UpdateData(TRUE);
	m_pData->m_strAddress = m_strAddress;
	EndDialog(IDRETRY);
}

void CMailBoxSetupWiz6::OnNext()
{
	if (UpdateData(TRUE))
	{
		m_pData->m_strAddress = m_strAddress;
		EndDialog(IDOK);
	}
}

void CMailBoxSetupWiz6::OnCancel()
{
	m_pData->WizardCancel(); // erases data, doesn't save changes
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz7 dialog


CMailBoxSetupWiz7::CMailBoxSetupWiz7(CWnd* pParent, CMailBoxDoc* pDoc)
	: CDialog(CMailBoxSetupWiz7::IDD, pParent)
{   
	m_pData = pDoc->GetMailData();
	//{{AFX_DATA_INIT(CMailBoxSetupWiz7)
	//}}AFX_DATA_INIT
}

void CMailBoxSetupWiz7::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxSetupWiz7)
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMailBoxSetupWiz7, CDialog)
	//{{AFX_MSG_MAP(CMailBoxSetupWiz7)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxSetupWiz7 message handlers

void CMailBoxSetupWiz7::OnCancel()
{
	m_pData->WizardCancel();
	
	CDialog::OnCancel();
}

void CMailBoxSetupWiz7::OnOK()
{
	// save information from the CMailData data class into the document class
	// which uses this Wizard.  The CMailData::Close() function does this.
	// NOTE: The data class is used to collect the data from the wizard into
	// a place where it can be deleted safely if the user cancels the wizard.
	// Once it's in the document class, the data is harder to locate and
	// delete.
	m_pData->Close();
	EndDialog(IDOK);
	
}

void CMailBoxSetupWiz7::OnBack()
{
	EndDialog(IDRETRY);
	return;
} 

/////////////////////////////////////////////////////////////////////////////
// CCompWiz1 dialog


CCompWiz1::CCompWiz1(CWnd* pParent /*=NULL*/)
	: CDialog(CCompWiz1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompWiz1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCompWiz1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompWiz1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompWiz1, CDialog)
	//{{AFX_MSG_MAP(CCompWiz1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompWiz1 message handlers
/////////////////////////////////////////////////////////////////////////////
// CCompWiz2 dialog


CCompWiz2::CCompWiz2(CWnd* pParent, CComposeView* pView)
	: CDialog(CCompWiz2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompWiz2)
	m_strSubject = "No Subject";
	//}}AFX_DATA_INIT                                  
	m_pComposeView = pView;
}

void CCompWiz2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompWiz2)
	DDX_Text(pDX, IDC_SUBJECT, m_strSubject);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompWiz2, CDialog)
	//{{AFX_MSG_MAP(CCompWiz2)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompWiz2 message handlers

void CCompWiz2::OnBack()
{
	if (UpdateData(TRUE))
	{
		if (m_strSubject.IsEmpty())
		{
			MessageBeep(-1);
			AfxMessageBox("You must provide a subject for your message.");
			return;
		}
		
		// If we made it this far, the user entered a valid subject
		EndDialog(IDRETRY);
	}
}

void CCompWiz2::OnOK()
{
	if (UpdateData(TRUE))
	{
		if (m_strSubject.IsEmpty())
		{
			MessageBeep(-1);
			AfxMessageBox("You must provide a subject for your message.");
			return;
		}
		
		// If we made it this far, the user entered a valid subject
		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCompWiz3 dialog


CCompWiz3::CCompWiz3(CWnd* pParent, CComposeView* pView)
	: CDialog(CCompWiz3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompWiz3)
	m_strTo = "";
	//}}AFX_DATA_INIT
	m_pComposeView = pView;
}

void CCompWiz3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompWiz3)
	DDX_Text(pDX, IDC_TO, m_strTo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompWiz3, CDialog)
	//{{AFX_MSG_MAP(CCompWiz3)
	ON_BN_CLICKED(IDC_CHOOSE, OnChoose)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompWiz3 message handlers

void CCompWiz3::OnChoose()
{
	CWaitCursor wait;
	CAddrChoose theDialog(this, m_pComposeView);
	
	if (theDialog.DoModal() == IDOK)
	{
		if (!theDialog.m_strAddress.IsEmpty())
		{
			m_strTo = theDialog.m_strAddress;
			UpdateData(FALSE);
		}
	}	
}

void CCompWiz3::OnBack()
{
	if (UpdateData(TRUE))
	{
		if (m_strTo.IsEmpty())
		{
			MessageBeep(-1);
			AfxMessageBox("Please enter a valid e-mail address.");
			return;
		}
		else if (m_strTo.Left(1) == "<" && m_strTo.Right(1) == ">")
		{
			// Cleanup m_strTo
			m_strTo = m_strTo.Right(m_strTo.GetLength() - 1);
			UpdateData(FALSE);
			
			m_strTo = m_strTo.Left(m_strTo.GetLength() - 1);
			UpdateData(FALSE);

			EndDialog(IDRETRY);
		}
		else if (m_strTo.Find('@') == -1)
		{
			MessageBeep(-1);
			AfxMessageBox("The e-mail address(s) must contain the '@' symbol.");
			return;
		}
		
		// E-Mail address is valid
		EndDialog(IDRETRY);
	}
}

void CCompWiz3::OnOK()
{
	if (UpdateData(TRUE))
	{
		if (m_strTo.IsEmpty())
		{
			MessageBeep(-1);
			AfxMessageBox("Please enter a valid e-mail address.");
			return;
		}
		else if (m_strTo.Left(1) == "<" && m_strTo.Right(1) == ">")
		{
			// Cleanup m_strTo
			m_strTo = m_strTo.Right(m_strTo.GetLength() - 1);
			UpdateData(FALSE);
			
			m_strTo = m_strTo.Left(m_strTo.GetLength() - 1);
			UpdateData(FALSE);
			
			CDialog::OnOK();
		}
		else if (m_strTo.Find('@') == -1)
		{
			MessageBeep(-1);
			AfxMessageBox("The e-mail address(es) must contain the '@' symbol.");
			return;
		}
		
		// E-Mail address is valid
		CDialog::OnOK();
	}
}
/////////////////////////////////////////////////////////////////////////////
// CCompWiz4 dialog


CCompWiz4::CCompWiz4(CWnd* pParent, CComposeView* pView)
	: CDialog(CCompWiz4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompWiz4)
	m_strCC = "";
	//}}AFX_DATA_INIT
	m_pComposeView = pView;
}

void CCompWiz4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompWiz4)
	DDX_Text(pDX, IDC_CC, m_strCC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompWiz4, CDialog)
	//{{AFX_MSG_MAP(CCompWiz4)
	ON_BN_CLICKED(IDC_CHOOSE, OnChoose)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompWiz4 message handlers

void CCompWiz4::OnChoose()
{
	CWaitCursor wait;
	CAddrChoose theDialog(this, m_pComposeView);
	
	if (theDialog.DoModal() == IDOK)
	{
		if (!theDialog.m_strAddress.IsEmpty())
		{
			m_strCC = theDialog.m_strAddress;
			UpdateData(FALSE);
		}
	}	
}

void CCompWiz4::OnOK()  // Next > button pressed
{
	if (UpdateData(TRUE))
	{
		if (m_strCC.IsEmpty())
		{     
			// CC is optional -- the user doesn't need to fill it in
			CDialog::OnOK();
		}
		else if (m_strCC.Left(1) == "<" && m_strCC.Right(1) == ">")
		{
			// Cleanup m_strCC
			m_strCC = m_strCC.Right(m_strCC.GetLength() - 1);
			UpdateData(FALSE);
			
			m_strCC = m_strCC.Left(m_strCC.GetLength() - 1);
			UpdateData(FALSE);
			
			CDialog::OnOK();
		}
		else if (m_strCC.Find('@') == -1)
		{
			MessageBeep(-1);
			AfxMessageBox("The e-mail address(es) must contain the '@' symbol.");
			return;
		}
		
		// E-Mail address is valid
		CDialog::OnOK();
	}
}

void CCompWiz4::OnBack()
{
	if (UpdateData(TRUE))
	{
		if (m_strCC.IsEmpty())
		{     
			// CC is optional -- the user doesn't need to fill it in
			EndDialog(IDRETRY);
		}
		else if (m_strCC.Left(1) == "<" && m_strCC.Right(1) == ">")
		{
			// Cleanup m_strCC
			m_strCC = m_strCC.Right(m_strCC.GetLength() - 1);
			UpdateData(FALSE);
			
			m_strCC = m_strCC.Left(m_strCC.GetLength() - 1);
			UpdateData(FALSE);
			
			EndDialog(IDRETRY);
		}
		else if (m_strCC.Find('@') == -1)
		{
			MessageBeep(-1);
			AfxMessageBox("The e-mail address(es) must contain the '@' symbol.");
			return;
		}
		
		// E-Mail address is valid
		EndDialog(IDRETRY);
	}
}  

/////////////////////////////////////////////////////////////////////////////
// CCompWiz5 dialog


CCompWiz5::CCompWiz5(CWnd* pParent, CComposeView* pView)
	: CDialog(CCompWiz5::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompWiz5)
	m_strBCC = "";
	//}}AFX_DATA_INIT     
	m_pComposeView = pView;
}

void CCompWiz5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompWiz5)
	DDX_Text(pDX, IDC_BCC, m_strBCC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompWiz5, CDialog)
	//{{AFX_MSG_MAP(CCompWiz5)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_CHOOSE, OnChoose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompWiz5 message handlers

void CCompWiz5::OnOK()
{
	if (UpdateData(TRUE))
	{
		if (m_strBCC.IsEmpty())
		{     
			// BCC is optional -- the user doesn't need to fill it in
			CDialog::OnOK();
		}
		else if (m_strBCC.Left(1) == "<" && m_strBCC.Right(1) == ">")
		{
			// Cleanup m_strBCC before the user's eyes
			m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - 1);
			UpdateData(FALSE);
			
			m_strBCC = m_strBCC.Left(m_strBCC.GetLength() - 1);
			UpdateData(FALSE);
			
			CDialog::OnOK();
		}
		else if (m_strBCC.Find('@') == -1)
		{
			MessageBeep(-1);
			AfxMessageBox("The e-mail address(es) must contain the '@' symbol.");
			return;
		}
		
		// E-Mail address is valid
		CDialog::OnOK();
	}
}

void CCompWiz5::OnBack()
{
	if (UpdateData(TRUE))
	{
		if (m_strBCC.IsEmpty())
		{     
			// BCC is optional -- the user doesn't need to fill it in
			EndDialog(IDRETRY);
		}
		else if (m_strBCC.Left(1) == "<" && m_strBCC.Right(1) == ">")
		{
			// Cleanup m_strBCC
			m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - 1);
			UpdateData(FALSE);
			
			m_strBCC = m_strBCC.Left(m_strBCC.GetLength() - 1);
			UpdateData(FALSE);
			
			EndDialog(IDRETRY);
		}
		else if (m_strBCC.Find('@') == -1)
		{
			MessageBeep(-1);
			AfxMessageBox("The e-mail address(es) must contain the '@' symbol.");
			return;
		}
		
		// E-Mail address is valid
		EndDialog(IDRETRY);
	}
}

void CCompWiz5::OnChoose()
{
	CWaitCursor wait;
	CAddrChoose theDialog(this, m_pComposeView);
	
	if (theDialog.DoModal() == IDOK)
	{
		if (!theDialog.m_strAddress.IsEmpty())
		{
			m_strBCC = theDialog.m_strAddress;
			UpdateData(FALSE);
		}
	}	
}
/////////////////////////////////////////////////////////////////////////////
// CCompWizFinish dialog


CCompWizFinish::CCompWizFinish(CWnd* pParent /*=NULL*/)
	: CDialog(CCompWizFinish::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompWizFinish)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCompWizFinish::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompWizFinish)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompWizFinish, CDialog)
	//{{AFX_MSG_MAP(CCompWizFinish)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompWizFinish message handlers

void CCompWizFinish::OnBack()
{	
	EndDialog(IDRETRY);
}

void CCompWizFinish::OnOK()
{
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz1 dialog


CFeedBackWiz1::CFeedBackWiz1(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedBackWiz1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeedBackWiz1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFeedBackWiz1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeedBackWiz1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeedBackWiz1, CDialog)
	//{{AFX_MSG_MAP(CFeedBackWiz1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz1 message handlers

void CFeedBackWiz1::OnOK()
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz2 dialog


CFeedBackWiz2::CFeedBackWiz2(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedBackWiz2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeedBackWiz2)
	m_nShowComments = 1;
	//}}AFX_DATA_INIT
}

void CFeedBackWiz2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeedBackWiz2)
	DDX_Radio(pDX, IDC_SHOW_WINDOW, m_nShowComments);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeedBackWiz2, CDialog)
	//{{AFX_MSG_MAP(CFeedBackWiz2)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz2 message handlers     

void CFeedBackWiz2::OnBack()
{
	if (!UpdateData(TRUE))
		return;
	EndDialog(IDRETRY);
}

/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz3 dialog


CFeedBackWiz3::CFeedBackWiz3(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedBackWiz3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeedBackWiz3)
	m_nEase = 2;
	m_nNav = 2;
	m_nQuality = 2;
	//}}AFX_DATA_INIT
}

void CFeedBackWiz3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeedBackWiz3)
	DDX_Radio(pDX, IDC_EASE_1, m_nEase);
	DDX_Radio(pDX, IDC_NAV_1, m_nNav);
	DDX_Radio(pDX, IDC_QUALITY_1, m_nQuality);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeedBackWiz3, CDialog)
	//{{AFX_MSG_MAP(CFeedBackWiz3)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz3 message handlers      

void CFeedBackWiz3::OnBack()
{
	if (!UpdateData(TRUE))
		return;
	EndDialog(IDRETRY);
}

/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz4 dialog


CFeedBackWiz4::CFeedBackWiz4(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedBackWiz4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeedBackWiz4)
	//}}AFX_DATA_INIT
}

void CFeedBackWiz4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeedBackWiz4)
	DDX_Text(pDX, IDC_FEATURES_ADD, m_strAdd);
	DDX_Text(pDX, IDC_FEATURES_DELETE, m_strNix);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeedBackWiz4, CDialog)
	//{{AFX_MSG_MAP(CFeedBackWiz4)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz4 message handlers

void CFeedBackWiz4::OnBack()
{
	if (!UpdateData(TRUE))
		return;
	EndDialog(IDRETRY);
}
/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz5 dialog


CFeedBackWiz5::CFeedBackWiz5(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedBackWiz5::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeedBackWiz5)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFeedBackWiz5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeedBackWiz5)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeedBackWiz5, CDialog)
	//{{AFX_MSG_MAP(CFeedBackWiz5)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeedBackWiz5 message handlers

void CFeedBackWiz5::OnBack()
{
	if (!UpdateData(TRUE))
		return;
	EndDialog(IDRETRY);
}
/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz1 dialog


CNewsSetupWiz1::CNewsSetupWiz1(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsSetupWiz1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewsSetupWiz1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CNewsSetupWiz1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsSetupWiz1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewsSetupWiz1, CDialog)
	//{{AFX_MSG_MAP(CNewsSetupWiz1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz1 message handlers
/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz2 dialog


CNewsSetupWiz2::CNewsSetupWiz2(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsSetupWiz2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewsSetupWiz2)
	m_nChoice = 0;
	//}}AFX_DATA_INIT
}

void CNewsSetupWiz2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsSetupWiz2)
	DDX_Radio(pDX, IDC_WALKTHROUGH, m_nChoice);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewsSetupWiz2, CDialog)
	//{{AFX_MSG_MAP(CNewsSetupWiz2)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz2 message handlers

void CNewsSetupWiz2::OnBack()
{
	if (!UpdateData(TRUE))
		return;
	EndDialog(IDRETRY);
}  

/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz3 dialog


CNewsSetupWiz3::CNewsSetupWiz3(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsSetupWiz3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewsSetupWiz3)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CNewsSetupWiz3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsSetupWiz3)
	DDX_Text(pDX, IDC_NEWS_ADDRESS, m_strServer);
	DDV_MaxChars(pDX, m_strServer, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewsSetupWiz3, CDialog)
	//{{AFX_MSG_MAP(CNewsSetupWiz3)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_ARTICLE_AGE, OnArticleAge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz3 message handlers

void CNewsSetupWiz3::OnBack()
{
	if (!UpdateData(TRUE))
		return;
	EndDialog(IDRETRY);
}      

void CNewsSetupWiz3::OnArticleAge()
{
	CArticleAge theDlg((CPropertySheet*)m_pParentWnd);
	m_pApp->Serialize(FALSE);
	theDlg.m_lDays = m_pApp->GetArticleDays();
	theDlg.m_nHours = m_pApp->GetArticleHours();
	theDlg.m_nMinutes = m_pApp->GetArticleMinutes();
	theDlg.m_nSeconds = m_pApp->GetArticleSeconds();
	
	if (theDlg.DoModal() == IDOK)
	{
		m_pApp->SetArticleDays(theDlg.m_lDays);
		m_pApp->SetArticleHours(theDlg.m_nHours);
		m_pApp->SetArticleMinutes(theDlg.m_nMinutes);
		m_pApp->SetArticleSeconds(theDlg.m_nSeconds);
		m_pApp->Serialize(TRUE);
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz4 dialog


CNewsSetupWiz4::CNewsSetupWiz4(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsSetupWiz4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewsSetupWiz4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CNewsSetupWiz4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsSetupWiz4)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewsSetupWiz4, CDialog)
	//{{AFX_MSG_MAP(CNewsSetupWiz4)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewsSetupWiz4 message handlers

void CNewsSetupWiz4::OnBack()
{
	if (!UpdateData(TRUE))
		return;
	EndDialog(IDRETRY);
}
/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz1 dialog


CCompNewsWiz1::CCompNewsWiz1(CWnd* pParent /*=NULL*/)
	: CDialog(CCompNewsWiz1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompNewsWiz1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCompNewsWiz1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompNewsWiz1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompNewsWiz1, CDialog)
	//{{AFX_MSG_MAP(CCompNewsWiz1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz1 message handlers
/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz2 dialog


CCompNewsWiz2::CCompNewsWiz2(CWnd* pParent /*=NULL*/)
	: CDialog(CCompNewsWiz2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompNewsWiz2)
	//}}AFX_DATA_INIT
}

void CCompNewsWiz2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompNewsWiz2)
	DDX_Text(pDX, IDC_SUBJECT, m_strSubject);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompNewsWiz2, CDialog)
	//{{AFX_MSG_MAP(CCompNewsWiz2)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz2 message handlers   

void CCompNewsWiz2::OnBack()
{
	if (UpdateData(TRUE))
	{
		if (m_strSubject.IsEmpty())
		{
			MessageBeep(-1);
			AfxMessageBox("You must provide a subject for your article.");
			return;
		}
		
		// If we made it this far, the user entered a valid subject
		EndDialog(IDRETRY);
	}
}

void CCompNewsWiz2::OnOK()
{
	if (UpdateData(TRUE))
	{
		if (m_strSubject.IsEmpty())
		{
			MessageBeep(-1);
			AfxMessageBox("You must provide a subject for your article.");
			return;
		}
		
		// If we made it this far, the user entered a valid subject
		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz3 dialog


CCompNewsWiz3::CCompNewsWiz3(CWnd* pParent /*=NULL*/)
	: CDialog(CCompNewsWiz3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompNewsWiz3)
	//}}AFX_DATA_INIT
}

void CCompNewsWiz3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompNewsWiz3)
	DDX_Text(pDX, IDC_NEWSGROUPS, m_strNewsgroups);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompNewsWiz3, CDialog)
	//{{AFX_MSG_MAP(CCompNewsWiz3)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz3 message handlers

void CCompNewsWiz3::OnBack()
{
	if (UpdateData(TRUE))
	{
		if (m_strNewsgroups.IsEmpty())
		{
			MessageBeep(-1);
			AfxMessageBox("Please enter the name of at least one newsgroup.");
			return;
		}
		else if (m_strNewsgroups.Find(' ') != -1 
			&& m_strNewsgroups.Find(',') != -1)
		{
			MessageBeep(-1);
			AfxMessageBox("List of newsgroups must not contain spaces.");
			return;	
		}
		
		// Newsgroups header is valid
		EndDialog(IDRETRY);
	}
}

void CCompNewsWiz3::OnOK()
{
	if (UpdateData(TRUE))
	{
		if (m_strNewsgroups.IsEmpty())
		{
			MessageBeep(-1);
			AfxMessageBox("Please enter the name of at least one newsgroup.");
			return;
		}
		else if (m_strNewsgroups.Find(' ') != -1 
			&& m_strNewsgroups.Find(',') != -1)
		{
			MessageBeep(-1);
			AfxMessageBox("List of newsgroups must not contain spaces.");
			return;	
		}
		
		// Newsgroups header is valid
		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz4 dialog


CCompNewsWiz4::CCompNewsWiz4(CWnd* pParent /*=NULL*/)
	: CDialog(CCompNewsWiz4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompNewsWiz4)
	m_strDistribution = "";
	//}}AFX_DATA_INIT
}

void CCompNewsWiz4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompNewsWiz4)
	DDX_Text(pDX, IDC_DISTRIBUTION, m_strDistribution);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompNewsWiz4, CDialog)
	//{{AFX_MSG_MAP(CCompNewsWiz4)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz4 message handlers

void CCompNewsWiz4::OnBack()
{
	if (UpdateData(TRUE))
	{
		if (m_strDistribution.IsEmpty())
		{
			// OK - Distribution is optional
			EndDialog(IDRETRY);
			return;
		}
		else if (m_strDistribution.Find(' ') != -1 ||
			m_strDistribution.Find(',') != -1)
		{
			MessageBeep(-1);
			AfxMessageBox("You may only enter one distribution code in the "
				"'Distribution' header.");
			return;
		}
		
		// If we made it this far, distribution is valid
		EndDialog(IDRETRY);
	}
}

void CCompNewsWiz4::OnOK()
{
	if (UpdateData(TRUE))
	{
		if (m_strDistribution.IsEmpty())
		{
			// OK - Distribution is optional
			CDialog::OnOK();
			return;
		}
		else if (m_strDistribution.Find(' ') != -1 ||
			m_strDistribution.Find(',') != -1)
		{
			MessageBeep(-1);
			AfxMessageBox("You may only enter one distribution code in the "
				"\'Distribution\' header.");
			return;
		}
		
		// If we made it this far, distribution is valid
		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz5 dialog


CCompNewsWiz5::CCompNewsWiz5(CWnd* pParent /*=NULL*/)
	: CDialog(CCompNewsWiz5::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompNewsWiz5)
	//}}AFX_DATA_INIT
}

void CCompNewsWiz5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompNewsWiz5)
	DDX_Text(pDX, IDC_FOLLOWUP_TO, m_strFollowupTo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompNewsWiz5, CDialog)
	//{{AFX_MSG_MAP(CCompNewsWiz5)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz5 message handlers

void CCompNewsWiz5::OnBack()
{
	if (UpdateData(TRUE))
	{
		if (m_strFollowupTo.IsEmpty())
		{
			// OK -- Followup-To is optional
			EndDialog(IDRETRY);
			return;
		}
		else if (m_strFollowupTo.Find(' ') != -1 
			&& m_strFollowupTo.Find(',') != -1)
		{
			MessageBeep(-1);
			AfxMessageBox("List of newsgroups must not contain spaces.");
			return;	
		}
		
		// Followup-To header is valid
		EndDialog(IDRETRY);
	}
}

void CCompNewsWiz5::OnOK()
{
	if (UpdateData(TRUE))
	{
		if (m_strFollowupTo.IsEmpty())
		{
			// OK -- Followup-To is optional
			CDialog::OnOK();
			return;
		}
		else if (m_strFollowupTo.Find(' ') != -1 
			&& m_strFollowupTo.Find(',') != -1)
		{
			MessageBeep(-1);
			AfxMessageBox("List of newsgroups must not contain spaces.");
			return;	
		}
		
		// Followup-To header is valid
		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz6 dialog


CCompNewsWiz6::CCompNewsWiz6(CWnd* pParent /*=NULL*/)
	: CDialog(CCompNewsWiz6::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompNewsWiz6)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCompNewsWiz6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompNewsWiz6)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCompNewsWiz6, CDialog)
	//{{AFX_MSG_MAP(CCompNewsWiz6)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCompNewsWiz6 message handlers

void CCompNewsWiz6::OnBack()
{
	EndDialog(IDRETRY);
}

void CCompNewsWiz6::OnOK()
{
	CDialog::OnOK();	// Finish button pressed - commit data
}
