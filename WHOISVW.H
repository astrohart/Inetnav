// whoisvw.h : header file
//

#ifndef __WHOISVW_H__
#define __WHOISVW_H__
#include "percent.h"
/////////////////////////////////////////////////////////////////////////////
// CWhoisView form view

class CWhoisView : public CFormView
{             
friend class CWhoisDoc; 
friend class CWhoisWnd;
	DECLARE_DYNCREATE(CWhoisView)
protected:
	CWhoisView();			// protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CWhoisView)
	enum { IDD = IDR_WHOISTYPE };
	CEdit	m_ctlHost;
	CEdit	m_ctlNIC;
	CEdit	m_ctlOutput;
	CStatic	m_ctlIcon;
	CButton	m_btnLookup;
	CButton	m_btnHelp;
	CButton	m_btnClear;
	CButton	m_btnCancel;
	CWhoisDoc*	m_pDoc;
	//}}AFX_DATA
	BOOL m_bUndo; // for undo and redo

// Attributes
public:     
	CWhoisDoc* GetDocument(); 
private:
	QSocket* m_pSocket;
	CInternetNavApp* m_pApp;
	CInetnavStatusBar* m_pStatusBar;
	CPercentDialog* m_pPercentDialog;
	
	BOOL m_bSuccess;

// Operations
public:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	
// UI helpers
public:              
	void EmptyUndoFlags(); // resets undo buffers on edit controls
		
// Implementation
public:
	virtual ~CWhoisView();
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

// Misc. helper functions
public:
	void OnFinishReceive();	// called upon completion of query
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CWhoisView)
	afx_msg void OnLookupCancel();
	afx_msg void OnClear();
	afx_msg void OnClickedHelp();
	afx_msg void OnLookup();
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLookup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLookupCancel(CCmdUI* pCmdUI);
	afx_msg void OnScrollLineDown();
	afx_msg void OnScrollLineUp();
	afx_msg void OnScrollPageDown();
	afx_msg void OnScrollPageUp();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnWindowRemember();
	afx_msg void OnEditChange();
	//}}AFX_MSG
	afx_msg LRESULT OnSocketResponse(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version is in whoisvw.cpp
inline CWhoisDoc* CWhoisView::GetDocument()
   { return (CWhoisDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
#endif //__WHOISVW_H__
