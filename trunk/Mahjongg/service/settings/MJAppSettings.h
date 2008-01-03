/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MJAppSettings.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 16.03.04 23:05 $
*
*================================================================================================*/

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//
#define BEGIN_SETTINGS_TABLE(thisClass) \
  static const SettingsRecord* GetSettingsMap() \
  { \
    static const SettingsRecord theMap[] = \
    {

#define BOOL_SETTING(name, variable, default) {eBool, name, &variable, (void*)default},
#define DWORD_SETTING(name, variable, default) {eDword, name, &variable, (void*)default},
#define STRING_SETTING(name, variable, default) {eString, name, &variable, default},
#define FLOAT_SETTING(name, variable, default)  {eFloat, name, &variable, (void*)default),

#define END_SETTINGS_TABLE() \
      { eUnknown, NULL, NULL, NULL } \
    }; \
    return theMap; \
  }


///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CAppSettings
{
	enum SETTINGS_TYPE
	{
		eUnknown,
		eBool,
		eDword,
		eString,
		eFloat
	};

	struct SettingsRecord
	{
		SETTINGS_TYPE enmType;
		LPCTSTR pszName;
		void* pVariable;
		void* pDefault;
	};

public:
	CAppSettings(void);
	~CAppSettings(void);

	virtual bool Save();
	virtual bool Load();

public:
	static LPCTSTR m_pszRegistryKeyName; //
	static LPCTSTR m_pszRegistrationURL; //

	enum INTERFACE_MODE
	{
		eBeginner,
		eExpert
	};

	enum GAME_MODE
	{
		eUndefined,
		eSolitaire,
		eFourWinds
	};

	BEGIN_SETTINGS_TABLE(CAppSettings)
	END_SETTINGS_TABLE()

public:
	// window position
	int m_nWndPosX;
	int m_nWndPosY;
	int m_nWndWidth;
	int m_nWndHeight;
	bool m_bWndMaximized;

	bool m_bFullScreen;

	GAME_MODE m_enmGameMode;

	INTERFACE_MODE m_enmInterfaceMode;

	//
	bool m_bSmartCursor;
	bool m_bShowRBHints;
	bool m_bTextHints;
	bool m_bSolvableBoards;
	bool m_bAutoSave;
	bool m_bHideClosedTiles;

	int m_nGraphDetail;

	bool m_bHighQualityTextures;
	bool m_bTexturedSides;
	bool m_bHighQualityBackground;
	bool m_bSmoothTiles;
	bool m_bTransparentTiles;
	bool m_bColoredLayers;

	bool m_bSound;

	bool m_bBitmapBack;
	COLORREF m_rgbBackground;

	CString m_strBackground;
	CString m_strPicturesFolder;

	CString m_strTileset;
	CString m_strTilesetFolder;

	CString m_strLayout;
	CString m_strLayoutsFolder;

	CString m_strMusic;
	CString m_strMusicFolder;

	bool m_bEnvLight;

	CString m_strPlayerName;

	float m_fAngleX;
	float m_fAngleY;
	float m_fAngleZ;
	float m_fRange;

	CString m_strCurrentUser;

	CString m_strLanguage;

	CString m_strScoresAccount;
	CString m_strScoresPassword;
};

extern CAppSettings g_AppSettings;
