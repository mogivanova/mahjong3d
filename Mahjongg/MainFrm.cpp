/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MainFrm.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 17.01.05 23:51 $
*
*================================================================================================*/

#include "stdafx.h"
#include "resource.h"

#include "MainFrm.h"

#include "Dialogs\aboutdlg.h"
#include "Dialogs\SetupExpertDlg.h"
#include "Dialogs\DemoDlg.h"
#include "Dialogs\Solitaire\PlayerNameDlg.h"
#include "Dialogs\Solitaire\BeginnerHintDlg.h"
#include "Service\Random.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMainFrame::CMainFrame() :
		m_objSolitaireView(this)
{
	m_pCurrentView = NULL;

	m_hWndClient = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMainFrame::~CMainFrame()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	if (m_pCurrentView != NULL && pMsg->hwnd == m_pCurrentView->m_hWnd)
		return m_pCurrentView->PreTranslateMessage(pMsg);

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::OnIdle()
{
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HMENU hMenu = GetMenu();

	CRect rcClient;
	GetClientRect(&rcClient);
	m_wndDefaultStatusBar.Create(m_hWnd, rcClient, NULL, WS_CHILD);

	RecreateStatusBar();

	// menu bar
	UIAddMenuBar(m_hWnd);

	UIEnable(ID_PLAY_UNDO, FALSE, FALSE);
	UIEnable(ID_GAME_REPEAT, FALSE, FALSE);

	UIEnable(ID_GAME_DEMO, FALSE , FALSE);

	UIEnable(ID_PLAY_HINT, FALSE, FALSE);
	UIEnable(ID_PLAY_SHUFFLE, FALSE, FALSE);

	UIUpdateMenuBar(FALSE, TRUE);

	//m_wndStatusBar.ShowWindow(SW_SHOW);
	//m_wndStatusBar.UpdateWindow();

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);

	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
LRESULT CMainFrame::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DefWindowProc(uMsg, wParam, lParam);
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
LRESULT CMainFrame::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rectClient;
	GetClientRect(&rectClient);

	if (m_hWndStatusBar != NULL)
	{
		CRect rcStatusBar;
		::GetClientRect(m_hWndStatusBar, rcStatusBar);
		rectClient.bottom -= rcStatusBar.Height();
	}

	HBRUSH hB = CreateSolidBrush(RGB(64, 56, 37));

	FillRect((HDC)wParam, &rectClient, hB);

	DeleteObject(hB);

	bHandled = TRUE;
	return 1;
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
LRESULT CMainFrame::OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
void CMainFrame::FinalConstruct()
{
	// initialize driver
	HDC hWndDC = GetWindowDC();
	int nBitsPerPixel = GetDeviceCaps(hWndDC, BITSPIXEL);

	if (nBitsPerPixel == 1)
	{
		MessageBox(g_LangManager.GetString("error_mono"),
							 g_LangManager.GetString("caption_error"), MB_OK | MB_ICONSTOP);
		return;
	}

	if (nBitsPerPixel == 4)
	{
		MessageBox(g_LangManager.GetString("error_16colors"),
							 g_LangManager.GetString("caption_error"), MB_OK | MB_ICONSTOP);
		return;
	}

	if (nBitsPerPixel == 8)
	{
		MessageBox(g_LangManager.GetString("error_256colors"),
							 g_LangManager.GetString("caption_error"), MB_OK | MB_ICONSTOP);
		return;
	}

	if (!m_objDriver.Initialize())
	{
		MessageBox(g_LangManager.GetString("error_no_driver"),
							 g_LangManager.GetString("caption_error"), MB_OK | MB_ICONSTOP);
	}
	else
	{
		m_objSolitaireView.SetDriver(&m_objDriver);

		BOOL bHandled = FALSE;

		switch (g_AppSettings.m_enmGameMode)
		{

		case CAppSettings::eSolitaire:
			OnNewSolitaireGame(0, 0, 0, bHandled);
			break;

		default:
			break;
		}
	}
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
LRESULT CMainFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0) ;
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPRECT pRect = (LPRECT)lParam;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPMINMAXINFO pInfo = (LPMINMAXINFO)lParam;

	pInfo->ptMinTrackSize.x = 600;
	pInfo->ptMinTrackSize.y = 460;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnPlayerName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CPlayerNameDlg dlgPlayer;
	dlgPlayer.DoModal();
	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CMainFrame::OnSettingsOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bool bUpdate = false;

	if (g_AppSettings.m_enmInterfaceMode == CAppSettings::eBeginner)
	{
		CSetupExpertDlg dlg;

		if (dlg.DoModal() == IDOK)
			bUpdate = true;
	}
	else
	{
		CSetupExpertDlg dlg;

		if (dlg.DoModal() == IDOK)
			bUpdate = true;
	}

	if (bUpdate)
	{
		g_LangManager.SetLanguage(g_AppSettings.m_strLanguage);

		SetRedraw(FALSE);
		HMENU hMenu = GetMenu();
		SetMenu(NULL);
		::DestroyMenu(hMenu);
		hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MJ_EXPERT));
		RecreateStatusBar();
		SetMenu(hMenu);
		SetRedraw(TRUE);

		if (m_pCurrentView != NULL)
		{
			CRect rcClient;
			GetClientRect(&rcClient);

			m_pCurrentView->OnUpdateOptions();
			m_pCurrentView->UpdateStatus();
			m_pCurrentView->UpdateUI();
		}

		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnMoveSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CRect rect;
	GetWindowRect(&rect);

	if (uMsg == WM_SIZE)
	{
		if (wParam == SIZE_MAXIMIZED)
		{
			g_AppSettings.m_bWndMaximized = true;
		}
		else if (wParam == SIZE_RESTORED)
		{
			g_AppSettings.m_bWndMaximized = false;

			g_AppSettings.m_nWndPosX = rect.left;
			g_AppSettings.m_nWndPosY = rect.top;
			g_AppSettings.m_nWndWidth = rect.right - rect.left;
			g_AppSettings.m_nWndHeight = rect.bottom - rect.top;
		}

		if (m_hWndStatusBar != NULL)
		{
			SendMessage(m_hWndStatusBar, WM_SIZE, 0, 0);
		}
	}
	else
	{
		g_AppSettings.m_nWndPosX = rect.left;
		g_AppSettings.m_nWndPosY = rect.top;
	}

	bHandled = FALSE;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnHelpContents(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strHelpFile = _T("Mahjongg.chm");

	if (g_LangManager.GetLanguage() == _T("German"))
	{
		strHelpFile = _T("Mahjongg_de.chm");
	}
	else if (g_LangManager.GetLanguage() == _T("French"))
	{
		strHelpFile = _T("Mahjongg_fr.chm");
	}

	CString strHelpPath = MakeInstallPath(_T("Help\\")) + strHelpFile;

	HWND hHelpWnd = HtmlHelp(m_hWnd, strHelpPath, HH_DISPLAY_TOC, NULL);

	/*
	HH_WINTYPE WinType;
	ZeroMemory(&WinType, sizeof(HH_WINTYPE));
	WinType.cbStruct = sizeof(HH_WINTYPE);
	HtmlHelp(m_hWnd, strHelpPath, HH_SET_WIN_TYPE, (DWORD)&WinType);
	*/

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnHelpIndex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strHelpFile = _T("Mahjongg.chm");

	if (g_LangManager.GetLanguage() == _T("German"))
	{
		strHelpFile = _T("Mahjongg_de.chm");
	}
	else if (g_LangManager.GetLanguage() == _T("French"))
	{
		strHelpFile = _T("Mahjongg_fr.chm");
	}

	CString strHelpPath = MakeInstallPath(_T("Help\\")) + strHelpFile;

	HWND hHelpWnd = HtmlHelp(m_hWnd, strHelpPath, HH_DISPLAY_INDEX, NULL);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnModeExpert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (g_AppSettings.m_bTextHints)
	{
		CBeginnerHintDlg dlg;
		dlg.m_strMessage = g_LangManager.GetString("str_switch_expert_mode");
		dlg.DoModal();
	}

	HMENU hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MJ_EXPERT));

	if (hMenu != NULL)
		SetMenu(hMenu);

	g_AppSettings.m_enmInterfaceMode = CAppSettings::eExpert;

	UpdateUI();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnModeBeginner(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	/*
	HMENU hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MJ_BEGINNER));
	if(hMenu != NULL)
	  SetMenu(hMenu);

	g_AppSettings.m_enmInterfaceMode = CAppSettings::eBeginner;

	*/
	UpdateUI();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnNewSolitaireGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// calculate client rectangle
	CRect rectClient;
	GetClientRect(&rectClient);

	CRect rectStatusBar;
	::GetWindowRect(m_hWndStatusBar, &rectStatusBar);
	rectClient.bottom -= (rectStatusBar.bottom - rectStatusBar.top);

	// create view window
	m_objSolitaireView.Create(m_hWnd, rectClient, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_objSolitaireView.Init();

	// load menu
	SetRedraw(FALSE);
	::DestroyMenu(GetMenu());
	HMENU hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MJ_EXPERT));
	RecreateStatusBar();
	// set menu
	SetMenu(hMenu);
	SetRedraw(TRUE);

	// set current view
	m_pCurrentView = &m_objSolitaireView;
	m_pCurrentView->Activate();
	m_objSolitaireView.UpdateView();

	m_hWndClient = m_pCurrentView->m_hWnd;

	// set settings
	g_AppSettings.m_enmGameMode = CAppSettings::eSolitaire;

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);

	return 0;
}


