// percent.h : header file
//

#ifndef __PERCENT_H__
#define __PERCENT_H__

class CFtpView;
/////////////////////////////////////////////////////////////////////////////
// CPercentDialog percentage-gauge dialog box

class CPercentDialog : public CDialog
{
// Construction
public:
	CPercentDialog(CWnd* pParent = NULL);	// standard constructor
	void Initialize (CString, CString, COLORREF Color = RGB (0,0,128));
	void SetPercent (int nPercent);
	void SetULPercent (unsigned long int ulPercent);
	
	// Helper functions for percent
	void CalculatePercent(int nAmount, int nTotal);
	void CalculateULPercent(unsigned long int ulAmount,
		unsigned long int ulTotal);

// Attributes
public:
	CString mTitle;
	CString mText;
	CRect mBarRect;
	COLORREF mColor;
	int mCurrentPercent;
	unsigned long int m_ulCurrentPercent;
	BOOL m_bULONG;
	int BARX, BARY, BARWIDTH, BARHEIGHT;

// Dialog Data
	//{{AFX_DATA(CPercentDialog)
	enum { IDD = IDD_PERCENT };
	//}}AFX_DATA

// Implementation
public:
	virtual void PostNcDestroy();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CPercentDialog)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; 

/////////////////////////////////////////////////////////////////////////////
// CFTPercentDialog dialog: percentage-gauge dialog box for FTP file transfers

class CFTPercentDialog : public CDialog
{
// Construction
public:
	CFTPercentDialog(CWnd* pParent = NULL);	// standard constructor 
	void Initialize (CString strFilePath, CString strFileName, 
			CFtpView* pView, BOOL bUpload);
					
	void SetPercent (unsigned long int ulPercent);
	
	// Helper functions for percent
	void CalculatePercent(unsigned long int ulBytes,
		unsigned long int ulFileSize);

// Attributes
public:
	CRect mBarRect;
	unsigned long int m_ulCurrentPercent;
	int BARX, BARY, BARWIDTH, BARHEIGHT;
	
// Dialog Data 
public:
	//{{AFX_DATA(CFTPercentDialog)
	enum { IDD = IDD_FTP_PERCENT };
	CStatic	m_ctlBytes;
	CString	m_strFilePath;
	CString	m_strWaitText;
	//}}AFX_DATA
	CString	m_strFileName;
	CFtpView* m_pView;

// Implementation
public:
	virtual void PostNcDestroy();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFTPercentDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCancelCall();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam); 
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif //__PERCENT_H__