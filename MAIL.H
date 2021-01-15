// mail.h : header file
//

#ifndef __MAIL_H__
#define __MAIL_H__   
#include "percent.h"

class CNewMailView;

/////////////////////////////////////////////////////////////////////////////
// CMailCommandState -- helps us keep track of what we're doing

	enum CMailCommandState
	{
		idle,   	// When we're not doing nothin'
		CheckMail,	// When we're checking the user's mail
		Delete,		// When we're deleting an e-mail message
		Send		// When we're sending an e-mail message
	};

/////////////////////////////////////////////////////////////////////////////
// CMailListBox -- a list box for showing mail messages which we use in the
// CNewMailView view class below.  CMailListBox shows a little 'file' icon
// next to each message

/*class CMailListBox : public CListBox
{
// drawing methods
public:  
	virtual void DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap,
			BITMAP& bitmap, short xStart, short yStart,
				COLORREF crTransparentColor);
				
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);  

// Attributes
public:
	CNewMailView* m_pMailView;

// Operations
public:
	void Attach(CNewMailView* pView)
		{ m_pMailView = pView; }
	
// Implementation
protected: // Generated message map functions
	//{{AFX_MSG(CMailListBox)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};*/ 

/////////////////////////////////////////////////////////////////////////////
// CMailData data structure class           

class CMailData
{
friend class CInternetNavApp;
friend class CMailBoxDoc;
// Construction
public:      
	CMailData();
	CMailData(CMailBoxDoc* pDoc, CInternetNavApp* pApp);
	~CMailData();

// Attributes
public:
	CString m_strAddress;   
	CString m_strDomainName;
	CString m_strSMTPHost;
	CString m_strPOP3Host;
	CString m_strPassword;
	CString m_strPOP3Logon;
	int m_nWalkThrough;

	CMailBoxDoc* m_pDocument;
	CInternetNavApp* m_pApp;  
	
public:
	void Attach(CMailBoxDoc* pDoc, CInternetNavApp* pApp)
		{	ASSERT(pDoc); ASSERT(pApp); m_pApp = pApp; m_pDocument = pDoc; }
	
	void Detach() { m_pApp = NULL; m_pDocument = NULL; }

// Implementation
public:
	void Serialize(BOOL IsLoading); // transfers data to and from document
	void DeleteContents(); // deletes contents of data structure
	
    void Open(); // initializes data
    void Close(); // writes all data to document-- Finish button in wizard
    void WizardCancel(); // in case user presses Cancel in Wizard
};

/////////////////////////////////////////////////////////////////////////////

class CMailBoxDoc : public CDocument
{        
friend class CMailBoxView;
friend class CNewMailView;
friend class CMessageView;
friend class CComposeView;
friend class CHelpInfoView;
friend class CArticleView;       
friend class CInternetNavApp; 
friend class CAskStaffView;
protected: // create from serialization only
	CMailBoxDoc();
	DECLARE_DYNCREATE(CMailBoxDoc)

// Attributes
public:
	// Data for e-mail connect and message setup
	//{{AFX_DATA(CMailBoxDoc) 
	CString m_strAddress; // e-mail address (user@somesite.com)
	CString m_strSMTPHost;// out-going mail server address
	CString m_strPOP3Host;// in-coming mail server address    
	CString m_strPOP3Logon; // in-coming mail server user's ID
	CString m_strDomainName;  
	CString m_strMailBoxTitle;  
	CString m_strPassword; // mail password
	CString m_strSignature; // Signature for mail messages
	BOOL m_bInitialized; 
	BOOL m_bDefault;  
	CString m_strUserName, m_strOrg;
	//}}AFX_DATA 
	                                     
	CUIntArray m_nDeleArray;
		                                  
	CInternetNavApp* m_pApp;
	
	CStringArray m_strAddressList; 
	
public: // helper functions to access data
	CString GetMailBoxTitle() {return m_strMailBoxTitle; }  
	
	CString GetUserID() { return m_strPOP3Logon; }
	CString GetPassword() { return m_strPassword; }
	
	void AddAddress(CString& strAddress);
	void RemoveAddress(int nAddress);
	
// Operations
public:
	// Functions to show the steps of the Mailbox Setup Wizard
	void ShowWizardChoiceStep(); 
	void ShowWizardStep3();
	void ShowWizardStep4();               
	void ShowWizardStep5();
	void ShowWizardStep6();
	void ShowWizardStep7();
	
	void OnWizardMailSettings();      
	
