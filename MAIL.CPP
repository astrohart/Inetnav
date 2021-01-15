// mail.cpp: implementation file for Mail

#include "stdafx.h"
#include "inetnav.h" 
#include <stdio.h>

#include "dialogs.h" 
#include "wizards.h"
#include "propshts.h" 

#include "mail.h" 
#include "news.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailBoxDoc

IMPLEMENT_DYNCREATE(CMailBoxDoc, CDocument)

BEGIN_MESSAGE_MAP(CMailBoxDoc, CDocument)
	//{{AFX_MSG_MAP(CMailBoxDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_MAILBOX_SETUP, OnMailboxSetup)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_MAILBOX_LOGON, OnMailboxLogon)
	ON_COMMAND(ID_MAIL_SETTINGS, OnMailSettings)
	ON_COMMAND(ID_MAIL_SIGNATURE, OnMailSignature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailBoxDoc construction/destruction

CMailBoxDoc::CMailBoxDoc()
{
	//{{AFX_DATA_INIT(CMailBoxDoc)    
	m_strSMTPHost = "";
	m_strAddress = "";
	m_strPOP3Host = "";                       
	m_strPOP3Logon = "";
	//}}AFX_DATA_INIT  
	m_pApp = (CInternetNavApp*)AfxGetApp();   
	m_strPathName.Empty();   
	m_strPassword.Empty();
	
	m_bInitialized = TRUE; // TRUE unless set to FALSE   
	m_bDefault = FALSE;
}

/*
//{{AFX_DATA_MAP(CMailBoxDoc)
//}}AFX_DATA_MAP
*/
CMailBoxDoc::~CMailBoxDoc()
{
	if (m_nDeleArray.GetSize() > 0)
	{
		m_nDeleArray.FreeExtra();
		m_nDeleArray.RemoveAll();
	}
}

BOOL CMailBoxDoc::OnNewDocument()
{   
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	if (!CDocument::OnNewDocument()) 
	{
		AfxMessageBox(IDP_FAILED_TO_CREATE_MAILBOX);
		m_bInitialized = FALSE;
		return FALSE;                               
	}

	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
		m_pApp->GetMainFrame()->ShowWindow(SW_SHOWMINNOACTIVE);
    
    m_mailData.Attach(this, m_pApp);
    
    m_bInitialized = FALSE;
    // Let user set the title of the new mailbox
    CMailBoxTitleDlg titleDlg(pParent);
    CString strFileName;
    
    switch (titleDlg.DoModal())
    {
    	case IDOK:
    		m_strMailBoxTitle = titleDlg.m_strMailBoxTitle;
    		strFileName = titleDlg.m_strFileName;
    		
    		if (strFileName.IsEmpty())
    			strFileName = "INTERNET.MAI";
    		break;
    	
    	case IDCANCEL:
    		m_bInitialized = FALSE;
    		AfxMessageBox(IDP_FAILED_TO_CREATE_MAILBOX, MB_ICONSTOP);
    		return FALSE;
    }
    
    // let the user setup the new mailbox
    OnMailboxSetup();
    
    if (m_bInitialized)
    {   
    	SetPathName(m_pApp->GetFilesDirectory() + strFileName);

    	int nAt = m_strAddress.Find('@');
		m_strDomainName = m_strAddress.Right(m_strAddress.GetLength() - nAt - 1);
		m_strPOP3Logon = m_strAddress.Left(nAt);
		 
		SetModifiedFlag(TRUE);
		
		SetTitle(m_strMailBoxTitle); 
		
        m_strSignature = "Sincerely,\r\n";
        m_strSignature += m_strUserName + "\r\n";
        m_strSignature += m_strOrg;
        
	    CMailSignature theDialog(pParent);
	    theDialog.m_strSignature = m_strSignature;
	    
	    if (theDialog.DoModal() == IDOK)
	    {
	    	// save the user's signature
	    	m_strSignature = theDialog.m_strSignature;
	    	m_bInitialized = TRUE; 
	    	
	    	// Now save the area data file to the user's hard drive
	    	if (!DoSave(GetPathName(), TRUE))
	    	{
	    		MessageBeep(-1);
	    		AfxMessageBox(AFX_IDP_FAILED_TO_SAVE_DOC, MB_ICONSTOP);
	    		return FALSE;
	    	}
	    	else
	    	{
	    		// Register this new mailbox with the application
	    		if (!m_pApp->RegisterMailbox(m_strMailBoxTitle,
	    				m_strPathName, m_strAddress, m_strUserName, m_strOrg))
	    		{
	    			return FALSE;
	    		}
				
				if (m_pApp->m_lpCmdLine[0] == '/' && 
						m_pApp->m_lpCmdLine[1] == 'S')
				{
					// Automatically set this mailbox as the default
					m_pApp->SetDefaultMailbox(m_strMailBoxTitle,
							GetPathName(), m_strAddress, m_strUserName,
								m_strOrg); 
								
					m_pApp->SaveMailboxes();
								
					return TRUE;
				}
				
				switch(AfxMessageBox(IDP_MAKE_MAILBOX_DEFAULT,
					MB_YESNO|MB_ICONQUESTION))
				{
					case IDYES:
						m_pApp->SetDefaultMailbox(m_strMailBoxTitle,
							GetPathName(), m_strAddress, m_strUserName, m_strOrg);
						AfxMessageBox("\'" + m_strMailBoxTitle + "\' "
							"is now the default mailbox.", MB_ICONASTERISK); 
						m_bDefault = TRUE;
						break;
			
					case IDNO:
						break;
				}
				
				m_pApp->SaveMailboxes();
				
				return TRUE;
	    	}
	    }

	    if (!m_bInitialized)
	 		AfxMessageBox(IDP_FAILED_TO_CREATE_MAILBOX, MB_ICONSTOP);
	 	
		return m_bInitialized;
	}       
	else
	{
	  	AfxMessageBox(IDP_FAILED_TO_CREATE_MAILBOX);
	  	
		m_strPathName.Empty();
        
        m_bInitialized = FALSE;
		return FALSE;
	}
}

BOOL CMailBoxDoc::OnOpenDocument(const char* pszPathName)
{   
	m_mailData.Attach(this, m_pApp);
	
	if (!CDocument::OnOpenDocument(pszPathName))
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_TO_OPEN_MAILBOX, MB_ICONSTOP|MB_OK);
		m_bInitialized = FALSE;
		return FALSE;            
	}                 
	
	SetTitle(m_strMailBoxTitle);
	 
	if (m_strPassword.IsEmpty())
	{
		// Get the user's password and user name so we don't have to repeatedly
		// ask the user every time we log on to the POP3 host
		CPOP3Dialog theDialog(CWnd::GetActiveWindow());
		theDialog.m_strUserID = m_strPOP3Logon;
		theDialog.m_strPassword = m_strPassword;
	
		if (theDialog.DoModal() == IDOK)
		{      
			m_strPOP3Logon = theDialog.m_strUserID;
			m_strPassword = theDialog.GetPassword();
			m_bInitialized = TRUE;
			SetTitle(m_strMailBoxTitle);            
		}
		else
			m_bInitialized = FALSE;
	}
	
	SetTitle(m_strMailBoxTitle);
	
	if (!m_bInitialized)
		AfxMessageBox(IDP_FAILED_TO_OPEN_MAILBOX, MB_ICONSTOP|MB_OK);
	
	m_bDefault = m_pApp->IsDefaultMailbox(m_strMailBoxTitle, GetPathName());
		
	return m_bInitialized;
}

BOOL CMailBoxDoc::OnSaveDocument(const char* pszPathName)
{
	if (!CDocument::OnSaveDocument(pszPathName))
		return FALSE;
		
	m_bInitialized = TRUE;        
	SetTitle(m_strMailBoxTitle);
	                                
	return TRUE;
}

void CMailBoxDoc::OnCloseDocument()
{
	m_mailData.Detach();
	
	CDocument::OnCloseDocument();
	return;
}

BOOL CMailBoxDoc::SaveModified()
{
	if (!IsModified())
		return TRUE;        // ok to continue

	CString strName = "your mailbox information";

	CString prompt = "";
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, strName); 
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL|MB_ICONQUESTION, 
				AFX_IDP_ASK_TO_SAVE))
	{
		case IDCANCEL:
			return FALSE;       // don't continue
	
		case IDYES:
			// If so, either Save or Update, as appropriate
			if (_access(m_strPathName, 6) != 0)
			{
				if (!CDocument::DoSave(NULL))
					return FALSE;   // don't continue
			}
			else
			{
				if (!CDocument::DoSave(m_strPathName))
					return FALSE;   // don't continue
			}
			break;
	
		case IDNO:
			// If not saving changes, revert the document
			break;
    
    	default:
			ASSERT(FALSE);
			break;
	}
	
	return TRUE;    // keep going
}

void CMailBoxDoc::SetTitle(const char* pszTitle)
{
	m_strTitle = m_strMailBoxTitle;
	UpdateFrameCounts();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxDoc data handling: serialization to/from a file, and deleting data

void CMailBoxDoc::Serialize(CArchive& ar)
{   
	// load/store the Address List first
	m_strAddressList.Serialize(ar);
	                   
	if (ar.IsStoring())
	{
		ar << m_strAddress << m_strDomainName << m_strSMTPHost;
		ar << m_strPOP3Host << m_strMailBoxTitle<< m_strPOP3Logon;  
		ar << m_strPassword << m_strSignature << m_strUserName << m_strOrg;
	}
	else
	{
		ar >> m_strAddress >> m_strDomainName >> m_strSMTPHost;
		ar >> m_strPOP3Host >> m_strMailBoxTitle >> m_strPOP3Logon;  
		ar >> m_strPassword >> m_strSignature >> m_strUserName >> m_strOrg;
		SetTitle(m_strMailBoxTitle);
	}
}   

void CMailBoxDoc::DeleteContents()
{   
	// This function is called to delete the document data. 
	m_strSMTPHost.Empty();
	m_strPOP3Host.Empty();
	m_strAddress.Empty();
	m_strPOP3Logon.Empty();
	m_strPassword.Empty();   
	m_strSignature.Empty(); 
	m_strUserName.Empty();
	m_strOrg.Empty();
	
	if (m_strAddressList.GetSize() > 0)
	{
		m_strAddressList.FreeExtra();
		m_strAddressList.RemoveAll();
	}
	
	return;
}

void CMailBoxDoc::AddAddress(CString& strAddress)
{
	if (strAddress.IsEmpty())
		return;		// can't add an empty address to the array!
		
	m_strAddressList.Add(strAddress);
	SetModifiedFlag();  // mark document as changed
	return;
}

void CMailBoxDoc::RemoveAddress(int nAddress)
{
	// Now call CStringArray::RemoveAt(nIndex) to remove the address;
	m_strAddressList.RemoveAt(nAddress);  
	m_strAddressList.FreeExtra(); // Free any extra memory
	SetModifiedFlag(); // mark document as changed
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxDoc diagnostics

#ifdef _DEBUG
void CMailBoxDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMailBoxDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMailBoxDoc commands

void CMailBoxDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// Enable the File Save command only if our data has been changed
	// by the user
	pCmdUI->Enable(IsModified());
}

void CMailBoxDoc::OnMailboxSetup()
{       
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	// This function presents the user with the Mailbox Setup Wizard
	// The Mailbox Setup Wizard guides the user through the process of
	// setting up this mailbox with their e-mail address, and the addresses
	// of their Internet Service Provider's e-mail SMTP and POP3 servers
	CMailBoxSetupWiz1 wiz1(pParent, this);
	
	m_bDefault = m_pApp->IsDefaultMailbox(m_strMailBoxTitle, GetPathName());
	
	switch (wiz1.DoModal())
	{
		case IDOK:
			ShowWizardChoiceStep();
			break;
		
		case IDCANCEL: 
			m_bInitialized = FALSE;
			break;
	}
	return;		
}

void CMailBoxDoc::ShowWizardStep3()
{ 
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	CMailBoxSetupWiz3 wiz3(pParent, this);
	
	switch (wiz3.DoModal())
	{
		case IDRETRY: // Back button pressed
			SetModifiedFlag();
			ShowWizardChoiceStep();
			break;
		
		case IDOK: // Next button pressed
			SetModifiedFlag();
			ShowWizardStep4();
			break;
			
		case IDCANCEL: 
			m_bInitialized = FALSE;
			break;
	}
	return;
}

void CMailBoxDoc::ShowWizardStep4()
{
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	CMailBoxSetupWiz4 wiz4(pParent, this);
	
	switch (wiz4.DoModal())
	{
		case IDRETRY: // Back button pressed
			SetModifiedFlag();
			ShowWizardStep3();
			break;
		
		case IDOK: // Next button pressed
			SetModifiedFlag();
			ShowWizardStep5();
			break;
		
		case IDCANCEL: 
			m_bInitialized = FALSE;
			break;
	}
	return;
}

void CMailBoxDoc::ShowWizardStep5()
{
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	CMailBoxSetupWiz5 wiz5(pParent, this);
	
	switch (wiz5.DoModal())
	{
		case IDRETRY: // Back button pressed
			SetModifiedFlag();
			ShowWizardStep4();
			break;
		
		case IDOK: // Next button pressed  
			SetModifiedFlag();
			ShowWizardStep6();
			break;
		
		case IDCANCEL: 
			m_bInitialized = FALSE;
			break;
	}
	return;
}

void CMailBoxDoc::ShowWizardStep6()
{
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	CMailBoxSetupWiz6 wiz6(pParent, this);
	
	switch (wiz6.DoModal())
	{
		case IDRETRY: // Back button pressed
			SetModifiedFlag();
			ShowWizardStep5();
			break;
		
		case IDOK: // Next button pressed  
			SetModifiedFlag();
			ShowWizardStep7();
			break;
		
		case IDCANCEL: 
			m_bInitialized = FALSE;
			break;
	}
	return;
}

void CMailBoxDoc::ShowWizardStep7()
{   
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	CMailBoxSetupWiz7 wiz7(pParent, this);
	CMailUserInfo theDialog(pParent);
	
	switch (wiz7.DoModal())
	{
		case IDRETRY: // Back button pressed
			SetModifiedFlag();
			ShowWizardStep6();
			break;
		
		case IDOK: // Finish button pressed 
	        theDialog.m_strUser = m_pApp->GetUserName();
	        theDialog.m_strOrg = m_pApp->GetUserCompany();
	        
	        if (theDialog.DoModal() == IDOK)
	        {
	        	m_strUserName = theDialog.m_strUser;
	        	m_strOrg = theDialog.m_strOrg;
	        	
				SetModifiedFlag();
				m_bInitialized = TRUE;
	        } 
	        else
	        	m_bInitialized = FALSE;
			break;
		
		case IDCANCEL: 
			m_bInitialized = FALSE;
			break;
	}
	return;
}

void CMailBoxDoc::OnFileSave()
{	
	CDocument::OnFileSave();
	
	SetTitle(GetMailBoxTitle());
	return;
}

void CMailBoxDoc::OnFileSaveAs()
{
	if ((AfxGetApp()->m_lpCmdLine[0] == '/')
		&& (AfxGetApp()->m_lpCmdLine[1] == 'S'))
	{
		AfxGetMainWnd()->ShowWindow(SW_SHOWMINNOACTIVE);
		AfxGetMainWnd()->UpdateWindow();   
	}
	
	CDocument::OnFileSaveAs();
	
	SetTitle(GetMailBoxTitle());
	return;
}

void CMailBoxDoc::OnMailboxLogon()
{    
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	// Get the user's password and user name so we don't have to repeatedly
	// ask the user every time we log on to the POP3 host
	CPOP3Dialog theDialog(pParent);
	theDialog.m_strUserID = m_strPOP3Logon;
	theDialog.m_strPassword = m_strPassword;
	
	if (theDialog.DoModal() == IDOK)
	{
		m_strPassword = theDialog.GetPassword();
		m_strPOP3Logon = theDialog.m_strUserID;        
		SetModifiedFlag();
	}
}                               

void CMailBoxDoc::OnMailSettings()
{   
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		                                      
	CMailSetup theDialog(pParent);
	theDialog.m_strEmailAddress = m_strAddress;
	theDialog.m_strPassword = m_strPassword;
	theDialog.m_strSMTPAddress = m_strSMTPHost;
	theDialog.m_strPOP3Address = m_strPOP3Host;
	theDialog.m_strUserID = m_strPOP3Logon;
	
	if (theDialog.DoModal() == IDOK)
	{
		m_strAddress = theDialog.m_strEmailAddress;
		m_strPassword = theDialog.m_strPassword;
		m_strSMTPHost = theDialog.m_strSMTPAddress;
		m_strPOP3Host = theDialog.m_strPOP3Address;
		m_strPOP3Logon = theDialog.m_strUserID; 
        
        CMailUserInfo dlg(pParent);
        dlg.m_strUser = m_strUserName;
        dlg.m_strOrg = m_strOrg;
	        
        if (dlg.DoModal() == IDOK)
        {
        	m_strUserName = dlg.m_strUser;
        	m_strOrg = dlg.m_strOrg;

			SetModifiedFlag();
			m_bInitialized = TRUE;
        } 
        else
        	m_bInitialized = FALSE;
	}
}

void CMailBoxDoc::ShowWizardChoiceStep()
{ 
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	CMailBoxSetupWiz2 wiz2(pParent, this);
	
	switch (wiz2.DoModal())
	{
		case IDRETRY:
			OnMailboxSetup();
		
		case IDOK:
			if (GetMailData()->m_nWalkThrough == 0)
				ShowWizardStep3();
			else
			{
				OnWizardMailSettings();
			}
			break;
		
		case IDCANCEL:
			m_bInitialized = FALSE;
			break;
	}
}

void CMailBoxDoc::OnWizardMailSettings()
{     
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	CMailSetup theDialog(pParent);
	theDialog.m_strEmailAddress = m_strAddress;
	theDialog.m_strPassword = m_strPassword;
	theDialog.m_strSMTPAddress = m_strSMTPHost;
	theDialog.m_strPOP3Address = m_strPOP3Host;
	theDialog.m_strUserID = m_strPOP3Logon;
	
	if (theDialog.DoModal() == IDOK)
	{
		m_strAddress = theDialog.m_strEmailAddress;
		m_strPassword = theDialog.m_strPassword;
		m_strSMTPHost = theDialog.m_strSMTPAddress;
		m_strPOP3Host = theDialog.m_strPOP3Address;
		m_strPOP3Logon = theDialog.m_strUserID;
		SetModifiedFlag();    
		m_bInitialized = TRUE;
		m_bInitialized = TRUE;
	}
	else
		m_bInitialized = FALSE;	
}

void CMailBoxDoc::OnMailSignature()
{ 
	CWnd* pParent = NULL;
	
	if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'S')
	{
		pParent = CWnd::GetActiveWindow();
	}
	else if (m_pApp->m_lpCmdLine[0] == '/' && m_pApp->m_lpCmdLine[1] == 'M'
		&& m_pApp->m_lpCmdLine[2] == 'A' && m_pApp->m_lpCmdLine[3] == 'I')
	{
		if (m_pApp->IsWindows95())
			pParent = CWnd::GetDesktopWindow();
		else
			pParent = CWnd::GetActiveWindow();
	}
	else
		pParent = AfxGetMainWnd();
		
	CMailSignature theDialog(pParent);
	theDialog.m_strSignature = m_strSignature;
	
	if (theDialog.DoModal() == IDOK)
	{
		m_strSignature = theDialog.m_strSignature;
		SetModifiedFlag();
		return;
	}
	return;
}
                                            
