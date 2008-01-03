#include "stdafx.h"
#include "..\resource.h"
#include "SetupPageInterface.h"
#include ".\setuppageinterface.h"

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CSetupPageInterface::CSetupPageInterface()
{
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CSetupPageInterface::~CSetupPageInterface()
{
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CSetupPageInterface::OnInitDialog(UINT , WPARAM , LPARAM , BOOL&)
{
	LocalizeControls();

	m_ctlLanguages.Attach(GetDlgItem(IDC_LANGUAGE));
	m_ctlSkins.Attach(GetDlgItem(IDC_SKIN));

	// get language names
	g_LangManager.GetLanguageNames(m_arrNames, m_arrLocalNames);

	if (m_arrNames.GetSize() == 0)
	{
		m_ctlLanguages.EnableWindow(FALSE);
	}
	else
	{
		// store items
		for (int i = 0; i < m_arrNames.GetSize(); i++)
		{
			CString strItem = m_arrLocalNames[i] + " (" + m_arrNames[i] + ")";
			int nItemIndex = m_ctlLanguages.AddString(strItem);

			m_ctlLanguages.SetItemData(nItemIndex, i);
		}

		for (int i = 0; i < m_ctlLanguages.GetCount(); i++)
		{
			if (m_arrNames[m_ctlLanguages.GetItemData(i)] == g_AppSettings.m_strLanguage)
				m_ctlLanguages.SetCurSel(i);
		}
	}

	DoDataExchange(FALSE);

	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CSetupPageInterface::Show(UINT nCommand)
{
	ShowWindow(nCommand);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CSetupPageInterface::Move(CRect &rect)
{
	SetWindowPos(m_hWnd, rect.left, rect.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CSetupPageInterface::OnBnClickedSkinEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DoDataExchange(TRUE);

	m_ctlSkins.EnableWindow(m_bSkinsEnabled);

	return 0;
}
