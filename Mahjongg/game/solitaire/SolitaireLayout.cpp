/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SolitaireLayout.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:45 $
*
*================================================================================================*/

#include "StdAfx.h"

#include "SolitaireLayout.h"
#include "Service\MJCrypt.h"
#include "..\..\Service\XML\XML.h"


CSolitaireLayout::CSolitaireLayout(void)
{
}


CSolitaireLayout::~CSolitaireLayout(void)
{
}


bool CSolitaireLayout::Load(LPCTSTR pszPathName)
{
	CString strPath = pszPathName;
	unsigned int nExt = strPath.ReverseFind('.');

	if (nExt != -1)
	{
		CString strExt = strPath.Mid(nExt);

		if (strExt == _T(".lay"))
			return LoadKyodai(pszPathName);
	}

	return LoadMJ(pszPathName);
}


bool CSolitaireLayout::LoadResource(UINT nResourceID)
{
	bool bResult = false;
	HRSRC hRsrc = FindResource(_Module.m_hInst, MAKEINTRESOURCE(nResourceID), TEXT("LAYOUT"));

	if (hRsrc != NULL)
	{
		HGLOBAL hResData = ::LoadResource(_Module.m_hInst, hRsrc);

		if (hResData != NULL)
		{
			char* pchLayout = (char*)LockResource(hResData);

			if (pchLayout != NULL)
			{
				bResult = LoadFromMJXML(pchLayout);
			}
		}
	}

	/*
	ATLTRACE("[");
	for(int k = 0; k < this->m_nDepth; k++)
	{
	  if(k > 0)
	    ATLTRACE(",\n");
	  ATLTRACE("[");
	  for(int j = 0; j < this->m_nHeight; j++)
	  {
	    if(j > 0)
	      ATLTRACE(",\n");
	    ATLTRACE("[");
	    for(int i = 0; i < m_nWidth; i++)
	    {
	      if(i > 0)
	        ATLTRACE(", ");
	      if(GetTile(i, j, k) != NO_TILE)
	      {
	        ATLTRACE("1");
	      }
	      else
	      {
	        ATLTRACE("0");
	      }
	    }
	    ATLTRACE("]\n");
	  }
	  ATLTRACE("]\n");
	}
	ATLTRACE("]\n");
	*/

	return bResult;
}


bool CSolitaireLayout::LoadMJ(LPCTSTR pszPathName)
{
	if (lstrlen(pszPathName) == 0)
		return false;

	bool bResult = false;

	HANDLE hFile = CreateFile(pszPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		char chStream[10000];
		memset(chStream, 0, sizeof(chStream));

		DWORD dwRead;
		::ReadFile(hFile, chStream, 10000, &dwRead, NULL);

		bResult = LoadFromMJXML(chStream);

		CloseHandle(hFile);
	}

	return bResult;
}


