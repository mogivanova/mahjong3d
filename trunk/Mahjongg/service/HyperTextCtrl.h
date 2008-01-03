/********************************************************************


		HyperTextCtrl.h - Controls that shows hyperlinks
		in text

		Copyright (C) 2001-2002 Magomed G. Abdurakhmanov

		History:

		11.06.2002
			+ CHyperTextCtrlFSB - uses flat scroll bars
			+ SetTextColor, SetBkColor
			- bugs fixed

		08.06.2002
			+ Now CHyperTextCtrl inherits from CHyperTextCtrlT<>

		07.06.2002
			+ .cpp file removed. Now all code is in HyperTextCtrl.h
			- bugs fixed
			* Tested on WinXP, VC++7, WTL7

		01.06.2001
			+ Control was created.
			* Tested on Win2K, VC++6 SP3, WTL3.1


********************************************************************/

#ifndef MAQ_HYPERTEXTCONTROL_H
#define MAQ_HYPERTEXTCONTROL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <list>
#include <vector>

namespace HyperTextControl
{

#define HTC_WORDWRAP			1	// word wrap text
#define HTC_AUTO_SCROLL_BARS	2	// auto hide scroll bars
#define HTC_UNDERLINE_LINKS		4	// underline links
#define HTC_UNDERLINE_HOVER		8	// underline hover links
#define HTC_ENABLE_TOOLTIPS		16	// enable hyperlink tool tips

// --------------------------------------------------------------
// CHyperLink

class CHyperLink
{
	int m_iBegin;
	int m_iEnd;
	CString m_sTitle;

	enum LinkType
	{
		lt_Shell = 0, /* http:// mailto:*/
		lt_Message = 1 /* WM_COMMAND */
	} m_Type;

	// used for lt_Shell
	CString m_sCommand;
	CString m_sDirectory;

	// used for lt_Message
	HWND m_hWnd;
	UINT m_uMsg;
	WPARAM m_wParam;
	LPARAM m_lParam;

public:

	CHyperLink(int iBegin, int iEnd, const CString& sTitle, const CString& sCommand, const CString& sDirectory)
	{
		m_Type = lt_Shell;
		m_iBegin = iBegin;
		m_iEnd = iEnd;
		m_sTitle = sTitle;
		m_sCommand = sCommand;
		m_sDirectory = sDirectory;
	}

	CHyperLink(int iBegin, int iEnd, const CString& sTitle, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		m_Type = lt_Message;
		m_iBegin = iBegin;
		m_iEnd = iEnd;
		m_sTitle = sTitle;
		m_hWnd = hWnd;
		m_uMsg = uMsg;
		m_wParam = wParam;
		m_lParam = lParam;
	}

	CHyperLink(const CHyperLink& Src)
	{
		m_Type = Src.m_Type;
		m_iBegin = Src.m_iBegin;
		m_iEnd = Src.m_iEnd;
		m_sTitle = Src.m_sTitle;
		m_sCommand = Src.m_sCommand;
		m_sDirectory = Src.m_sDirectory;
		m_hWnd = Src.m_hWnd;
		m_uMsg = Src.m_uMsg;
		m_wParam = Src.m_wParam;
		m_lParam = Src.m_lParam;
	}

	void Execute()
	{
		switch (m_Type)
		{

		case lt_Shell:
			ShellExecute(NULL, NULL, m_sCommand.c_str(), NULL, m_sDirectory.c_str(), SW_SHOWDEFAULT);
			break;

		case lt_Message:
			PostMessage(m_hWnd, m_uMsg, m_wParam, m_lParam);
			break;
		}
	}

	inline bool operator < (const CHyperLink& Arg)
	{
		return m_iEnd < Arg.m_iEnd;
	}

	inline int Begin()
	{
		return m_iBegin;
	}

	inline int End()
	{
		return m_iEnd;
	}

	inline int Len()
	{
		return m_iEnd - m_iBegin + 1;
	}

	inline CString Title()
	{
		return m_sTitle;
	}

	friend class CPreparedHyperText;
};

// --------------------------------------------------------------
// CPreparedHyperText

class CPreparedHyperText
{

protected:
	CString m_sText;
	std::list<CHyperLink> m_Links;

	inline void RemoveLastSign(CString& sLink)
	{
		int len = sLink.length();

		if (len > 0)
		{
			TCHAR c = sLink[len-1];

			switch (c)
			{
			case _T('.'):
						case _T(','):
							case _T(';'):
								case _T('\"'):
									case _T('\''):
										case _T('('):
											case _T(')'):
												case _T('['):
													case _T(']'):
														case _T('{'):
															case _T('}'):
																	sLink = sLink.substr(1, len - 1);
				break;
			}
		}
	}

