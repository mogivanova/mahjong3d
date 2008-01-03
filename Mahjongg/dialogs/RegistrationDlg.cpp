/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: RegistrationDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:51 $
*
*================================================================================================*/

#include "stdafx.h"

#include "..\resource.h"

//---------------------------------------------------------------------
BOOL RegisterString(LPCTSTR pszKey, LPTSTR pszValue, LPTSTR pszData)
{
	HKEY hKey;
	DWORD dwDisposition;

	if (ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, pszKey, 0, NULL, REG_OPTION_NON_VOLATILE,
																			KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition))
	{
		return FALSE;
	}

	if (ERROR_SUCCESS != RegSetValueEx(hKey, pszValue, 0, REG_SZ, (LPBYTE)pszData, lstrlen(pszData)))
	{
		RegCloseKey(hKey);
		return FALSE;
	}

	RegCloseKey(hKey);

	return TRUE;
}

//---------------------------------------------------------------------
DWORD APIENTRY RegisterDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR szBuffer[1024];
	int   Size;

	switch (msg)
	{

	case WM_COMMAND:

		switch (LOWORD(wParam))
		{

		case IDOK:
			Size = GetDlgItemText(hwnd, IDC_REGKEY, szBuffer, 1024);

			if (Size > 0)
			{
				szBuffer[Size] = '\0';

				if (RegisterString(g_AppSettings.m_pszRegistryKeyName, _T("Key"), (LPTSTR)szBuffer))
					MessageBox(hwnd, _T("Thank you for registration!\nPlease restart the application."), _T("OK"), MB_OK);
			}

			EndDialog(hwnd, IDOK);

			break;

		case IDC_PASTE:
			SendMessage(GetDlgItem(hwnd, IDC_REGKEY), WM_PASTE, 0, 0);
			break;

		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}

		break;
	}

	return FALSE;
}