void CMailBoxDoc::DeleteAll(QSocket* pSocket, CListBox* pListBox,
	CNewMailView* pView)
{  
	CWaitCursor wait;
	if (pSocket == NULL)
		return;
	
	if (m_nDeleArray.GetSize() == 0)
		return;
			
	char szCommand[20];
	
	CPercentDialog* pDialog = new CPercentDialog;
	
	CString title = "Message Filtering Progress";
	CString text = "Please wait while we remove unwanted messages ";
	text += "from your mailbox.  Thank you for your patience!";
	pDialog->Initialize(title, text);
	
	int nIndex;
	
	for (int i = 0;i < m_nDeleArray.GetSize();i++)
	{   
		sprintf(szCommand, "DELE %i\r\n", m_nDeleArray[i]);

		// Send the DELE command to mark this message for deletion
		pSocket->Send(szCommand);
		pSocket->GetLine();                  
	
		nIndex = (m_nDeleArray.GetAt(i)) - 1;
		pListBox->DeleteString(nIndex);
		
		pView->m_nMessages = pView->m_nMessages - 1;
		pView->m_strSubjectList.RemoveAt(nIndex);
		pView->m_strSenderList.RemoveAt(nIndex);
		
		pDialog->CalculatePercent(i + 1, m_nDeleArray.GetSize());
		pDialog->UpdateWindow();
	}         
	
	
	pDialog->SetPercent(100);     
	
	pDialog->ShowWindow(SW_HIDE);
	pDialog->DestroyWindow();

	if (m_nDeleArray.GetSize() > 0)
	{
		// Remove the elements from this array
		m_nDeleArray.FreeExtra();
		m_nDeleArray.RemoveAll();
	}
	
	return;
} 

void CMailBoxDoc::MarkMessageDeleted(int nMessage)
{
	m_nDeleArray.Add(nMessage);
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxView

IMPLEMENT_DYNCREATE(CMailBoxView, CFormView)

CMailBoxView::CMailBoxView()
	: CFormView(CMailBoxView::IDD)
{
	//{{AFX_DATA_INIT(CMailBoxView)
	m_pDoc = NULL;    
	m_strMailBoxTitle = "Internet Mail";
	//}}AFX_DATA_INIT        
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pStatusBar = m_pApp->GetStatusBar(); // pointer to status bar
	m_pSocket = NULL; // pointer to Internet communications class
}

CMailBoxView::~CMailBoxView()
{   
	delete m_pSocket;
	m_pStatusBar = NULL;
	m_pApp = NULL;
	m_pDoc = NULL;
}

void CMailBoxView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailBoxView)
	DDX_Control(pDX, IDC_FINGER, m_btnFinger);
	DDX_Control(pDX, IDC_HELPINFO, m_btnHelpInfo);
	DDX_Control(pDX, IDC_MAILBOX_CHECK, m_btnReadNewMail);
	DDX_Control(pDX, IDC_MAILBOX_SETUP, m_btnSetup);
	DDX_Control(pDX, IDC_MAILBOX_COMPOSE, m_btnCompose); 
	DDX_Control(pDX, IDC_MAIL_ADDRESS_BOOK, m_btnAddrBook);
	DDX_Text(pDX, IDC_MAILBOX_TITLE, m_strMailBoxTitle);
	//}}AFX_DATA_MAP 
	if (pDX->m_bSaveAndValidate)
		UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CMailBoxView, CFormView)
	//{{AFX_MSG_MAP(CMailBoxView)
	ON_BN_CLICKED(IDC_FINGER, OnFinger)
	ON_BN_CLICKED(IDC_HELPINFO, OnHelpAndInfo)
	ON_BN_CLICKED(IDC_MAILBOX_COMPOSE, OnMailboxCompose)
	ON_BN_CLICKED(IDC_MAILBOX_SETUP, OnMailboxSetup)
	ON_BN_CLICKED(IDC_MAILBOX_CHECK, OnMailboxCheck)
	ON_UPDATE_COMMAND_UI(ID_MAIL_SEND, OnUpdateMailSend)
	ON_UPDATE_COMMAND_UI(ID_MAIL_REPLY, OnUpdateMailReply)
	ON_UPDATE_COMMAND_UI(ID_MAIL_DELETE, OnUpdateMailDelete)
	ON_UPDATE_COMMAND_UI(ID_MAIL_DISPLAY_MESSAGE, OnUpdateMailDisplayMessage)
	ON_COMMAND(ID_MAILBOX_CHECK, OnMailboxCheck)
	ON_COMMAND(ID_MAILBOX_COMPOSE, OnMailboxCompose)
	ON_BN_CLICKED(IDC_MAIL_ADDRESS_BOOK, OnMailAddressBook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMailBoxView message handlers

void CMailBoxView::OnInitialUpdate()
{     
	m_pDoc = GetDocument();
	      
	if (!GetDocument()->m_strMailBoxTitle.IsEmpty())
		m_strMailBoxTitle = GetDocument()->m_strMailBoxTitle;     
	
	GetDocument()->SetTitle(GetDocument()->m_strMailBoxTitle);
	
	// Change the frame window's title to match that of the mailbox
	((CMailBoxWnd*)GetParentFrame())->OnUpdateFrameTitle(TRUE);
	
	// Load the bitmaps for the buttons in this view
	m_btnCompose.LoadBitmaps("POFFICEU", "POFFICED", "POFFICEF", "POFFICEX");
	
	m_btnSetup.LoadBitmaps("POFFICEU", "POFFICED", "POFFICEF", "POFFICEX");
	
	m_btnReadNewMail.LoadBitmaps("POFFICEU", "POFFICED", "POFFICEF", "POFFICEX");
	
	m_btnFinger.LoadBitmaps("FINGERU", "FINGERD", "FINGERF", "FINGERX");
	
	m_btnHelpInfo.LoadBitmaps("HELPINFOU", "HELPINFOD", "HELPINFOF", "HELPINFOX");
	
	m_btnAddrBook.LoadBitmaps("MAILADDRU", "MAILADDRD", "MAILADDRF", "MAILADDRX");
	
	CFormView::OnInitialUpdate();

	m_btnCompose.SizeToContent();   
	m_btnSetup.SizeToContent();
	m_btnReadNewMail.SizeToContent();
	m_btnFinger.SizeToContent();
	m_btnHelpInfo.SizeToContent();  
	m_btnAddrBook.SizeToContent();
	
	if (m_pApp->IsUseCTL3D())       
		Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL); 
	
	GetParentFrame()->ActivateFrame(SW_SHOW);
	GetParentFrame()->UpdateWindow();
}

void CMailBoxView::OnUpdate(CView*, LPARAM, CObject*)
{
	UpdateData(FALSE);
	return;
}

void CMailBoxView::OnFinger()
{
	// make a new document strFrom the application's 
	// Finger document type
	
	m_pApp->GetFingerType()->OpenDocumentFile(NULL);
	return; 
}

void CMailBoxView::OnHelpAndInfo()
{
	CWnd::OnHelp();
}

void CMailBoxView::OnMailboxCompose()
{
	CMultiDocTemplate* pCompose = m_pApp->GetComposeMailType();
	
	CComposeWnd* pFrame = 
		(CComposeWnd*)pCompose->CreateNewFrame(GetDocument(), NULL);
	if (pFrame == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	}

	pFrame->m_bIsUseCompWizard = TRUE;
	pFrame->m_bIsUseFeedbackWizard = FALSE;

	pFrame->GetInfoBar().ShowWindow(SW_SHOW);
	pFrame->GetInfoBar().UpdateWindow();
	pFrame->RecalcLayout();

	pFrame->GetRulerBar().ShowWindow(SW_SHOW);
	pFrame->GetRulerBar().UpdateWindow();
	pFrame->RecalcLayout();
			
	pCompose->InitialUpdateFrame(pFrame, GetDocument(), TRUE);
}

void CMailBoxView::OnMailboxSetup()
{
	PostMessage(WM_COMMAND, ID_MAILBOX_SETUP, NULL);
}
	
void CMailBoxView::OnMailboxCheck()
{
	m_pDoc = GetDocument();
	m_pApp = (CInternetNavApp*)AfxGetApp();       
	
	CNewMailWnd* pFrame;
	CMultiDocTemplate* pIB = m_pApp->GetInboxType();
	
	pFrame = (CNewMailWnd*)pIB->CreateNewFrame(m_pDoc, NULL);
	
	if (pFrame == NULL) 
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	}   
	
	
	pIB->InitialUpdateFrame(pFrame, m_pDoc, TRUE);
	
	CNewMailView* pView = (CNewMailView*)pFrame->GetActiveView();
	
	// Send the view the Windows message for this command -- the view
	// has the handler code  
	pView->SendMessage(WM_COMMAND, ID_MAILBOX_CHECK, 0L);
	return;
}

void CMailBoxView::OnMailAddressBook()
{
	CAddressBook theDialog(AfxGetMainWnd());
	theDialog.m_pComposeWnd = NULL;
	theDialog.m_pDoc = m_pDoc;
	
	theDialog.DoModal();
}

void CMailBoxView::OnUpdateMailSend(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMailBoxView::OnUpdateMailReply(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMailBoxView::OnUpdateMailDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMailBoxView::OnUpdateMailDisplayMessage(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMailBoxView diagnostics
#ifdef _DEBUG

CMailBoxDoc* CMailBoxView::GetDocument() // non-debug version is inline
{
	if (!m_pDocument->IsKindOf(RUNTIME_CLASS(CMailBoxDoc)))
	{
		AfxMessageBox("Unable to access document.",
			MB_ICONSTOP|MB_OK, AFX_IDP_INTERNAL_FAILURE);
		return NULL;
	}
	
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMailBoxDoc)));
	return (CMailBoxDoc*)m_pDocument;
}

#endif //_DEBUG  
/////////////////////////////////////////////////////////////////////////////
// CNewMailView

IMPLEMENT_DYNCREATE(CNewMailView, CFormView)

BEGIN_MESSAGE_MAP(CNewMailView, CFormView)
	//{{AFX_MSG_MAP(CNewMailView)
	ON_COMMAND(ID_MAIL_DELETE, OnMailDelete)
	ON_COMMAND(ID_MAIL_DISPLAY_MESSAGE, OnMailDisplay)
	ON_COMMAND(ID_MAILBOX_CHECK, OnCheckMailbox)
	ON_UPDATE_COMMAND_UI(ID_MAIL_DELETE, OnUpdateMailDelete)
	ON_UPDATE_COMMAND_UI(ID_MAIL_DISPLAY_MESSAGE, OnUpdateMailDisplay)
	ON_UPDATE_COMMAND_UI(ID_MAILBOX_CHECK, OnUpdateCheckMailbox)
	ON_LBN_DBLCLK(IDC_MESSAGE_LIST, OnMailDisplay)
	ON_BN_CLICKED(IDC_HELP, OnHelp)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SOCKET_RESPONSE, OnSocketResponse) 
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewMailView construction/destruction

CNewMailView::CNewMailView()
	: CFormView(CNewMailView::IDD)
{
	//{{AFX_DATA_INIT(CNewMailView)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_strMessageSelection = "";
	m_strNumMessages = "";
	//}}AFX_DATA_INIT
	m_pDoc = NULL;  
	m_nMessages = 0;
	
	m_pSocket = NULL;
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	State = idle;   // we're idle
	m_pPercentDialog = NULL;
	
	m_strResponse.Empty();
	m_strSubject.Empty();
	m_strFrom.Empty();
	m_strLine.Empty();
}

CNewMailView::~CNewMailView()
{
	if (m_pSocket) delete m_pSocket;

	// If necessary, destroy the percent gauge dialog box     
	if (m_pPercentDialog && IsWindow(m_pPercentDialog->GetSafeHwnd()))
	{
		m_pPercentDialog->ShowWindow(SW_HIDE);
		m_pPercentDialog->UpdateWindow();
		
		m_pPercentDialog->DestroyWindow();
	}

	m_pDoc = NULL;         
	m_pApp = NULL;
}

