// percent.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"
#include "percent.h" 

// for CFTPercentDialog's CFtpView pointer
#include "ftp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPercentDialog dialog

CPercentDialog::CPercentDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPercentDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPercentDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT      
	m_bULONG = FALSE;
}

void CPercentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPercentDialog)
	//}}AFX_DATA_MAP
}

void CPercentDialog::Initialize (CString Title, CString Text, COLORREF Color)
{
	// Initialize member variables.
	mTitle = Title;
	mText = Text;
	mColor = Color;
	mCurrentPercent = 0;
	m_ulCurrentPercent = 0;
	m_bULONG = FALSE;
	
	// Create the dialog.
	CDialog::Create (CPercentDialog::IDD);	
}

void CPercentDialog::SetPercent (int nPercent)
{
	// Set the current percent value.
	mCurrentPercent = nPercent;
	
	// the percent is not an unsigned long int, so set m_bULONG to FALSE
	m_bULONG = FALSE;

	// Invalidate the percent bar to force update with new value.
	InvalidateRect(mBarRect, FALSE);
	UpdateWindow();
}

void CPercentDialog::SetULPercent(unsigned long int ulPercent)
{
	// Set the current percent value
	m_ulCurrentPercent = ulPercent; 
	
	m_bULONG = TRUE;
	
	// Invalidate the percent bar to force update with new value
	InvalidateRect(mBarRect, FALSE);
	UpdateWindow();
}

void CPercentDialog::CalculatePercent(int nAmount, int nTotal)
{
	// To calculate percent, multiply nAmount by 100, then divide
	// the product by nTotal.
	int nPercent = ::MulDiv(nAmount, 100, nTotal);
	
	// If the value of nPercent is greater than 100, then set the
	// value of nPercent to be equal to 100.
	if (nPercent > 100)
		nPercent = 100;
	
	SetPercent(nPercent);
	return;	
}

void CPercentDialog::CalculateULPercent(unsigned long int ulAmount,
	unsigned long int ulTotal)
{                             
	// To calculate percent, multiply ulAmount by 100, then divide
	// the product by ulTotal
	
	unsigned long int ulDividend = ulAmount * 100;
	unsigned long int ulPercent	= ulDividend / ulTotal;

	// Now ulPercent contains the percentage value
	// The value of ulPercent can't be > 100, so if it is greater, then
	// we need to set ulPercent equal to 100.
	if (ulPercent > 100)
		ulPercent = 100;
	
	SetULPercent(ulPercent);
	return;
}

BEGIN_MESSAGE_MAP(CPercentDialog, CDialog)
	//{{AFX_MSG_MAP(CPercentDialog)
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPercentDialog message handlers

void CPercentDialog::OnPaint()
{
	CPaintDC PaintDC(this); // device context for painting
	CDC MemDC;
	CBitmap Bitmap, *pOldBitmap;
	CPen GrayPen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	CPen WhitePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
	CBrush GrayBrush(::GetSysColor(COLOR_BTNFACE));
	int Width;
	unsigned long int ulWidth;
	CRect Rect;
	char Junk[10];
	TEXTMETRIC TextMetric;

	// Create a device context to work on.
	MemDC.CreateCompatibleDC(&PaintDC);

	// Create the brush to draw the bar and set the text color
	CBrush Brush(mColor);
	MemDC.SetTextColor(RGB(255,255,255));

	// Draw the inset rectangle for the bar.
	CPen* pOldPen = PaintDC.SelectObject(&GrayPen);
	PaintDC.MoveTo(BARX, BARY);
	PaintDC.LineTo(BARX + BARWIDTH, BARY);
	PaintDC.MoveTo(BARX, BARY);
	PaintDC.LineTo(BARX, BARY + BARHEIGHT);
	PaintDC.SelectObject(pOldPen);
	pOldPen = PaintDC.SelectObject(&WhitePen);
	PaintDC.MoveTo(BARX + BARWIDTH, BARY + BARHEIGHT);
	PaintDC.LineTo(BARX + BARWIDTH, BARY);
	PaintDC.MoveTo(BARX + BARWIDTH, BARY + BARHEIGHT);
	PaintDC.LineTo(BARX, BARY + BARHEIGHT);
	PaintDC.SelectObject(pOldPen);

	// Create the bitmap to draw the bar on.
	Bitmap.CreateCompatibleBitmap(&PaintDC, BARWIDTH - 1, BARHEIGHT - 1);
	pOldBitmap = (CBitmap *)MemDC.SelectObject(&Bitmap);

	// Draw the background for the bar.
	Rect.SetRect(0, 0, BARWIDTH - 1, BARHEIGHT - 1);
	MemDC.FillRect(Rect, &GrayBrush);

	// Draw the percentange bar.    
	if (!m_bULONG)
	{
		Width = mCurrentPercent * (BARWIDTH - 1) / 100;
		Rect.SetRect(0, 0, Width, BARHEIGHT - 1);
		MemDC.FillRect(Rect, &Brush);
	}
	else
	{
		ulWidth =  m_ulCurrentPercent * (BARWIDTH - 1) / 100;
		Rect.SetRect(0,0, (unsigned long int)(int)ulWidth, BARHEIGHT - 1);
		MemDC.FillRect(Rect, &Brush);
	}

	// Display the percentage as text.
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.GetTextMetrics(&TextMetric);
	if (!m_bULONG)
	{
		wsprintf (Junk, "%d%%", mCurrentPercent);
	}
	else
	{
		wsprintf(Junk, "%lu%%", m_ulCurrentPercent);
	}
	MemDC.TextOut(BARWIDTH / 2 - (TextMetric.tmAveCharWidth * lstrlen (Junk) / 2), BARHEIGHT / 2 - (TextMetric.tmHeight / 2), (LPCSTR) Junk, lstrlen (Junk));

	// Blt the bar to the dialog.
	PaintDC.BitBlt(BARX + 1, BARY + 1, BARWIDTH - 1, BARHEIGHT - 1, &MemDC, 0, 0, SRCCOPY);

	// Clean up.
	MemDC.SelectObject(pOldBitmap);
	Bitmap.DeleteObject();
	Brush.DeleteObject();
	GrayBrush.DeleteObject();
	GrayPen.DeleteObject();
}

