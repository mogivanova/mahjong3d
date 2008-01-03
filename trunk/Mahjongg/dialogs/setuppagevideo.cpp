/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SetupPageVideo.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 16.03.04 23:09 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"
#include "SetupPageVideo.h"

CSetupPageVideo::CSetupPageVideo()
{
}

CSetupPageVideo::~CSetupPageVideo()
{
}

LRESULT CSetupPageVideo::OnInitDialog(UINT , WPARAM , LPARAM , BOOL&)
{
	LocalizeControls();

	m_nGraphDetail = g_AppSettings.m_nGraphDetail;
	m_bHighQualityTextures = g_AppSettings.m_bHighQualityTextures;
	m_bTexturedSides = g_AppSettings.m_bTexturedSides;
	m_bHighQualityBackground = g_AppSettings.m_bHighQualityBackground;
	m_bSmoothTiles = g_AppSettings.m_bSmoothTiles;
	m_bColoredLayers = g_AppSettings.m_bColoredLayers;

	DoDataExchange(FALSE);

	return 0;
}

void CSetupPageVideo::Show(UINT nCommand)
{
	ShowWindow(nCommand);
}

void CSetupPageVideo::Move(CRect &rect)
{
	SetWindowPos(m_hWnd, rect.left, rect.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