	void PrepareText(const CString& sText)
{
		m_sText = sText;
		m_Links.clear();

		enum
		{
			unknown,
			space,
			http0,		/* http://		*/
			http1, http2, http3, http4, http5, http6,
			ftp0,		/* ftp://		*/
			ftp1, ftp2, ftp3, ftp4, ftp5,
			ftp,		/* ftp.			*/
			www0,		/* www.			*/
			www1, www2, www3,
			mailto0, 	/* mailto:		*/
			mailto1, mailto2, mailto3, mailto4, mailto5, mailto6,
			mail		/* xxx@yyy		*/
		} state = space;

		int WordPos = 0;
		TCHAR sz[2];
		TCHAR& c = sz[0];
		sz[1] = 0;
		int last = m_sText.length() - 1;

		for (int i = 0; i <= last; i++)
		{
			c = m_sText[i];
			_tcslwr(sz);

			switch (state)
			{

			case unknown:

				if (tspace(c))
					state = space;
				else
					if (c == _T('@') && WordPos != i)
						state = mail;

				break;

			case space:
				WordPos = i;

				switch (c)
				{
				case _T('h'): state = http0;
					break;
				case _T('f'): state = ftp0;
					break;
				case _T('w'): state = www0;
					break;
				case _T('m'): state = mailto0;
					break;

				default:

					if (!tspace(c))
						state = unknown;
				}

				break;

				/*----------------- http -----------------*/

			case http0:

				if (c == _T('t'))
					state = http1;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case http1:
				if (c == _T('t'))
					state = http2;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case http2:
				if (c == _T('p'))
					state = http3;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case http3:
				if (c == _T(':'))
					state = http4;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case http4:
				if (c == _T('/'))
					state = http5;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case http5:
				if (c == _T('/'))
					state = http6;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case http6:
				if (tspace(c) || i == last)
				{
					int len = i == last ? i - WordPos + 1 : i - WordPos;
					CString s = m_sText.substr(WordPos, len);
					RemoveLastSign(s);
					m_Links.push_back(CHyperLink(WordPos, WordPos + len - 1, s, s, (LPCTSTR)NULL));
					state = space;
				}

				break;

				/*----------------- ftp -----------------*/

			case ftp0:

				if (c == _T('t'))
					state = ftp1;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case ftp1:
				if (c == _T('p'))
					state = ftp2;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case ftp2:
				if (c == _T(':'))
					state = ftp3;
				else
					if (c == _T('.'))
						state = ftp;
					else
						if (tspace(c))
							state = space;
						else
							state = unknown;

				break;

			case ftp3:
				if (c == _T('/'))
					state = ftp4;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case ftp4:
				if (c == _T('/'))
					state = ftp5;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case ftp:
				if (tspace(c) || i == last)
				{
					int len = i == last ? i - WordPos + 1 : i - WordPos;
					CString s = CString(_T("ftp://")) + m_sText.substr(WordPos, len);
					RemoveLastSign(s);
					m_Links.push_back(CHyperLink(WordPos, WordPos + len - 1, s, s, (LPCTSTR)NULL));
					state = space;
				}

				break;

			case ftp5:

				if (tspace(c) || i == last)
				{
					int len = i == last ? i - WordPos + 1 : i - WordPos;
					CString s = m_sText.substr(WordPos, len);
					RemoveLastSign(s);
					m_Links.push_back(CHyperLink(WordPos, WordPos + len - 1, s, s, (LPCTSTR)NULL));
					state = space;
				}

				break;

				/*----------------- www -----------------*/

			case www0:

				if (c == _T('w'))
					state = www1;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case www1:
				if (c == _T('w'))
					state = www2;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case www2:
				if (c == _T('.'))
					state = www3;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case www3:
				if (tspace(c) || i == last)
				{
					int len = i == last ? i - WordPos + 1 : i - WordPos;
					CString s = CString(_T("http://")) + m_sText.substr(WordPos, len);
					RemoveLastSign(s);
					m_Links.push_back(CHyperLink(WordPos, WordPos + len - 1, s, s, (LPCTSTR)NULL));
					state = space;
				}

				break;

				/*----------------- mailto -----------------*/

			case mailto0:

				if (c == _T('a'))
					state = mailto1;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case mailto1:
				if (c == _T('i'))
					state = mailto2;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case mailto2:
				if (c == _T('l'))
					state = mailto3;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case mailto3:
				if (c == _T('t'))
					state = mailto4;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case mailto4:
				if (c == _T('o'))
					state = mailto5;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case mailto5:
				if (c == _T(':'))
					state = mailto6;
				else
					if (tspace(c))
						state = space;
					else
						state = unknown;

				break;

			case mailto6:
				if (tspace(c) || i == last)
				{
					int len = i == last ? i - WordPos + 1 : i - WordPos;
					CString s = m_sText.substr(WordPos, len);
					RemoveLastSign(s);
					m_Links.push_back(CHyperLink(WordPos, WordPos + len - 1, s, s, (LPCTSTR)NULL));
					state = space;
				}

				break;

				/*----------------- mailto -----------------*/

			case mail:

				if (tspace(c) || i == last)
				{
					int len = i == last ? i - WordPos + 1 : i - WordPos;
					CString s = CString(_T("mailto:")) + m_sText.substr(WordPos, len);
					RemoveLastSign(s);
					m_Links.push_back(CHyperLink(WordPos, WordPos + len - 1, s, s, (LPCTSTR)NULL));
					state = space;
				}

				break;
			}
		}

		m_Links.sort();
	}

	bool tspace(TCHAR c)
	{
		return _istspace(c) || c < _T(' ') || c == _T(';') || c == _T(',') || c == _T('!');
	}

public:

	CPreparedHyperText()
	{
	}

	CPreparedHyperText(const CString& sText)
	{
		PrepareText(sText);
	}

	CPreparedHyperText(const CPreparedHyperText& src)
	{
		m_sText = src.m_sText;
		m_Links.assign(src.m_Links.begin(), src.m_Links.end());
	}

	void Clear()
	{
		m_sText.erase();
		m_Links.erase(m_Links.begin(), m_Links.end());
	}

