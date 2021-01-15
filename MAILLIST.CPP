// maillist.cpp: implementation file

#include "stdafx.h"
#include "inetnav.h"

#include "mail.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailListBox list box

BEGIN_MESSAGE_MAP(CMailListBox, CListBox)
	//{{AFX_MSG_MAP(CMailListBox)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMailListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// There's a HDC device context handle in the DRAWITEMSTRUCT that
	// we can use for drawing.  To make things easier on ourselves,
	// let's use CDC::FromHandle to get a CDC* pointer using the HDC handle
	// in the DRAWITEMSTRUCT structure
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);  

	COLORREF clrText, clrBk, clrOldText, clrOldBk;
	
	if (lpDrawItemStruct->itemID == -1) // nothing is selected 
	{
		// Draw the focus rectangle and return
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		return;                                     
	}	
	
	// Initialize colors		
	if (lpDrawItemStruct->itemAction & ODA_SELECT)	// sel status changed 
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED) // item is selected
		{
			// The current item is selected -- get the system's color
			// for highlighting and text
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		} 
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else // The current item isn't selected, but another item is
		{                                                           
			// Just get the system window and text color
			clrText = ::GetSysColor(COLOR_WINDOWTEXT);
			clrBk = ::GetSysColor(COLOR_WINDOW);
		}
		lpDrawItemStruct->itemAction |= ODA_DRAWENTIRE;
	}
	else if (lpDrawItemStruct->itemAction & ODA_FOCUS)
	{   
		// Current item may or may not have input focus
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// The current item is selected -- get the system's color
			// for highlighting and text
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			// The current item may be still selected let's take a gamble
			// here 
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else // The current item is not selected
		{
			clrText = ::GetSysColor(COLOR_WINDOWTEXT);
			clrBk = ::GetSysColor(COLOR_WINDOW);
		}
		lpDrawItemStruct->itemAction |= ODA_DRAWENTIRE;
	}				
	else
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// highlight and highlighttext colors
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
		else if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
		}
			
		clrText = ::GetSysColor(COLOR_WINDOWTEXT);
		clrBk = ::GetSysColor(COLOR_WINDOW);
	}
	
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) // draw whole control
	{
		CBitmap bmIcon;
		CString strItem;
		
		GetText(lpDrawItemStruct->itemID, strItem);
        
        // Load the icon for the online service from which this message
        // (item) was sent.  For America Online mail (the From address
        // contains "aol.com" or "AOL.COM" after the '@'), we load a 16x16
        // America Online icon.  For CompuServe mail (the From address
        // contains "compuserve.com" or "COMPUSERVE.COM" after the '@'),
        // we load a 16x16 version of the WinCIM icon.  For Prodigy mail 
        // (the From address contains "prodigy.com" or "PRODIGY.COM"), we
        // load a 16x16 version of the Prodigy icon.  If the mail is from
        // GNN (the From address contains "gnn.com" or "GNN.COM" after the 
        // '@') we load a 16x16 version of the GNNworks icon (to be added).
        if ((strItem.Find("aol.com") != -1) || 
        	(strItem.Find("AOL.COM") != -1))
        {
			// America Online mail message -- load the America Online icon
			// from the resource file to use for the item
			bmIcon.LoadBitmap(IDB_AMERICA_ONLINE);
		}
		else if ((strItem.Find("compuserve.com") != -1) ||
				 (strItem.Find("COMPUSERVE.COM") != -1))
		{
			// CompuServe mail message -- load the WinCIM icon from the
			// resource file to use for the item
			bmIcon.LoadBitmap(IDB_COMPUSERVE);
		}
		else if ((strItem.Find("prodigy.com") != -1) ||
				 (strItem.Find("PRODIGY.COM") != -1))
		{
			// Prodigy mail message -- load the Prodigy icon from the
			// resource file to use for the item
			bmIcon.LoadBitmap(IDB_PRODIGY);
		}
		else if ((strItem.Find("gnn.com") != -1) ||
				 (strItem.Find("GNN.COM") != -1))
		{  
			// GNN Mail Message -- load the GNN icon from the resource file
			// to use for the item bitmap
			bmIcon.LoadBitmap(IDB_GNN); // GNN icon bitmap
		}                                                  
		else
		{
			bmIcon.LoadBitmap(IDB_MAILFILE); // file bitmap
		}

       	clrOldText = pDC->SetTextColor(clrText); 
        clrOldBk = pDC->SetBkColor(clrBk);
        
        int nMode = pDC->SetBkMode(TRANSPARENT);
            
		CBrush brBk(clrBk);
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brBk);
		
		CBitmap* pBitmap = &bmIcon;
		
		// Draw bmIcon transparently onto the listbox
		BITMAP bitmap; // bitmap info structure
		DrawTransparentBitmap(pDC, pBitmap, bitmap,
			lpDrawItemStruct->rcItem.left + 5,
			lpDrawItemStruct->rcItem.top, RGB(255,255,255));
		
		// Now draw the text associated with the item 	
		GetText(lpDrawItemStruct->itemID, strItem);

		/*int lpint[1] = { 130 };*/
		int x = lpDrawItemStruct->rcItem.left + bitmap.bmWidth + 10;
		int y = lpDrawItemStruct->rcItem.top;  
		/*int nCount = strItem.GetLength();*/
			
		pDC->/*Tabbed*/TextOut(x, y, strItem/*, nCount, 1, lpint, x*/);		
							
		pDC->SetTextColor(clrOldText);
		pDC->SetBkColor(clrOldBk);
		
		pDC->SetBkMode(nMode);
		
		if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
				(lpDrawItemStruct->itemState & ODS_SELECTED))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);    
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
					(lpDrawItemStruct->itemState & ODS_SELECTED))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		else if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
					(lpDrawItemStruct->itemState & ODS_FOCUS))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
					(lpDrawItemStruct->itemState & ODS_FOCUS))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);      
			
		return;
	}
	
	if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);    
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
				(lpDrawItemStruct->itemState & ODS_SELECTED))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	else if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
				(lpDrawItemStruct->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
				(lpDrawItemStruct->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);      
}

