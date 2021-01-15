// finger.cpp - implementation of the CFingerDoc, CFingerView, and CFingerWnd
// classes which make up the Internet Users Information area of Internet
// Navigator
//

#include "stdafx.h"
#include "inetnav.h"

#include "finger.h"   
#include "dialogs.h"    
#include "statbar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFingerDoc  - holds data received from the Internet and stores it in files
//

IMPLEMENT_DYNCREATE(CFingerDoc, CDocument)

BEGIN_MESSAGE_MAP(CFingerDoc, CDocument)
	//{{AFX_MSG_MAP(CFingerDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFingerDoc construction/destruction

CFingerDoc::CFingerDoc()
{
	//{{AFX_DATA_INIT(CFingerDoc)
	m_strHost = "";
	m_strUser = "";
	m_strOutput = "";
	//}}AFX_DATA_INIT 
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

/*
//{{AFX_DATA_MAP(CFingerDoc)
//}}AFX_DATA_MAP
*/

CFingerDoc::~CFingerDoc()
{
}

BOOL CFingerDoc::OnNewDocument()
{
	CFingerAddr theDialog(m_pApp->GetMainFrame());
	theDialog.m_bDontShowAgain = !m_pApp->IsAskFingerAddr();
	
	if (m_pApp->IsAskFingerAddr())
	{
		if (theDialog.DoModal() == IDOK) // user pressed OK
		{ 
			// if the user doesn't want to have the Address dialog box
			// shown to them again, then tell the application that
			
			m_pApp->SetAskFingerAddr(!theDialog.m_bDontShowAgain);
			
			// get the user and host 
			if (theDialog.m_strAddr.IsEmpty())
			{
				m_strAddress = "";
				m_strUser = "";
				m_strHost = "";
				
				SetTitle("Internet Users Information");
				m_bFirst = FALSE;
			}
			else
			{
				m_strAddress = theDialog.m_strAddr;
			
				int nUserLength = m_strAddress.Find('@');
				m_strUser = m_strAddress.Left(nUserLength);
			
				int nHostStart = m_strAddress.ReverseFind('@');
				m_strHost = m_strAddress.Right(m_strAddress.GetLength() 
					- nHostStart - 1);
				
				m_bFirst = TRUE;
				SetTitle("Internet Users Information: " + m_strAddress);		
			}
		}
		else // user pressed Cancel 
		{
			return FALSE;                         
		}
	}		                                               
	else
	{
		SetTitle("Internet Users Information");
		m_bFirst = FALSE;          
	}

	// make a new instance of this document now
	if (!CDocument::OnNewDocument())
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC, MB_ICONSTOP|MB_OK);
		return FALSE;                                                
	}
		
	return TRUE;
}

BOOL CFingerDoc::OnOpenDocument(const char* pszPathName)
{
	CString strTitle = "";
	
	if (!CDocument::OnOpenDocument(pszPathName))   
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC, MB_ICONSTOP|MB_OK);
		return FALSE;
	}   
	
	if (!m_strAddress.IsEmpty() && !m_strHost.IsEmpty() && !m_strUser.IsEmpty())
	{
		strTitle = "Internet Users Information: ";
		strTitle += m_strAddress;
	}
	else
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC, MB_ICONSTOP|MB_OK);
		return FALSE;
	}
	
	SetTitle(strTitle);   

	return TRUE;
}

BOOL CFingerDoc::SaveModified()
{
	if (!IsModified())
		return TRUE;        // ok to continue

	CString strName = "the Internet Users Information results";

	CString prompt = "";
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, strName);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
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

void CFingerDoc::SetTitle(const char* pszTitle)
{
	CString strTitle = "Internet Users Information";
	if (m_strHost.IsEmpty() && m_strUser.IsEmpty())
	{
		m_strTitle = strTitle;
		UpdateFrameCounts();
		return;
	}
	else if (!m_strHost.IsEmpty() && !m_strUser.IsEmpty())
	{
		m_strAddress = m_strUser + '@' + m_strHost;
		m_strTitle = strTitle + ": " + m_strAddress;
		UpdateFrameCounts();
		return;
	}
	else
	{
		m_strTitle = strTitle;
		UpdateFrameCounts();
		return;
	}
}
	
