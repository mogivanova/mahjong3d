/*=================================================================================================
*
*  Copyright (c) 1998-2003 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg CE
*  $Workfile: ZLib.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 13.03.04 14:48 $
*
*================================================================================================*/

#include "stdafx.h"

#include "ZLibArchive.h"

#include "unzip.h"

/**
 * @brief    CZLibArchive
 *
 * CZLibArchive::CZLibArchive
 *
 * @return 
 */
CZLibArchive::CZLibArchive()
{
	m_bIsOpen = false;
	m_zf = NULL;
}


/**
 * @brief    ~CZLibArchive
 *
 * CZLibArchive::~CZLibArchive
 *
 * @return 
 */
CZLibArchive::~CZLibArchive()
{
	if (IsOpen())
		Close();
}


/**
 * @brief    Open
 *
 * CZLibArchive::Open
 *
 * @param LPCTSTR pszFileName
 * @return bool
 */
bool CZLibArchive::Open(LPCTSTR pszFileName)
{
	if (IsOpen())
		return false;

#ifdef UNICODE
	int nBufferSize = WideCharToMultiByte(CP_ACP, 0, pszFileName, -1, NULL, 0, NULL, NULL);

	char* szBuffer = new char[nBufferSize];

	WideCharToMultiByte(CP_ACP, 0, pszFileName, -1, szBuffer, nBufferSize, NULL, NULL);

	m_zf = unzOpen(szBuffer);

	delete [] szBuffer;

#else
	m_zf = unzOpen(pszFileName);

#endif

	if (m_zf == NULL)
		return false;

	m_bIsOpen = true;

	return true;
}


/**
 * @brief    Close
 *
 * CZLibArchive::Close
 *
 * @return bool
 */
bool CZLibArchive::Close()
{
	if (!IsOpen())
		return false;

	unzClose(m_zf);

	m_zf = NULL;

	m_bIsOpen = false;

	return true;
}


/**
 * @brief    ReadFile
 *
 * CZLibArchive::ReadFile
 *
 * @param LPCTSTR pszFileName
 * @param CZLibFile & fileData
 * @return bool
 */
bool CZLibArchive::ReadFile(LPCTSTR pszFileName, CZLibFile& fileData)
{
	USES_CONVERSION;

	bool bResult = false;
	int iResult = unzLocateFile(m_zf, T2CA(pszFileName), 0);

	if (iResult == UNZ_OK)
	{
		unz_file_info fi = {0};
		char szFn[_MAX_PATH];

		unzGetCurrentFileInfo(m_zf, &fi, szFn, _MAX_PATH, 0, 0, 0, 0);

		unzOpenCurrentFile(m_zf);

		fileData.m_dwSize = fi.uncompressed_size;
		fileData.m_pData = new BYTE[fileData.m_dwSize];

		unzReadCurrentFile(m_zf, fileData.m_pData, fi.uncompressed_size);

		unzCloseCurrentFile(m_zf);

		bResult = true;
	}

	return bResult;
}