void CPercentDialog::OnCancel()
{
	ShowWindow(SW_HIDE);
	UpdateWindow();
	
	DestroyWindow();
	
	return;
}

void CPercentDialog::OnOK()
{
	ShowWindow(SW_HIDE);
	UpdateWindow();
	
	DestroyWindow();
	
	return;
}

BOOL CPercentDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CWnd *pWnd, *pBarWnd;
	CRect Rect;

	// Set the window caption text.
	SetWindowText(mTitle);
      
	// Set the dialog text.
	pWnd = GetDlgItem(IDC_PERCENTTEXT);
	pWnd->SetWindowText(mText);

	// Get the coordinates for the percentage bar.
	pBarWnd = GetDlgItem (IDC_BAR);
	pBarWnd->GetWindowRect (Rect);
	ScreenToClient (Rect);
	BARX = Rect.left;
	BARY = Rect.top;
	BARWIDTH = Rect.Width ();
	BARHEIGHT = Rect.Height ();
	mBarRect.SetRect (BARX + 1, BARY + 1, BARX + BARWIDTH, BARY + BARHEIGHT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPercentDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	// This function deals with commands chosen from the Control menu
	// If the user chooses the Close command, we need to know about it
	// so we can close this window properly
	
	switch (nID & 0xFFF0)
	{
	    case SC_CLOSE:
			// Destroy this window
			ShowWindow(SW_HIDE);
			UpdateWindow();
			
			DestroyWindow();
			break;
		
		case SC_SIZE:
		case SC_MOVE:
		case SC_MINIMIZE:
		case SC_MAXIMIZE:
		case SC_NEXTWINDOW:
		case SC_PREVWINDOW:
		case SC_RESTORE:
		case SC_TASKLIST:
			CWnd::OnSysCommand(nID, lParam);
			break;
			
		default:
			CWnd::OnSysCommand(nID, lParam);
			break;
	}
}


void CPercentDialog::PostNcDestroy()
{
	ASSERT(m_hWnd == NULL);
	delete this;	// remove this object from memory
}

/////////////////////////////////////////////////////////////////////////////
// CFTPercentDialog dialog


CFTPercentDialog::CFTPercentDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFTPercentDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFTPercentDialog)
	//}}AFX_DATA_INIT
}

void CFTPercentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFTPercentDialog)
	DDX_Control(pDX, IDC_FILE_BYTES, m_ctlBytes);
	DDX_Text(pDX, IDC_FILE_PATH, m_strFilePath);
	DDX_Text(pDX, IDC_WAIT_TEXT, m_strWaitText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFTPercentDialog, CDialog)
	//{{AFX_MSG_MAP(CFTPercentDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_CANCEL_CALL, OnCancelCall)
	ON_WM_MOVE()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPercentDialog helper functions

