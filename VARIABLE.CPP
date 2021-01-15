
	// Mailbox management
	BOOL AddNewMailbox();	// creates a new mailbox
	void RegisterMailbox(const CString& strName, const CString& strPath);
	void RemoveMailbox(const CString& strName, const CString& strPath);
	
	BOOL OpenDefaultMailbox()
	void SetDefaultMailbox(const CString& strName, const CString& strPath,
		const CString& strSMTPServer);
	CString GetDefaultSMTPServer();
		
	CString GetMailboxName(const CString& strPath);
	CString GetMailboxPath(const CString& strName);
	
	CStringArray GetMailboxNames();
	CStringArray GetMailboxPaths(); 
	
	BOOL LoadMailboxes();
	BOOL SaveMailboxes();
	
	// Map of mailboxes -- maps mailbox names to pathnames and back
	CMapStringToString m_mapMailboxNameToPath;
	CMapStringToString m_mapMailboxPathToName;   
	
	CStringArray 	m_strMailboxPathList;
	CStringArray	m_strMailboxNameList;
	
	// Other mailbox variables
	CString m_strDefMailboxName, m_strDefMailboxPath;	