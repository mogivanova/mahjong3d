#include "stdafx.h"
#include "..\resource.h"
#include "SetupPageGame.h"

CSetupPageGame::CSetupPageGame()
{
}

CSetupPageGame::~CSetupPageGame()
{
}

LRESULT CSetupPageGame::OnInitDialog(UINT , WPARAM , LPARAM , BOOL&)
{
	LocalizeControls();

	m_bSmartCursor = g_AppSettings.m_bSmartCursor;
	m_bShowRBHints = g_AppSettings.m_bShowRBHints;
	m_bSolvableBoards = g_AppSettings.m_bSolvableBoards;
	m_bAutoSave = g_AppSettings.m_bAutoSave;
	m_bTextHints = g_AppSettings.m_bTextHints;
	m_bHideClosed = g_AppSettings.m_bHideClosedTiles;

	DoDataExchange(FALSE);

	return 0;
}

void CSetupPageGame::Show(UINT nCommand)
{
	ShowWindow(nCommand);
}


void CSetupPageGame::Move(CRect &rect)
{
	SetWindowPos(m_hWnd, rect.left, rect.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
