/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: EnterKeyDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:55 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"
#include "EnterKeyDlg.h"

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CEnterKeyDlg::CEnterKeyDlg()
{
	ZeroMemory(m_strKey, sizeof(m_strKey));
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CEnterKeyDlg::~CEnterKeyDlg()
{
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CEnterKeyDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	LocalizeControls();

	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CEnterKeyDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(TRUE);
	EndDialog(wID);
	return 0;
}

