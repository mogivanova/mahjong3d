/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: ScoresDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "ScoresDlg.h"

#include "service/HttpConnection.h"

/**
 *
 */
CScoresDlg::CScoresDlg()
{
	m_pScoreTable = NULL;
	m_pSelectedRecord = NULL;

	m_hUploadThread = NULL;
	m_dwThreadID = -1;
}

/**
 *
 */
CScoresDlg::~CScoresDlg()
{
}

/**
*
*/
LRESULT CScoresDlg::OnInitDialog(UINT , WPARAM , LPARAM , BOOL& )
{
	CenterWindow(GetParent());

	LocalizeControls();

	m_ImageList.Create(IDB_SCORE_PLACE, 16, 0, RGB(255, 0, 255));

	m_ctrlScores = GetDlgItem(IDC_SCORES);
	m_ctrlScores.SetImageList(m_ImageList, LVSIL_SMALL);

	CRect rect;
	int nWidth;

	m_ctrlScores.GetClientRect(&rect);

	nWidth = m_ctrlScores.GetStringWidth(_T("00 00:00:00")) + 15 + 16;
	rect.right -= nWidth;
	m_ctrlScores.InsertColumn(0, g_LangManager.GetString("str_played"), LVCFMT_LEFT, nWidth, 0);

	rect.right -= 20;
	m_ctrlScores.InsertColumn(2, g_LangManager.GetString("str_player"), LVCFMT_LEFT, rect.right - rect.left, 2);

	///
	m_ctlLayouts = GetDlgItem(IDC_LAYOUTS);

	for (POSITION pos = m_pScoreTable->m_mapScores.GetStartPosition(); pos != NULL; m_pScoreTable->m_mapScores.GetNext(pos))
	{
		CScoreTable::TLayoutScores::CPair* pPair = m_pScoreTable->m_mapScores.GetAt(pos);

		CString strLayout = pPair->m_key;

		int nItem = m_ctlLayouts.AddString(strLayout);
	}

	if (m_ctlLayouts.SelectString(0, m_strLayout) == CB_ERR)
	{
		if (m_ctlLayouts.SelectString(0, _T("Traditional")) == CB_ERR)
		{
			m_ctlLayouts.SetCurSel(0);
		}
	}

	BOOL bDummy;

	OnLayoutChanged(0, 0, NULL, bDummy);

	m_btnUpload = GetDlgItem(IDC_SCORES_UPLOAD);
	m_btnClose = GetDlgItem(IDC_CLOSE);
	m_ctlProgress = GetDlgItem(IDC_UPLOAD_PROGRESS);

	return 0;
}

/**
 *
 */
LRESULT CScoresDlg::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

/**
 *
 */
LRESULT CScoresDlg::OnClickedUpload(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_hUploadThread != NULL)
		return 0;

	m_btnUpload.EnableWindow(FALSE);

	m_btnClose.EnableWindow(FALSE);

	m_hUploadThread = ::CreateThread(NULL, 0, UploadThread, this, 0, &m_dwThreadID);



	return 0;
}

/**
 *
 */
LRESULT CScoresDlg::OnLayoutChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR chLayout[512] = {0};
	m_ctlLayouts.GetWindowText(chLayout, 512);
	CString strLayout = chLayout;

	FillScoresForLayout(strLayout);

	return 0;
}

/**
 *
 */
