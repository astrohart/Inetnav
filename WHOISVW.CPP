// whoisvw.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"

#include "whoisdoc.h"
#include "whoisvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhoisView

IMPLEMENT_DYNCREATE(CWhoisView, CFormView) 

BEGIN_MESSAGE_MAP(CWhoisView, CFormView)
	//{{AFX_MSG_MAP(CWhoisView)
	ON_BN_CLICKED(IDC_CANCEL, OnLookupCancel)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_HELP, OnClickedHelp)
	ON_BN_CLICKED(IDC_LOOKUP, OnLookup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateEditClearAll)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_LOOKUP, OnUpdateLookup)
	ON_UPDATE_COMMAND_UI(ID_LOOKUP_CANCEL, OnUpdateLookupCancel)
	ON_COMMAND(ID_SCROLL_LINE_DOWN, OnScrollLineDown)
	ON_COMMAND(ID_SCROLL_LINE_UP, OnScrollLineUp)
	ON_COMMAND(ID_SCROLL_PAGE_DOWN, OnScrollPageDown)
	ON_COMMAND(ID_SCROLL_PAGE_UP, OnScrollPageUp)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_COMMAND(ID_WINDOW_REMEMBER, OnWindowRemember)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, OnClear)
	ON_COMMAND(ID_LOOKUP_CANCEL, OnLookupCancel)
	ON_COMMAND(ID_LOOKUP_NIC, OnLookup)
	ON_EN_CHANGE(IDC_NIC, OnEditChange)
	ON_EN_CHANGE(IDC_OUTPUT, OnEditChange)
	ON_EN_CHANGE(IDC_HOST, OnEditChange)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Socket callback messages  
	ON_MESSAGE(WM_SOCKET_RESPONSE, OnSocketResponse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhoisView construction/destruction

CWhoisView::CWhoisView()
	: CFormView(CWhoisView::IDD)
{
	//{{AFX_DATA_INIT(CWhoisView)
	m_pDoc = NULL;
	//}}AFX_DATA_INIT
	m_pSocket = NULL; 
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_pStatusBar = m_pApp->GetMainFrame()->GetStatusBar();
	m_bUndo = FALSE; // starts out with Undo
	m_pPercentDialog = NULL;
	
	m_bSuccess = FALSE;
}

CWhoisView::~CWhoisView()
{ 
	delete m_pSocket; // cleanup the memory used by the socket  
	m_pSocket = NULL;
	
	delete m_pPercentDialog; // cleanup memory used by the percent dialog
	m_pPercentDialog = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWhoisView data exchange and validation

void CWhoisView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhoisView)
	DDX_Control(pDX, IDC_HOST, m_ctlHost);
	DDX_Control(pDX, IDC_NIC, m_ctlNIC);
	DDX_Control(pDX, IDC_OUTPUT, m_ctlOutput);
	DDX_Control(pDX, IDC_FORM_ICON, m_ctlIcon);
	DDX_Control(pDX, IDC_LOOKUP, m_btnLookup);
	DDX_Control(pDX, IDC_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_CANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_NIC, m_pDoc->m_strNIC);
	DDV_MaxChars(pDX, m_pDoc->m_strNIC, 255);
	DDX_Text(pDX, IDC_HOST, m_pDoc->m_strHost);
	DDV_MaxChars(pDX, m_pDoc->m_strHost, 255);
	DDX_Text(pDX, IDC_OUTPUT, m_pDoc->m_strOutput);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CWhoisView update helper functions

void CWhoisView::OnInitialUpdate()
{
	// Initialize the view's pointer to the document
	m_pDoc = GetDocument();
    
	// call base class initial update
	CFormView::OnInitialUpdate();    

	// subclass this  view with CTL3D so it looks nice
	Ctl3dSubclassDlgEx(m_hWnd, CTL3D_ALL); 
	
	// Disable all buttons and commands except Lookup and Help
	m_btnLookup.EnableWindow(m_pApp->IsOnline());
	m_btnHelp.EnableWindow(TRUE);
	m_btnClear.EnableWindow(FALSE);
	m_btnCancel.EnableWindow(FALSE);
    
    m_pDoc->UpdateAllViews(NULL, 0L, NULL);
    
  	if (!m_pDoc->m_strNIC.IsEmpty() || !m_pDoc->m_strHost.IsEmpty()
		|| !m_pDoc->m_strOutput.IsEmpty())
	{
		m_btnClear.EnableWindow(TRUE);
	}
	else
		m_btnClear.EnableWindow(FALSE);
			
	m_btnLookup.EnableWindow(m_pApp->IsOnline());
	m_btnCancel.EnableWindow(FALSE);

	m_pDoc->SetModifiedFlag(FALSE);
	return;
}

void CWhoisView::OnUpdate(CView*, LPARAM, CObject*)
{
	// display document's data by moving it from data variables to controls
	// with CWnd::UpdateData(bSave=FALSE);
	UpdateData(FALSE);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CWhoisView printing

BOOL CWhoisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->m_nNumPreviewPages = 1;
	return DoPreparePrinting(pInfo);
}

void CWhoisView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{  
	CFormView::OnBeginPrinting(pDC, pInfo);
	return;
}

void CWhoisView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{ 
	// Save data from the controls to the document
	UpdateData(TRUE);
	
	// let's just allow the printer DC to use its default font

    // We want a header that tells on whom the user got information
    CString strHeader = "InterNIC Site Information: ";
    strHeader += m_pDoc->m_strHost + " via "+ m_pDoc->m_strNIC;
    PrintPageHeader(pDC, pInfo, strHeader);
	
	// Now print the information obtained from the Whois query	
	CString strOutputCtlContents = "", strHeading = "";
	
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

void CWhoisView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFormView::OnEndPrinting(pDC, pInfo);
}

void CWhoisView::PrintPageHeader(CDC* pDC, CPrintInfo* pInfo,
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
// CWhoisView user-interface helper functions 

void CWhoisView::EmptyUndoFlags()
{
	// resets undo flags in Host, User, and Output controls
	m_ctlNIC.EmptyUndoBuffer();
	m_ctlHost.EmptyUndoBuffer();
	m_ctlOutput.EmptyUndoBuffer();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CWhoisView diagnostics

#ifdef _DEBUG
void CWhoisView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWhoisView::Dump(CDumpContext& dc) const
{
	dc << "m_pDoc->m_strHost: " << m_pDoc->m_strHost << "\r\n";
	dc << "m_pDoc->m_strNIC: " << m_pDoc->m_strNIC << "\r\n";
	dc << "m_pDoc->m_strOutput:\r\n" << m_pDoc->m_strOutput << "\r\n";
	
	CFormView::Dump(dc);
}

CWhoisDoc* CWhoisView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWhoisDoc)));
	return (CWhoisDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWhoisView socket response message handler(s)

LRESULT CWhoisView::OnSocketResponse(WPARAM amount, LPARAM buffer)
{
	if ((int)amount > 0)
	{
		if (buffer != 0)
		{   
			// tell the user we're receiving information
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
				"Please wait while we receive information from " + m_pDoc->m_strNIC + "...");			                               
            
            for (UINT i = 0; i < (int)amount; i++)
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
            	m_bSuccess = TRUE;

            	m_pPercentDialog->CalculatePercent(i + 1, (int)amount);
            	m_pPercentDialog->UpdateWindow();
           	}    
           	
           	m_pDoc->SetModifiedFlag();
           	m_btnLookup.EnableWindow(m_pApp->IsOnline());
           	m_btnCancel.EnableWindow(FALSE);
			UpdateData(FALSE); // Fills output area on screen 
			m_bSuccess = TRUE;
			
			m_pPercentDialog->SetPercent(100);
			m_pPercentDialog->UpdateWindow();
			m_pPercentDialog->ShowWindow(SW_HIDE);
			m_pPercentDialog->DestroyWindow();
			
			delete m_pPercentDialog;
			m_pPercentDialog = NULL;
		}
		return 0;
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
						m_bSuccess = FALSE;
						m_pDoc->m_strOutput.Empty();
						UpdateData(FALSE);
						// Clears output area on screen 
						m_pStatusBar->SetText(IDS_SENDING_REQUEST);
						/*wait.Restore();*/
						// Send the whois request
						m_pSocket->Send(m_pDoc->m_strHost + "\r\n");
						break;
					
					case DISCONNECTED: 
						
						// Activate user preferences
						OnFinishReceive();
						
						// data is now being processed in the upper half of
						// this function
						break;
					
					case ERRORSTATE:
						m_pStatusBar->SetText("Failed to get information because of a network error.  Try again later.");
						AfxMessageBox(IDP_FAILED_GET_INFO,
							MB_ICONSTOP|MB_OK);
						// re-enable Lookup button, disable Cancel button
						m_btnLookup.EnableWindow(m_pApp->IsOnline());
						m_btnCancel.EnableWindow(FALSE); 
						m_pStatusBar->ShowIdleMessage(); 
						GetParentFrame()->DestroyWindow();
						// all other status states represent an error
						/*wait.Restore();*/
						break;
					
					case TIMEDOUT:
						m_pStatusBar->SetText("Network call timed out.");
						AfxMessageBox("The timeout period for a network call has expired.  Try again.");
						m_btnLookup.EnableWindow(m_pApp->IsOnline());
						m_btnCancel.EnableWindow(FALSE);
						m_pStatusBar->ShowIdleMessage();  
						GetParentFrame()->DestroyWindow();
						
					default:
						m_pStatusBar->SetText("Failed to get information because of a network error.  Try again later.");
						AfxMessageBox(IDP_FAILED_GET_INFO,
							MB_ICONSTOP|MB_OK);
						// re-enable Lookup button, disable Cancel button
						m_btnLookup.EnableWindow(m_pApp->IsOnline());
						m_btnCancel.EnableWindow(FALSE); 
						m_pStatusBar->ShowIdleMessage();  
						GetParentFrame()->DestroyWindow();
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
// CWhoisView message handlers

void CWhoisView::OnLookup()
{  
	if (!m_pApp->IsOnline())
		return;

	m_btnLookup.EnableWindow(FALSE);
	m_btnCancel.EnableWindow(TRUE);
	/*wait.Restore();*/
	
	m_pStatusBar->SetText(IDS_RESOLVING_HOSTNAME);
	/*wait.Restore();*/
	
	if (!UpdateData(TRUE))
	{
		m_btnLookup.EnableWindow(m_pApp->IsOnline());
		m_btnCancel.EnableWindow(FALSE);
		
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP|MB_OK);
		m_pStatusBar->ShowIdleMessage();
		return;
	} 
	if (m_pDoc->m_strNIC.IsEmpty() || m_pDoc->m_strHost.IsEmpty())
	{
		MessageBeep(-1);
		AfxMessageBox(IDP_INFO_NEEDED);
		m_pStatusBar->ShowIdleMessage();
		return;
	}
	if (!m_pDoc->m_strNIC.IsEmpty() || !m_pDoc->m_strHost.IsEmpty())
	{
		if (m_pSocket != 0)
			delete m_pSocket;
		m_pSocket = new QSocket(TRUE);
		m_pSocket->SetReceiveTarget(this, WM_SOCKET_RESPONSE);
		// tell user we're now attempting to connect
		CString strMsg = "Attempting to connect to the InterNIC...  Please wait.";
		m_pStatusBar->SetText(strMsg);
		/*wait.Restore();*/
		if (!m_pSocket->Connect(m_pDoc->m_strNIC, 43))
		{  
			/*wait.Restore();*/  
			m_pStatusBar->SetText("Attempt to connect failed.");
			AfxMessageBox(IDP_NOCONNECT_NIC, MB_ICONASTERISK|MB_OK);
			delete m_pSocket;
			m_pSocket = NULL;
			m_btnLookup.EnableWindow(m_pApp->IsOnline());
			m_btnCancel.EnableWindow(FALSE);
			m_pStatusBar->ShowIdleMessage();
			return;
		}
	}
	else
	{
		AfxMessageBox(IDP_INFO_NEEDED); 
		m_pStatusBar->ShowIdleMessage();
		return;
	}	
}

