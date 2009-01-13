#include "stdafx.h"

#include "SolitaireView.h"

#include "..\..\Dialogs\DemoDlg.h"
#include "..\..\Service\random.h"
#include "..\..\MainFrm.h"
#include "..\..\Dialogs\ScoresDlg.h"
#include "..\..\Dialogs\Solitaire\PlayerNameDlg.h"
#include "..\..\Dialogs\Solitaire\BeginnerHintDlg.h"
#include "..\..\Dialogs\LayoutsDlg.h"

const UINT CSolitaireView::IDT_SECOND = 0x1000;
const UINT CSolitaireView::IDT_FRAME = 0x1001;
const UINT CSolitaireView::BUFSIZE = 1024;


/*
inline float absf(float fval)
{
  if(fval < 0.0f)
    return -1.0f;
  return 1.0f;
}
*/


CSolitaireView::CSolitaireView(CMainFrame* pContainer)
		: CMahjonggView(pContainer)
{
	m_fAngleX = 0.0f;
	m_fAngleY = 0.0f;
	m_fAngleZ = 0.0f;
	m_fRadius = 0.0f;

	m_bShowHint = false;

	m_enmBackgroundMode = eBGSolid;

	m_vBackground[0] = glCOLOR(getRValue(g_AppSettings.m_rgbBackground));
	m_vBackground[1] = glCOLOR(getGValue(g_AppSettings.m_rgbBackground));
	m_vBackground[2] = glCOLOR(getBValue(g_AppSettings.m_rgbBackground));
	m_vBackground[3] = 1.0f;

	m_bTexturesAreReady = false;

	m_enmDrawMode = eMahjongg3D;

	m_bActive = false;

	m_bLeftButtonDown = false;
	m_nHoldSeconds = 0;

	m_enmStatusMode = eNothing;

	m_arrTileName[1] = _T("str_tile_1");
	m_arrTileName[2] = _T("str_tile_2");
	m_arrTileName[3] = _T("str_tile_3");
	m_arrTileName[4] = _T("str_tile_4");
	m_arrTileName[5] = _T("str_tile_5");
	m_arrTileName[6] = _T("str_tile_6");
	m_arrTileName[7] = _T("str_tile_7");
	m_arrTileName[8] = _T("str_tile_8");
	m_arrTileName[9] = _T("str_tile_9");
	m_arrTileName[10] = _T("str_tile_10");
	m_arrTileName[11] = _T("str_tile_11");
	m_arrTileName[12] = _T("str_tile_12");
	m_arrTileName[13] = _T("str_tile_13");
	m_arrTileName[14] = _T("str_tile_14");
	m_arrTileName[15] = _T("str_tile_15");
	m_arrTileName[16] = _T("str_tile_16");
	m_arrTileName[17] = _T("str_tile_17");
	m_arrTileName[18] = _T("str_tile_18");
	m_arrTileName[19] = _T("str_tile_19");
	m_arrTileName[20] = _T("str_tile_20");
	m_arrTileName[21] = _T("str_tile_21");
	m_arrTileName[22] = _T("str_tile_22");
	m_arrTileName[23] = _T("str_tile_23");
	m_arrTileName[24] = _T("str_tile_24");
	m_arrTileName[25] = _T("str_tile_25");
	m_arrTileName[26] = _T("str_tile_26");
	m_arrTileName[27] = _T("str_tile_27");
	m_arrTileName[28] = _T("str_tile_28");
	m_arrTileName[29] = _T("str_tile_29");
	m_arrTileName[30] = _T("str_tile_30");
	m_arrTileName[31] = _T("str_tile_31");
	m_arrTileName[32] = _T("str_tile_32");
	m_arrTileName[33] = _T("str_tile_33");
	m_arrTileName[34] = _T("str_tile_34");
	m_arrTileName[35] = _T("str_tile_35");
	m_arrTileName[36] = _T("str_tile_36");
	m_arrTileName[37] = _T("str_tile_37");
	m_arrTileName[38] = _T("str_tile_38");
	m_arrTileName[39] = _T("str_tile_39");
	m_arrTileName[40] = _T("str_tile_40");
	m_arrTileName[41] = _T("str_tile_41");
	m_arrTileName[42] = _T("str_tile_42");

	m_enmMusic = eNoMusic;
}


CSolitaireView::~CSolitaireView()
{
}


bool CSolitaireView::Init()
{
	if (!TParent::Init())
		return false;

	//
	if (!ReloadImages())
		return false;

	HRSRC hRsrc = ::FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_GAME_WON), _T("JPEG"));

	if (hRsrc != NULL)
	{
		//_VERIFY(m_imgGameWon.LoadResource(hRsrc, ATL::CImage_FORMAT_JPG));
	}

	// load cursors
	m_arrCursors[eCursorPointer] = LoadCursor(NULL, IDC_ARROW);

	m_arrCursors[eCursorTile] = LoadCursor(_Module.m_hInstResource, MAKEINTRESOURCE(IDC_TILE));

	m_arrCursors[eCursorRotate] = LoadCursor(_Module.m_hInstResource, MAKEINTRESOURCE(IDC_ROTATING));

	// set point of view
	m_fAngleX = -20.0f;

	m_fAngleY = 0.0f;

	m_fAngleZ = 0.0f;

	m_fRadius = 11.0f;

	// create background texture name
	m_unBackgroundTexName = 0;

	glGenTextures(1, &m_unBackgroundTexName);

	m_bInitialized = true;

	// load background
	if (g_AppSettings.m_bBitmapBack)
	{
		if (g_AppSettings.m_strBackground == _T("default"))
		{
			HRSRC hRes = FindResource(_Module.m_hInstResource, MAKEINTRESOURCE(IDR_BACKGROUND), _T("JPEG"));
			//m_imgBackground.LoadResource(hRes, ATL::CImage_FORMAT_JPG);
		}
		else
		{
			//m_imgBackground.Load(g_AppSettings.m_strBackground, ATL::CImage_FORMAT_JPG);
		}
	}
	else
	{
		m_vBackground[0] = glCOLOR(getRValue(g_AppSettings.m_rgbBackground));
		m_vBackground[1] = glCOLOR(getGValue(g_AppSettings.m_rgbBackground));
		m_vBackground[2] = glCOLOR(getBValue(g_AppSettings.m_rgbBackground));
	}

	m_bTexturesAreReady = true;

	// create and load everything depending on options
	m_nGraphDetail = -1;
	OnUpdateOptions();

	// load layout

	if (g_AppSettings.m_strLayout == _T("*default*") || !m_objGame.LoadLayout(g_AppSettings.m_strLayout))
	{
		m_objGame.LoadMJLayout(IDRL_TRADITIONAL);
	}

	// load auto saved game
	CString strTmp;

	strTmp.LoadString(IDS_SAVE_GAME_FOLDER);

	if (!g_AppSettings.m_bAutoSave || FAILED(m_objGame.Load(MakeUserPath(strTmp) + _T("AutoSave.mjs"))) ||
			m_objGame.GetState() != CSolitaireGame::GS_GAME)
	{
		m_objGame.New();
	}

	// load scores
	m_objScores.Load(MakeUserPath(_T("")) + _T("scores.dat"));

	// set up timer
	::SetTimer(m_hWnd, IDT_SECOND, 1000, NULL);

	::SetTimer(m_hWnd, IDT_FRAME, 50, NULL);  // frame animation

	// set up cursor
	SetCursor(m_arrCursors[eCursorPointer]);

	// update user interface
	//UpdateUI();

	// update status bar
	//UpdateStatus();

	return true;
}
bool CSolitaireView::UpdateUI()
{
	UIEnable(ID_PLAY_UNDO, (m_objGame.GetBoard().CountTiles() != 144), FALSE);
	UIEnable(ID_GAME_REPEAT, (m_objGame.GetBoard().CountTiles() != 144), FALSE);

	UIEnable(ID_GAME_DEMO, FALSE, FALSE);

	UIEnable(ID_PLAY_HINT, FALSE, FALSE);
	UIEnable(ID_PLAY_SHUFFLE, FALSE, FALSE);

	UISetCheck(ID_BACKGROUND_DEFAULT, g_AppSettings.m_bBitmapBack && g_AppSettings.m_strBackground == _T("default"), FALSE);
	UISetCheck(ID_TILESET_DEFAULT, g_AppSettings.m_strTileset == _T("default"), FALSE);
	UISetCheck(ID_LAYOUT_DEFAULT, g_AppSettings.m_strLayout == _T("default"), FALSE);

	UIUpdateMenuBar(FALSE, TRUE);

	return true;
}