/////////////////////////////////////////////////////////////////////////////
// CFingerDoc serialization

void CFingerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strHost;
		ar << m_strUser;
		ar << m_strOutput;
		ar << m_strAddress;
	}
	else
	{
		ar >> m_strHost;
		ar >> m_strUser;
		ar >> m_strOutput;	
		ar >> m_strAddress;
	}
}

void CFingerDoc::DeleteContents()
{   
	// This function is called to delete the document's data, which are the
	// address of the host system to connect to, the user on which to 
	// retrieve information, and the information which the host sends  
	if (m_bFirst)
	{                  
		m_bFirst = FALSE;
		return;
	}
	
	m_strHost.Empty();
	m_strUser.Empty();
	m_strOutput.Empty();  
	m_strAddress.Empty();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFingerDoc diagnostics

#ifdef _DEBUG
void CFingerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFingerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFingerDoc commands

void CFingerDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// We want to enable the File Save menu item only if the user has 
	// modified the document.
	//
	// CDocument::IsModified() gives us a TRUE value if the document has
	// been modified; we use this return value for the BOOL bOn 
	// parameter of CCmdUI::Enable().
	pCmdUI->Enable(IsModified());
	return;	
}

void CFingerDoc::OnFileSave()
{ 
	if (!m_strUser.IsEmpty() && !m_strHost.IsEmpty())
		m_strAddress = m_strUser + '@' + m_strHost;
  	
  	CDocument::OnFileSave();
  	
    SetTitle("");
}

void CFingerDoc::OnFileSaveAs()
{
	if (!m_strUser.IsEmpty() && !m_strHost.IsEmpty())
		m_strAddress = m_strUser + '@' + m_strHost;
  	
  	CDocument::OnFileSaveAs();
  	
    SetTitle("");
}

/////////////////////////////////////////////////////////////////////////////
// CFingerView  - displays Finger data to the user

IMPLEMENT_DYNCREATE(CFingerView, CFormView)

BEGIN_MESSAGE_MAP(CFingerView, CFormView)
	//{{AFX_MSG_MAP(CFingerView)
	ON_BN_CLICKED(IDC_LOOKUP, OnLookup)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_HELP, OnClickedHelp)
	ON_EN_CHANGE(IDC_HOST, OnEditChange)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_BN_CLICKED(IDC_CANCEL, OnLookupCancel)
	ON_UPDATE_COMMAND_UI(ID_LOOKUP_CANCEL, OnUpdateLookupCancel)
	ON_UPDATE_COMMAND_UI(ID_LOOKUP, OnUpdateLookup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_COMMAND(ID_SCROLL_LINE_DOWN, OnScrollLineDown)
	ON_COMMAND(ID_SCROLL_LINE_UP, OnScrollLineUp)
	ON_COMMAND(ID_SCROLL_PAGE_DOWN, OnScrollPageDown)
	ON_COMMAND(ID_SCROLL_PAGE_UP, OnScrollPageUp)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_EN_CHANGE(IDC_USER, OnEditChange)
	ON_EN_CHANGE(IDC_OUTPUT, OnEditChange)
	ON_COMMAND(ID_LOOKUP, OnLookup)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, OnClear)
	ON_COMMAND(ID_LOOKUP_CANCEL, OnLookupCancel)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Socket response message
	ON_MESSAGE(WM_SOCKET_RESPONSE, OnSocketResponse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFingerView construction/destruction

CFingerView::CFingerView()
	: CFormView(CFingerView::IDD)
{
	//{{AFX_DATA_INIT(CFingerView)
	m_pDoc = NULL;
	//}}AFX_DATA_INIT                  
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_pSocket = NULL;
	m_bUndo = FALSE; // starts out with Undo
	// TODO: add construction code here
	m_pPercentDialog = NULL;     
	
	m_bSuccess = FALSE;
}

CFingerView::~CFingerView()
{
	delete m_pSocket; // cleanup the memory used by the socket
	m_pSocket = NULL;           

	delete m_pPercentDialog; // cleanup memory used by the percent dialog
	m_pPercentDialog = NULL;
}

void CFingerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFingerView)
	DDX_Control(pDX, IDC_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_USER, m_ctlUser);
	DDX_Control(pDX, IDC_OUTPUT, m_ctlOutput);
	DDX_Control(pDX, IDC_HOST, m_ctlHost);
	DDX_Control(pDX, IDC_LOOKUP, m_btnLookup);
	DDX_Control(pDX, IDC_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_CANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_HOST, m_pDoc->m_strHost);
	DDV_MaxChars(pDX, m_pDoc->m_strHost, 255);
	DDX_Text(pDX, IDC_USER, m_pDoc->m_strUser);
	DDX_Text(pDX, IDC_OUTPUT, m_pDoc->m_strOutput);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CFingerView update helper functions

