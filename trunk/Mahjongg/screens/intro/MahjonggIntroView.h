/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MahjonggIntroView.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#pragma once

#ifdef TTT
#include "..\mahjonggview.h"

class CMahjonggIntroView :
			public CMahjonggView
{
	typedef CMahjonggView TParent;

public:
	CMahjonggIntroView(CMainFrame* pContainer);
	~CMahjonggIntroView(void);

	BEGIN_MSG_MAP(CMahjonggIntroView)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)

	MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()

public:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnGameNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnGameDemo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:
	bool SetupPixelFormat(void);
	bool Init();

	bool CreateLists();
	bool LoadTextures();

	bool DrawScene(GLenum enmMode);


	void UpdateView();

	virtual bool UpdateUI();

protected:
	bool m_bInitialized;
	bool m_bBusy;
	bool m_bReady;

	CRect m_rectOld;
	HDC m_hDC;
	HGLRC m_hRC;

	GLfloat m_fAngleX;
	GLfloat m_fAngleY;
	GLfloat m_fAngleZ;
	GLfloat m_fRadius;

	GLfloat m_vBackground[4];

	CSize m_objSize;
};
#endif