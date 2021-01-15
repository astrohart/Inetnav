// dialogs.cpp : implementation file for all of the dialog boxes which
// Internet Navigator uses to gather information from and display 
// information to the user, except for the main form views for each client.
//

#include "stdafx.h"
#include "inetnav.h"
#include "appd.h"
#include "dialogs.h"
                 
#include "mail.h"               
#include "news.h"

#include "ftp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExitDlg dialog

CExitDlg::CExitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExitDlg)
	//}}AFX_DATA_INIT
}

void CExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExitDlg)
	DDX_Check(pDX, IDC_CONFIRM_EXIT, m_bConfirmExit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExitDlg, CDialog)
	//{{AFX_MSG_MAP(CExitDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////
// CExitDlg message handlers		
/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog

CConnectDlg::CConnectDlg(CWnd* pParent, CString strSiteName)
	: CDialog(CConnectDlg::IDD, pParent)
{  
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	//{{AFX_DATA_INIT(CConnectDlg)
	m_strUserID = "anonymous";
	m_strPassword = pApp->GetProfileString("Post Office", "MailAddress");
	//}}AFX_DATA_INIT
	m_strSiteName = strSiteName;
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDlg)
	DDX_Text(pDX, IDC_USER_ID, m_strUserID);
	DDV_MaxChars(pDX, m_strUserID, 255);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 255);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConnectDlg message handlers

BOOL CConnectDlg::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}
	
	// First, set the title of this dialog box to say, "Connect to site";
	// where site is the name of the FTP server
	// (i.e. if site == "ftp.microsoft.com": "Connect to ftp.microsoft.com")
	m_strSiteName.MakeLower();
	CString strText = "Connect to ";
	strText += m_strSiteName;
	
	SetWindowText(strText);     

	CenterWindow(((CInternetNavApp*)AfxGetApp())->GetMainFrame());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CWelcomeDlg dialog


CWelcomeDlg::CWelcomeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWelcomeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWelcomeDlg)
	m_strWelcome = "";
	//}}AFX_DATA_INIT
}

void CWelcomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomeDlg)
	DDX_Control(pDX, IDC_WELCOME_MSG, m_ctlWelcome);
	DDX_Text(pDX, IDC_WELCOME_MSG, m_strWelcome);
	//}}AFX_DATA_MAP                 
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CWelcomeDlg, CDialog)
	//{{AFX_MSG_MAP(CWelcomeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWelcomeDlg message handlers    

BOOL CWelcomeDlg::OnInitDialog()
{
	// initialize VBX controls etc
	if (!ExecuteDlgInit(m_lpDialogTemplate))
		return FALSE;

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init\n");
		#ifdef _DEBUG
			AfxMessageBox("UpdateData failed during dialog init.",
				MB_ICONSTOP);
		#endif
		EndDialog(IDABORT);
		return FALSE;
	}
	
	CFtpView* pView = (CFtpView*)m_pParentWnd;
	CString strSiteName = pView->m_pDoc->GetSiteName(), strText = "";
	
	strText = "Connected to ";
	strText += strSiteName;
	
	SetWindowText(strText);                  
	
	m_ctlWelcome.SetReadOnly(TRUE);

	CenterWindow(((CInternetNavApp*)AfxGetApp())->GetMainFrame());
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CIRConnectDlg dialog
                                                                             CIRConnectDlg::CIRConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIRConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIRConnectDlg)
	m_strServer = "";
	m_nPortNum = 6667;
	m_strNickname = "";
	m_strInitialChannel = "";
	//}}AFX_DATA_INIT
}

void CIRConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIRConnectDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_HELP, m_btnHelp);
	DDX_Text(pDX, IDC_SERVER, m_strServer);
	DDV_MaxChars(pDX, m_strServer, 64);
	DDX_Text(pDX, IDC_PORT_NUM, m_nPortNum);
	DDV_MinMaxInt(pDX, m_nPortNum, 1, 32766);
	DDX_Text(pDX, IDC_NICKNAME, m_strNickname);
	DDV_MaxChars(pDX, m_strNickname, 9);
	DDX_Text(pDX, IDC_CHANNEL, m_strInitialChannel);
	DDV_MaxChars(pDX, m_strInitialChannel, 200);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CIRConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CIRConnectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIRConnectDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CGopherConnect dialog


CGopherConnect::CGopherConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CGopherConnect::IDD, pParent)
{ 
	m_strMenuTitle.LoadString(IDS_GOPHER_TITLE);  
	m_strSiteAddress.LoadString(IDS_GOPHER_SERVER);
	//{{AFX_DATA_INIT(CGopherConnect)              
	m_strSelector = _T("/");
	m_nPortNum = 70;
	//}}AFX_DATA_INIT
}

void CGopherConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGopherConnect)
	DDX_Text(pDX, IDC_TITLE, m_strMenuTitle);
	DDX_Text(pDX, IDC_SITE_ADDRESS, m_strSiteAddress);
	DDX_Text(pDX, IDC_SELECTOR, m_strSelector);
	DDX_Text(pDX, IDC_PORT, m_nPortNum);
	//}}AFX_DATA_MAP   
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CGopherConnect, CDialog)
	//{{AFX_MSG_MAP(CGopherConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGopherConnect message handlers
/////////////////////////////////////////////////////////////////////////////
// CVeronicaSearchDlg dialog


CVeronicaSearchDlg::CVeronicaSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVeronicaSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVeronicaSearchDlg)
	//}}AFX_DATA_INIT
}

void CVeronicaSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVeronicaSearchDlg)
	DDX_Text(pDX, IDC_QUERY, m_strQuery);
	DDV_MaxChars(pDX, m_strQuery, 255);
	//}}AFX_DATA_MAP     
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CVeronicaSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CVeronicaSearchDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVeronicaSearchDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CAOLDirDlg dialog


CAOLDirDlg::CAOLDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAOLDirDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAOLDirDlg)
	//}}AFX_DATA_INIT
}

void CAOLDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOLDirDlg)
	DDX_Text(pDX, IDC_DIRECTORY, m_strDirectory);
	DDV_MaxChars(pDX, m_strDirectory, 16);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAOLDirDlg, CDialog)
	//{{AFX_MSG_MAP(CAOLDirDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAOLDirDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CWarnDlg dialog


CWarnDlg::CWarnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWarnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarnDlg)
	m_nType = 0;
	//}}AFX_DATA_INIT
}

void CWarnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarnDlg)
	DDX_Radio(pDX, IDC_OPEN_TYPE, m_nType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWarnDlg, CDialog)
	//{{AFX_MSG_MAP(CWarnDlg)
	ON_BN_CLICKED(1, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWarnDlg message handlers             

void CWarnDlg::OnOK()
{
	if (!UpdateData(TRUE))
		return;
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CIRCJoinDlg dialog


CIRCJoinDlg::CIRCJoinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIRCJoinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIRCJoinDlg)
	m_strChannel = "";
	//}}AFX_DATA_INIT
}

void CIRCJoinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIRCJoinDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_NEW_CHANNEL, m_strChannel);
	DDV_MaxChars(pDX, m_strChannel, 200);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(TRUE);
}

BEGIN_MESSAGE_MAP(CIRCJoinDlg, CDialog)
	//{{AFX_MSG_MAP(CIRCJoinDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIRCJoinDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CFingerAddr dialog


CFingerAddr::CFingerAddr(CWnd* pParent /*=NULL*/)
	: CDialog(CFingerAddr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFingerAddr)
	m_strAddr = "";
	m_bDontShowAgain = FALSE;
	//}}AFX_DATA_INIT
}

void CFingerAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFingerAddr)
	DDX_Text(pDX, IDC_ADDRESS, m_strAddr);
	DDX_Check(pDX, IDC_DONT_SHOW_AGAIN, m_bDontShowAgain);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CFingerAddr, CDialog)
	//{{AFX_MSG_MAP(CFingerAddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFingerAddr message handlers 
/////////////////////////////////////////////////////////////////////////////
// CIRCMessageDialog dialog


CIRCMessageDialog::CIRCMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIRCMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIRCMessageDialog)
	m_strMessage = "";
	m_strRecipient = "";
	//}}AFX_DATA_INIT
}

void CIRCMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIRCMessageDialog)
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	DDX_Text(pDX, IDC_RECIPIENT, m_strRecipient);
	DDV_MaxChars(pDX, m_strRecipient, 128);
	//}}AFX_DATA_MAP     
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CIRCMessageDialog, CDialog)
	//{{AFX_MSG_MAP(CIRCMessageDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIRCMessageDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// CIRCSendCmdDialog dialog


CIRCSendCmdDialog::CIRCSendCmdDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIRCSendCmdDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIRCSendCmdDialog)
	m_strCommand = "";
	//}}AFX_DATA_INIT
}

void CIRCSendCmdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIRCSendCmdDialog)
	DDX_Text(pDX, IDC_COMMAND, m_strCommand);
	DDV_MaxChars(pDX, m_strCommand, 510);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CIRCSendCmdDialog, CDialog)
	//{{AFX_MSG_MAP(CIRCSendCmdDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIRCSendCmdDialog message handlers

void CIRCSendCmdDialog::OnOK()
{
	// If the user has typed in a command, then add a CRLF pair on the end
	// and then close the dialog box.  Else, we will pretend the Cancel
	// button was pressed.
	UpdateData(TRUE);
	
	if (m_strCommand.IsEmpty())
	{
		// pretend the user chose Cancel; tell the caller of DoModal() that
		EndDialog(IDCANCEL);
		return;
	}
	
	if (m_strCommand.Right(2) != "\r\n")// add a CRLF because the user can't
		m_strCommand += "\r\n";
	EndDialog(IDOK); // tell the caller of DoModal() the user pressed OK
	return;	
}
    
/////////////////////////////////////////////////////////////////////////////
// CIRCNicknameDialog dialog


CIRCNicknameDialog::CIRCNicknameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIRCNicknameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIRCNicknameDialog)
	m_strNewNickname = "";
	//}}AFX_DATA_INIT
}

void CIRCNicknameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIRCNicknameDialog)
	DDX_Text(pDX, IDC_NEW_NICKNAME, m_strNewNickname);
	DDV_MaxChars(pDX, m_strNewNickname, 9);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CIRCNicknameDialog, CDialog)
	//{{AFX_MSG_MAP(CIRCNicknameDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIRCNicknameDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// CDisconnectDialog dialog


CDisconnectDialog::CDisconnectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDisconnectDialog::IDD, pParent)
{  
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//{{AFX_DATA_INIT(CDisconnectDialog)
	m_bConfirmSignOff = !m_pApp->IsConfirmSignOff();
	//}}AFX_DATA_INIT      
}

void CDisconnectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisconnectDialog)
	DDX_Check(pDX, IDC_CONFIRM_SIGNOFF, m_bConfirmSignOff);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CDisconnectDialog, CDialog)
	//{{AFX_MSG_MAP(CDisconnectDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDisconnectDialog message handlers

BOOL CDisconnectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bConfirmSignOff = !m_pApp->IsConfirmSignOff();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}                                                
	
void CDisconnectDialog::OnOK()
{
	if (!UpdateData(TRUE))
	{
		// don't fail -- just leave the m_pApp->m_bConfirmSignOff setting
		// alone 
		EndDialog(IDOK);
		return;
	}
	
	// now -- if UpdateData() worked, we have the value of the checkbox
	// as TRUE or FALSE in m_bConfirmSignOff.  If our m_bConfirmSignOff is
	// TRUE, we need to set the m_pApp->m_bConfirmSignOff to FALSE, and
	// vice-versa.
	m_pApp->SetConfirmSignOff(!m_bConfirmSignOff);
	m_pApp->Serialize(TRUE);
	
	EndDialog(IDOK);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxTitleDlg dialog


CMailBoxTitleDlg::CMailBoxTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailBoxTitleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailBoxTitleDlg)
	m_strMailBoxTitle = "Internet Mail";
	m_strFileName = "INTERNET.MAI";
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CMailBoxTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxTitleDlg)
	DDX_Text(pDX, IDC_MAILBOX_TITLE, m_strMailBoxTitle);
	DDV_MaxChars(pDX, m_strMailBoxTitle, 40);
	DDX_Text(pDX, IDC_EDIT1, m_strFileName);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMailBoxTitleDlg, CDialog)
	//{{AFX_MSG_MAP(CMailBoxTitleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxTitleDlg message handlers     

void CMailBoxTitleDlg::OnOK()
{ 
	BOOL bResult = TRUE;
	
	if (!UpdateData(TRUE))
		return;
		
	for (int i = 0;i < m_pApp->m_strMailboxPathList.GetSize();i++)
	{
		if (m_pApp->m_strMailboxPathList[i] == m_strFileName)
		{
			bResult = FALSE;
			break;
		}
	}
	
	for (int c = 0;c < m_pApp->m_strMailboxNameList.GetSize();c++)
	{
		if (m_pApp->m_strMailboxNameList[c] == m_strMailBoxTitle)
		{
			bResult = FALSE;
			break;
		}
	} 
	
	if (bResult)
	{	
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(IDP_MAILBOX_EXISTS, MB_ICONSTOP);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPOP3Dialog dialog


CPOP3Dialog::CPOP3Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPOP3Dialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPOP3Dialog)
	//}}AFX_DATA_INIT
}