void CNewMailView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewMailView)
	DDX_Control(pDX, IDC_HELP, m_btnHelp);
	DDX_Control(pDX, ID_MAIL_DELETE, m_btnDelete);
	DDX_Control(pDX, ID_MAIL_DISPLAY_MESSAGE, m_btnDisplay);
	DDX_Control(pDX, ID_MAILBOX_CHECK, m_btnCheck);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_MessageList);
	DDX_LBString(pDX, IDC_MESSAGE_LIST, m_strMessageSelection);
	DDX_Text(pDX, IDC_NUM_MESSAGES, m_strNumMessages);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
		GetDocument()->UpdateAllViews(this, 0L, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CNewMailView update handlers

void CNewMailView::OnInitialUpdate()
{ 
	// First obtain pointer to document class so we can access stored
	// information about mail settings
	m_pDoc = GetDocument();        
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	
	// Now call base-class version of this function
	CFormView::OnInitialUpdate();                           
	m_pStatusBar->ShowIdleMessage();
	                                                      
	m_btnCheck.EnableWindow(FALSE);
	m_btnDisplay.EnableWindow(FALSE);
	m_btnDelete.EnableWindow(FALSE);		                                                             

	GetDocument()->SetTitle(GetDocument()->m_strMailBoxTitle);
	((CNewMailWnd*)GetParentFrame())->OnUpdateFrameTitle(TRUE);
	
	/*m_MessageList.Attach(this); */
		   
	m_MessageList.SetTabStops(160); // dialog units
	m_MessageList.SetHorizontalExtent(LOWORD(GetDialogBaseUnits())*80);
	
	// Make this form view look good now.
	// First, subclass it with CTL3D to make it look 3D
	if (m_pApp->IsUseCTL3D())
		Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
}

void CNewMailView::OnUpdate(CView*, LPARAM, CObject*)
{
	if (!UpdateData(FALSE))
		AfxMessageBox("Form data update failed.");
}

/////////////////////////////////////////////////////////////////////////////
// CNewMailView communications

LRESULT CNewMailView::OnSocketResponse(WPARAM amount, LPARAM buffer)
{
	if ((int)amount > 0) // incoming data (via buffer)
	{                   
		// we aren't expecting any data to come here
	}
	else                          
	{
		switch ( (SocketReceiveCmd)amount )
		{
			case SocketStatusChanged:
				switch (m_pSocket->GetStatus())
				{     
					case CONNECTING:        // we don't care! 
						m_pStatusBar->SetText("Connecting to mail server...  Please wait.");
						break;
					
					case CONNECTED:
						OnSocketConnected();
						break;
						
					case DISCONNECTED:
						m_pStatusBar->ShowIdleMessage();
						break;
						
					case ERRORSTATE:
						m_pStatusBar->ShowIdleMessage();
						GetParentFrame()->PostMessage(WM_CLOSE);
						break;
						
					case TIMEDOUT:                 
						m_pStatusBar->ShowIdleMessage();
						GetParentFrame()->PostMessage(WM_CLOSE);
						break;                          
					
					default:
						m_pStatusBar->ShowIdleMessage();
						break;
				}
				break;
		}
	}
	
	m_pStatusBar->ShowIdleMessage();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CNewMailView implementation helpers

void CNewMailView::OnSocketConnected()
{ 
	CString strResponse = "";
	
	m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
	
	// switch to getline mode
	m_pSocket->SetReceiveTarget(NULL,0); 
	
	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	strResponse = m_pSocket->GetLine();	     

	// Log on to mail server
	if (!DoPOP3Logon())
	{
		// log off the server and disconnect the socket
		m_pSocket->Send("QUIT\r\n");
		strResponse = m_pSocket->GetLine();
		
		delete m_pSocket;
		m_pSocket = NULL; 
		
		State = idle;
		return;
	}
	
	switch (State)
	{
		case CheckMail:
			m_pStatusBar->SetText(IDS_ACCESSING_MAILBOX);
			DoFillListBox();
			
			State = idle;
			
			break;
			
		case Delete:
			m_pStatusBar->SetText(IDS_ACCESSING_MAILBOX);
			
			int nIndex = m_MessageList.GetCurSel();
			if (nIndex == -1)
				break;
			
			int nMessage = nIndex + 1;
			
			char command[20];
			sprintf(command, "DELE %i\r\n", nMessage);

			m_pStatusBar->SetText(IDS_DELETING_MESSAGE);
			m_pSocket->Send(command);
			strResponse = m_pSocket->GetLine();
			
			m_pSocket->Send("QUIT\r\n");                
			strResponse = m_pSocket->GetLine();
			
			delete m_pSocket;
			m_pSocket = NULL;
			
			m_nMessages = m_nMessages - 1;              
			if (m_nMessages >= 0)
				sprintf(command, "%i Messages", m_nMessages);
			else if (m_nMessages == 1)
				sprintf(command, "%i Message", m_nMessages);
			
			m_strNumMessages = command;
			UpdateData(FALSE);		
						
			State = idle;
			
			// TODO: Let the user choose, in Preferences, whether or not they
			// want the mail list refreshed after delete or not
			// Otherwise, for now, just take the string out of the listbox
			// at the selected index
			m_MessageList.DeleteString(nIndex);  
			m_strSubjectList.RemoveAt(nIndex);
			m_strSenderList.RemoveAt(nIndex);
			break;			
	}             
	
	return;
}

BOOL CNewMailView::DoPOP3Logon()
{   	
	CWaitCursor wait;
	CString strResponse = "";
	m_pStatusBar->SetText(IDS_CHECKING_PASSWORD);	

	// Logon to the user's POP3 server -- send the server their
	// User ID and Password and deny access if the server does
	int nLength = 0;
			
	m_pSocket->Send("USER " + m_pDoc->GetUserID() + "\r\n");
	strResponse = m_pSocket->GetLine();
	nLength = strResponse.GetLength();
	
	if (strResponse.Left(4) == "-ERR")
	{
		m_pStatusBar->ShowIdleMessage();
		
		if (nLength > 4)
		{
			strResponse = strResponse.Right(nLength - 5);
			
			MessageBeep(-1);
			AfxMessageBox(strResponse, MB_ICONSTOP, IDP_INVALID_USERID);
		}
			
		MessageBeep(-1);
		AfxMessageBox(IDP_INVALID_USERID, MB_ICONSTOP);
		return FALSE;
	}
	
	m_pStatusBar->SetText(IDS_CHECKING_PASSWORD);
		
	m_pSocket->Send("PASS " + m_pDoc->GetPassword() + "\r\n");
	strResponse = m_pSocket->GetLine();
	nLength = strResponse.GetLength();
	
	if (strResponse.Left(4) == "-ERR")
	{
		m_pStatusBar->ShowIdleMessage();
		
		if (nLength > 4)
		{
			strResponse = strResponse.Right(nLength - 5);

			MessageBeep(-1);
			AfxMessageBox(m_strResponse, MB_ICONSTOP,
				IDP_INVALID_POP3_PASSWORD);
		}
			
		MessageBeep(-1);
		AfxMessageBox(IDP_INVALID_POP3_PASSWORD, MB_ICONSTOP);
		return FALSE;
	}

	// We are now logged onto the user's POP3 mail server!
	return TRUE;
}
				
/////////////////////////////////////////////////////////////////////////////
// CNewMailView diagnostics

#ifdef _DEBUG
void CNewMailView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNewMailView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMailBoxDoc* CNewMailView::GetDocument() // non-debug version is inline
{
	if (!m_pDocument->IsKindOf(RUNTIME_CLASS(CMailBoxDoc)))
	{
		AfxMessageBox("Unable to access document.",
			MB_ICONSTOP|MB_OK, AFX_IDP_INTERNAL_FAILURE);
		return NULL;
	}
	
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMailBoxDoc)));
	return (CMailBoxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNewMailView message handlers

LRESULT CNewMailView::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{  
	// Update the buttons on this form
	UpdateDialogControls(this, FALSE);
	return 0;
}

void CNewMailView::OnCheckMailbox()
{   
	State = CheckMail;
	
	delete m_pSocket;
	m_pSocket = NULL;
	
	m_pSocket = new QSocket();
	m_pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);

	// Empty the list box
	if (m_MessageList.GetCount() > 0)
		m_MessageList.ResetContent();   

	m_pStatusBar->SetText("Connecting to mail server...  Please wait.");
	if (!m_pSocket->Connect(GetDocument()->m_strPOP3Host, 110))
	{
		m_pStatusBar->ShowIdleMessage(); 
		
		MessageBeep(-1);
		AfxMessageBox(IDP_NOCONNECT_POP3_HOST);

		State = idle;
		return;
	}
	
	m_pStatusBar->SetText("Connecting to mail server...  Please wait.");
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CNewMailView message management helpers

void CNewMailView::DoFillListBox()
{ 
	m_pStatusBar->SetText(IDS_ACCESSING_MAILBOX);              
	
	CString strResponse = "";
	m_strNumMessages = "";
	
	UpdateData(FALSE);
	
	m_pSocket->Send("STAT\r\n");
	strResponse = m_pSocket->GetLine();
	
	//strip away '+OK '
	strResponse = strResponse.Right(strResponse.GetLength()-4);  
	int space = strResponse.Find(' ');
	strResponse = strResponse.Left(space);
	int nMessages = atoi(strResponse); 
	
	if (nMessages > 1)
    	m_strNumMessages = strResponse + " Messages";
    else if (nMessages == 1)
    	m_strNumMessages = strResponse + " Message";
    else if (nMessages == 0)
    	m_strNumMessages = strResponse + " Messages";
    	
    UpdateData(FALSE);
    m_nMessages = nMessages;
    
    /*m_nMessages = total number of messages in mailbox*/
    /*i = the message number of the current message*/
    
	CString title = "Mail Retrieval Progress";
	CString text = "Please wait while we request your e-mail";
	text += " from the server.";
	text += "  Thank you for your patience!";
	m_pPercentDialog = new CPercentDialog(m_pApp->GetMainFrame());
	m_pPercentDialog->Initialize(title, text);
	
	m_pStatusBar->SetText("Retrieving messages...  Please wait.");
	
    BOOL bDelete = FALSE;
    BOOL bFrom = FALSE, bSubject = FALSE;

	for (int i = 1;i <= m_nMessages;i++)
	{
		m_strLine = "";
		char command[20];  //unlikely to have a 13 digit number of messages
		sprintf(command, "RETR %i \r\n",i); 
		
		m_pSocket->Send(command);
		strResponse = m_pSocket->GetLine();
		
		int length;
		if (strResponse.Left(3) == "+OK")
		{                                            
			//headers follow
			m_strSubject = "";
			m_strFrom = "";
			while ( (strResponse = m_pSocket->GetLine()) != ".\r\n"
		       && m_pSocket->IsConnected())
			{
				length = strResponse.GetLength();
				if (strResponse.Left(9) == "Subject: " && !bSubject)
				{  
					if (!m_strSubject.IsEmpty())
						continue;	// we already have subject
						
					m_strSubject = strResponse.Right(length - 9);
					length = m_strSubject.GetLength();
					if (length >= 2 
					   && m_strSubject[length-2] == '\r'
					   && m_strSubject[length-1] == '\n')
					{
						m_strSubject = m_strSubject.Left(length-2);
					}
                    
                    if (m_strSubject.IsEmpty())
                    	m_strSubject = "No Subject";

					m_strSubjectList.Add(m_strSubject);
				}
				if (strResponse.Left(6) == "From: ")
				{
					if (!m_strFrom.IsEmpty())
						continue;      // we already have from
						
					m_strFrom = strResponse.Right(length - 6);
					length = m_strFrom.GetLength();
					if (length >= 2 
					   && m_strFrom[length-2] == '\r'
					   && m_strFrom[length-1] == '\n')
					{
						m_strFrom = m_strFrom.Left(length-2);
					}
					
					m_strSenderList.Add(m_strFrom);
				}
			}
			
			if (m_strFrom == "cyberout@pleaseread.com")
			{
				// auto-delete this message -- put it in our delete array
				m_pDoc->MarkMessageDeleted(i);   
				bDelete = TRUE;
			}
			else if (m_strSubject.Find("BULK") != -1)
			{
				// auto-delete this message -- put it in our delete array
				m_pDoc->MarkMessageDeleted(i);
				bDelete = TRUE;
			}


		  /*if (m_pDoc->DoFilterMessages(m_pStatusBar, this, m_strFrom,
		  									m_strSubject, i))
		    {
		    	// messages have been filtered so set bDelete to TRUE
		    	bDelete = TRUE;
		    }*/

			DoCreateListLine();
		} //if strResponse = +OK 

		m_pPercentDialog->CalculatePercent(i, m_nMessages);
		m_pPercentDialog->UpdateWindow();
		
	} //for loop going through messages
	
	m_pPercentDialog->SetPercent(100);
	m_pPercentDialog->SetPercent(100);
	m_pPercentDialog->SetPercent(100);
	   
	m_pPercentDialog->UpdateWindow();
	m_pPercentDialog->DestroyWindow();

	// We've filtered out messages above and put their message numbers
	// in a special 'delete array' in the doc -- so now let's tell the
	// doc to step through its array and send the DELE command to the 
	// server for each one.
	if (bDelete)
	{
		m_pDoc->DeleteAll(m_pSocket, &m_MessageList, this);
		
		char szCommand[512];
		if (m_nMessages >= 0)
			sprintf(szCommand, "%i Messages", m_nMessages);
		else if (m_nMessages == 1)
			sprintf(szCommand, "%i Message", m_nMessages);
				
		m_strNumMessages = szCommand;
		UpdateData(FALSE);
				
		bDelete = FALSE;
	}
	
	m_pSocket->Send("QUIT\r\n");
	strResponse = m_pSocket->GetLine();          
	m_pStatusBar->ShowIdleMessage();
	
	delete m_pSocket;
	m_pSocket = NULL;          

	return;
}

void CNewMailView::DoCreateListLine()
{
	// This function produces the list of messages the user sees in the
	// listbox
	
	m_strLine.Empty();
	
	// Process From and Subject
	if (m_strFrom.GetLength() > 40)
	{
		m_strFrom = m_strFrom.Left(37); // so as to not overshoot tabstop
		m_strFrom += "...";		                                         
	}
	
	m_strLine = m_strFrom;
	m_strLine += "\t";
	m_strLine += m_strSubject;  // add the subject to the line
	
	// Add the finished line to the listbox with AddString()
	m_MessageList.AddString(m_strLine);
	
	m_strLine.Empty();
	return;
}

void CNewMailView::OnMailDelete()
{ 
	// Tell the user that we know about their delete request
	// and thank them for their patience!
	m_pStatusBar->SetText("Please wait...");
	delete m_pSocket;
	m_pSocket = new QSocket(TRUE);
	m_pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);
	
	State = Delete;
	m_pStatusBar->SetText("Connecting to mail server...  Please wait.");
	
	if (!m_pSocket->Connect(m_pDoc->m_strPOP3Host, 110))
	{
		m_pStatusBar->SetText("Unable to connect to mail server.");
		
		AfxMessageBox(IDP_NOCONNECT_POP3_DELETE_HOST, MB_ICONSTOP|MB_OK); 
		m_pStatusBar->ShowIdleMessage();
		delete m_pSocket;
		m_pSocket = NULL;
		return;
	}  
	
	m_pStatusBar->SetText("Connecting to mail server...  Please wait.");
	return;
}

void CNewMailView::OnMailDisplay()
{
	// Tell the user that we know about their display request
	// and thank them for their patience!
	m_pStatusBar->SetText("Locating selected message...  Please wait.");
	int nIndex = m_MessageList.GetCurSel();

	CMultiDocTemplate* pMessage = m_pApp->GetMessageType();
	CMessageWnd* pNewFrame = (CMessageWnd*)pMessage->CreateNewFrame(m_pDoc, NULL);
	
	if (pNewFrame == NULL)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	} 
	pNewFrame->SetSubject(m_strSubjectList[nIndex]);

	pMessage->InitialUpdateFrame(pNewFrame, m_pDoc);

	CMessageView* pMessageView = (CMessageView*)pNewFrame->GetActiveView();
	pMessageView->SetMessageNumber(nIndex, m_nMessages, FALSE); 
	return;  
}

void CNewMailView::OnUpdateCheckMailbox(CCmdUI* pCmdUI)
{
	// We can't let the user choose this item if we're checking
	// mail and sending & receiving stuff to and strFrom the server -- 
	// this would screw up communications and cause errors!
	
	// We only want this menu item enabled if we aren't doing any e-mail
	// stuff
	pCmdUI->Enable(State == idle);  
	m_btnCheck.EnableWindow(State == idle);
}

void CNewMailView::OnUpdateMailDelete(CCmdUI* pCmdUI)
{
	// If nothing in the list is selected, then gray this menu item  
	// Also, gray this menu item if we're communicating or receiving
	// information
	pCmdUI->Enable(((m_MessageList.GetCurSel()) >= 0) && (State == idle));
	m_btnDelete.EnableWindow(((m_MessageList.GetCurSel()) >= 0)
		&& (State == idle));
	
	return;
}

void CNewMailView::OnUpdateMailDisplay(CCmdUI* pCmdUI)
{
	// If nothing in the list is selected, then gray this menu item
	// Also, gray this menu item if we're communicating or receiving
	// information
	pCmdUI->Enable(((m_MessageList.GetCurSel()) >= 0) && (State == idle));
	m_btnDisplay.EnableWindow(((m_MessageList.GetCurSel()) >= 0) &&
		(State == idle));
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView -- displays electronic mail messages

IMPLEMENT_DYNCREATE(CMessageView, CEditView)

BEGIN_MESSAGE_MAP(CMessageView, CEditView)
	//{{AFX_MSG_MAP(CMessageView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_MIRROR_DISPLAY_FONT, OnMirrorDisplayFont)
	ON_COMMAND(ID_WORD_WRAP, OnWordWrap)
	ON_UPDATE_COMMAND_UI(ID_WORD_WRAP, OnUpdateWordWrap)
	ON_COMMAND(ID_CHOOSE_PRINT_FONT, OnChoosePrintFont)
	ON_UPDATE_COMMAND_UI(ID_CHOOSE_PRINT_FONT, OnUpdateChoosePrintFont)
	ON_COMMAND(ID_CHOOSE_FONT, OnChooseFont)
	ON_COMMAND(ID_PAGE_SETUP, OnPageSetup)
	ON_UPDATE_COMMAND_UI(ID_MIRROR_DISPLAY_FONT, OnUpdateMirrorDisplayFont)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_MESSAGE_FIRST, OnMessageFirst)
	ON_COMMAND(ID_MESSAGE_PREV, OnMessagePrev)
	ON_COMMAND(ID_MESSAGE_NEXT, OnMessageNext)
	ON_COMMAND(ID_MESSAGE_LAST, OnMessageLast)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_FIRST, OnUpdateMessageFirst)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_PREV, OnUpdateMessagePrev)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_NEXT, OnUpdateMessageNext)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_LAST, OnUpdateMessageLast)
	ON_COMMAND(ID_MAIL_REPLY, OnMailReply)  
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	//}}AFX_MSG_MAP
	// Edit change notification handler
	ON_EN_CHANGE(AFX_IDW_PANE_FIRST, CMessageView::OnEditChange)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Communications
	ON_MESSAGE(WM_SOCKET_RESPONSE, OnSocketResponse)
END_MESSAGE_MAP()

UINT CMessageView::m_nDefTabStops;
UINT CMessageView::m_nDefTabStopsOld;
BOOL CMessageView::m_bDefWordWrap;
BOOL CMessageView::m_bDefWordWrapOld;
LOGFONT NEAR CMessageView::m_lfDefFont;
LOGFONT NEAR CMessageView::m_lfDefFontOld;
LOGFONT NEAR CMessageView::m_lfDefPrintFont;
LOGFONT NEAR CMessageView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

static void GetProfileFont(LPCSTR szSec, LOGFONT* plf)
{
	CWinApp* pApp = AfxGetApp();
	plf->lfHeight = pApp->GetProfileInt(szSec, "Height", 0);
	if (plf->lfHeight != 0)
	{
		plf->lfWeight = pApp->GetProfileInt(szSec, "Weight", 0);
		plf->lfItalic = (BYTE)pApp->GetProfileInt(szSec, "Italic", 0);
		plf->lfUnderline = (BYTE)pApp->GetProfileInt(szSec, "Underline", 0);
		plf->lfPitchAndFamily = (BYTE)pApp->GetProfileInt(szSec, "PitchAndFamily", 0);
		CString strFont = pApp->GetProfileString(szSec, "FaceName", "Arial");
		strncpy((char*)plf->lfFaceName, strFont, sizeof plf->lfFaceName);
		plf->lfFaceName[sizeof plf->lfFaceName-1] = 0;
	}
}

static void WriteProfileFont(LPCSTR szSec, const LOGFONT* plf, LOGFONT* plfOld)
{
	CWinApp* pApp = AfxGetApp(); 
	if (plf->lfHeight != plfOld->lfHeight)
		pApp->WriteProfileInt(szSec, "Height", plf->lfHeight);
	if (plf->lfHeight != 0)
	{
		if (plf->lfHeight != plfOld->lfHeight)
			pApp->WriteProfileInt(szSec, "Height", plf->lfHeight);
		if (plf->lfWeight != plfOld->lfWeight)
			pApp->WriteProfileInt(szSec, "Weight", plf->lfWeight);
		if (plf->lfItalic != plfOld->lfItalic)
			pApp->WriteProfileInt(szSec, "Italic", plf->lfItalic);
		if (plf->lfUnderline != plfOld->lfUnderline)

			pApp->WriteProfileInt(szSec, "Underline", plf->lfUnderline);
		if (plf->lfPitchAndFamily != plfOld->lfPitchAndFamily)
			pApp->WriteProfileInt(szSec, "PitchAndFamily", plf->lfPitchAndFamily);
		if (strcmp(plf->lfFaceName, plfOld->lfFaceName) != 0)
			pApp->WriteProfileString(szSec, "FaceName", (LPCSTR)plf->lfFaceName);
	}
	*plfOld = *plf;
}

