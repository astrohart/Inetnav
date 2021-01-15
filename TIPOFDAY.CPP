// tipofday.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"
#include "tipofday.h"  

// an array of tip strings.  note that since this text is going into an edit control,
// a carriage return/line feed combination ("\r\n") is required to force a new line.
// a line feed alone is not enough.                        
const char* const apszTips[] =
{
"Welcome to Internet Navigator for Windows!  This window will show you a "
"different tip each time you start Internet Navigator."
"\r\n\r\nIf you don't want to see this window at startup anymore, clear the "
"'Show Tips at Startup' checkbox below,"
" then press Enter.",

"To get information on someone from their e-mail address, you can use the"
" Internet Users Information area of Internet Navigator.",

"To use the Internet Users Information area, just select the Internet "
"Users Information command from the Places menu.",

"You can save information retrieved with the Internet Users Information area "
"in files.  After the information has been retrieved by the area, just press "
"the F12 key.  Select the filename you want to use, then press Enter.",

"You can print out information retrieved with the Internet Users Information area."
"  After the information has been retrieved by the area, choose the Print command "
"from the File menu, or you can press and hold down the Control key while pressing P.",

"If you want information on a site (i.e. aol.com), you can get such information from the InterNIC "
"via the InterNIC Site Information area.",

"The InterNIC maintains a database of all the sites of the Internet.\r\n"
"\r\nTo use the InterNIC Site Information area, just select the InterNIC "
"Site Information command from the Places menu.",

"You can save information retrieved with the InterNIC Site Information area "
"in files.  After the information has been retrieved by the area, just press "
"the F12 key.  Select the filename you want to use, then press Enter.",

"You can print out information retrieved with the InterNIC Site Information area."
"  After the information has been retrieved by the area, choose the Print command "
"from the File menu, or you can press and hold down the Control key while pressing P.",

"To find out about how to search the InterNIC databases for information on sites, "
"press the Help button in the InterNIC Site Information window.",

"You can find information on using windows and areas by pressing F1 on the "
"keyboard while they are open.",

"You can use Internet Navigator with any Internet Service Provider which "
"provides TCP/IP services.\r\n\r\nYou can also use Internet Navigator over "
"a LAN which has TCP/IP protocols installed.",

"For Help on how to use Internet Navigator, press F1 at any time.",

"You can get Help on how to use any menu command.  To do so, select the menu"
" command and press F1.",                                        

"The Help & Information area of Internet Navigator is the central part of "
"Internet Navigator's Help system.  There, you can search the Help files, "
"go to the opening screen of Help, and more!",

"To use the Help & Information area of Internet Navigator, choose the Help &"
" Information command from the Places menu.  The Help & Information area is "
"available whether or not you're connected to the Internet.",

"The commands on the Places menu will take you to areas of Internet Navigator."
"  These areas are for Internet services such as:\r\n\r\n\t- FTP\r\n\t- Gopher\r\n\t-"
" UseNet Newsgroups\r\n\t- Electronic Mail\r\n\t- Online, Real-Time Chat\r\n\t  ...and more!",

"When you need help with an error message, press F1 while the message is visible."
"  The Internet Navigator Help file will display more information on what you can "
"do to address the error, and will tell you how to get help from WnDBSoft.",
  
"You can use the whole Internet with Internet Navigator.  This includes:\r\n\r\n"
"\t- FTP\r\n\t- Gopher\r\n\t- UseNet Newsgroups\r\n\t- Electronic Mail\r\n\t- "
"Online Chat\r\n\t  ...and more!",
       
"Internet Navigator is alot like the America Online software.  You can do many "
"separate things with the Internet at the same time in separate windows and areas.",

"Before you can use the Internet with Internet Navigator, you need to choose the "
"Connect command from the Places menu.  This allows Internet Navigator to access "
"information from the Internet.",

"You can disconnect from your Internet Service Provider and still work in Internet Navigator's "
"offline area.  You can view and edit files, compose and send mail, and get help all in the Offline area."
"  To enter the Offline area from the Internet, choose the Disconnect command from the Places menu."
"  You can then disconnect from your Internet Service Provider.",

"The FTP Browser of Internet Navigator lets you search the Net for cool "
"software to download, plus places where you can upload your files.\r\n\r\n"
"To use the FTP Browser, just select the Download Files command from the Places "
"menu.",

"When you want to download a file with the FTP Browser, just double-click the file you want.  "
"Internet Navigator asks you where you want to save the file and then copies the file to your computer.",

"You can also upload files, or transfer them from your computer to another."
"  Just click the Upload button in the FTP Browser window and select the file you want uploaded."
"  Internet Navigator then tries to transfer the selected file from your computer to the FTP site.",
                                                                           
"When you go to different areas of Internet Navigator, the menu bar and tool bar all change their buttons"
" and commands so they reflect the area you're in.",

"The version of the toolbar which shows when you start Internet Navigator "
"is called the Places toolbar.",

"Remember, the Help & Information area is available whether or not you're "
"connected to the Internet.  Use this area to get help on how to use "
"Internet Navigator.",

"You can go to the Preferences area to customize your copy of Internet Navigator."
"  To go to the Preferences area, select Preferences from the Places menu.",

"The Preferences area is available whether or not you're connected to the"
" Internet.  Use this area to customize your copy of Internet Navigator.",

"You can edit your Address Book while composing mail.  To open the Address "
"book, choose the 'Addresses...' button in the Compose Mail window.",

"To set preferences for the Internet areas of Internet Navigator, go to the "
"Preferences area and select the 'Internet Preferences' button.",
};
                                   
