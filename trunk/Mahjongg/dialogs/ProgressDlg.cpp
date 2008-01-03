/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: ProgressDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"
#include "ProgressDlg.h"

CProgressDlg::CProgressDlg()
{

}

CProgressDlg::~CProgressDlg()
{

}

LRESULT CProgressDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	LocalizeControls();

	return 0;
}

void CProgressDlg::SetRange(int nMin, int nMax)
{
	if (m_hWnd != NULL)
		::SendMessage(GetDlgItem(IDC_PROGRESS), PBM_SETRANGE32, nMin, nMax);
}

void CProgressDlg::SetPos(int nPos)
{
	if (m_hWnd != NULL)
		::SendMessage(GetDlgItem(IDC_PROGRESS), PBM_SETPOS, nPos, 0);
}
