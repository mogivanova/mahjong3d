/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: LocalizableDialog.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:47 $
*
*================================================================================================*/

#pragma once

template<class T>

class CLocalizableDialog
{

public:
	bool LocalizeControls();
};

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
inline BOOL CALLBACK LocalizeChildWindow(HWND hWnd, LPARAM lParam)
{
	TCHAR szBuffer[256];
	GetWindowText(hWnd, szBuffer, 256);
	CString strCaption = g_LangManager.GetString(szBuffer);

	if (strCaption != szBuffer)
		SetWindowText(hWnd, strCaption);

	return TRUE;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
template<class T>
bool CLocalizableDialog<T>::LocalizeControls()
{
	T* pT = static_cast<T*>(this);

	TCHAR szBuffer[256];
	GetWindowText(pT->m_hWnd, szBuffer, 256);
	CString strCaption = g_LangManager.GetString(szBuffer);

	if (strCaption != szBuffer)
		SetWindowText(pT->m_hWnd, strCaption);

	EnumChildWindows(pT->m_hWnd, LocalizeChildWindow, 0);

	return true;
};