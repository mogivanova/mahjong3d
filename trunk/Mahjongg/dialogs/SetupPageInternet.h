/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SetupPageInternet.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 23.03.04 23:11 $
*
*================================================================================================*/

#pragma once

#include "SetupPage.h"
#include "..\GUI\LocalizableDialog.h"

class CSetupPageInternet :
			public CLocalizableDialog<CSetupPageInternet>,
			public CSetupPage,
			public CDialogImpl<CSetupPageInternet>,
			public CWinDataExchange<CSetupPageInternet>
{

public:
	CFont m_objFont;
	CStatic m_ctrlLink;
	HCURSOR m_hPointer;
	HCURSOR m_hHand;

	TCHAR m_strLogin[512];
	TCHAR m_strPassword[512];

public:
	CSetupPageInternet(void);
	~CSetupPageInternet(void);

public:
	virtual void Move(CRect& rect);
	virtual void Show(UINT nCommand);
	enum { IDD = IDD_SETUP_INTERNET };

	BEGIN_MSG_MAP(CSetupPageInternet)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	COMMAND_HANDLER(IDC_REGISTER_LINK, BN_CLICKED, OnBnClickedLink)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupPageInternet)
	DDX_TEXT_LEN(IDC_LOGIN, m_strLogin, 512)
	DDX_TEXT_LEN(IDC_PASSWORD, m_strPassword, 512)
	END_DDX_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	LRESULT OnBnClickedLink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
};
