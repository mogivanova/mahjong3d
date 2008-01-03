#pragma once

#include "..\GUI\LocalizableDialog.h"

#include "SetupPage.h"
#include "SetupPageGame.h"
#include "SetupPageSound.h"
#include "SetupPageVideo.h"
#include "SetupPageInterface.h"
#include "SetupPageInternet.h"

class CSetupExpertDlg :
			public CLocalizableDialog<CSetupExpertDlg>,
			public CDialogImpl<CSetupExpertDlg>,
			public CUpdateUI<CSetupExpertDlg>,
			public CWinDataExchange<CSetupExpertDlg>
{

protected:
	CListViewCtrl m_ctrlSelector;

	CSetupPageVideo m_dlgVideo;
	CSetupPageSound m_dlgSound;
	CSetupPageGame m_dlgGame;
	CSetupPageInterface m_dlgInterface;
	CSetupPageInternet m_dlgInternet;

	CSetupPage* m_pCurrentPage;

	CImageList m_ImageList;
	ATL::CImage m_imgIcons;

public:
	CSetupExpertDlg(void);
	~CSetupExpertDlg(void);

protected:
	void AddPage(LPCTSTR pszPageName, CSetupPage* pPage, int nImage);

public:
	enum { IDD = IDD_SETUP_EXPERT };

	BEGIN_MSG_MAP(CSetupExpertDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	//MESSAGE_HANDLER(WM_PAINT, OnPaint)

	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem);

	COMMAND_ID_HANDLER(IDOK, OnOk)
	COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)

	NOTIFY_HANDLER(IDC_SELECTOR, LVN_ITEMCHANGED, OnSelectionChange)
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CSetupExpertDlg)
	END_UPDATE_UI_MAP()

	BEGIN_DDX_MAP(CSetupExpertDlg)
	END_DDX_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	LRESULT OnSelectionChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
};