void CPOP3Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPOP3Dialog)
	DDX_Control(pDX, IDC_PASSWORD, m_ctlPassword);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_USER_ID, m_strUserID);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CPOP3Dialog, CDialog)
	//{{AFX_MSG_MAP(CPOP3Dialog)
	ON_EN_CHANGE(IDC_PASSWORD, OnChangePassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPOP3Dialog message handlers

BOOL CPOP3Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnOK.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPOP3Dialog::OnChangePassword()
{
	UpdateData(TRUE);
	
	m_btnOK.EnableWindow(
		m_ctlPassword.GetWindowTextLength() != 0);
}

/////////////////////////////////////////////////////////////////////////////
// CUserInformation dialog


CUserInformation::CUserInformation(CWnd* pParent /*=NULL*/)
	: CDialog(CUserInformation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserInformation)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
	m_pApp->GetUserData()->Serialize(FALSE);
	m_strCompany = m_pApp->GetUserData()->GetUserCompany();
	m_strUser = m_pApp->GetUserData()->GetUserName();
}

void CUserInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserInformation)
	DDX_Text(pDX, IDC_USER, m_strUser);
	DDV_MaxChars(pDX, m_strUser, 256);
	DDX_Text(pDX, IDC_COMPANY, m_strCompany);
	DDV_MaxChars(pDX, m_strCompany, 256);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CUserInformation, CDialog)
	//{{AFX_MSG_MAP(CUserInformation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUserInformation message handlers

void CUserInformation::OnCancel()
{
	m_pApp->GetUserData()->OnWizardCancel();
	
	CDialog::OnCancel();
}

void CUserInformation::OnOK()
{    
	// Save data to member variables from screen
	UpdateData();
	
	// Update data in application data structure and save to INI
	m_pApp->GetUserData()->m_strUserName = m_strUser;
	m_pApp->GetUserData()->m_strUserCompany = m_strCompany;

	m_pApp->GetUserData()->OnWizardFinish();
	
	// Return IDOK to caller of DoModal() and close this dialog box	
	EndDialog(IDOK);
}

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsSetup dialog


CNewsgroupsSetup::CNewsgroupsSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsgroupsSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewsgroupsSetup)
	m_strNNTPHost = "";
	//}}AFX_DATA_INIT
}

void CNewsgroupsSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsgroupsSetup)
	DDX_Text(pDX, IDC_NNTP_HOST, m_strNNTPHost);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewsgroupsSetup, CDialog)
	//{{AFX_MSG_MAP(CNewsgroupsSetup)
	ON_BN_CLICKED(IDC_ARTICLE_AGE, OnArticleAge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsSetup message handlers     

void CNewsgroupsSetup::OnArticleAge()
{ 
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	CArticleAge theDlg(this);
	pApp->Serialize(FALSE);
	theDlg.m_lDays = pApp->GetArticleDays();
	theDlg.m_nHours = pApp->GetArticleHours();
	theDlg.m_nMinutes = pApp->GetArticleMinutes();
	theDlg.m_nSeconds = pApp->GetArticleSeconds();
	
	if (theDlg.DoModal() == IDOK)
	{ 
		pApp->SetArticleDays(theDlg.m_lDays);
		pApp->SetArticleHours(theDlg.m_nHours);
		pApp->SetArticleMinutes(theDlg.m_nMinutes);
		pApp->SetArticleSeconds(theDlg.m_nSeconds);
		pApp->Serialize(TRUE);
	}
	return;
}                           

/////////////////////////////////////////////////////////////////////////////
// CMailSetup dialog


CMailSetup::CMailSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CMailSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailSetup)
	m_strEmailAddress = "";
	m_strUserID = "";
	m_strPassword = "";
	m_strPOP3Address = "";
	m_strSMTPAddress = "";
	//}}AFX_DATA_INIT
}

void CMailSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailSetup)
	DDX_Text(pDX, IDC_EMAIL_ADDRESS, m_strEmailAddress);
	DDX_Text(pDX, IDC_USERID, m_strUserID);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_POP3_ADDRESS, m_strPOP3Address);
	DDX_Text(pDX, IDC_SMTP_ADDRESS, m_strSMTPAddress);
	//}}AFX_DATA_MAP  
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CMailSetup, CDialog)
	//{{AFX_MSG_MAP(CMailSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailSetup message handlers
/////////////////////////////////////////////////////////////////////////////
// CGroupSubscribe dialog


CGroupSubscribe::CGroupSubscribe(CWnd* pParent, QSocket* pSocket)
	: CDialog(CGroupSubscribe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupSubscribe)
	m_strNewsgroup = "";
	m_strSelection = "";
	//}}AFX_DATA_INIT    
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_pSocket = pSocket;
	m_bKeepAdding = TRUE;
}

void CGroupSubscribe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupSubscribe)
	DDX_Control(pDX, IDC_NEWSGROUPS_LIST, m_ctlGroupList);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_DOWNLOAD_GROUPS, m_btnGetList);
	DDX_Text(pDX, IDC_NEWSGROUP, m_strNewsgroup);
	DDV_MaxChars(pDX, m_strNewsgroup, 255);
	DDX_LBString(pDX, IDC_NEWSGROUPS_LIST, m_strSelection);
	//}}AFX_DATA_MAP
}

CGroupSubscribe::~CGroupSubscribe()
{
	if (m_hWnd != NULL)
	{
		TRACE0("Warning: calling DestroyWindow in CGroupSubscribe::~CGroupSubscribe\n");
		TRACE0("\tOnDestroy or PostNcDestroy in derived class will not be called\n");
		DestroyWindow();
	}

	m_pSocket = NULL;
}

