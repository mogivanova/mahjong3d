/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SolitaireLayout.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 16.03.04 0:15 $
*
*================================================================================================*/

#pragma once

#include "Board.h"

class CLayoutDescription
{

public:
	CString m_strName;
	CString m_strPath;
	bool m_bKyodaiFormat;
	bool m_bLocal;

public:
	CLayoutDescription()
	{
	}

	CLayoutDescription(CString strName, CString strPath, bool bLocal = true)
	{
		m_strName = strName;
		m_strPath = strPath;
		m_bLocal = bLocal;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
//

class CSolitaireLayout :
			public CBoard
{

protected:
	CString m_strName;
	CString m_strAuthor;

public:
	CSolitaireLayout(void);
	~CSolitaireLayout(void);

	bool Load(LPCTSTR pszPathName);
	bool LoadResource(UINT nResourceID);
	bool LoadMJ(LPCTSTR pszPathName);
	bool LoadKyodai(LPCTSTR pszPathName);

	CString GetName()
	{
		return m_strName;
	}

	virtual void Set(CSolitaireLayout& objLayout);

public:
	static bool LoadDescription(CString strPathName, CLayoutDescription** pDescription);

protected:
	//bool LoadFromMJString(char* pchLayout);
	bool LoadFromMJXML(char* pchLayout);
	bool LoadFromKyodaiString(TCHAR* pszLayout);
};
