/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: Common.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#include "stdafx.h"
#include "Common.h"

OSVERSIONINFO g_osvi;

LARGE_INTEGER g_liTimerFrequency;

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CString LoadResourceString(UINT nStringID)
{
	CString str;
	str.LoadString(nStringID);
	return str;
}


/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CString MakeInstallPath(CString strPathName)
{
	TCHAR pszFullPath[_MAX_PATH] = {0};

#ifdef UNICODE
	_wfullpath(pszFullPath, strPathName, _MAX_PATH);
#else
	_fullpath(pszFullPath, strPathName, _MAX_PATH);
#endif

	return CString(pszFullPath);
}

/**
*
*/
CString MakeUserPath(CString strPathName)
{
	TCHAR pszFullPath[_MAX_PATH] = {0};

	HRESULT hRes = ::SHGetSpecialFolderPath(NULL, pszFullPath, CSIDL_PERSONAL, FALSE);

	CString strUserPath = CString(pszFullPath) + _T("\\My Games\\Amazing Mahjongg 3D\\") + strPathName;

	return strUserPath;
}

/**
 * @brief MakeFullPath
 *
 * Make full path for a file name
 * 
 * @param strFileName
 * @return CString
 */
CString MakeFullPath(CString strFileName)
{
	TCHAR pszFullPath[_MAX_PATH] = {0};
	TCHAR pszDrive[_MAX_DRIVE] = {0};
	TCHAR pszDir[_MAX_DIR] = {0};

	GetModuleFileName(_Module.m_hInst, pszFullPath, _MAX_PATH);

#ifdef UNICODE
	_wsplitpath_s(pszFullPath, pszDrive, _MAX_DRIVE, pszDir, _MAX_DIR, NULL, 0, NULL, 0);
#else
	_splitpath_s(pszFullPath, pszDrive, pszDir, NULL, NULL);
#endif

	CString str = CString(pszDrive) + CString(pszDir) + strFileName;

#ifdef UNICODE
	_wfullpath(pszFullPath, (LPCTSTR)str, _MAX_PATH);
#else
	_fullpath(pszFullPath, str, _MAX_PATH);
#endif

	return CString(pszFullPath);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
void Sound(UINT nIDResource)
{
	if (g_AppSettings.m_bSound)
	{
		PlaySound(NULL, NULL, SND_PURGE);

		PlaySound(MAKEINTRESOURCE(nIDResource), _Module.m_hInstResource, SND_RESOURCE | SND_ASYNC);
	}
}