void CMessageView::Initialize()
{ 
	CWinApp* pApp = AfxGetApp();
	// Enable word wrap by default
	m_bDefWordWrap = pApp->GetProfileInt("E-Mail Message: Settings", "WordWrap", TRUE);
	m_bDefWordWrapOld = m_bDefWordWrap;
	m_nDefTabStops = pApp->GetProfileInt("E-Mail Message: Settings", "TabStops", 8*4);
	m_nDefTabStopsOld = m_nDefTabStops;
	GetProfileFont("E-Mail Message: Font", &m_lfDefFont);
	m_lfDefFontOld = m_lfDefFont;
	GetProfileFont("E-Mail Message: PrintFont", &m_lfDefPrintFont);
	m_lfDefPrintFontOld = m_lfDefPrintFont;       
	
}

void CMessageView::Terminate()
{
	CWinApp* pApp = AfxGetApp();     
	// Save settings if they changed
	if (m_nDefTabStops != m_nDefTabStopsOld)
		pApp->WriteProfileInt("E-Mail Message: Settings", "TabStops", m_nDefTabStops);
	if (m_bDefWordWrap != m_bDefWordWrapOld)
		pApp->WriteProfileInt("E-Mail Message: Settings", "WordWrap", m_bDefWordWrap);
	WriteProfileFont("E-Mail Message: Font", &m_lfDefFont, &m_lfDefFontOld);
	WriteProfileFont("E-Mail Message: PrintFont", &m_lfDefPrintFont, &m_lfDefPrintFontOld);
	
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView construction/destruction

CMessageView::CMessageView()
{
	// Load in settings strFrom INI file
	Initialize();
	
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_bUndo = FALSE; // start out unable to undo or redo anything
	m_nMessage = 0;
	m_pSocket = NULL;    
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
}

BOOL CMessageView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

CMessageView::~CMessageView()
{ 
	m_pApp = NULL; 
	m_pStatusBar = NULL;
	delete m_pSocket;
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView initial update message handler
// we use this to set this view's Edit control to read-only.
// Why should the user be able to modify something they haven't
// written??

void CMessageView::OnInitialUpdate()
{
	GetEditCtrl().SetReadOnly(TRUE);
	CEditView::OnInitialUpdate();
	
	m_pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView Word Wrap support

BOOL CMessageView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL CMessageView::SetWordWrap(BOOL bWordWrap)
{
	bWordWrap = !!bWordWrap;    // make sure ==TRUE || ==FALSE
	if (IsWordWrap() == bWordWrap)
		return FALSE;

	// preserve original control's state.
	CFont* pFont = GetFont();
	int nLen = GetBufferLength();
	char FAR* pSaveText = new far char[GetBufferLength()+1];
	GetWindowText(pSaveText, nLen+1);

	// create new edit control with appropriate style and size.
	DWORD dwStyle = dwStyleDefault & ~(ES_AUTOHSCROLL|WS_HSCROLL|WS_VISIBLE);
	if (!bWordWrap)
		dwStyle |= ES_AUTOHSCROLL|WS_HSCROLL;

	CWnd* pParent = GetParent();
	CRect rect;
	GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	CWnd* pFocus = GetFocus();

	UINT nID = GetDlgCtrlID();

	HWND hWnd = ::CreateWindow("edit", NULL, dwStyle,
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
		pParent->m_hWnd, (HMENU)nID,
		(HINSTANCE)m_segText, NULL);

	if (hWnd == NULL)
	{
		delete[] pSaveText;
		return FALSE;
	}

	// set the window text to nothing to make sure following set doesn't fail
	SetWindowText(NULL);

	// restore visual state
	::SetWindowText(hWnd, pSaveText);
	delete[] pSaveText;
	if (pFont != NULL)
	{
		ASSERT(pFont->m_hObject != NULL);
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)pFont->m_hObject, 0);
	}

	// detach old window, attach new
	HWND hWndOld = Detach();
	::SetWindowLong(hWndOld, GWL_WNDPROC, (LONG)*GetSuperWndProcAddr());
	::SetWindowWord(hWndOld, GWW_ID, nID+1);
	ASSERT(m_hWnd == NULL);
	SubclassWindow(hWnd);
	ASSERT(m_hWnd == hWnd);
	UINT nTabStops = m_nTabStops;
	GetParentFrame()->SendMessage(WM_RECALCPARENT);
	GetEditCtrl().SetTabStops(nTabStops);
	GetClientRect(&rect);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_DRAWFRAME);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	UpdateWindow();

	// destroy old
	::SetWindowPos(hWndOld, NULL, 0, 0, 0, 0,
		SWP_HIDEWINDOW|SWP_NOREDRAW|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|
		SWP_NOZORDER);
	::DestroyWindow(hWndOld);

	// restore rest of state...
	GetEditCtrl().LimitText(nMaxSize);
	if (pFocus == this)
		SetFocus();

	ASSERT_VALID(this);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView communications

LRESULT CMessageView::OnSocketResponse(WPARAM amount, LPARAM buffer)
{ 
	if ((int)amount > 0)
	{
		/*wait.Restore();*/
		m_pStatusBar->SetText(IDS_RECEIVING_MESSAGE);
		m_strMsg += (char*)buffer;
		/*wait.Restore();*/
		
		if (m_strMsg.Right(5) == "\r\n.\r\n")
		{     
			/*wait.Restore();*/
			// \r\n.\r\n means end of message 
			// but delete the \r\n.\r\n at the end, because it is just a 
			// signal to us
			m_strMsg = m_strMsg.Left(m_strMsg.GetLength() - 5);
			// Find .. at start of line, and drop one .
			
			if (m_strMsg.Left(2) == "..")
			{
				int nLength = m_strMsg.GetLength();
				m_strMsg = m_strMsg.Right(nLength - 1);
			}
			
			char* pszPosition;
			char* pszWindowBuffer = m_strMsg.GetBuffer(0);
			pszPosition = pszWindowBuffer;
			while (pszPosition = strstr(pszPosition, "\n.."))
			{         
				/*wait.Restore();*/
				int nLength = m_strMsg.GetLength();  
				// Skip second dot.
				strcpy(pszPosition+1, pszPosition+2);
				// Get past the \n. left behind.
				pszPosition += 2;
				/*wait.Restore();*/
			}          
			/*wait.Restore();*/
			GetEditCtrl().SetWindowText(m_strMsg);       
			
			// Extract subject header and put it in the title bar.
			UINT nLines = GetEditCtrl().GetLineCount();
			UINT nLineLength;
			char buffer[5004];
			for (UINT i = 0;i < nLines;i++)
			{
				nLineLength = GetEditCtrl().GetLine(i, buffer, 5000);
				if (strnicmp(buffer, "Subject: ", 9) == 0)
				{
					// Get the subject...
					buffer[nLineLength] = 0;
					((CMessageWnd*)GetParentFrame())->m_strSubject = buffer+9;
					((CMessageWnd*)GetParentFrame())->OnUpdateFrameTitle(TRUE);
					break;
				}
			}	
			GetParentFrame()->ActivateFrame(SW_SHOW);
			GetParentFrame()->UpdateWindow();
			/*wait.Restore();*/
			// Switch to getline mode.
			m_pSocket->SetReceiveTarget(NULL,0);
			
			m_pStatusBar->ShowIdleMessage();
			
			GetEditCtrl().SetReadOnly(TRUE);
			
			m_strSend = "QUIT\r\n";
			m_pSocket->Send(m_strSend);
			m_strSend.Empty();
			/*wait.Restore();*/
			m_strResponse = m_pSocket->GetLine();
			
			delete m_pSocket;
			m_pSocket = NULL;           
			
			/*wait.Restore();*/
			m_strMsg.Empty();
		}
		return 0;
	}
	else
	{
		// if amount < 0 it is a receive command
		CMailBoxDoc* pDoc = GetDocument();
		
		switch ((SocketReceiveCmd)amount)
		{
			case SocketStatusChanged:
				switch (m_pSocket->GetStatus())
				{   
					case CONNECTING:
						break;
						
					case CONNECTED:
						/*wait.Restore();*/
						m_strText = "Connected to mail server.";
						m_pStatusBar->SetText(m_strText);
						
						// switch to getline mode
						m_pSocket->SetReceiveTarget(NULL,0);
						m_strResponse = m_pSocket->GetLine(); 
						/*wait.Restore();*/
 
						/*wait.Restore();*/
						m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);
						/*wait.Restore();*/

						m_strSend = "USER " + pDoc->m_strPOP3Logon;
						m_strSend += "\r\n";
						/*wait.Restore();*/

						m_pSocket->Send(m_strSend);
						m_strResponse = m_pSocket->GetLine();
						/*wait.Restore();*/
						if (m_strResponse.Left(3) == "+OK")
						{
							/*wait.Restore();*/
						    m_pStatusBar->SetText(IDS_REQUESTING_ATTENTION);

							m_strSend = "PASS ";
							m_strSend += pDoc->m_strPassword;
							m_strSend += "\r\n";
							m_pSocket->Send(m_strSend);
							/*wait.Restore();*/

							m_strResponse = m_pSocket->GetLine();
							/*wait.Restore();*/ 
							
							m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
							
							if (m_strResponse.Left(3) == "+OK")
							{       
								m_strMsg.Empty();
								char szCommand[20];
								
								m_pStatusBar->SetText(IDS_REQUESTING_MAIL);
								sprintf(szCommand, "RETR %i\r\n",
									m_nMessage);
								m_pSocket->Send(szCommand);
								m_strResponse = m_pSocket->GetLine();
								// Set receive target back to this
								// Windows message    
								
								GetEditCtrl().SetReadOnly(FALSE);
								
								m_pSocket->SetReceiveTarget(this,
									WM_SOCKET_RESPONSE);
								m_strMsg.Empty();                                                               
							}                                                                                                                               
							else // Password error
							{
								/*wait.Restore();*/
								m_pStatusBar->ShowIdleMessage();
								AfxMessageBox(IDP_INVALID_POP3_PASSWORD);
								/*wait.Restore();*/
								m_pSocket->Send("QUIT\r\n");
								m_strResponse = m_pSocket->GetLine();
								delete m_pSocket;
								m_pSocket = NULL;
								State = idle;
							}
						}
						else // User ID error
						{ 
							/*wait.Restore();*/
							m_pStatusBar->ShowIdleMessage();
							AfxMessageBox(IDP_INVALID_EMAIL_ADDRESS);
							m_pSocket->Send("QUIT\r\n");
							m_strResponse = m_pSocket->GetLine();
							delete m_pSocket;
							m_pSocket = NULL;
							State = idle;
						}
						break;
					
					case DISCONNECTED:
						m_pStatusBar->ShowIdleMessage();
						break;
					
					case ERRORSTATE:
						m_pStatusBar->ShowIdleMessage();
						GetParentFrame()->DestroyWindow();
						break;
					
					case TIMEDOUT:
						m_pStatusBar->ShowIdleMessage();
						GetParentFrame()->DestroyWindow();
						break;
						
					default:
						m_pStatusBar->ShowIdleMessage();
						break;
				}
				break;
		}
	}
	
	m_pStatusBar->ShowIdleMessage();
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView drawing

void CMessageView::OnDraw(CDC* pDC)
{
	CMailBoxDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView printing

BOOL CMessageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void CMessageView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Begin print loop by calling default version -- let the
	// base class do its stuff first
	CEditView::OnBeginPrinting(pDC, pInfo);

	CString strFileName = GetDocument()->GetPathName(); 
	if (strFileName.IsEmpty()) strFileName = GetDocument()->GetTitle(); 
	BOOL bForceSysTime = strchr(strFileName, '.') == NULL;
	CTime timeSys = CTime::GetCurrentTime();
	CFileStatus status;
	CFile::GetStatus(strFileName, status);
	
	if (m_nHeaderTime != 0 || bForceSysTime)
		m_timeHeader = timeSys;
	else
		m_timeHeader = status.m_mtime;
    	
    if (m_nFooterTime != 0 || bForceSysTime)
		m_timeFooter = timeSys;
	else
		m_timeFooter = status.m_mtime;
		
	if (!pInfo->m_bPreview)
		return;

	pInfo->m_nCurPage = 0xFFFF;
	OnPrepareDC(pDC, pInfo);

	UINT nIndex = LOWORD(GetEditCtrl().GetSel());
	UINT nCurPage = 1;
	while (nCurPage < (UINT)m_aPageStart.GetSize())
	{
		if (nIndex < m_aPageStart[nCurPage])
			break;
		nCurPage++;
	}
	pInfo->m_nCurPage = nCurPage;        
	
	// Fill the CPrintInfo struct with needed information
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(m_aPageStart.GetSize()); // number of pages of message
	pInfo->m_pPD->m_pd.nMinPage = 1;
	pInfo->m_pPD->m_pd.nMaxPage = m_aPageStart.GetSize();
	pInfo->m_pPD->m_pd.nFromPage = 1;
	pInfo->m_pPD->m_pd.nToPage = m_aPageStart.GetSize();

	m_nPreviewPage = nCurPage;
}

void CMessageView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// get string to show as "filename" in header/footer
	CPageSetupSheet shtPageSetup;
	CString strFileName = GetDocument()->GetPathName();
	if (strFileName.IsEmpty())
		strFileName = GetDocument()->GetTitle();
    
    CMessageWnd* pWnd = (CMessageWnd*)GetParentFrame();
    
	// go thru CPageSetupSheet::CPageSetupPage to format the header 
	// and footer
	CString strHeader = pWnd->m_strSubject;
	CString strFooter = "";        
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.FormatFooter(strFooter, m_timeFooter, strFileName,
		pInfo->m_nCurPage);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cyChar = tm.tmHeight;
	CRect rectPage = pInfo->m_rectDraw;

	// draw and exclude space for header
	if (!strHeader.IsEmpty())
	{
		pDC->TextOut(rectPage.left, rectPage.top, strHeader);
		rectPage.top += cyChar + cyChar / 4;
		pDC->MoveTo(rectPage.left, rectPage.top);
		pDC->LineTo(rectPage.right, rectPage.top);
		rectPage.top += cyChar / 4;
	}

	// allow space for footer
	pInfo->m_rectDraw = rectPage;
	if (!strFooter.IsEmpty())
		pInfo->m_rectDraw.bottom -= cyChar + cyChar/4 + cyChar/4;

	// draw body text
	CEditView::OnPrint(pDC, pInfo);

	// draw footer
	if (!strFooter.IsEmpty())
	{
		rectPage.bottom -= cyChar;
		pDC->TextOut(rectPage.left, rectPage.bottom, strFooter);
		rectPage.bottom -= cyChar / 4;
		pDC->MoveTo(rectPage.left, rectPage.bottom);
		pDC->LineTo(rectPage.right, rectPage.bottom);
		rectPage.bottom -= cyChar / 4;
	}
}

void CMessageView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void CMessageView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView diagnostics

#ifdef _DEBUG
void CMessageView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMessageView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMailBoxDoc* CMessageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMailBoxDoc)));
	return (CMailBoxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMessageView message handlers

static void ScaleLogFont(LPLOGFONT plf, const CDC& dcFrom, const CDC& dcTo)
	// helper to scale log font member strFrom one DC to another!
{
	plf->lfHeight = MulDiv(plf->lfHeight,
		dcTo.GetDeviceCaps(LOGPIXELSY), dcFrom.GetDeviceCaps(LOGPIXELSY));
	plf->lfWidth = MulDiv(plf->lfWidth,
		dcTo.GetDeviceCaps(LOGPIXELSX), dcFrom.GetDeviceCaps(LOGPIXELSX));
}
 
int CMessageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;            
		
	if (m_lfDefFont.lfHeight != 0)
	{
		m_font.CreateFontIndirect(&m_lfDefFont);
		SetFont(&m_font);
	}
	if (m_lfDefPrintFont.lfHeight != 0)
	{
		m_fontPrint.CreateFontIndirect(&m_lfDefPrintFont);
		SetPrinterFont(&m_fontPrint);
	}
	return 0;
}

void CMessageView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing commands
	CEditView::OnRButtonDown(nFlags, point);
}

void CMessageView::OnSize(UINT nType, int cx, int cy)
{
	CWaitCursor wait;
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void CMessageView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void CMessageView::OnWordWrap()
{
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void CMessageView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void CMessageView::OnChoosePrintFont()
{
	CFont* pFont = GetPrinterFont();
	LOGFONT lf;
	LPLOGFONT plf = NULL;
	if (pFont != NULL)
	{
		pFont->GetObject(sizeof(LOGFONT), &lf);
		plf = &lf;
	}

	// magic to get printer dialog that would be used if we were printing!
	CPrintDialog dlgPrint(FALSE);
	if (!AfxGetApp()->GetPrinterDeviceDefaults(&dlgPrint.m_pd))
	{
		AfxMessageBox(IDP_ERR_GET_DEVICE_DEFAULTS);
		return;
	}
	/*wait.Restore();*/
	HDC hdcPrint = dlgPrint.CreatePrinterDC();
	if (hdcPrint == NULL)
	{
		AfxMessageBox(IDP_ERR_GET_PRINTER_DC);
		return;
	}

	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));
	CDC dcPrint;
	dcPrint.Attach(hdcPrint);

	if (plf != NULL)
	{
		// need to map initial logfont to screen metrics.
		::ScaleLogFont(plf, dcPrint, dcScreen);
	}

	// now bring up the dialog since we know the printer DC
	CFontDialog dlg(plf, CF_PRINTERFONTS, &dcPrint);
	if (dlg.DoModal() == IDOK)
	{
		// map the resulting logfont back to printer metrics.
		lf = dlg.m_lf;
		::ScaleLogFont(&lf, dcScreen, dcPrint);

		m_fontPrint.DeleteObject();
		if (m_fontPrint.CreateFontIndirect(&lf))
		{
			SetPrinterFont(&m_fontPrint);
			m_lfDefPrintFont = lf;
		}
	}
	//NOTE: destructor will call dcPrint.DeleteDC

	::ReleaseDC(NULL, dcScreen.Detach());
}

