#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
// glu functions

typedef void (APIENTRY* PFngluPerspective)(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
typedef void (APIENTRY* PFngluPickMatrix)(GLdouble x, GLdouble y, GLdouble width, GLdouble height, GLint viewport[4]);
typedef int (APIENTRY* PFngluScaleImage)(GLenum format, GLint widthin, GLint heightin, GLenum typein, const void *datain, GLint widthout, GLint heightout, GLenum typeout, void *dataout);

///////////////////////////////////////////////////////////////////////////////////////////////////
// WIN GL API functions
typedef WINGDIAPI int (WINAPI* PFnChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR* ppfd);
typedef WINGDIAPI BOOL (WINAPI* PFnSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
typedef WINGDIAPI HGLRC (WINAPI* PFnwglCreateContext)(HDC);
typedef WINGDIAPI BOOL (WINAPI* PFnwglMakeCurrent)(HDC, HGLRC);
typedef WINGDIAPI BOOL (WINAPI* PFnSwapBuffers)(HDC);
typedef WINGDIAPI HDC (WINAPI* PFnwglGetCurrentDC)(VOID);
typedef WINGDIAPI BOOL (WINAPI* PFnwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);
typedef WINGDIAPI BOOL (WINAPI* PFnwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT, FLOAT, int, LPGLYPHMETRICSFLOAT);
typedef WINGDIAPI void* (WINAPI* PFnwglGetProcAddress)(LPCSTR lpszProc);