void CMailListBox::DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap, 
	BITMAP& bitmap, short xStart, short yStart, COLORREF crTransparentColor)
{                                   
   COLORREF   crColor;
   CBitmap    bmAndBack, bmAndObject, bmAndMem, bmSave;
   CBitmap   *bmBackOld, *bmObjectOld, *bmMemOld, *bmSaveOld;
   CDC        memDC, backDC, objectDC, tempDC, saveDC;
   CPoint     ptSize;

   tempDC.CreateCompatibleDC(pDC);
   CBitmap* pOldTemp = tempDC.SelectObject(pBitmap);
   
   // Fill the BITMAP structure (which is a reference argument) with
   // bitmap information
   pBitmap->GetObject(sizeof(BITMAP), &bitmap);

   ptSize.x = bitmap.bmWidth;            // Get width of bitmap
   ptSize.y = bitmap.bmHeight;           // Get height of bitmap
   tempDC.DPtoLP(&ptSize, 1);      // Convert from device
                                     // to logical points

   // Create some DCs to hold temporary data.
   backDC.CreateCompatibleDC(pDC);
   objectDC.CreateCompatibleDC(pDC);
   memDC.CreateCompatibleDC(pDC);
   saveDC.CreateCompatibleDC(pDC);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC

   bmAndBack.CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject.CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem.CreateCompatibleBitmap(pDC, ptSize.x, ptSize.y);
   bmSave.CreateCompatibleBitmap(pDC, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = backDC.SelectObject(&bmAndBack);
   bmObjectOld = objectDC.SelectObject(&bmAndObject);
   bmMemOld    = memDC.SelectObject(&bmAndMem);
   bmSaveOld   = saveDC.SelectObject(&bmSave);

   // Set proper mapping mode.
   tempDC.SetMapMode(pDC->GetMapMode());

   // Save the bitmap sent here, because it will be overwritten.
   saveDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   crColor = tempDC.SetBkColor(crTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt()
   // from the source bitmap to a monochrome bitmap.
   objectDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0,
          SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   tempDC.SetBkColor(crColor);

   // Create the inverse of the object mask.
   backDC.BitBlt(0, 0, ptSize.x, ptSize.y, &objectDC, 0, 0,
          NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, pDC, xStart, yStart,
          SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, &objectDC, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   tempDC.BitBlt(0, 0, ptSize.x, ptSize.y, &backDC, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   memDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   pDC->BitBlt(xStart, yStart, ptSize.x, ptSize.y, &memDC, 0, 0,
          SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   tempDC.BitBlt(0, 0, ptSize.x, ptSize.y, &saveDC, 0, 0, SRCCOPY);
   
   CBitmap* pTemp = tempDC.SelectObject(pOldTemp);
   
   	// Delete the memory bitmaps.
   	pTemp = backDC.SelectObject(bmBackOld);
   	pTemp->DeleteObject();
   	
   	pTemp = objectDC.SelectObject(bmObjectOld);
   	pTemp->DeleteObject();
   	
   	pTemp = memDC.SelectObject(bmMemOld);
   	pTemp->DeleteObject();
    
    pTemp = saveDC.SelectObject(bmSaveOld);
    pTemp->DeleteObject();
    
   	// Delete the memory DCs.
   	memDC.DeleteDC();
   	backDC.DeleteDC();

   	objectDC.DeleteDC();
   	saveDC.DeleteDC();
	tempDC.DeleteDC();
}

void CMailListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 18;  
	return;
}  

void CMailListBox::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	CMenu popup;
	popup.CreatePopupMenu();
	if (GetCount() > 0)
	{
		popup.AppendMenu(MF_ENABLED|MF_STRING, ID_MAIL_DISPLAY_MESSAGE,
			"&Display Message\tENTER");
		popup.AppendMenu(MF_ENABLED|MF_STRING, ID_MAIL_DELETE,
			"Delete &Message\tDel");
		popup.AppendMenu(MF_ENABLED|MF_STRING, ID_MAIL_REPLY,
			"Re&ply to Message");
	}
	else if (GetCount() == 0)
	{
		popup.AppendMenu(MF_DISABLED|MF_STRING, ID_MAIL_DISPLAY_MESSAGE,
			"&Display Message\tENTER");
		popup.AppendMenu(MF_DISABLED|MF_STRING, ID_MAIL_DELETE,
			"Delete &Message\tDel");
		popup.AppendMenu(MF_DISABLED|MF_STRING, ID_MAIL_REPLY,
			"Re&ply to Message");
	}
	popup.AppendMenu(MF_SEPARATOR);
	popup.AppendMenu(MF_ENABLED|MF_STRING, ID_WINDOW_REFRESH,
		"&Refresh\tF5");

	ClientToScreen(&point);
	popup.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y,
				m_pMailView);
	return;
}