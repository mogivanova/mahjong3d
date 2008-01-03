/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MahjonggView.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:35 $
*
*================================================================================================*/

#pragma once

#include "..\resource.h"
#include "..\Service\OpenGL\OpenGLDriver.h"
#include "tileset\mjtileset.h"
#include "..\Service\3DS\3ds.h"

// ќбъ€влени€, которые мы будем использовать

///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CMainFrame;

///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CMahjonggView :
			public CWindowImpl<CMahjonggView>
{

protected:
	CMainFrame* m_pContainer;

public:
	// constructor
	CMahjonggView(CMainFrame* pContainer);
	~CMahjonggView();

public:
	// class info
	static CWndClassInfo& GetWndClassInfo()
	{
		static CWndClassInfo wc =
		{
			{ sizeof(WNDCLASSEX), CS_OWNDC | CS_HREDRAW | CS_VREDRAW, StartWindowProc,
				0, 0, NULL, NULL, NULL, CreateSolidBrush(RGB(0, 0, 64)), NULL, _T("MahjonggView"), NULL },
			NULL, NULL, NULL, TRUE, 0, _T("")
		};
		return wc;
	}

	BEGIN_MSG_MAP(CMahjonggView)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_DISPLAYCHANGE, OnDisplayChange)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)

	MESSAGE_HANDLER(WM_NCPAINT, OnNCPaint)

	COMMAND_ID_HANDLER(ID_TILESET_LOAD, OnTilesetLoad)
	COMMAND_ID_HANDLER(ID_TILESET_DEFAULT, OnTilesetDefault)
	END_MSG_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);

public:
	//	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNCPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDisplayChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//	LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	virtual LRESULT OnTilesetLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	virtual LRESULT OnTilesetDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	//	LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)

protected:


	enum LIST_NAMES
	{
		eTileNear = 1,
		eTileBody,

		eMaterialTileBottom, // bottom part of tile
		eMaterialTileBottomSelected, // bottom part of tile
		eMaterialTileTop,
		eMaterialTileTopSelected,

		eMaterialFaceNormal,
		eMaterialFaceHint,
		eMaterialFaceSelected,

		eChild,

		LIST_LIGHT,
		LIST_BACKGROUND,
		LIST_BACKGROUND_MATERIAL,
		LIST_BACKGROUND_IMAGE_MATERIAL,

		LIST_TEXT_COLOR,
		LIST_TEXT_BACKGROUND,

		LIST_FONT,

		LIST_GAME_OVER_STRING
	};

	enum TILE_FLAGS
	{
		// geometry
		TF_LEFT_SIDE     = 0x001,
		TF_RIGHT_SIDE    = 0x002,
		TF_TOP_SIDE      = 0x004,
		TF_BOTTOM_SIDE   = 0x008,
		TF_FACE          = 0x010,
		TF_ALL           = TF_LEFT_SIDE | TF_RIGHT_SIDE | TF_TOP_SIDE | TF_BOTTOM_SIDE,

		// matherials
		TF_HINT          = 0x020,
		TF_SELECTED      = 0x040,
		TF_TEXTURES      = 0x080,
		TF_CLOSED        = 0x100
	};

protected:
	///
	BOOL UIEnable(int nID, BOOL bEnable, BOOL bForceUpdate = FALSE);
	///
	BOOL UISetCheck(int nID, int nCheck, BOOL bForceUpdate = FALSE);
	///
	BOOL UISetCheck(int nID, bool bCheck, BOOL bForceUpdate = FALSE);
	///
	BOOL UIUpdateMenuBar(BOOL bForceUpdate = FALSE, BOOL bMainMenu = FALSE);

public:
	// set opengl driver
	void SetDriver(COpenGLDriver* pDriver);

	//
	virtual bool Init();
	//
	virtual void OnUpdateOptions();
	//
	virtual bool SetupLights(SYSTEMTIME& tm);

	// update current view
	virtual bool UpdateView();

	// update user interface
	virtual bool UpdateUI(void);

	// update status line
	virtual void UpdateStatus();

	virtual bool Activate();
	virtual bool Deactivate();

protected:
	// create texture
	virtual void CreateTileTexture(GLuint unTextureName, ATL::CImage& objImage);

	//
	virtual bool DrawScene();

	//
	virtual void DrawTile(DWORD dwFlags = TF_ALL, int nTileImage = -1, GLfloat* vLayer = NULL);

protected:
	//
	virtual bool CreateLists(HDC hDC);
	//
	virtual bool LoadTileTextures();
	//
	virtual bool LoadGeometry();

	//
	virtual unsigned int CreateListFromModel(GLuint nDisplayList, C3DObject* pObject);

	//
	void CalculateFrameRate();

protected:
	// OpenGL driver
	COpenGLDriver* m_pDriver;

	// is initialized
	bool m_bInitialized;
	// is busy
	bool m_bBusy;
	// is ready
	bool m_bReady;

	// window size
	CSize m_objSize;
	// old window rectangle
	CRect m_rectOld;
	// window device context
	HDC m_hDC;
	// window render context
	HGLRC m_hRC;

	// tile width
	GLfloat m_fTileWidth;
	// tile height
	GLfloat m_fTileHeight;
	// tile depth
	GLfloat m_fTileDepth;

	//
	float m_fFrameInterval;

	// tile models
	C3DModel m_objTileLowQuality;
	C3DModel m_objTileMediumQuality;
	C3DModel m_objTileHighQuality;

	// tile set
	CTileset m_objTileset;

	// tile textures
	UINT m_arrTileTexNames[43];
	// tile side textures
	UINT m_arrTileSideTexNames[2];

	// graph detail level
	int m_nGraphDetail;
	// textures are loaded
	bool m_bTexturesAreReady;

	//
	DWORD* m_apFont[256];
	//
	bool m_bFontIsReady;

	CPoint m_pntRButtonDown;
	bool m_bRButtonDown;

	bool m_bActive;
};

