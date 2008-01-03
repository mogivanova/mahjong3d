/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: Registry.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#include "stdafx.h"
#include "Registry.h"

CRegistry::CRegistry()
{
	m_hKey = NULL;
}

CRegistry::CRegistry(HKEY hKey, LPCTSTR szSubKey)
{
	m_hKey = NULL;
	Open(hKey, szSubKey);
}

CRegistry::~CRegistry()
{
	Close();
}

bool CRegistry::Open(HKEY hKey, LPCTSTR szSubKey)
{
	DWORD dwCreation;
	bool bResult = RegCreateKeyEx(hKey, szSubKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL,
																&m_hKey, &dwCreation) == ERROR_SUCCESS;
	return bResult;
}

bool CRegistry::Close()
{
	if (m_hKey != NULL)
		RegCloseKey(m_hKey);

	m_hKey = NULL;

	return true;
}

void CRegistry::WriteBool(LPCTSTR szName, bool bValue)
{
	DWORD dwValue = bValue;
	RegSetValueEx(m_hKey, szName, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
}

void CRegistry::WriteDWORD(LPCTSTR szName, DWORD dwValue)
{
	RegSetValueEx(m_hKey, szName, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
}

void CRegistry::WriteString(LPCTSTR szName, CString strValue)
{
	RegSetValueEx(m_hKey, szName, 0, REG_SZ, (LPBYTE)(LPCTSTR)strValue, sizeof(TCHAR) * strValue.GetLength());
}

bool CRegistry::ReadBool(LPCTSTR szName, bool bDefaultValue)
{
	DWORD dwResult = 0L;
	DWORD dwSize = sizeof(DWORD);

	if (RegQueryValueEx(m_hKey, szName, 0, NULL, (LPBYTE)&dwResult, &dwSize) != ERROR_SUCCESS)
	{
		return bDefaultValue;
	}

	return dwResult == TRUE;
}

DWORD CRegistry::ReadDWORD(LPCTSTR szName, DWORD dwDefaultValue)
{
	DWORD dwResult = 0L;
	DWORD dwSize = sizeof(DWORD);

	if (RegQueryValueEx(m_hKey, szName, 0, NULL, (LPBYTE)&dwResult, &dwSize) != ERROR_SUCCESS)
	{
		dwResult = dwDefaultValue;
	}

	return dwResult;
}

CString CRegistry::ReadString(LPCTSTR szName, CString strDefaultValue)
{
	CString strValue;
	DWORD dwSize = 0L;

	if (RegQueryValueEx(m_hKey, szName, 0, NULL, (LPBYTE)NULL, &dwSize) == ERROR_SUCCESS)
	{
		TCHAR* chBuffer = new TCHAR[dwSize + 1];
		memset(chBuffer, 0, sizeof(TCHAR) * (dwSize + 1));
		RegQueryValueEx(m_hKey, szName, 0, NULL, (LPBYTE)chBuffer, &dwSize);
		strValue = chBuffer;
		delete [] chBuffer;
	}
	else
	{
		strValue = strDefaultValue;
	}

	return strValue;
}
