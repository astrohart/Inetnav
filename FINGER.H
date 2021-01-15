// finger.h - definitions of the classes which make up the Internet Users
// Information area of Internet Navigator.  For implementations of the classes
// below, see finger.cpp

#ifndef __FINGER_H__
#define __FINGER_H__  
#include "percent.h"
/////////////////////////////////////////////////////////////////////////////

class CFingerDoc : public CDocument
{
protected: // create from serialization only
	CFingerDoc();
	DECLARE_DYNCREATE(CFingerDoc)

// Attributes
public:    
	//{{AFX_DATA(CFingerDoc)
	CString m_strHost;
	CString m_strUser; 
	CString m_strOutput;
	//}}AFX_DATA 
	CInternetNavApp* m_pApp;
	BOOL m_bFirst;      
	CString m_strAddress;
	
// Operations
public:
	virtual void SetTitle(const char* pszTitle);

// Implementation
public:
	virtual ~CFingerDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();                        
	virtual BOOL SaveModified();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument(); 
	virtual BOOL OnOpenDocument(const char* pszPathName);

// Generated message map functions
protected:
	//{{AFX_MSG(CFingerDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class CInetnavStatusBar;

class CFingerView : public CFormView
{  
friend class CInternetNavApp;
friend class CFingerDoc;  
friend class CFingerWnd;
protected: // create from serialization only
	CFingerView();
	DECLARE_DYNCREATE(CFingerView)

public:
	//{{AFX_DATA(CFingerView)
	enum { IDD = IDR_FINGERTYPE };
	CButton	m_btnHelp;
	CEdit	m_ctlUser;
	CEdit	m_ctlOutput;
	CEdit	m_ctlHost;
	CButton	m_btnLookup;
	CButton	m_btnClear;
	CButton	m_btnCancel;
	CFingerDoc*	m_pDoc;
	//}}AFX_DATA
	
	BOOL m_bUndo; // for Undo and Redo

// Attributes
public:
	CFingerDoc* GetDocument(); 
	CInetnavStatusBar* GetStatusBar() { return m_pStatusBar; }   
	CInternetNavApp* GetApp() { return m_pApp; }
private:
	QSocket* m_pSocket; 
	CInetnavStatusBar* m_pStatusBar;
	CInternetNavApp* m_pApp;
	CPercentDialog* m_pPercentDialog;
	
	BOOL m_bSuccess; // Was info retrieved successfully?

// Operations
public: 
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL);

// UI helpers
public:              
	void EmptyUndoFlags(); // resets undo buffers on edit controls 
	void ShowMsg(CString strMsg) { m_pStatusBar->SetText(strMsg); }
	void ShowMsg(UINT nIDMsg) { m_pStatusBar->SetText(nIDMsg); }
	
// Implementation
public:
	virtual ~CFingerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	void PrintPageHeader(CDC* pDC, CPrintInfo* pInfo, CString& strHeader);

// Callback functions
public:
	void OnFinishReceive();
	
// Generated message map functions
protected:
	//{{AFX_MSG(CFingerView)
	afx_msg void OnLookup();
	afx_msg void OnClear();
	afx_msg void OnClickedHelp();
	afx_msg void OnEditChange();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnLookupCancel();
	afx_msg void OnUpdateLookupCancel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLookup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnScrollLineDown();
	afx_msg void OnScrollLineUp();
	afx_msg void OnScrollPageDown();
	afx_msg void OnScrollPageUp();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnSocketResponse(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in finger.cpp
inline CFingerDoc* CFingerView::GetDocument()
   { return (CFingerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
// CFingerWnd frame

class CFingerWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFingerWnd)
protected:
	CFingerWnd();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:    
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
					CWnd* pParentWnd, CCreateContext* pContext = NULL);
					// 'pParentWnd' parameter is required for MDI Child

// Implementation
protected:
	virtual ~CFingerWnd();

	// Generated message map functions
	//{{AFX_MSG(CFingerWnd)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__FINGER_H__