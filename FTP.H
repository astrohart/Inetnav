// ftp.h: definitions for Internet Navigator FTP classes
// for implementation, see ftp.cpp

#ifndef __FTP_H__
#define __FTP_H__
#include "percent.h"

#define TYPE_ASCII 0
#define TYPE_BINARY 1 

/////////////////////////////////////////////////////////////////////////////
// CFilesListBox : a list box for displaying FTP lists with discriptive icons

class CFilesListBox : public CListBox
{
// drawing methods
public:
	// For drawing listbox bitmaps transparent
	virtual void DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap,
			BITMAP& bitmap, short xStart, short yStart,
				COLORREF crTransparentColor);
			
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

/////////////////////////////////////////////////////////////////////////////
// CDataChannelState: Helps us keep track of what's happening

	enum CDataChannelState
	{
		Idle,		   // when we're not doing anything
		ReceivingFile, // when we're downloading a file
		ReceivingList, // when we're getting a list of files
		SendingFile,   // when we're sending a file    
		ReceivingWelcome, // when we're expecting a welcome message
	};

/////////////////////////////////////////////////////////////////////////////
// CFtpDoc

// Forward declaration of CFtpView
class CFtpView;

class CFtpDoc : public CDocument
{        
friend class CFtpView;
protected: // create from serialization only
	CFtpDoc();
	DECLARE_DYNCREATE(CFtpDoc)

// Attributes
public:
	// Data for FTP site connect setup
	//{{AFX_DATA(CFtpDoc) 
	CString m_strSiteName;
	CString m_strUserID;
	CString m_strPassword;   
	//}}AFX_DATA 
	BOOL m_bEmergencyClose;  
	CString m_strPathTitle;
		
	CInternetNavApp* m_pApp;
	BOOL m_bFTPAutoConnect;

// Operations
public: 
	virtual void SetTitle(const char* pszTitle, BOOL bFTPDir = FALSE);  
	virtual void SetPathName(const char* pszPathName, BOOL bAddToMRU = TRUE);
	
	// Get/Set helpers
	CString& GetSiteName() { return m_strSiteName; }
	CString& GetUserID() { return m_strUserID; }
	CString& GetPassword() { return m_strPassword; }

// Implementation
public:
	virtual ~CFtpDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();                  // overridden to delete data
	virtual BOOL SaveModified();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument(); 
	virtual BOOL OnOpenDocument(const char* pszPathName); 
	virtual BOOL OnSaveDocument(const char* pszPathName);

// Generated message map functions
protected:
	//{{AFX_MSG(CFtpDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnToolsFileManager();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFtpView: a FTP Client view with controls for uploading, downloading,
// and browsing files, directories, and links; specifying, connecting to, 
// and disconnecting from a FTP site.  Also on this form is another control
// which displays messages this client and the remote server are exchanging.

class CFtpView : public CFormView
{   
typedef unsigned long int ULONG;
friend class CInetnavStatusBar;
friend class CFtpDoc;  
friend class CFtpWnd;
protected: // create from serialization only
	CFtpView();
	DECLARE_DYNCREATE(CFtpView)

public:
	//{{AFX_DATA(CFtpView)
	enum { IDD = IDR_FTPTYPE };
	CEdit	m_ctlSiteName;
	CButton	m_btnConnect;
	CButton	m_btnDisconnect;
	CButton	m_btnHelp;
	CButton	m_btnPut;
	CButton	m_btnGet;
	CFilesListBox m_ctlFiles;
	CButton	m_btnChgDir;
	CButton	m_btnUp;
	CFtpDoc*	m_pDoc;
	CString	m_strSelection;
	//}}AFX_DATA
	CString m_strLeftovers, m_strMsg, m_strPrevDir;  
	ULONG m_nFileType; 
	ULONG m_nFileSize; // size of file to download 
	ULONG m_nBytes; // amount downloaded  
	
	int m_nSelection; // current selection in list box, if any
	
// Attributes
public:
	CFtpDoc* GetDocument(); 

	BOOL m_bMarkDirty; // should we mark document as dirty during UpdateData?
	BOOL m_bEnableUp;      
	
	CFile* m_pUserFile; // file sent or downloaded by user
	
	CString m_strFileName; // name of downloaded/uploaded file (not inc. path)    
	CString m_strFilePath; // download/upload path
	CString m_strFileExt;   
	
	CFileStatus m_userStatus;  // status of a file to upload
	
