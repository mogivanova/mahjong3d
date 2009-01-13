/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: LayoutsDlg.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:46 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"
#include "LayoutsDlg.h"
#include ".\layoutsdlg.h"


CLayoutsDlg::CLayoutsDlg()
{
	m_bLayoutLoaded = false;
	m_bUpdating = false;
}


CLayoutsDlg::~CLayoutsDlg()
{
	for (int i = 0; i < m_lstDescriptions.GetSize(); i++)
	{
		delete m_lstDescriptions[i];
	}
}


LRESULT CLayoutsDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow(GetParent());

	LocalizeControls();

	m_ImageList.Create(IDB_LAYOUT_ICON, 16, 0, RGB(255, 0, 255));

	m_ctrlList = GetDlgItem(IDC_LAYOUTS_LIST);
	m_ctrlList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

	CRect rectClient;
	m_ctrlList.GetClientRect(&rectClient);

	int nWidth = rectClient.right - rectClient.left - 20;
	m_ctrlList.InsertColumn(0, g_LangManager.GetString("str_description"), LVCFMT_LEFT, nWidth, 0);

	m_ctrlList.SetImageList(m_ImageList, LVSIL_SMALL);

	m_lstDescriptions.Add(new CLayoutDescription(g_LangManager.GetString("str_default"), _T("default")));

	LoadInstalledLayouts();
	LoadUserLayouts();

	UpdateList();

	m_ctrlLayout.SetParentDlg(this);

	CRect rcLayout;
	::GetWindowRect(GetDlgItem(IDC_LAYOUT), &rcLayout);
	ScreenToClient(&rcLayout);

	m_ctrlLayout.Create(m_hWnd, rcLayout, NULL, WS_CHILD | WS_VISIBLE);

	return TRUE;
}


LRESULT CLayoutsDlg::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	return 0;
}


LRESULT CLayoutsDlg::OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int nSelected = m_ctrlList.GetSelectedIndex();

	if (nSelected == -1)
		return 0;

	CLayoutDescription* pDesc = (CLayoutDescription*) m_ctrlList.GetItemData(nSelected);

	m_strLayoutPath = pDesc->m_strPath;

	EndDialog(IDOK);

	return 0;
}


LRESULT CLayoutsDlg::OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString strFilter = g_LangManager.GetString("str_layout_filter");

	for (int i = 0; i < strFilter.GetLength(); i++)
	{
		if (strFilter.GetAt(i) == '\n')
			strFilter.SetAt(i, '\0');
	}

	CFileDialog dlg(TRUE, _T("lay"), NULL,

									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									strFilter,
									m_hWnd);

	//CString strInitialDir = MakeInstallPath(g_AppSettings.m_strLayoutsFolder);
	//dlg.m_ofn.lpstrInitialDir = strInitialDir;
	CString strInitialDir = MakeFullPath(_T("\\Layouts"));
	dlg.m_ofn.lpstrInitialDir = strInitialDir;

	if (dlg.DoModal() == IDOK)
	{
		m_strLayoutPath = CString(dlg.m_szFileName);
		EndDialog(IDOK);
	}

	return 0;
}


LRESULT CLayoutsDlg::OnDblclkTilesetList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	int nSelected = m_ctrlList.GetSelectedIndex();

	if (nSelected == -1)
		return 0;

	CLayoutDescription* pDesc = (CLayoutDescription*) m_ctrlList.GetItemData(nSelected);

	m_strLayoutPath = pDesc->m_strPath;

	EndDialog(IDOK);

	return 0;
}


CString CLayoutsDlg::GetLayoutPath()
{
	return m_strLayoutPath;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CLayoutsDlg::LoadInstalledLayouts()
{
	CString strPath = MakeInstallPath("Layouts\\");
	LoadLayouts(strPath);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CLayoutsDlg::LoadUserLayouts()
{
	CString strPath = MakeUserPath("Layouts\\");
	LoadLayouts(strPath);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CLayoutsDlg::LoadLayouts(CString strPath)
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

			CLayoutDescription* pDesc = NULL;

			if (strExt == ".mjl")
			{
				if (CSolitaireLayout::LoadDescription(strPath + strFile, &pDesc))
				{
					pDesc->m_strPath = strPath + strFile;
				}
				else
					pDesc = new CLayoutDescription(strFile, strPath + strFile);
			}
			else if (strExt == ".lay")
			{
				pDesc = new CLayoutDescription(strFile, strPath + strFile);
			}

			if (pDesc != NULL)
				m_lstDescriptions.Add(pDesc);
		}
	}
	while (FindNextFile(hFind, &fd));
}


