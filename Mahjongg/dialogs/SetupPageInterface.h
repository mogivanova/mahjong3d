#pragma once

#include "SetupPage.h"
#include "..\GUI\LocalizableDialog.h"

class CSetupPageInterface :
			public CLocalizableDialog<CSetupPageInterface>,
			public CSetupPage,
			public CDialogImpl<CSetupPageInterface>,
			public CWinDataExchange<CSetupPageInterface>
{

public:
	CSimpleArray<CString> m_arrNames, m_arrLocalNames;
	CComboBox m_ctlLanguages;

	BOOL m_bSkinsEnabled;

	CSimpleArray<CString> m_arrSkins;
	CComboBox m_ctlSkins;


public:
	CSetupPageInterface(void);
	~CSetupPageInterface(void);

public:
	virtual void Move(CRect& rect);
	virtual void Show(UINT nCommand);
	enum { IDD = IDD_SETUP_INTERFACE };

	BEGIN_MSG_MAP(CSetupPageInterface)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_SKIN_ENABLED, BN_CLICKED, OnBnClickedSkinEnabled)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupPageInterface)
	DDX_CHECK(IDC_SKIN_ENABLED, m_bSkinsEnabled);
	//DDX_CONTROL(IDC_LANGUAGE, m_ctlLanguages)
	END_DDX_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)

	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	LRESULT OnBnClickedSkinEnabled(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
};
