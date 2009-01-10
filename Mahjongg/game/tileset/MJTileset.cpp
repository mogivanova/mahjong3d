/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MJTileset.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 24.07.05 23:53 $
*
*================================================================================================*/

#include "stdafx.h"
#include <strstream>
#include "..\..\resource.h"

#include "MJTileset.h"
#include "service/ResourceStream.h"

#include "../../Service/ZLib/ZLib.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTileset::CTileset()
{
	m_bLoaded = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTileset::~CTileset()
{

}


/**
 * @brief    LoadResource
 *
 * CTileset::LoadResource
 *
 * @return bool
 */
bool CTileset::LoadResource()
{
	CComPtr<IStream> spStream;

	for (int N = 0; N < 43; N++)
	{
		if(SUCCEEDED(ResourceStream::Create(_Module.GetResourceInstance(), MAKEINTRESOURCE(TILE00 + N), _T("JPEG"), &spStream)))
		{
			m_aImages[N].Load(spStream);
			spStream.Release();
		}
	}

	if(SUCCEEDED(ResourceStream::Create(_Module.GetResourceInstance(), MAKEINTRESOURCE(LEFT_SIDE), _T("JPEG"), &spStream)))
	{
		m_aSideImages[0].Load(spStream);
		spStream.Release();
	}

	if(SUCCEEDED(ResourceStream::Create(_Module.GetResourceInstance(), MAKEINTRESOURCE(BOTTOM_SIDE), _T("JPEG"), &spStream)))
	{
		m_aSideImages[1].Load(spStream);
		spStream.Release();
	}

	m_bLoaded = true;

	m_strName = g_LangManager.GetString("str_default");

	return m_bLoaded;
}


/**
 * @brief    Load
 *
 * CTileset::Load
 *
 * @param LPCTSTR pszFileName
 * @return bool
 */
bool CTileset::Load(LPCTSTR pszFileName)
{
	CString strFileName = pszFileName;
	CString strExt = _T("mjt");

	/* */
	size_t nExtpos = strFileName.ReverseFind(_T('.'));

	if (nExtpos != 0xFFFFFFFF)
	{
		strExt = strFileName.Mid((int)nExtpos);
	}

	/* */
	if (strExt == _T(".mjt") || !LoadKyodaiTileset(pszFileName))
	{
		if (!LoadMJTileset(pszFileName))
		{
			LoadResource();
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CTileset::LoadMJTileset(LPCTSTR pszPathName)
{
	CZLibArchive objZA;

	if (!objZA.Open(pszPathName))
		return false;

	CZLibFile objZFIni;

	objZA.ReadFile(_T("tileset.ini"), objZFIni);

	CString strIniFile = objZFIni.ToString();

	while (!strIniFile.IsEmpty())
	{
		CString strLine;
		int nNewLine = strIniFile.Find('\n');

		if (nNewLine == -1)
		{
			strLine = strIniFile;
			strIniFile.Empty();
		}
		else
		{
			strLine = strIniFile.Left(nNewLine);
			strIniFile = strIniFile.Mid(nNewLine + 1);
		}

		strLine.TrimLeft();

		strLine.TrimRight();

		int nEqPos = strLine.Find('=');

		if (nEqPos != -1)
		{
			CString strVar = strLine.Left(nEqPos);
			CString strVal = strLine.Mid(nEqPos + 1);

			if (strVar == "Name")
				m_strName = strVal;

			if (strVal == "Author")
				m_strAuthor = strVal;
		}
	}

	for (int N = 0; N <= 42; N++)
	{
		CString strName;
		strName.Format(_T("%02d.jpg"), N);

		CZLibFile objZF;

		if (objZA.ReadFile(strName, objZF))
		{
			//m_aImages[N].Decode(objZF.GetData(), objZF.GetSize(), ATL::CImage_FORMAT_JPG);
		}
		else
		{
			HRSRC hRes = ::FindResource(_Module.m_hInstResource, MAKEINTRESOURCE(TILE00 + N), _T("JPEG"));
			_ASSERT(hRes != NULL);

			//bool bRes = m_aImages[N].LoadResource(hRes, ATL::CImage_FORMAT_JPG, _Module.m_hInstResource);
			//_TRACE(_T("Tile image %d is loaded %s\n"), N, bRes ? _T("Ok") : _T("Fail"));
		}
	}

	m_aSideImages[0].Create(1, 1, 24);

	m_aSideImages[1].Create(1, 1, 24);

	objZA.Close();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CTileset::LoadKyodaiTileset(LPCTSTR pszPathName)
{
	ATL::CImage objTilesetImage;

// if(!objTilesetImage.Load(pszPathName, ATL::CImage_FORMAT_JPG) &&
//   !objTilesetImage.Load(pszPathName, ATL::CImage_FORMAT_BMP))
	return false;

	m_strName = _T("");

	unsigned int nTilesetWidth = objTilesetImage.GetWidth();
	unsigned int nTilesetHeight = objTilesetImage.GetHeight();

	unsigned int nTileWidth = nTilesetWidth / 9;
	unsigned int nTileHeight = nTilesetHeight / 5;

	// tile positions col:row
	static int aTilePos[43][2] =
	{
		{8, 5},
		// dots suit
		{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1},
		// white dragon
		{6, 5},
		// bamboos
		{1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2},
		// green dragon
		{5, 5},
		// characters
		{1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3},
		// red dragon
		{7, 5},
		// winds
		{5, 4}, {6, 4}, {7, 4}, {8, 4},
		// seasons
		{1, 4}, {2, 4}, {3, 4}, {4, 4},
		// flowers
		{1, 5}, {1, 5}, {1, 5}, {1, 5}
	};

	// tiles

	for (int N = 0; N < 43; N++)
	{
		int nTileImageI = (aTilePos[N][0] - 1) * nTileWidth;
		int nTileImageJ = (5 - aTilePos[N][1]) * nTileHeight;

		CRect rect(nTileImageI, nTileImageJ, nTileImageI + nTileWidth, nTileImageJ + nTileHeight);

		GetSubImage(objTilesetImage, rect, m_aImages[N]);
	}

	// side images.
	CRect rect(7 * nTileWidth, 0 * nTileHeight, 8 * nTileWidth, 1 * nTileHeight);

	GetSubImage(objTilesetImage, rect, m_aSideImages[0]);

	GetSubImage(objTilesetImage, rect, m_aSideImages[1]);

	// ok
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
void CTileset::GetSubImage(ATL::CImage &src, CRect rect, ATL::CImage &dst)
{
	unsigned int nTileWidth = rect.right - rect.left;
	unsigned int nTileHeight = rect.bottom - rect.top;

	//dst.Create(nTileWidth, nTileHeight, src.GetBpp());

	for (unsigned int i = 0; i < nTileWidth; i++)
	{
		for (unsigned int j = 0; j < nTileHeight; j++)
		{
			//dst.SetPixelColor(i, j, src.GetPixelColor(rect.left + i, rect.top + j));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CTileset::LoadDescription(CString strPathName, CTilesetDescription** pDescription)
{
	*pDescription = NULL;

	bool bResult = false;

	CZLibArchive objZA;

	if (!objZA.Open(strPathName))
		return false;

	CTilesetDescription* pDesc = new CTilesetDescription;

	*pDescription = pDesc;

	/// read ini file, if exist
	CZLibFile objZF;

	if (objZA.ReadFile(_T("tileset.ini"), objZF))
	{
		CString strFile = objZF.ToString();

		int nPos = 0;

		while (nPos < strFile.GetLength())
		{
			int nNewPos = strFile.Find('\n', nPos);
			CString strLine;

			if (nNewPos == -1)
			{
				strLine = strFile.Mid(nPos);
				nPos = strFile.GetLength();
			}
			else
			{
				strLine = strFile.Mid(nPos, nNewPos - nPos);
				nPos = nNewPos + 1;
			}

			int nEqPos = strLine.Find('=');

			if (nEqPos != -1)
			{
				CString strVar = strLine.Mid(0, nEqPos);
				CString strVal = strLine.Mid(nEqPos + 1);

				if (strVar == "Name")
				{
					pDesc->m_strName = strVal;
				}

				if (strVar == "Author")
				{
					pDesc->m_strAuthor = strVal;
				}
			}
		}
	}
	else
	{
		int nPos = strPathName.ReverseFind(_T('\\'));

		if (nPos == -1)
			pDesc->m_strName = strPathName;
		else
			pDesc->m_strName = strPathName.Mid(nPos + 1);
	}

	int nTile[] = {01, 20, 32, 40};

	for (int N = 0; N < 4; N++)
	{
		CString strName;
		strName.Format(_T("%02d.jpg"), nTile[N]);

		CZLibFile objZF;

		if (objZA.ReadFile(strName, objZF))
		{
			//pDesc->m_imgTile[N].Decode(objZF.GetData(), objZF.GetSize(), ATL::CImage_FORMAT_JPG);
		}
	}

	pDesc->m_bKyodaiFormat = false;

	return true;
}