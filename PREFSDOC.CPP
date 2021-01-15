// prefsdoc.cpp : implementation file
//

#include "stdafx.h"
#include "inetnav.h"
#include "prefsdoc.h"
#include "prefview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefsDoc

IMPLEMENT_SERIAL(CPrefsDoc, CDocument, 0 /* schema number*/ )

CPrefsDoc::CPrefsDoc()
{
}

BOOL CPrefsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPrefsDoc::~CPrefsDoc()
{
}


BEGIN_MESSAGE_MAP(CPrefsDoc, CDocument)
	//{{AFX_MSG_MAP(CPrefsDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPrefsDoc serialization

void CPrefsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPrefsDoc commands

void CPrefsDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CPrefsDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}
