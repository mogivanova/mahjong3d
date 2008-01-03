/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: font.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 14.05.04 0:21 $
*
*================================================================================================*/

#include "stdafx.h"
#include "font.h"
#include "gdi.h"

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
CCustomFont::CCustomFont()
{
	for (int i = 0; i < 256; i++)
	{
		m_nWidth[i] = 12;
	}

	m_nHeight = 0;
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
CCustomFont::~CCustomFont()
{

}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
bool CCustomFont::Load(HRSRC hRes)
{
	if (!m_imgFont.LoadResource(hRes, CXIMAGE_FORMAT_BMP, _Module.m_hInstResource))
		return false;

	BYTE* pBits = m_imgFont.GetBits();

	unsigned int nWidth = m_imgFont.GetWidth() / 16;

	unsigned int nHeight = m_imgFont.GetHeight() / 16;

	for (int nCh = 0; nCh < 256; nCh++)
	{
		int nChX = (nCh % 16);
		int nChY = (nCh / 16);

		m_nWidth[nCh] = 0;

		for (unsigned int y = 0; y < nHeight; y++)
		{
			for (unsigned int x = 0; x < nWidth; x++)
			{
				int nPx = (nChX * nWidth + x);
				int nPy = (nChY * nHeight + y + 1); // yes, it works!

				int idx = m_imgFont.GetWidth() * m_imgFont.GetHeight() * 3 -
									(nPy * m_imgFont.GetWidth() - nPx) * 3;

				COLORREF px = RGB(pBits[idx + 2], pBits[idx + 1], pBits[idx]);

				if (px != RGB(255, 0, 255))
				{
					if (x > m_nWidth[nCh])
						m_nWidth[nCh] = x;
				}
			}
		}

		if (m_nWidth[nCh] > 0)
			m_nWidth[nCh]++;
	}

	m_nHeight = nHeight;

	return true;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CCustomFont::LoadMemory(BYTE* pMemoryImage, DWORD dwSize, DWORD imagetype)
{
	if (!m_imgFont.Decode(pMemoryImage, dwSize, imagetype))
		return false;

	BYTE* pBits = m_imgFont.GetBits();

	unsigned int nWidth = m_imgFont.GetWidth() / 16;

	unsigned int nHeight = m_imgFont.GetHeight() / 16;

	for (int nCh = 0; nCh < 256; nCh++)
	{
		int nChX = (nCh % 16);
		int nChY = (nCh / 16);

		m_nWidth[nCh] = 0;

		for (unsigned int y = 0; y < nHeight; y++)
		{
			for (unsigned int x = 0; x < nWidth; x++)
			{
				int nPx = (nChX * nWidth + x);
				int nPy = (nChY * nHeight + y + 1); // yes, it works!

				int idx = m_imgFont.GetWidth() * m_imgFont.GetHeight() * 3 -
									(nPy * m_imgFont.GetWidth() - nPx) * 3;

				COLORREF px = RGB(pBits[idx + 2], pBits[idx + 1], pBits[idx]);

				if (px != RGB(255, 0, 255))
				{
					if (x > m_nWidth[nCh])
						m_nWidth[nCh] = x;
				}
			}
		}

		if (m_nWidth[nCh] > 0)
			m_nWidth[nCh]++;
	}

	m_nHeight = nHeight;

	return true;
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
unsigned int CCustomFont::GetHeight()
{
	return m_nHeight;
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
unsigned int CCustomFont::GetWidth(LPCTSTR pszString, unsigned int nFlags)
{
	CString str = XLAT(pszString);

	int nWidth = 0;

	bool bMenu = (nFlags & DT_MENU) == DT_MENU;

	for (int i = 0; i < str.GetLength(); i++)
	{
		unsigned int nChar = (unsigned char)str.GetAt(i);

		if (bMenu && nChar == _T('&'))
			continue;

		if (nChar == _T(' '))
			nWidth += m_nWidth['o'];
		else
			nWidth += m_nWidth[nChar] + 1;
	}

	return nWidth;
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
void CCustomFont::GetStringDimensions(LPCTSTR pszString, CSize* psz)
{
	psz->cx = GetWidth(pszString, 0);
	psz->cy = GetHeight();
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
void CCustomFont::DrawString(HDC hDC, LPCTSTR pszString, RECT* pRect, unsigned int nFlags)
{
	CString str = XLAT(pszString);

	// create temporary dc
	CDC dcMem;
	dcMem.CreateCompatibleDC(hDC);

	CBitmap bmpFont;
	bmpFont.CreateCompatibleBitmap(hDC, m_imgFont.GetWidth(), m_imgFont.GetHeight());

	HBITMAP hOldBmp = (HBITMAP)dcMem.SelectBitmap(bmpFont);

	// set font
	m_imgFont.Draw(dcMem, CRect(CPoint(0, 0), CSize(m_imgFont.GetWidth(), m_imgFont.GetHeight())), NULL);

	bool bMenu = (nFlags & DT_MENU) == DT_MENU;

	int nX = pRect->left;

	if ((nFlags & DT_CENTER) == DT_CENTER)
	{
		// centered text
		nX = pRect->left +  ((pRect->right - pRect->left) - GetWidth(pszString, nFlags)) / 2;
	}
	else if ((nFlags & DT_RIGHT) == DT_RIGHT)
	{
		//
		nX = pRect->right - GetWidth(pszString, nFlags);
	}

	int nY = pRect->top;

	if ((nFlags & DT_VCENTER) == DT_VCENTER)
	{
		nY = pRect->top + ((pRect->bottom - pRect->top) - m_nHeight) / 2;
	}
	else
	{

	}

	bool bUnderlined = false;

	for (int i = 0; i < str.GetLength(); i++)
	{
		unsigned int nCh = (unsigned char)str.GetAt(i);

		if (nCh == _T(' '))
		{
			nX += m_nWidth['o'];
			continue;
		}

		if (bMenu && nCh == _T('&'))
		{
			bUnderlined = true;
			continue;
		}

		int nChX = (nCh % 16) * (m_imgFont.GetWidth() / 16);

		int nChY = (nCh / 16) * (m_imgFont.GetHeight() / 16);

		::TransparentBlt2(hDC, nX, nY, 12, 12, dcMem, nChX, nChY, RGB(255, 0, 255));

		if (bUnderlined)
		{
			int nChX = (((int)_T('_')) % 16) * (m_imgFont.GetWidth() / 16);
			int nChY = (((int)_T('_')) / 16) * (m_imgFont.GetHeight() / 16);

			::TransparentBlt2(hDC, nX, nY, 12, 12, dcMem, nChX, nChY, RGB(255, 0, 255));

			bUnderlined = false;
		}

		nX += m_nWidth[nCh] + 1;
	}

	// clean up
	dcMem.SelectBitmap(hOldBmp);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
const short CCustomFont::m_chXLATDef[256] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0x84, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0x96, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0x9C, 0xDD, 0xDE, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
};

const short CCustomFont::m_chXLATCyr[256] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x85, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xAB, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
char CCustomFont::XLAT(TCHAR ch)
{
	switch (g_LangManager.GetCodePage())
	{

	case CLangManager::eDefault:
		return (unsigned char)m_chXLATDef[ch];

	case CLangManager::eCyrillic:
		return (unsigned char)m_chXLATCyr[ch];
	}

	return _T('?');
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CString CCustomFont::XLAT(CString str)
{
	for (int i = 0; i < str.GetLength(); i++)
	{
		unsigned char ch = (unsigned char)str.GetAt(i);

		switch (g_LangManager.GetCodePage())
		{

		case CLangManager::eDefault:
			ch = (unsigned char)m_chXLATDef[ch];
			break;

		case CLangManager::eCyrillic:
			ch = (unsigned char)m_chXLATCyr[ch];
			break;

		default:
			ch = _T('?');
			break;
		}

		str.SetAt(i, ch);
	}

	return str;
}