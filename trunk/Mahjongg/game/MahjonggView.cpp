/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MahjonggView.cppDst $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 27.03.04 14:35 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\resource.h"

#include "MahjonggView.h"
#include "..\MainFrm.h"

// dialogs
#include "..\Dialogs\ProgressDlg.h"
#include "..\Dialogs\TilesetDlg.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//
CMahjonggView::CMahjonggView(CMainFrame* pContainer)
{
	m_pContainer = pContainer;

	m_pDriver = NULL;

	m_bBusy = false;
	m_bInitialized = false;
	m_bReady = false;

	m_fTileWidth = 0.74f;
	m_fTileHeight = 1.0f;
	m_fTileDepth = 0.35f;

	m_fFrameInterval = 0.0f;

	m_hDC = NULL;
	m_hRC = NULL;

	m_objSize.cx = m_objSize.cy = 0;

	m_pntRButtonDown.x = m_pntRButtonDown.y = 0;
	m_bRButtonDown = false;

	ZeroMemory(&m_rectOld, sizeof(CRect));

	ZeroMemory(&m_apFont, sizeof(m_apFont));
	m_bFontIsReady = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
CMahjonggView::~CMahjonggView()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
BOOL CMahjonggView::PreTranslateMessage(MSG* pMsg)
{
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggView::Init(void)
{
	m_hDC = ::GetDC(m_hWnd);
	_ASSERT(m_hDC != NULL);

	m_hRC = m_pDriver->Attach(m_hDC);

	if (m_hRC == NULL)
		return false;

	//
	GetClientRect(&m_rectOld);

	m_pDriver->ViewPort(0, 0, m_rectOld.right, m_rectOld.bottom);

	m_pDriver->MatrixMode(GL_PROJECTION);

	m_pDriver->LoadIdentity();

	m_pDriver->Perspective(45.0f, (GLfloat)m_rectOld.right / (GLfloat)m_rectOld.bottom, 1.0f, 30.0f);

	m_pDriver->MatrixMode(GL_MODELVIEW);

	// create command lists
	CreateLists(m_hDC);

	m_pDriver->ClearDepth(1.0f);

	m_pDriver->DepthFunc(GL_LEQUAL);

	m_pDriver->Enable(GL_DEPTH_TEST);

	//m_pDriver->Enable(GL_ALPHA_TEST);
	//m_pDriver->Enable(GL_STENCIL_TEST);

	//m_pDriver->Enable(GL_AUTO_NORMAL);

	m_pDriver->Enable(GL_LIGHTING);

	SYSTEMTIME tm;

	GetLocalTime(&tm);

	SetupLights(tm);

	// enable lighting
	m_pDriver->Enable(GL_LIGHT0);

	m_pDriver->Disable(GL_LIGHT1);

	// prepare tileset texture
	ZeroMemory(m_arrTileTexNames, sizeof(m_arrTileTexNames));

	m_pDriver->GenTextures(43, m_arrTileTexNames);

	ZeroMemory(m_arrTileSideTexNames, sizeof(m_arrTileSideTexNames));

	m_pDriver->GenTextures(2, m_arrTileSideTexNames);

	m_pDriver->TexEnv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// load geometry
	LoadGeometry();

	// load tile set
	if (g_AppSettings.m_strTileset == _T("default") || !m_objTileset.Load(g_AppSettings.m_strTileset))
	{
		m_objTileset.LoadResource();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggView::UpdateView()
{
	if (!m_bInitialized)
		return false;

	m_pContainer->UpdateMenu(m_pContainer->GetMenu());

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps = {0};
	HDC hDC = ::BeginPaint(m_hWnd, &ps);

	UpdateView();

	::EndPaint(m_hWnd, &ps);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnNCPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	QPROFILE_FUN(_T("CMahjonggView::OnSize"));

	int cx = LOWORD(lParam);
	int cy = HIWORD(lParam);

	m_objSize.cx = cx;
	m_objSize.cy = cy;

	if (m_bInitialized && cy > 0)
	{
		m_rectOld.right = cx;
		m_rectOld.bottom = cy;

		m_pDriver->ViewPort(0, 0, cx, cy);
		m_pDriver->MatrixMode(GL_PROJECTION);
		m_pDriver->LoadIdentity();
		m_pDriver->Perspective(45.0f, (GLfloat)cx / (GLfloat)cy, 1.0f, 40.0f);
		m_pDriver->MatrixMode(GL_MODELVIEW);
		m_pDriver->RasterPos(0, 0);
	}

	UpdateView();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnDisplayChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_bInitialized)
	{
		UpdateView();
		return 1;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
BOOL CMahjonggView::UIEnable(int nID, BOOL bEnable, BOOL bForceUpdate /* = FALSE */)
{
	return m_pContainer->UIEnable(nID, bEnable, bForceUpdate);
}

BOOL CMahjonggView::UISetCheck(int nID, bool bCheck, BOOL bForceUpdate /* = FALSE */)
{
	return m_pContainer->UISetCheck(nID, bCheck, bForceUpdate);
}

BOOL CMahjonggView::UISetCheck(int nID, int nCheck, BOOL bForceUpdate /* = FALSE */)
{
	return m_pContainer->UISetCheck(nID, nCheck, bForceUpdate);
}

BOOL CMahjonggView::UIUpdateMenuBar(BOOL bForceUpdate /* = FALSE */, BOOL bMainMenu /* = FALSE */)
{
	return m_pContainer->UIUpdateMenuBar(bForceUpdate, bMainMenu);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void CMahjonggView::SetDriver(COpenGLDriver* pDriver)
{
	m_pDriver = pDriver;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggView::SetupLights(SYSTEMTIME& tm)
{

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggView::DrawScene()
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggView::CreateLists(HDC hDC)
{
	///////////////////////////////////////////////////////////////////////////
	// list of material properties
	m_pDriver->NewList(eMaterialTileBottom, GL_COMPILE);
	{
		//GLfloat ambient[] = glRGB(253, 251, 230);
		//GLfloat diffuse[] = glRGB(253, 251, 230);
		GLfloat ambient[] = glRGB(170, 175, 170);
		GLfloat diffuse[] = glRGB(170, 175, 170);
		GLfloat specular[] = glRGB(0, 0, 0);
		GLfloat emission[] = glRGB(0, 0, 0);
		GLfloat shininess[] = {0.0f};

		m_pDriver->Material(GL_FRONT, GL_AMBIENT, ambient);
		m_pDriver->Material(GL_FRONT, GL_DIFFUSE, diffuse);
		m_pDriver->Material(GL_FRONT, GL_SPECULAR, specular);
		m_pDriver->Material(GL_FRONT, GL_EMISSION, emission);
		m_pDriver->Material(GL_FRONT, GL_SHININESS, shininess);
	}

	m_pDriver->EndList();

	///////////////////////////////////////////////////////////////////////////
	// list of selected material properties
	m_pDriver->NewList(eMaterialTileBottomSelected, GL_COMPILE);
	{
		GLfloat ambient[] = glRGB(170, 170, 170);
		GLfloat diffuse[] = glRGB(170, 170, 170);
		GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat emission[] = glRGB(70, 70, 0);
		GLfloat shininess[] = {0.0f};

		m_pDriver->Material(GL_FRONT, GL_AMBIENT, ambient);
		m_pDriver->Material(GL_FRONT, GL_DIFFUSE, diffuse);
		m_pDriver->Material(GL_FRONT, GL_SPECULAR, specular);
		m_pDriver->Material(GL_FRONT, GL_EMISSION, emission);
		m_pDriver->Material(GL_FRONT, GL_SHININESS, shininess);
	}

	m_pDriver->EndList();

	m_pDriver->NewList(eMaterialTileTop, GL_COMPILE);
	{
		GLfloat ambient[] = glRGBA(192, 192, 192, 1.0f);
		GLfloat diffuse[] = glRGBA(192, 192, 192, 1.0f);
		GLfloat specular[] = glRGB(0, 0, 0);
		GLfloat emission[] = glRGB(0, 0, 0);
		GLfloat shininess[] = {0.0f};

		m_pDriver->Material(GL_FRONT, GL_AMBIENT, ambient);
		m_pDriver->Material(GL_FRONT, GL_DIFFUSE, diffuse);
		m_pDriver->Material(GL_FRONT, GL_SPECULAR, specular);
		m_pDriver->Material(GL_FRONT, GL_EMISSION, emission);
		m_pDriver->Material(GL_FRONT, GL_SHININESS, shininess);
	}

	m_pDriver->EndList();

	///////////////////////////////////////////////////////////////////////////
	// list of selected material properties
	m_pDriver->NewList(eMaterialTileTopSelected, GL_COMPILE);
	{
		GLfloat ambient[] = glRGB(192, 192, 192);
		GLfloat diffuse[] = glRGB(192, 192, 192);
		GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat emission[] = glRGB(92, 92, 0);
		GLfloat shininess[] = {0.0f};

		m_pDriver->Material(GL_FRONT, GL_AMBIENT, ambient);
		m_pDriver->Material(GL_FRONT, GL_DIFFUSE, diffuse);
		m_pDriver->Material(GL_FRONT, GL_SPECULAR, specular);
		m_pDriver->Material(GL_FRONT, GL_EMISSION, emission);
		m_pDriver->Material(GL_FRONT, GL_SHININESS, shininess);
	}

	m_pDriver->EndList();

	//////////////////////////////////
	//
	m_pDriver->NewList(eMaterialFaceNormal, GL_COMPILE);
	{
		//GLfloat ambient[] = glRGB(253, 251, 230);
		//GLfloat diffuse[] = glRGB(253, 251, 230);
		GLfloat ambient[] = glRGBA(255, 255, 255, 0.7f);
		GLfloat diffuse[] = glRGBA(255, 255, 255, 0.7f);
		GLfloat specular[] = glRGB(0, 0, 0);
		GLfloat emission[] = glRGB(0, 0, 0);
		GLfloat shininess[] = {0.0f};

		m_pDriver->Material(GL_FRONT, GL_AMBIENT, ambient);
		m_pDriver->Material(GL_FRONT, GL_DIFFUSE, diffuse);
		m_pDriver->Material(GL_FRONT, GL_SPECULAR, specular);
		m_pDriver->Material(GL_FRONT, GL_EMISSION, emission);
		m_pDriver->Material(GL_FRONT, GL_SHININESS, shininess);
	}

	m_pDriver->EndList();

	///////////////////////////////////////////////////////////////////////////
	// list of selected material properties
	m_pDriver->NewList(eMaterialFaceHint, GL_COMPILE);
	{
		GLfloat ambient[] = {0.2f, 0.2f, 1.0f, 1.0f};
		GLfloat diffuse[] = {0.2f, 0.2f, 1.0f, 1.0f};
		GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat emission[] = {0.0f, 0.0f, 1.0f, 1.0f};
		GLfloat shininess[] = {0.0f};

		m_pDriver->Material(GL_FRONT, GL_AMBIENT, ambient);
		m_pDriver->Material(GL_FRONT, GL_DIFFUSE, diffuse);
		m_pDriver->Material(GL_FRONT, GL_SPECULAR, specular);
		m_pDriver->Material(GL_FRONT, GL_EMISSION, emission);
		m_pDriver->Material(GL_FRONT, GL_SHININESS, shininess);
	}

	m_pDriver->EndList();

	///////////////////////////////////////////////////////////////////////////
	// list of selected material properties
	m_pDriver->NewList(eMaterialFaceSelected, GL_COMPILE);
	{
		GLfloat ambient[] = glRGB(255, 255, 0);
		GLfloat diffuse[] = glRGB(255, 255, 0);
		GLfloat specular[] = glRGB(0, 0, 0);
		GLfloat emission[] = glRGB(50, 50, 0);
		GLfloat shininess[] = {0.5f};

		m_pDriver->Material(GL_FRONT, GL_AMBIENT, ambient);
		m_pDriver->Material(GL_FRONT, GL_DIFFUSE, diffuse);
		m_pDriver->Material(GL_FRONT, GL_SPECULAR, specular);
		m_pDriver->Material(GL_FRONT, GL_EMISSION, emission);
		m_pDriver->Material(GL_FRONT, GL_SHININESS, shininess);
	}

	m_pDriver->EndList();

	///////////////////////////////////////////////////////////////////////////
	// list of material properties
	m_pDriver->NewList(LIST_BACKGROUND, GL_COMPILE);
	{
		m_pDriver->Begin(GL_QUADS);
		{
			m_pDriver->TexCoord(0.0f, 0.0f);
			m_pDriver->Vertex(-1, -1, 1);
			m_pDriver->TexCoord(1.0f, 0.0f);
			m_pDriver->Vertex(1, -1, 1);
			m_pDriver->TexCoord(1.0f, 1.0f);
			m_pDriver->Vertex(1, 1, 1);
			m_pDriver->TexCoord(0.0f, 1.0f);
			m_pDriver->Vertex(-1, 1, 1);
		}

		m_pDriver->End();
	}

	m_pDriver->EndList();


	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnTilesetDefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_objTileset.LoadResource();
	g_AppSettings.m_strTileset = _T("default");

	LoadTileTextures();

	UpdateView();
	UpdateUI();
	UpdateStatus();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggView::OnTilesetLoad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CTilesetDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		if (m_objTileset.Load(dlg.GetTilesetPath()))
		{
			g_AppSettings.m_strTileset = dlg.GetTilesetPath();
			LoadTileTextures();
		}
	}

	UpdateView();

	UpdateUI();
	UpdateStatus();

	return 0;
}

/**
 * @brief    CreateTileTexture
 *
 * CMahjonggView::CreateTileTexture
 *
 * @param GLuint unTextureName
 * @param ATL::CImage & objImage
 * @return void
 */
void CMahjonggView::CreateTileTexture(GLuint unTextureName, ATL::CImage& objImage)
{
	LPBYTE pImageBits = (LPBYTE)objImage.GetBits();
	_ASSERT(pImageBits != NULL);

	// get image size
	int nImageWidth = objImage.GetWidth();
	int nImageHeight = objImage.GetHeight();
	bool bUpsideDown = objImage.GetPitch() < 0;

	_ASSERT(nImageWidth > 0 && nImageHeight > 0);

	COLORREF* pSourceBits = new COLORREF[nImageWidth * nImageHeight];
	memset(pSourceBits, 0, nImageWidth * nImageHeight * sizeof(COLORREF));

	if(bUpsideDown)
	{
		int nDst = 0;
		for(int y = nImageHeight - 1; y >= 0; y--)
		{
			for(int x = 0; x < nImageWidth; x++)
			{
				COLORREF rgb = objImage.GetPixel(x, y);
				pSourceBits[nDst] = objImage.GetPixel(x, y);;
				nDst++;
			}
		}
	}
	else
	{
		int nDst = 0;
		for(int y = 0; y < nImageHeight; y++)
		{
			for(int x = 0; x < nImageWidth; x++)
			{
				COLORREF rgb = objImage.GetPixel(x, y);
				pSourceBits[nDst] = objImage.GetPixel(x, y);;
				nDst++;
			}
		}
	}

	int nTexWidth = g_AppSettings.m_bHighQualityTextures ? 256 : 64;
	int nTexHeight = g_AppSettings.m_bHighQualityTextures ? 256 : 64;

	LPBYTE pTextureBits = new BYTE[nTexWidth * nTexHeight * 4];

	_ASSERT(pTextureBits != NULL);
	memset(pTextureBits, 0, sizeof(BYTE) * nTexWidth * nTexHeight * 4);

	// scale texture image
	m_pDriver->ScaleImage(GL_RGBA, 
		nImageWidth, nImageHeight, GL_UNSIGNED_BYTE, pSourceBits, 
		nTexWidth, nTexHeight, GL_UNSIGNED_BYTE, pTextureBits);

	// create texture
	m_pDriver->BindTexture(GL_TEXTURE_2D, unTextureName);

	if (g_AppSettings.m_bHighQualityTextures)
	{
		m_pDriver->TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_pDriver->TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		m_pDriver->TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		m_pDriver->TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	m_pDriver->TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	m_pDriver->TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	m_pDriver->TexImage2D(0, GL_RGB, nTexWidth, nTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTextureBits);

	// release memory
	delete [] pTextureBits;
	delete [] pSourceBits;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggView::LoadTileTextures()
{
	QPROFILE_FUN(_T("CMahjonggView::LoadTileTextures"));

	CProgressDlg dlgProgress;
	dlgProgress.Create(m_hWnd);
	dlgProgress.SetRange(0, 45);
	dlgProgress.SetPos(0);

	for (int i = 0; i < 43; i++)
	{
		CreateTileTexture(m_arrTileTexNames[i], m_objTileset.GetTile(i));

		dlgProgress.SetPos(i + 1);
	}

	dlgProgress.SetPos(43);

	CreateTileTexture(m_arrTileSideTexNames[0], m_objTileset.GetLeft());

	dlgProgress.SetPos(44);
	CreateTileTexture(m_arrTileSideTexNames[1], m_objTileset.GetBottom());

	dlgProgress.DestroyWindow();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggView::LoadGeometry()
{
	{
		CLoad3DS loader;
		loader.Load(IDR_TILE_LOW, &m_objTileLowQuality);
	}

	{
		CLoad3DS loader;
		loader.Load(IDR_TILE_MEDIUM, &m_objTileMediumQuality);
	}

	{
		CLoad3DS loader;
		loader.Load(IDR_TILE_HIGH, &m_objTileHighQuality);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void CMahjonggView::OnUpdateOptions()
{
	C3DModel* pModel = NULL;

	switch (g_AppSettings.m_nGraphDetail)
	{

	case 0:
		pModel = &m_objTileLowQuality;
		break;

	case 1:
		pModel = &m_objTileMediumQuality;
		break;

	case 2:
		pModel = &m_objTileHighQuality;
		break;
	}

	_ASSERT(pModel != NULL);

	unsigned int uTriangles = 0;

	for (int i = 0; i < pModel->numOfObjects; i++)
	{
		// Make sure we have valid objects just in case. (size() is in the vector class)
		if (pModel->pObject.GetSize() <= 0) break;

		// Get the current object that we are displaying
		C3DObject *pObject = &pModel->pObject[i];

		if (CString(pObject->strName) == _T("Tile Face"))
			uTriangles += CreateListFromModel(eTileNear, pObject);

		if (CString(pObject->strName) == _T("Tile Body"))
			uTriangles += CreateListFromModel(eTileBody, pObject);
	}

	_TRACE(_T("Object loaded. %d triangles used\n"), uTriangles);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
unsigned int CMahjonggView::CreateListFromModel(GLuint nDisplayList, C3DObject* pObject)
{
	QPROFILE_FUN(_T("CMahjonggView::CreateListFromModel"));

	unsigned int uTriangles = 0;

	m_fTileWidth = 0.0f;
	m_fTileHeight = 0.0f;
	m_fTileDepth = 0.0f;

	float fwmin = 0.0f, fwmax = 0.0f, fhmin = 0.0f, fhmax = 0.0f, fdmin = 0.0f, fdmax = 0.0f;

	m_pDriver->NewList(nDisplayList, GL_COMPILE);

	// This determines if we are in wireframe or normal mode
	m_pDriver->Begin(GL_TRIANGLES);

	// Go through all of the faces (polygons) of the object and draw them

	for (int j = 0; j < pObject->numOfFaces; j++)
	{
		//if(!pObject->pFaces[j].bSmooth)
		if (!g_AppSettings.m_bSmoothTiles)
		{
			m_pDriver->Normal(pObject->pFaces[j].vFaceNormal.x,
												pObject->pFaces[j].vFaceNormal.y,
												pObject->pFaces[j].vFaceNormal.z);
		}

		// Go through each corner of the triangle and draw it.
		for (int whichVertex = 0; whichVertex < 3; whichVertex++)
		{
			// Get the index for each point of the face
			int index = pObject->pFaces[j].vertIndex[whichVertex];

			// Give OpenGL the normal for this vertex.
			//if(pObject->pFaces[j].bSmooth)

			if (g_AppSettings.m_bSmoothTiles)
			{
				m_pDriver->Normal(pObject->pNormals[index].x, pObject->pNormals[index].y, pObject->pNormals[index].z);
			}

			if (pObject->pTexVerts)
				m_pDriver->TexCoord(pObject->pTexVerts[index].x, pObject->pTexVerts[index].y);

			// vertex
			m_pDriver->Vertex(pObject->pVerts[index].x, pObject->pVerts[index].y, pObject->pVerts[index].z);

			if (pObject->pVerts[index].x < fwmin)
				fwmin = pObject->pVerts[index].x;

			if (pObject->pVerts[index].x > fwmax)
				fwmax = pObject->pVerts[index].x;

			if (pObject->pVerts[index].y < fhmin)
				fhmin = pObject->pVerts[index].y;

			if (pObject->pVerts[index].y > fhmax)
				fhmax = pObject->pVerts[index].y;

			if (pObject->pVerts[index].z < fdmin)
				fdmin = pObject->pVerts[index].z;

			if (pObject->pVerts[index].z > fdmax)
				fdmax = pObject->pVerts[index].z;
		}

		uTriangles++;
	}

	m_fTileWidth = fwmax - fwmin;

	m_fTileHeight = fhmax - fhmin;
	m_fTileDepth = fdmax - fdmin;

	m_pDriver->End();                // End the drawing

	m_pDriver->EndList();

	return uTriangles;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void CMahjonggView::UpdateStatus()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggView::UpdateUI()
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
inline void CMahjonggView::DrawTile(DWORD dwFlags, int nTileImage, GLfloat* pvLayer)
{
	QPROFILE_FUN(_T("CMahjonggView::DrawTile"));

	// draw tile body

	if (g_AppSettings.m_bTexturedSides)
	{
		// textured tile sides
		m_pDriver->Enable(GL_TEXTURE_2D);
		m_pDriver->BindTexture(GL_TEXTURE_2D, m_arrTileTexNames[0]);
		m_pDriver->CallList(eMaterialFaceNormal);
	}
	else
	{
		// plain tile sides
		m_pDriver->Disable(GL_TEXTURE_2D);
		m_pDriver->CallList(eMaterialTileTop);
	}

	if (pvLayer != NULL && g_AppSettings.m_bColoredLayers)
		m_pDriver->Material(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pvLayer);

	m_pDriver->CallList(eTileBody);

	// draw tile face
	if ((dwFlags & TF_FACE) == TF_FACE)
	{
		if (g_AppSettings.m_bHideClosedTiles && (dwFlags & TF_CLOSED) == TF_CLOSED)
		{
			if (g_AppSettings.m_bTexturedSides)
			{
				//
				m_pDriver->Enable(GL_TEXTURE_2D);
				m_pDriver->BindTexture(GL_TEXTURE_2D, m_arrTileTexNames[0]);
				m_pDriver->CallList(eMaterialFaceNormal);
			}
			else
			{
				m_pDriver->Disable(GL_TEXTURE_2D);
				m_pDriver->CallList(eMaterialTileTop);
			}
		}
		else
		{
			m_pDriver->Enable(GL_TEXTURE_2D);
			m_pDriver->BindTexture(GL_TEXTURE_2D, m_arrTileTexNames[nTileImage]);

			GLuint uMaterial = eMaterialFaceNormal;

			if ((dwFlags & TF_SELECTED) == TF_SELECTED)
				uMaterial = eMaterialFaceSelected;
			else if ((dwFlags & TF_HINT) == TF_HINT)
				uMaterial = eMaterialFaceHint;

			if (uMaterial == eMaterialFaceNormal && pvLayer != NULL && g_AppSettings.m_bColoredLayers)
				m_pDriver->Material(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pvLayer);
			else
				m_pDriver->CallList(uMaterial);
		}

		m_pDriver->CallList(eTileNear);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
void CMahjonggView::CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;		// This will store our fps
	static float lastTime			= 0.0f;		// This will hold the time from the last frame

	static float frameTime = 0.0f;				// This stores the last frame's time

	// Get the current time in seconds.  Notice that we use timeGetTime() instead
	// of GetTickCount().  This is because on some computers, depending on their
	// CPU, GetTickCount() does update as fast or correctly as clock().  You need
	// to include <mmsystem.h> to use this function.  You will also need to include
	// winmm.lib and mmsystem.h in your code.
	float currentTime = timeGetTime() * 0.001f;

	// Here we store the elapsed time between the current and last frame,
	// then keep the current frame in our static variable for the next frame.
	m_fFrameInterval = currentTime - frameTime;
	frameTime = currentTime;

	// Increase the frame counter
	++framesPerSecond;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.

	if ( currentTime - lastTime > 1.0f )
	{
		// Here we set the lastTime to the currentTime
		lastTime = currentTime;

		// Reset the frames per second
		framesPerSecond = 0;
	}
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CMahjonggView::Activate()
{
	m_bActive = true;
	return true;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CMahjonggView::Deactivate()
{
	m_bActive = false;
	return true;
}
