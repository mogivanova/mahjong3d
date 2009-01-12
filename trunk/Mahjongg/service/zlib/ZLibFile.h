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

class CZLibFile
{

public:
	CZLibFile()
	{
		m_pData = NULL;
		m_dwSize = 0;
	}

	~CZLibFile()
	{
		if (m_pData != NULL) delete [] m_pData;
	}

	LPBYTE GetData()
	{
		return m_pData;
	}

	DWORD GetSize()
	{
		return m_dwSize;
	}

	CString ToString()
	{
		CString strResult;
		char* pchBuffer = new char[m_dwSize + 1];
		memcpy(pchBuffer, m_pData, m_dwSize);
		pchBuffer[m_dwSize] = '\0';
		strResult = pchBuffer;
		delete [] pchBuffer;
		return strResult;
	}

protected:
	LPBYTE m_pData;
	DWORD m_dwSize;

	friend class CZLibArchive;
};