	void SetText(const CString& sText)
	{
		Clear();
		PrepareText(sText);
	}

	void AppendText(const CString& sText)
	{
		int len = m_sText.length();
		CPreparedHyperText ht(sText);
		m_sText += sText;

		for (std::list<CHyperLink>::iterator it = ht.m_Links.begin(); it != ht.m_Links.end(); it++)
		{
			CHyperLink hl = *it;
			hl.m_iBegin += len;
			hl.m_iEnd += len;
			m_Links.push_back(hl);
		}
	}

	void AppendHyperLink(const CString& sText, const CString& sTitle, const CString& sCommand, const CString& sDirectory)
	{
		ATLASSERT(sText.length() > 0);
		ATLASSERT(sCommand.length() > 0);

		int len = m_sText.length();
		m_sText += sText;
		m_Links.push_back(CHyperLink(len, len + sText.length() - 1, sTitle, sCommand, sDirectory));
	}

	void AppendHyperLink(const CString& sText, const CString& sTitle, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ATLASSERT(sText.length() > 0);

		int len = m_sText.length();
		m_sText += sText;
		m_Links.push_back(CHyperLink(len, len + sText.length() - 1, sTitle, hWnd, uMsg, wParam, lParam));
	}

	inline CString& GetText()
	{
		return m_sText;
	}

	inline std::list<CHyperLink>& GetLinks()
	{
		return m_Links;
	}

	//friend class CHyperTextCtrl;
};

// --------------------------------------------------------------
// CHyperTextCtrlT

template <typename T>

class CHyperTextCtrlT : public T
{

protected:

	class CLinePartInfo
	{

	public:
		int m_iBegin;
		int m_iEnd;
		CHyperLink* m_pHyperLink;

		inline CLinePartInfo(int iBegin, int iEnd, CHyperLink* pHyperLink = NULL)
		{
			m_iBegin = iBegin;
			m_iEnd = iEnd;
			m_pHyperLink = pHyperLink;
		}

		inline CLinePartInfo(const CLinePartInfo& Src)
		{
			m_iBegin = Src.m_iBegin;
			m_iEnd = Src.m_iEnd;
			m_pHyperLink = Src.m_pHyperLink;
		}

		inline int Begin()
		{
			return m_iBegin;
		}

		inline int End()
		{
			return m_iEnd;
		}

		inline int Len()
		{
			return m_iEnd - m_iBegin + 1;
		}
	};

class CLineInfo : public CSimpleArray<CLinePartInfo>
	{

	public:
		int m_iBegin;
		int m_iEnd;

		inline CLineInfo(int iBegin, int iEnd)
		{
			m_iBegin = iBegin;
			m_iEnd = iEnd;
		}

		inline CLineInfo(const CLineInfo& Src)
		{
			m_iBegin = Src.m_iBegin;
			m_iEnd = Src.m_iEnd;
			assign(Src.begin(), Src.end());
		}

		inline int Begin()
		{
			return m_iBegin;
		}

		inline int End()
		{
			return m_iEnd;
		}

		inline int Len()
		{
			return m_iEnd - m_iBegin + 1;
		}
	};

class CVisPart : public CLinePartInfo
	{

	public:
		CRect m_rcBounds;
		int m_iRealBegin;
		int m_iRealLen;
		CVisPart* m_pPrev;
		CVisPart* m_pNext;

		inline CVisPart(
			const CLinePartInfo& LinePartInfo,
			const CRect& rcBounds,
			int iRealBegin,
			int iRealLen,
			CVisPart* pPrev,
			CVisPart* pNext
		) : CLinePartInfo(LinePartInfo)
		{
			m_rcBounds = rcBounds;
			m_iRealBegin = iRealBegin;
			m_iRealLen = iRealLen;
			m_pPrev = pPrev;
			m_pNext = pNext;
		}

		inline CVisPart(const CVisPart& Src) : CLinePartInfo(Src)
		{
			m_rcBounds = Src.m_rcBounds;
			m_iRealBegin = Src.m_iRealBegin;
			m_iRealLen = Src.m_iRealLen;
			m_pPrev = Src.m_pPrev;
			m_pNext = Src.m_pNext;
		}
	};

class CVisLine : public CSimpleArray<CVisPart>
		{	};


	CPreparedHyperText m_Text;
	CSimpleArray<CLineInfo> m_Lines;
	CFontHandle m_Font;
	COLORREF m_BkColor;
	COLORREF m_TextColor;
	COLORREF m_LinkColor;
	COLORREF m_HoverColor;
	HCURSOR m_LinkCursor;
	HCURSOR m_DefaultCursor;

	CToolTipCtrl m_tip;

	//temporary variables
	int m_iMaxWidth;				// The maximum line width
	int m_iLineHeight;				// Height of one line
	int m_iLinesHeight;				// Sum of height of all lines
	bool m_bDontUpdateSizeInfo;		// Used to prevent recursive call of the UpdateSize() method
	int m_iVertPos;					// Vertical position in percents
	int m_iHorzPos;					// Horizontal position in percents
	CFont m_DefaultFont;			// This font is set by default
	CFont m_LinksFont;				// Copied from main font to faster draw (link)
	CFont m_HoverFont;				// Copied from main font to faster draw (hover link)
	CSimpleArray<CVisLine> m_VisLines;	// Currently visible text
	CVisPart* m_pActivePart;		// Active part of link (hovered)
	int m_iWheelDelta;				// Mouse wheel scroll delta

