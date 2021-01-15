// news.h: header file
//

#ifndef __NEWS_H__
#define __NEWS_H__
#include "socket.h"
#include "percent.h"  
#include "gopher.h"
/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsListBox -- this listbox is used for showing the list of
// newsgroups the user is subscribed to

class CNewsgroupsListBox : public CListBox
{   
// Iplementation
public:
	// For drawing listbox bitmaps transparent
	virtual void DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap,
			BITMAP& bitmap, short xStart, short yStart,
				COLORREF crTransparentColor);
				
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewsgroupsListBox)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

/////////////////////////////////////////////////////////////////////////////
// CArticleType -- specifies what we're getting from the newsgroups server:
// an article, an article's headers, or an article's body

	enum CArticleType
	{
		Article,	// we're downloading an entire article, headers and body
		Headers,	// we're retrieving headers
		Body,		// we're retrieving the body
		IDLE,		// we ain't doin' nothin'!
	};      
	
/////////////////////////////////////////////////////////////////////////////

class CNewsView : public CFormView
{
protected: // create from serialization only
	CNewsView();
	DECLARE_DYNCREATE(CNewsView)

public:
	//{{AFX_DATA(CNewsView)
	enum { IDD = IDR_NEWSAREATYPE };
	CBitmapButton	m_btnPostOffice;
	CBitmapButton	m_btnCompose;
	CBitmapButton	m_btnGroupMan;
	CBitmapButton	m_btnHelp;
	CBitmapButton	m_btnSetup;
	CNewsDoc*	m_pDoc;
	//}}AFX_DATA

// Attributes
public:
	CNewsDoc* GetDocument();
	CInternetNavApp* m_pApp;

// Operations
public:   
	virtual void OnInitialUpdate();

// Implementation
public:
	virtual ~CNewsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 
// Generated message map functions
protected:
	//{{AFX_MSG(CNewsView)
	afx_msg void OnMailManager();
	afx_msg void OnHelpinfo();
	afx_msg void OnNewsgroupsSetup();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnGroupManager();
	afx_msg void OnArticleAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in newsvw.cpp
inline CNewsDoc* CNewsView::GetDocument()
   { return (CNewsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManager form view 
// This class is the interface of the Internet Navigator Internet Newsgroups
// Manager, which manages the list of UseNet newsgroups the user subscribes
// to.

class CNewsgroupsManager : public CFormView
{
	DECLARE_DYNCREATE(CNewsgroupsManager)
protected:
	CNewsgroupsManager();			// protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CNewsgroupsManager)
	enum { IDD = IDR_GROUPMAN };
	CNewsgroupsListBox	m_ctlGroupList;
	CNewsDoc*	m_pDoc;
	CString	m_strSelection;
	//}}AFX_DATA
	CInternetNavApp* m_pApp; 
	CInetnavStatusBar* m_pStatusBar;

// Attributes
public:
	CNewsDoc* GetDocument();
	CPercentDialog* m_pPercentDialog;	
	
// Operations
public:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint = 0L,
				CObject* pHint = NULL);

// Implementation
protected:
	virtual ~CNewsgroupsManager();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);   
	void PrintPageHeader(CDC* pDC, CPrintInfo* pInfo, CString& strHeader);
	
	// Generated message map functions
	//{{AFX_MSG(CNewsgroupsManager)
	afx_msg void OnGroupAdd();
	afx_msg void OnListOutOfSpace();
	afx_msg void OnGroupRemove();
	afx_msg void OnUpdateGroupRemove(CCmdUI* pCmdUI);
	afx_msg void OnGroupOpen();
	afx_msg void OnUpdateGroupOpen(CCmdUI* pCmdUI);
	afx_msg void OnArticleAge();
	//}}AFX_MSG
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in newsvw.cpp
inline CNewsDoc* CNewsgroupsManager::GetDocument()
   { return (CNewsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionView: allows the user to select articles they want to
// view

class CArticleSelectionView : public CFormView
{
protected: // create from serialization only
	CArticleSelectionView();
	DECLARE_DYNCREATE(CArticleSelectionView)

public:
	//{{AFX_DATA(CArticleSelectionView)
	enum { IDD = IDD_ARTSELTYPE_FORM };
	CListBox m_ctlArticleList;
	CString	m_strSelection;
	CString	m_strGroupLabel;
	//}}AFX_DATA
	int m_nArticles;

// Attributes
public:
	CNewsDoc* GetDocument();
	
	CString m_strGroup; // Name of the group whose articles are being
								// displayed
	CNewsDoc* m_pDoc;
	CInternetNavApp* m_pApp;
	CInetnavStatusBar* m_pStatusBar;
	CPercentDialog* m_pPercentDialog;

// Operations
public:  
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint = 0L,
			CObject* pHint = NULL);
	
	void DoFillListBox(); 
	BOOL TestLists();  

// Implementation
public:
	virtual ~CArticleSelectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Generated message map functions
protected:
	//{{AFX_MSG(CArticleSelectionView)
	afx_msg void OnWindowRefresh();
	afx_msg void OnArticleOpen();
	afx_msg void OnUpdateArticleOpen(CCmdUI* pCmdUI);
	afx_msg void OnArticleAge();
	afx_msg void OnArticleAdd();
	//}}AFX_MSG
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in newsvw.cpp
inline CNewsDoc* CArticleSelectionView::GetDocument()
   { return (CNewsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CArticleView view -- for viewing newsgroup articles

class CArticleView : public CEditView
{ 
friend class CArticleWnd;
protected: // create from serialization only
	CArticleView(); 
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_DYNCREATE(CArticleView)

// Attributes
public:            
	// static init/term...
	static void Initialize();
	static void Terminate();

	CNewsDoc* GetDocument();    
	CInternetNavApp* m_pApp; 
	CInetnavStatusBar* m_pStatusBar;
	CArticleWnd* m_pParentFrame;  
	
	BOOL m_bFirstCall;
	
	CString m_strArticle;
	  
	int m_nArticles;
	
	// Newsgroup name and article subject
	CString m_strGroup; // Name of newsgroup of this article 
	CString m_strAuthor; // Article author's e-mail address
	CString m_strSubject; // Name of this article's subject.
	int m_nMessageID; // index of MessageID of this article
	
	CString& GetGroupName() { return m_strGroup; }
	CString& GetSubject() { return m_strSubject; }  
	int GetMessageIDIndex() { return m_nMessageID; }
	CString& GetAuthor() { return m_strAuthor; }
	
	void SetGroupName(const CString& strGroup) { m_strGroup = strGroup; }
	void SetSubject(const CString& strSubject) { m_strSubject = strSubject; }
	void SetMessageIDIndex(const int nIndex)
		{ m_nMessageID = nIndex; }   
	void SetAuthor(const CString& strAuthor) { m_strAuthor = strAuthor; }
	
	int GetNumArticles() { return m_nArticles; }
	void SetNumArticles(int nArticles) { m_nArticles = nArticles; }
	
	CStringArray m_strMessageIDs;
    
    CStringArray m_strSubjectList;  
    CStringArray m_strSenderList;
    
    CArticleType Type;
    
    CNewsDoc* m_pDoc;
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
    
	void GetArticle(CString& strMessageID);
	void GetArticleHeaders(CString& strMessageID);
	void GetArticleBody(CString& strMessageID);
	    
// Implementation
public:           
	BOOL IsUpdatePending() {return (m_uTimerID != NULL);}
	
protected:
	virtual ~CArticleView();
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
	//{{AFX_MSG(CArticleView)
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
	afx_msg void OnArticleFirst();
	afx_msg void OnUpdateArticleFirst(CCmdUI* pCmdUI);
	afx_msg void OnArticlePrevious();
	afx_msg void OnUpdateArticlePrevious(CCmdUI* pCmdUI);
	afx_msg void OnArticleNext();
	afx_msg void OnUpdateArticleNext(CCmdUI* pCmdUI);
	afx_msg void OnArticleLast();
	afx_msg void OnUpdateArticleLast(CCmdUI* pCmdUI);
	afx_msg void OnArticleFollowup();
	afx_msg void OnArticleReply();
	afx_msg void OnArticleAdd();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnArticleOptions();
	//}}AFX_MSG
	afx_msg LRESULT OnSocketResponse(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditChange();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in textview.cpp
inline CNewsDoc* CArticleView::GetDocument()
   { return (CNewsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeView view -- for composing and posting newsgroup articles

class CArticleComposeView : public CEditView
{ 
friend class CArticleComposeWnd;
protected: // create from serialization only
	CArticleComposeView(); 
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_DYNCREATE(CArticleComposeView)

// Attributes
public: 
	// The References, Newsgroups, Subject, Distribution, Keywords, Date, and
	// From headers
	CString		m_strReferences; // Other articles referenced by this one
	CString		m_strNewsgroups; // Newsgroups to post to
	CString		m_strDistribution; // Distribution of this article
	CString		m_strKeywords;	// Search keywords for this article
	CString		m_strDate;		// The date this article was posted
	CString		m_strFrom;		// Author of this article  
	CString		m_strReplyTo;   // To whom readers should reply
	CString		m_strFollowupTo; // Groups to which readers should post
									// followups
	CString 	m_strSubject;
	CString		m_strMessageID;
	
	// Functions to retrieve the above information
	CString& GetReferences() { return m_strReferences; }
	CString& GetNewsgroups() { return m_strNewsgroups; }
	CString& GetDistribution() { return m_strDistribution; }
	CString& GetKeywords() { return m_strKeywords; }
	CString& GetDate() { return m_strDate; }
	CString& GetAuthor() { return m_strFrom; } 
	CString& GetReplyTo() { return m_strReplyTo; }
	CString& GetFollowupTo() { return m_strFollowupTo; }
	CString& GetSubject() { return m_strSubject; }
	CString& GetMessageID() { return m_strMessageID; }
	
	// Functions to set the above information
	void SetReferences(const CString& strReferences)
		{ m_strReferences = strReferences; }
	void SetNewsgroups(const CString& strNewsgroups)
		{ m_strNewsgroups = strNewsgroups; }        
	void SetReplyTo(const CString& strReplyTo)
		{ m_strReplyTo = strReplyTo; }
	void SetFollowupTo(const CString& strFollowupTo)
		{ m_strFollowupTo = strFollowupTo; }
	void SetDistribution(const CString& strDistribution)
		{ m_strDistribution = strDistribution; }
	void SetKeywords(const CString& strKeywords)
		{ m_strKeywords = strKeywords; }
	void SetDate(const CString& strDate)
		{ m_strDate = strDate; }
	void SetFrom(const CString& strFrom) 
		{ m_strFrom = strFrom; } 
	void SetSubject(const CString& strSubject)
		{ m_strSubject = strSubject; }
	void SetMessageID(const CString& strMessageID)
		{ m_strMessageID = strMessageID; }
	
	// Function to get the current date into the m_strDate member
	void FillDate();
	
	// pointer to document
	CNewsDoc* m_pDoc;
	           
	// static init/term...
	static void Initialize();
	static void Terminate();

	CNewsDoc* GetDocument();    
	CInternetNavApp* m_pApp;
	
	CInetnavStatusBar* m_pStatusBar;  
	
	// Is this a followup?
	BOOL m_bFollowup, m_bOriginalMaterial;
	
	// Get/Set functions for m_bFollowup
	BOOL IsFollowup() { return m_bFollowup; }
	void SetFollowup(const BOOL bFollowup = TRUE)
		{ m_bFollowup = bFollowup; }
	
	BOOL DoFollowup(CString& strReferences, CString& strNewsgroups,
		CString& strSubject, CString& strMessageID, CString& strAuthor,
			CString& strDistribution);

// Operations
public:  
	void DoComposeNewsWizard();
	void DoCompNewsWiz2();
	void DoCompNewsWiz3();
	void DoCompNewsWiz4();
	void DoCompNewsWiz5();
	void DoCompNewsWiz6();
	
	// Initial update...
	virtual void OnInitialUpdate();
	
	// Word wrap...
	BOOL IsWordWrap() const;
	BOOL SetWordWrap(BOOL bWordWrap);

	// Buffer access...
	LPCSTR LockBuffer() const
		{ return CEditView::LockBuffer(); }
	void UnlockBuffer() const
		{ CEditView::UnlockBuffer(); } 
	
	// Connection/FlashConnect management...
	QSocket* GetConnection();
	void ReleaseConnection();
	
// Implementation
public:
	// Article Headers management...
	BOOL PrepareHeaders(); // Prepares headers for sending
	void SendHeaders(QSocket* pSocket); // Sends article headers to the sever           
	BOOL ValidateHeaders(); // Checks to make sure that all the headers are
								// in order
	
	BOOL IsUpdatePending() {return (m_uTimerID != NULL);}
	
protected:   
	virtual ~CArticleComposeView();
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
	//{{AFX_MSG(CArticleComposeView)
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
	afx_msg void OnArticleSend();
	afx_msg void OnUpdateArticleSend(CCmdUI* pCmdUI);
	afx_msg void OnNewsgroupsSetup();
	//}}AFX_MSG                       
	afx_msg LRESULT OnComposeWizard(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditChange();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in textview.cpp
inline CNewsDoc* CArticleComposeView::GetDocument()
   { return (CNewsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewsDoc document

class CNewsDoc : public CDocument
{  
friend class CNewsView;
friend class CNewsgroupsManager;
friend class CArticleSelectionView;
friend class CArticleView;
friend class CArticleComposeView; 
friend class CGopherCenter;
protected: // create from serialization only
	CNewsDoc();
	DECLARE_DYNCREATE(CNewsDoc)

// Attributes
private:
	QSocket*	m_pSocket;
	BOOL m_bSocketAvailable; 
	
	BOOL m_bInitialized;
	
	CWnd* m_pParent;
	
public:
	CString m_strNNTPHost;    
	CString m_strOldNNTPHost;
	
	BOOL m_bCanPost;  
	BOOL m_bArticleFlashConnect;
	
	CString m_strEmailAddress, m_strSignature;     
	
	CStringArray m_strMessageIDs;
    
    CStringArray m_strSubjectList;  
    CStringArray m_strSenderList;	
	
	// A list of the newsgroups the user has subscribed to
	CStringArray m_strGroupsArray;
	CInternetNavApp*  m_pApp; 
	
	CString m_strRevNNTPHost;

	void DoNewsgroupsSetup();

private:
	void DoNewsWiz2();
	void DoNewsWiz3();
	void DoNewsWiz4();
		
public:
	QSocket* GetSocket();
	
	BOOL IsSocketAvailable() { return m_bSocketAvailable; }
	void SetSocketAvailable(BOOL bAvailable = TRUE)
		{ m_bSocketAvailable = bAvailable; }
		
	void ReleaseSocket();
	
	BOOL CanPost() { return m_bCanPost; }  

// Operations
public:
	BOOL ConnectToNewsServer(); 
	void ChangeNewsServer(); // called after m_strNNTPHost has been modified
	void DisconnectFromNewsServer();
	
	virtual void SetTitle(const char* pszTitle);

// Implementation
public:
	virtual ~CNewsDoc(); 

	virtual void Serialize(CArchive& ar);   // overridden for document i/o 
	virtual void DeleteContents(); 
	void DeleteArrays();
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif //_DEBUG

protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(const char* pszPathName);
	virtual BOOL OnSaveDocument(const char* pszPathName);
	
public:
	void DisplayNNTPError(int nResponseCode);

// Generated message map functions
protected:
	//{{AFX_MSG(CNewsDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnNewsgroupsSetup();
	afx_msg void OnArticleAdd();
	afx_msg void OnUpdateArticleAdd(CCmdUI* pCmdUI);
	afx_msg void OnArticleSignature();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewsWnd frame

class CNewsWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CNewsWnd)
protected:
	CNewsWnd();			// protected constructor used by dynamic creation

// Attributes
public:
	CInternetNavApp* m_pApp;
  		
// Operations
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child 
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	
// Implementation
protected:
	virtual ~CNewsWnd();

	// Generated message map functions
	//{{AFX_MSG(CNewsWnd)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CNewsgroupsManagerWnd frame

class CNewsgroupsManagerWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CNewsgroupsManagerWnd)
protected:
	CNewsgroupsManagerWnd();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child 
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	
// Implementation
protected:
	virtual ~CNewsgroupsManagerWnd();

	// Generated message map functions
	//{{AFX_MSG(CNewsgroupsManagerWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CArticleSelectionWnd frame

class CArticleSelectionWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CArticleSelectionWnd)
protected:
	CArticleSelectionWnd();			// protected constructor used by dynamic creation

// Attributes
public: 
	CString m_strGroup;

// Operations
public:  
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child 
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	
// Implementation
protected:
	virtual ~CArticleSelectionWnd();

	// Generated message map functions
	//{{AFX_MSG(CArticleSelectionWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CArticleWnd MDI child frame -- this frame holds CArticleView -- a
// CEditView which displays the contents of newsgroup articles

class CArticleWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CArticleWnd)
protected:
	CArticleWnd();			// protected constructor used by dynamic creation

// Attributes
public: 
	int m_nArticles;      
	
	CString m_strText;
	
	// Newsgroup name and article subject
	CString m_strGroup; // Name of newsgroup of this article 
	CString m_strAuthor; // Article author's e-mail address
	CString m_strSubject; // Name of this article's subject.
	int m_nMessageID; // index of MessageID of this article

	CString& GetGroupName() { return m_strGroup; }
	CString& GetSubject() { return m_strSubject; }  
	int GetMessageIDIndex() { return m_nMessageID; }
	CString& GetAuthor() { return m_strAuthor; }
	
	void SetGroupName(const CString& strGroup) { m_strGroup = strGroup; }
	void SetSubject(const CString& strSubject) { m_strSubject = strSubject; }
	void SetMessageIDIndex(const int nIndex)
		{ m_nMessageID = nIndex; }   
	void SetAuthor(const CString& strAuthor) { m_strAuthor = strAuthor; }
	
	int GetNumArticles() { return m_nArticles; }
	void SetNumArticles(int nArticles) { m_nArticles = nArticles; }  

protected: // control bar embedded members
	CDialogBar m_wndRulerBar; // for a ruler along the top edge of the window
	CBitmapButton m_btnRuler;
	CDialogBar m_wndBtnBar;

// Operations
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child
    void OnUpdateFrameTitle(BOOL bAddToTitle = TRUE);
    
// Implementation
public:
	virtual ~CArticleWnd();
protected:

	// Generated message map functions
	//{{AFX_MSG(CArticleWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CArticleComposeBar dialog bar

class CArticleComposeBar : public CDialogBar
{ 
friend class CArticleComposeWnd;
// Construction
public:
	CArticleComposeBar();	// standard constructor for dialog bars 
	BOOL Create(CWnd* pParentWnd, LPCSTR lpszTemplateName,
		UINT nStyle, UINT nID);
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);

// Dialog Data
	//{{AFX_DATA(CArticleComposeBar)
	enum { IDD = IDD_NEWS_COMPOSE_DIALOG_BAR };
	CString	m_strSubject;
	CString	m_strNewsgroups;
	CString	m_strDistribution;
	CString	m_strFollowupTo;
	//}}AFX_DATA
	CWnd* m_pParentWnd;  // for WinHelp() call  
	CInternetNavApp* m_pApp;
	
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE);

	BOOL UpdateInfo() { return UpdateData(TRUE); }
    
    // Get/Set functions
    CString& GetSubject() { return m_strSubject; }
    CString& GetNewsgroups() { return m_strNewsgroups; }
    CString& GetDistribution() { return m_strDistribution; }
    CString& GetFollowupTo() { return m_strFollowupTo; }
    
    void SetSubject(CString& strSubject) { m_strSubject = strSubject; }
    void SetNewsgroups(CString& strNewsgroups)
    	{ m_strNewsgroups = strNewsgroups; }
    void SetDistribution(CString& strDistribution)
    	{ m_strDistribution = strDistribution; }
    void SetFollowupTo(CString& strFollowupTo)
    	{ m_strFollowupTo = strFollowupTo; }
    	    
    BOOL m_bFollowup;
    
    BOOL IsFollowup() { return m_bFollowup; }
    void SetFollowup(const BOOL bFollowup = TRUE)
    	{ m_bFollowup = bFollowup; }
	
	BOOL DoFollowup(CString& strNewsgroups,
		CString& strSubject, CString& strAuthor,
			CString& strDistribution);

// Implementation  
public:
	CNewsDoc* m_pDoc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CArticleComposeBar)
	afx_msg void OnEditChange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelp();
	afx_msg void OnUpdateHelp(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CArticleComposeWnd frame

class CArticleComposeWnd : public CMDIChildWnd
{  
friend class CArticleComposeView;
	DECLARE_DYNCREATE(CArticleComposeWnd)
protected:
	CArticleComposeWnd();			// protected constructor used by dynamic creation

// Attributes   
protected:
	CArticleComposeBar m_wndComposeBar;
	CBitmapButton m_btnRuler;
	CDialogBar m_wndRulerBar;
		
public:
	// The References, Newsgroups, Subject, Distribution, Keywords, Date, and
	// From headers
	CString		m_strReferences; // Other articles referenced by this one
	CString		m_strNewsgroups; // Newsgroups to post to
	CString		m_strDistribution; // Distribution of this article
	CString		m_strKeywords;	// Search keywords for this article
	CString		m_strDate;		// The date this article was posted
	CString		m_strFrom;		// Author of this article  
	CString		m_strReplyTo;   // To whom readers should reply
	CString		m_strFollowupTo; // Groups to which readers should post
									// followups
	CString		m_strSubject;
	
	// Functions to retrieve the above information
	CString& GetReferences() { return m_strReferences; }
	CString& GetNewsgroups() { return m_strNewsgroups; }
	CString& GetDistribution() { return m_strDistribution; }
	CString& GetKeywords() { return m_strKeywords; }
	CString& GetDate() { return m_strDate; }
	CString& GetAuthor() { return m_strFrom; } 
	CString& GetReplyTo() { return m_strReplyTo; }
	CString& GetFollowupTo() { return m_strFollowupTo; }
	CString& GetSubject() { return m_strSubject; }
	
	// Functions to set the above information
	void SetReferences(const CString& strReferences)
		{ m_strReferences = strReferences; }
	void SetNewsgroups(const CString& strNewsgroups)
		{ m_strNewsgroups = strNewsgroups; }        
	void SetReplyTo(const CString& strReplyTo)
		{ m_strReplyTo = strReplyTo; }
	void SetFollowupTo(const CString& strFollowupTo)
		{ m_strFollowupTo = strFollowupTo; }
	void SetDistribution(const CString& strDistribution)
		{ m_strDistribution = strDistribution; }
	void SetKeywords(const CString& strKeywords)
		{ m_strKeywords = strKeywords; }
	void SetDate(const CString& strDate)
		{ m_strDate = strDate; }
	void SetFrom(const CString& strFrom) 
		{ m_strFrom = strFrom; }   
	void SetSubject(const CString& strSubject)
		{ m_strSubject = strSubject; }

	CArticleComposeBar& GetArticleComposeBar() { return m_wndComposeBar; }
	BOOL UpdateBarInfo() { return m_wndComposeBar.UpdateInfo(); }
		
// Operations
public:    
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child
    void OnUpdateFrameTitle(BOOL bAddToTitle = TRUE);
    
    BOOL DoFollowup(CString& strNewsgroups, CString& strSubject,
    	CString& strAuthor, CString& strDistribution);
    	
// Implementation
protected:
	virtual ~CArticleComposeWnd();

	// Generated message map functions
	//{{AFX_MSG(CArticleComposeWnd)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__NEWS_H__