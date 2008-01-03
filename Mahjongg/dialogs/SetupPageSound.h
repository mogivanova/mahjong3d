/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SetupPageSound.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 23.03.04 23:11 $
*
*================================================================================================*/

#pragma once

#include "SetupPage.h"
#include "..\GUI\LocalizableDialog.h"

class CSetupPageSound :
			public CLocalizableDialog<CSetupPageSound>,
			public CSetupPage,
			public CDialogImpl<CSetupPageSound>,
			public CWinDataExchange<CSetupPageSound>
{

public:
	BOOL m_bSound;

public:
	CSetupPageSound(void);
	~CSetupPageSound(void);

public:
	virtual void Move(CRect& rect);
	virtual void Show(UINT nCommand);
	enum { IDD = IDD_SETUP_SOUND };

	BEGIN_MSG_MAP(CSetupPageSound)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupPageSound)
	DDX_CHECK(IDC_SOUND, m_bSound)
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
