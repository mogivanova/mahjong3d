/*=================================================================================================
*
*  Copyright (c) 1998-2003 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg CE
*  $Workfile: ZLib.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:25 $
*
*================================================================================================*/

#pragma once

#include "ZLibFile.h"

class CZLibArchive
{

public:
	CZLibArchive();
	~CZLibArchive();

	bool Open(LPCTSTR pszFileName);
	bool IsOpen()
	{
		return m_bIsOpen;
	}

	bool Close();

	bool ReadFile(LPCTSTR pszFileName, CZLibFile& fileData);

protected:
	void* m_zf;
	bool m_bIsOpen;
};