bool CSolitaireView::UpdateView()
{
	QPROFILE_FUN(_T("CSolitaireView::UpdateView"));

	if (!TParent::UpdateView())
		return false;

	switch (m_enmDrawMode)
	{

	case eMahjongg3D:

		if (DrawScene())
		{
			//glFinish();
			SwapBuffers(m_hDC);
		}

		break;

	case eFireWorks:

		if (DrawFireWorks())
		{
			//glFinish();
			SwapBuffers(m_hDC);
		}

		break;
	}

	return true;
}


LRESULT CSolitaireView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TParent::OnCreate(uMsg, wParam, lParam, bHandled);

	::SetTimer(m_hWnd, 2000, 40, NULL);

	return 0;
}
LRESULT CSolitaireView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TParent::OnDestroy(uMsg, wParam, lParam, bHandled);

	if (g_AppSettings.m_bAutoSave)
		m_objGame.Save(MakeUserPath(LoadResourceString(IDS_SAVE_GAME_FOLDER)) + _T("AutoSave.mjs"));

	return 0;
}


LRESULT CSolitaireView::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CSolitairePos posSelected;

	if (m_objGame.GetState() == CSolitaireGame::GS_GAME && g_AppSettings.m_bShowRBHints)
	{
		CPoint point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);

		if (PickTile(point, m_objGame.m_posCurrent))
		{
			if (!m_objGame.m_posCurrent.IsClear() && m_objGame.GetBoard().IsFree(m_objGame.m_posCurrent))
			{
				posSelected = m_objGame.m_posCurrent;
				m_bShowHint = true;
			}
		}

		if (m_bShowHint)
		{
			m_vecHint.RemoveAll();

			m_vecHint.Add(posSelected);

			for (UINT k = 0; k < m_objGame.GetBoard().GetDepth(); k++)
			{
				for (UINT i = 0; i < m_objGame.GetBoard().GetWidth(); i++)
				{
					for (UINT j = 0; j < m_objGame.GetBoard().GetHeight(); j++)
					{
						if (!m_objGame.GetBoard().IsFree(i, j, k) || CSolitairePos(i, j, k) == posSelected)
							continue;

						if (TileEqual(m_objGame.GetBoard().GetTile(i, j, k), m_objGame.GetBoard().GetTile(posSelected)))
							m_vecHint.Add(CSolitairePos(i, j, k));
					}
				}
			}

			if (m_vecHint.GetSize() == 1)
			{
				m_vecHint.RemoveAll();
				m_bShowHint = false;
			}
			else
			{
				UpdateView();
			}
		}

		m_enmStatusMode = eHint;

		UpdateStatus();
	}

	m_bRButtonDown = true;

	// rotation mode
	m_pntRButtonDown.x = GET_X_LPARAM(lParam);
	m_pntRButtonDown.y = GET_Y_LPARAM(lParam);

	SetCapture();

	return 0;
}
LRESULT CSolitaireView::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bRButtonDown = false;

	m_vecHint.RemoveAll();

	m_bShowHint = false;

	ReleaseCapture();

	UpdateView();

	m_enmStatusMode = eNothing;
	UpdateStatus();

	return 0;
}


LRESULT CSolitaireView::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	short sMove = HIWORD(wParam);

	GLfloat fMove = ((GLfloat) sMove) / (120.0f * 3.0f);

	m_fRadius += fMove;

	if (m_fRadius < 8.0f)
		m_fRadius = 8.0f;

	if (m_fRadius > 20.0f)
		m_fRadius = 20.0f;

	UpdateView();

	return 0;
}


LRESULT CSolitaireView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//QPROFILE_FUN("CSolitaireView::OnMouseMove");

	CPoint point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	CSolitairePos posTile;

	if (m_objGame.GetState() == CSolitaireGame::GS_GAME &&
			g_AppSettings.m_bSmartCursor &&
			PickTile(point, posTile) &&
			!posTile.IsClear() && m_objGame.GetBoard().IsFree(posTile))
	{
		SetCursor(m_arrCursors[eCursorTile]);
	}
	else
	{
		SetCursor(m_arrCursors[eCursorPointer]);
	}

	// check rotating
	if (m_bRButtonDown)
	{
		GLfloat fAngleX = m_fAngleX;
		GLfloat fAngleZ = m_fAngleZ;

		GLfloat dx = (GLfloat)(point.x - m_pntRButtonDown.x);
		GLfloat dy = (GLfloat)(point.y - m_pntRButtonDown.y);

		float fSpeed = m_fFrameInterval * 0.75f;

		m_fAngleZ += dx * fSpeed;
		m_fAngleX += dy * fSpeed;

		if (m_fAngleX < -90.0f) m_fAngleX = -90.0f;

		if (m_fAngleX > 0.0f) m_fAngleX = 0.0f;

		if (m_fAngleZ < -180.0f) m_fAngleZ = 180.0f - ( -180 - m_fAngleZ);

		if (m_fAngleZ > 180.0f) m_fAngleZ = -180.0f - ( 180 - m_fAngleZ);

		if (m_bShowHint)
		{
			GLfloat rMotion = (GLfloat)sqrt(dx * dx + dy * dy);

			if (rMotion > 5)
			{
				m_bShowHint = false;
			}
		}

		if (m_fAngleX != fAngleX || m_fAngleZ != fAngleZ)
		{
			UpdateView();
		}
	}

	return 0;
}


LRESULT CSolitaireView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//QPROFILE_FUN("CSolitaireView::OnLButtonDown");

	CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	m_nHoldSeconds = 0;

	switch (m_enmDrawMode)
	{

	case eMahjongg3D:
	{
		if (m_objGame.GetState() == CSolitaireGame::GS_GAME)
		{
			CSolitairePos posTile;

			if (!PickTile(pt, posTile))
			{
				m_objGame.m_posCurrent.Clear();

				if (!m_objGame.m_posSelected.IsClear())
				{
					m_objGame.m_posSelected.Clear();
				}
			}
			else
			{
				m_objGame.m_posCurrent = posTile;
			}

			m_bLeftButtonDown = true;

		}
	}

	break;

	case eFireWorks:
	{
		SendMessage(WM_COMMAND, MAKEWPARAM(IDC_STOP_FIREWORKS, 0), 0);
	}

	break;
	}

	UpdateStatus();

	UpdateUI();

	return 0;
}