BEGIN_MESSAGE_MAP(CGroupSubscribe, CDialog)
	//{{AFX_MSG_MAP(CGroupSubscribe)
	ON_BN_CLICKED(IDC_DOWNLOAD_GROUPS, OnDownloadGroups)
	ON_LBN_SELCHANGE(IDC_NEWSGROUPS_LIST, OnSelect)
	ON_LBN_DBLCLK(IDC_NEWSGROUPS_LIST, OnDblclkNewsgroupsList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupSubscribe communications

void CGroupSubscribe::DoFillListBox()
{
	if (!m_bKeepAdding)
		return;
		
	m_btnOK.EnableWindow(FALSE);
	m_btnHelp.EnableWindow(FALSE);
	m_btnCancel.EnableWindow(FALSE);
	m_btnGetList.EnableWindow(FALSE);
		
	// Fill the list box with the newsgroup names eturned
	// from the server

	CString strResponse = ""; // Response string from server
	
	if (m_pSocket == NULL)
	{    
		AfxMessageBox(IDP_CONNECTION_BUSY, MB_ICONASTERISK);
		return;
	}

	m_pSocket->Send("LIST\r\n");
	strResponse = m_pSocket->GetLine();
	if (atoi(strResponse) == 215)
	{ 
		// Group names
		while ((strResponse = m_pSocket->GetLine()) != ".\r\n"
			&& m_pSocket->IsConnected() && m_bKeepAdding)
		{    
			m_pStatusBar->SetText("Adding newsgroup names...  Please wait.");
			// Set up a message pump so all Windows messages continue to
			// be sent so we don't hang up the user's system
		    for (;;)
		    {
				MSG msg;
				m_pStatusBar->SetText("Adding newsgroup names...  Please wait.");
				if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{ 
					if (!m_pApp->PreTranslateMessage(&msg))
					{
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				}
				else
				{
					break; // no more messages to pump
			    }
			}

			m_pStatusBar->SetText("Adding newsgroup names...  Please wait.");  
			int nLength = strResponse.Find(' ');
			strResponse = strResponse.Left(nLength);
			// Add group names to list box
			if (m_bKeepAdding)
			{
				if (m_ctlGroupList.AddString(strResponse) == LB_ERRSPACE)					
				{    
					m_bKeepAdding = FALSE;
					break;
				}
			}
			else
				break;
			
			// Set up a message pump so all Windows messages continue to
			// be sent so we don't hang up the user's system
		    for (;;)
		    {
				MSG msg; 
				if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{ 
					if (!m_pApp->PreTranslateMessage(&msg))
					{
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				}
				else
				{    
					// to correctly handle idle processing, call
					// CWinApp::OnIdle()
					m_pApp->OnIdle(0); // update UI
					m_pApp->OnIdle(1); // delete temp objects
					
					break; // no more messages to pump
			    }
			}
		}
	}
	
	m_pStatusBar->ShowIdleMessage();

	// TODO: Add FlashConnect support here.
	
	// Re-enable the buttons to show we're done
	m_btnOK.EnableWindow(TRUE);
	m_btnCancel.EnableWindow(TRUE);
	m_btnHelp.EnableWindow(TRUE);
	m_btnGetList.EnableWindow(TRUE);

	return;
}
						
/////////////////////////////////////////////////////////////////////////////
// CGroupSubscribe message handlers

void CGroupSubscribe::OnDownloadGroups()
{
	m_bKeepAdding = TRUE;
	
	DoFillListBox(); // Fills the listbox with names of UseNet newsgroups the
					// server returns
}

void CGroupSubscribe::OnDblclkNewsgroupsList()
{
	UpdateData(TRUE);
	m_strNewsgroup = m_strSelection;
    
    m_bKeepAdding = FALSE;
    
	m_pStatusBar->ShowIdleMessage();
		
	UpdateData(FALSE);
	
	CGroupSubscribe::OnOK();
}

void CGroupSubscribe::OnOK()
{
	UpdateData(TRUE);
    
    // Validate the user's newsgroup selection with the server
	// by making sure it's not empty
	if (m_strNewsgroup.IsEmpty()) // empty newsgroup name -- invalid
	{
		AfxMessageBox("Please type or select a newsgroup name.");
		return;
	} 

	EndDialog(IDOK);
}

void CGroupSubscribe::OnSelect()
{
	UpdateData(TRUE);
	m_strNewsgroup = m_strSelection;
    
    m_bKeepAdding = FALSE;
    
	m_pStatusBar->ShowIdleMessage();
		
	UpdateData(FALSE);
}

void CGroupSubscribe::OnCancel()
{
	m_pStatusBar->ShowIdleMessage();
	
	EndDialog(IDCANCEL);
}

/////////////////////////////////////////////////////////////////////////////
// CAddressBook dialog

CAddressBook::CAddressBook(CWnd* pParent /*=NULL*/) 
	: CDialog(CAddressBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddressBook)
	//}}AFX_DATA_INIT
}

void CAddressBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddressBook)
	DDX_Control(pDX, IDC_BCC_ADDRESS, m_btnBCC);
	DDX_Control(pDX, IDC_CC_ADDRESS, m_btnCC);
	DDX_Control(pDX, IDC_TO_ADDRESS, m_btnTo);
	DDX_Control(pDX, IDC_ADDRESS_LIST, m_ctlAddressList);
	DDX_LBString(pDX, IDC_ADDRESS_LIST, m_strSelection);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddressBook, CDialog)
	//{{AFX_MSG_MAP(CAddressBook)
	ON_BN_CLICKED(IDC_TO_ADDRESS, OnTo)
	ON_BN_CLICKED(IDC_CC_ADDRESS, OnCC)
	ON_BN_CLICKED(IDC_BCC_ADDRESS, OnBCC)
	ON_BN_CLICKED(IDC_ADD_ADDRESS, OnAddAddress)
	ON_BN_CLICKED(IDC_REMOVE_ADDRESS, OnRemoveAddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddressBook message handlers

BOOL CAddressBook::OnInitDialog()
{
	CDialog::OnInitDialog();	

	// Read in the addresses stored in the document, and put them in the
	// list

	for (int i = 0;i < m_pDoc->m_strAddressList.GetSize();i++)
	{
		m_ctlAddressList.AddString(m_pDoc->m_strAddressList[i]);
	}

	// If the m_pComposeWnd pointer is equal to NULL, gray the To, CC, and
	// BCC buttons
	m_btnTo.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
	m_btnCC.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
	m_btnBCC.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
	
	// Automatically select the first list item
	if (m_ctlAddressList.GetCount() > 0)
		m_ctlAddressList.SetCurSel(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
} 

void CAddressBook::OnTo()
{
	if (!UpdateData(TRUE))
		return;
		
	if (m_pComposeWnd == NULL) return;
	
	m_pComposeWnd->GetInfoBar().SetTO(m_strSelection);
	m_pComposeWnd->GetInfoBar().UpdateData(FALSE);	
}

void CAddressBook::OnCC()
{
	if (!UpdateData(TRUE))
		return;
		
	if (m_pComposeWnd == NULL) return;
	
	m_pComposeWnd->GetInfoBar().SetCC(m_strSelection);
	m_pComposeWnd->GetInfoBar().UpdateData(FALSE);	
}

void CAddressBook::OnBCC()
{
	if (!UpdateData(TRUE))
		return;
		
	if (m_pComposeWnd == NULL) return;
	
	m_pComposeWnd->GetInfoBar().SetBCC(m_strSelection);
	m_pComposeWnd->GetInfoBar().UpdateData(FALSE);
}

void CAddressBook::OnAddAddress()
{
    CAddAddress theDialog(this);
    
    if (theDialog.DoModal() == IDOK)
    {
    	m_pDoc->AddAddress(theDialog.m_strAddress);
    	m_ctlAddressList.AddString(theDialog.m_strAddress);
    	return;
    }  
    
	// Lastly, update the To, CC, and BCC buttons
	m_btnTo.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
	m_btnCC.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
	m_btnBCC.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
}

void CAddressBook::OnRemoveAddress()
{ 	
	int nIndex = m_ctlAddressList.GetCurSel();
	if (nIndex < 0)
	{
		// ask the user if they want to remove the first entry
		if (AfxMessageBox("Do you want to remove the first address?",	
				MB_ICONQUESTION|MB_YESNO) == IDYES)
			nIndex = 0;    
		else
			return;
	}
	
	m_pDoc->RemoveAddress(nIndex);   
	
	// Now delete the address from the listbox
	m_ctlAddressList.DeleteString(nIndex);
	
	// Lastly, update the To, CC, and BCC buttons
	m_btnTo.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
	m_btnCC.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
	m_btnBCC.EnableWindow(m_pComposeWnd && m_ctlAddressList.GetCount() > 0);
		
	return;			
}

/////////////////////////////////////////////////////////////////////////////
// CAddAddress dialog


CAddAddress::CAddAddress(CWnd* pParent /*=NULL*/)
	: CDialog(CAddAddress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddAddress)
	m_strAddress = "";
	//}}AFX_DATA_INIT
}

void CAddAddress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddAddress)
	DDX_Text(pDX, IDC_ADDRESS, m_strAddress);
	DDV_MaxChars(pDX, m_strAddress, 512);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddAddress, CDialog)
	//{{AFX_MSG_MAP(CAddAddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddAddress message handlers

