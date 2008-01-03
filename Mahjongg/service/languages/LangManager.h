/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: LangManager.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#pragma once

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/

class CLangManager
{

public:
	enum CODE_PAGE
	{
		eDefault,
		eCyrillic
	};

protected:

	class CLangDescriptor
	{
		typedef CSimpleMap<CString, CString> TStringMap;

	public:
		CString m_strLanguageName;
		CString m_strLanguageLocalName;
		CODE_PAGE m_enmCodePage;
		CSimpleArray<CString> m_arrNames;
		CSimpleArray<CString> m_arrValues;
		CSimpleMap<CString, int> m_mapIndexes;

	public:
		CLangDescriptor();
		CLangDescriptor(const CLangDescriptor&);
		~CLangDescriptor();

		bool Load(CString strFilePath);

		CString GetString(CString strName);
	};

protected:
	bool m_bInitialized;
	CSimpleArray<CLangDescriptor> m_vecLangDescriptors;
	unsigned int m_nCurrent;

public:
	CLangManager();
	~CLangManager();

public:
	bool Initialize();

	bool GetLanguageNames(CSimpleArray<CString>& arrNames, CSimpleArray<CString>& arrLocalNames);
	bool SetLanguage(CString strLanguage);

	CString GetLanguage();
	CODE_PAGE GetCodePage();

	CString GetString(CString strName);

protected:
	bool Initialize(LPCTSTR pszInitPath);
};

extern CLangManager g_LangManager;