void CWhoisView::OnClear()
{ 
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();
	CMultiDocTemplate* pWhois = pApp->GetWhoisType();
	
	// make sure the document is saved, if the user wants to
	/*wait.Restore();*/
	m_pDoc->m_strNIC.Empty();
	m_pDoc->m_strHost.Empty();
	m_pDoc->m_strOutput.Empty();
	m_pDoc->SetModifiedFlag();
	m_pDoc->UpdateAllViews(NULL, 0L, NULL);      
	
	m_btnClear.EnableWindow(FALSE);
	
	return;
}

void CWhoisView::OnLookupCancel()
{
	m_bSuccess = FALSE;
	
    // cancel lookup operation
    if (m_pSocket) // if m_pSocket != NULL then we're currently using it
    {  
    	MessageBeep(-1);                                         
    	m_pStatusBar->SetText("Lookup cancelled.");
    	AfxMessageBox("Lookup cancelled.", MB_ICONASTERISK|MB_OK);
    	
    	// get rid of the socket
    	delete m_pSocket;
    	m_pSocket = NULL;
    	
    	// clear settings
    	m_pDoc->m_strNIC.Empty();
    	m_pDoc->m_strHost.Empty();
    	m_pDoc->m_strOutput.Empty();           
    	UpdateData(FALSE);  
    	
    	// enable the Lookup button and disable the Cancel and Clear buttons
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

void CWhoisView::OnFinishReceive()
{
	if (!m_bSuccess)
		return;
	
	if (m_pApp->IsAutoWhoisSave())
		m_pDoc->SaveModified();
	
	if (m_pApp->IsAutoWhoisPrint())
		PostMessage(WM_COMMAND, ID_FILE_PRINT);
	
	return;
}

void CWhoisView::OnClickedHelp()
{
	CWnd::OnHelp();
	return;
}

void CWhoisView::OnEditChange()
{	 
	if (!UpdateData(TRUE))
		return;
		
	if (!m_pDoc->m_strNIC.IsEmpty() || !m_pDoc->m_strHost.IsEmpty()
		|| !m_pDoc->m_strOutput.IsEmpty())
	{
		m_btnClear.EnableWindow(TRUE);
	}
	else if (m_pDoc->m_strNIC.IsEmpty() && m_pDoc->m_strHost.IsEmpty() 
		&& m_pDoc->m_strOutput.IsEmpty())
	{
		m_btnClear.EnableWindow(FALSE);
	}
	
	m_pDoc->SetModifiedFlag();	
	return;
}

void CWhoisView::OnEditUndo()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Undo();     
	// Change the "redo flag": if TRUE, then we can redo the editing action
	// which was just undone. if FALSE, this OnEditUndo() command WAS a Redo()
	// command
	m_bUndo = TRUE;
	return;
}

void CWhoisView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	// if the CEdit::CanUndo() function says we can undo the last editing
	// action, then the Edit Undo command can be enabled
	CEdit* pEdit = (CEdit*)GetFocus(); 
	if (pEdit == &m_ctlNIC || pEdit == &m_ctlHost || pEdit == &m_ctlOutput)
		pCmdUI->Enable(pEdit->CanUndo()); 
	else
		pCmdUI->Enable(FALSE);
}

