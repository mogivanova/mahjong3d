/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: OpenGLDriver.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#pragma once

#include "gl.h"
#include "glu.h"
#include "gl_ext.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//

class COpenGLDriver
{

public:
	// constructor
	COpenGLDriver();
	// destructor
	~COpenGLDriver();

public:
	bool Initialize();

	CString GetVendor();
	CString GetRenderer();
	CString GetVersion();

	bool IsExtensionAvailable(LPCTSTR pszExtension);
	bool IsExtensionAvailable(GL_EXTENSION enExtension);

protected:
	void InitFunctions();
	void InitExtensions(HDC hDC);

public:
	void* GetProcAddress(LPCSTR pszName);

public:
	void DeleteLists(GLuint nBase, GLsizei nRange);
	GLuint GenLists(GLuint nRange);

	CString GetString(GLenum enName);
	void LineWidth(GLfloat width);
	void BlendFunc(GLenum sfactor, GLenum dfactor);
	void Hint(GLenum target, GLenum mode);
	void PolygonMode(GLenum face, GLenum mode);
	void Scale(GLfloat x, GLfloat y, GLfloat z);
	void CallLists(GLsizei n, GLenum type, const GLvoid* lists);
	void DrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);

	HGLRC Attach(HDC hDC);

	void GenTextures(GLsizei n, GLuint* textures);

	void Enable(GLenum cap);
	void Disable(GLenum cap);

	void LightModel(GLenum pname, const GLfloat* params);
	void ShadeModel(GLenum mode);

	void ScaleImage(GLenum format, GLint widthin, GLint heightin, GLenum typein, const void* datain, GLint widthout, GLint heightout, GLenum typeout, void* dataout);
	void TexEnv(GLenum target, GLenum pname, GLfloat param);

	void BindTexture(GLenum target, GLuint texture);
	void TexCoord(GLfloat s, GLfloat t);
	void TexImage2D(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
	void TexParameter(GLenum target, GLenum pname, GLfloat param);

	void NewList(GLuint list, GLenum mode);
	void EndList();
	void ListBase(GLuint base);

	void Begin(GLenum mode);
	void Vertex(GLfloat x, GLfloat y, GLfloat z);
	void Normal(GLfloat x, GLfloat y, GLfloat z);
	void End();

	void Color(GLubyte R, GLubyte G, GLubyte B);

	void Translate(GLdouble x, GLdouble y, GLdouble z);
	void Translate(GLfloat x, GLfloat y, GLfloat z);
	void Rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

	void Material(GLenum face, GLenum pname, const GLfloat* params);

	void CallList(GLuint list);

	void Clear(GLbitfield mask);

	void SwapBuffers(HDC hDC);

	void Light(GLenum light, GLenum pname, GLfloat param);
	void Light(GLenum light, GLenum pname, GLint param);
	void Light(GLenum light, GLenum pname, const GLfloat* params);
	void Light(GLenum light, GLenum pname, const GLint* params);

	void DepthFunc(GLenum func);

	void ClearDepth(GLclampd depth);

	void Flush();

	void Finish();

	void LoadIdentity();
	void PushMatrix();
	void PopMatrix();

	void Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

	void InitNames();
	void LoadName(GLuint name);
	void PushName(GLuint name);
	void PopName();

	void PickMatrix(GLdouble x, GLdouble y, GLdouble width, GLdouble height, GLint viewport[4]);

	GLint RenderMode(GLenum mode);

	void SelectBuffer(GLsizei size, GLuint* buffer);

	void GetInteger(GLenum pname, GLint* params);

	void Perspective(GLdouble fovy, GLdouble aspect, GLdouble znear, GLdouble zfar);

	void MatrixMode(GLenum enmMode);

	void ViewPort(GLint x, GLint y, GLsizei width, GLsizei height);

	void RasterPos(GLint x, GLint y);
	void RasterPos(GLfloat x, GLfloat y);
	void RasterPos(GLdouble x, GLdouble y);

	void PointSize(GLfloat size);

	void StencilFunc(GLenum func, GLint ref, GLuint mask);

	bool UseFontOutlines(HDC hDC, DWORD dwFirst, DWORD dwCount, DWORD dwListBase, FLOAT fDeviation,
											 FLOAT fExtrusion, int nFormat, LPGLYPHMETRICSFLOAT  lpgmf);

	HDC GetCurrentDC();

protected:
	bool InitMultisampling(HDC hDC);

protected:
	// main opengl dll handle
	HMODULE m_hOpenGL;
	// glu dll handle
	HMODULE m_hGLU;

#ifdef _DEBUG
	// debug error messages
	CSimpleMap<GLenum, LPTSTR> m_mapErrorNames;
#endif

	// vendor CString
	CString m_strVendor;
	// renderer CString
	CString m_strRenderer;
	// version CString
	CString m_strVersion;

	// extensions
	CSimpleMap<GL_EXTENSION, bool> m_mapExtensions;

protected:
	PFnChoosePixelFormat m_pfnChoosePixelFormat;
	PFnSetPixelFormat m_pfnSetPixelFormat;
	PFnwglCreateContext m_pfnwglCreateContext;
	PFnwglMakeCurrent m_pfnwglMakeCurrent;
	PFnglClearDepth m_pfnglClearDepth;
	PFnglDepthFunc m_pfnglDepthFunc;
	PFnglEnable m_pfnglEnable;
	PFnglEnable m_pfnglDisable;
	PFnSwapBuffers m_pfnSwapBuffers;
	PFnwglGetCurrentDC m_pfnwglGetCurrentDC;
	PFnglTranslatef m_pfnglTranslatef;
	PFnglRotatef m_pfnglRotatef;
	PFnglLoadName m_pfnglLoadName;
	PFnglCallList m_pfnglCallList;
	PFnglFinish m_pfnglFinish;
	PFnglNewList m_pfnglNewList;
	PFnglBegin m_pfnglBegin;
	PFnglNormal3f m_pfnglNormal3f;
	PFnglVertex3f m_pfnglVertex3f;
	PFnglEnd m_pfnglEnd;
	PFnglEndList m_pfnglEndList;
	PFnglTexCoord2f m_pfnglTexCoord2f;
	PFnglMaterialfv m_pfnglMaterialfv;
	PFnglLightfv m_pfnglLightfv;
	PFnglLightf m_pfnglLightf;
	PFnglMatrixMode m_pfnglMatrixMode;
	PFnglGenTextures m_pfnglGenTextures;
	PFnglBindTexture m_pfnglBindTexture;
	//PFngluBuild2DMipmaps m_pfngluBuild2DMipmaps;
	PFnglTexParameterf m_pfnglTexParameterf;
	PFnglLightModelfv m_pfnglLightModelfv;
	PFnglLoadIdentity m_pfnglLoadIdentity;
	PFnglGetIntegerv m_pfnglGetIntegerv;
	PFnglSelectBuffer m_pfnglSelectBuffer;
	PFnglPushName m_pfnglPushName;
	PFnglPushMatrix m_pfnglPushMatrix;
	PFnglPopMatrix m_pfnglPopMatrix;
	PFnglFlush m_pfnglFlush;
	PFngluPerspective m_pfngluPerspective;
	PFngluPickMatrix m_pfngluPickMatrix;
	PFnglViewport m_pfnglViewport;
	PFnglRenderMode m_pfnglRenderMode;
	PFnglInitNames m_pfnglInitNames;
	PFnglClear m_pfnglClear;
	PFnglPopName m_pfnglPopName;
	PFnglGenLists m_pfnglGenLists;
	PFnglDeleteLists m_pfnglDeleteLists;
	PFnglPixelStorei m_pfnglPixelStorei;
	PFnglTexEnvf m_pfnglTexEnvf;
	PFnglGetError m_pfnglGetError;
	PFnglTexImage2D m_pfnglTexImage2D;
	PFngluScaleImage m_pfngluScaleImage;
	PFnglShadeModel m_pfnglShadeModel;

	PFnglRasterPos2f m_pfnglRasterPos2f;
	PFnglRasterPos2i m_pfnglRasterPos2i;

	PFnglDrawPixels m_pfnglDrawPixels;

	PFnglListBase m_pfnglListBase;

	PFnglCallLists m_pfnglCallLists;

	PFnglScalef m_pfnglScalef;

	PFnwglUseFontBitmaps m_pfnwglUseFontBitmaps;
	PFnwglUseFontOutlines m_pfnwglUseFontOutlines;

	PFnglColor3ub m_pfnglColor3ub;

	PFnglHint m_pfnglHint;

	PFnglLineWidth m_pfnglLineWidth;

	PFnglGetString m_pfnglGetString;
	PFnwglGetProcAddress m_pfnwglGetProcAddress;

	PFnglOrtho m_pfnglOrtho;

	PFnglPointSize m_pfnglPointSize;

	PFnglBlendFunc m_pfnglBlendFunc;

	PFnglStencilFunc m_pfnglStencilFunc;

protected:
	PFnwglChoosePixelFormatARB m_pwglChoosePixelFormatARB;

};
