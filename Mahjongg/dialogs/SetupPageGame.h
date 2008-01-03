#pragma once

#include "SetupPage.h"
#include "..\GUI\LocalizableDialog.h"

class CSetupPageGame :
			public CLocalizableDialog<CSetupPageGame>,
			public CSetupPage,
			public CDialogImpl<CSetupPageGame>,
			public CWinDataExchange<CSetupPageGame>
{

public:
	BOOL m_bSmartCursor;
	BOOL m_bShowRBHints;
	BOOL m_bSolvableBoards;
	BOOL m_bAutoSave;
	BOOL m_bTextHints;
	BOOL m_bHideClosed;

public:
	CSetupPageGame(void);
	~CSetupPageGame(void);

public:
	virtual void Move(CRect& rect);
	virtual void Show(UINT nCommand);

	enum { IDD = IDD_SETUP_GAME };

	BEGIN_MSG_MAP(CSetupPageGame)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupPageGame)
	DDX_CHECK(IDC_SMART_CURSOR, m_bSmartCursor)
	DDX_CHECK(IDC_RB_HINTS, m_bShowRBHints)
	DDX_CHECK(IDC_SOLVABLE_BOARDS, m_bSolvableBoards)
	DDX_CHECK(IDC_AUTO_SAVE, m_bAutoSave)
	DDX_CHECK(IDC_TEXT_HINTS, m_bTextHints)
	DDX_CHECK(IDC_HIDE_CLOSED, m_bHideClosed);
	END_DDX_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)

	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
};
