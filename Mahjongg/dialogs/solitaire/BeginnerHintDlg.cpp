/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: BeginnerHintDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\..\resource.h"
#include "BeginnerHintDlg.h"

LRESULT CBeginnerHintDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	LocalizeControls();

	::SetWindowText(GetDlgItem(IDC_MESSAGE), m_strMessage);

	m_bShowHints = g_AppSettings.m_bTextHints ? FALSE : TRUE;

	DoDataExchange(FALSE);

	return 0;
}

LRESULT CBeginnerHintDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(TRUE);

	g_AppSettings.m_bTextHints = m_bShowHints != TRUE;

	EndDialog(IDOK);
	return 0;
}