void CAddAddress::OnOK()
{   
	// Validate the user's input!
	// Make sure the address contains an '@' and no spaces.
	BOOL bOK = FALSE;
	
	if (!UpdateData(TRUE))
		return;
	
	if (m_strAddress.Find('@') > 0)
		bOK = TRUE;
	else
	{      
		MessageBeep(-1);
		AfxMessageBox("E-Mail address must contain an '@' symbol.",
			MB_ICONSTOP|MB_OK); 
		return;
	}
	
	if (m_strAddress.Find(' ') <= -1 && bOK)
		EndDialog(IDOK);
	else
	{     
		MessageBeep(-1);
		AfxMessageBox("The address must contain no spaces.",
			MB_ICONSTOP|MB_OK);
		return;
	}
}     

/////////////////////////////////////////////////////////////////////////////
// CMailSignature dialog


CMailSignature::CMailSignature(CWnd* pParent /*=NULL*/)
	: CDialog(CMailSignature::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailSignature)
	m_strSignature = "";
	//}}AFX_DATA_INIT
}

void CMailSignature::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailSignature)
	DDX_Control(pDX, IDC_SIGNATURE, m_ctlSignature);
	DDX_Text(pDX, IDC_SIGNATURE, m_strSignature);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMailSignature, CDialog)
	//{{AFX_MSG_MAP(CMailSignature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailSignature message handlers

void CMailSignature::OnOK()
{ 
	if (m_ctlSignature.GetLineCount() > 4)
	{
		AfxMessageBox(IDP_INVALID_SIGNATURE, MB_ICONSTOP);
		return;
	}
		
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CArticleAge dialog


CArticleAge::CArticleAge(CWnd* pParent /*=NULL*/)
	: CDialog(CArticleAge::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArticleAge)
	m_lDays = 0;
	m_nHours = 0;
	m_nMinutes = 0;
	m_nSeconds = 0;
	//}}AFX_DATA_INIT
}

void CArticleAge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArticleAge)
	DDX_Text(pDX, IDC_AGE_DAYS, m_lDays);
	DDV_MinMaxLong(pDX, m_lDays, 0, 25000);
	DDX_Text(pDX, IDC_AGE_HOURS, m_nHours);
	DDV_MinMaxInt(pDX, m_nHours, 0, 23);
	DDX_Text(pDX, IDC_AGE_MINUTES, m_nMinutes);
	DDV_MinMaxInt(pDX, m_nMinutes, 0, 59);
	DDX_Text(pDX, IDC_AGE_SECONDS, m_nSeconds);
	DDV_MinMaxInt(pDX, m_nSeconds, 0, 59);
	//}}AFX_DATA_MAP     
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CArticleAge, CDialog)
	//{{AFX_MSG_MAP(CArticleAge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArticleAge message handlers
/////////////////////////////////////////////////////////////////////////////
// CInetnavWelcomeDlg dialog


CInetnavWelcomeDlg::CInetnavWelcomeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInetnavWelcomeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInetnavWelcomeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CInetnavWelcomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetnavWelcomeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP       
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CInetnavWelcomeDlg, CDialog)
	//{{AFX_MSG_MAP(CInetnavWelcomeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInetnavWelcomeDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CFTPUserDir dialog


CFTPUserDir::CFTPUserDir(CWnd* pParent /*=NULL*/)
	: CDialog(CFTPUserDir::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFTPUserDir)
	m_strDirectory = "";
	//}}AFX_DATA_INIT
}

void CFTPUserDir::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFTPUserDir)
	DDX_Text(pDX, IDC_DIRECTORY, m_strDirectory);
	//}}AFX_DATA_MAP 
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CFTPUserDir, CDialog)
	//{{AFX_MSG_MAP(CFTPUserDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFTPUserDir message handlers
/////////////////////////////////////////////////////////////////////////////
// CFlashConnectBegin dialog


CFlashConnectBegin::CFlashConnectBegin(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashConnectBegin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlashConnectBegin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFlashConnectBegin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlashConnectBegin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlashConnectBegin, CDialog)
	//{{AFX_MSG_MAP(CFlashConnectBegin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFlashConnectBegin message handlers
/////////////////////////////////////////////////////////////////////////////
// CFlashConnectEnd dialog


CFlashConnectEnd::CFlashConnectEnd(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashConnectEnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlashConnectEnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFlashConnectEnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlashConnectEnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlashConnectEnd, CDialog)
	//{{AFX_MSG_MAP(CFlashConnectEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFlashConnectEnd message handlers
/////////////////////////////////////////////////////////////////////////////
// COpenFtpSite dialog


