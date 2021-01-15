	/*delete m_pPercentDialog;
	m_pPercentDialog = new CPercentDialog(m_pApp->GetMainFrame());
	m_pPercentDialog->Initialize("Sending Mail Message",
		"Internet Navigator is telling the mail server who you "
		"want the message sent to...  Please wait.");
	m_pPercentDialog->SetPercent(0);
	for (int a = 0; a < m_strToList.GetSize(); a++)
	{
		m_strTo = m_strToList[a];
		if (!m_strTo.IsEmpty() && m_strTo.Find(' ') == 0)
			m_strTo = m_strTo.Right(m_strTo.GetLength() - 1);
		strSend = "RCPT TO: <" + m_strTo + ">\r\n";
		m_pSocket->Send(strSend);
		strSend.Empty();
		response = m_pSocket->GetLine(); 
		if ((atoi(response.Left(3))) == 251)
		{ 
			CString strMsg = m_strTo + ": ";
			strMsg += response.Right(response.GetLength() - 4);
			AfxMessageBox(strMsg,
				MB_ICONINFORMATION|MB_OK);      
		}
		else if ((atoi(response.Left(3))) == 550)
		{
			CString strMsg = m_strTo + ": ";
			strMsg += response.Right(response.GetLength() - 4);
			AfxMessageBox(strMsg,
				MB_ICONEXCLAMATION|MB_OK);
			return 0;
		} 
									
		q = 100 / m_strToList.GetSize();
		m_pPercentDialog->SetPercent(a * q);
		continue;
	}       
	q = 0;
	m_pPercentDialog->SetPercent(100);
								
	if (bCC)
	{
		m_pPercentDialog->SetPercent(0);
		for (int n = 0; n < m_strCCList.GetSize(); n++)
		{
			m_strCC = m_strCCList[n];
			if (!m_strCC.IsEmpty() && m_strCC.Find(' ') == 0)
				m_strTo = m_strTo.Right(m_strTo.GetLength() - 1);
			strSend = "RCPT TO: <" + m_strCC + ">\r\n";
			m_pSocket->Send(strSend);
			strSend.Empty();
			response = m_pSocket->GetLine();
			if ((atoi(response.Left(3))) == 251)
			{ 
				CString strMsg = m_strTo + ": ";
				strMsg += response.Right(response.GetLength() - 4);
				AfxMessageBox(strMsg,
					MB_ICONINFORMATION|MB_OK);      
			}
			else if ((atoi(response.Left(3))) == 550)
			{
				CString strMsg = m_strTo + ": ";
				strMsg += response.Right(response.GetLength() - 4);
				AfxMessageBox(strMsg,
					MB_ICONEXCLAMATION|MB_OK);    
				return 0;
			} 							
										
			q = 100 / m_strCCList.GetSize();
			m_pPercentDialog->SetPercent(n * q);		
			continue;
		}   
		q = 0;
		m_pPercentDialog->SetPercent(100);
	}
								
	if (bBCC)
	{
		for (int c = 0; c < m_strBCCList.GetSize(); c++)
		{
			m_strBCC = m_strBCCList[c];
			if (!m_strBCC.IsEmpty() && m_strBCC.Find(' ') == 0)
				m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - 1);
			strSend = "RCPT TO: <" + m_strBCC + ">\r\n";
			m_pSocket->Send(strSend);
			strSend.Empty();
			response = m_pSocket->GetLine();
			if ((atoi(response.Left(3))) == 251)
			{ 
				CString strMsg = m_strTo + ": ";
				strMsg += response.Right(response.GetLength() - 4);
				AfxMessageBox(strMsg,
					MB_ICONINFORMATION|MB_OK);
			}
			else if ((atoi(response.Left(3))) == 550)
			{
				CString strMsg = m_strTo + ": ";
				strMsg += response.Right(response.GetLength() - 4);
				AfxMessageBox(strMsg,
					MB_ICONEXCLAMATION|MB_OK);
				return 0;		
			}                           
										
			q = 100 / m_strBCCList.GetSize();
			m_pPercentDialog->SetPercent(c * q);
			continue;
		}
		q = 0;
		m_pPercentDialog->SetPercent(100);
	} 
								
	m_pPercentDialog->SetPercent(100);
	m_pPercentDialog->DestroyWindow();
	delete m_pPercentDialog;
	m_pPercentDialog = NULL; */ 
	
