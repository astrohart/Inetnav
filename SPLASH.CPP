// splash.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"
#include "splash.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd dialog


CSplashWnd::CSplashWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplashWnd)
	m_pApp = (CInternetNavApp*)AfxGetApp();
	m_strUserCompany = "";
	m_strUserName = "";
	//}}AFX_DATA_INIT 
	
	m_pParentWnd = CWnd::GetDesktopWindow();
}

void CSplashWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashWnd)
	DDX_Text(pDX, IDC_USER_COMPANY, m_strUserCompany);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSplashWnd, CDialog)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplashWnd message handlers

BOOL CSplashWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_icon.SubclassDlgItem(IDC_BIGICON, this);
	m_icon.SizeToContent();
	
	// Fill in user name and company and display the information
	m_strUserName = m_pApp->GetUserName();
	m_strUserCompany = m_pApp->GetUserCompany();
	UpdateData(FALSE);
	
	// Set a timer for how long this window is visible (2.5 seconds)
	SetTimer(4, 2500, NULL); // amount of time this window should remain
								// visible -- 2.5 seconds

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	if (this != NULL)
	{
		KillTimer(nIDEvent);
		ShowWindow(SW_HIDE);
		UpdateWindow();
		DestroyWindow();
		UpdateWindow();
		m_pApp->m_bContinueCursor = FALSE;
		m_pApp->EndWaitCursor();
        
        if (m_pApp->m_lpCmdLine[0] != '\0') 
        {
        	// If the user has launched an associated file or put a file on
        	// the command line or whatever, DON'T show the Tip of the Day
        	// and DON'T connect to the Internet automatically.  This is
        	// annoying and gets in the user's way
        	return;     
        }
        		
		// Show the user the Tip Of The Day, if they said they want to see it
		// in Preferences
		if (m_pApp->IsShowTOD() && m_pApp->GetShowTips() == 0 && 
			!m_pApp->m_bPseudoShowTOD)
		{
			m_pApp->OnTipOfDay();		
		}
			
		if (m_pApp->StartOnline() && !m_pApp->IsPseudoOnline())
			m_pApp->SignOn();
		
		return;
	}
	else
		return;
}     

/////////////////////////////////////////////////////////////////////////////
// CBigIcon

BEGIN_MESSAGE_MAP(CBigIcon, CButton)
	//{{AFX_MSG_MAP(CBigIcon)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBigIcon message handlers

#define CY_SHADOW   4
#define CX_SHADOW   4

void CBigIcon::SizeToContent()
{
	// get system icon size
	int cxIcon = ::GetSystemMetrics(SM_CXICON);
	int cyIcon = ::GetSystemMetrics(SM_CYICON);

	// a big icon should be twice the size of an icon + shadows
	SetWindowPos(NULL, 0, 0, cxIcon*2 + CX_SHADOW + 4, cyIcon*2 + CY_SHADOW + 4,
		SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
}

void CBigIcon::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	ASSERT(pDC != NULL);

	CRect rect;
	GetClientRect(rect);
	int cxClient = rect.Width();
	int cyClient = rect.Height();

	// load icon
	HICON hicon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (hicon == NULL)
		return;

	// draw icon into off-screen bitmap
	int cxIcon = ::GetSystemMetrics(SM_CXICON);
	int cyIcon = ::GetSystemMetrics(SM_CYICON);

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(pDC, cxIcon, cyIcon))
		return;
	CDC dcMem;
	if (!dcMem.CreateCompatibleDC(pDC))
		return;
	CBitmap* pBitmapOld = dcMem.SelectObject(&bitmap);
	if (pBitmapOld == NULL)
		return;

	// blt the bits already on the window onto the off-screen bitmap
	dcMem.StretchBlt(0, 0, cxIcon, cyIcon, pDC,
		2, 2, cxClient-CX_SHADOW-4, cyClient-CY_SHADOW-4, SRCCOPY);

	// draw the icon on the background
	dcMem.DrawIcon(0, 0, hicon);

	// draw border around icon
	CPen pen;
	pen.CreateStockObject(BLACK_PEN);
	CPen* pPenOld = pDC->SelectObject(&pen);
	pDC->Rectangle(0, 0, cxClient-CX_SHADOW, cyClient-CY_SHADOW);
	if (pPenOld)
		pDC->SelectObject(pPenOld);

	// draw shadows around icon
	CBrush darkGrayBrush;
	darkGrayBrush.CreateStockObject(DKGRAY_BRUSH);
	rect.SetRect(cxClient-CX_SHADOW, CY_SHADOW, cxClient, cyClient);
	pDC->FillRect(rect, &darkGrayBrush);
	rect.SetRect(CX_SHADOW, cyClient-CY_SHADOW, cxClient, cyClient);
	pDC->FillRect(rect, &darkGrayBrush);

	// draw the icon contents
	pDC->StretchBlt(2, 2, cxClient-CX_SHADOW-4, cyClient-CY_SHADOW-4,
		&dcMem, 0, 0, cxIcon, cyIcon, SRCCOPY);
}

BOOL CBigIcon::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;    // we don't do any erasing...
}

/////////////////////////////////////////////////////////////////////////////