LRESULT CSolitaireView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	if (m_objGame.GetState() == CSolitaireGame::GS_GAME)
	{
		m_bLeftButtonDown = false;

		//if(m_objGame.m_nMatches > 0)
		//{
		//  m_objGame.m_nMatches = 0;
		//  m_bNeedToUpdate = true;
		//}

		CSolitairePos posTile;

		if (!PickTile(pt, posTile) || m_objGame.m_posCurrent != posTile)
		{
			m_enmStatusMode = eNothing;
			m_nStatusSeconds = 0;
			m_objGame.m_posCurrent.Clear();
		}

		if (!m_objGame.m_posCurrent.IsClear())
		{
			if (!m_objGame.m_objBoard.IsFree(m_objGame.m_posCurrent) || m_nHoldSeconds > 1)
			{
				m_enmStatusMode = eNothing;
				m_nStatusSeconds = 0;
				m_objGame.m_posCurrent.Clear();
			}
			else
			{
				if (m_objGame.m_posSelected.IsClear())
				{
					// select
					Sound(IDR_SELECT);
					m_objGame.m_posSelected = m_objGame.m_posCurrent;
					//m_objGame.m_posCurrent.RemoveAll();
					m_enmStatusMode = eTileName;
					m_nStatusSeconds = 2;
					m_bNeedToUpdate = true;
				}
				else if (m_objGame.m_posSelected == m_objGame.m_posCurrent)
				{
					// unselect
					Sound(IDR_SELECT2);
					m_objGame.m_posSelected.Clear();
					m_objGame.m_posCurrent.Clear();
					m_enmStatusMode = eNothing;
					m_bNeedToUpdate = true;
				}
				else
				{
					if (TileEqual(m_objGame.m_objBoard.GetTile(m_objGame.m_posCurrent), m_objGame.m_objBoard.GetTile(m_objGame.m_posSelected)))
					{
						m_objGame.RemoveTiles(m_objGame.m_posCurrent, m_objGame.m_posSelected);

						m_objGame.m_posSelected.Clear();
						m_objGame.m_posCurrent.Clear();

						m_bTilesRemoved = true;
						m_nStatusSeconds = 3;

						m_enmStatusMode = eNothing;

						Sound(IDR_REMOVE);

						m_bNeedToUpdate = true;

						if (m_objGame.m_objBoard.CountTiles() == 0)
						{
							m_objGame.m_enmState = CSolitaireGame::GS_GAMEWON;
							GameWon();
							CSolitaireGame::m_nGamesPlayed++;
						}
						else if (!m_objGame.m_objBoard.HaveMoreMoves())
						{
							m_objGame.m_enmState = CSolitaireGame::GS_GAMEOVER;
							GameOver();
							CSolitaireGame::m_nGamesPlayed++;
						}

						if (g_AppSettings.m_bAutoSave)
							m_objGame.Save(MakeUserPath(LoadResourceString(IDS_SAVE_GAME_FOLDER)) + _T("AutoSave.mjs"));
					}
				}
			}
		}

		if (m_bNeedToUpdate)
		{
			UpdateView();
		}

		UpdateStatus();

		UpdateUI();
	}

	return 0;
}


inline void CSolitaireView::DrawBoard(float fBoardCx, float fBoardCy)
{
	char imin = 0, imax = m_objGame.GetBoard().GetWidth(), istep = 1;
	char jmin = 0, jmax = m_objGame.GetBoard().GetHeight(), jstep = 1;

	static GLfloat vColors[10][4] =
	{
		{0.55f, 0.55f, 0.55f, 0.7f},
		{0.60f, 0.60f, 0.60f, 0.7f},
		{0.65f, 0.65f, 0.65f, 0.7f},
		{0.70f, 0.70f, 0.70f, 0.7f},
		{0.75f, 0.75f, 0.75f, 0.7f},
		{0.80f, 0.80f, 0.80f, 0.7f},
		{0.85f, 0.85f, 0.85f, 0.7f},
		{0.90f, 0.90f, 0.90f, 0.7f},
		{0.95f, 0.95f, 0.95f, 0.7f},
		{1.00f, 1.00f, 1.00f, 0.7f}
	};

	for (unsigned char k = 0; k < m_objGame.GetBoard().GetDepth(); k++)
	{
		for (unsigned char i = imin; i < imax; i += istep)
		{
			for (unsigned char j = 0; j < jmax; j += jstep)
			{
				if (m_objGame.GetBoard().GetTile(i, j, k) != NO_TILE && m_objGame.GetBoard().GetTile(i, j, k) != EMPTY_TILE)
				{
					GLfloat fi = ((GLfloat) i - fBoardCx) * m_fTileWidth / 2.0f;
					GLfloat fj = ((GLfloat) j - fBoardCy) * m_fTileHeight / 2.0f;
					GLfloat fk = (GLfloat) k * m_fTileDepth;

					// set tile position
					glPushMatrix();

					glTranslatef(fi, fj, fk);

					// tile drawing flags
					DWORD dwFlags = 0L;

					/*
					if(i < 2 || m_objGame.GetBoard().GetTile(i - 2, j, k) == NO_TILE)
					dwFlags |= TF_LEFT_SIDE;

					if(i > m_objGame.GetBoard().GetWidth() - 3 || m_objGame.GetBoard().GetTile(i + 2, j, k) == NO_TILE)
					dwFlags |= TF_RIGHT_SIDE;

					if(j < 2 || m_objGame.GetBoard().GetTile(i, j - 2, k) == NO_TILE)
					dwFlags |= TF_BOTTOM_SIDE;

					if(j > m_objGame.GetBoard().GetHeight() - 3 || m_objGame.GetBoard().GetTile(i, j + 2, k) == NO_TILE)
					dwFlags |= TF_TOP_SIDE;
					*/

					if ( k == m_objGame.GetBoard().GetDepth() - 1 ||
							 (k < m_objGame.GetBoard().GetDepth() - 1 && m_objGame.GetBoard().GetTile(i, j, k + 1) == NO_TILE))
						dwFlags |= TF_FACE;

					// tile is selected
					if (!m_objGame.GetSelection().IsClear() && m_objGame.GetSelection() == CSolitairePos(i, j, k))
						dwFlags |= TF_SELECTED;

					if (!m_objGame.GetBoard().IsFree(i, j, k))
						dwFlags |= TF_CLOSED;

					// tile is in hint array
					if (m_bShowHint)
					{
						for (int idx = 0; idx < m_vecHint.GetSize(); idx++)
						{
							if (m_vecHint[idx] == CSolitairePos(i, j, k))
							{
								dwFlags |= TF_HINT;
								break;
							}
						}
					}

					dwFlags |= TF_TEXTURES;

					//
					DrawTile(dwFlags, m_objGame.GetBoard().GetTile(i, j, k),
									 vColors[9 - m_objGame.GetBoard().GetDepth() + k]);

					// return to previous state
					glPopMatrix();
				}
			}
		}
	}
}


