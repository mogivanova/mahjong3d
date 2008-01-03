/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MJAppSettings.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 8.11.04 22:59 $
*
*================================================================================================*/

#include "StdAfx.h"
#include "..\..\resource.h"

#include "..\Registry.h"

#include "mjappsettings.h"

LPCTSTR CAppSettings::m_pszRegistryKeyName = _T("Software\\Thinking Thing Software\\Amazing Mahjongg 3D");
LPCTSTR CAppSettings::m_pszRegistrationURL = _T("http://www.mahjongg4u.com/order");

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAppSettings::CAppSettings(void)
{
	// window position
	m_nWndPosX = CW_USEDEFAULT;
	m_nWndPosY = CW_USEDEFAULT;
	m_nWndWidth = CW_USEDEFAULT;
	m_nWndHeight = CW_USEDEFAULT;
	m_bWndMaximized = false;

	//
	m_bFullScreen = false;

	// [2/15/2003]
	m_enmGameMode = eSolitaire;

	// user interface mode
	m_enmInterfaceMode = eBeginner;

	m_bSmartCursor = true;
	m_bShowRBHints = true;
	m_bTextHints = true;
	m_bSolvableBoards = true;
	m_bAutoSave = true;
	m_bHideClosedTiles = false;

	// detail level
	m_nGraphDetail = 1;

	m_bHighQualityTextures = true;
	m_bTexturedSides = false;
	m_bHighQualityBackground = true;
	m_bSmoothTiles = false;
	m_bTransparentTiles = false;
	m_bColoredLayers = false;

	m_bSound = true;

	m_bBitmapBack = false;
	m_rgbBackground = RGB(0, 0, 64);
	m_strBackground = _T("default");

	CString strTmp;
	strTmp.LoadString(IDS_BACKGROUND_FOLDER);
	m_strPicturesFolder = MakeInstallPath(strTmp);

	m_strLayout = _T("default");
	strTmp.LoadString(IDS_LAYOUTS_FOLDER);
	m_strLayoutsFolder = MakeInstallPath(strTmp);

	m_strTileset = _T("default");
	strTmp.LoadString(IDS_TILESET_FOLDER);
	m_strTilesetFolder = MakeInstallPath(strTmp);

	m_strMusic = _T("Music/default.xm");
	strTmp.LoadString(IDS_MUSIC_FOLDER);
	m_strMusicFolder = MakeInstallPath(strTmp);

	m_strPlayerName = _T("Player");

	m_strLanguage = _T("User");

	m_strScoresAccount = _T("");
	m_strScoresPassword = _T("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAppSettings::~CAppSettings(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CAppSettings::Load()
{
	CRegistry reg;

	if (!reg.Open(HKEY_CURRENT_USER, m_pszRegistryKeyName))
		return false;

	m_nWndPosX = reg.ReadDWORD(_T("PosX"), m_nWndPosX);

	m_nWndPosY = reg.ReadDWORD(_T("PosY"), m_nWndPosX);

	m_nWndWidth = reg.ReadDWORD(_T("Width"), m_nWndPosX);

	m_nWndHeight = reg.ReadDWORD(_T("Height"), m_nWndPosX);

	m_bWndMaximized = reg.ReadBool(_T("Maximized"), m_bWndMaximized);

	m_bFullScreen = reg.ReadBool(_T("FullScreen"), m_bFullScreen);

	//m_enmGameMode = (GAME_MODE)reg.ReadDWORD(_T("LastGameMode"), m_enmGameMode);
	m_enmGameMode = eSolitaire;

	//m_enmInterfaceMode = (INTERFACE_MODE)reg.ReadDWORD(_T("Interface"), m_enmInterfaceMode);
	m_enmInterfaceMode = eExpert;

	m_bSmartCursor = reg.ReadBool(_T("SmartCursor"), m_bSmartCursor);

	m_bShowRBHints = reg.ReadBool(_T("ShowRBHints"), m_bShowRBHints);

	m_bTextHints = reg.ReadBool(_T("TextHints"), m_bTextHints);

	m_bSolvableBoards = reg.ReadBool(_T("SolvableBoards"), m_bSolvableBoards);

	m_bAutoSave = reg.ReadBool(_T("AutoSave"), m_bAutoSave);

	m_bHideClosedTiles = reg.ReadBool(_T("HideClosedTiles"), m_bHideClosedTiles);

	m_nGraphDetail = reg.ReadDWORD(_T("GraphDetail"), m_nGraphDetail);

	m_bHighQualityTextures = reg.ReadBool(_T("HighQualityTextures"), m_bHighQualityTextures);

	m_bTexturedSides = reg.ReadBool(_T("TexturedSides"), m_bTexturedSides);

	m_bHighQualityBackground = reg.ReadBool(_T("HighQualityBackground"), m_bHighQualityBackground);

	m_bSmoothTiles = reg.ReadBool(_T("SmoothTiles"), m_bSmoothTiles);

	m_bTransparentTiles = reg.ReadBool(_T("TransparentTiles"), m_bTransparentTiles);

	m_bColoredLayers = true; //reg.ReadBool(_T("ColoredLayers"), m_bColoredLayers);

	m_bSound = reg.ReadBool(_T("Sound"), m_bSound);

	m_bBitmapBack = reg.ReadBool(_T("BitmapBackground"), m_bBitmapBack);

	m_rgbBackground = reg.ReadDWORD(_T("BackgroundColor"), m_rgbBackground);

	m_strBackground = reg.ReadString(_T("BackgroundImage"), m_strBackground);

	m_strPicturesFolder = reg.ReadString(_T("LastPicturesDir"), m_strPicturesFolder);

	m_strLayout = reg.ReadString(_T("Layout"), m_strLayout);

	m_strLayoutsFolder = reg.ReadString(_T("LastLayoutsDir"), m_strLayoutsFolder);

	m_strTileset = reg.ReadString(_T("Tileset"), m_strTileset);

	m_strTilesetFolder = reg.ReadString(_T("LastTilesetDir"), m_strTilesetFolder);

	m_strMusic = reg.ReadString(_T("Music"), m_strMusic);

	m_strMusicFolder = reg.ReadString(_T("LastMusicDir"), m_strMusicFolder);

	m_strPlayerName = reg.ReadString(_T("PlayerName"), m_strPlayerName);

	///m_bEnvLight = reg.ReadBool(_T("EnvironmentLighting"), m_bEnvLight);
	m_bEnvLight = false;

	//CAppSettings::m_strRegistrationURL = LoadString(IDS_REGISTRATION);

	m_strLanguage = reg.ReadString(_T("Language"), m_strLanguage);

	m_strScoresAccount = reg.ReadString(_T("Account"), m_strScoresAccount);

	m_strScoresPassword = reg.ReadString(_T("Password"), m_strScoresPassword);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CAppSettings::Save()
{
	CRegistry reg;

	if (!reg.Open(HKEY_CURRENT_USER, m_pszRegistryKeyName))
		return false;

	reg.WriteDWORD(_T("PosX"), m_nWndPosX);

	reg.WriteDWORD(_T("PosY"), m_nWndPosY);

	reg.WriteDWORD(_T("Width"), m_nWndWidth);

	reg.WriteDWORD(_T("Height"), m_nWndHeight);

	reg.WriteDWORD(_T("Maximized"), m_bWndMaximized);

	reg.WriteDWORD(_T("FullScreen"), m_bFullScreen);

	reg.WriteDWORD(_T("LastGameMode"), m_enmGameMode);

	reg.WriteDWORD(_T("Interface"), m_enmInterfaceMode);

	reg.WriteBool(_T("SmartCursor"), m_bSmartCursor);

	reg.WriteBool(_T("ShowRBHints"), m_bShowRBHints);

	reg.WriteBool(_T("TextHints"), m_bTextHints);

	reg.WriteBool(_T("SolvableBoards"), m_bSolvableBoards);

	reg.WriteBool(_T("AutoSave"), m_bAutoSave);

	reg.WriteBool(_T("HideClosedTiles"), m_bHideClosedTiles);

	reg.WriteDWORD(_T("GraphDetail"), m_nGraphDetail);

	reg.WriteBool(_T("HighQualityTextures"), m_bHighQualityTextures);

	reg.WriteBool(_T("TexturedSides"), m_bTexturedSides);

	reg.WriteBool(_T("HighQualityBackground"), m_bHighQualityBackground);

	reg.WriteBool(_T("SmoothTiles"), m_bSmoothTiles);

	reg.WriteBool(_T("TransparentTiles"), m_bTransparentTiles);

	reg.WriteBool(_T("ColoredLayers"), m_bColoredLayers);

	reg.WriteDWORD(_T("Sound"), m_bSound);

	reg.WriteDWORD(_T("EnvironmentLighting"), m_bEnvLight);

	reg.WriteDWORD(_T("BitmapBackground"), m_bBitmapBack);

	reg.WriteDWORD(_T("BackgroundColor"), m_rgbBackground);

	reg.WriteString(_T("BackgroundImage"), m_strBackground);

	reg.WriteString(_T("LastPicturesDir"), m_strPicturesFolder);

	reg.WriteString(_T("Layout"), m_strLayout);

	reg.WriteString(_T("LastLayoutsDir"), m_strLayoutsFolder);

	reg.WriteString(_T("Tileset"), m_strTileset);

	reg.WriteString(_T("LastTilesetDir"), m_strTilesetFolder);

	reg.WriteString(_T("Music"), m_strMusic);

	reg.WriteString(_T("LastMusicDir"), m_strMusicFolder);

	reg.WriteString(_T("PlayerName"), m_strPlayerName);

	reg.WriteString(_T("Language"), m_strLanguage);

	reg.WriteString(_T("Account"), m_strScoresAccount);

	reg.WriteString(_T("Password"), m_strScoresPassword);

	return true;
}