// a constant holding the number of tips in the above array                                   
const int gnNumTips = sizeof(apszTips) / sizeof(const char* const);

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipOfDay dialog

CTipOfDay::CTipOfDay(const int nIndex, const BOOL bShowTipAtStartup, CWnd* pParent /*=NULL*/)
	: CDialog(CTipOfDay::IDD, pParent)
{
  // sanity check
  ASSERT(gnNumTips > 0);

  // do bounds checking to prevent disaster
  if(nIndex < 0)
    m_nIndex = 0;
  else if(nIndex >= gnNumTips)
    m_nIndex = gnNumTips - 1;
  else
    m_nIndex = nIndex;  
    
  //{{AFX_DATA_INIT(CDialogTipOfTheDay)
  m_bShowTips = bShowTipAtStartup;
  m_strTipText = "";
  //}}AFX_DATA_INIT
}

void CTipOfDay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTipOfDay)
	DDX_Check(pDX, IDC_SHOW_TIP_AT_STARTUP, m_bShowTips);
	DDX_Text(pDX, IDC_TIP_TEXT_EDIT, m_strTipText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTipOfDay, CDialog)
	//{{AFX_MSG_MAP(CTipOfDay)
	ON_BN_CLICKED(ID_NEXT_TIP, OnNextTip)
	ON_BN_CLICKED(ID_PREV_TIP, OnPrevTip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTipOfDay message handlers

void CTipOfDay::OnNextTip()
{
  // go to next tip; wrap if at end of list
  if(++m_nIndex >= gnNumTips)
      m_nIndex = 0;          
      
  // write tip text to edit control    
  m_strTipText = apszTips[m_nIndex];
  UpdateData(FALSE);
}

void CTipOfDay::OnPrevTip()
{
  // go to previous tip; wrap if at beginning of list
  if (--m_nIndex < 0)
    m_nIndex = gnNumTips - 1;
    
  // write tip text to edit control  
  m_strTipText = apszTips[m_nIndex];
  UpdateData(FALSE);	
}

BOOL CTipOfDay::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Fill the edit control with the next tip
	OnNextTip();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

int CTipOfDay::GetIndex(void)
{
  return m_nIndex;
}


BOOL CTipOfDay::GetShowTipsAtStartup(void)
{
  return m_bShowTips;
}