void CLayoutsDlg::UpdateList()
{
	m_ctrlList.DeleteAllItems();

	m_ctrlList.SetRedraw(FALSE);
	m_bUpdating = true;

	int nSelIndex = 0;

	int nIndex = 0;

	for (int i = 0; i < m_lstDescriptions.GetSize(); i++)
	{
		CLayoutDescription* desc = m_lstDescriptions[i];

		nIndex = m_ctrlList.InsertItem(nIndex, desc->m_strName, desc->m_bKyodaiFormat ? 2 : 1);

		m_ctrlList.SetItemText(nIndex, 1, desc->m_strName);

		m_ctrlList.SetItemData(nIndex, (DWORD_PTR)desc);

		if (desc->m_strPath == g_AppSettings.m_strLayout)
			nSelIndex = nIndex;
	}

	m_bUpdating = false;

	m_ctrlList.SetRedraw(TRUE);

	m_ctrlList.SelectItem(nSelIndex);

	m_ctrlList.InvalidateRect(NULL);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CLayoutCtrl::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dcPaint(m_hWnd);

	HBRUSH hOldBrush = dcPaint.SelectBrush(GetSysColorBrush(COLOR_WINDOW));

	CRect rcLayout;
	GetClientRect(&rcLayout);

	dcPaint.Rectangle(&rcLayout);

	dcPaint.SelectBrush(hOldBrush);

	//CSize szTile(m_imgTile.GetWidth(), m_imgTile.GetHeight());

	CSize szTile(rcLayout.Width() / 22,  rcLayout.Height() / 12);
	CBrush arrBrush[5];

	arrBrush[0].CreateSolidBrush(RGB(128, 128, 128));
	arrBrush[1].CreateSolidBrush(RGB(144, 144, 144));
	arrBrush[2].CreateSolidBrush(RGB(160, 160, 160));
	arrBrush[3].CreateSolidBrush(RGB(176, 176, 176));
	arrBrush[4].CreateSolidBrush(RGB(192, 192, 192));

	if (m_pParentDlg->m_bLayoutLoaded)
	{
		int nSx = (rcLayout.Width() - szTile.cx * (m_pParentDlg->m_objLayout.GetWidth() / 2)) / 2;
		int nSy = (rcLayout.Height() - szTile.cy * (m_pParentDlg->m_objLayout.GetHeight() / 2)) / 2;

		for (int k = 0; k < m_pParentDlg->m_objLayout.GetDepth(); k++)
		{
			for (int i = m_pParentDlg->m_objLayout.GetWidth() - 1; i >= 0; i--)
			{
				for (int j = 0; j < m_pParentDlg->m_objLayout.GetHeight(); j++)
				{
					CTile nTile = m_pParentDlg->m_objLayout.GetTile(i, m_pParentDlg->m_objLayout.GetHeight() - 1 - j, k);

					if (nTile != NO_TILE)
					{
						int nX = nSx + rcLayout.left + (i * szTile.cx) / 2 + k * 3;
						int nY = nSy + rcLayout.top + (j * szTile.cy) / 2 - k * 3;

						CRect rcTileFace(CPoint(nX, nY), szTile);
						CRect rcTileBottom = rcTileFace;

						for (int t = 0; t < 3; t++)
						{
							rcTileBottom.OffsetRect(-1, 1);
							dcPaint.FillRect(rcTileBottom, (HBRUSH)GetStockObject(BLACK_BRUSH));
						}

						dcPaint.Rectangle(rcTileFace);

						rcTileFace.DeflateRect(1, 1);
						dcPaint.FillRect(rcTileFace, arrBrush[k]);
					}
				}
			}
		}
	}

	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CLayoutsDlg::OnSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMLISTVIEW pNMH = (LPNMLISTVIEW) pnmh;

	CLayoutDescription* pDesc = (CLayoutDescription*)m_ctrlList.GetItemData(pNMH->iItem);

	if (pDesc != NULL)
	{
		if (!m_bUpdating)
		{
			if (pDesc->m_strPath == _T("default"))
				m_bLayoutLoaded = m_objLayout.LoadResource(IDRL_TRADITIONAL);
			else
				m_bLayoutLoaded = m_objLayout.Load(pDesc->m_strPath);

			InvalidateRect(NULL);
		}
	}

	return 0;
}

