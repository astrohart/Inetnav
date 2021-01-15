// appd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAppFileDialog dialog

class CAppFileDialog : public CFileDialog
{
// Construction
public:
	CAppFileDialog(BOOL bOpenFileDialog = TRUE, LPCSTR lpszDefExt = NULL, LPCSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, LPCSTR lpszFilter = NULL,
	CWnd* pParentWnd = NULL);
	CWnd* m_pParent;
 
// Initialization
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV Support	

	// Generated message map functions
	//{{AFX_MSG(CAppFileDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