bool CSolitaireView::DrawScene()
{
	if (m_bBusy || !m_bInitialized || !m_bTexturesAreReady)
		return false;

	QPROFILE_MT_FUN(_T("CSolitaireView::DrawScene"));

	float currentTime = timeGetTime() * 0.001f;

	m_bBusy = true;

	// clear depth buffer & colour buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//if (g_AppSettings.m_bSmoothTiles)
	//	glEnable(GL_MULTISAMPLE_ARB);

	// background colour or image
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	glNormal3f(0.0f, 0.0f, -1.0f);

	glEnable(GL_LIGHT0);


	// background
	switch (m_enmBackgroundMode)
	{

	case eBGSolid:
	{
		GLfloat vEmission[] = glRGB(0, 0, 0);
		glDisable(GL_TEXTURE_2D);
		glCallList(LIST_BACKGROUND_MATERIAL);
		glMaterialfv(GL_FRONT, GL_AMBIENT, m_vBackground);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_vBackground);
		glMaterialfv(GL_FRONT, GL_EMISSION, vEmission);
		glCallList(LIST_BACKGROUND);
		break;
	}

	case eBGImage:
	{
		GLfloat vEmission[] = glRGB(0, 0, 0);

		glCallList(LIST_BACKGROUND_IMAGE_MATERIAL);
		glMaterialfv(GL_FRONT, GL_EMISSION, vEmission);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_unBackgroundTexName);
		glCallList(LIST_BACKGROUND);
		break;
	}
	}

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	// set viewport
	glTranslatef(0.0f, 0.0f, -m_fRadius);

	glRotatef(m_fAngleX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fAngleY, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fAngleZ, 0.0f, 0.0f, 1.0f);

	GLfloat fBoardCx = (GLfloat) m_objGame.GetBoard().GetWidth() / 2.0f;
	GLfloat fBoardCy = (GLfloat) m_objGame.GetBoard().GetHeight() / 2.0f;

	// lightning

	if (m_objGame.GetState() == CSolitaireGame::GS_GAME || m_objGame.GetState() == CSolitaireGame::GS_DEMO)
	{
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}

	// enable or disable selection light
	if (!m_objGame.GetSelection().IsClear())
	{
		GLfloat fi = ((GLfloat) m_objGame.GetSelection().i - fBoardCx) * m_fTileWidth / 2.0f;
		GLfloat fj = ((GLfloat) m_objGame.GetSelection().j - fBoardCy) * m_fTileHeight / 2.0f;
		GLfloat fk = ((GLfloat) m_objGame.GetSelection().k + 0.5f) * m_fTileDepth;

		glPushMatrix();

		glTranslatef(fi, fj, fk);

		glCallList(LIST_LIGHT);

		glEnable(GL_LIGHT1);

		glPopMatrix();
	}
	else
	{
		// we do not have selected tile
		glDisable(GL_LIGHT1);
	}

	// draw board tiles
	DrawBoard(fBoardCx, fBoardCy);

	glPopMatrix();

	// no more moves message
	if (m_objGame.GetState() == CSolitaireGame::GS_GAMEOVER)
	{
		glMatrixMode(GL_PROJECTION);

		glPushMatrix();

		glLoadIdentity();

		GLfloat fWidth = ((GLfloat)m_objSize.cx - (GLfloat)m_imgGameOver.GetWidth()) / (GLfloat)m_objSize.cx;
		GLfloat fHeight = ((GLfloat)m_objSize.cy - (GLfloat)m_imgGameOver.GetHeight()) / (GLfloat)m_objSize.cy;

		glTranslatef(fWidth - 1.0f , fHeight - 1.0f, 0.0f);
		//glTranslate(-fWidth / 2.0f , 0.0f, 0.0f);

		glRasterPos2f(0.0f, 0.0f);

		glDrawPixels(m_imgGameOver.GetWidth(), m_imgGameOver.GetHeight(),
								 GL_BGR_EXT, GL_UNSIGNED_BYTE, m_imgGameOver.GetBits());

		glPopMatrix();
	}

	//if (g_AppSettings.m_bSmoothTiles)
	//	glDisable(GL_MULTISAMPLE_ARB);

	// finish scene description
	glFinish();

	// force execution
	glFlush();

	m_bBusy = false;

	m_fFrameInterval = timeGetTime() * 0.001f - currentTime;

	return true;
}
bool CSolitaireView::PickTile(CPoint point, CSolitairePos& posTile)
{
	if (m_bBusy || !m_bInitialized || !m_bTexturesAreReady)
		return false;

	QPROFILE_MT_FUN(_T("CSolitaireView::PickTile"));

	m_bBusy = true;

	glClear(GL_DEPTH_BUFFER_BIT);

	GLuint selectBuf[BUFSIZE];

	ZeroMemory(selectBuf, sizeof(selectBuf));

	GLint hits = 0;

	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(BUFSIZE, selectBuf);

	glRenderMode(GL_SELECT);

	glInitNames();

	glPushName((unsigned) - 1);

	int nClientW = (m_rectOld.right - m_rectOld.left);

	int nClientH = (m_rectOld.bottom - m_rectOld.top);

	// set pick matrix
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	// set new projection matrix
	glLoadIdentity();

	gluPickMatrix((GLdouble) point.x, (GLdouble) (nClientH - point.y), 1.0, 1.0, viewport);

	gluPerspective(45.0f, (GLfloat)nClientW / (GLfloat)nClientH, 1.0f, 40.0f);

	// enter model mode
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	// clear depth buffer & colour buffer
	glLoadIdentity();

	// set viewport
	glTranslatef(0.0f, 0.0f, -m_fRadius);

	glRotatef(m_fAngleX, 1.0f, 0.0f, 0.0f);

	glRotatef(m_fAngleY, 0.0f, 1.0f, 0.0f);

	glRotatef(m_fAngleZ, 0.0f, 0.0f, 1.0f);

	GLfloat fBoardCx = (GLfloat) m_objGame.GetBoard().GetWidth() / 2.0f;

	GLfloat fBoardCy = (GLfloat) m_objGame.GetBoard().GetHeight() / 2.0f;

	// draw board tiles
	for (unsigned char k = 0; k < m_objGame.GetBoard().GetDepth(); k++)
	{
		glLoadName(k);

		for (unsigned char i = 0; i < m_objGame.GetBoard().GetWidth(); i++)
		{
			glPushName(i);

			for (unsigned char j = 0; j < m_objGame.GetBoard().GetHeight(); j++)
			{
				glPushName(j);

				if (m_objGame.GetBoard().GetTile(i, j, k) != NO_TILE)
				{
					GLfloat fi = ((GLfloat) i - fBoardCx) * m_fTileWidth / 2.0f;
					GLfloat fj = ((GLfloat) j - fBoardCy) * m_fTileHeight / 2.0f;
					GLfloat fk = (GLfloat) k * m_fTileDepth;

					// set tile position
					glPushMatrix();

					glTranslatef(fi, fj, fk);

					//
					DWORD dwFlags = 0L;

					//if( k == m_objGame.GetBoard().GetDepth() - 1 ||
					//  (k < m_objGame.GetBoard().GetDepth() - 1 && m_objGame.GetBoard().GetTile(i, j, k+1) == NO_TILE))
					dwFlags |= TF_FACE;

					DrawTile(dwFlags);

					// return to previous state
					glPopMatrix();
				}

				glPopName();
			}

			glPopName();
		}
	}

	glPopMatrix();

	// restore projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// force execution
	glFlush();

	// finish scene description
	//glFinish();

	glMatrixMode(GL_MODELVIEW);

	// execute commands
	//glFlush();

	// set render mode
	hits = glRenderMode(GL_RENDER);

	posTile.Clear();

	if (hits > 0)
	{
		int i = 0, j = 0;
		GLint names = 0;
		GLuint *ptr = NULL;

		ptr = (GLuint*) selectBuf;

		for (i = 0; i < hits; i++)
		{
			names = *ptr;
			// pass system information
			ptr++;
			ptr++;
			ptr++;

			for (j = 0; j < names; j++)
			{
				//  for each name
				if (j == 0)
					posTile.k = *ptr;

				if (j == 1)
					posTile.i = *ptr;

				if (j == 2)
					posTile.j = *ptr;

				ptr++;
			}
		}
	}

	m_bBusy = false;

	return true;
}


