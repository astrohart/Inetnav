
void DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap, short xStart,
                           short yStart, COLORREF crTransparentColor)
{
	BITMAP     bm;
   	COLORREF   crColor;
   	CBitmap    bmAndBack, bmAndObject, bmAndMem, bmSave;
   	CBitmap    *bmBackOld, *bmObjectOld, *bmMemOld, *bmSaveOld;
   	CDC        memDC, backDC, objectDC, tempDC, saveDC;
   	CPoint      ptSize;

   	tempDC.CreateCompatibleDC(pDC);
   	CBitmap* pOldTempBitmap = tempDC.SelectObject(pBitmap); 
   	// Select the bitmap

   	pBitmap->GetObject(sizeof(BITMAP), &bm);

	ptSize.x = bm.bmWidth;            // Get width of bitmap
   	ptSize.y = bm.bmHeight;           // Get height of bitmap
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
   	bmBackOld = backDC.SelectObject(&bmAndBack);
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
   	objectDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0, SRCCOPY);

   	// Set the background color of the source DC back to the original
   	// color.
   	tempDC.SetBkColor(crColor);

	// Create the inverse of the object mask.
   	backDC.BitBlt(0, 0, ptSize.x, ptSize.y, &objectDC, 0, 0, NOTSRCCOPY);

   	// Copy the background of the main DC to the destination.
   	memDC.BitBlt(0, 0, ptSize.x, ptSize.y, pDC, xStart, yStart, SRCCOPY);

   	// Mask out the places where the bitmap will be placed.
   	memDC.BitBlt(0, 0, ptSize.x, ptSize.y, &objectDC, 0, 0, SRCAND);

   	// Mask out the transparent colored pixels on the bitmap.
   	tempDC.BitBlt(0, 0, ptSize.x, ptSize.y, &backDC, 0, 0, SRCAND);

   	// XOR the bitmap with the background on the destination DC.
   	memDC.BitBlt(0, 0, ptSize.x, ptSize.y, &tempDC, 0, 0, SRCPAINT);

   	// Copy the destination to the screen.
   	pDC->BitBlt(xStart, yStart, ptSize.x, ptSize.y, &memDC, 0, 0, SRCCOPY);

   	// Place the original bitmap back into the bitmap sent here.
   	tempDC.BitBlt(0, 0, ptSize.x, ptSize.y, &saveDC, 0, 0, SRCCOPY);
   
   	CBitmap *pBack, *pObj, *pSave, *pMem;
	   
   	// Delete the memory bitmaps.
   	pBack = backDC.SelectObject(bmBackOld);
   	pBack->DeleteObject();
   
   	pObj = objectDC.SelectObject(bmObjectOld);
   	pObj->DeleteObject();
 
   	pSave = saveDC.SelectObject(bmSaveOld);
   	pSave->DeleteObject();
   
   	pMem = memDC.SelectObject(bmMemOld);
   	pMem->DeleteObject();     
   
   	tempDC.SelectObject(pOldTempBitmap);
   
   	// Delete the memory DCs.
   	memDC.DeleteDC();
   	backDC.DeleteDC();

   	objectDC.DeleteDC();
   	saveDC.DeleteDC();
   	tempDC.DeleteDC();
   	
   	return;
}

