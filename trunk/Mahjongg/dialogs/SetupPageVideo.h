/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SetupPageVideo.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 23.03.04 23:11 $
*
*================================================================================================*/

#pragma once

#include "SetupPage.h"
#include "..\GUI\LocalizableDialog.h"

class CSetupPageVideo :
			public CLocalizableDialog<CSetupPageVideo>,
			public CSetupPage,
			public CDialogImpl<CSetupPageVideo>,
			public CWinDataExchange<CSetupPageVideo>
{

public:
	int m_nGraphDetail;
	BOOL m_bHighQualityTextures;
	BOOL m_bTexturedSides;
	BOOL m_bHighQualityBackground;
	BOOL m_bSmoothTiles;
	BOOL m_bColoredLayers;

public:
	CSetupPageVideo(void);
	~CSetupPageVideo(void);

public:
	virtual void Move(CRect& rect);
	virtual void Show(UINT nCommand);
	enum { IDD = IDD_SETUP_GRAPHICS };

	BEGIN_MSG_MAP(CSetupPageVideo)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupPageVideo)
	DDX_RADIO(IDC_GRAPHICS, m_nGraphDetail)
	DDX_CHECK(IDC_HQ_TILE_TEX, m_bHighQualityTextures)
	DDX_CHECK(IDC_TEXTURED_SIDES, m_bTexturedSides)
	DDX_CHECK(IDC_HQ_BACKGROUND, m_bHighQualityBackground)
	DDX_CHECK(IDC_SMOOTH_TILES, m_bSmoothTiles)
	DDX_CHECK(IDC_COLORED_LAYERS, m_bColoredLayers);
	END_DDX_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)

	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

public:
	LRESULT CallDefaultWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
};
