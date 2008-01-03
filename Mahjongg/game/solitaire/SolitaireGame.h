/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SolitaireGame.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#pragma once

#include "SolitaireLayout.h"
#include "SolitaireBoard.h"
#include "ScoreRecord.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CSolitaireGame
{

public:
	enum GAME_STATE
	{
		GS_UNDEFINED = 0,
		GS_INTRO,
		GS_GAME,
		GS_GAMEOVER,
		GS_GAMEWON,
		GS_CLOSING,
		GS_DEMO
	};

	enum CLICK_RESULT
	{
		CR_TILES_REMOVED,
		CR_WRONG_STATE,
		CR_TILE_BLOCKED,
		CR_TILE_SELECTED,
		CR_TILE_UNSELECTED,
		CR_TILES_NOT_EQUAL
	};

	typedef CSimpleArray<CSolitairePos> TRemoveVec;

protected:
	static const unsigned char SAVEFILE_VERSION;

public:
	GAME_STATE m_enmState;

	CSolitaireLayout m_objLayout;
	CSolitaireBoard m_objOriginalBoard;
	CSolitaireBoard m_objBoard;

	CSolitairePos m_posCurrent;
	CSolitairePos m_posSelected;

	unsigned int m_nMoves;
	CSolitaireMove m_vecUndo[72];

	//CMJGameTime m_tmGame;
	SScoreRecord m_objScores;

	TRemoveVec m_vecRemove;

	static unsigned int m_nGamesPlayed;

public:
	CSolitaireGame(void);
	~CSolitaireGame(void);

	bool LoadLayout(CString pszPathName);
	bool LoadMJLayout(UINT nResourceID);
	bool LoadMJLayout(CString pszPathName);
	bool LoadKyodaiLayout(CString pszPathName);

	GAME_STATE GetState()
	{
		return m_enmState;
	}

	void SetState(GAME_STATE enmState)
	{
		m_enmState = enmState;
	}

	CSolitaireBoard& GetBoard()
	{
		return m_objBoard;
	}

	CSolitaireLayout& GetLayout()
	{
		return m_objLayout;
	}

	void Select(int i, int j, int k);
	void Select(CSolitairePos posSelection);
	void DropSelection(void)
	{
		m_posSelected.Clear();
	}

	CSolitairePos GetSelection()
	{
		return m_posSelected;
	}

	CTile GetSelectedTile(void)
	{
		return m_objBoard.GetTile(m_posSelected);
	}

	void RemoveTiles(CSolitairePos posTile1, CSolitairePos posTile2);
	CLICK_RESULT ClickTile(int i, int j, int k);

	bool New();
	bool Replay();
	bool Replay(CSolitaireBoard& objBoard);
	bool Undo(void);

	bool Demo();
	TRemoveVec& GetRemoveVec()
	{
		return m_vecRemove;
	}

	HRESULT Save(CString pszPathName);
	HRESULT Load(CString pszPathName);

	void TickSecond();

	CMJGameTime GetPlayTime();

	CSolitaireBoard& GetOriginalBoard()
	{
		return m_objOriginalBoard;
	}

	CSolitaireMove& GetMove(int i)
	{
		return m_vecUndo[i];
	}
};