bool CSolitaireView::DrawFireWorks()
{
	if (m_bBusy || !m_bInitialized || !m_bTexturesAreReady)
		return false;

	QPROFILE_MT_FUN(_T("CSolitaireView::DrawFireWorks"));

	m_bBusy = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	glNormal3f(0.0f, 0.0f, -1.0f);

	glColor3ub(0, 0, 64);

	glDisable(GL_TEXTURE_2D);

	glCallList(LIST_BACKGROUND);

	glLoadIdentity();


	GLfloat fWidth = ((GLfloat)m_objSize.cx - (GLfloat)m_imgGameWon.GetWidth()) / (GLfloat)m_objSize.cx;

	GLfloat fHeight = ((GLfloat)m_objSize.cy - (GLfloat)m_imgGameWon.GetHeight()) / (GLfloat)m_objSize.cy;

	glTranslatef(fWidth - 1.0f , fHeight - 1.0f, 0.0f);

	glRasterPos2f(0.0f, 0.0f);

	glDrawPixels(m_imgGameWon.GetWidth(), m_imgGameWon.GetHeight(),
							 GL_BGR_EXT, GL_UNSIGNED_BYTE, m_imgGameWon.GetBits());


	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -3.0f);

	glPointSize(1.8f);

	m_objFireWorks.Draw();

	glPopMatrix();

	// finish scene description
	glFinish();

	// force execution
	glFlush();

	m_bBusy = false;

	return true;
}


bool CSolitaireView::CreateLists(HDC hDC)
{
	TParent::CreateLists(hDC);

	///////////////////////////////////////////////////////////////////////////
	// list of selection light
	glNewList(LIST_LIGHT, GL_COMPILE);
	{
		GLfloat ambient[] = {0.2f, 0.2f, 0.0f, 1.0f};
		GLfloat diffuse[] = {1.0f, 1.0f, 0.0f, 1.0f};
		GLfloat position[] = { m_fTileWidth / 2.0f, m_fTileHeight / 2.0f, 0.4f, 1.0f};

		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, position);

		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.6f);
	}

	glEndList();

	///////////////////////////////////////////////////////
	//
	glNewList(LIST_BACKGROUND_MATERIAL, GL_COMPILE);
	{
		GLfloat specular[] = glRGB(0, 0, 64);
		GLfloat emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat shininess[] = {0.0f};

		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	}

	glEndList();

	/////////////////////////////////////////////////
	glNewList(LIST_BACKGROUND_IMAGE_MATERIAL, GL_COMPILE);
	{
		GLfloat ambient[] = glRGB(255, 255, 255);
		GLfloat diffuse[] = glRGB(255, 255, 255);
		GLfloat specular[] = glRGB(255, 255, 255);
		GLfloat emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat shininess[] = {0.0f};

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	}

	glEndList();

	/////////////////////////////////////////////////
	glNewList(LIST_GAME_OVER_STRING, GL_COMPILE);
	{

	}

	glEndList();

	glNewList(LIST_TEXT_COLOR, GL_COMPILE);
	{
		GLfloat ambient[] = glRGB(255, 255, 255);
		GLfloat diffuse[] = glRGB(255, 255, 255);
		GLfloat specular[] = glRGB(0, 0, 0);
		GLfloat emission[] = glRGB(0, 0, 0);
		GLfloat shininess[] = {0.0f};

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	}

	glEndList();

	glNewList(LIST_TEXT_BACKGROUND, GL_COMPILE);
	{
		GLfloat ambient[] = glRGB(0, 0, 0);
		GLfloat diffuse[] = glRGB(0, 0, 0);
		GLfloat specular[] = glRGB(0, 0, 0);
		GLfloat emission[] = glRGB(0, 0, 0);
		GLfloat shininess[] = {0.0f};

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	}

	glEndList();

	/////////////////////////////////////////////////
	// font
	//m_nFontOffset = (*pfnglGenLists)(256);

	{
		CFont* pFont = new CFont();
		pFont->CreateFont(10, 0, 0, 0, 600, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS,
											CLIP_DEFAULT_PRECIS, DRAFT_QUALITY,
											DEFAULT_PITCH | FF_SWISS, _T("Arial"));

		HFONT hOldFont = (HFONT)SelectObject(hDC, pFont->m_hFont);

		GLYPHMETRICSFLOAT agmf[256];
		BOOL bResult = wglUseFontOutlines(hDC, 0, 255, 1000, 0.0f, 0.1f, WGL_FONT_POLYGONS, &agmf[0]);

		SelectObject(hDC, hOldFont);
		delete pFont;
	}


	return true;
}


LRESULT CSolitaireView::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	static SYSTEMTIME tm = {0, 0, 0, 0, 0, 0, 0, 0};

	if (wParam == IDT_SECOND)
	{
		bool bUpdate = false;

		if (m_bTilesRemoved)
		{
			if (m_nStatusSeconds == 0)
			{
				if (m_objGame.GetState() == CSolitaireGame::GS_GAME)
					m_enmStatusMode = eNothing;

				m_bTilesRemoved = false;

				bUpdate = true;
			}
			else
			{
				m_nStatusSeconds--;
			}
		}

		if (!m_objGame.m_posCurrent.IsClear())
		{
			//if(m_nStatusSeconds == 0)
			//{
			//  m_enmStatusMode = eNothing;
			//}
			//else
			//{
			//  m_nStatusSeconds--;
			//}

			if (m_bLeftButtonDown)
			{
				if (m_nHoldSeconds > 1)
				{
					if (!m_objGame.m_objBoard.IsFree(m_objGame.m_posCurrent))
					{
					}
					else
					{
						//FillMatchLine(m_objGame.m_posCurrent);
						//if(m_objGame.m_nMatches > 0)
						//{
						//  m_bNeedToUpdate = true;
						//  Redraw();
						//  CopyBuffer();
						//}
					}

					//m_enmStatusMode = eHint;
				}
				else
				{
					m_enmStatusMode = eTileName;
				}

				m_nHoldSeconds++;
			}
		}

		if (m_bActive && m_objGame.GetState() == CSolitaireGame::GS_GAME)
		{
			m_objGame.TickSecond();
			bUpdate = true;
		}

		if (bUpdate)
			UpdateStatus();

		/*
		*/
	}

	if (wParam == IDT_FRAME)
	{
		if (m_enmDrawMode == eFireWorks)
		{
			m_objFireWorks.Step();
			UpdateView();
		}
	}

	return 0;
}


inline void Gradient(float fPos, float* pfRGB,
										 float fPos0, float* pfRGB0,
										 float fPos1, float* pfRGB1)
{
	for (int i = 0; i < 3; i++)
	{
		pfRGB[i] = (pfRGB1[i] - pfRGB0[i]) / (fPos1 - fPos0) * (fPos - fPos0) + pfRGB0[i];
	}
}


#define TM(H) ((H) * 60.0f)

