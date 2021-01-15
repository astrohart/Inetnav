// gopher.h: definitions of classes used by Internet Navigator for Internet
// Gopher

#ifndef __GOPHER_H__
#define __GOPHER_H__
#include "statbar.h"

/////////////////////////////////////////////////////////////////////////////
// CGopherListBox list box -- a list box which shows a folder icon next to
// a link to a directory a file icon next to a document link, a hand lens
// icon next to a search link, and a blank file icon next to any other kind
// of link that this application can't follow.

class CGopherListBox : public CListBox
{
// drawing methods
public: 
	// For drawing listbox bitmaps transparent
	virtual void DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap,
			BITMAP& bitmap, short xStart, short yStart,
				COLORREF crTransparentColor);
				
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	
	// Generated message map functions
	//{{AFX_MSG(CGopherListBox)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGopherDoc document

class CGopherDoc : public CDocument
{         
friend class CGopherMenu;
	DECLARE_DYNCREATE(CGopherDoc)
protected:
	CGopherDoc();			// protected constructor used by dynamic creation

// Attributes
public: 
	CString m_strMenuName;		// title of menu
	CString m_strSiteAddress;	// address of server
	CString m_strSelector;		// selector path of item on server
	CString	m_strQuery; 		// search query string
	
	UINT m_nPortNum;			// port number of Gopher menu  
	
	BOOL m_bTopLevelMenu;		// is this the first menu the user opened?
	BOOL m_bNewMenu;			// set from OnNewDocument()
	
	CInternetNavApp* m_pApp;	// pointer to the application
	
public:
	BOOL IsTopLevelMenu() { return m_bTopLevelMenu; }

// Operations
public:    
	BOOL ProcessSelector(const char* pszSelector, BOOL bSearch);                 

// Implementation
protected:
	virtual ~CGopherDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual	BOOL OnNewDocument(); 
	virtual BOOL OnOpenDocument(const char* pszSelector); 
	virtual void OnCloseDocument();   
	virtual void SetPathName( const char* pszPathName, BOOL bAddToMRU = TRUE );

