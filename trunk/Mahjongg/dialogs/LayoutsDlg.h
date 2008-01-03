/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: LayoutsDlg.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:53 $
*
*================================================================================================*/

#pragma once

#include "..\GUI\LocalizableDialog.h"
#include "../Game/Solitaire/SolitaireLayout.h"

class CLayoutDescription;

class CLayoutsDlg;

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/

class CLayoutCtrl :
			public CWindowImpl<CLayoutCtrl>
{

public:
	CLayoutCtrl()
	{
		m_pParentDlg = NULL;
	}

	DECLARE_WND_CLASS(NULL);

	BEGIN_MSG_MAP(CLayoutCtrl)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetParentDlg(CLayoutsDlg* pDlg)
	{
		m_pParentDlg = pDlg;
	}

protected:
	CLayoutsDlg* m_pParentDlg;
};

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/

class CLayoutsDlg :
			public CLocalizableDialog<CLayoutsDlg>,
			public CDialogImpl<CLayoutsDlg>,
			public CUpdateUI<CLayoutsDlg>,
			public CWinDataExchange<CLayoutsDlg>
{

protected:
	CListViewCtrl m_ctrlList;
	CImageList m_ImageList;
	CString m_strLayoutPath;

public:
	CLayoutsDlg(void);
	~CLayoutsDlg(void);

public:
	enum { IDD = IDD_LAYOUTS };

	BEGIN_MSG_MAP(CLayoutsDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

	COMMAND_ID_HANDLER(IDOK, OnOk)
	COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	COMMAND_HANDLER(IDC_BROWSE, BN_CLICKED, OnClickedBrowse)

	NOTIFY_HANDLER(IDC_LAYOUTS_LIST, LVN_ITEMCHANGED, OnSelChange)
	NOTIFY_HANDLER(IDC_LAYOUTS_LIST, NM_DBLCLK, OnDblclkTilesetList)
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CLayoutsDlg)
	END_UPDATE_UI_MAP()

	BEGIN_DDX_MAP(CLayoutsDlg)
	END_DDX_MAP()

public:
	// Handler prototypes (uncomment arguments if needed):
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
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

public:
	CString GetLayoutPath();

protected:
	void LoadInstalledLayouts();
	void LoadUserLayouts();

	void LoadLayouts(CString strPath);

	void UpdateList();

public:
	typedef CSimpleArray<CLayoutDescription*> TDescList;
	TDescList m_lstDescriptions;

	CSolitaireLayout m_objLayout;
	bool m_bLayoutLoaded;
	bool m_bUpdating;

	CLayoutCtrl m_ctrlLayout;
};