/*------------------------------===---------------------------------------------->8-- Cut here ----
*
*/
bool CMainFrame::UpdateUI()
{
	if (m_pCurrentView != NULL)
	{
		m_pCurrentView->UpdateUI();
		m_pCurrentView->UpdateStatus();
	}

	UIUpdateMenuBar(FALSE, TRUE);

	return true;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CMainFrame::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL fActive = LOWORD(wParam);

	if (fActive == WA_ACTIVE || fActive == WA_CLICKACTIVE)
	{
		if (m_pCurrentView != NULL)
			m_pCurrentView->Activate();
	}
	else if (fActive == WA_INACTIVE)
	{
		if (m_pCurrentView != NULL)
			m_pCurrentView->Deactivate();
	}

	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CMainFrame::StartFireworks()
{
	// load menu
	::DestroyMenu(GetMenu());
	//HMENU hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MJ_FIREWORKS));

	// set menu
	SetMenu(NULL);

	UpdateUI();

	SendMessage(WM_NCPAINT, 1, 0);

	return true;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CMainFrame::StopFireworks()
{
	// load menu
	//::DestroyMenu(GetMenu());
	HMENU hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MJ_EXPERT));

	// set menu
	SetMenu(hMenu);

	//
	UpdateUI();

	SendMessage(WM_NCPAINT, 1, 0);

	return true;
}

