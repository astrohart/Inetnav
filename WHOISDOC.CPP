// whoisdoc.cpp : implementation of the CWhoisDoc class
//

#include "stdafx.h"
#include "inetnav.h"

#include "whoisdoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhoisDoc 

IMPLEMENT_SERIAL(CWhoisDoc, CDocument, 1)

BEGIN_MESSAGE_MAP(CWhoisDoc, CDocument)
	//{{AFX_MSG_MAP(CWhoisDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhoisDoc construction/destruction

CWhoisDoc::CWhoisDoc()
{    
	//{{AFX_DATA_INIT(CWhoisDoc)
	m_strNIC = ""; // main Network Information Center (NIC)
	m_strHost = "";
	m_strOutput = "";
	//}}AFX_DATA_INIT
	
	m_pApp = (CInternetNavApp*)AfxGetApp();
}

/*
//{{AFX_DATA_MAP(CWhoisDoc)
//}}AFX_DATA_MAP
*/

CWhoisDoc::~CWhoisDoc()
{
}

BOOL CWhoisDoc::OnNewDocument()
{
	// Load the Network Information Center text box with the address
	// of DS.INTERNIC.NET, the main database of hosts on the Internet	                                     
	m_strNIC.LoadString(IDS_DS_INTERNIC_NET);
	
	CString strTitle = "InterNIC Site Information";
	SetTitle(strTitle);           
	SetModifiedFlag(FALSE); 
	
	if (!CDocument::OnNewDocument())
		return FALSE;             
	return TRUE;
}

BOOL CWhoisDoc::OnOpenDocument(const char* pszPathName)
{
	if (!CDocument::OnOpenDocument(pszPathName))   
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC, MB_ICONSTOP|MB_OK);
		return FALSE;
    }
    
	CString strTitle = "InterNIC Site Information: ";
	strTitle += GetTitle(); 

	SetTitle(strTitle);   

	return TRUE;
}

BOOL CWhoisDoc::SaveModified()
{
	if (!IsModified())
		return TRUE;        // ok to continue

	CString strName = "the information you have obtained from the InterNIC";
	CString prompt = "";
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, strName);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (_access(m_strPathName, 6) != 0)
		{
			if (!CDocument::DoSave(NULL))
				return FALSE;   // don't continue
		}
		else
		{
			if (!CDocument::DoSave(m_strPathName))
				return FALSE;   // don't continue
		}
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}

void CWhoisDoc::SetTitle(const char* /*pszTitle*/)
{
	CString strTitle = "InterNIC Site Information";
	if (m_strHost.IsEmpty() && m_strNIC.IsEmpty())
	{
		// set this window's (and document's) title to say,
		// "InterNIC Site Information".
		m_strTitle = strTitle;
		UpdateFrameCounts();
		return;
	}
	else if (m_strHost.IsEmpty() || m_strNIC.IsEmpty())
	{
		m_strTitle = strTitle;
		UpdateFrameCounts();
		return;
	}
	else if (!m_strHost.IsEmpty() && !m_strNIC.IsEmpty())
	{
		m_strTitle = strTitle;
		m_strTitle += ": ";
		m_strTitle += m_strHost;
		UpdateFrameCounts();
		return;
	}
	else
	{
		m_strTitle = strTitle;
		UpdateFrameCounts();
		return;
	}
}
               	
/////////////////////////////////////////////////////////////////////////////
// CWhoisDoc serialization

void CWhoisDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strNIC;
		ar << m_strHost;
		ar << m_strOutput;
	}
	else
	{
		ar >> m_strNIC;
		ar >> m_strHost;
		ar >> m_strOutput;
	}
}

void CWhoisDoc::DeleteContents()
{
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CWhoisDoc diagnostics

#ifdef _DEBUG
void CWhoisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWhoisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWhoisDoc commands

void CWhoisDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	// Only enable the File, Save menu item if the document data has
	// changed
	pCmdUI->Enable(IsModified());
	return;
}

void CWhoisDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
