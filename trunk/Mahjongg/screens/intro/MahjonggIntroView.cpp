/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MahjonggIntroView.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#include "StdAfx.h"
/*
#include "mahjonggintroview.h"

CMahjonggIntroView::CMahjonggIntroView(CMainFrame* pContainer)
  : CMahjonggView(pContainer)
{
}

CMahjonggIntroView::~CMahjonggIntroView(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggIntroView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  UpdateUI();

  Init();

  //SetTimer(IDT_SECOND, 1000, NULL);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggIntroView::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  QPROFILE_FUN(_T("CMahjonggIntroView::OnPaint"));

  PAINTSTRUCT ps;
  BeginPaint(&ps);

  UpdateView();

  EndPaint(&ps);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggIntroView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  QPROFILE_FUN(_T("CMahjonggIntroView::OnSize"));

  int cx = LOWORD(lParam);
  int cy = HIWORD(lParam);

  m_objSize.cx = cx;
  m_objSize.cy = cy;

  if(m_bInitialized && cy > 0)
  {
    if((m_rectOld.right > cx) || (m_rectOld.bottom > cy))
      RedrawWindow();

    m_rectOld.right = cx;
    m_rectOld.bottom = cy;

    CHECK_GL(glViewport(0, 0, cx, cy));
    CHECK_GL(glMatrixMode(GL_PROJECTION));
    CHECK_GL(glLoadIdentity());
    (*pfngluPerspective)(45.0f, (GLfloat)cx/(GLfloat)cy, 1.0f, 80.0f);
    CHECK_GL(glMatrixMode(GL_MODELVIEW));
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggIntroView::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggIntroView::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggIntroView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggIntroView::OnGameNew(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMahjonggIntroView::OnGameDemo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggIntroView::SetupPixelFormat()
{
  static PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
      1,                              // version number
      PFD_DRAW_TO_WINDOW |            // support window
      PFD_SUPPORT_OPENGL |          // support OpenGL
      PFD_DOUBLEBUFFER,             // double buffered
      PFD_TYPE_RGBA,                  // RGBA type
      16,                             // 24-bit color depth
      0, 0, 0, 0, 0, 0,               // color bits ignored
      1,                              // no alpha buffer
      0,                              // shift bit ignored
      0,                              // no accumulation buffer
      0, 0, 0, 0,                     // accum bits ignored
      16,                             // 32-bit z-buffer
      0,                              // no stencil buffer
      0,                              // no auxiliary buffer
      PFD_MAIN_PLANE,                 // main layer
      0,                              // reserved
      0, 0, 0                         // layer masks ignored
  };

  int pixelformat;
  if((pixelformat = ChoosePixelFormat(m_hDC, &pfd)) == 0)
  {
    MessageBox("ChoosePixelFormat failed");
    return false;
  }

  if(SetPixelFormat(m_hDC, pixelformat, &pfd) == FALSE)
  {
    MessageBox("SetPixelFormat failed");
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggIntroView::Init()
{
  PIXELFORMATDESCRIPTOR pfd;
  int         n = 0;
  GLfloat     fAspect = 0.0f;
  GLfloat     fNearPlane = 0.0f, fFarPlane = 0.0f;

  m_hDC = ::GetDC(m_hWnd);
  _ASSERT(m_hDC != NULL);

  if (!SetupPixelFormat())
    return false;

  n = ::GetPixelFormat(m_hDC);
  ::DescribePixelFormat(m_hDC, n, sizeof(pfd), &pfd);

  m_hRC = wglCreateContext(m_hDC);
  wglMakeCurrent(m_hDC, m_hRC);

  // create command lists
  CreateLists();

  // load textures
  LoadTextures();
  //LoadFont();

  // set original viewpoint
  m_fAngleX = -20.0f;
  m_fAngleY = 0.0f;
  m_fAngleZ = 0.0f;

  m_fRadius = 13.0f;

  glFrontFace(GL_CCW);

  glClearDepth(1.0f);

  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_AUTO_NORMAL);

  glEnable(GL_LIGHTING);

  static GLfloat ambient[4] = {0.4f, 0.4f, 0.4f, 1.0f};
  static GLfloat diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat position[] = { -4.0f , 4.0f, -2.0f, 1.0f };

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.095f);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

  // enable lighting
  glEnable(GL_LIGHT0);
  glDisable(GL_LIGHT1);

  glShadeModel(GL_SMOOTH);

  m_vBackground[0] = 0.0f;
  m_vBackground[1] = 0.0f;
  m_vBackground[2] = 0.3f;
  m_vBackground[3] = 1.0f;

  m_bInitialized = true;

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggIntroView::LoadTextures()
{
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggIntroView::CreateLists()
{
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggIntroView::UpdateUI()
{
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CMahjonggIntroView::UpdateView()
{
  if(DrawScene(GL_RENDER))
    SwapBuffers(wglGetCurrentDC());

  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CMahjonggIntroView::DrawScene(GLenum enmMode)
{
  return true;
}
*/