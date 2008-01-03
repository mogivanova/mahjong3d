/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: PlayerNameDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 14.05.04 0:55 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\..\resource.h"
#include "PlayerNameDlg.h"

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CPlayerNameDlg::OnInitDialog(UINT , WPARAM , LPARAM , BOOL&)
{
	CenterWindow(GetParent());

	LocalizeControls();

	m_ctrlPlayerName = GetDlgItem(IDC_PLAYER_NAME);

	m_ctrlPlayerName.SetWindowText(g_AppSettings.m_strPlayerName);

	return TRUE;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CPlayerNameDlg::OnOk(WORD , WORD wID, HWND , BOOL&)
{
	TCHAR chBuffer[1024];
	m_ctrlPlayerName.GetWindowText(chBuffer, 1024);
	g_AppSettings.m_strPlayerName = chBuffer;

	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CPlayerNameDlg::OnCancel(WORD , WORD wID, HWND , BOOL&)
{
	EndDialog(wID);
	return 0;
}
