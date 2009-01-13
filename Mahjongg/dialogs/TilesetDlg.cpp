/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: TilesetDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:46 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"
#include "TilesetDlg.h"
#include "..\Game\Tileset\MJTileset.h"


CTilesetDlg::CTilesetDlg(void)
{
	m_bUpdating = false;
	m_pDesc = NULL;
}


CTilesetDlg::~CTilesetDlg(void)
{
	for (int i = 0; i < m_lstDescriptions.GetSize(); i++)
	{
		delete m_lstDescriptions[i];
	}
}


LRESULT CTilesetDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow(GetParent());

	LocalizeControls();

	m_ImageList.Create(IDB_TILESET_ICON, 16, 0, RGB(255, 0, 255));

	m_ctrlList = GetDlgItem(IDC_TILESET_LIST);
	m_ctrlList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

	CRect rectClient;
	m_ctrlList.GetClientRect(&rectClient);

	int nWidth = rectClient.right - rectClient.left - 20;
	m_ctrlList.InsertColumn(0, g_LangManager.GetString("str_description"), LVCFMT_LEFT, nWidth / 3, 0);
	m_ctrlList.InsertColumn(1, g_LangManager.GetString("str_author"), LVCFMT_LEFT, nWidth * 2 / 3, 1);

	m_ctrlList.SetImageList(m_ImageList, LVSIL_SMALL);

	CTilesetDescription* pDesc = new CTilesetDescription(g_LangManager.GetString("str_tileset_default"), "Mark Sanctuary", "", "*default*");

	int nTile[] = {01, 20, 32, 40};

	for (int N = 0; N < 4; N++)
	{
		HRSRC hRes = ::FindResource(_Module.m_hInstResource, MAKEINTRESOURCE(TILE00 + nTile[N]), _T("JPEG"));
		_ASSERT(hRes != NULL);

		//bool bRes = pDesc->m_imgTile[N].LoadResource(hRes, ATL::CImage_FORMAT_JPG, _Module.m_hInstResource);
	}

	m_lstDescriptions.Add(pDesc);

	LoadInstalledTilesets();
	LoadUserTilesets();

	UpdateList();

	m_ctrlTileset.SetParentDlg(this);

	CRect rcTileset;
	::GetWindowRect(GetDlgItem(IDC_TILESET), &rcTileset);
	ScreenToClient(&rcTileset);

	m_ctrlTileset.Create(m_hWnd, rcTileset, NULL, WS_CHILD | WS_VISIBLE);

	return TRUE;
}


LRESULT CTilesetDlg::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	return 0;
}


LRESULT CTilesetDlg::OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nSelected = m_ctrlList.GetSelectedIndex();

	if (nSelected == -1)
		return 0;

	CTilesetDescription* pDesc = (CTilesetDescription*) m_ctrlList.GetItemData(nSelected);

	m_strTileset = pDesc->m_strPath;

	EndDialog(IDOK);

	return 0;
}


LRESULT CTilesetDlg::OnDblclkTilesetList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int nSelected = m_ctrlList.GetSelectedIndex();

	if (nSelected == -1)
		return 0;

	CTilesetDescription* pDesc = (CTilesetDescription*) m_ctrlList.GetItemData(nSelected);

	m_strTileset = pDesc->m_strPath;

	EndDialog(IDOK);

	return 0;
}