bool CSolitaireLayout::LoadKyodai(LPCTSTR pszPathName)
{
	bool bResult = false;

	HANDLE hFile = CreateFile(pszPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		char chStream[10000];
		DWORD dwRead;
		::ReadFile(hFile, chStream, 10000, &dwRead, NULL);

		CString strFile = chStream;

		int nPos = 0;
		nPos = strFile.Find('\n');
		CString strVersion = strFile.Left(nPos);
		strFile = strFile.Mid(nPos + 1);

		nPos = strFile.Find('\n');
		m_strName = strFile.Left(nPos);
		strFile = strFile.Mid(nPos + 1);

		//
		SetDimensions(34, 20, 5);

		//
		int nCountTiles = 0;

		for (unsigned int k = 0; k < m_nDepth; k++)
		{
			for (unsigned int j = 0; j < m_nHeight; j++)
			{
				for (unsigned int i = 0; i < m_nWidth; i++)
				{
					if (strFile.GetAt(i + (j * m_nWidth) + (k * m_nWidth * m_nHeight)) == '1')
					{
						//GetTile(m_nWidth - i - 1, m_nHeight - j - 1, k) = EMPTY_TILE;
						GetTile(i, m_nHeight - j - 1, k) = EMPTY_TILE;
						nCountTiles++;
					}
				}
			}
		}

		if (nCountTiles != 0)
			bResult = true;

		CloseHandle(hFile);
	}

	return bResult;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
bool CSolitaireLayout::LoadFromMJXML(char* pchLayout)
{
	bool bResult = false;

	CString strLayout = pchLayout;

	if (strLayout.Find(_T("<?xml")) == -1)
		return false;

	xmlTag root;

	errType objResult = ParseXML(strLayout, &root);

	if (objResult.code() == eSuccess && root.childs.GetSize() > 0)
	{
		xmlTag* pLayout = root.childs[0];

		if (pLayout != NULL && pLayout->getname() == _T("mj:layout"))
		{
			for (int i = 0; i < pLayout->childs.GetSize(); i++)
			{
				xmlTag* pChild = pLayout->childs[i];

				if (pChild->getname() == _T("mj:name"))
				{
					m_strName = pChild->getvalue();
				}
				else if (pChild->getname() == _T("mj:author"))
				{
					m_strAuthor = pChild->getvalue();
				}
				else if (pChild->getname() == _T("mj:tiles"))
				{
					int I = -1, J = -1, K = -1;

					for (int k = 0; k < pChild->items.GetSize(); k++)
					{
						Item* pItem = pChild->items[k];

						if (pItem->getname() == _T("width"))
							I = _ttoi(pItem->getvalue());
						else if (pItem->getname() == _T("height"))
							J = _ttoi(pItem->getvalue());
						else if (pItem->getname() == _T("depth"))
							K = _ttoi(pItem->getvalue());
					}

					SetDimensions(I, J, K);

					for (int j = 0; j < pChild->childs.GetSize(); j++)
					{
						xmlTag* pTile = pChild->childs[j];

						if (pTile->getname() == _T("mj:tile"))
						{
							int I = -1, J = -1, K = -1;

							for (int k = 0; k < pTile->items.GetSize(); k++)
							{
								Item* pItem = pTile->items[k];

								if (pItem->getname() == _T("i"))
									I = _ttoi(pItem->getvalue());
								else if (pItem->getname() == _T("j"))
									J = _ttoi(pItem->getvalue());
								else if (pItem->getname() == _T("k"))
									K = _ttoi(pItem->getvalue());
							}

							if (I != -1 && J != -1 && K != -1)
							{
								GetTile(I, J, K) = EMPTY_TILE;
							}
						}
					}
				}
			}
		}

		bResult = true;
	}

	return bResult;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
/*
bool CSolitaireLayout::LoadFromMJString(char* pchLayout)
{
  CMJStream stream(pchLayout);

  int nCheck = stream.ReadShortByte();

  int W = stream.ReadShortByte();
  int H = stream.ReadShortByte();
  int D = stream.ReadShortByte();
  SetDimensions(W, H, D);

  int nTileCount = 0;
  for(unsigned int i = 0; i < m_nWidth; i++)
  {
    for(unsigned int j = 0; j < m_nHeight; j++)
    {
      for(unsigned int k = 0; k < m_nDepth; k++)
      {
        unsigned char ch = stream.ReadShortByte();
        _ASSERT(ch != 44);
        if(ch != 0)
        {
          GetTile(i, j, k) = EMPTY_TILE;
          nTileCount++;
        }
      }
    }
  }

  nCheck = stream.ReadShortByte();
  _ASSERT(nCheck == 44);

  if(nTileCount != 144)
    return false;

  m_strName = stream.ReadString();

  return true;
}
*/

bool CSolitaireLayout::LoadFromKyodaiString(TCHAR* pszLayout)
{
	return false;
}


void CSolitaireLayout::Set(CSolitaireLayout& objLayout)
{
	CBoard::Set(objLayout);
	m_strName = objLayout.m_strName;
}


bool CSolitaireLayout::LoadDescription(CString strPathName, CLayoutDescription** pDescription)
{
	*pDescription = NULL;
	bool bResult = false;

	CSolitaireLayout tmp;

	if (tmp.Load(strPathName) || tmp.LoadKyodai(strPathName))
	{
		CLayoutDescription* pDesc = new CLayoutDescription;
		*pDescription = pDesc;

		pDesc->m_strName = tmp.GetName();
		pDesc->m_bLocal = true;
		pDesc->m_strPath = strPathName;
		pDesc->m_bKyodaiFormat = false;

		bResult = true;
	}

	return bResult;
}