/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: ExtAPI.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#pragma once

class CUser32Ext
{

public:
	CUser32Ext();
	~CUser32Ext();

public:
	BOOL SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

protected:
	HMODULE m_hModule;

protected:
	typedef BOOL (WINAPI* PFnSetLayeredWindowAttributes)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

protected:
	PFnSetLayeredWindowAttributes m_pfnSetLayeredWindowAttributes;

};

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
extern CUser32Ext g_User32;

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/

class CUXThemeExt
{

public:
	CUXThemeExt();
	~CUXThemeExt();

public:
	HRESULT SetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIDList);

protected:
	HMODULE m_hModule;

protected:
	typedef HRESULT (WINAPI* PFnSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIDList);

protected:
	PFnSetWindowTheme m_pfnSetWindowTheme;
};

extern CUXThemeExt g_UXTheme;