void CFingerView::OnInitialUpdate()
{   
	// Initialize the view's pointer to the document
	m_pDoc = GetDocument();      
	
	// call base class initial update
	CFormView::OnInitialUpdate();

	// subclass this  view with CTL3D so it looks nice
	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL);
	
	// Disable all buttons and commands except Lookup and Help
	m_btnLookup.EnableWindow(m_pApp->IsOnline());
	m_btnHelp.EnableWindow(m_pApp->IsOnline());
	m_btnClear.EnableWindow(FALSE);
	m_btnCancel.EnableWindow(FALSE);
 
	if (!m_pDoc->m_strHost.IsEmpty() || !m_pDoc->m_strUser.IsEmpty()
		|| !m_pDoc->m_strOutput.IsEmpty())
	{
		m_btnClear.EnableWindow(TRUE);
	}
	else
		m_btnClear.EnableWindow(FALSE);
  
	return;
}

void CFingerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// display document's data by moving it from data variables to controls
	// with CWnd::UpdateData(bSave=FALSE);
	UpdateData(FALSE);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFingerView printing

BOOL CFingerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->m_nNumPreviewPages = 1;
	return DoPreparePrinting(pInfo);
}

void CFingerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{  
	CFormView::OnBeginPrinting(pDC, pInfo);
	return;
}

void CFingerView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// let's just allow the printer DC to use its default font

    // We want a header that tells on whom the user got information
    CString strHeader = "Information on " + m_pDoc->m_strAddress;
    PrintPageHeader(pDC, pInfo, strHeader);
	
	// Now print the information obtained from the Finger query	
	CString strOutputCtlContents = "";
	
	// next, take the text from the output box and draw it on the
	// printer device context  
	m_ctlOutput.GetWindowText(strOutputCtlContents);

	if (m_pDoc->m_strOutput != strOutputCtlContents)  
		UpdateData(TRUE);
	if (!m_pDoc->m_strOutput.IsEmpty())
	{
		strOutputCtlContents = m_pDoc->m_strOutput;
		pDC->DrawText(strOutputCtlContents, -1, pInfo->m_rectDraw, 
				DT_LEFT|DT_NOPREFIX|DT_WORDBREAK);
	}

	return;
}

void CFingerView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFormView::OnEndPrinting(pDC, pInfo);
}

