/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: ScoreRecord.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:46 $
*
*================================================================================================*/

#include "StdAfx.h"
#include "ScoreRecord.h"

#include "service/FileStorage.h"

#define CHECK_VALUE 0xEBABDAED
#define SCORE_TABLE_SIZE 100

#define STR_SCORES_PASS _T("{3CFA3D03-78D6-47c4-95C2-EAE6ECA3C7FD}")

/**
*
*/
SScoreRecord::SScoreRecord()
{
	memset(m_strLayoutName, 0, sizeof(m_strLayoutName));
	memset(m_strPlayerName, 0, sizeof(m_strPlayerName));
	memset(&m_tmStarted, 0, sizeof(m_tmStarted));
	m_nUndoTaken = 0;
	m_bUploaded = false;
}

SScoreRecord::SScoreRecord(const SScoreRecord& record)
{
	memcpy(this, &record, sizeof(SScoreRecord));
}

/**
*
*/
HRESULT SScoreRecord::Reset()
{
	memset(m_strLayoutName, 0, sizeof(m_strLayoutName));
	memset(m_strPlayerName, 0, sizeof(m_strPlayerName));

	m_tmPlayed.Clear();

	SYSTEMTIME stm = {0};
	GetLocalTime(&stm);
	SystemTimeToFileTime(&stm, &m_tmStarted);

	m_nUndoTaken = 0;

	return S_OK;
}

/**
*
*/
HRESULT SScoreRecord::Set(CString strLayoutName, CString strPlayerName)
{
	memset(m_strLayoutName, 0, sizeof(m_strLayoutName));
	lstrcpyn(m_strLayoutName, strLayoutName, sizeof(m_strLayoutName) / sizeof(TCHAR));

	memset(m_strPlayerName, 0, sizeof(m_strPlayerName));
	lstrcpyn(m_strPlayerName, strPlayerName, sizeof(m_strPlayerName) / sizeof(TCHAR));

	return S_OK;
}

/**
*
*/
bool operator<(SScoreRecord& rec1, SScoreRecord& rec2)
{
	return rec1.m_tmPlayed < rec2.m_tmPlayed;
}

/**
*
*/
SLayoutStats::SLayoutStats()
{
	memset(m_strLayoutName, 0, sizeof(m_strLayoutName));
	m_nTimesPlayedTotal = 0;
	m_nTimesPlayedStored = 0;
	m_nTimesWonTotal = 0;
	m_nTimesWonStored = 0;
}

/**
*
*/
SLayoutStats::SLayoutStats(const SLayoutStats& stats)
{
	memcpy(this, &stats, sizeof(SLayoutStats));
}

/**
*
*/
SLayoutStats& SLayoutStats::operator =(const SLayoutStats & stats)
{
	memcpy(this, &stats, sizeof(SLayoutStats));
	return *this;
}

/**
*
*/
HRESULT SLayoutStats::SetLayout(CString strLayoutName)
{
	memset(m_strLayoutName, 0, sizeof(m_strLayoutName));
	lstrcpyn(m_strLayoutName, strLayoutName, sizeof(m_strLayoutName) / sizeof(TCHAR));
	return S_OK;
}

/**
*
*/
HRESULT SLayoutStats::Update(CString strLayoutName, bool bWon, CMJGameTime& tmPlayed)
{
	memset(m_strLayoutName, 0, sizeof(m_strLayoutName));
	lstrcpyn(m_strLayoutName, strLayoutName, sizeof(m_strLayoutName) / sizeof(TCHAR));

	m_nTimesWonTotal += bWon ? 1 : 0;
	m_nTimesWonStored += bWon ? 1 : 0;

	m_tmPlayedTotal += tmPlayed;
	m_tmPlayedStored += tmPlayed;

	return S_OK;
}

/**
*
*/
CScoreTable::CScoreTable()
{

}

/**
*
*/
HRESULT CScoreTable::Clear()
{
	m_mapScores.RemoveAll();

	m_mapStats.RemoveAll();

	return S_OK;
}

/**
*
*/
HRESULT CScoreTable::Load(LPCTSTR pszFilePath)
{
	if (FAILED(LoadCurrent(pszFilePath)))
	{
		RETURN(E_FAIL);
	}
	else
	{
		// save old scores in new format
		Save(pszFilePath);
	}

	return S_OK;
}