void CMessageView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void CMessageView::OnChooseFont()
{
   // get current font description
   CFont* pFont = GetFont();
   LOGFONT lf;
   if (pFont != NULL)
	   pFont->GetObject(sizeof(LOGFONT), &lf);
   else
	   ::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

	CFontDialog dlg(&lf, CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
	if (dlg.DoModal() == IDOK)
	{
		// switch to new font.
		m_font.DeleteObject();
		if (m_font.CreateFontIndirect(&lf))
		{
			SetFont(&m_font);
			m_lfDefFont = lf;
		}
	}
}

void CMessageView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());      
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.m_bOutputView = TRUE;
	shtPageSetup.m_pgTabStops.m_nTabStops = m_nTabStops/4;
	if (shtPageSetup.DoModal() == IDOK)
	{
		// get values entered into this tabbed dialog box
		shtPageSetup.m_pgHeaderFooter.Terminate();
		
		m_nHeaderTime = shtPageSetup.m_pgHeaderFooter.m_nHeaderTime;
		m_nFooterTime = shtPageSetup.m_pgHeaderFooter.m_nFooterTime;
		SetTabStops(shtPageSetup.m_pgTabStops.m_nTabStops*4);
		m_nDefTabStops = m_nTabStops;
		
		return;
	}  
	return;
}

void CMessageView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void CMessageView::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strTextSect = "E-Mail Message";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strTextSect, "E-MailMessageLeft", rect.left);
	pApp->WriteProfileInt(strTextSect, "E-MailMessageRight", rect.right);
	pApp->WriteProfileInt(strTextSect, "E-MailMessageTop", rect.top);
	pApp->WriteProfileInt(strTextSect, "E-MailMessageBottom", rect.bottom);
	pApp->WriteProfileInt(strTextSect, "E-MailMessageCX", rect.Width());
	pApp->WriteProfileInt(strTextSect, "E-MailMessageCY", rect.Height());
}

void CMessageView::OnEditUndo()
{
	CEditView::OnEditUndo();
	
	// change the "redo flag" to TRUE so the undone action can be redone
	m_bUndo = TRUE;
	return;
}

void CMessageView::OnEditRedo()
{
	if (m_bUndo)
	{
		CEditView::OnEditUndo();
		m_bUndo = FALSE;    // can't redo anymore -- not until another undo
	}
	return; // we're done
}       

void CMessageView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bUndo && GetEditCtrl().CanUndo());
	return;
} 

void CMessageView::SetMessageNumber(UINT nMessage, 
	UINT nNumMessages /*=CMessageView::m_nMessages*/, BOOL bMove /*=FALSE*/)
{   
	m_nMessages = nNumMessages;
	CMailBoxDoc* pDoc = GetDocument();
	
	if (!bMove)
	{
		// Index in the list box is zero based; 
		// mailbox message index is 1-based
		m_nMessage = nMessage + 1;                                              
	}
	else
	{
		// Just set message number to nMessage without adding or subtracting
		// if the user is scrolling through the mailbox.
		m_nMessage = nMessage;
	}
		
	delete m_pSocket;
	m_pSocket = new QSocket();
	m_pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);
	
	// now, connect to the server to download the mail message
	m_strText = "Contacting mail server...  Thank you for your patience!";
	m_pStatusBar->SetText(m_strText);   
	
	m_strMsg.Empty();
	
	if (!m_pSocket->Connect(pDoc->m_strPOP3Host, 110))
	{
		m_pStatusBar->ShowIdleMessage();
		MessageBeep(-1);
		AfxMessageBox(IDP_NOCONNECT_POP3_HOST);
		delete m_pSocket;
		m_pSocket = NULL;
		return;
	}
	
	// NOTE: To find out what is going to go on when m_pSocket->Connect()
	// comes through with the connection, please see the message handler
	// for the WM_SOCKET_RESPONSE message, that is, 
	// CMessageView::OnSocketResponse().
	
	return;
}	

void CMessageView::OnEditChange()
{
	// The mail message, when downloaded and put into this view, will cause
	// the document's modified flag to be set by CEditView, our base class.
	
	// We only modify the document when the user uses the Mailbox Setup
	// Wizard to change document settings, NOT when the contents of this view
	// is modified.
	
	// We will, therefore, set the document's modified flag to FALSE here
	// so the document stays unmodified.
	
	GetDocument()->SetModifiedFlag(FALSE);
	return;
}

void CMessageView::OnMessageFirst()
{
	CString strText = "Retrieving first mail message...  Please wait.";
	m_pStatusBar->SetText(strText);
	m_nMessage = 1;
	
	// First erase all of the text of the current message
	// First select all text
	CEditView::OnEditSelectAll();
	
	// Next, clear the text
	CEditView::OnEditClear();    
	GetEditCtrl().EmptyUndoBuffer();
	
	// Now get the first message
	m_strMsg.Empty();
	
	// CMessageView::SetMessageNumber() takes care of getting the message
	// which is identified by the message number we pass + 1 and setting the
	// current message number to the number we pass it + 1 also.

	SetMessageNumber(m_nMessage, m_nMessages, TRUE);
}

void CMessageView::OnMessagePrev()
{
	CString strText = "Retrieving previous mail message...  Please wait.";
	m_pStatusBar->SetText(strText);
	m_nMessage = m_nMessage - 1;
	
	// First erase all of the text of the current message
	// First select all text
	CEditView::OnEditSelectAll();
	
	// Next, clear the text
	CEditView::OnEditClear();    
	GetEditCtrl().EmptyUndoBuffer();  // prevent user from undoing the clear
	
	// Now get the previous message
	
	// CMessageView::SetMessageNumber() takes care of getting the message
	// which is identified by the message number we pass + 1 and setting the
	// current message number to the number we pass it + 1 also.

	SetMessageNumber(m_nMessage, m_nMessages, TRUE);	
}

void CMessageView::OnMessageNext()
{
	CString strText = "Retrieving next mail message...  Please wait.";
	m_pStatusBar->SetText(strText);
	m_nMessage = m_nMessage + 1;
	
	// First erase all of the text of the current message
	// First select all text
	CEditView::OnEditSelectAll();
	
	// Next, clear the text
	CEditView::OnEditClear();    
	GetEditCtrl().EmptyUndoBuffer();
	
	// Now get the next message
	
	// CMessageView::SetMessageNumber() takes care of getting the message
	// which is identified by the message number we pass + 1 and setting the
	// current message number to the number we pass it + 1 also.

	SetMessageNumber(m_nMessage, m_nMessages, TRUE);	
}

void CMessageView::OnMessageLast()
{
	m_nMessage = m_nMessages;
	CString strText = "Retrieving last mail message...  Please wait.";
	m_pStatusBar->SetText(strText);
	
	
	// First erase all of the text of the current message
	// First select all text
	CEditView::OnEditSelectAll();
	
	// Next, clear the text
	CEditView::OnEditClear();    
	GetEditCtrl().EmptyUndoBuffer();
	
	// Now get the last message
	
	// CMessageView::SetMessageNumber() takes care of getting the message
	// which is identified by the message number we pass + 1 and setting the
	// current message number to the number we pass it + 1 also.

	SetMessageNumber(m_nMessage, m_nMessages, TRUE);	
}

void CMessageView::OnUpdateMessageFirst(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nMessage != 1 && m_nMessages != 1);
}

void CMessageView::OnUpdateMessagePrev(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nMessage != 1 && m_nMessages != 1);
}

void CMessageView::OnUpdateMessageNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nMessage != m_nMessages && m_nMessages != 1);
}

void CMessageView::OnUpdateMessageLast(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nMessage != m_nMessages && m_nMessages != 1);	
}

void CMessageView::OnMailReply()
{    
	// TODO: Add an ability to quote the selected message text in the reply	
	TRACE0("INF: Replying to e-mail message.\r\n");

	CMultiDocTemplate* pCompose = m_pApp->GetComposeMailType();
	m_pDoc = GetDocument();
	
	if (pCompose == NULL)
		return;
		
	CComposeWnd* pFrame = 
		(CComposeWnd*)pCompose->CreateNewFrame(m_pDoc, NULL);
	if (pFrame == NULL)
	{ 
		MessageBeep(-1);
		AfxMessageBox(IDP_FAILED_OPEN_WINDOW, MB_ICONSTOP|MB_OK);
		return;         
	}
	
	pFrame->m_bIsUseCompWizard = FALSE;
	pFrame->m_bIsUseFeedbackWizard = FALSE;
	
	pCompose->InitialUpdateFrame(pFrame, m_pDoc, TRUE);
		
	pFrame->GetInfoBar().ShowWindow(SW_SHOW);
	pFrame->GetInfoBar().UpdateWindow();
	pFrame->RecalcLayout();

	pFrame->GetRulerBar().ShowWindow(SW_SHOW);
	pFrame->GetRulerBar().UpdateWindow();
	pFrame->RecalcLayout();
	
	CComposeView* pView = (CComposeView*)pFrame->GetActiveView();
	pView->m_pFrame = pFrame;                                 

	UINT nLineLenngth;
	m_pStatusBar->SetText("Searching for sender of message...  Please wait.");
	CString strSubject = "", strSender = "";
	char buffer[5004];
	// For \r, \n, zero term, and perhaps an extra .
	UINT lines = GetEditCtrl().GetLineCount();
	UINT linelength;

	for (UINT i = 0;i < lines;i++)
	{
		linelength = GetEditCtrl().GetLine(i, buffer, 5000);
		if (strnicmp(buffer, "Reply-To: ", 10) == 0)
		{
			buffer[linelength] = 0;
			strSender = buffer + 10;
			
			CString strSender2 = strSender; // in case we're wrong below
			// Chop off any parantheses, spaces, punctuation, names, etc.
			int nSpace = 0;     
			int nLength = strSender.GetLength();
			if (strSender.Right(1) == ">")
				nSpace = strSender.ReverseFind(' ');
			else
				nSpace = strSender.Find(' ');
					
				
			// Take string up to space
			if (nSpace == -1) 
			{
				break; // we have strSender -- bing! we're done.
			}
				
			// Take the strSender up to the space
			if (strSender.Right(1) == ">")
			{
				strSender = strSender.Right(nLength - nSpace - 1);
				strSender = strSender.Right(nLength - 1);  
				strSender = strSender.Left(nLength - 1);
					
				// bing! we now have strSender!
				break;
			}
						
			strSender = strSender.Left(nSpace);  
				
			if (strSender.Find('@') == -1)
			{
				// oops -- we guessed wrong -- other part of e-mail address
				// has the actual address
				strSender = strSender2; // get back original strSender
				strSender = strSender.Right(strSender.GetLength() 
						- nSpace - 1);   
			}  
			else
				break;
					
			if ((nSpace = strSender.Find('<')) != -1)
			{
				// e-mail address is enclosed in angle brackets -- remove
				// angle brackets
				strSender = strSender.Right(nLength - nSpace - 1);
					
				// initialize nLength with new length of strSender
				nLength = strSender.GetLength();
					
				strSender = strSender.Left(nLength - 1);
				break;
			} 
			else if ((nSpace = strSender.Find('\"')) != -1)
			{
				// e-mail address is enclosed in quotation marks -- remove
				strSender = strSender.Right(nLength - nSpace - 1);
					
				// initialize nLength with new length of strSender
				nLength = strSender.GetLength();
					
				strSender = strSender.Left(nLength - 1);
				break;
			} 
			else if ((nSpace = strSender.Find('(')) != -1)
			{
				// e-mail address is enclosed in parentheses -- remove
				strSender = strSender.Right(nLength - nSpace - 1);
					
				// initialize nLength with new length of strSender
				nLength = strSender.GetLength();
					
				strSender = strSender.Left(nLength - 1);
				break;
			} 
			else if ((nSpace = strSender.Find('[')) != -1)
			{
				// e-mail address is enclosed in square brackets -- remove
				strSender = strSender.Right(nLength - nSpace - 1);
					
				// initialize nLength with new length of strSender
				nLength = strSender.GetLength();
					
				strSender = strSender.Left(nLength - 1);
	
				break;
			} 			
		}
	}

	if (strSender.IsEmpty())
	{
		for (UINT d = 0;d < lines;d++)
		{
			linelength = GetEditCtrl().GetLine(d, buffer, 5000);
			if (strnicmp(buffer, "From: ", 6) == 0)
			{
				buffer[linelength] = 0;
				strSender = buffer + 6;
				
				CString strSender2 = strSender; // in case we're wrong below
				// Chop off any parantheses, spaces, punctuation, names, etc.
				int nSpace = 0;     
				int nLength = strSender.GetLength();
				if (strSender.Right(1) == ">")
					nSpace = strSender.ReverseFind(' ');
				else
					nSpace = strSender.Find(' ');
					
				
				// Take string up to space
				if (nSpace == -1) 
				{
					break; // we have strSender -- bing! we're done.
				}
				
				// Take the strSender up to the space
				if (strSender.Right(1) == ">")
				{
					strSender = strSender.Right(nLength - nSpace - 1);
					strSender = strSender.Right(nLength - 1);  
					strSender = strSender.Left(nLength - 1);
					
					// bing! we now have strSender!
					break;
				}
						
				strSender = strSender.Left(nSpace);  
				
				if (strSender.Find('@') == -1)
				{
					// oops -- we guessed wrong -- other part of e-mail address
					// has the actual address
					strSender = strSender2; // get back original strSender
					strSender = strSender.Right(strSender.GetLength() 
							- nSpace - 1);   
				}  
				else
					break;
					
				if ((nSpace = strSender.Find('<')) != -1)
				{
					// e-mail address is enclosed in angle brackets -- remove
					// angle brackets
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('\"')) != -1)
				{
					// e-mail address is enclosed in quotation marks -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('(')) != -1)
				{
					// e-mail address is enclosed in parentheses -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
					break;
				} 
				else if ((nSpace = strSender.Find('[')) != -1)
				{
					// e-mail address is enclosed in square brackets -- remove
					strSender = strSender.Right(nLength - nSpace - 1);
					
					// initialize nLength with new length of strSender
					nLength = strSender.GetLength();
					
					strSender = strSender.Left(nLength - 1);
	
					break;
				} 			
			}
		}
	}

	m_pStatusBar->SetText("Searching for subject of message...  Please wait.");
	
	for (UINT c = 0;c < lines;c++)
	{
		linelength = GetEditCtrl().GetLine(c, buffer, 5000);
		if (strnicmp(buffer, "Subject: ", 9) == 0)
		{
			buffer[linelength] = 0;
			strSubject = buffer + 9;
			
			if (strSubject.Left(4).CompareNoCase("Re: ") == 0)
			{
				int nSpace = strSubject.Find(' ');
				strSubject = strSubject.Right(strSubject.GetLength() -
									nSpace - 1);
				strSubject = "Re: " + strSubject;
				break;         
			}
			else if ((strSubject.Left(2).CompareNoCase("[R")) == 0)
			{
				int nSpace = strSubject.Find(' ');
				strSubject = strSubject.Right(strSubject.GetLength() -
									nSpace - 1);
				strSubject = "Re: " + strSubject;
				break;         
			}							
		}
	}

	TRACE0("INF: Transferring information to Compose Mail window.\r\n");
	pView->m_pFrame->m_wndInfoBar.m_strTo = strSender;
	pView->m_pFrame->m_wndInfoBar.m_strSubject = strSubject;		
	pView->m_pFrame->m_wndInfoBar.UpdateData(FALSE);        
	pView->m_pFrame->SetWindowText(strSubject);
	
	// we're done!
	return;
}

void CMessageView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}    

void CMessageView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMessageView::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMessageView::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMessageView::OnUpdateEditReplace(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView

IMPLEMENT_DYNCREATE(CComposeView, CEditView)

BEGIN_MESSAGE_MAP(CComposeView, CEditView)
	//{{AFX_MSG_MAP(CComposeView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_MIRROR_DISPLAY_FONT, OnMirrorDisplayFont)
	ON_COMMAND(ID_WORD_WRAP, OnWordWrap)
	ON_UPDATE_COMMAND_UI(ID_WORD_WRAP, OnUpdateWordWrap)
	ON_COMMAND(ID_CHOOSE_PRINT_FONT, OnChoosePrintFont)
	ON_UPDATE_COMMAND_UI(ID_CHOOSE_PRINT_FONT, OnUpdateChoosePrintFont)
	ON_COMMAND(ID_CHOOSE_FONT, OnChooseFont)
	ON_COMMAND(ID_PAGE_SETUP, OnPageSetup)
	ON_UPDATE_COMMAND_UI(ID_MIRROR_DISPLAY_FONT, OnUpdateMirrorDisplayFont)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_MAIL_SEND, OnMailSend)
	//}}AFX_MSG_MAP   
	// Document modification supressor
	ON_EN_CHANGE(AFX_IDW_PANE_FIRST, CComposeView::OnEditChange)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview) 
	// Communications over the Internet
	ON_MESSAGE(WM_SOCKET_RESPONSE, OnSocketResponse)  
	// Other stuff
	ON_MESSAGE(WM_COMPOSE_WIZARD, OnComposeWizard)
	ON_MESSAGE(WM_FEEDBACK_WIZARD, OnFeedbackWizard)
END_MESSAGE_MAP()

UINT CComposeView::m_nDefTabStops;
UINT CComposeView::m_nDefTabStopsOld;
BOOL CComposeView::m_bDefWordWrap;
BOOL CComposeView::m_bDefWordWrapOld;
LOGFONT NEAR CComposeView::m_lfDefFont;
LOGFONT NEAR CComposeView::m_lfDefFontOld;
LOGFONT NEAR CComposeView::m_lfDefPrintFont;
LOGFONT NEAR CComposeView::m_lfDefPrintFontOld;

/////////////////////////////////////////////////////////////////////////////
// Static initialization/termination