void CFingerView::PrintPageHeader(CDC* pDC, CPrintInfo* pInfo,
			CString& strHeader)
{
	// Print a page header consisting of the name of
	// the document and a horizontal line
	pDC->TextOut(0, 25, strHeader);  // 1/4 inch down

	// Draw a line across the page, below the header
	TEXTMETRIC textMetric;
	pDC->GetTextMetrics(&textMetric);
	int y = 35 + textMetric.tmHeight;          // line 1/10th inch below text
	pDC->MoveTo(0, y);                          // from left margin
	pDC->LineTo(pInfo->m_rectDraw.right, y);    // to right margin
            
	// Subtract out from the drawing rectange the space used by the header.
	y += 25;    // space 1/4 inch below (top of) line
	pInfo->m_rectDraw.top += y; 
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFingerView user-interface helper functions 

void CFingerView::EmptyUndoFlags()
{
	// resets undo flags in Host, User, and Output controls
	m_ctlHost.EmptyUndoBuffer();
	m_ctlUser.EmptyUndoBuffer();
	m_ctlOutput.EmptyUndoBuffer();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFingerView diagnostics

#ifdef _DEBUG
void CFingerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFingerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFingerDoc* CFingerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFingerDoc)));
	return (CFingerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFingerView socket response message handler(s)

LRESULT CFingerView::OnSocketResponse(WPARAM amount, LPARAM buffer)
{
	if ((int)amount > 0)
	{
	    if (buffer != 0)
    	{     
			// tell the user we're receiving information 
			if (m_pApp->IsFingerReportQueryProgress())
				m_pStatusBar->SetText(IDS_RECEIVING_RESPONSE);
			/*wait.Restore();*/
			
			// Don't have to use amount as your data will
			// not contain NULLS and the socket class NULL
			// terminates the buffer 
			
			// Turn newline characters to carriage-return-linefeed pairs
			// so the output doesn't have ugly vertical bars in it;
			// instead it has line breaks where the \r\n is
			
			m_pPercentDialog = new CPercentDialog(this);
			m_pPercentDialog->Initialize("Information Retrieval Progress",
			"Please wait while we receive and process information from " 
				+ m_pDoc->m_strHost + "...  Thank you for your patience!");
			
			                               
            for (int i = 0; i < (int)amount; i++)
            {
            	if (((char*)buffer)[i] == '\n' &&
            		(i == 0 || (((char*)buffer)[i - 1] != '\r')))
            	{
            		// insert "\r"s before any "\n"s so we make a "\r\n"
            		// pair where the line should wrap back to the left
            		m_pDoc->m_strOutput += '\r';
            	}
            	m_pDoc->m_strOutput += ((char*)buffer)[i];
            	m_pDoc->SetModifiedFlag();
            	UpdateData(FALSE);
            
            	m_pPercentDialog->CalculatePercent(i + 1, (int)amount);
            	m_pPercentDialog->UpdateWindow();
           	}
           	m_pDoc->SetModifiedFlag();
			UpdateData(FALSE); // Fills output area on screen
			
			m_pPercentDialog->SetPercent(100);
			m_pPercentDialog->UpdateWindow();
			m_pPercentDialog->ShowWindow(SW_HIDE);
			m_pPercentDialog->DestroyWindow();
			
			delete m_pPercentDialog;
			m_pPercentDialog = NULL; 
			
			m_bSuccess = TRUE;
		}
	}
	else
	{
		// if amount < 0 it is a receive command
		switch ((SocketReceiveCmd)amount)
		{
			case SocketStatusChanged:
				switch (m_pSocket->GetStatus())
				{       
					case CONNECTING:
						break;
						
					case CONNECTED:
						if (!m_pDoc->m_strOutput.IsEmpty())
						{
							m_pDoc->m_strOutput.Empty();
							UpdateData(FALSE);
							// Clears output area on screen 
						}
						if (m_pApp->IsFingerReportQueryProgress())
							m_pStatusBar->SetText(IDS_SENDING_REQUEST);
						/*wait.Restore();*/
						// Send the finger request
						m_pSocket->Send(m_pDoc->m_strUser + "\r\n"); 
						if (m_pApp->IsFingerReportQueryProgress()) 
						{
							m_pStatusBar->SetText("Information request has "
							"been sent and acknowledged.");
						}
						// After the server processes the Finger command
						// we have just sent, the data it will send back 
						// comes in the if ((int)amount > 0) block above.
						break;
					
					case DISCONNECTED:
						// The server has cut its link with us because
						// it has sent us all the information it has
						// or wants to send.

						// CFingerView::OnFinishReceive handles activating
						// user preferences upon completion of information
						// reception
						OnFinishReceive();
						m_btnLookup.EnableWindow(m_pApp->IsOnline());
						m_btnCancel.EnableWindow(FALSE);
						break;
					
					case ERRORSTATE:    
						m_bSuccess = FALSE;
						if (m_pApp->IsFingerReportQueryProgress())  
						{
							m_pStatusBar->SetText("Failed to get information "
							"because of a network error.");
						}
						AfxMessageBox(IDP_FAILED_GET_INFO,
							MB_ICONSTOP|MB_OK);
						// re-enable Lookup button, disable Cancel button
						m_btnLookup.EnableWindow(m_pApp->IsOnline());
						m_btnCancel.EnableWindow(FALSE); 
						m_pStatusBar->ShowIdleMessage();
						// all other status states represent an error
						/*wait.Restore();*/
						break;
					
					case TIMEDOUT:  
						m_bSuccess = FALSE;
						if (m_pApp->IsFingerReportQueryProgress())
						{
							m_pStatusBar->SetText("Network call timed out.");
						}
						m_btnLookup.EnableWindow(m_pApp->IsOnline());
						m_btnCancel.EnableWindow(FALSE);
						m_pStatusBar->ShowIdleMessage();
						
					default: 
						m_bSuccess = FALSE;
						if (m_pApp->IsFingerReportQueryProgress()) 
						{
							m_pStatusBar->SetText("Failed to get information "
							"because of a network error.");
						}
						AfxMessageBox(IDP_FAILED_GET_INFO,
							MB_ICONSTOP|MB_OK);
						// re-enable Lookup button, disable Cancel button
						m_btnLookup.EnableWindow(m_pApp->IsOnline());
						m_btnCancel.EnableWindow(FALSE); 
						m_pStatusBar->ShowIdleMessage();
						// all other status states represent an error
						/*wait.Restore();*/
						break;
				}
				break;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CFingerView message handlers

void CFingerView::OnLookup()
{    
	m_pDoc = GetDocument();

	// don't proceed if we're offline
	if (!m_pApp->IsOnline())
		return;
	
	// Now disable and enable buttons to show the user something's happening  
	m_btnLookup.EnableWindow(FALSE);                                        
	m_btnCancel.EnableWindow(m_pApp->IsOnline());
	
	// Inform the user we're starting the Finger query...
	if (m_pApp->IsFingerReportQueryProgress())
		m_pStatusBar->SetText(IDS_RESOLVING_HOSTNAME);
	
	if (!UpdateData(TRUE))              
	{
		m_btnLookup.EnableWindow(FALSE);
		m_btnCancel.EnableWindow(m_pApp->IsOnline());
		
		m_pStatusBar->ShowIdleMessage();
		return;
	}

	if (!m_pDoc->m_strHost.IsEmpty() || !m_pDoc->m_strUser.IsEmpty())
	{
		m_pDoc->m_strOutput.Empty();  
		m_pDoc->SetModifiedFlag(); // we've changed document data
		UpdateData(FALSE); //clears output area on-screen
		delete m_pSocket;
		m_pSocket = new QSocket(TRUE);
		m_pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);

		m_pDoc->m_strAddress = m_pDoc->m_strUser + '@' + m_pDoc->m_strHost;
		m_pDoc->SetModifiedFlag();
		m_pDoc->SetTitle("Internet Users Information: " + m_pDoc->m_strAddress);
		
		CString strText = "Contacting " + m_pDoc->m_strHost;
		strText += "...  Please wait.";
		if (m_pApp->IsFingerReportQueryProgress())
			m_pStatusBar->SetText(strText);
		strText.Empty();
		
		if (!m_pSocket->Connect(m_pDoc->m_strHost, 79))
		{     
			m_pStatusBar->ShowIdleMessage();
			AfxMessageBox(IDP_NOCONNECT_HOST);
			delete m_pSocket;
			m_pSocket = 0;  
			
			m_btnLookup.EnableWindow(m_pApp->IsOnline());
			m_btnCancel.EnableWindow(FALSE);
			return;
		}

		return;		
	}
	else
	{
		AfxMessageBox(IDP_INFO_NEEDED, MB_ICONSTOP|MB_OK);
		return;
	}
}

void CFingerView::OnClear()
{
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	CMultiDocTemplate* pFinger = pApp->GetFingerType();
	
	// make sure the document is saved, if the user wants to
	/*wait.Restore();*/
	m_pDoc->m_strHost.Empty();
	m_pDoc->m_strUser.Empty();
	m_pDoc->m_strOutput.Empty(); 
	m_pDoc->SetModifiedFlag(TRUE);
	m_pDoc->UpdateAllViews(NULL, 0L, NULL);      
	
	m_bSuccess = FALSE;	
	m_btnClear.EnableWindow(FALSE);
	return;
}

void CFingerView::OnLookupCancel()
{ 
	m_bSuccess = FALSE;
	
    // cancel lookup operation
    if (m_pSocket) // if m_pSocket != NULL then we're currently using it
    {
 		// get rid of the socket
    	delete m_pSocket;
    	m_pSocket = NULL;
    	
    	// clear settings
    	m_pDoc->m_strHost.Empty();
    	m_pDoc->m_strUser.Empty();
    	m_pDoc->m_strOutput.Empty();
    	m_pDoc->m_strAddress.Empty();
    	m_pDoc->SetTitle("Internet Users Information"); 
    	m_pDoc->SetModifiedFlag();          
    	UpdateData(FALSE);  
    	
    	m_btnCancel.EnableWindow(FALSE);
    	m_btnClear.EnableWindow(FALSE);
    	m_btnLookup.EnableWindow(m_pApp->IsOnline());
        
        // empty the undo/redo buffers for all edit controls
        EmptyUndoFlags();   
        
        GetParentFrame()->DestroyWindow();
        
       	// exit this function
    	return;
    }
    return;    	
}

void CFingerView::OnClickedHelp()
{ 
	CWnd::OnHelp();
	return;
}

void CFingerView::OnEditChange()
{
	if (!UpdateData(TRUE))
		return;
		
	if (!m_pDoc->m_strHost.IsEmpty() || !m_pDoc->m_strUser.IsEmpty()
		|| !m_pDoc->m_strOutput.IsEmpty())
	{
		m_btnClear.EnableWindow(TRUE);
	}
	else if (m_pDoc->m_strHost.IsEmpty() && m_pDoc->m_strUser.IsEmpty() 
		&& m_pDoc->m_strOutput.IsEmpty())
	{
		m_btnClear.EnableWindow(FALSE);
	}
	
	m_pDoc->SetModifiedFlag(); // let document know its data is changed
	return;
}

void CFingerView::OnEditUndo()
{	
	// Change the "redo flag" to TRUE so the undone action can be redone
	m_bUndo = TRUE;
	return;
}

void CFingerView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	// if the CEdit::CanUndo() function says we can undo the last editing
	// action, then the Edit Undo command can be enabled
	CEdit* pEdit = (CEdit*)GetFocus(); 
	if (pEdit == &m_ctlHost || pEdit == &m_ctlUser || pEdit == &m_ctlOutput)
		pCmdUI->Enable(pEdit->CanUndo()); 
	else
		pCmdUI->Enable(FALSE);
}

void CFingerView::OnEditRedo()
{
	if (m_bUndo) // something was undone!
	{
		CEdit* pEdit = (CEdit*)GetFocus();
		pEdit->Undo();  
		m_bUndo = FALSE; // can't redo anymore; not until another undo
	}
	return;	// we're done
}

void CFingerView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	CEdit* pEdit = (CEdit*)GetFocus();     
	if (pEdit == &m_ctlHost || pEdit == &m_ctlUser || pEdit == &m_ctlOutput)	
		pCmdUI->Enable((pEdit->CanUndo() && m_bUndo));
	else
		pCmdUI->Enable(FALSE);
	return;	
}

void CFingerView::OnEditCut()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Cut();
	return;
}