/**
*
*/
HRESULT CScoreTable::LoadCurrent(LPCTSTR pszFilePath)
{
	Clear();

	try
	{
		CFileStorage objFile;

		if (objFile.Create(pszFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING) == INVALID_HANDLE_VALUE)
			return E_FAIL;

		DWORD dwSize = objFile.GetSize();

		CBuffer objBuffer(dwSize);

		if (FAILED(objFile.Read(objBuffer.GetBuffer(), objBuffer.GetSize())))
			return false;

		CMJCryptStream objLoadStream;

		//objLoadStream.SetBuffer(objBuffer2.GetBuffer(), pSink->TotalPutLength());

		///
		_TRACE(_T("Info: CScoreTable::Load - Scores are open\n"));

		DWORD dwCheck;

		if (FAILED(objLoadStream.Read(&dwCheck, sizeof(DWORD))) || dwCheck != CHECK_VALUE)
			RETURN(E_FAIL);

		_TRACE(_T("Info: CScoreTable::Load - Check value passed\n"));

		// read version
		CString strAppID;

		if (FAILED(objLoadStream.Read(&strAppID)) || strAppID != APP_ID)
			RETURN(E_FAIL);

		_TRACE(_T("Info: CScoreTable::Load - App ID passed\n"));

		DWORD dwVersion;

		if (FAILED(objLoadStream.Read(&dwVersion)))
			RETURN(E_FAIL);

		// current version
		DWORD dwStatsCount = 0;

		if (FAILED(objLoadStream.Read(&dwStatsCount)))
			RETURN(E_FAIL);

		for (unsigned int i = 0; i < dwStatsCount; i++)
		{
			SLayoutStats rec;

			if (FAILED(objLoadStream.Read(&rec, sizeof(SLayoutStats))))
				RETURN(E_FAIL);

			m_mapStats.SetAt(CString(rec.m_strLayoutName), rec);
		}

		// read check value
		if (FAILED(objLoadStream.Read(&dwCheck, sizeof(DWORD))) || dwCheck != CHECK_VALUE)
			RETURN(E_FAIL);

		DWORD dwLayoutCount = 0;

		if (FAILED(objLoadStream.Read(&dwLayoutCount)))
			RETURN(E_FAIL);

		for (unsigned int i = 0; i < dwLayoutCount; i++)
		{
			DWORD dwRecCount = 0;

			if (FAILED(objLoadStream.Read(&dwRecCount)))
				RETURN(E_FAIL);

			for (unsigned int j = 0; j < dwRecCount; j++)
			{
				SScoreRecord rec;

				if (FAILED(objLoadStream.Read(&rec, sizeof(SScoreRecord))))
					RETURN(E_FAIL);

				m_mapScores[CString(rec.m_strLayoutName)].Add(rec);
			}
		}
	}
	catch (...)
	{
		return E_FAIL;
	}

	return S_OK;
}

/**
*
*/
HRESULT CScoreTable::Save(LPCTSTR pszFilePath)
{
	try
	{
		CMJCryptStream objSaveStream;

		objSaveStream.Write((DWORD)CHECK_VALUE);
		objSaveStream.Write(CString(APP_ID));
		objSaveStream.Write((DWORD)APP_VERSION);

		objSaveStream.Write((DWORD)m_mapStats.GetCount());

		for (POSITION pos = m_mapStats.GetStartPosition(); pos != NULL; m_mapStats.GetNext(pos))
		{
			SLayoutStats& rec = m_mapStats.GetValueAt(pos);
			objSaveStream.Write(&rec, sizeof(SLayoutStats));
		}

		objSaveStream.Write((DWORD)CHECK_VALUE);

		objSaveStream.Write((DWORD)m_mapScores.GetCount());

		for (POSITION pos = m_mapScores.GetStartPosition(); pos != NULL; m_mapScores.GetNext(pos))
		{
			TLayoutScores::CPair* pPair = m_mapScores.GetAt(pos);
			//CString strLayout = m_mapScores.GetKeyAt(i);
			TRecordList& list = pPair->m_value;
			TRecordList::TIterator it = list.GetHeadPosition();

			objSaveStream.Write((DWORD)list.GetSize());

			while (it != NULL)
			{
				SScoreRecord& rec = list.GetNext(it);
				objSaveStream.Write(&rec, sizeof(SScoreRecord));
			}
		}

		CFileStorage objFile;

		if (objFile.Create(pszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS) == INVALID_HANDLE_VALUE)
			return E_FAIL;

		//objFile.Write(objBuffer.GetBuffer(), pSink->TotalPutLength());
	}
	catch (...)
	{
		return E_FAIL;
	}

	return S_OK;
}

/**
*
*/
HRESULT CScoreTable::AddRecord(SScoreRecord& record)
{
	CString strLayout = record.m_strLayoutName;

	m_mapScores[strLayout].Add(record);

	return S_OK;
}

/**
*
*/
HRESULT CScoreTable::GameStarted(CString strLayoutName)
{
	SLayoutStats& rec = m_mapStats[strLayoutName];

	rec.m_nTimesPlayedTotal++;
	rec.m_nTimesPlayedStored++;

	return S_OK;
}

/**
*
*/
HRESULT CScoreTable::GameFinished(CString strLayoutName, bool bWon, CMJGameTime& tmPlayTime)
{
	SLayoutStats& rec = m_mapStats[strLayoutName];

	rec.Update(strLayoutName, bWon, tmPlayTime);

	return S_OK;
}