void CComposeView::Initialize()
{ 
	CWinApp* pApp = AfxGetApp();
	// Enable word wrap by default
	m_bDefWordWrap = pApp->GetProfileInt("Compose Mail: Settings", "WordWrap", TRUE);
	m_bDefWordWrapOld = m_bDefWordWrap;
	m_nDefTabStops = pApp->GetProfileInt("Compose Mail: Settings", "TabStops", 8*4);
	m_nDefTabStopsOld = m_nDefTabStops;
	GetProfileFont("Compose Mail: Font", &m_lfDefFont);
	m_lfDefFontOld = m_lfDefFont;
	GetProfileFont("Compose Mail: PrintFont", &m_lfDefPrintFont);
	m_lfDefPrintFontOld = m_lfDefPrintFont;       
	
}

void CComposeView::Terminate()
{
	CWinApp* pApp = AfxGetApp();     
	// Save settings if they changed
	if (m_nDefTabStops != m_nDefTabStopsOld)
		pApp->WriteProfileInt("Compose Mail: Settings", "TabStops", m_nDefTabStops);
	if (m_bDefWordWrap != m_bDefWordWrapOld)
		pApp->WriteProfileInt("Compose Mail: Settings", "WordWrap", m_bDefWordWrap);
	WriteProfileFont("Compose Mail: Font", &m_lfDefFont, &m_lfDefFontOld);
	WriteProfileFont("Compose Mail: PrintFont", &m_lfDefPrintFont, &m_lfDefPrintFontOld);
	
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView construction/destruction

CComposeView::CComposeView()
{
	// Load in settings from INI file
	Initialize();
	
	m_nTabStops = m_nDefTabStops;
	m_uTimerID = 0;
	m_pApp = (CInternetNavApp*)AfxGetApp();   
	m_bUndo = FALSE; // start out unable to undo or redo anything 
	m_pSocket = NULL;  
	m_pPercentDialog = NULL;   
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_bShowWindow = TRUE;     
	
	m_nEase = m_nNav = m_nQuality = 3;
}

BOOL CComposeView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;

	if (m_bDefWordWrap)
		cs.style &= ~(WS_HSCROLL|ES_AUTOHSCROLL);

	return TRUE;
}

CComposeView::~CComposeView()
{ 
	m_pApp = NULL;
	delete m_pSocket;

	// If necessary, delete the percent gauge dialog box     
	if (m_pPercentDialog && IsWindow(m_pPercentDialog->GetSafeHwnd()))
	{
		m_pPercentDialog->ShowWindow(SW_HIDE);
		m_pPercentDialog->UpdateWindow();
		
		m_pPercentDialog->DestroyWindow();
	}
	
	// Write settings out to INI file
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView information updating

void CComposeView::OnInitialUpdate()
{
	m_pDoc = GetDocument();    
	m_pFrame = (CComposeWnd*)GetParentFrame();

	m_pFrame->OnUpdateFrameTitle(TRUE);
    m_pFrame->UpdateBarInfo();
     	
	CEditView::OnInitialUpdate();
	    
    if (m_pFrame->IsUseCompWizard() && !m_pFrame->IsUseFeedbackWizard())
    {
    	GetParentFrame()->ActivateFrame(SW_HIDE);
    	PostMessage(WM_COMPOSE_WIZARD);          
    }  
    else if (!m_pFrame->IsUseCompWizard() && m_pFrame->IsUseFeedbackWizard())
    {
      	GetParentFrame()->ActivateFrame(SW_HIDE);
     	PostMessage(WM_FEEDBACK_WIZARD);         
    }
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView communications

LRESULT CComposeView::OnSocketResponse(WPARAM amount, LPARAM buffer)
{  
	m_pDoc = GetDocument();
	
	if ((int)amount > 0)
	{    
		// We don't care if there's any data for us
	}
	else                          
	{
		// if amount < 0 it is a receive command
		switch ( (SocketReceiveCmd)amount )
		{
			case SocketStatusChanged:
				switch (m_pSocket->GetStatus())
				{
					case CONNECTING:
						m_pStatusBar->SetText("Connecting to mail server...");
						break;
						
					case CONNECTED:
						// The CComposeView::SendMailMessage function
						// handles the grisly details of sending the user's
						// mail message.
						SendMailMessage();
						break;
						
					case DISCONNECTED:   
						m_pStatusBar->ShowIdleMessage();
						break;
						
					case ERRORSTATE:                             
						m_pStatusBar->ShowIdleMessage();
						GetParentFrame()->SendMessage(WM_CLOSE);
						break;
					
					case TIMEDOUT:
						m_pStatusBar->ShowIdleMessage();
						GetParentFrame()->SendMessage(WM_CLOSE);
						break;
						
					default:
						m_pStatusBar->ShowIdleMessage(); 
						if (!GetParentFrame()->IsWindowVisible())
							GetParentFrame()->SendMessage(WM_CLOSE);
						break;
				}
				break;
		}
	}
	return 0; 
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView SMTP communications

void CComposeView::SendMailMessage()
{   
	// Variables we need
	CString response = "", strSend = "", strMessageIn = "";
	CString strMessageOut = "";
	CString strCRLF = "\r\n";    
	
	TRACE0("Allocating parsing buffer...\r\n");
	char buffer[1024];
	UINT i, lines, linelength;
	int nAt, nLength;
	
	m_pStatusBar->SetText("Connected to mail server.  "
		"Requesting server attention...");
	//switch to getline
	m_pSocket->SetReceiveTarget(NULL,0);
	response = m_pSocket->GetLine();
	strSend = m_pDoc->m_strDomainName;
						
	// send the HELO command
	// parameter: part of e-mail address after the '@'
	m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
	m_pSocket->Send("HELO " + strSend + "\r\n");
	m_pStatusBar->SetText(IDS_WAITING_RESPONSE);						
	response = m_pSocket->GetLine(); 
	
	// diagnostics
	#ifdef _DEBUG
	afxDump << response;
	#endif
						
	if ((atoi(response.Left(1))) == 2)
	{ 
		response.Empty();  
	
		// tell the server from whom this message is being sent
		// with the 'MAIL FROM <user@host>' command  
		m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
		strSend = "MAIL FROM: <";
		strSend += m_pDoc->m_strAddress;
		strSend += ">\r\n";
		
		m_pSocket->Send(strSend);
		m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
		response = m_pSocket->GetLine(); 
		m_pStatusBar->ShowIdleMessage();
							
		// diagnostics
		#ifdef _DEBUG
			afxDump << response;
		#endif
							
		if ((atoi(response.Left(3))) == 250)
		{
			//determine recipient(s) -- to, cc, and bcc.   
        	m_pFrame->UpdateBarInfo();
            m_pFrame->ShowWindow(SW_HIDE);      
            m_pFrame->UpdateWindow();

            // now, form To and CC headers in the mail message
            // no one is to see who got the BCC message, so we omit
            // that header)  
                                
            m_pFrame->UpdateBarInfo();
            strCRLF = "\r\n";  
            // Form mail message headers
			m_pFrame->UpdateBarInfo();                                
            GetEditCtrl().GetWindowText(strMessageIn);
			
			// Prepare the mail headers			
			PrepareMailHeaders();
				
			/*if (!ValidateMailHeaders())
			{
				MessageBeep(-1);
				AfxMessageBox("Some information you filled out in "
					"the Compose Mail window was invalid.  Correct"
				" the error, and try again!");   
				
				m_pStatusBar->ShowIdleMessage();	
				m_pSocket->Send("QUIT\r\n");
					
				delete m_pSocket;
				m_pSocket = NULL;
					
				m_pFrame->ActivateFrame(SW_RESTORE);
	
				return;
			}*/
				
			m_pStatusBar->SetText("Processing recipients...  Please wait.");
			if (!ProcessRecipients())
			{ 
				m_pStatusBar->ShowIdleMessage();
				m_pSocket->Send("QUIT\r\n");
									
				delete m_pSocket;
				m_pSocket = NULL;
					
				m_pFrame->ActivateFrame(SW_RESTORE);
				m_pStatusBar->ShowIdleMessage();
					
				return;
			}

			// diagnostics
			#ifdef _DEBUG
				afxDump << response;
			#endif
				
			m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
			m_pSocket->Send("DATA\r\n");  

			m_pStatusBar->SetText(IDS_DELIVERING_MAIL);
			response = m_pSocket->GetLine();
			
			// diagnostics
			#ifdef _DEBUG
				afxDump << response;
			#endif
     		
        	m_pStatusBar->SetText(IDS_DELIVERING_MAIL);
            
            // Send the message headers  
            PrepareMailHeaders();
   	        SendMailHeaders();
            
       	    lines = GetEditCtrl().GetLineCount();

			#ifdef _DEBUG
           		afxDump << "Mail Message Follows:\r\n\r\n";
           	#endif
            
            if (m_pFrame->m_bIsSuggestionBox 
            	&& m_pFrame->IsUseFeedbackWizard())
            {
            	m_pSocket->Send(m_strFeedback);
            }
            
            if (m_bShowWindow)
            {              
            	for (i = 0; i < lines; i++)
				{
					linelength = GetEditCtrl().GetLine(i,buffer,5000);
					buffer[linelength] = 0;
					if (buffer[0] == '.')
					{
						m_pSocket->Send('.' + (CString)buffer + "\r\n");
					}
					else
					{
						m_pSocket->Send((CString)buffer + "\r\n");
					}
				}   
			}

	        // Add the user's signature  
			// one line below the last line of the
			// user's message
			if (!m_pDoc->m_strSignature.IsEmpty())
			{
				m_pSocket->Send("\r\n");
				m_pSocket->Send(m_pDoc->m_strSignature);
				m_pSocket->Send("\r\n");
			}
								
 			m_pSocket->Send("\r\n.\r\n");
 			m_pStatusBar->SetText(IDS_WAITING_RESPONSE);
			response = m_pSocket->GetLine();
                                
	        // close the view and logoff of mail server
    	    m_pStatusBar->SetText(IDS_TALKING_TO_SERVER);
        	m_pSocket->Send("QUIT\r\n");

			delete m_pSocket;
			m_pSocket = NULL;
			
			if (m_pFrame->m_bIsInetnavTechSupport &&
				!m_pFrame->m_bIsISPTechSupport &&
					m_pApp->IsMailSendConfirm())
			{
				m_pStatusBar->SetText("Your request has been sent!");
				MessageBeep(-1);
				AfxMessageBox("Your request has been sent!",
					MB_ICONASTERISK|MB_OK);	
				m_pStatusBar->ShowIdleMessage();
			}
			else if (!m_pFrame->m_bIsInetnavTechSupport &&
						!m_pFrame->m_bIsISPTechSupport &&
					m_pApp->IsMailSendConfirm())
			{					
				m_pStatusBar->SetText("Your mail message has been sent!");
				MessageBeep(-1);
				AfxMessageBox("Your mail message has been sent!",
					 MB_ICONASTERISK|MB_OK);   
				m_pStatusBar->ShowIdleMessage();
			}
			else if (!m_pFrame->m_bIsInetnavTechSupport &&
						m_pFrame->m_bIsISPTechSupport &&
					m_pApp->IsMailSendConfirm()) 
			{
				m_pStatusBar->SetText("Your request has been sent!");
				MessageBeep(-1);
				AfxMessageBox("Your request has been sent!",
					MB_ICONASTERISK|MB_OK);	
				m_pStatusBar->ShowIdleMessage();			
			}
			else if (!m_pFrame->m_bIsInetnavTechSupport &&
						!m_pFrame->m_bIsISPTechSupport &&
					m_pFrame->m_bIsSuggestionBox && !m_pFrame->IsUseFeedbackWizard()
				&& m_pApp->IsMailSendConfirm())		
			{
				m_pStatusBar->SetText("Your suggestion has been sent!");
				MessageBeep(-1);
				AfxMessageBox("Your suggestion has been sent!",
					MB_ICONASTERISK|MB_OK);	
				m_pStatusBar->ShowIdleMessage();
			}				
			else if (!m_pFrame->m_bIsInetnavTechSupport &&
						!m_pFrame->m_bIsISPTechSupport &&
					m_pFrame->m_bIsSuggestionBox && m_pFrame->IsUseFeedbackWizard()
				&& m_pApp->IsMailSendConfirm())		
			{
				m_pStatusBar->SetText("Your feedback has been sent!");
				MessageBeep(-1);
				AfxMessageBox("Your feedback has been sent!",
					MB_ICONASTERISK|MB_OK);	
				m_pStatusBar->ShowIdleMessage();
			}				
			
			// If the user was offline before sending,
			// disconnect them again
			if (m_pApp->IsFlashConnect())
			{
				// If this is a FlashConnect, end it now
				m_pApp->SetFlashConnect(FALSE);
				m_bDisconnectAfterSend = FALSE;
				m_pStatusBar->ShowIdleMessage();
	
				// Since this window was only hidden during
				// sending, destroy it now to get
				// rid of the open document     
				if (m_pApp->IsMailSendClose())
					GetParentFrame()->SendMessage(WM_CLOSE);
				return;
			}
			
			if (m_pApp->IsMailSendClose())					
				GetParentFrame()->SendMessage(WM_CLOSE);
			return;  								
		}
		else
		{   
			MessageBeep(-1);
			AfxMessageBox(IDP_INVALID_EMAIL_ADDRESS, MB_ICONSTOP);
			m_pStatusBar->SetText("Disconnecting from mail sever...  Please wait.");
			m_pSocket->Send("QUIT\r\n");  
			m_pStatusBar->SetText("Disconnected from mail server.");
    	
			// free memory which now-no-longer-needed socket used
			delete m_pSocket;
			m_pSocket = NULL;       
			
			if (m_pApp->IsFlashConnect())
			{
				m_pApp->AbortFlashConnect();
				m_bDisconnectAfterSend = FALSE;
				m_pStatusBar->ShowIdleMessage();
				
				// Since this window is only hidden during 
				// sending, destroy it to free the window's
				// memory
				GetParentFrame()->SendMessage(WM_CLOSE);
				return;
			}
			
			GetParentFrame()->SendMessage(WM_CLOSE);
			return;
		}
		
	}
	else
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_INVALID_FROM_ADDRESS, MB_ICONSTOP);
		
		m_pStatusBar->SetText("Disconnecting from mail sever...  Please wait.");
		m_pSocket->Send("QUIT\r\n");  
		m_pStatusBar->SetText("Disconnected from mail sever.");
							
		// free memory which now-no-longer-needed socket used
		delete m_pSocket;
		m_pSocket = NULL;          
		
			
		if (m_pApp->IsFlashConnect())
		{
			m_pApp->AbortFlashConnect();
			m_bDisconnectAfterSend = FALSE;
			m_pStatusBar->ShowIdleMessage();
				
			// Since this window is only hidden during 
			// sending, destroy it to free the window's
			// memory
			GetParentFrame()->SendMessage(WM_CLOSE);
			return;
		}
	
		GetParentFrame()->SendMessage(WM_CLOSE);
		return;
	} 
}

void CComposeView::PrepareMailHeaders()
{
	// Get the mail headers from the information dialog bar which the
	// frame window has
	m_pFrame->GetInfoBar().UpdateInfo();
	m_pFrame->GetInfoBar().UpdateInfo();
	m_pFrame->GetInfoBar().UpdateInfo();
	
	m_strTo = m_pFrame->GetInfoBar().GetTO();
	
	m_strCC.Empty();
	m_strBCC.Empty();
	
	if (m_pFrame->GetInfoBar().GetCC().IsEmpty() &&
			m_pFrame->GetInfoBar().GetBCC().IsEmpty())
		return;
	
	// If we made it this far, get the appropriate CC or BCC headers
	if (!m_pFrame->GetInfoBar().GetCC().IsEmpty())
		m_strCC = m_pFrame->GetInfoBar().GetCC();
	
	if (!m_pFrame->GetInfoBar().GetBCC().IsEmpty())
		m_strBCC = m_pFrame->GetInfoBar().GetBCC();
	
	// Now we need the subject
	m_strSubject = m_pFrame->GetInfoBar().m_strSubject;
	return;
}
	
BOOL CComposeView::ValidateMailHeaders()
{
	// NOTE: Call this function after you call PrepareMailHeaders()
	m_pStatusBar->SetText("Validating headers...  Please wait.");
	
	// Subject header must not be empty
	if (m_pFrame->GetInfoBar().m_strSubject.IsEmpty())
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_SUBJECT_REQ);
		return FALSE;                  
	}
	
	// To header must contain an '@'symbol and no spaces -- same rules
	// for CC and BCC lines
	if (m_strTo.IsEmpty()) 
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_TOADDRREQ, MB_ICONSTOP|MB_OK);
		return FALSE;                                   
	}
	
	if (m_strTo.Find(' ') != -1)
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_TO_NO_SPACES, MB_ICONSTOP|MB_OK);
		return FALSE;                                      
	}
	
	if (m_strCC.IsEmpty() && m_strBCC.IsEmpty())
	{
		// CC and BCC are optional headers
		
		// TODO: Add any additional, relevant tests here which are
		// not dependent on the CC and BCC headers.
		
		// Since we've made it this far, we've passed the above tests
		// so return TRUE
		return TRUE;
	}
	else
	{
		// Since we made it this far, obviously m_strCC AND m_strBCC aren't
		// empty, which means that one or the other or both of them are
		// filled
		if (!m_strCC.IsEmpty() && m_strBCC.IsEmpty())
		{
			// The CC header isn't empty, validate it
			if ((m_strCC.Find('@')) == -1 || (m_strCC.Find(' ')) != -1)
				return FALSE;    
			return TRUE;
		}
		else if (m_strCC.IsEmpty() && !m_strBCC.IsEmpty())
		{
			// The BCC header isn't empty, so validate it
			if ((m_strBCC.Find('@')) == -1 || (m_strBCC.Find(' ')) != -1)
				return FALSE;  
			return TRUE;
		}
		else if (!m_strCC.IsEmpty() && !m_strBCC.IsEmpty())
		{
			// Validate both CC and BCC
			if ((m_strCC.Find('@')) == -1 || (m_strCC.Find(' ')) != -1)
				return FALSE;    
			
			if ((m_strBCC.Find('@')) == -1 || (m_strBCC.Find(' ')) != -1)
				return FALSE;  				
			
			return TRUE;
		}
	}
	
	// TODO: Add any additional, relevant tests here
	
	// Since we made it this far, we've passed all the tests, so return
	// TRUE
	return TRUE;
}