void CFingerView::OnEditCopy()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Copy();
	return;
}

void CFingerView::OnEditPaste()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Paste();
	return;
}

void CFingerView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
	return;
}

void CFingerView::OnUpdateLookupCancel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnCancel.IsWindowEnabled());
	return;
}

void CFingerView::OnUpdateLookup(CCmdUI* pCmdUI)
{   
	if (m_pApp->IsOnline()) m_btnLookup.EnableWindow(TRUE);
	pCmdUI->Enable(m_btnLookup.IsWindowEnabled() && m_pApp->IsOnline());
	return;
}

void CFingerView::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnClear.IsWindowEnabled());
	return;	
}

void CFingerView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	// enable this menu item only if we have any data to display
	pCmdUI->Enable((!m_pDoc->m_strOutput.IsEmpty() && 
			!m_pDoc->m_strUser.IsEmpty() && !m_pDoc->m_strHost.IsEmpty()));
	return;
}

void CFingerView::OnScrollLineDown()
{
	// Scroll down a line using the built-in edit box method
	// CEdit::LineScroll.
	m_ctlOutput.LineScroll(1, 0); // scrolls down one line
}

void CFingerView::OnScrollLineUp()
{
	// Scroll up a line using the built-in edit box method
	// CEdit::LineScroll.
	m_ctlOutput.LineScroll(-1, 0); // scrolls up one line
}

