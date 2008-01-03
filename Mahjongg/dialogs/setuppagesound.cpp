/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SetupPageSound.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"
#include "SetupPageSound.h"

CSetupPageSound::CSetupPageSound()
{
}

CSetupPageSound::~CSetupPageSound()
{
}

LRESULT CSetupPageSound::OnInitDialog(UINT , WPARAM , LPARAM , BOOL&)
{
	LocalizeControls();

	m_bSound = g_AppSettings.m_bSound;

	DoDataExchange(FALSE);

	return 0;
}

void CSetupPageSound::Show(UINT nCommand)
{
	ShowWindow(nCommand);
}

void CSetupPageSound::Move(CRect &rect)
{
	SetWindowPos(m_hWnd, rect.left, rect.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