BOOL CComposeView::ProcessRecipients()
{       
	// This function is what makes it possible for the user to enter
	// multiple recipients, seperated by commas, on the To, CC, and BCC
	// lines
	
	// Clear out the lists
	m_strToList.RemoveAll();
	m_strCCList.RemoveAll();
	m_strBCCList.RemoveAll();
	
	m_pFrame->UpdateBarInfo();
	m_strTo = m_pFrame->m_wndInfoBar.GetTO();
	m_strCC = m_pFrame->m_wndInfoBar.GetCC();
	m_strBCC = m_pFrame->m_wndInfoBar.GetBCC();
	
	CString strCurTo = "", strCurCC = "", strCurBCC = "";
	
	int nComma, nSpace, i;
	 
	m_pStatusBar->SetText(IDS_PROCESSING_TO);
	if ((nComma = m_strTo.Find(',')) == -1)
	{
		m_strToList.Add(m_strTo);
	}
	else                         
	{
		m_pStatusBar->SetText(IDS_PROCESSING_TO);
		strCurTo = m_strTo.Left(nComma);
		m_strTo = m_strTo.Right(m_strTo.GetLength() - nComma - 2);
		if (m_strTo.Left(1) == " ")
			m_strTo = m_strTo.Right(m_strTo.GetLength() - 1);
			
		// Add the first recipient to our list
		m_strToList.Add(strCurTo);

		while ((nComma = m_strTo.Find(',')) != -1)
		{   
			m_pStatusBar->SetText(IDS_PROCESSING_TO);
			// As long as the list has commas in it, keep
			// on parsing
			strCurTo = m_strTo.Left(nComma);
			m_strToList.Add(strCurTo);
				
			m_strTo = m_strTo.Right(m_strTo.GetLength() - nComma - 2);
			if (m_strTo.Left(1) == " ")    
				m_strTo = m_strTo.Right(m_strTo.GetLength() - 1);
		}
			
		if ((nComma = m_strTo.Find('@')) != -1)
		{   
			m_pStatusBar->SetText(IDS_PROCESSING_TO);         
			// After this final address, there's nothing left
			m_strToList.Add(m_strTo);
		}
	}      
	
	if (!m_strCC.IsEmpty())
	{
		m_pStatusBar->SetText(IDS_PROCESSING_CC);
		if ((nComma = m_strCC.Find(',')) == -1)
		{        
			m_pStatusBar->SetText(IDS_PROCESSING_CC);
			m_strCCList.Add(m_strCC);
		}
		else                         
		{  
			m_pStatusBar->SetText(IDS_PROCESSING_CC);
			strCurCC = m_strCC.Left(nComma);
			m_strCC = m_strCC.Right(m_strCC.GetLength() - nComma - 2);
			if (m_strCC.Left(1) == " ")
				m_strCC = m_strCC.Right(m_strCC.GetLength() - 1);
			
			// Add the first recipient to our list
			m_strCCList.Add(strCurCC);
			
			while ((nComma = m_strCC.Find(',')) != -1)
			{
				m_pStatusBar->SetText(IDS_PROCESSING_CC);
				// As long as the list has commas in it, keep
				// on parsing
				strCurCC = m_strCC.Left(nComma);
				m_strCCList.Add(strCurCC);
				
				m_strCC = m_strCC.Right(m_strCC.GetLength() - nComma - 2);
				if (m_strCC.Left(1) == " ")    
					m_strCC = m_strCC.Right(m_strCC.GetLength() - 1);
			}
			
			if ((nComma = m_strCC.Find('@')) != -1)
			{  
				m_pStatusBar->SetText(IDS_PROCESSING_CC);          
				// After this final address, there's nothing left
				m_strToList.Add(m_strCC);
			}
		}	
	}
	
	if (!m_strBCC.IsEmpty())
	{  
		m_pStatusBar->SetText(IDS_PROCESSING_BCC);
		if ((nComma = m_strBCC.Find(',')) == -1)
		{                      
			m_pStatusBar->SetText(IDS_PROCESSING_BCC);
			m_strBCCList.Add(m_strBCC);
		}
		else                         
		{
			m_pStatusBar->SetText(IDS_PROCESSING_BCC);
			strCurBCC = m_strBCC.Left(nComma);
			m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - nComma - 2);
			if (m_strBCC.Left(1) == " ")
				m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - 1);
			
			// Add the first recipient to our list
			m_strBCCList.Add(strCurBCC);
			
			while ((nComma = m_strBCC.Find(',')) != -1)
			{  
				m_pStatusBar->SetText(IDS_PROCESSING_BCC);
				// As long as the list has commas in it, keep
				// on parsing
				strCurBCC = m_strBCC.Left(nComma);
				m_strBCCList.Add(strCurBCC);
				
				m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - nComma - 2);
				if (m_strBCC.Left(1) == " ")    
					m_strBCC = m_strBCC.Right(m_strCC.GetLength() - 1);
			}
			
			if ((nComma = m_strBCC.Find('@')) != -1)
			{
				m_pStatusBar->SetText(IDS_PROCESSING_BCC);            
				// After this final address, there's nothing left
				m_strBCCList.Add(m_strBCC);
			}
		}	
	}
	
	// Now iterate through the To, CC, and BCC recipient arrays
	// and send a 'RCPT TO:' command to the server for each one
	CString response; 
	
	m_pStatusBar->SetText(IDS_SENDING_TO);
	for (i = 0;i < m_strToList.GetSize();i++)
	{ 
		m_pStatusBar->SetText(IDS_SENDING_TO);
	    m_pSocket->Send("RCPT TO: <" + m_strToList[i] + ">\r\n");
	    response = m_pSocket->GetLine(); 
	}
	
	m_pStatusBar->SetText(IDS_SENDING_CC);
	for (i = 0;i < m_strCCList.GetSize();i++)
	{   	
		m_pStatusBar->SetText(IDS_SENDING_CC);
		m_pSocket->Send("RCPT TO: <" + m_strCCList[i] + ">\r\n");
		response = m_pSocket->GetLine();
	}
	
	m_pStatusBar->SetText(IDS_SENDING_BCC);
	for (i = 0;i < m_strBCCList.GetSize();i++)
	{     
		m_pStatusBar->SetText(IDS_SENDING_BCC);
		m_pSocket->Send("RCPT TO: <" + m_strBCCList[i] + ">\r\n");
		response = m_pSocket->GetLine();
	}
	
	m_pStatusBar->ShowIdleMessage();
	// we're done! 
	return TRUE;
}

void CComposeView::SendMailHeaders()
{
	// NOTE: Call this function after sending the DATA command
   	m_pSocket->Send("To: " + m_strTo + "\r\n");
   	
   	if (!m_strCC.IsEmpty())
		m_pSocket->Send("CC: " + m_strCC + "\r\n");
		
    m_pSocket->Send("From: " + m_pDoc->m_strAddress + " (" + m_pDoc->m_strUserName + ")" + "\r\n");
    m_pSocket->Send("Subject: " + m_pFrame->GetInfoBar().GetSubject() + "\r\n");
    m_pSocket->Send("Reply-To: " + m_pDoc->m_strAddress + "\r\n");
    m_pSocket->Send("Organization: " + m_pDoc->m_strOrg + "\r\n"); 
    m_pSocket->Send("X-Mailer: Internet Navigator 2.0 for Windows\r\n");
    m_pSocket->Send("X-Mailer-Author: Brian Hart\r\n\r\n");
    
    // All headers are now sent
    return;	
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView Compose Mail Wizard: helps the user enter the subject and
// addressing information for their new message

LRESULT CComposeView::OnComposeWizard(WPARAM, LPARAM)
{  
	// Start the Compose Mail Wizard
	DoCompWizard();
	
	return 0;
}

void CComposeView::DoCompWizard()
{
	// Show Step 1
	CCompWiz1 wiz1(m_pApp->GetMainFrame());
	
	int nResult = wiz1.DoModal();
	switch (nResult)
	{
		case IDOK:		// Next button pressed
			// Go on to Step 2
			DoCompWiz2();
			break;
			
		case IDCANCEL:	// Cancel button pressed   
			GetParentFrame()->DestroyWindow();
			break;    			
	}
}

void CComposeView::DoCompWiz2()
{
	// Show Step 2
	CCompWiz2 wiz2(m_pApp->GetMainFrame(), this);
	wiz2.m_strSubject = m_pFrame->GetInfoBar().GetSubject();
	
	int nResult = wiz2.DoModal();
	CString strSubject = "";
	
	switch (nResult) 
	{
		case IDOK:		// Next button pressed
			// Go on to Step 3
			strSubject = wiz2.m_strSubject;
			m_pFrame->GetInfoBar().SetSubject(strSubject);
			m_pFrame->GetInfoBar().UpdateData(FALSE);
					
			DoCompWiz3();
			break;
		
		case IDRETRY:	// Back button pressed
			// Go back to Step 1
			strSubject = wiz2.m_strSubject;
			m_pFrame->GetInfoBar().SetSubject(strSubject);
			m_pFrame->GetInfoBar().UpdateData(FALSE);
					
			DoCompWizard();			
			break;
		
		case IDCANCEL:	// Cancel button pressed   
			GetParentFrame()->DestroyWindow();
			break;    			
	}
}

void CComposeView::DoCompWiz3()
{
	// Show Step 3
	CCompWiz3 wiz3(m_pApp->GetMainFrame(), this);
	wiz3.m_strTo = m_pFrame->GetInfoBar().GetTO();
	
	int nResult = wiz3.DoModal();
	CString strTo = "";
	
	switch (nResult) 
	{
		case IDOK:		// Next button pressed
			// Go on to Step 4
			strTo = wiz3.m_strTo;
			m_pFrame->GetInfoBar().SetTO(strTo);
			m_pFrame->GetInfoBar().UpdateData(FALSE);
					
			DoCompWiz4();
			break;
		
		case IDRETRY:	// Back button pressed
			// Go back to Step 2
			strTo = wiz3.m_strTo;
			m_pFrame->GetInfoBar().SetTO(strTo);
			m_pFrame->GetInfoBar().UpdateData(FALSE);
					
			DoCompWiz2();			
			break;
		
		case IDCANCEL:	// Cancel button pressed   
			GetParentFrame()->DestroyWindow();
			break;    			
	}
}

void CComposeView::DoCompWiz4()
{
	// Show Step 4
	CCompWiz4 wiz4(m_pApp->GetMainFrame(), this);
	wiz4.m_strCC = m_pFrame->GetInfoBar().GetCC();
	
	int nResult = wiz4.DoModal();
	CString strCC = "";
	
	switch (nResult) 
	{
		case IDOK:		// Next button pressed
			// Go on to Step 5
			strCC = wiz4.m_strCC;
			m_pFrame->GetInfoBar().SetCC(strCC);
			m_pFrame->GetInfoBar().UpdateData(FALSE);
					
			DoCompWiz5();
			break;
		
		case IDRETRY:	// Back button pressed
			// Go back to Step 3
			strCC = wiz4.m_strCC;
			m_pFrame->GetInfoBar().SetCC(strCC);
			m_pFrame->GetInfoBar().UpdateData(FALSE);
			
			DoCompWiz3();			
			break;
		
		case IDCANCEL:	// Cancel button pressed   
			GetParentFrame()->DestroyWindow();
			break;    			
	}
}

void CComposeView::DoCompWiz5()
{
	// Show Step 5
	CCompWiz5 wiz5(m_pApp->GetMainFrame(), this); 
	wiz5.m_strBCC = m_pFrame->GetInfoBar().GetBCC();
	
	int nResult = wiz5.DoModal();
	CString strBCC = "";
	
	switch (nResult) 
	{
		case IDOK:		// Next button pressed
			// Go on to Finish
			strBCC = wiz5.m_strBCC;
			m_pFrame->GetInfoBar().SetBCC(strBCC);
			m_pFrame->GetInfoBar().UpdateData(FALSE);
					
			DoCompWizFinish();
			break;
		
		case IDRETRY:	// Back button pressed
			// Go back to Step 4
			strBCC = wiz5.m_strBCC;
			m_pFrame->GetInfoBar().SetBCC(strBCC);
			m_pFrame->GetInfoBar().UpdateData(FALSE);
			DoCompWiz4();			
			break;
		
		case IDCANCEL:	// Cancel button pressed
			GetParentFrame()->DestroyWindow();
			break;    		
	}
}

void CComposeView::DoCompWizFinish()
{
	// Show the Finish -- Step 6
	CCompWizFinish wizFinish(m_pApp->GetMainFrame());
	
	int nResult = wizFinish.DoModal();
	switch (nResult)
	{
		case IDOK:		// Finish button pressed
			// Make this view and frame window visible -- the user is
			// finished!
			m_pFrame->ShowWindow(SW_SHOW);
			m_pFrame->UpdateWindow();
			break;                    
			
		case IDRETRY:	// Back button pressed
			// Go back to Step 5
			DoCompWiz5();
			break;
		
		case IDCANCEL:	// Cancel button pressed   
			GetParentFrame()->DestroyWindow();
			break;    			
	}
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView Feedback Wizard

LRESULT CComposeView::OnFeedbackWizard(WPARAM, LPARAM)
{           
	// Prepare feedback message 
	m_strFeedback = "Internet Navigator Version: 2.0\r\n";
	if (m_pApp->IsWindows95())
		m_strFeedback += "Operating System: Windows 95";
	else if (!m_pApp->IsWindows95())
		m_strFeedback += "Operating System: Windows 3.1, 3.11, or NT\r\n";
	m_strFeedback += "\r\nDear WnDBSoft Software International,\r\n\r\n";
	m_strFeedback += "Below is how I rated parts of Internet Navigator on";
	m_strFeedback += "\r\n";
	m_strFeedback += "a 1 to 5 scale, with 5 being excellent,";
	m_strFeedback += "\r\nusing the Feedback Wizard and the Ask the Staff area.";
	m_strFeedback += "\r\n---------------------------------------------------";
	m_strFeedback += "Feedback Ratings:\r\n\r\n";
	
	DoFeedbackWizard();
	return 0;
}

void CComposeView::DoFeedbackWizard()
{         
	CFeedBackWiz1 wiz1(m_pApp->GetMainFrame());
	
	int nResult = wiz1.DoModal();
	switch (nResult)
	{
		case IDCANCEL:
			m_bShowWindow = FALSE;
			GetParentFrame()->DestroyWindow();
			break;
		
		case IDOK:	// next
			DoFeedbackWiz2();
			break;
	}	
}

void CComposeView::DoFeedbackWiz2()
{
	CFeedBackWiz2 wiz2(m_pApp->GetMainFrame());
	wiz2.m_nShowComments = !m_bShowWindow;
	
	int nResult = wiz2.DoModal();
	switch (nResult)
	{
		case IDCANCEL:
			m_bShowWindow = FALSE;
			GetParentFrame()->DestroyWindow();
			break;
		
		case IDOK:   // next
			m_bShowWindow = !wiz2.m_nShowComments;
			DoFeedbackWiz3();
			break;
		
		case IDRETRY: // back
			m_bShowWindow = !wiz2.m_nShowComments;
			DoFeedbackWizard();
			break;
	}
}

void CComposeView::DoFeedbackWiz3()
{
	CFeedBackWiz3 wiz3(m_pApp->GetMainFrame());
	wiz3.m_nEase = m_nEase - 1;
	wiz3.m_nNav = m_nNav - 1;
	wiz3.m_nQuality = m_nQuality - 1;
	
	int nResult = wiz3.DoModal();
	switch (nResult)
	{
		case IDCANCEL:
			m_bShowWindow = FALSE;
			GetParentFrame()->DestroyWindow();
			break;
		
		case IDOK:		// next
			m_nEase = wiz3.m_nEase + 1;
			m_nNav = wiz3.m_nNav + 1;
			m_nQuality = wiz3.m_nQuality + 1;
			DoFeedbackWiz4();
			break;
		
		case IDRETRY:	// back
			m_nEase = wiz3.m_nEase + 1;
			m_nNav = wiz3.m_nNav + 1;
			m_nQuality = wiz3.m_nQuality + 1;
			DoFeedbackWiz2();
			break;           
	}
}

void CComposeView::DoFeedbackWiz4()
{
	CFeedBackWiz4 wiz4(m_pApp->GetMainFrame());
	wiz4.m_strAdd = m_strAdd;
	wiz4.m_strNix = m_strNix;
	
	int nResult = wiz4.DoModal();
	switch (nResult)
	{
		case IDCANCEL:
			m_bShowWindow = FALSE;
			GetParentFrame()->DestroyWindow();
			break;
		
		case IDOK:		// next
			m_strAdd = wiz4.m_strAdd;
			m_strNix = wiz4.m_strNix;
			DoFeedbackWiz5();
			break;
		
		case IDRETRY:	// back
			m_strAdd = wiz4.m_strAdd;
			m_strNix = wiz4.m_strNix;
			DoFeedbackWiz3();
			break;           
	}
}

void CComposeView::DoFeedbackWiz5()
{
	CFeedBackWiz5 wiz5(m_pApp->GetMainFrame());
	
	int nResult = wiz5.DoModal();
	switch (nResult)
	{
		case IDCANCEL:
			m_bShowWindow = FALSE;
			GetParentFrame()->DestroyWindow();
			break;
		
		case IDOK:		// Finish 
			PrepareFeedbackMessage();
			break; 
			
		case IDRETRY:	// Back 
			DoFeedbackWiz4();
			break;
	}	
}

void CComposeView::PrepareFeedbackMessage()
{
	m_strFeedback += "Navigation: ";
	switch (m_nNav)
	{
		case 1:
			m_strFeedback += "Poor\r\n";
			break;
		
		case 2:
			m_strFeedback += "Fair\r\n";
			break;
		
		case 3:
			m_strFeedback += "Average\r\n";
			break;
		
		case 4:
			m_strFeedback += "Good\r\n";
			break;
		
		case 5:
			m_strFeedback += "Excellent!\r\n";
			break;
	}
	
	m_strFeedback += "Ease of Use: ";
	switch (m_nEase)
	{
		case 1:
			m_strFeedback += "Poor\r\n";
			break;
		
		case 2:
			m_strFeedback += "Fair\r\n";
			break;
		
		case 3:
			m_strFeedback += "Average\r\n";
			break;
		
		case 4:
			m_strFeedback += "Good\r\n";
			break;
		
		case 5:
			m_strFeedback += "Excellent!\r\n";
			break;
	}
	
	m_strFeedback += "Online Help & Information: "; 
	switch (m_nQuality)
	{
		case 1:
			m_strFeedback += "Poor\r\n\r\n";
			break;
		
		case 2:
			m_strFeedback += "Fair\r\n\r\n";
			break;
		
		case 3:
			m_strFeedback += "Average\r\n\r\n";
			break;
		
		case 4:
			m_strFeedback += "Good\r\n\r\n";
			break;
		
		case 5:
			m_strFeedback += "Excellent!\r\n\r\n";
			break;
	}                             
	
	if (!m_strAdd.IsEmpty())
	{
		m_strFeedback += "Features which I would like to see in the next";
		m_strFeedback += "\r\nversion are:\r\n\r\n";
		m_strFeedback += m_strAdd + "\r\n\r\n";
	}
	
	if (!m_strNix.IsEmpty())
	{
		m_strFeedback += "You really gotta nix these features:\r\n\r\n";
		m_strFeedback += m_strNix;
	}
	
	if (!m_bShowWindow)
	{
		// Send message
		OnMailSend();
	}                  
	else
	{ 
		m_strFeedback += "\r\n";
		m_strFeedback += "\r\n-----------------------------------------------";
		m_strFeedback += "Additional Comments/Suggestions:\r\n\r\n";
		
		m_pFrame->ActivateFrame(SW_SHOW);
		m_pFrame->UpdateWindow();     
		                                                        
		MessageBeep(-1);		                                                        	
		AfxMessageBox("What you type in this feedback window will be sent "
			"tacked on the end of the message we formed from the comments you "
			"entered in the Feedback Wizard.", MB_ICONASTERISK); 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView Word Wrap support

BOOL CComposeView::IsWordWrap() const
{
	return (GetStyle() & ES_AUTOHSCROLL) == 0;
}

BOOL CComposeView::SetWordWrap(BOOL bWordWrap)
{
	bWordWrap = !!bWordWrap;    // make sure ==TRUE || ==FALSE
	if (IsWordWrap() == bWordWrap)
		return FALSE;

	// preserve original control's state.
	CFont* pFont = GetFont();
	int nLen = GetBufferLength();
	char FAR* pSaveText = new far char[GetBufferLength()+1];
	GetWindowText(pSaveText, nLen+1);

	// create new edit control with appropriate style and size.
	DWORD dwStyle = dwStyleDefault & ~(ES_AUTOHSCROLL|WS_HSCROLL|WS_VISIBLE);
	if (!bWordWrap)
		dwStyle |= ES_AUTOHSCROLL|WS_HSCROLL;

	CWnd* pParent = GetParent();
	CRect rect;
	GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	CWnd* pFocus = GetFocus();

	UINT nID = GetDlgCtrlID();

	HWND hWnd = ::CreateWindow("edit", NULL, dwStyle,
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
		pParent->m_hWnd, (HMENU)nID,
		(HINSTANCE)m_segText, NULL);

	if (hWnd == NULL)
	{
		delete[] pSaveText;
		return FALSE;
	}

	// set the window text to nothing to make sure following set doesn't fail
	SetWindowText(NULL);

	// restore visual state
	::SetWindowText(hWnd, pSaveText);
	delete[] pSaveText;
	if (pFont != NULL)
	{
		ASSERT(pFont->m_hObject != NULL);
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)pFont->m_hObject, 0);
	}

	// detach old window, attach new
	HWND hWndOld = Detach();
	::SetWindowLong(hWndOld, GWL_WNDPROC, (LONG)*GetSuperWndProcAddr());
	::SetWindowWord(hWndOld, GWW_ID, nID+1);
	ASSERT(m_hWnd == NULL);
	SubclassWindow(hWnd);
	ASSERT(m_hWnd == hWnd);
	UINT nTabStops = m_nTabStops;
	GetParentFrame()->SendMessage(WM_RECALCPARENT);
	GetEditCtrl().SetTabStops(nTabStops);
	GetClientRect(&rect);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_DRAWFRAME);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	UpdateWindow();

	// destroy old
	::SetWindowPos(hWndOld, NULL, 0, 0, 0, 0,
		SWP_HIDEWINDOW|SWP_NOREDRAW|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|
		SWP_NOZORDER);
	::DestroyWindow(hWndOld);

	// restore rest of state...
	GetEditCtrl().LimitText(nMaxSize);
	if (pFocus == this)
		SetFocus();

	ASSERT_VALID(this);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView drawing

void CComposeView::OnDraw(CDC* pDC)
{
	CMailBoxDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView printing

BOOL CComposeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default prepare printing
	return CEditView::OnPreparePrinting(pInfo);
}

void CComposeView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Begin print loop by calling default version -- let the
	// base class do its stuff first
	CEditView::OnBeginPrinting(pDC, pInfo);

	CString strFileName = GetDocument()->GetPathName(); 
	if (strFileName.IsEmpty()) strFileName = GetDocument()->GetTitle(); 
	BOOL bForceSysTime = strchr(strFileName, '.') == NULL;
	CTime timeSys = CTime::GetCurrentTime();
	CFileStatus status;
	CFile::GetStatus(strFileName, status);
	
	if (m_nHeaderTime != 0 || bForceSysTime)
		m_timeHeader = timeSys;
	else
		m_timeHeader = status.m_mtime;
    	
    if (m_nFooterTime != 0 || bForceSysTime)
		m_timeFooter = timeSys;
	else
		m_timeFooter = status.m_mtime;
 
	if (!pInfo->m_bPreview)
		return;

	pInfo->m_nCurPage = 0xFFFF;
	OnPrepareDC(pDC, pInfo);

	UINT nIndex = LOWORD(GetEditCtrl().GetSel());
	UINT nCurPage = 1;
	while (nCurPage < (UINT)m_aPageStart.GetSize())
	{
		if (nIndex < m_aPageStart[nCurPage])
			break;
		nCurPage++;
	}
	pInfo->m_nCurPage = nCurPage;        
	
	// Fill the CPrintInfo struct with needed information
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(m_aPageStart.GetSize()); // number of pages of message
	pInfo->m_pPD->m_pd.nMinPage = 1;
	pInfo->m_pPD->m_pd.nMaxPage = m_aPageStart.GetSize();
	pInfo->m_pPD->m_pd.nFromPage = 1;
	pInfo->m_pPD->m_pd.nToPage = m_aPageStart.GetSize();

	m_nPreviewPage = nCurPage;
}

void CComposeView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{	    
	// get string to show as "New Mail Message" in header/footer
	CPageSetupSheet shtPageSetup;
	CString strFileName = GetDocument()->GetPathName();
	if (strFileName.IsEmpty())
		strFileName = GetDocument()->GetTitle();

	// go thru CPageSetupSheet::CPageSetupPage to format the header 
	// and footer
	m_pFrame->UpdateBarInfo();
	CString strHeader = m_pFrame->m_wndInfoBar.GetSubject();
	if (strHeader.IsEmpty())
		strHeader = "New Mail Message";
	CString strFooter = "";        
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.FormatFooter(strFooter, m_timeFooter, strFileName,
		pInfo->m_nCurPage);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cyChar = tm.tmHeight;
	CRect rectPage = pInfo->m_rectDraw;

	// draw and exclude space for header
	if (!strHeader.IsEmpty())
	{
		pDC->TextOut(rectPage.left, rectPage.top, strHeader);
		rectPage.top += cyChar + cyChar / 4;
		pDC->MoveTo(rectPage.left, rectPage.top);
		pDC->LineTo(rectPage.right, rectPage.top);
		rectPage.top += cyChar / 4;
	}

	// allow space for footer
	pInfo->m_rectDraw = rectPage;
	if (!strFooter.IsEmpty())
		pInfo->m_rectDraw.bottom -= cyChar + cyChar/4 + cyChar/4;

	// draw body text
	CEditView::OnPrint(pDC, pInfo);

	// draw footer
	if (!strFooter.IsEmpty())
	{
		rectPage.bottom -= cyChar;
		pDC->TextOut(rectPage.left, rectPage.bottom, strFooter);
		rectPage.bottom -= cyChar / 4;
		pDC->MoveTo(rectPage.left, rectPage.bottom);
		pDC->LineTo(rectPage.right, rectPage.bottom);
		rectPage.bottom -= cyChar / 4;
	}
}

void CComposeView::OnScrollTo(CDC*, CPrintInfo* pInfo, POINT)
{
	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage < (UINT)m_aPageStart.GetSize());
	if (nPage != m_nPreviewPage)
	{
		UINT nIndex = m_aPageStart[nPage];
		GetEditCtrl().SetSel((int)nIndex, (int)nIndex);
	}
}