void CWhoisView::OnEditRedo()
{
	if (m_bUndo) // something was undone!
	{
		CEdit* pEdit = (CEdit*)GetFocus();
		pEdit->Undo();  
		m_bUndo = FALSE; // can't redo anymore; not until another undo
	}
	return;	// we're done
}

void CWhoisView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	CEdit* pEdit = (CEdit*)GetFocus();     
	if (pEdit == &m_ctlNIC || pEdit == &m_ctlHost || pEdit == &m_ctlOutput)	
		pCmdUI->Enable((pEdit->CanUndo() && m_bUndo));
	else
		pCmdUI->Enable(FALSE);
	return;	
}

void CWhoisView::OnEditCut()
{ 
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Cut();
	return;
}

void CWhoisView::OnEditCopy()
{ 
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Copy();
	return;
}

void CWhoisView::OnEditPaste()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	pEdit->Paste();
	return;
}

void CWhoisView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
	return;
}

void CWhoisView::OnUpdateLookupCancel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnCancel.IsWindowEnabled());
	return;
}

void CWhoisView::OnUpdateLookup(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnLookup.IsWindowEnabled());
	return;	
}

void CWhoisView::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_btnClear.IsWindowEnabled());
	return;
}

void CWhoisView::OnScrollLineDown()
{   
	// Scroll a line down using the built-in edit box method
	// CEdit::LineScroll.
	m_ctlOutput.LineScroll(1, 0); // scrolls down one line
}

