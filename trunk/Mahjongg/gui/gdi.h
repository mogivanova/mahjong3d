/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: gdi.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:47 $
*
*================================================================================================*/

#pragma once

inline BOOL TransparentBlt2(HDC hDestDC, int nX, int nY, int nW, int nH,
														HDC hSrcDC, int nSX, int nSY, COLORREF rgbBackground)
{
	HDC hdcBack = ::CreateCompatibleDC(hDestDC);
	HDC hdcObject = ::CreateCompatibleDC(hDestDC);
	HDC hdcMem = ::CreateCompatibleDC(hDestDC);
	HDC hdcSave = ::CreateCompatibleDC(hDestDC);

	HBITMAP bmAndBack = ::CreateBitmap(nW, nH, 1, 1, NULL);
	HBITMAP bmAndObject = ::CreateBitmap(nW, nH, 1, 1, NULL);
	HBITMAP bmAndMem = ::CreateCompatibleBitmap(hDestDC, nW, nH);
	HBITMAP bmSave = ::CreateCompatibleBitmap(hDestDC, nW, nH);

	HBITMAP bmBackOld = (HBITMAP)::SelectObject(hdcBack, bmAndBack);
	HBITMAP bmObjectOld = (HBITMAP)::SelectObject(hdcObject, bmAndObject);
	HBITMAP bmMemOld = (HBITMAP)::SelectObject(hdcMem, bmAndMem);
	HBITMAP bmSaveOld = (HBITMAP)::SelectObject(hdcSave, bmSave);

	::BitBlt(hdcSave, 0, 0, nW, nH, hSrcDC, nSX, nSY, SRCCOPY);

	COLORREF cColor = ::SetBkColor(hSrcDC, rgbBackground);
	::BitBlt(hdcObject, 0, 0, nW, nH, hSrcDC, nSX, nSY, SRCCOPY);
	::SetBkColor(hSrcDC, cColor);

	::BitBlt(hdcBack, 0, 0, nW, nH, hdcObject, 0, 0, NOTSRCCOPY);

	::BitBlt(hdcMem, 0, 0, nW, nH, hDestDC, nX, nY, SRCCOPY);

	::BitBlt(hdcMem, 0, 0, nW, nH, hdcObject, 0, 0, SRCAND);

	::BitBlt(hSrcDC, nSX, nSY, nW, nH, hdcBack, 0, 0, SRCAND);

	::BitBlt(hdcMem, 0, 0, nW, nH, hSrcDC, nSX, nSY, SRCPAINT);

	::BitBlt(hDestDC, nX, nY, nW, nH, hdcMem, 0, 0, SRCCOPY);

	::BitBlt(hSrcDC, nSX, nSY, nW, nH, hdcSave, 0, 0, SRCCOPY);

	::DeleteObject(::SelectObject(hdcBack, bmBackOld));
	::DeleteObject(::SelectObject(hdcObject, bmObjectOld));
	::DeleteObject(::SelectObject(hdcMem, bmMemOld));
	::DeleteObject(::SelectObject(hdcSave, bmSaveOld));

	::DeleteDC(hdcMem);
	::DeleteDC(hdcBack);
	::DeleteDC(hdcObject);
	::DeleteDC(hdcSave);

	return TRUE;
}