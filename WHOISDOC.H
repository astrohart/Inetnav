// whoisdoc.h : header file
//

#ifndef __WHOISDOC_H__
#define __WHOISDOC_H__
/////////////////////////////////////////////////////////////////////////////
// CWhoisDoc document

class CWhoisDoc : public CDocument
{ 
friend class CWhoisView;
protected: // create from serialization only
	CWhoisDoc();
	DECLARE_SERIAL(CWhoisDoc)

// Attributes
public: 
	//{{AFX_DATA(CWhoisDoc)
	CString m_strNIC;
	CString m_strHost;
	CString m_strOutput;
	//}}AFX_DATA
	
	CInternetNavApp* m_pApp;

// Operations
public: 
	virtual void SetTitle(const char* pszTitle);

// Implementation
protected:
	virtual ~CWhoisDoc(); 
	
	// implementation helpers
	BOOL DoSave(const char* pszPathName, BOOL bReplace = TRUE);
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual void DeleteContents();      
	virtual BOOL SaveModified(); // overridden for custom save prompt
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif       

protected:
	virtual	BOOL OnNewDocument(); 
	virtual BOOL OnOpenDocument(const char* pszPathName);

	// Generated message map functions
protected:
	//{{AFX_MSG(CWhoisDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__WHOISDOC_H__