	// Deletes each message number in m_nDeleArray
	void DeleteAll(QSocket* pSocket, CListBox* pListBox, CNewMailView* pView);    
	void MarkMessageDeleted(int nMessage);

	// To keep the title the way we want it
	virtual void SetTitle(const char* pszTitle);
	
// Implementation
public:
	virtual ~CMailBoxDoc();

	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();			// overridden to delete data
	virtual BOOL SaveModified();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument(); 
	virtual BOOL OnOpenDocument(const char* pszPathName); 
	virtual BOOL OnSaveDocument(const char* pszPathName); 
	virtual void OnCloseDocument();  

	// CMailData data class for Mailbox Setup Wizard
	CMailData m_mailData;
public:
	CMailData* GetMailData() { return &m_mailData; }

// Generated message map functions
protected:
	//{{AFX_MSG(CMailBoxDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnMailboxSetup();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnMailboxLogon();
	afx_msg void OnMailSettings();
	afx_msg void OnMailSignature();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMailBoxView form view

class CMailBoxView : public CFormView
{
	DECLARE_DYNCREATE(CMailBoxView)
protected:
	CMailBoxView();			// protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CMailBoxView)
	enum { IDD = IDR_MAILBOXTYPE };
	CBitmapButton	m_btnFinger;
	CBitmapButton	m_btnHelpInfo;
	CBitmapButton	m_btnReadNewMail;
	CBitmapButton	m_btnSetup;
	CBitmapButton	m_btnCompose; 
	CBitmapButton	m_btnAddrBook;
	CMailBoxDoc*	m_pDoc;
	CString 		m_strMailBoxTitle;
	//}}AFX_DATA

// Attributes
public:
	CInternetNavApp* m_pApp;
	CInetnavStatusBar* m_pStatusBar;
	QSocket* m_pSocket;  
	