void CFTPercentDialog::Initialize(CString strFilePath, CString strFileName,
									CFtpView* pView, BOOL bUpload)
{
	// Initialize member variables. 
	CString text, str;
	
	str = strFileName;
	strFileName.MakeUpper();
	
	if (!bUpload)
	{
		AfxFormatString1(text, IDS_DL_WAIT_TEXT, strFileName);
	}
	else
	{
		AfxFormatString1(text, IDS_UL_WAIT_TEXT, strFileName);
	}
    
    strFileName = str;
    
	m_strWaitText = text;
	
	m_strFilePath = strFilePath;
	m_pView = pView;
	
	// Sets percent to 0.
	m_ulCurrentPercent = 0;	

	// Create the dialog.  

	CDialog::Create(CFTPercentDialog::IDD);
}	
		
void CFTPercentDialog::SetPercent(unsigned long int ulPercent)
{
	// Set the current percent value
	m_ulCurrentPercent = ulPercent; 

	// Invalidate the percent bar to force update with new value
	InvalidateRect(mBarRect, FALSE);
	UpdateWindow();
}

void CFTPercentDialog::CalculatePercent(unsigned long int ulBytes,
	unsigned long int ulFileSize)
{                             
	// To calculate percent, multiply ulBytes by 100, then divide
	// the product by ulFileSize
	
	unsigned long int ulDividend = 0, ulPercent = 0;
	
	if (ulFileSize != 200 && ulBytes > 0)
	{
		ulDividend = ulBytes * 100;
		ulPercent = ulDividend / ulFileSize;
	}
    
    if (ulBytes == 0)
    {
    	// We've started the download/upload
    	char szFormat[512] = "%lu of %lu bytes transferred.";
    	char szInfo[512];
    	
    	sprintf(szInfo, szFormat, ulBytes, ulFileSize);
    	
    	m_ctlBytes.SetWindowText(szInfo);
    	
    	SetPercent(0);
    	return;
    }
    	
    // If the file size is 200, that means Internet Navigator was unable
    // to determine the file's size on the server or on the local machine.
    if (ulFileSize != 200)
    {
    	char szFormat[512] = "%lu of %lu bytes transferred.";
    	char szInfo[512];
    
    	sprintf(szInfo, szFormat, ulBytes, ulFileSize);
    
	    TRACE2("%lu of %lu bytes transferred.\r\n", ulBytes, ulFileSize);
    
    	m_ctlBytes.SetWindowText(szInfo);
    	
    	if (ulPercent > 100)
    		ulPercent = 100;
    	SetPercent(ulPercent);
  	}
  	else
  	{    
  		// If the file size given is 200, we assume that the
  		// file size is in error, and therefore inaccurate.  In this
  		// case we just will show the user the number of bytes transferred.
  		char szFormat[512] = "%lu bytes transferred.";
		char szInfo[512];
		
		sprintf(szInfo, szFormat, ulBytes);
		
		TRACE1("%lu bytes transferred.\r\n", ulBytes);
		
		m_ctlBytes.SetWindowText(szInfo);
		
		// Since we don't know when 100% of the file has been downloaded,
		// always set the percent to 0; the purpose of this dialog box then
		// is to just show the user the path of the file and how many bytes
		// have been transferred so far.
		ulPercent = 0;
		SetPercent(0);
	}
	return;
}   

/////////////////////////////////////////////////////////////////////////////
// CFTPercentDialog message handlers

