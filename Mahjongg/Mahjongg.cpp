#include "stdafx.h"

#include "resource.h"

#include "Game\MahjonggView.h"

#include "MainFrm.h"

#include "Service\random.h"

#include "Service\Settings\MJAppSettings.h"

#include "Dialogs\RegistrationDlg.h"

CAppModule _Module;

CAppSettings g_AppSettings;

CLangManager g_LangManager;

SYSTEMTIME g_tmProgramStarted;

/**
 * @brief ResolveVersion
 *
 * ResolveVersion
 *
 * @return void
 */
void ResolveVersion()
{
	memset(&g_osvi, 0, sizeof(OSVERSIONINFO));
	g_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&g_osvi);
}

/**
 * @brief SetCurrentPath
 *
 * SetCurrentPath
 *
 * @return void
 */
void SetCurrentPath()
{
	TCHAR pszFullPath[_MAX_PATH] = {0};
	TCHAR pszDrive[_MAX_DRIVE] = {0};
	TCHAR pszDir[_MAX_DIR] = {0};
	TCHAR pszFname[_MAX_FNAME] = {0};
	TCHAR pszExt[_MAX_EXT] = {0};

	GetModuleFileName(_Module.m_hInst, pszFullPath, _MAX_PATH);

#ifdef UNICODE
	_wsplitpath_s(pszFullPath, pszDrive, _MAX_DRIVE, pszDir, _MAX_DIR, pszFname, _MAX_FNAME, pszExt, _MAX_EXT);
#else
	_splitpath(pszFullPath, pszDrive, pszDir, pszFname, pszExt);
#endif

	CString strModulePath = CString(pszDrive) + CString(pszDir);

#ifdef UNICODE
	_wchdir(strModulePath);
#else
	_chdir(strModulePath);
#endif
}

/**
 *
 */
int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	InitCommonControls();

	// go to
	SetCurrentPath();

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	//
	ResolveVersion();

	//
	::GetLocalTime(&g_tmProgramStarted);

	// load language settings
	g_LangManager.Initialize();

	// load ini
	g_AppSettings.Load();

	if (g_AppSettings.m_strLanguage == _T("User"))
	{
		LANGID langID = GetUserDefaultLangID();

		int nPrimaryLang = (langID & 0x3FF);
		int nSecondaryLang = (langID >> 10);

		g_AppSettings.m_strLanguage = _T("English");

		if (nPrimaryLang == 0x07)
			g_AppSettings.m_strLanguage = _T("German");
		else if (nPrimaryLang == 0x0C)
			g_AppSettings.m_strLanguage = _T("French");
		else if (nPrimaryLang == 0x19)
			g_AppSettings.m_strLanguage = _T("Russian");
	}

	g_LangManager.SetLanguage(g_AppSettings.m_strLanguage);

	//
	CRect rectWnd(g_AppSettings.m_nWndPosX, g_AppSettings.m_nWndPosY,
								g_AppSettings.m_nWndPosX + g_AppSettings.m_nWndWidth, g_AppSettings.m_nWndPosY + g_AppSettings.m_nWndHeight);

	CRect rcScr(CPoint(0, 0), CSize(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN)));
	rcScr.DeflateRect(40, 40);

	CRect rcCaption = rectWnd;
	rcCaption.bottom = rcCaption.top + ::GetSystemMetrics(SM_CYCAPTION);

	CRect rcOnScreen;
	rcOnScreen.IntersectRect(&rcCaption, &rcScr);

	if (rcOnScreen.IsRectEmpty())
	{
		rectWnd = CRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
	}

	//
	CMainFrame wndMain;

	bool bMaximized = g_AppSettings.m_bWndMaximized;

	if (wndMain.CreateEx(NULL, &rectWnd) == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	if (bMaximized)
	{
		wndMain.ShowWindow(SW_MAXIMIZE);
	}
	else
	{
		wndMain.ShowWindow(SW_SHOW);
	}

	wndMain.FinalConstruct();

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	// save ini
	g_AppSettings.Save();

	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	int nRet = 0;

	// find previously started copy of program
	HWND hWndCopy = ::FindWindow(_T("AmazingMahjongg3D"), _T("Amazing Mahjongg 3D"));

	if (hWndCopy != NULL)
	{
		::ShowWindow(hWndCopy, SW_RESTORE);
		::SetForegroundWindow(hWndCopy);
		return 0;
	}

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	randomizeMT();

	HRESULT hRes = _Module.Init(NULL, hInstance);

	ATLASSERT(SUCCEEDED(hRes));

	nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();

	return nRet;
}
