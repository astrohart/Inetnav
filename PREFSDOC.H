// prefsdoc.h : header file
//

#ifndef __PREFSDOC_H__
#define __PREFSDOC_H__

/////////////////////////////////////////////////////////////////////////////
// CPrefsDoc document

class CPrefsDoc : public CDocument
{
	DECLARE_SERIAL(CPrefsDoc)
protected:
	CPrefsDoc();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Implementation
protected:
	virtual ~CPrefsDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
	virtual	BOOL OnNewDocument();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPrefsDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////
#endif //__PREFSDOC_H__