COpenFtpSite::COpenFtpSite(CWnd* pParent /*=NULL*/)
	: CDialog(COpenFtpSite::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenFtpSite)
	m_strSite = "ftp.somesite.com";
	m_bUseAnonFTP = TRUE;
	//}}AFX_DATA_INIT
}

void COpenFtpSite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenFtpSite)
	DDX_Control(pDX, IDC_USE_ANON_FTP, m_ctlUseAnonFTP);
	DDX_Control(pDX, IDC_SITE_ADDRESS, m_ctlSiteAddress);
	DDX_Text(pDX, IDC_SITE_ADDRESS, m_strSite);
	DDX_Check(pDX, IDC_USE_ANON_FTP, m_bUseAnonFTP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COpenFtpSite, CDialog)
	//{{AFX_MSG_MAP(COpenFtpSite)
	ON_EN_CHANGE(IDC_SITE_ADDRESS, OnChangeSiteAddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenFtpSite message handlers

void COpenFtpSite::OnChangeSiteAddress()
{   
	// If the user types "pweb.netcom.com" in the text box, disable the
	// 'Use Anonymous FTP' check box
	CString rString = "";
	m_ctlSiteAddress.GetWindowText(rString);
	
	if (rString == "pweb.netcom.com")
	{
		UpdateData(TRUE);
		m_bUseAnonFTP = FALSE;
		UpdateData(FALSE);
	}
		
	m_ctlUseAnonFTP.EnableWindow(rString != "pweb.netcom.com");
}

/////////////////////////////////////////////////////////////////////////////
// CArticleSignature dialog


CArticleSignature::CArticleSignature(CWnd* pParent /*=NULL*/)
	: CDialog(CArticleSignature::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArticleSignature)
	m_strSignature = "";
	//}}AFX_DATA_INIT
}

void CArticleSignature::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArticleSignature)
	DDX_Control(pDX, IDC_SIGNATURE, m_ctlSignature);
	DDX_Text(pDX, IDC_SIGNATURE, m_strSignature);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CArticleSignature, CDialog)
	//{{AFX_MSG_MAP(CArticleSignature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArticleSignature message handlers

void CArticleSignature::OnCancel()
{
	CDialog::OnCancel();
}

void CArticleSignature::OnOK()
{
	if (m_ctlSignature.GetLineCount() > 6)
	{
		AfxMessageBox(IDP_INVALID_SIGNATURE, MB_ICONSTOP);
		return;
	}

	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CDirectoryDialog dialog


CDirectoryDialog::CDirectoryDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
        LPCSTR lpszDefExt,
        LPCSTR lpszFileName,
        DWORD dwFlags,
        LPCSTR lpszFilter,
        CWnd* pParentWnd) 
	: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
                                 dwFlags, lpszFilter, pParentWnd)
{
	//{{AFX_DATA_INIT(CDirectoryDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT 
	m_pParentWnd = pParentWnd;
}

void CDirectoryDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirectoryDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDirectoryDialog, CFileDialog)
	//{{AFX_MSG_MAP(CDirectoryDialog)
		ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDirectoryDialog message handlers

BOOL CDirectoryDialog::OnInitDialog()
{

	//Let's hide these windows so the user cannot tab to them.  Note that in
	//the private template (in cddemo.dlg) the coordinates for these guys are
	//*outside* the coordinates of the dlg window itself.  Without the following
	//ShowWindow()'s you would not see them, but could still tab to them.
    
	GetDlgItem(stc2)->ShowWindow(SW_HIDE);
   	GetDlgItem(stc3)->ShowWindow(SW_HIDE);
   	GetDlgItem(edt1)->ShowWindow(SW_HIDE);
   	GetDlgItem(lst1)->ShowWindow(SW_HIDE);
   	GetDlgItem(cmb1)->ShowWindow(SW_HIDE);
    
	//We must put something in this field, even though it is hidden.  This is
	//because if this field is empty, or has something like "*.txt" in it,
	//and the user hits OK, the dlg will NOT close.  We'll jam something in
	//there (like "Junk") so when the user hits OK, the dlg terminates.
	//Note that we'll deal with the "Junk" during return processing (see below)

	//Now set the focus to the directories listbox.  Due to some painting
	//problems, we *must* also process the first WM_PAINT that comes through
	//and set the current selection at that point.  Setting the selection
	//here will NOT work.  See comment below in the on paint handler.
            
   	GetDlgItem(lst2)->SetFocus();
            
   	m_bDlgJustCameUp=TRUE;
             
   	CFileDialog::OnInitDialog();
   
   	return FALSE;
 
}	

void CDirectoryDialog::OnPaint()
{
    CPaintDC dc(this); // device context for painting

	//This code makes the directory listbox "highlight" an entry when it first
	//comes up.  W/O this code, the focus is on the directory listbox, but no
	//focus rectangle is drawn and no entries are selected.  Ho hum.

     if (m_bDlgJustCameUp)
     {
        m_bDlgJustCameUp=FALSE;
        SendDlgItemMessage(lst2, LB_SETCURSEL, 0, 0L);
     }
    
    // NOTE: Do NOT call CFileDialog::OnPaint() for painting messages
}

/////////////////////////////////////////////////////////////////////////////
// CDLComplete dialog


CDLComplete::CDLComplete(CWnd* pParent /*=NULL*/)
	: CDialog(CDLComplete::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLComplete)
	m_strFileDL = "";
	//}}AFX_DATA_INIT
}

void CDLComplete::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLComplete)
	DDX_Text(pDX, IDC_FILE_DOWNLOADED, m_strFileDL);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDLComplete, CDialog)
	//{{AFX_MSG_MAP(CDLComplete)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDLComplete message handlers
/////////////////////////////////////////////////////////////////////////////
// CFtpDisconnected dialog


CFtpDisconnected::CFtpDisconnected(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpDisconnected::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpDisconnected)
	m_strMsg = "";
	//}}AFX_DATA_INIT
}

void CFtpDisconnected::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpDisconnected)
	DDX_Text(pDX, IDC_FILE_DOWNLOADED, m_strMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFtpDisconnected, CDialog)
	//{{AFX_MSG_MAP(CFtpDisconnected)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFtpDisconnected message handlers

BOOL CFtpDisconnected::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString strWindowText = "Disconnected From " + m_strSiteName;
	SetWindowText(strWindowText);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
/////////////////////////////////////////////////////////////////////////////
// CFileDownload dialog


CFileDownload::CFileDownload(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDownload::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDownload)
	m_strFileName = "";
	//}}AFX_DATA_INIT
}

void CFileDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDownload)
	DDX_Text(pDX, IDC_FILE_NAME, m_strFileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileDownload, CDialog)
	//{{AFX_MSG_MAP(CFileDownload)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFileDownload message handlers
/////////////////////////////////////////////////////////////////////////////
// CConnectMsg dialog


CConnectMsg::CConnectMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CConnectMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectMsg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConnectMsg, CDialog)
	//{{AFX_MSG_MAP(CConnectMsg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConnectMsg message handlers
/////////////////////////////////////////////////////////////////////////////
// CAddrChoose dialog


CAddrChoose::CAddrChoose(CWnd* pParent, CComposeView* pView)
	: CDialog(CAddrChoose::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddrChoose)
	m_strAddress = "";
	//}}AFX_DATA_INIT       
	m_pComposeView = pView;
}

void CAddrChoose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddrChoose)
	DDX_Control(pDX, IDC_ADDRESS_LIST, m_ctlAddress);
	DDX_LBString(pDX, IDC_ADDRESS_LIST, m_strAddress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddrChoose, CDialog)
	//{{AFX_MSG_MAP(CAddrChoose)
	ON_LBN_DBLCLK(IDC_ADDRESS_LIST, OnDblclkAddressList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddrChoose message handlers

BOOL CAddrChoose::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CMailBoxDoc* pDoc = m_pComposeView->GetDocument();
	
	// load in the address book from the document and populate the list box
	// with the address book's contents

	for (int i = 0;i < pDoc->m_strAddressList.GetSize();i++)
	{
		m_ctlAddress.AddString(pDoc->m_strAddressList[i]);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAddrChoose::OnDblclkAddressList()
{
	// If the user double-clicked an address, let's just pretend
	// the user has selected an address and pressed the OK button
	CDialog::OnOK();
} 

void CAddrChoose::OnAdd()
{
    CAddAddress theDialog(this);
    CMailBoxDoc* pDoc = m_pComposeView->GetDocument();
    
    if (theDialog.DoModal() == IDOK)
    {
    	pDoc->AddAddress(theDialog.m_strAddress);
    	m_ctlAddress.AddString(theDialog.m_strAddress);   
    	pDoc->DoSave(pDoc->GetPathName());
    	return;
    }  
}

void CAddrChoose::OnRemove()
{
	CMailBoxDoc* pDoc = m_pComposeView->GetDocument();
	
	int nIndex = m_ctlAddress.GetCurSel();
	if (nIndex < 0)
	{
		// ask the user if they want to remove the first entry
		if (AfxMessageBox("Do you want to remove the first address?",	
				MB_ICONQUESTION|MB_YESNO) == IDYES)
			nIndex = 0;    
		else
			return;
	}
	
	pDoc->RemoveAddress(nIndex);
	pDoc->DoSave(pDoc->GetPathName());   
	
	// Now delete the address from the listbox
	m_ctlAddress.DeleteString(nIndex);
}

/////////////////////////////////////////////////////////////////////////////
// COpenHelpDlg dialog


COpenHelpDlg::COpenHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenHelpDlg)
	m_nSelection = 1;
	//}}AFX_DATA_INIT
}

void COpenHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenHelpDlg)
	DDX_Radio(pDX, IDC_OPEN_CONTENTS, m_nSelection);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COpenHelpDlg, CDialog)
	//{{AFX_MSG_MAP(COpenHelpDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenHelpDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CFtpPrefsAdvanced dialog


CFtpPrefsAdvanced::CFtpPrefsAdvanced(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpPrefsAdvanced::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpPrefsAdvanced)     
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_strDLDir = m_pApp->GetFTPDownloadDir();
	m_strULDir = m_pApp->GetFTPUploadDir();
	//}}AFX_DATA_INIT
}

void CFtpPrefsAdvanced::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpPrefsAdvanced)
	DDX_Text(pDX, IDC_DOWNLOAD_DIR, m_strDLDir);
	DDX_Text(pDX, IDC_UPLOAD_DIR, m_strULDir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFtpPrefsAdvanced, CDialog)
	//{{AFX_MSG_MAP(CFtpPrefsAdvanced)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_DIRBROWSE, OnDirbrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFtpPrefsAdvanced message handlers

void CFtpPrefsAdvanced::OnDirbrowse()
{
	CString strDir1 = m_pApp->GetFTPUploadDir();
	CString strDir2 = m_pApp->DoDirectoryDialog(this, strDir1);
	
	m_strULDir = strDir2;
	UpdateData(FALSE);
	
	return;	
}

void CFtpPrefsAdvanced::OnBrowse()
{
	CString strDir1 = m_pApp->GetFTPDownloadDir();
	CString strDir2 = m_pApp->DoDirectoryDialog(this, strDir1);
	
	m_strDLDir = strDir2;
	UpdateData(FALSE);
	
	return;	
}

void CFtpPrefsAdvanced::OnOK()
{
	if (!UpdateData(TRUE))
		return;
	
	m_pApp->SetFTPDownloadDir(m_strDLDir); 
	m_pApp->SetFTPUploadDir(m_strULDir);
	m_pApp->Serialize(TRUE);
	
	CDialog::OnOK();
}                                       

/////////////////////////////////////////////////////////////////////////////
// CFlashConnectAbort dialog


CFlashConnectAbort::CFlashConnectAbort(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashConnectAbort::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlashConnectAbort)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFlashConnectAbort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlashConnectAbort)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlashConnectAbort, CDialog)
	//{{AFX_MSG_MAP(CFlashConnectAbort)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFlashConnectAbort message handlers
/////////////////////////////////////////////////////////////////////////////
// CInternetMailboxes dialog


