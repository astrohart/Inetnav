// tipofday.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTipOfDay dialog

class CTipOfDay : public CDialog
{
// Construction
public:
  CTipOfDay(const int nIndex, const BOOL bShowTipAtStartup, CWnd* pParent = NULL); // standard constructor
  int GetIndex(void);
  BOOL GetShowTipsAtStartup(void);

// Dialog Data
	//{{AFX_DATA(CTipOfDay)
	enum { IDD = IDD_TIP_OF_THE_DAY };
	BOOL	m_bShowTips;
	CString	m_strTipText;
	//}}AFX_DATA
	
private:
	int	m_nIndex; // index of current tip

// Implementation
protected:            
	/*virtual void OnSetFont(CFont* pFont);*/
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CTipOfDay)
	afx_msg void OnNextTip();
	afx_msg void OnPrevTip();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