	/*void Activate()
	{
		if(GetParent() == NULL)
			SetActiveWindow();
		else
		{
			HWND hwndParent = GetParent();
			HWND hwndParentNext = ::GetParent(hwndParent);
			while(hwndParentNext != NULL)
			{
				hwndParent = hwndParentNext;
				hwndParentNext = ::GetParent(hwndParent);
			}

			::SetActiveWindow(hwndParent);
		}
	}*/

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return FALSE;
	}

	//message handlers
	LRESULT OnCreate(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)lParam;
		m_iMaxWidth = 0;
		m_iLinesHeight = 0;
		m_bDontUpdateSizeInfo = false;
		m_iHorzPos = 0;
		m_iVertPos = 0;
		m_DefaultFont.CreatePointFont(100, _T("Times New Roman"));
		m_Font = m_DefaultFont;
		m_LinkColor = RGB(0, 0, 255);
		m_HoverColor = RGB(255, 0, 0);
		m_BkColor = GetSysColor(COLOR_WINDOW);
		m_TextColor = GetSysColor(COLOR_WINDOWTEXT);
		m_LinkCursor = LoadCursor(NULL, IDC_ARROW);
		m_DefaultCursor = LoadCursor(NULL, IDC_ARROW);
		m_pActivePart = NULL;
		m_iWheelDelta = 0;

		// create a tool tip
		m_tip.Create(*this);
		ATLASSERT(m_tip.IsWindow());

		if (m_tip.IsWindow())
			m_tip.Activate(TRUE);

		UpdateFonts();

