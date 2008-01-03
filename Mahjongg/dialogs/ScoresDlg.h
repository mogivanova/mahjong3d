/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: ScoresDlg.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:54 $
*
*================================================================================================*/

#pragma once

#include "..\Game\Solitaire\ScoreRecord.h"
#include "..\GUI\LocalizableDialog.h"

class CScoresDlg :
			public CLocalizableDialog<CScoresDlg>,
			public CDialogImpl<CScoresDlg>
{

protected:
	CListViewCtrl m_ctrlScores;
	CImageList m_ImageList;
	CComboBox m_ctlLayouts;
	CButton m_btnUpload;
	CButton m_btnClose;
	WTL::CProgressBarCtrl m_ctlProgress;

public:
	CScoreTable* m_pScoreTable;
	SScoreRecord* m_pSelectedRecord;
	CString m_strLayout;

public:
	enum {IDD = IDD_SCORES};

	BEGIN_MSG_MAP(CScoresDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDC_CLOSE, OnCloseCmd)
	COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	COMMAND_HANDLER(IDC_SCORES_UPLOAD, BN_CLICKED, OnClickedUpload)

	COMMAND_HANDLER(IDC_LAYOUTS, CBN_SELCHANGE, OnLayoutChanged)

	END_MSG_MAP()

	CScoresDlg();
	~CScoresDlg();

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClickedUpload(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnLayoutChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

protected:
	HRESULT FillScoresForLayout(CString strLayout);

protected:
	static DWORD WINAPI UploadThread(LPVOID lpParameter);

	HANDLE m_hUploadThread;
	DWORD m_dwThreadID;

	HRESULT m_hrUploadResult;
	CString m_strUploadMessage;

};
