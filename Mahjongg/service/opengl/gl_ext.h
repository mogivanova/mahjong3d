/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: gl_ext.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#pragma once

enum GL_EXTENSION
{
	GL_ARB_multitexture,
	GL_EXT_texture_env_combine,
	GL_EXT_WGL_ARB_multisample
};

typedef const char * (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);
