/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: TilesetDlg.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:54 $
*
*================================================================================================*/

#pragma once

#include "..\GUI\LocalizableDialog.h"

class CTilesetDescription;

class CTilesetDlg;

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/

class CTilesetCtrl :
			public CWindowImpl<CTilesetCtrl>
{

public:
	CTilesetCtrl()
	{
		m_pParentDlg = NULL;
	}

	DECLARE_WND_CLASS(NULL);

	BEGIN_MSG_MAP(CTilesetCtrl)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetParentDlg(CTilesetDlg* pDlg)
	{
		m_pParentDlg = pDlg;
	}

protected:
	CTilesetDlg* m_pParentDlg;
};

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/

class CTilesetDlg :
			public CLocalizableDialog<CTilesetDlg>,
			public CDialogImpl<CTilesetDlg>,
			public CUpdateUI<CTilesetDlg>,
			public CWinDataExchange<CTilesetDlg>
{

protected:
	CListViewCtrl m_ctrlList;
	CImageList m_ImageList;
	CString m_strTileset;

public:
	CTilesetDlg(void);
	~CTilesetDlg(void);

public:
	enum { IDD = IDD_TILESETS };

	BEGIN_MSG_MAP(CTilesetDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOk)
	COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	COMMAND_HANDLER(IDC_BROWSE, BN_CLICKED, OnClickedBrowse)
	NOTIFY_HANDLER(IDC_TILESET_LIST, LVN_ITEMCHANGED, OnSelChange)
	NOTIFY_HANDLER(IDC_TILESET_LIST, NM_DBLCLK, OnDblclkTilesetList)
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CTilesetDlg)
	END_UPDATE_UI_MAP()

	BEGIN_DDX_MAP(CTilesetDlg)
	END_DDX_MAP()

public: // message handlers
	//	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//	LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	//	LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	LRESULT OnSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDblclkTilesetList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

public:
	CString GetTilesetPath();

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

protected:
	void LoadInstalledTilesets();
	void LoadUserTilesets();

	void LoadTilesets(CString strPath);

	void UpdateList();

public:
	typedef CSimpleArray<CTilesetDescription*> TDescList;
	TDescList m_lstDescriptions;

	bool m_bUpdating;

	CTilesetDescription* m_pDesc;

	CTilesetCtrl m_ctrlTileset;
};