void CWhoisView::OnScrollLineUp()
{
    // Scroll a line up using the built-in edit box method
	// CEdit::LineScroll.
	m_ctlOutput.LineScroll(-1, 0); // scrolls up one line
}

void CWhoisView::OnScrollPageDown()
{
	// Scroll down a page, which is ten lines, with the built-in edit box
	// method CEdit::LineScroll.
	m_ctlOutput.LineScroll(10, 0); // scrolls down ten lines (a page)
}

void CWhoisView::OnScrollPageUp()
{
	// Scroll up a page, which is ten lines, with the built in edit box
	// method CEdit::LineScroll.
	m_ctlOutput.LineScroll(-10, 0); // scrolls up ten lines (a page)
}

void CWhoisView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	// enable this menu item only if we have any data to print
	pCmdUI->Enable((!m_pDoc->m_strOutput.IsEmpty() && 
			!m_pDoc->m_strHost.IsEmpty() && !m_pDoc->m_strNIC.IsEmpty()));
	return;
}

void CWhoisView::OnUpdateFilePrint(CCmdUI* pCmdUI)
{
	// enable this menu item only if we have any data to print
	pCmdUI->Enable((!m_pDoc->m_strOutput.IsEmpty() && 
			!m_pDoc->m_strHost.IsEmpty() && !m_pDoc->m_strNIC.IsEmpty()));
	return;
}

void CWhoisView::OnWindowRemember()
{
	// write out this window's position and dimensions to INETNAV.INI,
	// then open the file.
	
	CInternetNavApp* pApp = (CInternetNavApp*)AfxGetApp();

	CRect rect;
	CString strWhoisSect = "Whois";
	
	GetParentFrame()->GetWindowRect(&rect);
	
	pApp->WriteProfileInt(strWhoisSect, "WhoisLeft", rect.left);
	pApp->WriteProfileInt(strWhoisSect, "WhoisRight", rect.right);
	pApp->WriteProfileInt(strWhoisSect, "WhoisTop", rect.top);
	pApp->WriteProfileInt(strWhoisSect, "WhoisBottom", rect.bottom);
	pApp->WriteProfileInt(strWhoisSect, "WhoisCX", rect.Width());
	pApp->WriteProfileInt(strWhoisSect, "WhoisCY", rect.Height());	
}
