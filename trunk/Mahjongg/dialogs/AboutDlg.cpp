/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: AboutDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.08.05 0:46 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"

#include "aboutdlg.h"
#include "RegistrationDlg.h"

#include "..\Service\Registry.h"
#include ".\aboutdlg.h"

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	LocalizeControls();

	/*
	#if(_WIN32_WINNT >= 0x0500)
	SetWindowLong(GWL_EXSTYLE, GetWindowLong(GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(m_hWnd, RGB(255, 255, 255), 230, LWA_ALPHA);
	#endif
	*/

	FillControls();

	m_hPointer = LoadCursor(NULL, IDC_ARROW);
	m_hHand = LoadCursor(_Module.m_hInstResource, MAKEINTRESOURCE(IDC_HANDCUR));

	return TRUE;
}


LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


LRESULT CAboutDlg::OnBnClickedLicense(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}


LRESULT CAboutDlg::OnBnClickedRegister(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DialogBox(NULL, MAKEINTRESOURCE(IDD_REGISTER), m_hWnd, (DLGPROC)RegisterDlgProc);

	return 0;
}


LRESULT CAboutDlg::OnBnClickedBuyNow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(GetDesktopWindow(), _T("open"), LoadResourceString(IDS_REGISTRATION), NULL, NULL, 0);

	return 0;
}


LRESULT CAboutDlg::OnBnClickedEmail(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(GetDesktopWindow(), _T("open"),
							 _T("mailto:info@mahjongg4u.com?subject=Amazing%20Mahjongg%203D"), NULL, NULL, 0);

	return 0;
}
LRESULT CAboutDlg::OnBnClickedWWW(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(GetDesktopWindow(), _T("open"), LoadResourceString(IDS_WWW), NULL, NULL, 0);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  [6/1/2003]
LRESULT CAboutDlg::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hWnd = (HWND)wParam;

	if (hWnd == GetDlgItem(IDC_EMAIL) ||
			//hWnd == GetDlgItem(IDC_DISTRIBUTOR_LOGO) ||
			hWnd == GetDlgItem(IDC_WWW))
	{
		::SetCursor(m_hHand);
		return TRUE;
	}
	else
	{
		::SetCursor(m_hPointer);
		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CModuleVersion::Get(LPCTSTR pszModulePath /* = NULL */)
{
	bool bResult = false;
	TCHAR* szFileName = NULL;
	BYTE* pBuffer = NULL;

	m_wMajor = m_wMinor = m_wInternal = m_wBuild = 0;
	m_bDebug = false;

	szFileName = new TCHAR[MAX_PATH];

	// get path to program
	GetModuleFileName(GetModuleHandle(pszModulePath), szFileName, MAX_PATH);

	// get size of info
	DWORD dwInfoSize = GetFileVersionInfoSize(szFileName, NULL);

	if (dwInfoSize != 0)
	{
		// allocate buffer
		pBuffer = new BYTE[dwInfoSize];

		if (pBuffer != NULL)
		{
			// get version info
			if (GetFileVersionInfo( szFileName, NULL, dwInfoSize, pBuffer) != 0)
			{
				// get fixed values
				VS_FIXEDFILEINFO* pVersion = NULL;
				UINT uLen = 0;
				VerQueryValue(pBuffer, _T("\\"), (LPVOID*)&pVersion, &uLen);

				if (pVersion != NULL)
				{
					// get version data
					m_wMajor = HIWORD(pVersion->dwProductVersionMS);
					m_wMinor = LOWORD(pVersion->dwProductVersionMS);
					m_wInternal = HIWORD(pVersion->dwProductVersionLS);
					m_wBuild = LOWORD(pVersion->dwProductVersionLS);

					m_bDebug = (pVersion->dwFileFlags & VS_FF_DEBUG) != 0;

					bResult = true;
				}
			}
		}
	}

	if (szFileName != NULL)
		delete [] szFileName;

	if (pBuffer != NULL)
		delete [] pBuffer;

	return bResult;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CAboutDlg::OnStnClickedDistributorLogo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//ShellExecute(GetDesktopWindow(), _T("open"), LoadResourceString(IDS_DISTRIBUTOR_WWW), NULL, NULL, 0);
	return 0;
}

/**
 *
 */
HRESULT CAboutDlg::FillControls()
{
	m_ctrlAboutStatic = GetDlgItem(IDC_ABOUT_GAME);

	CModuleVersion ver;

	if (ver.Get())
	{
		CString strVersion;
		strVersion.Format(IDS_VERSION_FORMAT, ver.m_wMajor, ver.m_wMinor);
		m_ctrlAboutStatic.SetWindowText(strVersion);
	}

	CFont* m_objFont = new CFont();

	m_objFont->CreateFont(8, 0, 0, 0, 600, FALSE, TRUE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS,
												CLIP_DEFAULT_PRECIS, DRAFT_QUALITY,
												DEFAULT_PITCH | FF_SWISS, _T("Helv"));

	m_ctrlEmail = GetDlgItem(IDC_EMAIL);
	m_ctrlEmail.SetFont(m_objFont->m_hFont, FALSE);

	m_ctrlWWW = GetDlgItem(IDC_WWW);
	m_ctrlWWW.SetFont(m_objFont->m_hFont, FALSE);

	//m_ctrlAboutText = GetDlgItem(IDC_ABOUT_TEXT);
	//m_ctrlRegistered = GetDlgItem(IDC_REGISTERED);

	//m_ctrlOk.Attach(GetDlgItem(IDC_REGISTER));

	return 0;
}