bool CSolitaireView::SetupLights(SYSTEMTIME& tm)
{
	TParent::SetupLights(tm);

	bool bChanged = true;

	GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat diffuse[4] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat pos[4] = { -4.0f , 4.0f, -2.0f, 1.0f };

	/*
	REG_CRYPT_BEGIN;

	static float ambient_night[3] = {0.2f, 0.2f, 0.25f};
	static float ambient_morning[3] = {0.35f, 0.30f, 0.25f};
	static float ambient_day[3] = {0.4f, 0.4f, 0.4f};
	static float ambient_evening[3] = {0.35f, 0.35f, 0.25f};

	static float diffuse_night[3] = {0.6f, 0.6f, 0.7f};
	static float diffuse_morning[3] = {0.75f, 0.6f, 0.7f};
	static float diffuse_day[3] = {0.8f, 0.8f, 0.8f};
	static float diffuse_evening[3] = {0.7f, 0.6f, 0.7f};

	if(g_AppSettings.m_bEnvLight)
	{
	float fTime = (float)((tm.wHour * 60) + tm.wMinute);

	if(fTime > TM(0) && fTime <= TM(6))
	{
	Gradient(fTime, ambient, TM(0), ambient_night, TM(6), ambient_night);
	Gradient(fTime, diffuse, TM(0), diffuse_night, TM(6), diffuse_night);
	} else if(fTime > TM(6) && fTime <= TM(8))
	{
	Gradient(fTime, ambient, TM(6), ambient_night, TM(8), ambient_morning);
	Gradient(fTime, diffuse, TM(6), diffuse_night, TM(8), diffuse_morning);
	} else if(fTime > TM(8) && fTime <= TM(10))
	{
	Gradient(fTime, ambient, TM(8), ambient_morning, TM(10), ambient_day);
	Gradient(fTime, diffuse, TM(8), diffuse_morning, TM(10), diffuse_day);
	} else if(fTime > TM(10) && fTime <= TM(18))
	{
	Gradient(fTime, ambient, TM(10), ambient_day, TM(18), ambient_day);
	Gradient(fTime, diffuse, TM(10), diffuse_day, TM(18), diffuse_day);
	} else if(fTime > TM(18) && fTime <= TM(20))
	{
	Gradient(fTime, ambient, TM(18), ambient_day, TM(20), ambient_evening);
	Gradient(fTime, diffuse, TM(18), diffuse_day, TM(20), diffuse_evening);
	} else if(fTime > TM(20) && fTime <= TM(22))
	{
	Gradient(fTime, ambient, TM(20), ambient_evening, TM(22), ambient_night);
	Gradient(fTime, diffuse, TM(20), diffuse_evening, TM(22), diffuse_night);
	} if(fTime > TM(22) && fTime <= TM(24))
	{
	Gradient(fTime, ambient, TM(22), ambient_night, TM(24), ambient_night);
	Gradient(fTime, diffuse, TM(22), diffuse_night, TM(24), diffuse_night);
	}

	float fLightPos = (float)(tm.wHour * 60 + tm.wMinute) * 3.1415926f / TM(24);

	GLfloat position[4] = { 4.0f * cosf(fLightPos), 4.0f * sinf(fLightPos), -2.0f * sinf(fLightPos), 1.0f };
	memcpy(pos, position, sizeof(GLfloat) * 4);
	}
	REG_CRYPT_END;
	*/

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	//UpdateView();

	return bChanged;
}


LRESULT CSolitaireView::OnHelpUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_objGame.Undo())
		InvalidateRect(NULL);

	UpdateStatus();

	UpdateUI();

	return 0;
}


LRESULT CSolitaireView::OnGameNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_objGame.GetState() == CSolitaireGame::GS_GAME && m_objGame.GetBoard().CountTiles() != 144)
	{
		CString strMessage = g_LangManager.GetString("str_new_game");

		if (MessageBox(strMessage, _T("Amazing Mahjongg 3D"), MB_YESNO | MB_ICONQUESTION) != IDYES)
			return 0;
	}

	m_objGame.New();

	m_objScores.GameStarted(m_objGame.GetLayout().GetName());

	UpdateView();

	UpdateUI();
	UpdateStatus();

	return 0;
}


LRESULT CSolitaireView::OnGameRepeat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_objGame.Replay();

	UpdateView();

	UpdateUI();
	UpdateStatus();

	return 0;
}


LRESULT CSolitaireView::OnGameSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strFilter = g_LangManager.GetString("str_save_game_filter");

	for (int i = 0; i < strFilter.GetLength(); i++)
	{
		if (strFilter.GetAt(i) == '\n')
			strFilter.SetAt(i, '\0');
	}

	CFileDialog dlg(FALSE, _T("mjs"), NULL,

									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									strFilter,
									m_hWnd);

	CString strTmp;
	strTmp.LoadString(IDS_SAVE_GAME_FOLDER);

	CString strInitialDir = MakeUserPath(strTmp);
	dlg.m_ofn.lpstrInitialDir = strInitialDir;

	if (dlg.DoModal() == IDOK)
	{
		m_objGame.Save(dlg.m_szFileName);
	}

	UpdateUI();

	UpdateStatus();

	return 0;
}


LRESULT CSolitaireView::OnGameLoad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strFilter = g_LangManager.GetString("str_save_game_filter");

	for (int i = 0; i < strFilter.GetLength(); i++)
	{
		if (strFilter.GetAt(i) == '\n')
			strFilter.SetAt(i, '\0');
	}

	CFileDialog dlg(TRUE, _T("mjs"), NULL,

									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									strFilter,
									m_hWnd);

	CString strTmp;
	strTmp.LoadString(IDS_SAVE_GAME_FOLDER);

	CString strInitialDir = MakeUserPath(strTmp);
	dlg.m_ofn.lpstrInitialDir = strInitialDir;

	if (dlg.DoModal() == IDOK)
	{
		if (FAILED(m_objGame.Load(dlg.m_szFileName)))
			m_objGame.New();
	}

	UpdateView();

	UpdateUI();
	UpdateStatus();

	return 0;
}


LRESULT CSolitaireView::OnGameDemo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_objGame.GetState() == CSolitaireGame::GS_GAME && m_objGame.GetBoard().CountTiles() != 144)
	{
		CString strMessage = g_LangManager.GetString("str_new_game");

		if (MessageBox(strMessage, _T("Amazing Mahjongg 3D"), MB_YESNO | MB_ICONQUESTION) != IDYES)
			return 0;
	}

	m_objGame.Demo();

	m_vecHint.RemoveAll();

	m_bShowHint = false;

	UpdateView();

	UpdateUI();
	UpdateStatus();

	return 0;
}


LRESULT CSolitaireView::OnGameScores(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CScoresDlg dlg;
	dlg.m_pScoreTable = &m_objScores;

	if (dlg.DoModal() == IDC_SCORES_REPLAY)
	{
		SScoreRecord* pRec = dlg.m_pSelectedRecord;

		//m_objGame.Replay(pRec->m_objOriginalBoard);

		UpdateView();

		UpdateUI();
		UpdateStatus();
	}

	return 0;
}
LRESULT CSolitaireView::OnHelpHint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#ifdef DEMO
	CDemoDlg dlg;
	dlg.DoModal();
#endif
	return 0;
}


LRESULT CSolitaireView::OnHelpShuffle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#ifdef DEMO
	CDemoDlg dlg;
	dlg.DoModal();
#endif
	return 0;
}