/** Update menu in default view mode (without skin)
*
*/
bool CMainFrame::UpdateMenu(HMENU hMenu)
{
	const int numItems = GetMenuItemCount(hMenu);

	for (int i = 0; i < numItems; i++)
	{
		MENUITEMINFO mii = {0};
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_ID | MIIM_TYPE | MIIM_SUBMENU | MIIM_DATA;

		_VERIFY(GetMenuItemInfo(hMenu, i, TRUE, &mii));

		if (mii.fType == MFT_STRING)
		{
			TCHAR* chBuffer = new TCHAR[mii.cch + 1];
			mii.cch = mii.cch + 1;
			mii.dwTypeData = chBuffer;
			_VERIFY(GetMenuItemInfo(hMenu, i, TRUE, &mii));

			CString strMenu = g_LangManager.GetString(chBuffer);
			delete [] chBuffer;

			mii.dwTypeData = (LPTSTR)(LPCTSTR)strMenu;
			mii.cch = strMenu.GetLength();

			mii.fMask = MIIM_TYPE | MIIM_DATA;

			/*
			if(mii.wID >= ID_MENU_MORE_1 && mii.wID <= ID_MENU_MORE_13)
			{
			  mii.fType |= MFT_OWNERDRAW;
			  mii.dwItemData = (DWORD)hMenu;
			}
			*/

			_VERIFY(SetMenuItemInfo(hMenu, i, TRUE, &mii));
		}

		if (mii.hSubMenu != NULL)
		{
			UpdateMenu(mii.hSubMenu);
		}
	}

	return true;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CMainFrame::RecreateStatusBar()
{
	m_hWndStatusBar = m_wndDefaultStatusBar;
	m_wndDefaultStatusBar.ShowWindow(SW_SHOW);

	INT nWidths[] = {100, 200, -1};

	SendMessage(m_hWndStatusBar, SB_SETPARTS, 3, (LPARAM)nWidths);

	return true;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CMainFrame::OnSwitchLanguage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	switch (wID)
	{

	case ID_LANG_GERMAN:
		g_AppSettings.m_strLanguage = _T("German");
		break;

	case ID_LANG_FRENCH:
		g_AppSettings.m_strLanguage = _T("French");
		break;

	case ID_LANG_ENGLISH:

	default:
		g_AppSettings.m_strLanguage = _T("English");
		break;
	}

	g_LangManager.SetLanguage(g_AppSettings.m_strLanguage);

	HMENU hMenu = GetMenu();
	SetMenu(NULL);
	::DestroyMenu(hMenu);
	hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MJ_EXPERT));

	RecreateStatusBar();

	SetMenu(hMenu);

	SetRedraw(TRUE);

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);

	return 0;
}

