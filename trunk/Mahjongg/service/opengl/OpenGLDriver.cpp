/*=================================================================================================
*
*  Copyright (c) 1998-2003 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg CE
*  $Workfile: OpenGLDriver.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 13.03.04 14:48 $
*
*================================================================================================*/

#include "stdafx.h"
#include "OpenGLDriver.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef DEBUG
#define CHECK_GL(expr) \
{ \
  expr; \
  GLenum enmGLError = (*m_pfnglGetError)(); \
  if(enmGLError != GL_NO_ERROR) \
  { \
  _TRACE(_T("OpenGL error while calling %s: %s\r\n"), #expr, m_mapErrorNames.Lookup(enmGLError)); \
  _ASSERT(FALSE);\
  } \
}
#else
#define CHECK_GL(expr) (expr)
#endif

#define WGL_SAMPLE_BUFFERS_ARB  0x2041
#define WGL_SAMPLES_ARB         0x2042

///////////////////////////////////////////////////////////////////////////////////////////////////
// constructor
COpenGLDriver::COpenGLDriver()
{
	m_hOpenGL = NULL;
	m_hGLU = NULL;

	m_pfnChoosePixelFormat = NULL;
	m_pfnSetPixelFormat = NULL;
	m_pfnwglCreateContext = NULL;
	m_pfnwglMakeCurrent = NULL;
	m_pfnglClearDepth = NULL;
	m_pfnglDepthFunc = NULL;
	m_pfnglEnable = NULL;
	m_pfnglDisable = NULL;
	m_pfnSwapBuffers = NULL;
	m_pfnwglGetCurrentDC = NULL;
	m_pfnglTranslatef = NULL;
	m_pfnglRotatef = NULL;
	m_pfnglLoadName = NULL;
	m_pfnglCallList = NULL;
	m_pfnglFinish = NULL;
	m_pfnglNewList = NULL;
	m_pfnglBegin = NULL;
	m_pfnglNormal3f = NULL;
	m_pfnglVertex3f = NULL;
	m_pfnglEnd = NULL;
	m_pfnglEndList = NULL;
	m_pfnglTexCoord2f = NULL;
	m_pfnglMaterialfv = NULL;
	m_pfnglLightfv = NULL;
	m_pfnglLightf = NULL;
	m_pfnglMatrixMode = NULL;
	m_pfnglGenTextures = NULL;
	m_pfnglBindTexture = NULL;
	m_pfnglTexParameterf = NULL;
	m_pfnglLightModelfv = NULL;
	m_pfnglLoadIdentity = NULL;
	m_pfnglGetIntegerv = NULL;
	m_pfnglSelectBuffer = NULL;
	m_pfnglPushName = NULL;
	m_pfnglPushMatrix = NULL;
	m_pfnglPopMatrix = NULL;
	m_pfnglFlush = NULL;
	m_pfngluPerspective = NULL;
	m_pfngluPickMatrix = NULL;
	m_pfnglViewport = NULL;
	m_pfnglInitNames = NULL;
	m_pfnglClear = NULL;
	m_pfnglPopName = NULL;
	m_pfnglGenLists = NULL;
	m_pfnglDeleteLists = NULL;
	m_pfnwglUseFontBitmaps = NULL;
	m_pfnwglUseFontOutlines = NULL;
	m_pfnglPixelStorei = NULL;
	m_pfnglTexEnvf = NULL;
	m_pfnglGetError = NULL;
	m_pfnglTexImage2D = NULL;
	m_pfngluScaleImage = NULL;
	m_pfnglShadeModel = NULL;
	m_pfnglRasterPos2f = NULL;
	m_pfnglRasterPos2i = NULL;
	m_pfnglDrawPixels = NULL;
	m_pfnglListBase = NULL;
	m_pfnglCallLists = NULL;
	m_pfnglScalef = NULL;
	m_pfnglColor3ub = NULL;
	m_pfnglHint = NULL;
	m_pfnglLineWidth = NULL;
	m_pfnwglGetProcAddress = NULL;
	m_pfnglGetString = NULL;
	m_pfnglOrtho = NULL;
	m_pfnglPointSize = NULL;
	m_pfnglBlendFunc = NULL;
	m_pfnglStencilFunc = NULL;

#ifdef _DEBUG
	m_mapErrorNames.SetAt(GL_INVALID_ENUM, _T("An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag."));
	m_mapErrorNames.SetAt(GL_INVALID_VALUE, _T("A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag."));
	m_mapErrorNames.SetAt(GL_INVALID_OPERATION, _T("The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag."));
	m_mapErrorNames.SetAt(GL_STACK_OVERFLOW, _T("This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag."));
	m_mapErrorNames.SetAt(GL_STACK_UNDERFLOW, _T("This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag."));
	m_mapErrorNames.SetAt(GL_OUT_OF_MEMORY, _T("There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded."));
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// destructor
COpenGLDriver::~COpenGLDriver()
{
	if (m_hOpenGL != NULL)
		::FreeLibrary(m_hOpenGL);

	if (m_hGLU != NULL)
		::FreeLibrary(m_hGLU);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize
bool COpenGLDriver::Initialize()
{
	QPROFILE_FUN(_T("COpenGLDriver::Initialize"));

	m_hOpenGL = LoadLibrary(_T("opengl.dll"));

	if (m_hOpenGL == NULL)
		return false;

	m_hGLU = LoadLibrary(_T("glu.dll"));

	if (m_hGLU == NULL)
		return false;

	// set up functions
	m_pfnwglGetProcAddress = (PFnwglGetProcAddress)::GetProcAddress(m_hOpenGL, "wglGetProcAddress");

	m_pfnChoosePixelFormat = (PFnChoosePixelFormat)::GetProcAddress(m_hOpenGL, "wglChoosePixelFormat");

	m_pfnSetPixelFormat = (PFnSetPixelFormat)::GetProcAddress(m_hOpenGL, "wglSetPixelFormat");

	m_pfnwglCreateContext = (PFnwglCreateContext)::GetProcAddress(m_hOpenGL, "wglCreateContext");

	m_pfnwglMakeCurrent = (PFnwglMakeCurrent)::GetProcAddress(m_hOpenGL, "wglMakeCurrent");

	InitFunctions();

	return true;
}

/**
*
*/
bool COpenGLDriver::IsExtensionAvailable(LPCTSTR pszExtension)
{
	const size_t extlen = lstrlen(pszExtension);
	CString strSupported;

	// попытка использовать wglGetExtensionStringARB на текущем контексте, если возможно
	PFnwglGetExtensionsStringARB pwglGetExtString = (PFnwglGetExtensionsStringARB)GetProcAddress("wglGetExtensionsStringARB");

	if (pwglGetExtString)
		strSupported = (*pwglGetExtString)((*m_pfnwglGetCurrentDC)());

	// Если  проверка не пройдена, то попытаемся использовать стандартное расширение OpenGL
	if (strSupported.IsEmpty())
		strSupported = GetString(GL_EXTENSIONS);

	// Если и это не поддерживается, тогда работаем без расширений
	if (strSupported.IsEmpty())
		return false;

	// Начинаем проверку с начала строки, увеличиваем на 1, при false совпадение
	for (int i = 0; i < strSupported.GetLength(); i++)
	{
		// Продвигаем p до следующего возможного совпадения
		i = strSupported.Find(pszExtension);

		if (i == -1)
			return false; // Совпадения нет

		//Убедимся, что есть совпадение в начале строки,
		//или первый символ – пробел, или может быть случайное
		//совпадение "wglFunkywglExtension" с "wglExtension"

		// Также убедимся, что текущий символ пустой или пробел
		// или еще "wglExtensionTwo" может совпасть с "wglExtension"
	}

	return false;
}

bool COpenGLDriver::IsExtensionAvailable(GL_EXTENSION enExtension)
{
	return m_mapExtensions.Lookup(enExtension);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
HGLRC COpenGLDriver::Attach(HDC hDC)
{
	//
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	/* size of this pfd */
		1,				/* version num */
		PFD_DRAW_TO_WINDOW |		/* support window */
		PFD_SUPPORT_OPENGL |		/* support OpenGL */
		PFD_DOUBLEBUFFER,		/* support double buffering */
		PFD_TYPE_RGBA,			/* RGBA type */
		24,				/* 8-bit color depth */
		0, 0, 0, 0, 0, 0,		/* color bits (ignored) */
		0,				/* no alpha buffer */
		0,				/* alpha bits (ignored) */
		0,				/* no accumulation buffer */
		0, 0, 0, 0,			/* accum bits (ignored) */
		16,				/* 16-bit depth buffer */
		0,				/* no stencil buffer */
		0,				/* no auxiliary buffers */
		PFD_MAIN_PLANE,			/* main layer */
		0,				/* reserved */
		0, 0, 0,			/* no layer, visible, damage masks */
	};

	int SelectedPixelFormat = (*m_pfnChoosePixelFormat)(hDC, &pfd);

	if (SelectedPixelFormat == 0)
	{
		_TRACE(_T("OpenGL error: ChoosePixelFormat is failed\r\n"));
		return NULL;
	}

	BOOL retVal = (*m_pfnSetPixelFormat)(hDC, SelectedPixelFormat, &pfd);

	if (retVal != TRUE)
	{
		_TRACE(_T("OpenGL error: SetPixelFormat is failed\r\n"));
		return NULL;
	}

	HGLRC hRC = (*m_pfnwglCreateContext)(hDC);

	if (hRC == NULL)
	{
		_TRACE(_T("OpenGL error: CreateContext is failed\r\n"));
		return NULL;
	}

	(*m_pfnwglMakeCurrent)(hDC, hRC);

	InitExtensions(hDC);

	//

	if (IsExtensionAvailable(GL_EXT_WGL_ARB_multisample))
	{
		_TRACE(_T("OpenGL info: multisampling is enabled\r\n"));

		int iAttributes[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, // Истинна, если формат пикселя может быть использован в окне
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE, // Истинна, если поддерживается OpenGL
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, // Полная аппаратная поддержка
			WGL_COLOR_BITS_ARB, 24,         // Цветность
			WGL_ALPHA_BITS_ARB, 8,          // Размерность альфа-канала
			WGL_DEPTH_BITS_ARB, 16,         // Глубина буфера глубины
			WGL_STENCIL_BITS_ARB, 0,        // Глубина буфера шаблона
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE, // Истина, если используется двойная буферизация
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // Что мы и хотим
			WGL_SAMPLES_ARB, 4 ,            // проверка на 4x тип
			0, 0
		};
		float fAttributes[] = {0, 0};
		int pixelFormat;
		UINT numFormats;

		BOOL valid = (*m_pwglChoosePixelFormatARB)(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);


	}

	// extensions
	m_strVendor = GetString(GL_VENDOR);

	m_strRenderer = GetString(GL_RENDERER);

	m_strVersion = GetString(GL_VERSION);

	_TRACE(_T("%s %s %s\r\n"), m_strVendor, m_strRenderer, m_strVersion);

	return hRC;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::InitFunctions()
{
	QPROFILE_FUN(_T("COpenGLDriver::InitFunctions"));

	m_pfnwglGetCurrentDC = (PFnwglGetCurrentDC)::GetProcAddress(m_hOpenGL, "wglGetCurrentDC");
	m_pfnglClearDepth = (PFnglClearDepth)::GetProcAddress(m_hOpenGL, "glClearDepth");
	m_pfnglDepthFunc = (PFnglDepthFunc)::GetProcAddress(m_hOpenGL, "glDepthFunc");

	m_pfnglEnable = (PFnglEnable)::GetProcAddress(m_hOpenGL, "glEnable");
	m_pfnglDisable = (PFnglEnable)::GetProcAddress(m_hOpenGL, "glDisable");

	m_pfnSwapBuffers = (PFnSwapBuffers)::GetProcAddress(m_hOpenGL, "wglSwapBuffers");
	m_pfnglTranslatef = (PFnglTranslatef)::GetProcAddress(m_hOpenGL, "glTranslatef");
	m_pfnglRotatef = (PFnglRotatef)::GetProcAddress(m_hOpenGL, "glRotatef");
	m_pfnglLoadName = (PFnglLoadName)::GetProcAddress(m_hOpenGL, "glLoadName");
	m_pfnglCallList = (PFnglCallList)::GetProcAddress(m_hOpenGL, "glCallList");
	m_pfnglFinish = (PFnglFinish)::GetProcAddress(m_hOpenGL, "glFinish");
	m_pfnglNewList = (PFnglNewList)::GetProcAddress(m_hOpenGL, "glNewList");
	m_pfnglBegin = (PFnglBegin)::GetProcAddress(m_hOpenGL, "glBegin");
	m_pfnglNormal3f = (PFnglNormal3f)::GetProcAddress(m_hOpenGL, "glNormal3f");
	m_pfnglVertex3f = (PFnglVertex3f)::GetProcAddress(m_hOpenGL, "glVertex3f");
	m_pfnglEnd = (PFnglEnd)::GetProcAddress(m_hOpenGL, "glEnd");
	m_pfnglEndList = (PFnglEndList)::GetProcAddress(m_hOpenGL, "glEndList");
	m_pfnglTexCoord2f = (PFnglTexCoord2f)::GetProcAddress(m_hOpenGL, "glTexCoord2f");
	m_pfnglMaterialfv = (PFnglMaterialfv)::GetProcAddress(m_hOpenGL, "glMaterialfv");
	m_pfnglLightfv = (PFnglLightfv)::GetProcAddress(m_hOpenGL, "glLightfv");
	m_pfnglLightf = (PFnglLightf)::GetProcAddress(m_hOpenGL, "glLightf");
	m_pfnglMatrixMode = (PFnglMatrixMode)::GetProcAddress(m_hOpenGL, "glMatrixMode");
	m_pfnglGenTextures = (PFnglGenTextures)::GetProcAddress(m_hOpenGL, "glGenTextures");
	m_pfnglBindTexture = (PFnglBindTexture)::GetProcAddress(m_hOpenGL, "glBindTexture");
	//m_pfngluBuild2DMipmaps = (PFngluBuild2DMipmaps)::::GetProcAddress(m_hOpenGL, m_hGLU, "gluBuild2DMipmaps");
	m_pfnglTexParameterf = (PFnglTexParameterf)::GetProcAddress(m_hOpenGL, "glTexParameterf");
	m_pfnglLightModelfv = (PFnglLightModelfv)::GetProcAddress(m_hOpenGL, "glLightModelfv");
	m_pfnglLoadIdentity = (PFnglLoadIdentity)::GetProcAddress(m_hOpenGL, "glLoadIdentity");
	m_pfnglGetIntegerv = (PFnglGetIntegerv)::GetProcAddress(m_hOpenGL, "glGetIntegerv");
	m_pfnglSelectBuffer = (PFnglSelectBuffer)::GetProcAddress(m_hOpenGL, "glSelectBuffer");
	m_pfnglPushName = (PFnglPushName)::GetProcAddress(m_hOpenGL, "glPushName");
	m_pfnglPushMatrix = (PFnglPushMatrix)::GetProcAddress(m_hOpenGL, "glPushMatrix");
	m_pfnglPopMatrix = (PFnglPopMatrix)::GetProcAddress(m_hOpenGL, "glPopMatrix");
	m_pfnglFlush = (PFnglFlush)::GetProcAddress(m_hOpenGL, "glFlush");
	m_pfngluPerspective = (PFngluPerspective)::GetProcAddress(m_hGLU, "gluPerspective");
	m_pfngluPickMatrix = (PFngluPickMatrix)::GetProcAddress(m_hGLU, "gluPickMatrix");
	m_pfnglViewport = (PFnglViewport)::GetProcAddress(m_hOpenGL, "glViewport");
	m_pfnglRenderMode = (PFnglRenderMode)::GetProcAddress(m_hOpenGL, "glRenderMode");
	m_pfnglInitNames = (PFnglInitNames)::GetProcAddress(m_hOpenGL, "glInitNames");
	m_pfnglClear = (PFnglClear)::GetProcAddress(m_hOpenGL, "glClear");
	m_pfnglPopName = (PFnglPopName)::GetProcAddress(m_hOpenGL, "glPopName");
	m_pfnglGenLists = (PFnglGenLists)::GetProcAddress(m_hOpenGL, "glGenLists");
	m_pfnglDeleteLists = (PFnglDeleteLists)::GetProcAddress(m_hOpenGL, "glDeleteLists");
	m_pfnwglUseFontBitmaps = (PFnwglUseFontBitmaps)::GetProcAddress(m_hOpenGL, "wglUseFontBitmapsA");
	m_pfnwglUseFontOutlines = (PFnwglUseFontOutlines)::GetProcAddress(m_hOpenGL, "wglUseFontOutlinesA");
	m_pfnglPixelStorei = (PFnglPixelStorei)::GetProcAddress(m_hOpenGL, "glPixelStorei");
	m_pfnglTexEnvf = (PFnglTexEnvf)::GetProcAddress(m_hOpenGL, "glTexEnvf");
	m_pfnglGetError = (PFnglGetError)::GetProcAddress(m_hOpenGL, "glGetError");
	m_pfnglTexImage2D = (PFnglTexImage2D)::GetProcAddress(m_hOpenGL, "glTexImage2D");
	m_pfngluScaleImage = (PFngluScaleImage)::GetProcAddress(m_hGLU, "gluScaleImage");
	m_pfnglShadeModel = (PFnglShadeModel)::GetProcAddress(m_hOpenGL, "glShadeModel");
	m_pfnglRasterPos2f = (PFnglRasterPos2f)::GetProcAddress(m_hOpenGL, "glRasterPos2f");
	m_pfnglRasterPos2i = (PFnglRasterPos2i)::GetProcAddress(m_hOpenGL, "glRasterPos2i");
	m_pfnglDrawPixels = (PFnglDrawPixels)::GetProcAddress(m_hOpenGL, "glDrawPixels");
	m_pfnglListBase = (PFnglListBase)::GetProcAddress(m_hOpenGL, "glListBase");
	m_pfnglCallLists = (PFnglCallLists)::GetProcAddress(m_hOpenGL, "glCallLists");
	m_pfnglScalef = (PFnglScalef)::GetProcAddress(m_hOpenGL, "glScalef");
	m_pfnglColor3ub = (PFnglColor3ub)::GetProcAddress(m_hOpenGL, "glColor3ub");
	m_pfnglHint = (PFnglHint)::GetProcAddress(m_hOpenGL, "glHint");
	m_pfnglLineWidth = (PFnglLineWidth)::GetProcAddress(m_hOpenGL, "glLineWidth");
	m_pfnglGetString = (PFnglGetString)::GetProcAddress(m_hOpenGL, "glGetString");
	m_pfnglOrtho = (PFnglOrtho)::GetProcAddress(m_hOpenGL, "glOrtho");
	m_pfnglPointSize = (PFnglPointSize)::GetProcAddress(m_hOpenGL, "glPointSize");
	m_pfnglBlendFunc = (PFnglBlendFunc)::GetProcAddress(m_hOpenGL, "glBlendFunc");
	m_pfnglStencilFunc = (PFnglStencilFunc)::GetProcAddress(m_hOpenGL, "glStencilFunc");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::InitExtensions(HDC hDC)
{
	QPROFILE_FUN(_T("COpenGLDriver::InitExtensions"));

	CString strExtensions = GetString(GL_EXTENSIONS);

	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;
	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)GetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtensionsStringARB)
	{
		strExtensions += (*wglGetExtensionsStringARB)(hDC);
	}

	if (strExtensions.Find(_T("GL_ARB_multitexture")) != -1)
	{
		// set this extension enabled
		m_mapExtensions.SetAt(GL_ARB_multitexture, true);

		// load functions

	}

	if (strExtensions.Find(_T("GL_EXT_texture_env_combine")) != -1)
	{
		// set extension enabled
		m_mapExtensions.SetAt(GL_EXT_texture_env_combine, true);

		// load function
	}

	if (strExtensions.Find(_T("WGL_ARB_multisample")) != -1)
	{
		// set extension enabled
		//m_mapExtensions.SetAt(GL_EXT_WGL_ARB_multisample, true);

		//m_pwglChoosePixelFormatARB = (PFnwglChoosePixelFormatARB)::GetProcAddress(m_hOpenGL, "wglChoosePixelFormatARB");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void* COpenGLDriver::GetProcAddress(LPCSTR pszName)
{
	_ASSERT(m_pfnwglGetProcAddress != NULL);
	return ((*m_pfnwglGetProcAddress)(pszName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::ViewPort(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (m_pfnglViewport == NULL)
	{
		m_pfnglViewport = (PFnglViewport)GetProcAddress("glViewport");
		_ASSERT(m_pfnglViewport != NULL);
	}

	CHECK_GL((*m_pfnglViewport)(x, y, width, height));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::MatrixMode(GLenum enmMode)
{
	_ASSERT(m_pfnglMatrixMode != NULL);
	CHECK_GL((*m_pfnglMatrixMode)(enmMode));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::LoadIdentity()
{
	_ASSERT(m_pfnglLoadIdentity != NULL);
	CHECK_GL((*m_pfnglLoadIdentity)());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::Perspective(GLdouble fovy, GLdouble aspect, GLdouble znear, GLdouble zfar)
{
	_ASSERT(m_pfngluPerspective != NULL);
	CHECK_GL((*m_pfngluPerspective)(fovy, aspect, znear, zfar));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::GetInteger(GLenum pname, GLint *params)
{
	_ASSERT(m_pfnglGetIntegerv != NULL);
	CHECK_GL((*m_pfnglGetIntegerv)(pname, params));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::SelectBuffer(GLsizei size, GLuint *buffer)
{
	_ASSERT(m_pfnglSelectBuffer != NULL);
	CHECK_GL((*m_pfnglSelectBuffer)(size, buffer));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
GLint COpenGLDriver::RenderMode(GLenum mode)
{
	_ASSERT(m_pfnglRenderMode != NULL);
	return (*m_pfnglRenderMode)(mode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::InitNames()
{
	_ASSERT(m_pfnglInitNames != NULL);
	CHECK_GL((*m_pfnglInitNames)());
}

void COpenGLDriver::PushName(GLuint name)
{
	_ASSERT(m_pfnglPushName != NULL);
	CHECK_GL((*m_pfnglPushName)(name));
}

void COpenGLDriver::PushMatrix()
{
	_ASSERT(m_pfnglPushMatrix != NULL);
	CHECK_GL((*m_pfnglPushMatrix)());
}

void COpenGLDriver::PickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height, GLint viewport[])
{
	_ASSERT(m_pfngluPickMatrix != NULL);
	CHECK_GL((*m_pfngluPickMatrix)(x, y, width, height, viewport));
}

void COpenGLDriver::PopMatrix()
{
	_ASSERT(m_pfnglPopMatrix != NULL);
	CHECK_GL((*m_pfnglPopMatrix)());
}

void COpenGLDriver::Flush()
{
	_ASSERT(m_pfnglFlush != NULL);
	CHECK_GL((*m_pfnglFlush)());
}

void COpenGLDriver::ClearDepth(GLclampd depth)
{
	_ASSERT(m_pfnglClearDepth != NULL);
	CHECK_GL((*m_pfnglClearDepth)(depth));
}

void COpenGLDriver::DepthFunc(GLenum func)
{
	_ASSERT(m_pfnglDepthFunc != NULL);
	CHECK_GL((*m_pfnglDepthFunc)(func));
}

void COpenGLDriver::Enable(GLenum cap)
{
	_ASSERT(m_pfnglEnable != NULL);
	CHECK_GL((*m_pfnglEnable)(cap));
}

void COpenGLDriver::Light(GLenum light, GLenum pname, GLfloat param)
{
	_ASSERT(m_pfnglLightf != NULL);
	CHECK_GL((*m_pfnglLightf)(light, pname, param));
}

void COpenGLDriver::Light(GLenum light, GLenum pname, const GLfloat* params)
{
	_ASSERT(m_pfnglLightfv != NULL);
	CHECK_GL((*m_pfnglLightfv)(light, pname, params));
}

void COpenGLDriver::SwapBuffers(HDC hDC)
{
	_ASSERT(m_pfnSwapBuffers != NULL);
	(*m_pfnSwapBuffers)(hDC);
}

void COpenGLDriver::Clear(GLbitfield mask)
{
	_ASSERT(m_pfnglClear != NULL);
	CHECK_GL((*m_pfnglClear)(mask));
}

void COpenGLDriver::Normal(GLfloat x, GLfloat y, GLfloat z)
{
	_ASSERT(m_pfnglNormal3f != NULL);
	(*m_pfnglNormal3f)(x, y, z);
}

void COpenGLDriver::CallList(GLuint list)
{
	_ASSERT(m_pfnglCallList != NULL);
	CHECK_GL((*m_pfnglCallList)(list));
}

void COpenGLDriver::Material(GLenum face, GLenum pname, const GLfloat *params)
{
	_ASSERT(m_pfnglMaterialfv != NULL);
	CHECK_GL((*m_pfnglMaterialfv)(face, pname, params));
}

void COpenGLDriver::BindTexture(GLenum target, GLuint texture)
{
	_ASSERT(m_pfnglBindTexture != NULL);
	CHECK_GL((*m_pfnglBindTexture)(target, texture));
}

void COpenGLDriver::Disable(GLenum cap)
{
	_ASSERT(m_pfnglDisable != NULL);
	CHECK_GL((*m_pfnglDisable)(cap));
}

void COpenGLDriver::Translate(GLfloat x, GLfloat y, GLfloat z)
{
	_ASSERT(m_pfnglTranslatef != NULL);
	CHECK_GL((*m_pfnglTranslatef)(x, y, z));
}

void COpenGLDriver::Rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	_ASSERT(m_pfnglRotatef != NULL);
	CHECK_GL((*m_pfnglRotatef)(angle, x, y, z));
}

void COpenGLDriver::LoadName(GLuint name)
{
	_ASSERT(m_pfnglLoadName != NULL);
	CHECK_GL((*m_pfnglLoadName)(name));
}

void COpenGLDriver::PopName()
{
	_ASSERT(m_pfnglPopName != NULL);
	CHECK_GL((*m_pfnglPopName)());
}

void COpenGLDriver::Finish()
{
	_ASSERT(m_pfnglFinish != NULL);
	CHECK_GL((*m_pfnglFinish)());
}

void COpenGLDriver::NewList(GLuint list, GLenum mode)
{
	_ASSERT(m_pfnglNewList != NULL);
	CHECK_GL((*m_pfnglNewList)(list, mode));
}

void COpenGLDriver::Begin(GLenum mode)
{
	_ASSERT(m_pfnglBegin != NULL);
	(*m_pfnglBegin)(mode);
}

void COpenGLDriver::Vertex(GLfloat x, GLfloat y, GLfloat z)
{
	_ASSERT(m_pfnglVertex3f != NULL);
	(*m_pfnglVertex3f)(x, y, z);
}

void COpenGLDriver::End()
{
	_ASSERT(m_pfnglEnd != NULL);
	CHECK_GL((*m_pfnglEnd)());
}

void COpenGLDriver::EndList()
{
	_ASSERT(m_pfnglEndList != NULL);
	CHECK_GL((*m_pfnglEndList)());
}

void COpenGLDriver::TexCoord(GLfloat s, GLfloat t)
{
	_ASSERT(m_pfnglTexCoord2f != NULL);
	(*m_pfnglTexCoord2f)(s, t);
}

void COpenGLDriver::GenTextures(GLsizei n, GLuint *textures)
{
	_ASSERT(m_pfnglGenTextures != NULL);
	CHECK_GL((*m_pfnglGenTextures)(n, textures));
}

void COpenGLDriver::ScaleImage(GLenum format, GLint widthin, GLint heightin, GLenum typein, const void *datain, GLint widthout, GLint heightout, GLenum typeout, void *dataout)
{
	_ASSERT(m_pfngluScaleImage != NULL);
	CHECK_GL((*m_pfngluScaleImage)(format, widthin, heightin, typein, datain, widthout, heightout, typeout, dataout));
}

void COpenGLDriver::TexImage2D(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	_ASSERT(m_pfnglTexImage2D != NULL);
	CHECK_GL((*m_pfnglTexImage2D)(GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels));
}

void COpenGLDriver::LightModel(GLenum pname, const GLfloat *params)
{
	_ASSERT(m_pfnglLightModelfv != NULL);
	CHECK_GL((*m_pfnglLightModelfv)(pname, params));
}

void COpenGLDriver::ShadeModel(GLenum mode)
{
	_ASSERT(m_pfnglShadeModel != NULL);
	CHECK_GL((*m_pfnglShadeModel)(mode));
}

void COpenGLDriver::TexEnv(GLenum target, GLenum pname, GLfloat param)
{
	_ASSERT(m_pfnglTexEnvf != NULL);
	CHECK_GL((*m_pfnglTexEnvf)(target, pname, param));
}

void COpenGLDriver::TexParameter(GLenum target, GLenum pname, GLfloat param)
{
	_ASSERT(m_pfnglTexParameterf != NULL);
	CHECK_GL((*m_pfnglTexParameterf)(target, pname, param));
}

void COpenGLDriver::RasterPos(GLfloat x, GLfloat y)
{
	_ASSERT(m_pfnglRasterPos2f != NULL);
	CHECK_GL((*m_pfnglRasterPos2f)(x, y));
}

void COpenGLDriver::RasterPos(GLint x, GLint y)
{
	_ASSERT(m_pfnglRasterPos2i != NULL);
	CHECK_GL((*m_pfnglRasterPos2i)(x, y));
}

void COpenGLDriver::DrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
	_ASSERT(m_pfnglDrawPixels != NULL);
	CHECK_GL((*m_pfnglDrawPixels)(width, height, format, type, pixels));
}

bool COpenGLDriver::UseFontOutlines(HDC hDC, DWORD dwFirst, DWORD dwCount, DWORD dwListBase,
																		FLOAT fDeviation, FLOAT fExtrusion, int nFormat,
																		LPGLYPHMETRICSFLOAT  lpgmf)
{
	_ASSERT(m_pfnwglUseFontOutlines != NULL);
	return (*m_pfnwglUseFontOutlines)(hDC, dwFirst, dwCount, dwListBase, fDeviation, fExtrusion, nFormat, lpgmf) == TRUE;
}

void COpenGLDriver::ListBase(GLuint base)
{
	_ASSERT(m_pfnglListBase != NULL);
	CHECK_GL((*m_pfnglListBase)(base));
}

void COpenGLDriver::CallLists(GLsizei n, GLenum type, const GLvoid *lists)
{
	_ASSERT(m_pfnglCallLists != NULL);
	CHECK_GL((*m_pfnglCallLists)(n, type, lists));
}

void COpenGLDriver::Scale(GLfloat x, GLfloat y, GLfloat z)
{
	_ASSERT(m_pfnglScalef != NULL);
	CHECK_GL((*m_pfnglScalef)(x, y, z));
}

void COpenGLDriver::Color(GLubyte R, GLubyte G, GLubyte B)
{
	_ASSERT(m_pfnglColor3ub != NULL);
	CHECK_GL((*m_pfnglColor3ub)(R, G, B));
}

void COpenGLDriver::PolygonMode(GLenum face, GLenum mode)
{
	_ASSERT(FALSE);
}

void COpenGLDriver::Hint(GLenum target, GLenum mode)
{
	_ASSERT(m_pfnglHint != NULL);
	CHECK_GL((*m_pfnglHint)(target, mode));
}

void COpenGLDriver::BlendFunc(GLenum sfactor, GLenum dfactor)
{
	_ASSERT(m_pfnglBlendFunc != NULL);
	CHECK_GL((*m_pfnglBlendFunc)(sfactor, dfactor));
}

void COpenGLDriver::LineWidth(GLfloat width)
{
	_ASSERT(m_pfnglLineWidth != NULL);
	CHECK_GL((*m_pfnglLineWidth)(width));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
CString COpenGLDriver::GetString(GLenum enName)
{
	USES_CONVERSION;

	_ASSERT(m_pfnglGetString != NULL);
	const char* szResult = (const char*)(*m_pfnglGetString)(enName);

	if (szResult == NULL)
	{
#ifdef _DEBUG
		GLenum enmGLError = (*m_pfnglGetError)();

		if (enmGLError != GL_NO_ERROR)
		{
			_TRACE(_T("OpenGL error while calling %s: %s\r\n"), _T("GetString"), m_mapErrorNames.Lookup(enmGLError));
		}

#endif
		return _T("");
	}

	return CString(A2CT(szResult));
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	_ASSERT(m_pfnglOrtho != NULL);
	CHECK_GL((*m_pfnglOrtho)(left, right, bottom, top, zNear, zFar));
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::PointSize(GLfloat size)
{
	_ASSERT(m_pfnglPointSize != NULL);
	CHECK_GL((*m_pfnglPointSize)(size));
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
GLuint COpenGLDriver::GenLists(GLuint nRange)
{
	_ASSERT(m_pfnglGenLists != NULL);
	return (*m_pfnglGenLists)(nRange);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::DeleteLists(GLuint nBase, GLsizei nRange)
{
	_ASSERT(m_pfnglDeleteLists != NULL);
	CHECK_GL((*m_pfnglDeleteLists)(nBase, nRange));
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
void COpenGLDriver::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	_ASSERT(m_pfnglStencilFunc != NULL);
	CHECK_GL((*m_pfnglStencilFunc)(func, ref, mask));
}