LRESULT CSolitaireView::OnLayoutLoad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CLayoutsDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		CString strMessage = g_LangManager.GetString("str_new_game");

		if (MessageBox(strMessage, _T("Amazing Mahjongg 3D"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			CString strPathName = dlg.GetLayoutPath();

			if (strPathName == _T("*default*") || !m_objGame.LoadLayout(strPathName))
			{
				m_objGame.LoadMJLayout(IDRL_TRADITIONAL);
			}

			m_objGame.New();

			g_AppSettings.m_strLayoutsFolder = strPathName;
			g_AppSettings.m_strLayout = strPathName;

			UpdateView();
			UpdateUI();
			UpdateStatus();
		}
	}

	return 0;
}


LRESULT CSolitaireView::OnLayoutDefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strMessage = g_LangManager.GetString("str_new_game");

	if (MessageBox(strMessage, _T("Amazing Mahjongg 3D"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		m_objGame.LoadMJLayout(IDRL_TRADITIONAL);

		m_objGame.New();

		g_AppSettings.m_strLayout = _T("default");

		UpdateView();
		UpdateUI();
		UpdateStatus();
	}

	return 0;
}


LRESULT CSolitaireView::OnBackgroundColor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CColorDialog dlg;

	dlg.m_cc.lpCustColors[0] = RGB(0, 0, 64);

	if (dlg.DoModal() == IDOK)
	{
		m_vBackground[0] = glCOLOR(GetRValue(dlg.m_cc.rgbResult));
		m_vBackground[1] = glCOLOR(GetGValue(dlg.m_cc.rgbResult));
		m_vBackground[2] = glCOLOR(GetBValue(dlg.m_cc.rgbResult));

		m_enmBackgroundMode = eBGSolid;
		g_AppSettings.m_bBitmapBack = false;
		g_AppSettings.m_rgbBackground = dlg.m_cc.rgbResult;

		UpdateView();
	}

	return 0;
}


LRESULT CSolitaireView::OnBackgroundLoad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strFilter = g_LangManager.GetString("str_image_filter");

	for (int i = 0; i < strFilter.GetLength(); i++)
	{
		if (strFilter.GetAt(i) == '\n')
			strFilter.SetAt(i, '\0');
	}

	CFileDialog dlg(TRUE, NULL, NULL,

									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									strFilter,
									m_hWnd);

	//CString strInitialDir = MakeUserPath(g_AppSettings.m_strPicturesFolder);
	//dlg.m_ofn.lpstrInitialDir = strInitialDir;
	CString strInitialDir = MakeFullPath(_T("\\Background Images"));
	dlg.m_ofn.lpstrInitialDir = strInitialDir;


	if (dlg.DoModal() == IDOK)
	{
		//if(m_imgBackground.Load(dlg.m_szFileName, ATL::CImage_FORMAT_JPG))
		//{
		//  ShowBackgroundImage();

		//  g_AppSettings.m_bBitmapBack = true;
		//  g_AppSettings.m_strBackground = dlg.m_szFileName;

		//  UpdateView();
		//}
	}

	return 0;
}


LRESULT CSolitaireView::OnBackgroundDefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HRSRC hRes = FindResource(_Module.m_hInstResource, MAKEINTRESOURCE(IDR_BACKGROUND), _T("JPEG"));
	//if(m_imgBackground.LoadResource(hRes, ATL::CImage_FORMAT_JPG))
	//{
	//  ShowBackgroundImage();

	//  g_AppSettings.m_bBitmapBack = true;
	//  g_AppSettings.m_strBackground = _T("default");

	//  UpdateView();
	//}
	return 0;
}


void CSolitaireView::ShowBackgroundImage()
{
	unsigned int nTexWidth = 512;
	unsigned int nTexHeight = 512;

	if (g_AppSettings.m_bHighQualityBackground)
	{
		nTexWidth = 1024;
		nTexHeight = 1024;
	}

	glBindTexture(GL_TEXTURE_2D, m_unBackgroundTexName);

	BYTE* pBits = new BYTE[3 * nTexHeight * nTexWidth];

	gluScaleImage(GL_RGB, m_imgBackground.GetWidth(), m_imgBackground.GetHeight(),
								GL_UNSIGNED_BYTE, m_imgBackground.GetBits(),
								nTexWidth, nTexHeight, GL_UNSIGNED_BYTE, pBits);

	if (g_AppSettings.m_bHighQualityBackground)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nTexWidth, nTexHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBits);

	delete pBits;

	m_enmBackgroundMode = eBGImage;
}


void CSolitaireView::OnTileClick(UINT i, UINT j, UINT k)
{
	CSolitaireGame::CLICK_RESULT enmResult = m_objGame.ClickTile(i, j, k);

	if (g_AppSettings.m_bTextHints)
	{
		CBeginnerHintDlg dlg;

		switch (enmResult)
		{

		case CSolitaireGame::CR_TILES_NOT_EQUAL:
			dlg.m_strMessage = g_LangManager.GetString("str_tiles_not_equal");
			dlg.DoModal();
			break;

		case CSolitaireGame::CR_TILE_BLOCKED:
			dlg.m_strMessage = g_LangManager.GetString("str_tile_blocked");
			dlg.DoModal();
			break;
		}
	}

	if (m_objGame.GetState() == CSolitaireGame::GS_GAMEWON)
	{
		GameOver();
	}

	if (m_objGame.GetState() == CSolitaireGame::GS_GAMEOVER)
	{
	}
}
void CSolitaireView::UpdateStatus()
{
	if (m_pContainer->m_hWndStatusBar == NULL)
		return;

	//
	CString strTime;

	CString strStatus;

	CString strTilesLeft;

	// tiles left
	strTilesLeft.Format(g_LangManager.GetString("str_tiles_left"), m_objGame.GetBoard().CountTiles());

	// time
	if (m_objGame.m_objScores.m_tmPlayed.GetDays() > 0)
	{
		strTime.Format(g_LangManager.GetString("str_game_time_days"), m_objGame.m_objScores.m_tmPlayed.GetDays(), m_objGame.m_objScores.m_tmPlayed.GetHours(), m_objGame.m_objScores.m_tmPlayed.GetMinutes(), m_objGame.m_objScores.m_tmPlayed.GetSeconds());
	}
	else if (m_objGame.m_objScores.m_tmPlayed.GetHours() > 0)
	{
		strTime.Format(g_LangManager.GetString("str_game_time_hours"), m_objGame.m_objScores.m_tmPlayed.GetHours(), m_objGame.m_objScores.m_tmPlayed.GetMinutes(), m_objGame.m_objScores.m_tmPlayed.GetSeconds());
	}
	else
	{
		strTime.Format(g_LangManager.GetString("str_game_time"), m_objGame.m_objScores.m_tmPlayed.GetMinutes(), m_objGame.m_objScores.m_tmPlayed.GetSeconds());
	}

	// status
	strStatus.Format(g_LangManager.GetString("str_status"),
									 m_objTileset.GetName().GetLength() != 0 ? m_objTileset.GetName() : g_LangManager.GetString("str_unknown"),
									 m_objGame.GetLayout().GetName());

	switch (m_enmStatusMode)
	{

	case eNothing:
		break;

	case eTileName:

		if (!m_objGame.m_posCurrent.IsClear())
		{
			int nTile = m_objGame.m_objBoard.GetTile(m_objGame.m_posCurrent);
			strStatus = g_LangManager.GetString(m_arrTileName[nTile]);
			break;
		}

		break;

	case eHint:
	{
		if (!m_objGame.m_posCurrent.IsClear())
		{
			if (!m_objGame.m_objBoard.IsFree(m_objGame.m_posCurrent))
			{
				strStatus = g_LangManager.GetString("str_tile_is_blocked");
			}
			else
			{
				if (m_vecHint.GetSize() == 0)
				{
					strStatus = g_LangManager.GetString("str_no_matching_tiles");
				}
				else if (m_vecHint.GetSize() == 2)
				{
					strStatus = g_LangManager.GetString("str_1_matching_tile");
				}
				else
				{
					strStatus.Format(g_LangManager.GetString("str_n_matching_tiles"), m_vecHint.GetSize() - 1);
				}
			}
		}
	}

	break;

	default:
		ATLASSERT(false);
	}

	//
	::SendMessage(m_pContainer->m_hWndStatusBar, SB_SETTEXT, 0, (LPARAM)(LPCTSTR)strTilesLeft);

	::SendMessage(m_pContainer->m_hWndStatusBar, SB_SETTEXT, 1, (LPARAM)(LPCTSTR)strTime);

	::SendMessage(m_pContainer->m_hWndStatusBar, SB_SETTEXT, 2, (LPARAM)(LPCTSTR)strStatus);
}


