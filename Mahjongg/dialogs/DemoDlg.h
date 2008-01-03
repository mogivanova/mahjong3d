/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: DemoDlg.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:54 $
*
*================================================================================================*/

#pragma once

#ifdef DEMO

class CDemoDlg :
			public CLocalizableDialog<CDemoDlg>,
			public CFramePainter<CDemoDlg>,
			public CDialogImpl<CDemoDlg>
{

public:
	CDemoDlg(void);
	~CDemoDlg(void);

public:
	enum { IDD = IDD_DEMO_MODE };

	BEGIN_MSG_MAP(CDemoDlg)

	if (g_AppSettings.m_bSkinEnabled)
		CHAIN_MSG_MAP(CFramePainter<CDemoDlg>)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		END_MSG_MAP()

		// Handler prototypes (uncomment arguments if needed):
		//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
};

#endif
