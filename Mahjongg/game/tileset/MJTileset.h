/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MJTileset.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 24.07.05 23:53 $
*
*================================================================================================*/

#pragma once

class CTilesetDescription
{

public:
	CString m_strName;
	CString m_strAuthor;
	CString m_strRemark;

	bool m_bKyodaiFormat;

	CString m_strPath;
	bool m_bLocal;

	ATL::CImage m_imgTile[4];

public:
	CTilesetDescription()
	{
		m_bKyodaiFormat = false;
	}

	CTilesetDescription(CString strName, CString strAuthor, CString strRemark, CString strPath, bool bLocal = true)
	{
		m_strName = strName;
		m_strAuthor = strAuthor;
		m_strRemark = strRemark;

		m_bKyodaiFormat = strPath == "*default*" ? false : true;

		m_strPath = strPath;
		m_bLocal = bLocal;

		int nPos = strPath.Find('.');

		if (nPos != -1)
		{
			CString strExt = strPath.Mid(nPos);

			if (strExt == ".mjt")
			{
				m_bKyodaiFormat = false;
			}
		}
	}

	CTilesetDescription& operator=(const CTilesetDescription& left)
	{
		m_strName = left.m_strName;
		m_strAuthor = left.m_strAuthor;
		m_strRemark = left.m_strRemark;

		m_bKyodaiFormat = left.m_bKyodaiFormat;

		m_strPath = left.m_strPath;

		m_bLocal = left.m_bLocal;

		return *this;
	}
};

class CTileset
{
	bool m_bLoaded;
	CString m_strName;
	CString m_strAuthor;
	ATL::CImage m_aImages[43];
	ATL::CImage m_aSideImages[3];

public:
	CTileset();
	~CTileset();

	bool IsLoaded()
	{
		return m_bLoaded;
	}

	CString GetName()
	{
		return m_strName;
	}

	ATL::CImage& GetTile(unsigned int i)
	{
		return m_aImages[i];
	}

	ATL::CImage& GetLeft()
	{
		return m_aSideImages[0];
	}

	ATL::CImage& GetBottom()
	{
		return m_aSideImages[1];
	}

	/// load from resource
	bool LoadResource();

	/// load from .jpg or .mjt/.zip file
	bool Load(LPCTSTR pszPathName);
	/// load from .mjt/.zip file
	bool LoadMJTileset(LPCTSTR pszPathName);
	/// load from .jpg file
	bool LoadKyodaiTileset(LPCTSTR pszPathName);

public:
	static bool LoadDescription(CString strPathName, CTilesetDescription** pDescription);

protected:
	void GetSubImage(ATL::CImage& src, CRect rect, ATL::CImage& dst);
};
