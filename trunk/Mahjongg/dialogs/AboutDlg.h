/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: AboutDlg.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 27.04.04 23:58 $
*
*================================================================================================*/

#pragma once

//#include "..\Service\HyperTextCtrl.h"
#include "..\GUI\LocalizableDialog.h"

class CModuleVersion
{

public:
	LPCTSTR m_szModule; //
	WORD m_wMajor, m_wMinor, m_wInternal, m_wBuild; //
	bool m_bDebug; //

	// конструктор
	CModuleVersion()
	{
		m_szModule = NULL;
		m_wMajor = m_wMinor, m_wInternal = m_wBuild = 0;
		m_bDebug = false;
	}

	//
	bool Get(LPCTSTR pszModulePath = NULL);
};

class CAboutDlg :
			public CLocalizableDialog<CAboutDlg>,
			public CDialogImpl<CAboutDlg>
{
	typedef CDialogImpl<CAboutDlg> TBaseClass;

protected:
	CStatic m_ctrlAboutStatic;
	CStatic m_ctrlRegistered;
	CStatic m_ctrlEmail;
	CStatic m_ctrlWWW;
	//HyperTextControl::CHyperTextCtrl m_ctrlEmail;
	//HyperTextControl::CHyperTextCtrl m_ctrlWWW;
	CEdit m_ctrlAboutText;

	CButton m_ctrlOk;

	HCURSOR m_hPointer;
	HCURSOR m_hHand;

public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
	COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	COMMAND_HANDLER(IDC_BUY_NOW, BN_CLICKED, OnBnClickedBuyNow)
	COMMAND_HANDLER(IDC_REGISTER, BN_CLICKED, OnBnClickedRegister)
	COMMAND_HANDLER(IDC_EMAIL, BN_CLICKED, OnBnClickedEmail)
	COMMAND_HANDLER(IDC_WWW, BN_CLICKED, OnBnClickedWWW)

	COMMAND_HANDLER(IDC_DISTRIBUTOR_LOGO, STN_CLICKED, OnStnClickedDistributorLogo)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedLicense(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBuyNow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRegister(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedEmail(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedWWW(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	LRESULT OnStnClickedDistributorLogo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	HRESULT FillControls();

	CFont m_objFont;
};