CString CTilesetDlg::GetTilesetPath()
{
	return m_strTileset;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CTilesetDlg::LoadInstalledTilesets()
{
	CString strPath = MakeInstallPath("Tile sets\\");
	LoadTilesets(strPath);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CTilesetDlg::LoadUserTilesets()
{
	CString strPath = MakeUserPath("Tile sets\\");
	LoadTilesets(strPath);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CTilesetDlg::LoadTilesets(CString strPath)
{
	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

	CString strSearchMask = strPath + "*.*";

	HANDLE hFind = FindFirstFile(strSearchMask, &fd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		CString strFile = fd.cFileName;

		if (strFile == "." || strFile == "..")
			continue;

		int nPos = strFile.ReverseFind('.');

		CString strExt = "";

		if (nPos != -1)
		{
			strExt = strFile.Mid(nPos);

			CTilesetDescription* pDesc = NULL;

			if (strExt == ".mjt")
			{
				if (CTileset::LoadDescription(strPath + strFile, &pDesc))
				{
					pDesc->m_strPath = strPath + strFile;
				}
				else
					pDesc = new CTilesetDescription(strFile, "", "", strPath + strFile);
			}
			else if (strExt == ".jpg" || strExt == ".bmp")
			{
				pDesc = new CTilesetDescription(strFile, "", "", strPath + strFile);
			}

			if (pDesc != NULL)
				m_lstDescriptions.Add(pDesc);
		}
	}
	while (FindNextFile(hFind, &fd));
}
void CTilesetDlg::UpdateList()
{
	m_ctrlList.DeleteAllItems();

	m_ctrlList.SetRedraw(FALSE);
	m_bUpdating = true;

	int nSelIndex = 0;

	int nIndex = 0;

	for (int i = 0; i < m_lstDescriptions.GetSize(); i++)
	{
		CTilesetDescription* desc = m_lstDescriptions[i];

		nIndex = m_ctrlList.InsertItem(nIndex, desc->m_strName, desc->m_bKyodaiFormat ? 2 : 1);

		m_ctrlList.SetItemText(nIndex, 1, desc->m_strAuthor);

		m_ctrlList.SetItemData(nIndex, (DWORD_PTR)desc);
	}

	m_bUpdating = false;

	m_ctrlList.SetRedraw(TRUE);

	m_ctrlList.SelectItem(nSelIndex);

	m_ctrlList.InvalidateRect(NULL);
}


LRESULT CTilesetDlg::OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString strFilter = g_LangManager.GetString("str_tileset_filter");

	for (int i = 0; i < strFilter.GetLength(); i++)
	{
		if (strFilter.GetAt(i) == '\n')
			strFilter.SetAt(i, '\0');
	}

	CFileDialog dlg(TRUE, _T("mjt"), NULL,

									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									strFilter,
									m_hWnd);

	//CString strInitialDir = MakeInstallPath(g_AppSettings.m_strTilesetFolder);
	//dlg.m_ofn.lpstrInitialDir = strInitialDir;

	CString strInitialDir = MakeFullPath(_T("\\Tile Sets"));
	dlg.m_ofn.lpstrInitialDir = strInitialDir;

	if (dlg.DoModal() == IDOK)
	{
		m_strTileset = CString(dlg.m_szFileName);
		EndDialog(IDOK);
	}

	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CTilesetCtrl::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dcPaint(m_hWnd);

	CRect rcLayout;
	GetClientRect(&rcLayout);

	dcPaint.Rectangle(&rcLayout);

	if (m_pParentDlg->m_pDesc != NULL)
	{
		int nX = rcLayout.left;
		int nY = rcLayout.top;
		int nSx = rcLayout.Width() / 2;
		int nSy = rcLayout.Height() / 2;

		CRect rcTile(CPoint(nX, nY), CSize(nSx, nSy));
		CRect rcTilePlace = rcTile;
		rcTilePlace.DeflateRect(2, 2);

		dcPaint.Rectangle(rcTile);

		if (m_pParentDlg->m_pDesc->m_imgTile[0].GetBits() != NULL)
			m_pParentDlg->m_pDesc->m_imgTile[0].Draw(dcPaint, rcTilePlace);

		rcTile = CRect(CPoint(nX + nSx, nY), CSize(nSx, nSy));

		rcTilePlace = rcTile;

		rcTilePlace.DeflateRect(2, 2);

		dcPaint.Rectangle(rcTile);

		if (m_pParentDlg->m_pDesc->m_imgTile[1].GetBits() != NULL)
			m_pParentDlg->m_pDesc->m_imgTile[1].Draw(dcPaint, rcTilePlace);

		rcTile = CRect(CPoint(nX, nY + nSy), CSize(nSx, nSy));

		rcTilePlace = rcTile;

		rcTilePlace.DeflateRect(2, 2);

		dcPaint.Rectangle(rcTile);

		if (m_pParentDlg->m_pDesc->m_imgTile[2].GetBits() != NULL)
			m_pParentDlg->m_pDesc->m_imgTile[2].Draw(dcPaint, rcTilePlace);

		rcTile = CRect(CPoint(nX + nSx, nY + nSy), CSize(nSx, nSy));

		rcTilePlace = rcTile;

		rcTilePlace.DeflateRect(2, 2);

		dcPaint.Rectangle(rcTile);

		if (m_pParentDlg->m_pDesc->m_imgTile[3].GetBits() != NULL)
			m_pParentDlg->m_pDesc->m_imgTile[3].Draw(dcPaint, rcTilePlace);
	}


	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CTilesetDlg::OnSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMLISTVIEW pNMH = (LPNMLISTVIEW) pnmh;

	m_pDesc = (CTilesetDescription*)m_ctrlList.GetItemData(pNMH->iItem);

	if (m_pDesc != NULL)
	{
		if (!m_bUpdating)
		{
			InvalidateRect(NULL);
		}
	}

	return 0;
}