CInternetMailboxes::CInternetMailboxes(CWnd* pParent /*=NULL*/)
	: CDialog(CInternetMailboxes::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInternetMailboxes)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CInternetMailboxes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInternetMailboxes)
	DDX_Control(pDX, IDC_MAILBOX_LIST, m_ctlMailboxList);
	DDX_Control(pDX, IDC_SET_DEFAULT_MAILBOX, m_btnSetDefMailbox);
	DDX_LBString(pDX, IDC_MAILBOX_LIST, m_strSelection);
	DDX_Text(pDX, IDC_DEFAULT_MAILBOX, m_strDefMailbox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInternetMailboxes, CDialog)
	//{{AFX_MSG_MAP(CInternetMailboxes)
	ON_BN_CLICKED(IDC_ADD_MAILBOX, OnAddMailbox)
	ON_BN_CLICKED(IDC_REMOVE_MAILBOX, OnRemoveMailbox)
	ON_BN_CLICKED(IDC_SET_DEFAULT_MAILBOX, OnSetDefaultMailbox)
	ON_LBN_DBLCLK(IDC_MAILBOX_LIST, OnDblclkMailboxList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInternetMailboxes message handlers

void CInternetMailboxes::OnAddMailbox()
{
	CWaitCursor wait;
	
	if (!m_pApp->AddNewMailbox())
		return;
	
	m_ctlMailboxList.ResetContent();
	
	for (int i = 0;i < m_pApp->m_strMailboxNameList.GetSize();i++)
	{
		m_ctlMailboxList.AddString(m_pApp->m_strMailboxNameList[i]);
	}            
	
	m_strDefMailbox = m_pApp->m_strDefMailboxName;
	UpdateData(FALSE);
	
	// Save mailboxes
	m_pApp->SaveMailboxes();
}

void CInternetMailboxes::OnRemoveMailbox()
{
	CWaitCursor wait;
	
	if (UpdateData(TRUE))
	{
		if (m_ctlMailboxList.GetCurSel() < 0)
			return;
		
		if (!m_pApp->RemoveMailboxName(m_strSelection))
			return;
		
		m_ctlMailboxList.DeleteString(m_ctlMailboxList.GetCurSel());
		
		m_strDefMailbox = m_pApp->m_strDefMailboxName;
		
		m_pApp->SaveMailboxes();
	}
}

void CInternetMailboxes::OnSetDefaultMailbox()
{
	if (UpdateData(TRUE))
	{
		if (m_ctlMailboxList.GetCurSel() < 0)
			return;
		
		CString strPath = m_pApp->GetMailboxPath(m_strSelection);
		CString strAddr = m_pApp->GetMailboxAddress(m_strSelection);
		CString strUser = m_pApp->GetMailboxUser(m_strSelection);
		CString strOrg = m_pApp->GetMailboxOrg(m_strSelection);
		CString strName = m_strSelection;
		
		m_pApp->SetDefaultMailbox(strName, strPath, strAddr, strUser, strOrg);
		m_strDefMailbox = m_strSelection;
		UpdateData(FALSE);
	}			
}

void CInternetMailboxes::OnDblclkMailboxList()
{
	OnSetDefaultMailbox();
}

BOOL CInternetMailboxes::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Fill the listbox with names of mailboxes
	m_pApp->LoadMailboxes();
		
	for (int i = 0;i < m_pApp->m_strMailboxNameList.GetSize();i++)
	{
		m_ctlMailboxList.AddString(m_pApp->m_strMailboxNameList[i]);
	}          
	
	m_ctlMailboxList.SetCurSel(0);
			
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInternetMailboxes::OnOK()
{
	CDialog::OnOK();
}       

/////////////////////////////////////////////////////////////////////////////
// CMailUserInfo dialog


CMailUserInfo::CMailUserInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CMailUserInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailUserInfo)
	m_strOrg = "";
	m_strUser = "";
	//}}AFX_DATA_INIT
}

void CMailUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailUserInfo)
	DDX_Text(pDX, IDC_COMPANY, m_strOrg);
	DDX_Text(pDX, IDC_USER, m_strUser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMailUserInfo, CDialog)
	//{{AFX_MSG_MAP(CMailUserInfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailUserInfo message handlers
/////////////////////////////////////////////////////////////////////////////
// CFtpViruses dialog


CFtpViruses::CFtpViruses(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpViruses::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpViruses)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CFtpViruses::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpViruses)
	DDX_Text(pDX, IDC_VIRUSCAN_PATH, m_strPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFtpViruses, CDialog)
	//{{AFX_MSG_MAP(CFtpViruses)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_DISCLAIMER, OnDisclaimer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFtpViruses message handlers

void CFtpViruses::OnOK()
{
	if (UpdateData(TRUE))
	{
		m_pApp->SetVirusScannerPath(m_strPath);
		
		EndDialog(IDOK);
	}
}

void CFtpViruses::OnBrowse()
{
	BeginWaitCursor();
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	CString strFilter = "Programs|*.exe|Command|*.com|Batch|*.bat|Information|*.pif||";
	// this button allows the user to open a dialog box similar to
	// file open or file save, and then the filename returned is 
	// entered into the command line edit box; which is run when
	// the user clicks OK
	CAppFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_CREATEPROMPT, 
	strFilter, this);
	
	dlg.m_ofn.lpstrTitle = "Browse";
	
	if (dlg.DoModal() == IDOK)
	{
		// get the filename the user selected, and then show it
		// in the command line edit box. 
		UpdateData(TRUE);
		m_strPath = dlg.GetPathName();
		UpdateData(FALSE);
	}		 	
}

void CFtpViruses::OnDisclaimer()
{
	CString str = "Internet Navigator attempts to offer you some protection";
	str += " against Internet viruses by allowing you to run a virus scan ";
	str += "program before opening a file you have downloaded with FTP. ";
	str += " WnDBSoft Software International will not be held ";
	str += "liable for damage caused to your system by viruses.  ";
	str += "Remember, Internet Navigator does not force you to download ";
	str += "any file from the Internet; you have the freedom to choose ";
	str += "which files are sent to your computer on the Internet.";   
	
	MessageBeep(-1);
	MessageBox(str, "Disclaimer", MB_ICONASTERISK);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFtpVirusScan dialog


CFtpVirusScan::CFtpVirusScan(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpVirusScan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpVirusScan)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	//}}AFX_DATA_INIT
}

void CFtpVirusScan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpVirusScan)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFtpVirusScan, CDialog)
	//{{AFX_MSG_MAP(CFtpVirusScan)
	ON_BN_CLICKED(IDABORT, OnAbort)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFtpVirusScan message handlers

void CFtpVirusScan::OnAbort()
{
	EndDialog(IDABORT);	// tells CFtpView to delete file
}

BOOL CFtpVirusScan::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetTimer(1, 1000, NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFtpVirusScan::OnTimer(UINT nIDEvent)
{
	// Run the user's virus scanner
	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);	
		
		// Run the virus scanner
		::WinExec(m_pApp->GetVirusScannerPath(), SW_SHOW);
	}
	else
		CDialog::OnTimer(nIDEvent);
}
/////////////////////////////////////////////////////////////////////////////
// CArticleOptions dialog


CArticleOptions::CArticleOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CArticleOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArticleOptions)
	m_nDisplay = -1;
	//}}AFX_DATA_INIT
}

void CArticleOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArticleOptions)
	DDX_Radio(pDX, IDC_ARTICLE_DISPLAY, m_nDisplay);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CArticleOptions, CDialog)
	//{{AFX_MSG_MAP(CArticleOptions)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArticleOptions message handlers
