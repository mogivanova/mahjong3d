/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SetupExpertDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 16.03.04 23:09 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"

#include "SetupExpertDlg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSetupExpertDlg::CSetupExpertDlg(void)
{
	m_pCurrentPage = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSetupExpertDlg::~CSetupExpertDlg(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CSetupExpertDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	LocalizeControls();

	m_ctrlSelector = GetDlgItem(IDC_SELECTOR);
	m_ctrlSelector.SetExtendedListViewStyle(LVS_EX_UNDERLINEHOT | LVS_EX_ONECLICKACTIVATE);

	//m_ImageList.Create(IDB_SETUP, 32, 0, RGB(255, 0, 255));
	//m_ImageList.Create(32, 32, ILC_COLOR32, 0, 0);
	m_ImageList.Attach(ImageList_LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_SETUP), 32, 0, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION));
	m_ctrlSelector.SetImageList(m_ImageList, LVSIL_NORMAL);

	CRect rcLV;
	::GetClientRect(GetDlgItem(IDC_SELECTOR), &rcLV);

	m_ctrlSelector.InsertColumn(0, _T(""), LVCFMT_CENTER, rcLV.Width(), 0);

	// create pages
	m_dlgGame.Create(m_hWnd, NULL);
	AddPage(g_LangManager.GetString("str_game_options"), &m_dlgGame, 0);

	m_dlgVideo.Create(m_hWnd, NULL);
	AddPage(g_LangManager.GetString("str_video_options"), &m_dlgVideo, 1);

	m_dlgSound.Create(m_hWnd, NULL);
	AddPage(g_LangManager.GetString("str_sound_options"), &m_dlgSound, 2);

	m_dlgInterface.Create(m_hWnd, NULL);
	AddPage(g_LangManager.GetString("str_interface_options"), &m_dlgInterface, 3);

	m_dlgInternet.Create(m_hWnd, NULL);
	AddPage(g_LangManager.GetString("str_internet_options"), &m_dlgInternet, 4);

	m_ctrlSelector.SelectItem(0);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CSetupExpertDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDCANCEL);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CSetupExpertDlg::OnOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_dlgGame.DoDataExchange(TRUE);

	g_AppSettings.m_bSmartCursor = m_dlgGame.m_bSmartCursor == TRUE;
	g_AppSettings.m_bShowRBHints = m_dlgGame.m_bShowRBHints == TRUE;
	g_AppSettings.m_bSolvableBoards = m_dlgGame.m_bSolvableBoards == TRUE;
	g_AppSettings.m_bAutoSave = m_dlgGame.m_bAutoSave == TRUE;
	g_AppSettings.m_bTextHints = m_dlgGame.m_bTextHints == TRUE;
	g_AppSettings.m_bHideClosedTiles = m_dlgGame.m_bHideClosed == TRUE;

	m_dlgVideo.DoDataExchange(TRUE);

	g_AppSettings.m_nGraphDetail = m_dlgVideo.m_nGraphDetail;
	g_AppSettings.m_bHighQualityTextures = m_dlgVideo.m_bHighQualityTextures == TRUE;
	g_AppSettings.m_bTexturedSides = m_dlgVideo.m_bTexturedSides == TRUE;
	g_AppSettings.m_bHighQualityBackground = m_dlgVideo.m_bHighQualityBackground == TRUE;
	g_AppSettings.m_bSmoothTiles = m_dlgVideo.m_bSmoothTiles == TRUE;
	g_AppSettings.m_bColoredLayers = m_dlgVideo.m_bColoredLayers == TRUE;

	m_dlgSound.DoDataExchange(TRUE);

	g_AppSettings.m_bSound = m_dlgSound.m_bSound == TRUE;

	m_dlgInterface.DoDataExchange(TRUE);

	int iLangIndex = m_dlgInterface.m_ctlLanguages.GetItemData(m_dlgInterface.m_ctlLanguages.GetCurSel());

	if (iLangIndex < 0 || iLangIndex >= m_dlgInterface.m_arrNames.GetSize())
		iLangIndex = 0;

	g_AppSettings.m_strLanguage = m_dlgInterface.m_arrNames[iLangIndex];

	m_dlgInternet.DoDataExchange(TRUE);

	g_AppSettings.m_strScoresAccount = m_dlgInternet.m_strLogin;

	g_AppSettings.m_strScoresPassword = m_dlgInternet.m_strPassword;

	g_AppSettings.Save();

	EndDialog(IDOK);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void CSetupExpertDlg::AddPage(LPCTSTR pszPageName, CSetupPage *pPage, int nImage)
{
	_ASSERT(pPage != NULL);

	int nIndex = m_ctrlSelector.GetItemCount();

	int nItem = m_ctrlSelector.InsertItem(nIndex, pszPageName, nImage);

	m_ctrlSelector.SetItemData(nItem, (DWORD)pPage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CSetupExpertDlg::OnSelectionChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int nSelected = m_ctrlSelector.GetSelectedIndex();

	if (nSelected == -1)
		return 0;

	if (m_pCurrentPage != NULL)
		m_pCurrentPage->Show(SW_HIDE);

	m_pCurrentPage = (CSetupPage*) m_ctrlSelector.GetItemData(nSelected);

	_ASSERT(m_pCurrentPage != NULL);

	CRect rectFrame;

	::GetWindowRect(GetDlgItem(IDC_FRAME), &rectFrame);

	ScreenToClient(&rectFrame);

	m_pCurrentPage->Move(rectFrame);

	m_pCurrentPage->Show(SW_SHOW);

	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CSetupExpertDlg::OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{

	return 0;
}