void CFingerView::OnScrollPageDown()
{
	// Scroll down a page (ten lines) using the built-in edit box method
	// CEdit::LineScroll.
	m_ctlOutput.LineScroll(10, 0); // scrolls down ten lines (one page)
}

void CFingerView::OnScrollPageUp()
{
	// Scroll up a page (ten lines) using the built-in edit box method
	// CEdit::LineScroll.
	m_ctlOutput.LineScroll(-10, 0); // scrolls up ten lines (one page)
}

void CFingerView::OnUpdateFilePrint(CCmdUI* pCmdUI)
{
	// enable this menu item only if we have any data to print
	pCmdUI->Enable((!m_pDoc->m_strOutput.IsEmpty() && 
			!m_pDoc->m_strUser.IsEmpty() && !m_pDoc->m_strHost.IsEmpty()));
	return;
}

void CFingerView::OnFinishReceive()
{
	if (!m_bSuccess)
		return;
	
	if (m_pApp->IsAutoFingerSave())
		m_pDoc->SaveModified();
	
	if (m_pApp->IsAutoFingerPrint())
		PostMessage(WM_COMMAND, ID_FILE_PRINT);
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFingerWnd

IMPLEMENT_DYNCREATE(CFingerWnd, CMDIChildWnd)

CFingerWnd::CFingerWnd()
{
}

CFingerWnd::~CFingerWnd()
{
}


BEGIN_MESSAGE_MAP(CFingerWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFingerWnd)
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFingerWnd message handlers

void CFingerWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	if (!bActivate)
	{
		CFingerView* pView = (CFingerView*)GetActiveView();
		pView->OnActivateView(FALSE, NULL, pView);
	}
	else
	{
		CFingerView* pView = (CFingerView*)GetActiveView();
		pView->OnActivateView(TRUE, pView, NULL);
	}
	return;
}