	CString m_strPWDirectory;
	CString m_strAOLScreenName;
	CString m_strDLFileName;     
	
	BOOL IsProgramFile();	// Tells us whether or not a binary file is 
								// a program (EXE, COM, BAT, or PIF)
	BOOL IsHelpFile();		// Tells us whether or not a binary file is
								// a Windows Help file (HLP)
// Implementation
private: // "sockets": used for Internet communications
	QSocket* m_pControlSocket; // main communications manager socket
	QSocket* m_pListeningSocket; // data locator socket
	QSocket* m_pDataSocket; // manages data transfer 
	
	CMapStringToString m_ftpLinkDestMap;
	
	CString m_strWelcome;

public:        
	CInetnavStatusBar* m_pStatusBar;   
	CInternetNavApp* m_pApp;                    
	CFTPercentDialog* m_pPercentDialog;
	CString m_strDirectory; // AOL FTP directory  
	
	BOOL m_bConnected; // are we connected to the FTP site?
	BOOL m_bNoWelcome; // Do we have the welcome message yet? 
	BOOL m_bAppClosing;

	CDataChannelState State; // tells us what's happening

// Operations
public: 
	void ResetToolBar(); // To force the toolbar to be compatible with this
							// area
	
	virtual void OnInitialUpdate();  
	
	void OnEndList();		// When the transfer of a listing is complete
	void OnEndDownload();	// When the current download is completed     
	void OnEndUpload();		// When the current upload is completed
	
	void GetFileType(); // determines whether ASCII or binary from file ext
	
	// For downloading and uploading
	void DoDownload(char* pszBuffer, unsigned long int ulBuffer);
	void DoUpload();
	void AdvanceFilePointer(LONG lOff, UINT nBytesRead);
	
	void DoOpenDownloadedFile();
	void DoVirusScan();
	
	// Handles displaying changed document data
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	
	// Other helper functions
	BOOL GetUserIDAndPassword();
	BOOL HandleWelcomeMsg();
	BOOL HandleResponse(BOOL bRETR = FALSE);  
	BOOL GetDownloadInfo();
	BOOL GetUploadInfo();
	void ListFiles();  
	
	// diagnostics -- implementation in ftpview.cpp
	#ifdef _DEBUG
		void ReportFTPCommand(CString strCmd);
	#endif //_DEBUG   
	
    void ReportFTPError(CString& strResponse);	
    
    BOOL OpenDataConnection();						

public:
	virtual ~CFtpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CFtpView::OnActivateView(BOOL bActivate, CView* pActivateView,
								CView* pDeactiveView);

// Processing/Filtering Helper Functions
public:
	BOOL OnAddToFilesList(CString& strItem);
	void EmitShellExecuteError(HINSTANCE hInstance);
	void EmitFileException(CFileException* pFileException);								

// Generated message map functions
protected:
	//{{AFX_MSG(CFtpView)
	afx_msg void OnDblclkFiles();
	afx_msg void OnSelchangeFiles();
	afx_msg void OnClickedHelp();
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	afx_msg void OnGet();
	afx_msg void OnPut();
	afx_msg void OnChangeDirectoryUp();
	afx_msg void OnChangeRemoteDirectory();
	afx_msg void OnEditChange();
	afx_msg void OnWindowRefresh();
	afx_msg void OnUpdateWindowRefresh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSiteChgDir(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSiteChgDirUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateConnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDownload(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUpload(CCmdUI* pCmdUI);
	afx_msg void OnWindowRemember();
	afx_msg void OnTransfersLink();
	afx_msg void OnUpdateTransfersLink(CCmdUI* pCmdUI);
	afx_msg void OnCancelCall();
	afx_msg void OnViewFtpProperties();
	afx_msg void OnSiteChgUserDir();
	afx_msg void OnDownloadFile();
	afx_msg void OnFileDelete();
	//}}AFX_MSG
	// Socket callback message handlers
	afx_msg LRESULT OnControlSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListeningSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDataSocket(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ftp.cpp
inline CFtpDoc* CFtpView::GetDocument()
   { return (CFtpDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CFtpWnd frame

class CFtpWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFtpWnd)
protected:
	CFtpWnd();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:  
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs); 
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child
// Implementation
protected:
	virtual ~CFtpWnd();

	// Generated message map functions
	//{{AFX_MSG(CFtpWnd)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__FTP_H__