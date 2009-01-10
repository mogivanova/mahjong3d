#pragma once

#include "Settings\MJAppSettings.h"
#include "ExtAPI.h"

#define APP_ID _T("Amazing Mahjongg 3D")
#define APP_VERSION 0x01030000
#define STR_VERSION _T("1.3.0")

#define STR_ONLINE_SERVER _T("http://www.mahjongg4u.com/scripts/sendscores.php")

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
inline float glCOLOR(BYTE c)
{
	return (float)c / 255.0f;
}

inline BYTE getRValue(COLORREF rgb)
{ 
	return LOBYTE(rgb);
}

inline BYTE getGValue(COLORREF rgb)
{
	return LOBYTE(rgb >> 8);
}

inline BYTE getBValue(COLORREF rgb)
{
	return LOBYTE(rgb >> 16);
}

#define glRGB(r, g, b) {glCOLOR(r), glCOLOR(g), glCOLOR(b), 1.0f}
#define glRGBA(r, g, b, a) {glCOLOR(r), glCOLOR(g), glCOLOR(b), a}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
#ifndef TPM_NOANIMATION
#define TPM_NOANIMATION 0x4000L
#endif
#ifndef TPM_HORPOSANIMATION
#define TPM_HORNEGANIMATION 0x0400L
#endif
#ifndef TPM_HORNEGANIMATION
#define TPM_HORNEGANIMATION 0x0800L
#endif
#ifndef TPM_VERPOSANIMATION
#define TPM_VERPOSANIMATION 0x1000L
#endif
#ifndef TPM_VERNEGANIMATION
#define TPM_VERNEGANIMATION 0x2000L
#endif

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CString LoadResourceString(UINT nStringID);
CString MakeInstallPath(CString pszPath);
CString MakeUserPath(CString pszPath);
CString MakeFullPath(CString pszPath);

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
extern OSVERSIONINFO g_osvi;
extern SYSTEMTIME g_tmProgramStarted;

void Sound(UINT nIDResource);