HRESULT CScoresDlg::FillScoresForLayout(CString strLayout)
{
	m_ctrlScores.DeleteAllItems();

	if (strLayout == _T(""))
		return S_FALSE;

	CScoreTable::TRecordList& lstScores = m_pScoreTable->m_mapScores[strLayout];

	CScoreTable::TRecordList::TIterator it = lstScores.GetHeadPosition();

	int nPlace = 0;

	int iItem = 0;

	while (it != NULL)
	{
		SScoreRecord& rec = lstScores.GetNext(it);

		CString strPlayTime;
		strPlayTime.Format(_T("%02d %02d:%02d:%02d"),
											 rec.m_tmPlayed.m_nDays, rec.m_tmPlayed.m_nHours, rec.m_tmPlayed.m_nMinutes, rec.m_tmPlayed.m_nSeconds);

		if (nPlace > 3)
			nPlace = 3;

		int nIndex = m_ctrlScores.InsertItem(iItem++, strPlayTime, nPlace++);

		m_ctrlScores.SetItemText(nIndex, 1, rec.m_strPlayerName);

		m_ctrlScores.SetItemData(nIndex, (DWORD_PTR)it);
	}

	return S_OK;
}

/**
*
*/
CString valueOf(int n)
{
	TCHAR ch[20 + 1];
	swprintf(ch, 20, _T("%20d"), n);
	return ch;
}

/**
*
*/
CString valueOf(FILETIME& ftm)
{
	SYSTEMTIME tm = {0};
	FileTimeToSystemTime(&ftm, &tm);
	CString strResult;
	strResult.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
									 tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
	return strResult;
}

/**
*
*/
CString valueOf(CMJGameTime tm)
{
	CString strResult;
	strResult.Format(_T("%d"), tm.GetLong());
	return strResult;
}

/**
 *
 */