		return 0;
	}

	LRESULT OnPaint(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CPaintDC dc(m_hWnd); // device context for painting

		CFontHandle hOldFont = dc.SelectFont(m_Font);

		dc.SetBkColor(m_BkColor);

		int ypos = 0;
		LPCTSTR s = m_Text.GetText().c_str();

		CRect rc;
		CRect rcClient;
		GetClientRect(&rcClient);
		rc.left = dc.m_ps.rcPaint.left;
		rc.right = 2;
		rc.top = dc.m_ps.rcPaint.top;
		rc.bottom = dc.m_ps.rcPaint.bottom;

		CBrush brBk;
		brBk.CreateSolidBrush(m_BkColor);
		dc.FillRect(&rc, brBk);

		for (CSimpleArray<CVisLine>::iterator it = m_VisLines.begin(); it != m_VisLines.end(); it++)
		{
			int iLastX = dc.m_ps.rcPaint.left;

			for (CVisLine::iterator jt = it->begin(); jt != it->end(); jt++)
			{
				if (jt->m_pHyperLink == NULL)
					dc.SetTextColor(m_TextColor);
				else
				{
					if (m_pActivePart != NULL && m_pActivePart->m_pHyperLink == jt->m_pHyperLink)
					{
						dc.SetTextColor(m_HoverColor);
						dc.SelectFont(m_HoverFont);
					}
					else
					{
						dc.SetTextColor(m_LinkColor);
						dc.SelectFont(m_LinksFont);
					}
				}

				TextOut(dc, jt->m_rcBounds.left, jt->m_rcBounds.top, s + jt->m_iRealBegin, jt->m_iRealLen);

				if (jt->m_pHyperLink != NULL)
					dc.SelectFont(m_Font);

				iLastX = jt->m_rcBounds.right;
			}

			rc.left = iLastX;

			rc.right = dc.m_ps.rcPaint.right;
			rc.top = ypos;
			rc.bottom = ypos + m_iLineHeight;

			dc.FillRect(&rc, brBk);

			ypos += m_iLineHeight;
		}

		rc.left = dc.m_ps.rcPaint.left;

		rc.right = dc.m_ps.rcPaint.right;
		rc.top = ypos;
		rc.bottom = dc.m_ps.rcPaint.bottom;
		dc.FillRect(&rc, brBk);

		dc.SelectFont(hOldFont);
		return 0;
	}

	LRESULT OnSize(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		WORD cx, cy;
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);

		UpdateSize(IsWindowVisible() == TRUE);
		return 0;
	}

	LRESULT OnShowWindow(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (TRUE == (BOOL)wParam)
			UpdateSize(false);

		return 0;
	}

	LRESULT OnSetText(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Text.SetText((LPTSTR)lParam);
		UpdateSize(IsWindowVisible() == TRUE);
		return TRUE;
	}

	LRESULT OnGetText(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int bufsize = wParam;
		LPTSTR buf = (LPTSTR)lParam;

		if (lParam == NULL || bufsize == 0 || m_Text.GetText().empty())
			return 0;

		int cpy = m_Text.GetText().length() > (bufsize - 1) ? (bufsize - 1) : m_Text.GetText().length();

		_tcsncpy(buf, m_Text.GetText().c_str(), cpy);

		return cpy;
	}

	LRESULT OnSetFont(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_Font = (HFONT)wParam;
		UpdateFonts();
		UpdateSize(LOWORD(lParam) != 0);
		return 0;
	}

	LRESULT OnGetFont(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return (LRESULT)m_Font.m_hFont;
	}

	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;

		GetScrollInfo(SB_HORZ, &si);

		switch (LOWORD(wParam))
		{

		case SB_LEFT:
			si.nPos = si.nMin;
			break;

		case SB_RIGHT:
			si.nPos = si.nMax;
			break;

		case SB_LINELEFT:

			if (si.nPos > si.nMin)
				si.nPos -= 1;

			break;

		case SB_LINERIGHT:
			if (si.nPos < si.nMax)
				si.nPos += 1;

			break;

		case SB_PAGELEFT:
			if (si.nPos > si.nMin)
				si.nPos -= si.nPage;

			if (si.nPos < si.nMin)
				si.nPos = si.nMin;

			break;

		case SB_PAGERIGHT:
			if (si.nPos < si.nMax)
				si.nPos += si.nPage;

			if (si.nPos > si.nMax)
				si.nPos = si.nMax;

			break;

		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;

			break;
		}

		if (si.nMax != si.nMin)
			m_iHorzPos = si.nPos * 100 / (si.nMax - si.nMin);

		SetScrollInfo(SB_HORZ, &si);

		UpdateVisLines();

		InvalidateRect(NULL, FALSE);

		return TRUE;
	}

	LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(SB_VERT, &si);

		switch (LOWORD(wParam))
		{

		case SB_TOP:
			si.nPos = si.nMin;
			break;

		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

		case SB_LINEUP:

			if (si.nPos > si.nMin)
				si.nPos -= 1;

			break;

		case SB_LINEDOWN:
			if (si.nPos < si.nMax)
				si.nPos += 1;

			break;

		case SB_PAGEUP:
			if (si.nPos > si.nMin)
				si.nPos -= si.nPage;

			if (si.nPos < si.nMin)
				si.nPos = si.nMin;

			break;

		case SB_PAGEDOWN:
			if (si.nPos < si.nMax)
				si.nPos += si.nPage;

			if (si.nPos > si.nMax)
				si.nPos = si.nMax;

			break;

		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;

			break;
		}

		if (si.nMax != si.nMin)
			m_iVertPos = si.nPos * 100 / (si.nMax - si.nMin);

		SetScrollInfo(SB_VERT, &si);

		UpdateVisLines();

		InvalidateRect(NULL, FALSE);

		return TRUE;
	}

	LRESULT OnMouseMessage(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (Msg == WM_MOUSEMOVE)
		{
			CPoint pt = {LOWORD(lParam), HIWORD(lParam)};
			CRect rcClient;
			GetClientRect(&rcClient);

			if (PtInRect(&rcClient, pt))
			{
				bool bFound = false;

				if (GetCapture() != m_hWnd)
					SetCapture();

				unsigned int i = pt.y / m_iLineHeight;

				if (i < m_VisLines.size())
				{
					CSimpleArray<CVisLine>::iterator it = m_VisLines.begin() + i;

					for (CVisLine::iterator jt = it->begin(); jt != it->end(); jt++)
						if (pt.x >= jt->m_rcBounds.left && pt.x <= jt->m_rcBounds.right)
						{
							if (jt->m_pHyperLink != NULL)
							{
								HighlightLink(&*jt, pt);
								bFound = true;
							}

							break;
						}
				}

				if (!bFound)
					RestoreLink();
			}
			else
				ReleaseCapture();
		}
		else
			if (Msg == WM_LBUTTONDOWN)
			{
				CPoint pt = {LOWORD(lParam), HIWORD(lParam)};
				CRect rcClient;
				GetClientRect(&rcClient);

				if (PtInRect(&rcClient, pt))
				{
					bool bFound = false;
					unsigned int i = pt.y / m_iLineHeight;

					if (i < m_VisLines.size())
					{
						CSimpleArray<CVisLine>::iterator it = m_VisLines.begin() + i;

						for (CVisLine::iterator jt = it->begin(); jt != it->end(); jt++)
							if (pt.x >= jt->m_rcBounds.left && pt.x <= jt->m_rcBounds.right)
							{
								if (jt->m_pHyperLink != NULL)
								{
									jt->m_pHyperLink->Execute();
									bFound = true;
								}

								break;
							}
					}

					/*if(!bFound)
						Activate();*/
				}
			}

#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
			else
				if (Msg == WM_MOUSEWHEEL)
				{
					CPoint pt = {LOWORD(lParam), HIWORD(lParam)};
					CRect rc;
					GetWindowRect(&rc);

					if (PtInRect(&rc, pt))
					{
						int iScrollLines;
						SystemParametersInfo(SPI_GETWHEELSCROLLLINES,
																 0,
																 &iScrollLines,
																 0);

						m_iWheelDelta -= (short)HIWORD(wParam);

						if (abs(m_iWheelDelta) >= WHEEL_DELTA)
						{
							if (m_iWheelDelta > 0)
							{
								for (int i = 0; i < iScrollLines; i++)
									PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
							}
							else
							{
								for (int i = 0; i < iScrollLines; i++)
									PostMessage(WM_VSCROLL, SB_LINEUP, 0);
							}

							m_iWheelDelta %= WHEEL_DELTA;
						}
					}
				}

#endif

		MSG msg = { m_hWnd, Msg, wParam, lParam };

		if (m_tip.IsWindow())
			m_tip.RelayEvent(&msg);

		return 0;
	}

	LRESULT OnCaptureChanged(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RestoreLink();
		return 0;
	}

	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return TRUE;
	}

	// Operations
	inline const CPreparedHyperText& GetHyperText()
	{
		return m_Text;
	}

	inline void SetHyperText(const CPreparedHyperText& Src, bool bInvalidate = true)
	{
		m_Text = Src;
		UpdateSize(bInvalidate);
	}

	inline void AppendText(const CString& sText, bool bInvalidate = true)
	{
		m_Text.AppendText(sText);
		UpdateSize(bInvalidate);
	}

	inline void AppendHyperLink(const CString& sText, const CString& sTitle, const CString& sCommand, const CString& sDirectory, bool bInvalidate = true)
	{
		m_Text.AppendHyperLink(sText, sTitle, sCommand, sDirectory);
		UpdateSize(bInvalidate);
	}

	inline void AppendHyperLink(const CString& sText, const CString& sTitle, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool bInvalidate = true)
	{
		m_Text.AppendHyperLink(sText, sTitle, hWnd, uMsg, wParam, lParam);
		UpdateSize(bInvalidate);
	}

	inline COLORREF GetBkColor()
	{
		return m_BkColor;
	}

	inline void SetBkColor(COLORREF Color)
	{
		m_BkColor = Color;
	}

	inline COLORREF GetTextColor()
	{
		return m_TextColor;
	}

	inline void SetTextColor(COLORREF Color)
	{
		m_TextColor = Color;
	}

	inline COLORREF GetLinkColor()
	{
		return m_LinkColor;
	}

	inline void SetLinkColor(COLORREF LinkColor, bool bInvalidate = true)
	{
		m_LinkColor = LinkColor;

		if (bInvalidate)
			InvalidateRect(NULL, FALSE);
	}

	inline COLORREF GetHoverColor()
	{
		return m_HoverColor;
	}

	inline void SetHoverColor(COLORREF HoverColor)
	{
		m_HoverColor = HoverColor;
	}

	inline HCURSOR GetLinkCursor()
	{
		return m_LinkCursor;
	}

	inline void SetLinkCursor(HCURSOR LinkCursor)
	{
		m_LinkCursor = LinkCursor;
	}

	inline HCURSOR GetDefaultCursor()
	{
		return m_DefaultCursor;
	}

	inline void SetDefaultCursor(HCURSOR DefaultCursor)
	{
		m_DefaultCursor = DefaultCursor;
	}

