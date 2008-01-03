/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: ScoreRecord.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#pragma once

#include "Board.h"
#include "Service\MJCrypt.h"
#include "game/solitaire/MJPlayTime.h"
#include "service/SortedList.h"

/**
*
*/

struct SScoreRecord
{
	///
	TCHAR m_strLayoutName[30];
	///
	TCHAR m_strPlayerName[50];
	///
	FILETIME m_tmStarted;
	///
	CMJGameTime m_tmPlayed;
	///
	unsigned int m_nUndoTaken;
	///
	bool m_bUploaded;

	///
	SScoreRecord();
	///
	SScoreRecord(const SScoreRecord& record);
	///
	SScoreRecord& operator=(const SScoreRecord& record);
	///

	/// New game
	HRESULT Reset();
	///
	HRESULT Set(CString strLayoutName, CString strPlayerName);

	friend bool operator<(SScoreRecord& r1, SScoreRecord& r2);
};

/**
*
*/

struct SLayoutStats
{
	///
	TCHAR m_strLayoutName[30];
	///
	unsigned int m_nTimesWonTotal;
	///
	unsigned int m_nTimesPlayedTotal;
	///
	CMJGameTime m_tmPlayedTotal;
	///
	unsigned int m_nTimesWonStored;
	///
	unsigned int m_nTimesPlayedStored;
	///
	CMJGameTime m_tmPlayedStored;

	///
	SLayoutStats();
	///
	SLayoutStats(const SLayoutStats& stats);
	///
	SLayoutStats& operator=(const SLayoutStats& stats);

	///
	HRESULT SetLayout(CString strLayoutName);
	///
	HRESULT Update(CString strLayoutName, bool bWon, CMJGameTime& tmPlayed);

};

/**
*
*/

class CScoreTable
{

public:
	///
	CScoreTable();

public:
	///
	HRESULT Clear();
	///
	HRESULT Load(LPCTSTR pszFilePath);
	///
	HRESULT Save(LPCTSTR pszFilePath);

	/// !Need CALLBACK
	HRESULT Upload();

	/// update statistics
	HRESULT GameStarted(CString strLayoutName);
	/// update statistics
	HRESULT GameFinished(CString strLayoutName, bool bWon, CMJGameTime& tmPlayTime);
	///
	HRESULT AddRecord(SScoreRecord& record);

protected:
	///
	HRESULT LoadCurrent(LPCTSTR pszPath);

public:
	typedef CSortedList<SScoreRecord> TRecordList;
	typedef ATL::CAtlMap<ATL::CString, TRecordList> TLayoutScores;
	///
	TLayoutScores m_mapScores;

	typedef ATL::CAtlMap<ATL::CString, SLayoutStats> TLayoutStats;
	///
	TLayoutStats m_mapStats;
};