void CComposeView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CComposeView diagnostics

#ifdef _DEBUG
void CComposeView::AssertValid() const
{
	CEditView::AssertValid();
}

void CComposeView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMailBoxDoc* CComposeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMailBoxDoc)));
	return (CMailBoxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CComposeView message handlers

int CComposeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;            
		
	if (m_lfDefFont.lfHeight != 0)
	{
		m_font.CreateFontIndirect(&m_lfDefFont);
		SetFont(&m_font);
	}
	if (m_lfDefPrintFont.lfHeight != 0)
	{
		m_fontPrint.CreateFontIndirect(&m_lfDefPrintFont);
		SetPrinterFont(&m_fontPrint);
	}
	return 0;
}

void CComposeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParentFrame()->BringWindowToTop();
	
	// TODO: Add code here for a right-button menu for editing and
	// formatting commands
	
	CEditView::OnRButtonDown(nFlags, point);
}

void CComposeView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);

	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
}

void CComposeView::OnMirrorDisplayFont()
{
	if (GetPrinterFont() != NULL)
	{
		SetPrinterFont(NULL);
		m_lfDefPrintFont.lfHeight = 0;
		
		AfxMessageBox("Print font is now the same as the display font.",
				MB_OK|MB_ICONINFORMATION);
	}
}

void CComposeView::OnWordWrap()
{
	SetWordWrap(!IsWordWrap());
	m_bDefWordWrap = IsWordWrap();
}

void CComposeView::OnUpdateWordWrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsWordWrap());
}

void CComposeView::OnChoosePrintFont()
{
	CFont* pFont = GetPrinterFont();
	LOGFONT lf;
	LPLOGFONT plf = NULL;
	if (pFont != NULL)
	{
		pFont->GetObject(sizeof(LOGFONT), &lf);
		plf = &lf;
	}

	// magic to get printer dialog that would be used if we were printing!
	CPrintDialog dlgPrint(FALSE);
	if (!AfxGetApp()->GetPrinterDeviceDefaults(&dlgPrint.m_pd))
	{
		AfxMessageBox(IDP_ERR_GET_DEVICE_DEFAULTS);
		return;
	}

	HDC hdcPrint = dlgPrint.CreatePrinterDC();
	if (hdcPrint == NULL)
	{
		AfxMessageBox(IDP_ERR_GET_PRINTER_DC);
		return;
	}

	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));
	CDC dcPrint;
	dcPrint.Attach(hdcPrint);

	if (plf != NULL)
	{
		// need to map initial logfont to screen metrics.
		::ScaleLogFont(plf, dcPrint, dcScreen);
	}

	// now bring up the dialog since we know the printer DC
	CFontDialog dlg(plf, CF_PRINTERFONTS, &dcPrint);
	if (dlg.DoModal() == IDOK)
	{
		// map the resulting logfont back to printer metrics.
		lf = dlg.m_lf;
		::ScaleLogFont(&lf, dcScreen, dcPrint);

		m_fontPrint.DeleteObject();
		if (m_fontPrint.CreateFontIndirect(&lf))
		{
			SetPrinterFont(&m_fontPrint);
			m_lfDefPrintFont = lf;
		}
	}
	//NOTE: destructor will call dcPrint.DeleteDC

	::ReleaseDC(NULL, dcScreen.Detach());
}

void CComposeView::OnUpdateChoosePrintFont(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetPrinterFont() != NULL);
}

void CComposeView::OnChooseFont()
{
   // get current font description
   CFont* pFont = GetFont();
   LOGFONT lf;
   if (pFont != NULL)
	   pFont->GetObject(sizeof(LOGFONT), &lf);
   else
	   ::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

	CFontDialog dlg(&lf, CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
	if (dlg.DoModal() == IDOK)
	{
		// switch to new font.
		m_font.DeleteObject();
		if (m_font.CreateFontIndirect(&lf))
		{
			CWaitCursor wait;
			SetFont(&m_font);
			m_lfDefFont = lf;
    	}
	}
}

void CComposeView::OnPageSetup()
{
	CPageSetupSheet shtPageSetup(AfxGetMainWnd());      
	shtPageSetup.m_pgHeaderFooter.Initialize();
	shtPageSetup.m_pgHeaderFooter.m_bOutputView = TRUE;
	shtPageSetup.m_pgTabStops.m_nTabStops = m_nTabStops/4;
	if (shtPageSetup.DoModal() == IDOK)
	{
		// get values entered into this tabbed dialog box
		shtPageSetup.m_pgHeaderFooter.Terminate();
		
		m_nHeaderTime = shtPageSetup.m_pgHeaderFooter.m_nHeaderTime;
		m_nFooterTime = shtPageSetup.m_pgHeaderFooter.m_nFooterTime;
		SetTabStops(shtPageSetup.m_pgTabStops.m_nTabStops*4);
		m_nDefTabStops = m_nTabStops;
     		
		return;
	}  
	return;
}

void CComposeView::OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI)
{
	// Only put a check mark next to this menu item
	// if the printer font is the same as the font used
	// for display
	pCmdUI->SetCheck(GetPrinterFont() == NULL);
}

void CComposeView::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strTextSect = "Text";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strTextSect, "TextLeft", rect.left);
	pApp->WriteProfileInt(strTextSect, "TextRight", rect.right);
	pApp->WriteProfileInt(strTextSect, "TextTop", rect.top);
	pApp->WriteProfileInt(strTextSect, "TextBottom", rect.bottom);
	pApp->WriteProfileInt(strTextSect, "TextCX", rect.Width());
	pApp->WriteProfileInt(strTextSect, "TextCY", rect.Height());
}

void CComposeView::OnEditUndo()
{	
	CEditView::OnEditUndo();
	
	// change the "redo flag" to TRUE so the undone action can be redone
	m_bUndo = TRUE;
	return;
}

void CComposeView::OnEditRedo()
{
	if (m_bUndo)
	{
		CEditView::OnEditUndo();
		m_bUndo = FALSE;	// can't redo anymore -- not until another undo
	}
	return;	// we're done
}	

void CComposeView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bUndo && GetEditCtrl().CanUndo());
	return;
} 

void CComposeView::OnMailSend()
{
	// First, get the headers the user entered into the dialog bar
	// into the member variables of this view in a transmittable form
	
	CComposeWnd* pFrame = (CComposeWnd*)GetParentFrame();
	
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP|MB_OK);
		return;
	}
	
	pFrame->ActivateFrame(SW_RESTORE);
	pFrame->UpdateWindow();         
	pFrame->ActivateFrame(SW_HIDE);
	pFrame->UpdateWindow();
	
	m_pApp->GetMainFrame()->ResetToolBar();
	
	// Before connecting to the server, gather and validate the headers
	PrepareMailHeaders();

	if (!m_pApp->IsOnline()) // Internet Navigator isn't connected
	{   						// to the Internet, so begin a FlashConnect
		CString strText = "Starting FlashConnect...  Please wait.";
		m_pStatusBar->SetText(strText);                                   
		if (!m_pApp->SetFlashConnect(TRUE))           
		{
			m_pStatusBar->ShowIdleMessage();
			pFrame->ActivateFrame(SW_RESTORE);
			pFrame->UpdateWindow();
			return; // user cancelled the FlashConnect
		}
	}
	
	delete m_pSocket;
	m_pSocket = NULL;
	
	m_pSocket = new QSocket();
	
	m_pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);
	
	m_pStatusBar->SetText("Connecting to mail server...  Please wait.");
	if (!m_pSocket->Connect(m_pDoc->m_strSMTPHost, 25))
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_NOCONNECT_SMTP_HOST, MB_OK);
        
        if (m_pSocket)
        {
			delete m_pSocket;
			m_pSocket = NULL;
		}
		
		// If this is a FlashConnect, end it now
		if (m_pApp->IsFlashConnect() && m_pApp->SignOff())
		{ 
			m_pApp->AbortFlashConnect();
			m_bDisconnectAfterSend = FALSE;
			m_pStatusBar->ShowIdleMessage();
			pFrame->ActivateFrame(SW_RESTORE);
			pFrame->UpdateWindow();
			return;
		}
		pFrame->ShowWindow(SW_SHOW);
		pFrame->UpdateWindow();
		return;
	}
}

void CComposeView::OnEditChange()
{     
	m_bUndo = FALSE;
	
	GetDocument()->SetModifiedFlag(FALSE);
	return;
}  