BOOL CFingerWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext)
{
	// Set CRect to be used for window size and position   
	CRect windowRect;
	windowRect.left = 100; 
	windowRect.right = 537;
	windowRect.top = 13;
	windowRect.bottom = 379;
	 
	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);
	ASSERT(m_hMenuShared == NULL);      // only do once

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	// parent must be MDI Frame (or NULL for default)
	ASSERT(pParentWnd == NULL || pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	// will be a child of MDIClient
	ASSERT(!(dwDefaultStyle & WS_POPUP));
	dwDefaultStyle |= WS_CHILD;

	// if available - get MDI child menus from doc template
	ASSERT(m_hMenuShared == NULL);      // only do once
	CMultiDocTemplate* pTemplate;
	if (pContext != NULL &&
		(pTemplate = (CMultiDocTemplate*)pContext->m_pNewDocTemplate) != NULL)
	{
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)));
		// get shared menu from doc template
		m_hMenuShared = pTemplate->m_hMenuShared;
		m_hAccelTable = pTemplate->m_hAccelTable;
	}
	else
	{
		TRACE0("Warning: no shared menu/acceltable for MDI Child window\n");
			// if this happens, programmer must load these manually
	}

	CString strFullString = "", strTitle = "";
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(strTitle, strFullString, 0);    // first sub-string

	ASSERT(m_hWnd == NULL);
	if (!Create(GetIconWndClass(dwDefaultStyle, nIDResource),
	  strTitle, dwDefaultStyle, windowRect,
	  (CMDIFrameWnd*)pParentWnd, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	// it worked !
	return TRUE;
}