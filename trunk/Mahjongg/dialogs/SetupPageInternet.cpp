/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SetupPageInternet.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"
#include "SetupPageInternet.h"

CSetupPageInternet::CSetupPageInternet()
{
}

CSetupPageInternet::~CSetupPageInternet()
{
}

LRESULT CSetupPageInternet::OnInitDialog(UINT , WPARAM , LPARAM , BOOL&)
{
	LocalizeControls();

	m_hPointer = LoadCursor(NULL, IDC_ARROW);
	m_hHand = LoadCursor(_Module.m_hInstResource, MAKEINTRESOURCE(IDC_HANDCUR));

	memset(m_strLogin, 0, sizeof(m_strLogin));
	memset(m_strPassword, 0, sizeof(m_strPassword));

	lstrcpyn(m_strLogin, g_AppSettings.m_strScoresAccount, 512);
	lstrcpyn(m_strPassword, g_AppSettings.m_strScoresPassword, 512);

	// bold font for www
	m_objFont.CreateFont(8, 0, 0, 0, 600, FALSE, TRUE, 0,
											 ANSI_CHARSET, OUT_DEFAULT_PRECIS,
											 CLIP_DEFAULT_PRECIS, DRAFT_QUALITY,
											 DEFAULT_PITCH | FF_SWISS, _T("Helv"));

	m_ctrlLink = GetDlgItem(IDC_REGISTER_LINK);
	m_ctrlLink.SetFont(m_objFont.m_hFont, FALSE);

	DoDataExchange(FALSE);

	return 0;
}

/**
 *
 */
LRESULT CSetupPageInternet::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hWnd = (HWND)wParam;

	if (hWnd == GetDlgItem(IDC_REGISTER_LINK))
	{
		::SetCursor(m_hHand);
		return TRUE;
	}
	else
	{
		::SetCursor(m_hPointer);
		return TRUE;
	}

	return FALSE;
}

void CSetupPageInternet::Show(UINT nCommand)
{
	ShowWindow(nCommand);
}

void CSetupPageInternet::Move(CRect &rect)
{
	SetWindowPos(m_hWnd, rect.left, rect.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

/**
 *
 */
LRESULT CSetupPageInternet::OnBnClickedLink(WORD , WORD , HWND , BOOL& )
{
	ShellExecute(GetDesktopWindow(), _T("open"), LoadResourceString(IDS_ACCOUNT_REGISTER), NULL, NULL, 0);

	return 0;
}