/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SolitaireView.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 17.01.05 23:45 $
*
*================================================================================================*/

#pragma once

#include "SolitaireGame.h"

#include "..\mahjonggview.h"
#include "resource.h"
#include "ScoreRecord.h"
#include "FireWorks.h"

class CSolitaireView :
			public CMahjonggView
{
	typedef CMahjonggView TParent;

	enum STATUS_MODE
	{
		eNothing,
		eTileName,
		eHint
	};

	enum MUSIC_MODE
	{
		eNoMusic,
		eDefaultMusic,
		eLoadedMusic
	};

public:
	CSolitaireView(CMainFrame* pContainer);
	~CSolitaireView(void);

	// message map

public:

	BEGIN_MSG_MAP_EX(CSolitaireView)
	CHAIN_MSG_MAP(TParent)

	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
	MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)

	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)

	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDown)

	MESSAGE_HANDLER((WM_MOUSELAST + 1), OnMouseWheel) // WM_MOUSEWHEEL

	MESSAGE_HANDLER(WM_TIMER, OnTimer)

	COMMAND_ID_HANDLER(ID_GAME_NEW, OnGameNew)
	COMMAND_ID_HANDLER(ID_GAME_REPEAT, OnGameRepeat)
	COMMAND_ID_HANDLER(ID_GAME_SAVE, OnGameSave)
	COMMAND_ID_HANDLER(ID_GAME_LOAD, OnGameLoad)
	COMMAND_ID_HANDLER(ID_GAME_DEMO, OnGameDemo)
	COMMAND_ID_HANDLER(ID_PLAY_SCORES, OnGameScores)

	COMMAND_ID_HANDLER(ID_PLAY_UNDO, OnHelpUndo)
	COMMAND_ID_HANDLER(ID_PLAY_HINT, OnHelpHint)
	COMMAND_ID_HANDLER(ID_PLAY_SHUFFLE, OnHelpShuffle)

	COMMAND_ID_HANDLER(ID_BACKGROUND_COLOR, OnBackgroundColor)
	COMMAND_ID_HANDLER(ID_BACKGROUND_LOAD, OnBackgroundLoad)
	COMMAND_ID_HANDLER(ID_BACKGROUND_DEFAULT, OnBackgroundDefault)

	COMMAND_ID_HANDLER(ID_LAYOUT_LOAD, OnLayoutLoad)
	COMMAND_ID_HANDLER(ID_LAYOUT_DEFAULT, OnLayoutDefault);

	COMMAND_ID_HANDLER(ID_VIEW_ZOOMIN, OnViewZoomIn);
	COMMAND_ID_HANDLER(ID_VIEW_ZOOMOUT, OnViewZoomOut);
	COMMAND_ID_HANDLER(ID_VIEW_ROTATE_LEFT, OnViewRotateLeft);
	COMMAND_ID_HANDLER(ID_VIEW_ROTATE_RIGHT, OnViewRotateRight);
	COMMAND_ID_HANDLER(ID_VIEW_ROTATE_UP, OnViewRotateUp);
	COMMAND_ID_HANDLER(ID_VIEW_ROTATE_DOWN, OnViewRotateDown);
	COMMAND_ID_HANDLER(ID_VIEW_DEFAULT, OnViewDefault);

	COMMAND_ID_HANDLER(IDC_STOP_FIREWORKS, OnStopFireworks);

#ifdef _DEBUG
	COMMAND_ID_HANDLER(IDC_DEBUG_GAME_OVER, OnDebugGameOver);
	COMMAND_ID_HANDLER(IDC_DEBUG_GAME_WON, OnDebugGameWon);
#endif

	END_MSG_MAP()
	// message handlers

public:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnGameNew(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnGameRepeat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnGameSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnGameLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnGameDemo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnGameScores(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnHelpUndo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnHelpHint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnHelpShuffle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual LRESULT OnBackgroundColor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnBackgroundLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnBackgroundDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual LRESULT OnLayoutLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnLayoutDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual LRESULT OnViewZoomIn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnViewZoomOut(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnViewRotateLeft(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnViewRotateRight(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnViewRotateUp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnViewRotateDown(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnViewDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual LRESULT OnStopFireworks(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	virtual LRESULT OnMusicStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnMusicDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnMusicLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

#ifdef _DEBUG
	virtual LRESULT OnDebugGameOver(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnDebugGameWon(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
#endif

protected:
	const static UINT IDT_SECOND;
	const static UINT IDT_FRAME;
	const static UINT BUFSIZE;

	enum BACKGROUND_MODE
	{
		eBGSolid = 1,
		eBGImage
	};

	enum CURSOR_MODE
	{
		eCursorPointer,     // pointer
		eCursorTile,        // over tile
		eCursorRotate,      // rotate mode
		eCursorHint,        // hint is showing
		eCursorHintRotate   // showing hint and rotate
	};

	enum DETAIL_LEVEL
	{
		eDetailLow,
		eDetailMedium,
		eDetailHigh
	};

	enum DRAW_MODE
	{
		eMahjongg3D,
		eFireWorks,
		eMahjongg
	};

public:
	virtual bool Init();
	virtual bool UpdateView();

	virtual void OnUpdateOptions();

	virtual bool ReloadImages();

protected:
	// draw board
	void DrawBoard(float fBoardCx, float fBoardCy);
	// draw game scene
	virtual bool DrawScene();
	// draw fireworks
	bool DrawFireWorks();

	// create command lists
	virtual bool CreateLists(HDC hDC);
	// setup lights
	virtual bool SetupLights(SYSTEMTIME& tm);

	// pick tile
	bool PickTile(CPoint pt, CSolitairePos& posTile);
	// click on tile
	void OnTileClick(UINT i, UINT j, UINT k);

	//
	void ShowBackgroundImage();

public:
	virtual bool UpdateUI(void);
	virtual void UpdateStatus();

	void GameOver();
	void GameWon();

protected:
	HCURSOR m_arrCursors[5];

	CSolitaireGame m_objGame;
	CScoreTable m_objScores;

	bool m_bShowHint;
	CSimpleArray<CSolitairePos> m_vecHint;

	GLfloat m_fAngleX;
	GLfloat m_fAngleY;
	GLfloat m_fAngleZ;
	GLfloat m_fRadius;

	BACKGROUND_MODE m_enmBackgroundMode;
	GLfloat m_vBackground[4];
	GLuint m_unBackgroundTexName;
	ATL::CImage m_imgBackground;

	UINT m_nFontOffset;

	ATL::CImage m_imgGameOver;
	ATL::CImage m_imgGameWon;

	DRAW_MODE m_enmDrawMode;

	STATUS_MODE m_enmStatusMode;
	unsigned int m_nStatusSeconds;

	CFireWorks m_objFireWorks;

	bool m_bLeftButtonDown;
	unsigned int m_nHoldSeconds;

	bool m_bNeedToUpdate;
	bool m_bTilesRemoved;

	CString m_arrTileName[43];

	MUSIC_MODE m_enmMusic;
};