BOOL CFTPercentDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CWnd *pWnd, *pBarWnd;
	CRect Rect;

	// Set the window caption text.
	SetWindowText("File Transfer Progress");
	
	// Get the coordinates for the percentage bar.
	pBarWnd = GetDlgItem (IDC_BAR);
	pBarWnd->GetWindowRect (Rect);
	ScreenToClient (Rect);
	BARX = Rect.left;
	BARY = Rect.top;
	BARWIDTH = Rect.Width ();
	BARHEIGHT = Rect.Height ();
	mBarRect.SetRect (BARX + 1, BARY + 1, BARX + BARWIDTH, BARY + BARHEIGHT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFTPercentDialog::OnPaint()
{
	CPaintDC PaintDC(this); // device context for painting
	CDC MemDC;
	CBitmap Bitmap, *pOldBitmap;
	CPen GrayPen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	CBrush GrayBrush(::GetSysColor(COLOR_BTNFACE));  
	CPen WhitePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
	int Width;
	unsigned long int ulWidth;
	CRect Rect;
	char Junk[10];
	TEXTMETRIC TextMetric;

	// Create a device context to work on.
	MemDC.CreateCompatibleDC(&PaintDC);

	// Create the brush to draw the bar and set the text color
	CBrush Brush(RGB(0,0,128));
	MemDC.SetTextColor(RGB(255,255,255));

	// Draw the inset rectangle for the bar.
	CPen* pOldPen = PaintDC.SelectObject(&GrayPen);
	PaintDC.MoveTo(BARX, BARY);
	PaintDC.LineTo(BARX + BARWIDTH, BARY);
	PaintDC.MoveTo(BARX, BARY);
	PaintDC.LineTo(BARX, BARY + BARHEIGHT);
	PaintDC.SelectObject(pOldPen);
	pOldPen = PaintDC.SelectObject(&WhitePen);
	PaintDC.MoveTo(BARX + BARWIDTH, BARY + BARHEIGHT);
	PaintDC.LineTo(BARX + BARWIDTH, BARY);
	PaintDC.MoveTo(BARX + BARWIDTH, BARY + BARHEIGHT);
	PaintDC.LineTo(BARX, BARY + BARHEIGHT);
	PaintDC.SelectObject(pOldPen);

	// Create the bitmap to draw the bar on.
	Bitmap.CreateCompatibleBitmap(&PaintDC, BARWIDTH - 1, BARHEIGHT - 1);
	pOldBitmap =(CBitmap *) MemDC.SelectObject(&Bitmap);

	// Draw the background for the bar.
	Rect.SetRect(0, 0, BARWIDTH - 1, BARHEIGHT - 1);
	MemDC.FillRect(Rect, &GrayBrush);

	// Draw the percentange bar.
	ulWidth =  m_ulCurrentPercent * (BARWIDTH - 1) / 100;
	Rect.SetRect(0,0, (unsigned long int)(int)ulWidth, BARHEIGHT - 1);
	MemDC.FillRect(Rect, &Brush);

	// Display the percentage as text.
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.GetTextMetrics(&TextMetric);
	wsprintf(Junk, "%lu%%", m_ulCurrentPercent);
	MemDC.TextOut(BARWIDTH / 2 - (TextMetric.tmAveCharWidth * lstrlen (Junk) / 2), BARHEIGHT / 2 - (TextMetric.tmHeight / 2), (LPCSTR) Junk, lstrlen (Junk));

	// Blt the bar to the dialog.
	PaintDC.BitBlt(BARX + 1, BARY + 1, BARWIDTH - 1, BARHEIGHT - 1, &MemDC, 0, 0, SRCCOPY);

	// Clean up.
	MemDC.SelectObject(pOldBitmap);
	Bitmap.DeleteObject();
	Brush.DeleteObject();
	GrayBrush.DeleteObject();
	GrayPen.DeleteObject();  
	WhitePen.DeleteObject();
}

void CFTPercentDialog::OnCancelCall()
{
	// Send the WM_COMMAND message, wParam == ID_CANCEL_CALL, to the
	// CFtpView via our m_pView pointer
	
	ShowWindow(SW_HIDE);
	UpdateWindow();
	
	if (m_pView)
	{
		m_pView->GetParentFrame()->UpdateWindow();

		m_pView->PostMessage(WM_COMMAND, ID_CANCEL_CALL, 0L);
		return;
	}

	DestroyWindow();
	return;
}

void CFTPercentDialog::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	return;
}

void CFTPercentDialog::PostNcDestroy()
{
	ASSERT(m_hWnd == NULL);
	delete this;		// remove this object from memory
}

void CFTPercentDialog::OnCancel()
{
	// Close this window properly
	ShowWindow(SW_HIDE);
	UpdateWindow();
	
	DestroyWindow();
	
	return;
}

void CFTPercentDialog::OnOK()
{  
	// Close this window properly
	ShowWindow(SW_HIDE);
	UpdateWindow();
	
	DestroyWindow();
	
	return;
}

void CFTPercentDialog::OnSysCommand(UINT nID, LPARAM lParam)
{   
	// This function deals with commands chosen from the Control menu
	// If the user chooses the Close command, we need to know about it
	// so we can close this window properly
	
	switch (nID & 0xFFF0)
	{
	    case SC_CLOSE:
			// Destroy this window
			ShowWindow(SW_HIDE);
			UpdateWindow();
			
			DestroyWindow();
			break;
		
		case SC_SIZE:
		case SC_MOVE:
		case SC_MINIMIZE:
		case SC_MAXIMIZE:
		case SC_NEXTWINDOW:
		case SC_PREVWINDOW:
		case SC_RESTORE:
		case SC_TASKLIST:
			CWnd::OnSysCommand(nID, lParam);
			break;
			
		default:
			CWnd::OnSysCommand(nID, lParam);
			break;
	}
}
