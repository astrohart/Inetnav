// ircdoc.h : interface of the CIRClientDoc class
//

#ifndef __IRCDOC_H__
#define __IRCDOC_H__
#include "statbar.h"

/////////////////////////////////////////////////////////////////////////////

class CIRClientDoc : public CDocument
{        
friend class CIRClientView;
protected: // create from serialization only
	CIRClientDoc();
	DECLARE_DYNCREATE(CIRClientDoc)

// Attributes
public: 
	CIRClientView* m_pServerView;
		
private:
	CString m_strChannel;
	CInternetNavApp* m_pApp;
	CInetnavStatusBar* m_pStatusBar;  
	CString m_strWindowTitle; // is serialized to *.IRC files  
	BOOL m_bChangeNick;

// Operations
protected:
	virtual void OnChangedViewList();
public: 
	virtual void SetTitle(const char* pszTitle);
	CString GetChannelName() { return m_strChannel; }
	void SetChannelName(const CString strName) { m_strChannel = strName; }
	
	void PrintChatTranscript();
	
	CIRClientView* GetServerView() { return m_pServerView; } 

// Implementation
public:
	virtual ~CIRClientDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();
	virtual BOOL SaveModified(); 

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CIRClientDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnChatChangeGroup();
	afx_msg void OnChatPrivateMessage();
	afx_msg void OnChatChangeNickname();
	afx_msg void OnFileClose();
	afx_msg void OnUpdateChatChangeNickname(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChatChangeGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChatPrivateMessage(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif	// __TEXTDOC_H__