DWORD WINAPI CScoresDlg::UploadThread(LPVOID lpParameter)
{
	CScoresDlg* pDlg = (CScoresDlg*)lpParameter;

	try
	{
		pDlg->m_ctlProgress.ShowWindow(SW_SHOW);
		pDlg->m_ctlProgress.SetRange(0, 100);
		pDlg->m_ctlProgress.SetPos(0);

		_TRACE(_T("UploadThreadProc started\r\n"));

		/// http

		if (FAILED(CGLHttpRequest::Initialize(CString(APP_ID) + _T("/") + CString(STR_VERSION))))
		{
			throw 1;
		}

		pDlg->m_ctlProgress.SetPos(10);

		_TRACE(_T("CGLHttpRequest initialized\r\n"));

		/// login, get session id
		CString strSessionID;
		{
			CGLHttpRequest objRequest;

			CGLHttpRequest::TRequestParams objParams;
			objParams[CString(_T("a"))] = CString(_T("l"));
			objParams[CString(_T("l"))] = g_AppSettings.m_strScoresAccount;
			objParams[CString(_T("p"))] = g_AppSettings.m_strScoresPassword;

			pDlg->m_ctlProgress.SetPos(20);

			CString strResponse;

			if (FAILED(objRequest.Post(STR_ONLINE_SERVER, objParams, strResponse)))
			{
				_TRACE(_T("Login request failed: ") + strResponse);
				_TRACE(_T("\r\n"));
				throw 1;
			}

			pDlg->m_ctlProgress.SetPos(30);

			CGLHttpRequest::TResponse mapResponse;

			if (FAILED(objRequest.Parse(strResponse, mapResponse)) || mapResponse[_T("s")] != "loginok")
			{
				_TRACE(_T("Login request failed: ") + strResponse);
				_TRACE(_T("\r\n"));
				throw 1;
			}

			strSessionID = mapResponse[_T("sid")];

			_TRACE(_T("Login request passed, sid: ") + strSessionID);
			_TRACE(_T("\r\n"));
		}

		pDlg->m_ctlProgress.SetPos(40);

		/// upload scores
		CString strData;

		CScoreTable::TLayoutStats& stats = pDlg->m_pScoreTable->m_mapStats;
		CScoreTable::TLayoutScores& scores = pDlg->m_pScoreTable->m_mapScores;

		strData += _T("0\t") + CString(APP_ID) + _T("/") + CString(STR_VERSION) + _T("\n");

		for (POSITION pos = stats.GetStartPosition(); pos != NULL; stats.GetNext(pos))
		{
			SLayoutStats& stat = stats.GetValueAt(pos);

			strData +=
				CString(_T("1")) + _T("\t") +
				CString(stat.m_strLayoutName) + _T("\t") +
				valueOf(stat.m_nTimesPlayedStored) + _T("\t") +
				valueOf(stat.m_nTimesWonStored) + _T("\t") +
				valueOf(stat.m_tmPlayedStored) +
				_T("\n");
		}

		for (POSITION pos = scores.GetStartPosition(); pos != NULL; scores.GetNext(pos))
		{
			CScoreTable::TRecordList& list = scores.GetValueAt(pos);
			CScoreTable::TRecordList::TIterator it = list.GetHeadPosition();

			while (it != NULL)
			{
				SScoreRecord& rec = list.GetNext(it);
				CString strLine;
				strLine += CString(_T("2")) + _T("\t");
				strLine += valueOf(rec.m_tmStarted) + _T("\t");
				strLine += CString(rec.m_strLayoutName) + _T("\t");
				strLine += CString(rec.m_strPlayerName) + _T("\t");
				strLine += valueOf(rec.m_tmPlayed) + _T("\t");
				strLine += valueOf(rec.m_nUndoTaken) + _T("\t");
				strLine += (rec.m_bUploaded ? CString(_T("u")) : CString(_T("i"))) + _T("\n");

				strData += strLine;
			}
		}

		_TRACE(_T("Upload block created\r\n"));

		/// encode data
		CString strUpload = strData;

		_TRACE(_T("Upload block encoded\r\n"));

		/// upload
		{
			CGLHttpRequest objRequest;

			CGLHttpRequest::TRequestParams objParams;
			objParams[CString(_T("a"))] = CString(_T("s"));
			objParams[CString(_T("i"))] = strSessionID;
			objParams[CString(_T("d"))] = strUpload;

			pDlg->m_ctlProgress.SetPos(50);

			CString strResponse;

			if (FAILED(objRequest.Post(STR_ONLINE_SERVER, objParams, strResponse)) ||
					strResponse.Find(_T("error")) != -1)
			{
				_TRACE(_T("Upload failed: ") + strResponse);
				_TRACE(_T("\r\n"));
				throw 1;
			}

			_TRACE(_T("Upload finished\r\n"));
		}

		pDlg->m_ctlProgress.SetPos(90);

		CGLHttpRequest::Uninitialize();

		for (POSITION pos = stats.GetStartPosition(); pos != NULL; stats.GetNext(pos))
		{
			SLayoutStats& stat = stats.GetValueAt(pos);
			stat.m_nTimesPlayedStored = 0;
			stat.m_nTimesWonStored = 0;
			stat.m_tmPlayedStored = 0;
		}

		for (POSITION pos = scores.GetStartPosition(); pos != NULL; scores.GetNext(pos))
		{
			CScoreTable::TRecordList& list = scores.GetValueAt(pos);
			CScoreTable::TRecordList::TIterator it = list.GetHeadPosition();

			while (it != NULL)
			{
				SScoreRecord& rec = list.GetNext(it);
				rec.m_bUploaded = true;
			}
		}

		pDlg->m_pScoreTable->Save(MakeUserPath(_T("")) + _T("scores.dat"));

		pDlg->m_ctlProgress.SetPos(100);
	}
	catch (...)
	{
		CString strDescription;
		strDescription.Format(g_LangManager.GetString(_T("str_upload_failed")), _T(""));
		::MessageBox(pDlg->m_hWnd, strDescription, g_LangManager.GetString(_T("caption_error")),
								 MB_OK | MB_ICONSTOP);
	}

	pDlg->m_btnUpload.EnableWindow();

	pDlg->m_btnClose.EnableWindow();

	pDlg->m_dwThreadID = 0;
	pDlg->m_hUploadThread = NULL;

	pDlg->m_ctlProgress.ShowWindow(SW_HIDE);

	return 0;
}