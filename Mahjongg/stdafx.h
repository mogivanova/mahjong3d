/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: stdafx.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 20.03.04 13:25 $
*
*================================================================================================*/

#pragma once

// Change these values to use different versions
#define WINVER		0x0600
#define _WIN32_WINNT	0x0600
#define _WIN32_IE	0x0600
#define _RICHEDIT_VER	0x0100

#define _WTL_NO_CSTRING
#define __ATLTYPES_H__

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlctrls.h>
#include <atlctrlw.h>

#include <atlmisc.h>
#include <atlframe.h>
#include <atldlgs.h>
#include <atlddx.h>
#include <atlcrack.h>
#include <atlcoll.h>
#include <atlstr.h>
#include <atlimage.h>
#include <atltheme.h>

#include <math.h>

#include <htmlhelp.h>
#pragma comment(lib, "htmlhelp.lib")

#include "wininet.h"
#pragma comment(lib, "wininet.lib")

#include <direct.h>

#include "Service\Profile\QProfile.h"

#include "Service\Common.h"
#include "Service\Debug.h"

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "winmm.lib")

#include "Service\Languages\LangManager.h"

#include "resource.h"