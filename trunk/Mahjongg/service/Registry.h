/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: Registry.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#pragma once

class CRegistry
{

protected:
	HKEY m_hKey;

public:
	CRegistry();
	CRegistry(HKEY hKey, LPCTSTR szSubKey);
	~CRegistry();

	bool Open(HKEY hKey, LPCTSTR szSubKey);
	bool Close();

	void WriteBool(LPCTSTR szName, bool bValue);
	void WriteDWORD(LPCTSTR szName, DWORD dwValue);
	void WriteString(LPCTSTR szName, CString strValue);

	bool ReadBool(LPCTSTR szName, bool bDefaultValue);
	DWORD ReadDWORD(LPCTSTR szName, DWORD dwDefaultValue);
	CString ReadString(LPCTSTR szName, CString strDefaultValue);
};
