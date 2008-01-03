#pragma once

#define DT_MENU 0x8000
#define DT_MENUBAR 0x10000

class CCustomFont
{

protected:
	ATL::CImage m_imgFont;
	unsigned int m_nWidth[256];
	unsigned int m_nHeight;

public:
	CCustomFont();
	~CCustomFont();

	bool Load(HRSRC hRes);
	bool LoadMemory(BYTE* pMemoryImage, DWORD dwSize, DWORD imagetype = 0);

	unsigned int GetHeight();
	unsigned int GetWidth(LPCTSTR pszString, unsigned int nFlags);

	void GetStringDimensions(LPCTSTR pszString, CSize* psz);

	void DrawString(HDC hDC, LPCTSTR pszString, RECT* pRect, unsigned int nFlags);

protected:
	char XLAT(TCHAR ch);
	CString XLAT(CString str);

protected:
	static const short m_chXLATDef[256];
	static const short m_chXLATCyr[256];
};