/**
 *
 */
LRESULT CMainFrame::OnHelpWWW(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShellExecute(GetDesktopWindow(), _T("open"), LoadResourceString(IDS_WWW), NULL, NULL, 0);

	return 0;
}

/*
LRESULT CMainFrame::OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  if(wParam == 0)
  {
    // menu item
    LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT)lParam;

    HMENU hMenu = (HMENU)pmis->itemData;

    MENUITEMINFO mii = {0};
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_TYPE;
    ::GetMenuItemInfo(hMenu, pmis->itemID, FALSE, &mii);

    bool bTopMenu = ::GetMenu(m_hWnd) == hMenu;

    if((mii.fType & MFT_SEPARATOR) == MFT_SEPARATOR)
    {
      pmis->itemWidth = 10;
      pmis->itemHeight = 6;
    }
    else
    {
      TCHAR buffer[256] = {0};
      if(GetMenuString(hMenu, pmis->itemID, buffer, sizeof(buffer) - 1, MF_BYCOMMAND))
      {
        CString strMenuItem = g_LangManager.GetString(buffer);

        CClientDC dc(m_hWnd);
        //CFont* pft = CFont::FromHandle(hMenuFont ? hMenuFont : hGuiFont);
        //CFont* of = dc.SelectObject(pft);
        CSize osz = dc.GetOutputTabbedTextExtent(strMenuItem, 0, NULL);

          pmis->itemHeight = szImage.cy + 5;
          if (osz.cy > (int) pmis->itemHeight)
            pmis->itemHeight = (int) osz.cy;
          pmis->itemWidth  = osz.cx + 2 + 15;
          pmis->itemWidth += pmis->itemHeight > (UINT) szImage.cx ? (UINT) pmis->itemHeight : (UINT) szImage.cx;

          //dc.SelectObject(of);
      }
    }

    bHandled = TRUE;
    return TRUE;
  }
  else
  {
    // not a menu item
  }

  bHandled = FALSE;
  return 0;
}

LRESULT CMainFrame::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  if(wParam == 0)
  {
    // menu item
    LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;

    HMENU hMenu = (HMENU)pdis->itemData;

    MENUITEMINFO mii = {0};
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_TYPE;

    ::GetMenuItemInfo(hMenu, pdis->itemID, FALSE, &mii);

    if((mii.fType & MFT_SEPARATOR) == MFT_SEPARATOR)
    {
    }
    else
    {
      TCHAR buffer[256] = {0};
      if(GetMenuString(hMenu, pdis->itemID, buffer, sizeof(buffer) - 1, MF_BYCOMMAND))
      {
        CString strMenuItem = g_LangManager.GetString(buffer);


      }
    }

    bHandled = TRUE;
    return TRUE;
  }
  else
  {
    // not a menu item
  }

  return TRUE;
}
*/

/**
 *
 */
LRESULT CMainFrame::OnMenuMoreGames(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CAtlMap<WORD, CString> mapIDUrl;

	CString strURL = mapIDUrl[wID];

	if (strURL != _T(""))
		ShellExecute(GetDesktopWindow(), _T("open"), strURL, NULL, NULL, 0);

	return 0;
}