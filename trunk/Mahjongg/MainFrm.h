/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MainFrm.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 30.03.04 10:08 $
*
*================================================================================================*/

#pragma once

#include "Service\OpenGL\OpenGLDriver.h"

#include "Game\Solitaire\SolitaireView.h"

#define CHAIN_VIEW_MSG_MAP(pView) \
{ \
	if(pView != NULL && uMsg == WM_COMMAND && pView->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult)) \
	return TRUE; \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CMainFrame :
	public CFrameWindowImpl<CMainFrame>,
	public CUpdateUI<CMainFrame>,
	public CMessageFilter,
	public CIdleHandler
{

protected:
	COpenGLDriver m_objDriver;

	CMahjonggView* m_pCurrentView;

	CSolitaireView m_objSolitaireView;

	CStatusBarCtrl m_wndDefaultStatusBar;

public:
	CMainFrame();
	~CMainFrame();

public:
	void FinalConstruct();

	// class info
	static CFrameWndClassInfo& GetWndClassInfo()
	{
		static CFrameWndClassInfo wc =
		{
			{ sizeof(WNDCLASSEX), 0, StartWindowProc,
			0, 0, NULL, NULL, NULL, CreateSolidBrush(RGB(0, 0, 0)), NULL, _T("AmazingMahjongg3D"), NULL },
			NULL, NULL, IDC_ARROW, TRUE, 0, _T(""), IDR_MAINFRAME
		};
		return wc;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	// message map
	BEGIN_MSG_MAP(CMainFrame)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)

		MESSAGE_HANDLER(WM_CREATE, OnCreate)

		MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)

		MESSAGE_HANDLER(WM_MOVE, OnMoveSize)
		MESSAGE_HANDLER(WM_SIZE, OnMoveSize)

		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)

		COMMAND_ID_HANDLER(ID_GAME_NEW_SOLITAIRE, OnNewSolitaireGame)

		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_GAME_PLAYERNAME, OnPlayerName)

		COMMAND_ID_HANDLER(ID_LANG_ENGLISH, OnSwitchLanguage)
		COMMAND_ID_HANDLER(ID_LANG_GERMAN, OnSwitchLanguage)
		COMMAND_ID_HANDLER(ID_LANG_FRENCH, OnSwitchLanguage)

		COMMAND_ID_HANDLER(ID_GAME_OPTIONS, OnSettingsOptions)

		COMMAND_ID_HANDLER(ID_HELP_CONTENTS, OnHelpContents)
		COMMAND_ID_HANDLER(ID_HELP_INDEX, OnHelpIndex)

		COMMAND_ID_HANDLER(ID_MODE_EXPERT, OnModeExpert)
		COMMAND_ID_HANDLER(ID_MODE_BEGINNER, OnModeBeginner)

		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)

		CHAIN_VIEW_MSG_MAP(m_pCurrentView);

	END_MSG_MAP()

		// update user interface
		virtual bool UpdateUI(void);

	// update ui map
	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_GAME_NEW, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_GAME_REPEAT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_GAME_SAVE, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_GAME_LOAD, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_APP_EXIT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_PLAY_UNDO, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_GAME_PLAYERNAME, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_PLAY_SCORES, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_VIEW_ROTATE_LEFT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_VIEW_ROTATE_RIGHT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_VIEW_ROTATE_UP, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_VIEW_ROTATED_OWN, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_VIEW_ZOOMIN, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_VIEW_ZOOMOUT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_VIEW_DEFAULT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_BACKGROUND_DEFAULT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_BACKGROUND_COLOR, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_BACKGROUND_LOAD, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_TILESET_DEFAULT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_TILESET_LOAD, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_LAYOUT_DEFAULT, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_LAYOUT_LOAD, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_GAME_OPTIONS, UPDUI_MENUBAR)
		UPDATE_ELEMENT(ID_HELP_CONTENTS, UPDUI_MENUBAR)

	END_UPDATE_UI_MAP()

public:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMoveSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNewSolitaireGame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAppAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnPlayerName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSwitchLanguage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSettingsOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnHelpContents(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHelpIndex(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHelpWWW(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnModeExpert(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnModeBeginner(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnMenuMoreGames(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(uMsg, wParam, lParam);
	}

public:
	bool StartFireworks();
	bool StopFireworks();

	bool UpdateMenu(HMENU hMenu);
	bool RecreateStatusBar();
	// LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
};