	CMailBoxDoc* GetDocument();               

// Operations
public:    
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint = 0L, 
				CObject* pHint = NULL);

// Implementation
protected:
	virtual ~CMailBoxView();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CMailBoxView)
	afx_msg void OnFinger();
	afx_msg void OnHelpAndInfo();
	afx_msg void OnMailboxCompose();
	afx_msg void OnMailboxSetup();
	afx_msg void OnMailboxCheck();
	afx_msg void OnUpdateMailSend(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMailReply(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMailDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMailDisplayMessage(CCmdUI* pCmdUI);
	afx_msg void OnMailAddressBook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mail.cpp
inline CMailBoxDoc* CMailBoxView::GetDocument()
   { return (CMailBoxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewMailView form view: presents a list of the user's incoming electronic
// mail

class CNewMailView : public CFormView
{
friend class CMailBoxView;
protected: // create from serialization only
	CNewMailView();
	DECLARE_DYNCREATE(CNewMailView)

// Form data
public:
	//{{AFX_DATA(CNewMailView)
	enum { IDD = IDR_NEWMAILTYPE };
	CButton	m_btnHelp;
	CButton	m_btnDelete;
	CButton	m_btnDisplay;
	CButton	m_btnCheck;
	CListBox m_MessageList;
	CInternetNavApp* m_pApp;
	CString	m_strMessageSelection;
	CString	m_strNumMessages;
	//}}AFX_DATA             
	int m_nMessages;

// Attributes
public:
	CMailBoxDoc* GetDocument();     
	CMailBoxDoc* m_pDoc;           
	CInetnavStatusBar* m_pStatusBar;
	
	QSocket* m_pSocket;       	// TODO: Move to CMailBoxDoc 
	
private:
	CMailCommandState State;	// keeps track of what's going on
	CPercentDialog* m_pPercentDialog;// dialog for showing percentage done of
										// tasks as they are completed 
	// Common CString variables used in functions
	CString m_strLine; // current line of mail message
	CString m_strFrom; // message source address
	CString m_strSubject; // message subject   
	CString m_strResponse;
	
public:	
	// List of message subject
	CStringArray m_strSubjectList;
	
	// List of message senders
	CStringArray m_strSenderList;
		
// Operations
public:         
	virtual void OnInitialUpdate();    
	virtual void OnUpdate(CView* pSender, LPARAM lHint = 0L, 
				CObject* pHint = NULL);
	
// Implementation
public:
	virtual ~CNewMailView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void DoFillListBox(); // fills list box with list of messages
	void DoCreateListLine(); 
	BOOL DoPOP3Logon();		// TODO: Move this to CMailBoxDoc

	void OnSocketConnected();	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Generated message map functions
protected:
	//{{AFX_MSG(CNewMailView)
	afx_msg void OnMailDelete();
	afx_msg void OnMailDisplay();
	afx_msg void OnCheckMailbox();
	afx_msg void OnUpdateMailDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMailDisplay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCheckMailbox(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnSocketResponse(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mail.cpp
inline CMailBoxDoc* CNewMailView::GetDocument()
   { return (CMailBoxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageView view -- for viewing e-mail

class CMessageView : public CEditView
{ 
friend class CMessageWnd;
protected: // create from serialization only
	CMessageView(); 
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_DYNCREATE(CMessageView)

// Attributes
public:            
	// static init/term...
	static void Initialize();
	static void Terminate();

	CMailBoxDoc* GetDocument();    
	CInternetNavApp* m_pApp;   
	CMailBoxDoc* m_pDoc;
	UINT m_nMessage;	// message number of this e-mail message
	QSocket* m_pSocket; // communications channel class 
	CInetnavStatusBar* m_pStatusBar; // to report progress to the user

private:
	CMailCommandState State;
	
	// Common CString variables used in functions
	CString m_strText; // status bar text
	CString m_strSend; // sent via m_pSocket
	CString m_strMsg;  // message box text
	CString m_strResponse; // mail server responses    
	CString m_strLine; // current line of mail message
	CString m_strFrom; // message source address
	CString m_strSubject; // message subject          
	int m_nMessages;
	
// Operations
public:
	virtual void OnInitialUpdate();
	
	// Word wrap...
	BOOL IsWordWrap() const;
	BOOL SetWordWrap(BOOL bWordWrap);

	// Buffer access...
	LPCSTR LockBuffer() const
		{ return CEditView::LockBuffer(); }
	void UnlockBuffer() const
		{ CEditView::UnlockBuffer(); }
    
    // to set the number of the e-mail message the user wants this view
    // to retrieve
    void SetMessageNumber(UINT nMessage, 
    	UINT nNumMessages,	BOOL bMove = FALSE);
    
// Implementation
public:           
	BOOL IsUpdatePending() { return (m_uTimerID != NULL); }
	
protected:   
	virtual ~CMessageView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	UINT m_uTimerID; // ==0 when no outstanding

	static LOGFONT NEAR m_lfDefFont;
	static LOGFONT NEAR m_lfDefFontOld;
	CFont m_font;

	static LOGFONT NEAR m_lfDefPrintFont;
	static LOGFONT NEAR m_lfDefPrintFontOld;
	CFont m_fontPrint;

	static UINT m_nDefTabStops;
	static UINT m_nDefTabStopsOld;
	static BOOL m_bDefWordWrap;
	static BOOL m_bDefWordWrapOld;
	
	int m_nHeaderTime;
	int m_nFooterTime;

	UINT m_nPreviewPage;
	CTime m_timeHeader;
	CTime m_timeFooter;

	BOOL m_bUndo;  // undo/redo flag

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo); 
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnScrollTo(CDC* pDC, CPrintInfo* pInfo, POINT point);

// Generated message map functions
protected:
	//{{AFX_MSG(CMessageView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMirrorDisplayFont();
	afx_msg void OnWordWrap();
	afx_msg void OnUpdateWordWrap(CCmdUI* pCmdUI);
	afx_msg void OnChoosePrintFont();
	afx_msg void OnUpdateChoosePrintFont(CCmdUI* pCmdUI);
	afx_msg void OnChooseFont();
	afx_msg void OnPageSetup();
	afx_msg void OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI);
	afx_msg void OnWindowRemember();
	afx_msg void OnEditUndo();    
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnMessageFirst();
	afx_msg void OnMessagePrev();
	afx_msg void OnMessageNext();
	afx_msg void OnMessageLast();
	afx_msg void OnUpdateMessageFirst(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMessagePrev(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMessageNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMessageLast(CCmdUI* pCmdUI);
	afx_msg void OnMailReply();  
	afx_msg void OnEditChange();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnSocketResponse(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mail.cpp
inline CMailBoxDoc* CMessageView::GetDocument()
   { return (CMailBoxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CComposeView view -- for composing e-mail messages 
// see compview.cpp for the implementation of this class

class CComposeView : public CEditView
{ 
friend class CComposeWnd;
protected: // create from serialization only
	CComposeView(); 
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_DYNCREATE(CComposeView)

// Attributes
public:            
	// static init/term...
	static void Initialize();
	static void Terminate();

	CMailBoxDoc* GetDocument();
	CMailBoxDoc* m_pDoc;    
	CInternetNavApp* m_pApp;   
	
	QSocket* m_pSocket;
	
	CStringArray m_strToList, m_strCCList, m_strBCCList;
	CString m_strTo, m_strCC, m_strBCC;  
	
	CInetnavStatusBar* m_pStatusBar;
	CComposeWnd* m_pFrame;   
	CPercentDialog* m_pPercentDialog;
	CMailCommandState State;   
	
	BOOL m_bDisconnectAfterSend; 
	
	CString m_strFeedback;           
	
	BOOL m_bShowWindow;
	int m_nEase, m_nNav, m_nQuality;
	CString m_strEase, m_strNav, m_strQuality; 
	
	CString m_strAdd, m_strNix;	

// Operations
public:
	// Word wrap...
	BOOL IsWordWrap() const;
	BOOL SetWordWrap(BOOL bWordWrap);

	// Buffer access...
	LPCSTR LockBuffer() const
		{ return CEditView::LockBuffer(); }
	void UnlockBuffer() const
		{ CEditView::UnlockBuffer(); }   
		
	virtual void OnInitialUpdate();
     
    // For SMTP-sending the mail message the user composes with this
    // view
	void SendMailMessage(); 
	
	// To validate mail message headers
	BOOL ValidateMailHeaders();
	BOOL ProcessRecipients();
	
	// For sending of mail message headers
	void SendMailHeaders();
	void PrepareMailHeaders();      
	
	void PrepareFeedbackMessage();
	void SendFeedbackMessage();
	
// Implementation
public:           
	BOOL IsUpdatePending() {return (m_uTimerID != NULL);}
	
	// Helpers for the Compose Mail Wizard
	void DoCompWizard();
	void DoCompWiz2();
	void DoCompWiz3();
	void DoCompWiz4();
	void DoCompWiz5();
	void DoCompWizFinish();   
	
	void DoFeedbackWizard();
	void DoFeedbackWiz2();
	void DoFeedbackWiz3();
	void DoFeedbackWiz4();
	void DoFeedbackWiz5();	     
	
protected:
	virtual ~CComposeView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:           
	CString m_strSubject;
	
	UINT m_uTimerID; // ==0 when no outstanding

	static LOGFONT NEAR m_lfDefFont;
	static LOGFONT NEAR m_lfDefFontOld;
	CFont m_font;

	static LOGFONT NEAR m_lfDefPrintFont;
	static LOGFONT NEAR m_lfDefPrintFontOld;
	CFont m_fontPrint;

	static UINT m_nDefTabStops;
	static UINT m_nDefTabStopsOld;
	static BOOL m_bDefWordWrap;
	static BOOL m_bDefWordWrapOld;
	
	int m_nHeaderTime;
	int m_nFooterTime;

	UINT m_nPreviewPage;
	CTime m_timeHeader;
	CTime m_timeFooter;

	BOOL m_bUndo;  // undo/redo flag

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo); 
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnScrollTo(CDC* pDC, CPrintInfo* pInfo, POINT point);

// Generated message map functions
protected:
	//{{AFX_MSG(CComposeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMirrorDisplayFont();
	afx_msg void OnWordWrap();
	afx_msg void OnUpdateWordWrap(CCmdUI* pCmdUI);
	afx_msg void OnChoosePrintFont();
	afx_msg void OnUpdateChoosePrintFont(CCmdUI* pCmdUI);
	afx_msg void OnChooseFont();
	afx_msg void OnPageSetup();
	afx_msg void OnUpdateMirrorDisplayFont(CCmdUI* pCmdUI);
	afx_msg void OnWindowRemember();
	afx_msg void OnEditUndo();    
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnMailSend();
	afx_msg void OnEditChange();
	//}}AFX_MSG     
	afx_msg LRESULT OnSocketResponse(WPARAM wParam, LPARAM lParam);  
	afx_msg LRESULT OnComposeWizard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFeedbackWizard(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mail.cpp
inline CMailBoxDoc* CComposeView::GetDocument()
   { return (CMailBoxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailBoxWnd frame

class CMailBoxWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMailBoxWnd)
protected:
	CMailBoxWnd();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:     
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child 
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

// Implementation
protected:
	virtual ~CMailBoxWnd();

	// Generated message map functions
	//{{AFX_MSG(CMailBoxWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewMailWnd frame

class CNewMailWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CNewMailWnd)
protected:
	CNewMailWnd();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child 
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

// Implementation
protected:
	virtual ~CNewMailWnd();

	// Generated message map functions
	//{{AFX_MSG(CNewMailWnd)
	afx_msg void OnWindowRemember();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMessageWnd frame

class CMessageWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMessageWnd)
protected:
	CMessageWnd();			// protected constructor used by dynamic creation

// Attributes
public:            
	CString m_strSubject; 

// Data access helper functions 
public:
	CString GetMessageSubject() { return m_strSubject; }
	void SetSubject(CString strSubject) { m_strSubject = strSubject; }
	
protected: // control bar embedded members
	CDialogBar m_wndRulerBar; // for a ruler along the top edge of the window
	CBitmapButton m_btnRuler;   
	CDialogBar m_wndMailBar;  // for a command button dialog bar

// Operations
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	
// Implementation
public:
	virtual ~CMessageWnd();
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CComposeBar dialog bar

class CComposeBar : public CDialogBar
{ 
friend class CComposeWnd;
// Construction
public:
	CComposeBar();	// standard constructor for dialog bars 
	BOOL Create(CWnd* pParentWnd, LPCSTR lpszTemplateName,
		UINT nStyle, UINT nID);
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);

// Dialog Data
	//{{AFX_DATA(CComposeBar)
	enum { IDD = IDD_MAIL_COMPOSE_DIALOG_BAR };
	CString	m_strTo;
	CString	m_strCC;
	CString	m_strBCC;
	CString	m_strSubject;
	//}}AFX_DATA
	CWnd* m_pParentWnd;  // for WinHelp() call  
	CInternetNavApp* m_pApp;     

	CString GetSubject() { return m_strSubject; }
	CString GetTO() { return m_strTo; }
	CString GetCC() { return m_strCC; }
	CString GetBCC() { return m_strBCC; }  
	
	void SetSubject(const CString& strSubject)
		{ m_strSubject = strSubject; }
	void SetTO(const CString& strTo)
		{ m_strTo = strTo; }
	void SetCC(const CString& strCC)
		{ m_strCC = strCC; }
	void SetBCC(const CString& strBCC)
		{ m_strBCC = strBCC; }
		
	BOOL UpdateInfo() { return UpdateData(TRUE); }

// Implementation  
public:
	CMailBoxDoc* m_pDoc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CComposeBar)
	afx_msg void OnEditChange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelp();
	afx_msg void OnUpdateHelp(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CComposeWnd frame

class CComposeWnd : public CMDIChildWnd
{
friend class CComposeBar;
friend class CComposeView;  
friend class CMessageView;
	DECLARE_DYNCREATE(CComposeWnd)
protected:
	CComposeWnd();			// protected constructor used by dynamic creation

// Attributes
public:
	BOOL m_bIsInetnavTechSupport;
	BOOL m_bIsISPTechSupport;
	BOOL m_bIsSuggestionBox; 
	
	BOOL m_bIsUseCompWizard;  
	BOOL m_bIsUseFeedbackWizard; // should be set in combo with sug. box flag
	
	BOOL IsUseCompWizard() { return m_bIsUseCompWizard; }
	BOOL IsUseFeedbackWizard() { return m_bIsUseFeedbackWizard; }
	
protected: // control bar embedded members
	CDialogBar m_wndRulerBar; // for a ruler along the top edge of the window
	CBitmapButton m_btnRuler;   
	CComposeBar m_wndInfoBar;  // for a header information dialog bar
	CDialogBar m_wndDlgBar;                    
	CDialogBar m_wndISPBar;     
	CDialogBar m_wndSuggBar;
	
	CMailBoxDoc* m_pDoc;

// Operations
public: 
	CComposeBar& GetInfoBar() { return m_wndInfoBar; } 
	CDialogBar& GetWnDBSoftHelpBar() { return m_wndDlgBar; }    
	CDialogBar& GetISPHelpBar() { return m_wndISPBar; }    
	CDialogBar& GetSuggBar() { return m_wndSuggBar; }
	CDialogBar& GetRulerBar() { return m_wndRulerBar; }
	
	BOOL UpdateBarInfo() { return m_wndInfoBar.UpdateInfo(); }
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child 
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);


// Implementation
protected:
	virtual ~CComposeWnd();

	// Generated message map functions
	//{{AFX_MSG(CComposeWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHelp(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	afx_msg void OnMailAddressBook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__MAILBOX_H__