	// Generated message map functions
protected:
	//{{AFX_MSG(CGopherDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnGopherSiteProperties();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGopherMenu form view

class CGopherMenu : public CFormView
{        
friend class CInternetNavApp;
friend class CInetnavStatusBar;
	DECLARE_DYNCREATE(CGopherMenu)
protected:
	CGopherMenu();			// protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CGopherMenu)
	enum { IDD = IDR_GOPHERMENU }; 
	CBitmapButton m_btnView;
	CBitmapButton m_btnSearch; 
	CBitmapButton m_btnHelp;
	CBitmapButton m_btnBack;
	CString	m_strSelection;
	//}}AFX_DATA 
	CGopherListBox m_menuList;

// Attributes
public:                
	CGopherDoc* GetDocument();
	HCURSOR m_hCursorOld;
	CInternetNavApp* m_pApp; 
	CInetnavStatusBar* m_pStatusBar;
private:
	QSocket* m_pSocket;

// Operations
public:
	virtual void OnInitialUpdate();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Implementation
protected:
	virtual ~CGopherMenu();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	// Generated message map functions
	//{{AFX_MSG(CGopherMenu)       
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintSetup(CCmdUI* pCmdUI);
	afx_msg void OnView();
	afx_msg void OnClickedHelp();
	afx_msg void OnDblclkMenuList();
	afx_msg void OnSearch();
	afx_msg void OnClose();
	afx_msg void OnUpdateFilePageSetup(CCmdUI* pCmdUI); 
	afx_msg void OnWindowRemember();
	afx_msg void OnSelchangeMenuList();
	afx_msg void OnUpdateGopherSearch(CCmdUI* pCmdUI);
	afx_msg void OnCancelCall();
	//}}AFX_MSG
	afx_msg LRESULT OnGetGopherMenu(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in textview.cpp
inline CGopherDoc* CGopherMenu::GetDocument()
   { return (CGopherDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CGopherWnd frame

class CGopherWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGopherWnd)
protected:
	CGopherWnd();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:     
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child

// Implementation
protected:
	virtual ~CGopherWnd();

	// Generated message map functions
	//{{AFX_MSG(CGopherWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COutputDoc document

class COutputDoc : public CDocument
{             
friend class COutputView;
protected: // create from serialization only
	COutputDoc();
	DECLARE_DYNCREATE(COutputDoc)

// Attributes
public:  
	// Host, Selector, Port, and Title information for Gopher
	CString m_strDocTitle, m_strDocSelector, m_strDocHost;
	UINT m_nDocPort;
	
// Operations
public:  
	virtual void SetPathName(const char* pszPathName, BOOL bAddToMRU = TRUE);

// Implementation
public:
	virtual ~COutputDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();
	virtual BOOL SaveModified();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();   
	virtual BOOL OnOpenDocument(const char* pszSelector);
	virtual BOOL OnSaveDocument(const char* pszPathName);

// Generated message map functions
protected:
	//{{AFX_MSG(COutputDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COutputView text view

class COutputView : public CEditView
{ 
friend class CInternetNavApp;
friend class CInetnavStatusBar;
protected: // create from serialization only
	COutputView(); 
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_DYNCREATE(COutputView)

// Attributes
public:                  
	HCURSOR m_hCursorOld;
	// static init/term...
	static void Initialize();
	static void Terminate();

	COutputDoc* GetDocument();    
	CInternetNavApp* m_pApp;  
	CInetnavStatusBar* m_pStatusBar;

private:
	QSocket* m_pSocket;

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

// Implementation
public:           
	BOOL IsUpdatePending() {return (m_uTimerID != NULL);}

	virtual ~COutputView();
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

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo); 
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnScrollTo(CDC* pDC, CPrintInfo* pInfo, POINT point);

// Generated message map functions
protected:
	//{{AFX_MSG(COutputView)
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
	//}}AFX_MSG
	afx_msg LRESULT OnSocketResponse(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in gopher.cpp
inline COutputDoc* COutputView::GetDocument()
   { return (COutputDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputWnd frame

class COutputWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(COutputWnd)
protected:
	COutputWnd();			// protected constructor used by dynamic creation

// Attributes
public:  
protected: // control bar embedded members
	CDialogBar m_wndRulerBar; // for a ruler along the top edge of this window
	CBitmapButton m_btnRuler; 

// Operations
public:        
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child

// Implementation
protected:
	virtual ~COutputWnd();

	// Generated message map functions
	//{{AFX_MSG(COutputWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewRuler();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGopherSiteProperties dialog

class CGopherSiteProperties : public CDialog
{
// Construction
public:
	CGopherSiteProperties(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGopherSiteProperties)
	enum { IDD = IDD_GOPHER_SITE_PROPERTIES };
	CString	m_strServer;
	CString	m_strPath;
	CString	m_strName;
	UINT	m_nPort;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CGopherSiteProperties)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGophCenterDoc document

class CGophCenterDoc : public CDocument
{
	DECLARE_SERIAL(CGophCenterDoc)
protected:
	CGophCenterDoc();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual void SetTitle(const char* pszTitle);
	
// Implementation
protected:
	virtual ~CGophCenterDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual	BOOL OnNewDocument();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGophCenterDoc)
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGopherCenter form view

class CGopherCenter : public CFormView
{
	DECLARE_DYNCREATE(CGopherCenter)
protected:
	CGopherCenter();			// protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CGopherCenter)
	enum { IDD = IDR_GOPHCENTERTYPE };
	CBitmapButton	m_btnSearch;
	CBitmapButton	m_btnOpenSite;
	CBitmapButton	m_btnMsgBoard;
	CBitmapButton	m_btnHelp;
	CBitmapButton	m_btnInfo;
	CInternetNavApp*	m_pApp;
	//}}AFX_DATA     
	
	// Pointer to status bar
	CInetnavStatusBar* m_pStatusBar;

// Attributes
public:

// Operations
public:     
	#ifdef _DEBUG
		CGophCenterDoc* GetDocument();
	#endif
	
	virtual void OnInitialUpdate();

// Implementation
protected:
	virtual ~CGopherCenter();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif
	
	// Generated message map functions
	//{{AFX_MSG(CGopherCenter)
	afx_msg void OnInformationAboutGopher();
	afx_msg void OnOpenSite();
	afx_msg void OnMessageBoard();
	afx_msg void OnClickedHelp();
	afx_msg void OnGopherSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()   
	
#ifndef _DEBUG  // debug version in gopher.cpp
inline CGophCenterDoc* CGopherCenter::GetDocument()
   { return (CGophCenterDoc*)m_pDocument; }
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CGophCenterWnd frame

class CGophCenterWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGophCenterWnd)
protected:
	CGophCenterWnd();			// protected constructor used by dynamic creation

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
	virtual ~CGophCenterWnd();

	// Generated message map functions
	//{{AFX_MSG(CGophCenterWnd)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__GOPHER_H__