void CSolitaireView::OnUpdateOptions()
{
	TParent::OnUpdateOptions();

	ReloadImages();

	// change detail level

	switch (g_AppSettings.m_nGraphDetail)
	{

	case eDetailLow:
	{
		// low details
		glShadeModel(GL_FLAT);

		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

		glDisable(GL_DITHER);

		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);

		LoadTileTextures();

		//CreateLQTileList();
	}

	break;

	case eDetailMedium:
	{
		// medium details
		glShadeModel(GL_SMOOTH);

		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

		glDisable(GL_DITHER);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);

		LoadTileTextures();

		//CreateMQTileList();
	}

	break;

	case eDetailHigh:
	{
		// high details
		glShadeModel(GL_SMOOTH);

		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.095f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

		glEnable(GL_DITHER);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		LoadTileTextures();

		//CreateHQTileList();
	}

	break;
	}

	// redraw background if need
	if (g_AppSettings.m_bBitmapBack)
	{
		ShowBackgroundImage();
	}

	m_nGraphDetail = g_AppSettings.m_nGraphDetail;

	UpdateUI();
	UpdateView();
}

LRESULT CSolitaireView::OnViewZoomIn(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_fRadius -= 0.5f;

	if (m_fRadius < 8.0f)
		m_fRadius = 8.0f;

	UpdateView();

	return 0;
}


LRESULT CSolitaireView::OnViewZoomOut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_fRadius += 0.5f;

	if (m_fRadius > 20.0f)
		m_fRadius = 20.0f;

	UpdateView();

	return 0;
}


LRESULT CSolitaireView::OnViewRotateLeft(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_fAngleZ -= 1.0f;

	if (m_fAngleZ < -180.0f) m_fAngleZ = 180.0f - ( -180 - m_fAngleZ);

	UpdateView();

	return 0;
}


LRESULT CSolitaireView::OnViewRotateRight(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_fAngleZ += 1.0f;

	if (m_fAngleZ > 180.0f) m_fAngleZ = -180.0f - ( 180 - m_fAngleZ);

	UpdateView();

	return 0;
}


LRESULT CSolitaireView::OnViewRotateUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_fAngleX -= 1.0f;

	if (m_fAngleX < -90.0f) m_fAngleX = -90.0f;

	UpdateView();

	return 0;
}
LRESULT CSolitaireView::OnViewRotateDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_fAngleX += 1.0f;

	if (m_fAngleX > 0.0f) m_fAngleX = 0.0f;

	UpdateView();

	return 0;
}


LRESULT CSolitaireView::OnViewDefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_fAngleX = -20.0f;
	m_fAngleY = 0.0f;
	m_fAngleZ = 0.0f;

	m_fRadius = 11.0f;

	UpdateView();

	return 0;
}

#ifdef _DEBUG
/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CSolitaireView::OnDebugGameOver(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GameOver();

	return 0;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CSolitaireView::OnDebugGameWon(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GameWon();

	return 0;
}

#endif

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CSolitaireView::GameOver()
{
	// no more moves after this click
	m_objGame.SetState(CSolitaireGame::GS_GAMEOVER);

	Sound(IDR_NOMOVES);

	//
	m_objScores.GameFinished(m_objGame.GetLayout().GetName(), false, m_objGame.GetPlayTime());
	m_objScores.Save(MakeUserPath(_T("")) + _T("scores.dat"));

	UpdateView();
	UpdateUI();
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
void CSolitaireView::GameWon()
{
	// game is over after this click
	m_objGame.SetState(CSolitaireGame::GS_GAMEWON);

	Sound(IDR_WOW);

	glDisable(GL_LIGHTING);

	m_enmDrawMode = eFireWorks;

	m_objFireWorks.Init();

	m_objFireWorks.Step();

	UpdateView();
	UpdateUI();

	m_pContainer->StartFireworks();
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
LRESULT CSolitaireView::OnStopFireworks(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pContainer->StopFireworks();

	if (g_AppSettings.m_strPlayerName == _T("Player"))
	{
		CPlayerNameDlg dlgPlayer;
		dlgPlayer.DoModal();
	}

	m_objGame.m_objScores.Set(m_objGame.GetLayout().GetName(), g_AppSettings.m_strPlayerName);

	m_objScores.AddRecord(m_objGame.m_objScores);

	m_objScores.GameFinished(m_objGame.GetLayout().GetName(), true, m_objGame.GetPlayTime());
	m_objScores.Save(MakeUserPath(_T("")) + _T("scores.dat"));

	CScoresDlg dlgScores;
	dlgScores.m_pScoreTable = &m_objScores;
	dlgScores.m_strLayout = m_objGame.GetLayout().GetName();

	dlgScores.DoModal();

	m_enmDrawMode = eMahjongg3D;

	m_objGame.New();

	glEnable(GL_LIGHTING);

	UpdateView();

	return 0;
}

bool CSolitaireView::ReloadImages()
{
	CString strLanguage = g_LangManager.GetLanguage();
	HRSRC hRsrc = NULL;

	if (strLanguage == _T("English"))
	{
		hRsrc = ::FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_GAME_OVER_EN), _T("JPEG"));
	}
	else if (strLanguage == _T("German"))
	{
		hRsrc = ::FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_GAME_OVER_DE), _T("JPEG"));
	}
	else
	{
		hRsrc = ::FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_GAME_OVER_EN), _T("JPEG"));
	}

	if (hRsrc != NULL)
	{
		//_VERIFY(m_imgGameOver.LoadResource(hRsrc, ATL::CImage_FORMAT_JPG));
	}

	return true;
}

/**
 *
 */
LRESULT CSolitaireView::OnMusicDefault(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return 0;
}

/**
 *
 */
LRESULT CSolitaireView::OnMusicLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString strFilter = g_LangManager.GetString("str_music_filter");

	for (int i = 0; i < strFilter.GetLength(); i++)
	{
		if (strFilter.GetAt(i) == '\n')
			strFilter.SetAt(i, '\0');
	}

	CFileDialog dlg(TRUE, _T("xm"), NULL,

									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									strFilter,
									m_hWnd);

	CString strInitialDir = MakeFullPath(_T("\\Music"));
	dlg.m_ofn.lpstrInitialDir = strInitialDir;

	if (dlg.DoModal() == IDOK)
	{
	}

	UpdateUI();

	return 0;
}

/**
 *
 */
LRESULT CSolitaireView::OnMusicStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	g_AppSettings.m_strMusic = _T("");

	m_enmMusic = eNoMusic;

	UpdateUI();

	return 0;
}

