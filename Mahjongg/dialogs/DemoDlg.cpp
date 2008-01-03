/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: DemoDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#include "StdAfx.h"
#include "..\Resource.h"
#include "demodlg.h"

#ifdef DEMO

CDemoDlg::CDemoDlg(void)
{
}

CDemoDlg::~CDemoDlg(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CDemoDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	LocalizeControls();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CDemoDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

#endif