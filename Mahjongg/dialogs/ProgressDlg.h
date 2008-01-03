/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: ProgressDlg.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:54 $
*
*================================================================================================*/

#pragma once

#include "..\GUI\LocalizableDialog.h"

class CProgressDlg :
			public CLocalizableDialog<CProgressDlg>,
			public CDialogImpl<CProgressDlg>
{

public:
	enum {IDD = IDD_PROGRESS};

	CProgressDlg();
	virtual ~CProgressDlg();

	void SetPos(int nPos);
	void SetRange(int nMin, int nMax);

	BEGIN_MSG_MAP(CProgressDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
};