void CComposeView::ProccessMailRecipients()
{   
	// Clear out the lists
	m_strToList.RemoveAll();
	m_strCCList.RemoveAll();
	m_strBCCList.RemoveAll();
	
	m_strTo = m_pFrame->m_wndInfoBar.GetTO();
	m_strCC = m_pFrame->m_wndInfoBar.GetCC();
	m_strBCC = m_pFrame->m_wndInfoBar.GetBCC();
	
	CString strCurTo = "", strCurCC = "", strCurBCC = "";
	
	int nComma, nSpace, i;
	 
	m_pStatusBar->SetText(IDS_PROCESSING_TO);
	if ((nComma = m_strTo.Find(',')) == -1)
	{
		m_strToList.Add(m_strTo);
	}
	else                         
	{
		m_pStatusBar->SetText(IDS_PROCESSING_TO);
		strCurTo = m_strTo.Left(nComma);
		m_strTo = m_strTo.Right(m_strTo.GetLength() - nComma - 1);
		if (m_strTo.Left(1) == " ")
			m_strTo = m_strTo.Right(m_strTo.GetLength() - 1);
			
		// Add the first recipient to our list
		m_strToList.Add(strCurTo);
			
		while ((nComma = m_strTo.Find(',')) != -1)
		{   
			m_pStatusBar->SetText(IDS_PROCESSING_TO);
			// As long as the list has commas in it, keep
			// on parsing
			strCurTo = m_strTo.Left(nComma);
			m_strToList.Add(strCurTo);
				
			m_strTo = m_strTo.Right(m_strTo.GetLength() - nComma - 1);
			if (m_strTo.Left(1) == " ")    
				m_strTo = m_strTo.Right(m_strTo.GetLength() - 1);
		}
			
		if ((nComma = m_strTo.Find('@')) != -1)
		{   
			m_pStatusBar->SetText(IDS_PROCESSING_TO);         
			// After this final address, there's nothing left
			m_strToList.Add(m_strTo);
		}
	}      
	
	if (!m_strCC.IsEmpty())
	{
		m_pStatusBar->SetText(IDS_PROCESSING_CC);
		if ((nComma = m_strCC.Find(',')) == -1)
		{        
			m_pStatusBar->SetText(IDS_PROCESSING_CC);
			m_strCCList.Add(m_strCC);
		}
		else                         
		{  
			m_pStatusBar->SetText(IDS_PROCESSING_CC);
			strCurCC = m_strCC.Left(nComma);
			m_strCC = m_strCC.Right(m_strCC.GetLength() - nComma - 1);
			if (m_strCC.Left(1) == " ")
				m_strCC = m_strCC.Right(m_strCC.GetLength() - 1);
			
			// Add the first recipient to our list
			m_strCCList.Add(strCurCC);
			
			while ((nComma = m_strCC.Find(',')) != -1)
			{
				m_pStatusBar->SetText(IDS_PROCESSING_CC);
				// As long as the list has commas in it, keep
				// on parsing
				strCurCC = m_strCC.Left(nComma);
				m_strCCList.Add(strCurCC);
				
				m_strCC = m_strCC.Right(m_strCC.GetLength() - nComma - 1);
				if (m_strCC.Left(1) == " ")    
					m_strCC = m_strCC.Right(m_strCC.GetLength() - 1);
			}
			
			if ((nComma = m_strCC.Find('@')) != -1)
			{  
				m_pStatusBar->SetText(IDS_PROCESSING_CC);          
				// After this final address, there's nothing left
				m_strToList.Add(m_strCC);
			}
		}	
	}
	
	if (!m_strBCC.IsEmpty())
	{  
		m_pStatusBar->SetText(IDS_PROCESSING_BCC);
		if ((nComma = m_strBCC.Find(',')) == -1)
		{                      
			m_pStatusBar->SetText(IDS_PROCESSING_BCC);
			m_strBCCList.Add(m_strBCC);
		}
		else                         
		{
			m_pStatusBar->SetText(IDS_PROCESSING_BCC);
			strCurBCC = m_strBCC.Left(nComma);
			m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - nComma - 1);
			if (m_strBCC.Left(1) == " ")
				m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - 1);
			
			// Add the first recipient to our list
			m_strBCCList.Add(strCurBCC);
			
			while ((nComma = m_strBCC.Find(',')) != -1)
			{  
				m_pStatusBar->SetText(IDS_PROCESSING_BCC);
				// As long as the list has commas in it, keep
				// on parsing
				strCurBCC = m_strBCC.Left(nComma);
				m_strBCCList.Add(strCurBCC);
				
				m_strBCC = m_strBCC.Right(m_strBCC.GetLength() - nComma - 1);
				if (m_strBCC.Left(1) == " ")    
					m_strBCC = m_strBCC.Right(m_strCC.GetLength() - 1);
			}
			
			if ((nComma = m_strBCC.Find('@')) != -1)
			{
				m_pStatusBar->SetText(IDS_PROCESSING_BCC);            
				// After this final address, there's nothing left
				m_strBCCList.Add(m_strBCC);
			}
		}	
	}
	
	// Now iterate through the To, CC, and BCC recipient arrays
	// and send a 'RCPT TO:' command to the server for each one
	CString response; 
	
	m_pStatusBar->SetText(IDS_SENDING_TO);
	for (i = 0;i < m_strToList.GetSize();i++)
	{ 
		m_pStatusBar->SetText(IDS_SENDING_TO);
	    m_pSocket->Send("RCPT TO: <" + m_strToList[i] + ">\r\n");
	    response = m_pSocket->GetLine(); 
	}
	
	m_pStatusBar->SetText(IDS_SENDING_CC);
	for (i = 0;i < m_strCCList.GetSize();i++)
	{   	
		m_pStatusBar->SetText(IDS_SENDING_CC);
		m_pSocket->Send("RCPT TO: <" + m_strCCList[i] + ">\r\n");
		response = m_pSocket->GetLine();
	}
	
	m_pStatusBar->SetText(IDS_SENDING_BCC);
	for (i = 0;i < m_strBCCList.GetSize();i++)
	{     
		m_pStatusBar->SetText(IDS_SENDING_BCC);
		m_pSocket->Send("RCPT TO: <" + m_strBCCList[i] + ">\r\n");
		response = m_pSocket->GetLine();
	}
	
	m_pStatusBar->ShowIdleMessage();
	// we're done! 
	return;
}