protected:

	inline bool check_bits(DWORD Value, DWORD Mask)
	{
		return (Value & Mask) == Mask;
	}

	void UpdateSize(bool bRepaint/* = false*/)
	{
		if (m_bDontUpdateSizeInfo)
			return;

		m_bDontUpdateSizeInfo = true;

		DWORD dwStyle = GetWindowLongPtr(GWL_STYLE);

		bool bFirstPhase = false;

		if (check_bits(dwStyle, HTC_AUTO_SCROLL_BARS))
		{
			ShowScrollBar(SB_BOTH, FALSE);
			bFirstPhase = true;
		}

		CClientDC dc(*this);

		CFontHandle hOldFont = dc.SelectFont(m_Font);

		int iScrollHeight = GetSystemMetrics(SM_CYHSCROLL);

second_phase:
		m_Lines.clear();
		CRect rc;
		GetClientRect(&rc);
		int w = rc.right - rc.left;
		rc.DeflateRect(2, 0);
		m_iMaxWidth = 0;
		m_iLinesHeight = 0;
		long iMaxWidthChars = 0;
		CSize sz;

		if ((rc.right - rc.left) > 5 && (rc.bottom - rc.top) > 5)
		{
			std::list<CHyperLink>::iterator it = m_Text.GetLinks().begin();
			LPCTSTR s = m_Text.GetText();
			int len = m_Text.GetText().GetLength();
			int width = rc.Width();

			int npos, /* new position */
			pos = 0, /* current position */
						ll, /* line length */
						rll; /* line length with wordwrap (if used)*/

			while (len > 0)
			{
				ll = len;
				npos = ll;

				for (int i = 0; i < len; i++)
				{
					if (s[i] == _T('\r') || s[i] == _T('\n'))
					{
						if (s[i] == _T('\r') && ((i + 1) < len) && s[i+1] == _T('\n'))
							npos = i + 2;
						else
							npos = i + 1;

						ll = i;

						break;
					}
				}

				if (!::GetTextExtentExPoint(dc, s , (ll > 512) ? 512 : ll, width, &rll, NULL, &sz) || sz.cy == 0)
				{
					::GetTextExtentExPoint(dc, _T(" ") , 1, 0, NULL, NULL, &sz);
					sz.cx = 0;
					rll = ll;
				}

				if (rll > ll)
					rll = ll;

				if (!check_bits(dwStyle, HTC_WORDWRAP))
					rll = ll;
				else
					if (rll < ll)
						npos = rll;

				if (rll > 0)
				{
					if ((rll < len) && !_istspace(s[rll]))
						for (int i = rll - 1; i >= 0; i--)
							if (_istspace(s[i]))
							{
								rll = i;
								npos = i + 1;
								break;
							}
				}

				if (npos == 0)
					npos = 1;

				CLineInfo li(pos, pos + rll - 1);

				CLinePartInfo pl(pos, pos + rll - 1);

				while (it != m_Text.GetLinks().end() && it->End() < pos)
					it++;

				while (it != m_Text.GetLinks().end() && (it->Begin() >= pl.Begin() && it->Begin() <= pl.End()) ||
							 (it->End() >= pl.Begin() && it->End() <= pl.End()) ||
							 (pl.Begin() >= it->Begin() && pl.End() <= it->End()))
				{
					int b = it->Begin();
					int e = it->End();

					if (b < pl.Begin())
						b = pl.Begin();

					if (e > pl.End())
						e = pl.End();

					if (b > pl.Begin())
					{
						CLinePartInfo pln(pl.Begin(), b - 1);
						li.push_back(pln);
						pl.m_iBegin = e + 1;
					}

					CLinePartInfo pln(b, e, &*it);

					li.push_back(pln);
					pl.m_iBegin = e + 1;

					if (e < pl.End())
						it++;
					else
						break;
				}

				if (pl.Len() > 0)
					li.push_back(pl);

				m_iLineHeight = sz.cy;

				m_iLinesHeight += m_iLineHeight;

				if (sz.cx > m_iMaxWidth)
					m_iMaxWidth = sz.cx;

				if (iMaxWidthChars < li.Len())
					iMaxWidthChars = li.Len();

				m_Lines.push_back(li);

				pos += npos;

				s += npos;

				len -= npos;

				if (bFirstPhase && ((m_iLinesHeight + iScrollHeight) > rc.Height()))
				{
					bFirstPhase = false;
					ShowScrollBar(SB_VERT, TRUE);
					dwStyle |= WS_VSCROLL;
					goto second_phase;
				}
			}

			if (bRepaint)
				InvalidateRect(rc);
		}

		dc.SelectFont(hOldFont);

		// Update scroll bars
		dwStyle = GetWindowLongPtr(GWL_STYLE);

		if (check_bits(dwStyle, HTC_AUTO_SCROLL_BARS) && !check_bits(dwStyle, HTC_WORDWRAP))
		{
			if (m_iMaxWidth > rc.Width())
			{
				ShowScrollBar(SB_HORZ, TRUE);
				dwStyle |= WS_HSCROLL;
			};
		}

		SCROLLINFO si;

		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;

		if (check_bits(dwStyle, WS_HSCROLL) && m_iMaxWidth != 0)
		{
			si.nMin = 0;
			si.nMax = iMaxWidthChars + iMaxWidthChars / 2;
			int i = m_Text.GetText().GetLength();
			si.nPos = (int)(double(si.nMax) * m_iHorzPos / 100);
			si.nPage = (rc.Width() * si.nMax) / m_iMaxWidth;
			SetScrollInfo(SB_HORZ, &si, FALSE);
		}

		if (check_bits(dwStyle, WS_VSCROLL) && m_iLinesHeight != 0)
		{
			si.nMin = 0;
			si.nMax = m_Lines.size();
			si.nPos = (int)(double(si.nMax) * m_iVertPos / 100);
			si.nPage = (rc.Height() * si.nMax) / m_iLinesHeight;
			SetScrollInfo(SB_VERT, &si, FALSE);
		}

		m_bDontUpdateSizeInfo = false;

		UpdateVisLines();
	}

	void UpdateFonts()
	{
		DWORD dwStyle = GetWindowLongPtr(GWL_STYLE);

		if (!m_LinksFont.IsNull())
			m_LinksFont.DeleteObject();

		if (!m_HoverFont.IsNull())
			m_HoverFont.DeleteObject();

		LOGFONT lf;

		m_Font.GetLogFont(&lf);

		if (check_bits(dwStyle, HTC_UNDERLINE_LINKS))
			lf.lfUnderline = TRUE;

		m_LinksFont.CreateFontIndirect(&lf);

		m_Font.GetLogFont(&lf);

		if (check_bits(dwStyle, HTC_UNDERLINE_HOVER))
			lf.lfUnderline = TRUE;

		m_HoverFont.CreateFontIndirect(&lf);
	}

	void UpdateVisLines()
	{
		RestoreLink();
		DWORD dwStyle = GetWindowLongPtr(GWL_STYLE);

		int id = 1;

		if (check_bits(dwStyle, HTC_ENABLE_TOOLTIPS))
		{
			for (CSimpleArray<CVisLine>::iterator itv = m_VisLines.begin(); itv != m_VisLines.end(); itv++)
				for (CVisLine::iterator jt = itv->begin(); jt != itv->end(); jt++)
				{
					if (jt->m_pHyperLink != NULL)
						m_tip.DelTool(*this, id++);
				}
		}

		m_VisLines.clear();

		CClientDC dc(m_hWnd); // device context for painting

		CSimpleArray<CLineInfo>::iterator it = m_Lines.begin();
		int iVertPos = 0;
		int iHorzPos = 0;

		if (check_bits(dwStyle, WS_VSCROLL))
			iVertPos = GetScrollPos(SB_VERT);

		if (check_bits(dwStyle, WS_HSCROLL))
			iHorzPos = GetScrollPos(SB_HORZ);

		if (iVertPos >= (int)m_Lines.size())
			return;

		it += iVertPos;

		CFontHandle hOldFont = dc.SelectFont(m_Font);

		int ypos = 0;

		LPCTSTR s = m_Text.GetText();

		CRect rcClient;

		GetClientRect(rcClient);

		for (; it != m_Lines.end(); it++)
		{
			int XPos = 2;
			int LinePos = it->Begin();
			int Offset = 0;
			int Len = 0;

			CVisLine vl;
			CRect rcBounds;

			CSimpleArray<CLinePartInfo>::iterator jt;

			for (jt = it->begin(); jt != it->end(); jt++)
			{
				if (jt->Begin() <= (LinePos + iHorzPos) && jt->End() >= (LinePos + iHorzPos))
				{
					Offset = LinePos + iHorzPos;
					Len = jt->Len() - ((LinePos + iHorzPos) - jt->Begin());
					break;
				}
			}

			while (jt != it->end())
			{
				if (Len > 0)
				{
					CSize sz;
					::GetTextExtentExPoint(dc, s + Offset, Len, 0, NULL, NULL, &sz);

					rcBounds.left = XPos;
					XPos += sz.cx;
					rcBounds.right = XPos;
					rcBounds.top = ypos;
					rcBounds.bottom = ypos + m_iLineHeight;

					vl.push_back(CVisPart(*jt, rcBounds, Offset, Len, NULL, NULL));
				}

				if (XPos > rcClient.Width())
					break;

				jt++;

				Offset = jt->Begin();

				Len = jt->Len();
			}

			m_VisLines.push_back(vl);

			ypos += m_iLineHeight;

			if (ypos > rcClient.bottom)
				break;
		}

		CVisPart *pPrev = NULL, *pNext;

		id = 1;

		for (CSimpleArray<CVisLine>::iterator it2 = m_VisLines.begin(); it2 != m_VisLines.end(); it2++)
			for (CVisLine::iterator jt = it2->begin(); jt != it2->end(); jt++)
			{
				pNext = &*jt;

				if (pPrev != NULL &&
						pPrev->m_pHyperLink != NULL &&
						pPrev->m_pHyperLink == pNext->m_pHyperLink &&
						pPrev != pNext)
				{
					pPrev->m_pNext = pNext;
					pNext->m_pPrev = pPrev;
				}

				pPrev = pNext;

				if (check_bits(dwStyle, HTC_ENABLE_TOOLTIPS) && jt->m_pHyperLink != NULL)
					m_tip.AddTool(*this, (LPCTSTR)jt->m_pHyperLink->Title(), jt->m_rcBounds, id++);
			}

		dc.SelectFont(hOldFont);
	}

	void HighlightLink(CVisPart* Part, const CPoint& MouseCoords)
	{
		if (m_pActivePart == Part)
			return;

		if (m_pActivePart != Part && m_pActivePart != NULL && Part != NULL && m_pActivePart->m_pHyperLink != Part->m_pHyperLink)
			RestoreLink();

		m_pActivePart = Part;

		while (m_pActivePart->m_pPrev != NULL)
			m_pActivePart = m_pActivePart->m_pPrev;

		CClientDC dc(*this);

		CFontHandle hOldFont = dc.SelectFont(m_HoverFont);

		dc.SetBkColor(m_BkColor);

		dc.SetTextColor(m_HoverColor);

		LPCTSTR s = m_Text.GetText();

		CVisPart* p = m_pActivePart;

		while (p != NULL)
		{
			TextOut(dc, p->m_rcBounds.left, p->m_rcBounds.top,
							s + p->m_iRealBegin, p->m_iRealLen);
			p = p->m_pNext;
		}

		dc.SelectFont(hOldFont);

		SetCursor(m_LinkCursor);
	}

	void RestoreLink()
	{
		if (m_pActivePart == NULL)
			return;

		CClientDC dc(*this);

		CFontHandle hOldFont = dc.SelectFont(m_LinksFont);

		dc.SetBkColor(m_BkColor);

		dc.SetTextColor(m_LinkColor);

		LPCTSTR s = m_Text.GetText();

		CVisPart* p = m_pActivePart;

		while (p != NULL)
		{
			TextOut(dc, p->m_rcBounds.left, p->m_rcBounds.top,
							s + p->m_iRealBegin, p->m_iRealLen);
			p = p->m_pNext;
		}

		dc.SelectFont(hOldFont);

		m_pActivePart = NULL;
		SetCursor(m_DefaultCursor);
	}
};

