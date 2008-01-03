/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SolitaireGame.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 14.05.04 0:27 $
*
*================================================================================================*/

#include "StdAfx.h"
#include "SolitaireGame.h"
#include "resource.h"
#include "Service\MJCrypt.h"
#include "service/Buffer.h"
#include "service/FileStorage.h"

const TCHAR* STR_GAME_PASS = _T("{55E7D40E-B566-4ef3-BC1C-BCC6C3520B13}");

const unsigned char CSolitaireGame::SAVEFILE_VERSION = 0x13;

unsigned int CSolitaireGame::m_nGamesPlayed = 0L;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSolitaireGame::CSolitaireGame(void)
{
	m_enmState = GS_UNDEFINED;

	m_nMoves = 0;
	m_posSelected.Clear();

	ZeroMemory(m_vecUndo, 72 * sizeof(CSolitaireMove));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSolitaireGame::~CSolitaireGame(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CSolitaireGame::Select(int i, int j, int k)
{
	m_posSelected = CSolitairePos(i, j, k);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CSolitaireGame::RemoveTiles(CSolitairePos posTile1, CSolitairePos posTile2)
{
	m_vecUndo[m_nMoves] = CSolitaireMove(posTile1, m_objBoard.GetTile(posTile1), posTile2, m_objBoard.GetTile(posTile2));
	m_nMoves++;

	m_objBoard.GetTile(posTile1) = NO_TILE;
	m_objBoard.GetTile(posTile2) = NO_TILE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSolitaireGame::CLICK_RESULT CSolitaireGame::ClickTile(int i, int j, int k)
{
	if (m_enmState != GS_GAME)
		return CR_WRONG_STATE;

	// valid only for free tiles
	if (!m_objBoard.IsFree(i, j, k))
		return CR_TILE_BLOCKED;

	// no selected position
	if (m_posSelected.IsClear())
	{
		Select(i, j, k);
		Sound(IDR_SELECT);
		return CR_TILE_SELECTED;
	}

	// clear selected position
	if (GetSelection() == CSolitairePos(i, j, k))
	{
		DropSelection();
		Sound(IDR_SELECT);
		return CR_TILE_UNSELECTED;
	}

	// tiles are not equal
	if (!TileEqual(GetSelectedTile(), m_objBoard.GetTile(i, j, k)))
	{
		DropSelection();
		//Select(i, j, k);
		return CR_TILES_NOT_EQUAL;
	}

	// remove tiles
	RemoveTiles(GetSelection(), CSolitairePos(i, j, k));

	// clear selection
	DropSelection();

	if (m_objBoard.CountTiles() == 0)
	{
		// game over
		m_enmState = GS_GAMEWON;

		CSolitaireGame::m_nGamesPlayed++;
	}
	else if (!m_objBoard.HaveMoreMoves())
	{
		// no more moves
		m_enmState = GS_GAMEOVER;

		CSolitaireGame::m_nGamesPlayed++;
	}

	if (g_AppSettings.m_bAutoSave)
		Save(MakeUserPath(LoadResourceString(IDS_SAVE_GAME_FOLDER)) + _T("AutoSave.mjs"));

	return CR_TILES_REMOVED;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::Undo(void)
{
	bool bUndoMade = false;

	if (m_nMoves > 0)
	{
		m_objBoard.GetTile(m_vecUndo[m_nMoves - 1].m_pos1) = m_vecUndo[m_nMoves - 1].m_Tile1;
		m_objBoard.GetTile(m_vecUndo[m_nMoves - 1].m_pos2) = m_vecUndo[m_nMoves - 1].m_Tile2;

		m_nMoves--;

		bUndoMade = true;

		m_enmState = GS_GAME;

		if (g_AppSettings.m_bAutoSave)
			Save(MakeUserPath(LoadResourceString(IDS_SAVE_GAME_FOLDER)) + _T("AutoSave.mjs"));
	}

	return bUndoMade;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::New()
{
	m_objBoard.Set(m_objLayout);
	m_objBoard.PlaceTiles();
	m_objOriginalBoard = m_objBoard;

	m_nMoves = 0;

	m_objScores.Reset();
	lstrcpyn(m_objScores.m_strLayoutName, m_objLayout.GetName(), 50);

	m_posSelected.Clear();
	m_posCurrent.Clear();

	m_enmState = GS_GAME;

	if (g_AppSettings.m_bAutoSave)
		Save(MakeUserPath(LoadResourceString(IDS_SAVE_GAME_FOLDER)) + _T("AutoSave.mjs"));

	Sound(IDR_DRAGON);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::Replay()
{
	// restore board
	m_objBoard = m_objOriginalBoard;

	// clear undo stack
	m_nMoves = 0;

	// set up parameters
	m_objScores.Reset();

	m_posSelected.Clear();
	m_posCurrent.Clear();

	m_enmState = GS_GAME;

	if (g_AppSettings.m_bAutoSave)
		Save(MakeUserPath(LoadResourceString(IDS_SAVE_GAME_FOLDER)) + _T("AutoSave.mjs"));

	Sound(IDR_DRAGON);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::Replay(CSolitaireBoard& objBoard)
{
	// restore board
	m_objOriginalBoard = m_objBoard = objBoard;

	// clear undo stack
	m_nMoves = 0;

	// set up parameters
	m_objScores.Reset();

	m_posSelected.Clear();
	m_posCurrent.Clear();

	m_enmState = GS_GAME;

	if (g_AppSettings.m_bAutoSave)
		Save(MakeUserPath(LoadResourceString(IDS_SAVE_GAME_FOLDER)) + _T("AutoSave.mjs"));

	Sound(IDR_DRAGON);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::Demo()
{
	m_objBoard.Set(m_objLayout);
	m_objBoard.PlaceTilesForDemo(m_vecRemove);
	m_objOriginalBoard = m_objBoard;

	m_nMoves = 0;

	m_objScores.Reset();

	m_enmState = GS_DEMO;

	Sound(IDR_DRAGON);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::LoadLayout(CString strPathName)
{
	CString strPath = strPathName;
	unsigned int nExt = strPath.ReverseFind('.');

	if (nExt != -1)
	{
		CString strExt = strPath.Mid(nExt);

		if (strExt == _T(".lay"))
			return LoadKyodaiLayout(strPath);
	}

	return LoadMJLayout(strPath);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::LoadMJLayout(UINT nResourceID)
{
	if (!m_objLayout.LoadResource(nResourceID))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::LoadMJLayout(CString strPathName)
{
	CSolitaireLayout objLayout;

	if (!objLayout.Load(strPathName))
		return false;

	m_objLayout.Set(objLayout);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CSolitaireGame::LoadKyodaiLayout(CString strPathName)
{
	CSolitaireLayout objLayout;

	if (objLayout.LoadKyodai(strPathName))
		m_objLayout.Set(objLayout);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CSolitaireGame::TickSecond()
{
	if (m_enmState == GS_GAME)
	{
		m_objScores.m_tmPlayed.Advance();
	}

	if (m_enmState == GS_DEMO)
	{
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMJGameTime CSolitaireGame::GetPlayTime()
{
	return m_objScores.m_tmPlayed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
HRESULT CSolitaireGame::Save(CString strPathName)
{
	try
	{
		CMJCryptStream strSave;

		// guard byte / version
		strSave.Write((unsigned int)SAVEFILE_VERSION);

		// board
		m_objBoard.Save(strSave);
		strSave.Write((unsigned int)m_enmState);
		m_objOriginalBoard.Save(strSave);

		strSave.Write(m_posSelected.i);
		strSave.Write(m_posSelected.j);
		strSave.Write(m_posSelected.k);

		strSave.Write(m_objScores.m_strLayoutName);
		strSave.Write(m_objScores.m_tmStarted.dwHighDateTime);
		strSave.Write(m_objScores.m_tmStarted.dwLowDateTime);

		strSave.Write(m_objScores.m_tmPlayed.GetLong());
		strSave.Write(m_objScores.m_nUndoTaken);

		// undo stack

		for (unsigned int i = 0; i < m_nMoves; i++)
		{
			strSave.Write(m_vecUndo[i].m_pos1.i);
			strSave.Write(m_vecUndo[i].m_pos1.j);
			strSave.Write(m_vecUndo[i].m_pos1.k);
			strSave.Write(m_vecUndo[i].m_Tile1);

			strSave.Write(m_vecUndo[i].m_pos2.i);
			strSave.Write(m_vecUndo[i].m_pos2.j);
			strSave.Write(m_vecUndo[i].m_pos2.k);
			strSave.Write(m_vecUndo[i].m_Tile2);
		}

		// undo end
		strSave.Write(44);

		CFileStorage objFile;

		if (objFile.Create(strPathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS) == INVALID_HANDLE_VALUE)
			return E_FAIL;

		//objFile.Write(objBuffer.GetBuffer(), pSink->TotalPutLength());
	}
	catch (...)
	{
		return E_FAIL;
	}

	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
HRESULT CSolitaireGame::Load(CString strPathName)
{
	try
	{
		CFileStorage objFile;

		if (objFile.Create(strPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING) == INVALID_HANDLE_VALUE)
			return E_FAIL;

		DWORD dwSize = objFile.GetSize();

		CBuffer objBuffer(dwSize);

		if (FAILED(objFile.Read(objBuffer.GetBuffer(), objBuffer.GetSize())))
			return false;

		CMJCryptStream strLoad;

		//strLoad.SetBuffer(objBuffer2.GetBuffer(), pSink->TotalPutLength());

		unsigned int nVersion;

		strLoad.Read(&nVersion);

		if (nVersion != SAVEFILE_VERSION)
			throw 1;

		m_objBoard.Load(strLoad);

		strLoad.Read((int*)&m_enmState);

		m_objOriginalBoard.Load(strLoad);

		strLoad.Read(&m_posSelected.i);

		strLoad.Read(&m_posSelected.j);

		strLoad.Read(&m_posSelected.k);

		m_objScores.Reset();

		CString strLayout;

		strLoad.Read(&strLayout);

		lstrcpyn(m_objScores.m_strLayoutName, strLayout, 50);

		strLoad.Read(&m_objScores.m_tmStarted.dwHighDateTime);

		strLoad.Read(&m_objScores.m_tmStarted.dwLowDateTime);

		long lPlayed;

		strLoad.Read(&lPlayed);

		m_objScores.m_tmPlayed.SetLong(lPlayed);

		strLoad.Read(&m_objScores.m_nUndoTaken);

		//
		unsigned char ch;

		strLoad.Read(&ch);

		while (ch != 44)
		{
			unsigned char i1, j1, k1, t1;
			i1 = ch;
			strLoad.Read(&j1);
			strLoad.Read(&k1);
			strLoad.Read(&t1);
			unsigned char i2, j2, k2, t2;
			strLoad.Read(&i2);
			strLoad.Read(&j2);
			strLoad.Read(&k2);
			strLoad.Read(&t2);

			m_vecUndo[m_nMoves] = CSolitaireMove(CSolitairePos(i1, j1, k1), t1, CSolitairePos(i2, j2, k2), t2);
			m_nMoves++;

			strLoad.Read(&ch);
		}
	}
	catch (...)
	{
		return E_FAIL;
	}

	return S_OK;
}