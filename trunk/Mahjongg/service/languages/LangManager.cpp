/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: LangManager.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 29.04.04 10:32 $
*
*================================================================================================*/

#include "stdafx.h"
#include "..\..\resource.h"

#include "LangManager.h"
#include "..\XML\XML.h"

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CLangManager::CLangDescriptor::CLangDescriptor()
{
	m_enmCodePage = eDefault;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CLangManager::CLangDescriptor::CLangDescriptor(const CLangDescriptor& desc)
{
	m_strLanguageName = desc.m_strLanguageName;
	m_strLanguageLocalName = desc.m_strLanguageLocalName;
	m_enmCodePage = desc.m_enmCodePage;

	for (int i = 0; i < desc.m_arrValues.GetSize(); i++)
	{
		m_arrNames.Add(desc.m_arrNames[i]);
		m_arrValues.Add(desc.m_arrValues[i]);
		m_mapIndexes.Add(desc.m_arrNames[i], m_arrNames.GetSize() - 1);
	}
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CLangManager::CLangDescriptor::~CLangDescriptor()
{
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CLangManager::CLangDescriptor::Load(CString strLanguageDescription)
{
	bool bResult = true;

	xmlTag root;
	errType objResult = ParseXML(CString(strLanguageDescription), &root);

	if (objResult.code() == eSuccess)
	{
		for (int i = 0; i < root.childs.GetSize(); i++)
		{
			xmlTag* pChild = root.childs[i];

			if (pChild->getname() == "language")
			{
				for (int j = 0; j < pChild->childs.GetSize(); j++)
				{
					xmlTag* pLangChild = pChild->childs[j];

					if (pLangChild->getname() == "language_name")
						m_strLanguageName = pLangChild->getvalue();

					if (pLangChild->getname() == "language_local_name")
						m_strLanguageLocalName = pLangChild->getvalue();

					if (pLangChild->getname() == "codepage")
					{
						m_enmCodePage = eDefault;

						if (pLangChild->getvalue() == "Cyrillic")
							m_enmCodePage = eCyrillic;
					}

					if (pLangChild->getname() == "strings")
					{
						for (int k = 0; k < pLangChild->childs.GetSize(); k++)
						{
							xmlTag* pString = pLangChild->childs[k];

							m_arrNames.Add(pString->getname());
							m_arrValues.Add(pString->getvalue());

							m_mapIndexes.Add(pString->getname(), m_arrNames.GetSize() - 1);
						}
					}
				}
			}
		}
	}
	else
	{
		_TRACE(_T("Error loading XML %d, %d\n"), objResult.code(), objResult.position());
	}

	return bResult;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CString CLangManager::CLangDescriptor::GetString(CString strName)
{
	CString strRetval = strName;

	int it = m_mapIndexes.FindKey(strName);

	if (it != -1)
	{
		int nIdx = m_mapIndexes.GetValueAt(it);

		if (nIdx >= 0)
		{
			strRetval = m_arrValues[nIdx];
		}
	}

#ifdef _DEBUG
	else
	{
		if (it == -1)
		{
			m_mapIndexes.Add(strName, -2);
			_TRACE(_T("Unknown string for language '%s':\n<%s></%s>\n"), m_strLanguageName, strName, strName);
		}
	}

#endif
	return strRetval;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CLangManager::CLangManager()
{
	m_bInitialized = false;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CLangManager::~CLangManager()
{
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CLangManager::Initialize()
{
	QPROFILE_FUN(_T("CLangManager::Initialize"));
	//
	HRSRC hRsrc = FindResource(_Module.m_hInst, MAKEINTRESOURCE(IDR_LANG_DEFAULT), TEXT("LANGUAGE"));

	if (hRsrc != NULL)
	{
		HGLOBAL hResData = ::LoadResource(_Module.m_hInst, hRsrc);

		if (hResData != NULL)
		{
			char* pchLayout = (char*)LockResource(hResData);

			if (pchLayout != NULL)
			{
				CLangDescriptor objLangDescriptor;

				if (objLangDescriptor.Load(pchLayout))
				{
					m_vecLangDescriptors.Add(objLangDescriptor);
				}
			}
		}
	}

	//
	bool bResult1 = Initialize(MakeInstallPath(_T("Languages")));

	bool bResult2 = Initialize(MakeUserPath(_T("Languages")));

	if (!bResult1 && !bResult2)
		return false;

	m_bInitialized = true;

	return true;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CLangManager::Initialize(LPCTSTR pszInitPath)
{
	CString strFiles = pszInitPath + CString("/*.xml");

	WIN32_FIND_DATA FindFileData;
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATA));

	HANDLE hFind = ::FindFirstFile(strFiles, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		CString strFilePath = CString(pszInitPath) + "\\" + CString(FindFileData.cFileName);
		HANDLE hFile = ::CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			DWORD dwSize = GetFileSize(hFile, NULL);
			char* szBuffer = new char[dwSize + 1];

			DWORD dwRead = 0;
			ReadFile(hFile, szBuffer, dwSize, &dwRead, NULL);

			CloseHandle(hFile);

			szBuffer[dwSize] = '\0';

			CLangDescriptor objLangDescriptor;

			if (objLangDescriptor.Load(szBuffer))
			{
				m_vecLangDescriptors.Add(objLangDescriptor);
			}
		}
	}
	while (FindNextFile(hFind, &FindFileData));

	return true;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CLangManager::GetLanguageNames(CSimpleArray<CString>& arrNames, CSimpleArray<CString>& arrLocalNames)
{
	arrNames.RemoveAll();
	arrLocalNames.RemoveAll();

	for (int i = 0; i < m_vecLangDescriptors.GetSize(); i++)
	{
		arrNames.Add(m_vecLangDescriptors[i].m_strLanguageName);
		arrLocalNames.Add(m_vecLangDescriptors[i].m_strLanguageLocalName);
	}

	return (m_vecLangDescriptors.GetSize() > 0);
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CLangManager::SetLanguage(CString strLanguage)
{
	for (int i = 0; i < m_vecLangDescriptors.GetSize(); i++)
	{
		if (m_vecLangDescriptors[i].m_strLanguageName == strLanguage)
		{
			m_nCurrent = i;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CString CLangManager::GetLanguage()
{
	return m_vecLangDescriptors[m_nCurrent].m_strLanguageName;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CLangManager::CODE_PAGE CLangManager::GetCodePage()
{
	return m_vecLangDescriptors[m_nCurrent].m_enmCodePage;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
CString CLangManager::GetString(CString strName)
{
	return m_vecLangDescriptors[m_nCurrent].GetString(strName);
}