/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: ExtAPI.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:37 $
*
*================================================================================================*/

#include "stdafx.h"

#include "ExtAPI.h"

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
CUser32Ext g_User32;

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
CUser32Ext::CUser32Ext()
{
	m_pfnSetLayeredWindowAttributes = NULL;

	m_hModule = ::LoadLibrary(_T("user32.dll"));

	if (m_hModule != NULL)
	{
		m_pfnSetLayeredWindowAttributes = (PFnSetLayeredWindowAttributes)::GetProcAddress(m_hModule, "SetLayeredWindowAttributes");
	}
}

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
CUser32Ext::~CUser32Ext()
{
}

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
BOOL CUser32Ext::SetLayeredWindowAttributes(HWND hWnd, COLORREF crKey, BYTE btAlpha, DWORD dwFlags)
{
	if (m_pfnSetLayeredWindowAttributes == NULL)
		return FALSE;

	return (*m_pfnSetLayeredWindowAttributes)(hWnd, crKey, btAlpha, dwFlags);
}

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
CUXThemeExt g_UXTheme;

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
CUXThemeExt::CUXThemeExt()
{
	m_pfnSetWindowTheme = NULL;

	m_hModule = ::LoadLibrary(_T("uxtheme.dll"));

	if (m_hModule != NULL)
	{
		m_pfnSetWindowTheme = (PFnSetWindowTheme)::GetProcAddress(m_hModule, "SetWindowTheme");
	}
}

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
CUXThemeExt::~CUXThemeExt()
{
}

/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
HRESULT CUXThemeExt::SetWindowTheme(HWND hWnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIDList)
{
	if (m_pfnSetWindowTheme == NULL)
		return E_FAIL;

	return (*m_pfnSetWindowTheme)(hWnd, pszSubAppName, pszSubIDList);
}