// --------------------------------------------------------------
// CHyperTextCtrl

class CHyperTextCtrl : public CHyperTextCtrlT< CWindowImpl<CHyperTextCtrl, CWindow> >
{

public:
	//message map
	BEGIN_MSG_MAP(CHyperTextCtrl)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SETTEXT, OnSetText)
	MESSAGE_HANDLER(WM_GETTEXT, OnGetText)
	MESSAGE_HANDLER(WM_SETFONT, OnSetFont)
	MESSAGE_HANDLER(WM_GETFONT, OnGetFont)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
	MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//window class
	DECLARE_WND_CLASS_EX(_T("maqHyperTextCtrl"), CS_DBLCLKS | CS_HREDRAW, COLOR_WINDOW);
};

/*
class CHyperTextCtrlFSB : public CHyperTextCtrlT< CWindowImpl<CHyperTextCtrl, CFSBWindow> >
{
public:
	//message map
	BEGIN_MSG_MAP(CHyperTextCtrl)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SETTEXT, OnSetText)
	MESSAGE_HANDLER(WM_GETTEXT, OnGetText)
	MESSAGE_HANDLER(WM_SETFONT, OnSetFont)
	MESSAGE_HANDLER(WM_GETFONT, OnGetFont)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
	MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//window class
	DECLARE_WND_CLASS_EX(_T("maqHyperTextCtrlFSB"), CS_DBLCLKS | CS_HREDRAW, COLOR_WINDOW);

	LRESULT OnCreate(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		FlatSB_Initialize();
		FlatSB_SetScrollProp(WSB_PROP_VSTYLE, FSB_FLAT_MODE, TRUE);
		FlatSB_SetScrollProp(WSB_PROP_HSTYLE, FSB_FLAT_MODE, TRUE);
		return CHyperTextCtrlT< CWindowImpl<CHyperTextCtrl, CFSBWindow> >::OnCreate(Msg, wParam, lParam, bHandled);
	}
};
*/

//
}; //namespace HyperTextControl

#endif

