/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: XML.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:32 $
*
*================================================================================================*/

#include "stdafx.h"
#include "xml.h"

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
int find_first_of(CString str, LPCTSTR pszMatch, int nPos)
{
	if (pszMatch == NULL)
		return -1;

	for (int i = nPos; i < str.GetLength(); i++)
	{
		TCHAR ch = str.GetAt(i);
		TCHAR* pch = (TCHAR*)pszMatch;

		while (*pch != '\0')
		{
			if (ch == *pch)
				return i;

			pch++;
		}
	}

	return -1;
}

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
int find_first_not_of(CString str, LPCTSTR pszMatch, int nPos)
{
	if (pszMatch == NULL)
		return -1;

	CString strMatch = pszMatch;

	for (int i = nPos; i < str.GetLength(); i++)
	{
		TCHAR ch = str.GetAt(i);

		if (strMatch.Find(ch) == -1)
			return i;
	}

	return -1;
}

/*
inline CString trim(const CString& strInput)
{
  if(strInput.length() == 0)
    return strInput;
  int b = strInput.find_first_not_of(" \t");
  int e = strInput.find_last_not_of(" \t");
  if(b == -1) // No non-spaces
    return "";
  return CString(strInput, b, e - b + 1);
}
*/

bool xmlTag::parseRawValue = false;

/*------------------------------------------------------------------------------->8-- Cut here ----
*
*/
errType ParseXML(const CString& strInput, xmlTag* pRootTag)
{
	//try
	{
		int nLTPos = strInput.Find('<'), nGTPos = 0, nDelimeterPos = 0,
								 nCloseTagPos = 0, nOpenTagPos1 = 0, nOpenTagPos2 = 0,
																									nCloseTagGTPos = 0, nOpenTagGTpos = 0;
		xmlTag *pTag = NULL;
		CString strTag, strTagName, strCloseTag, strOpenTag1, strOpenTag2, v_tag, i_name, i_value;
		tagType enmTagType = tagXML;
		errType enmErrorCode = eSuccess;

		if (nLTPos == -1)
			pRootTag->setvalue(strInput);

		nGTPos = 0;

		while ( nLTPos != -1 )
		{
			if ((nLTPos - nGTPos > 0) && (nGTPos != -1) && (xmlTag::parseRawValue))
			{
				// make tagRawValue

				if ((nLTPos - (nGTPos ? nGTPos : -1) - 1) > 0)
				{
					strTag = strInput.Mid(nGTPos ? nGTPos + 1 : nGTPos, nLTPos - ( nGTPos ? nGTPos : -1 ) - 1);
					pTag = new xmlTag(_T(""), strTag, pRootTag, tagRawValue);
					pRootTag->childs.Add(pTag);
				}
			}

			nGTPos = strInput.Find('>', nLTPos);

			if (nGTPos == -1)
				return errType(eMissingGt, nLTPos);

			switch (strInput[nLTPos + 1])
			{

			case '/':
				return errType(eClosedOrphan, nGTPos);

			case '!':

				if (strInput[nLTPos + 2] == strInput[nLTPos + 3] && strInput[nLTPos + 2] == '-')
				{
					// comment
					enmTagType = tagComment;
					nGTPos = strInput.Find(_T("-->"), nLTPos);
					nLTPos += 3;
				}
				else
				{
					// DTD blocks
					if (strInput.Mid(nLTPos + 2, 7).Compare(_T("[CDATA[")) == 0)
					{
						// CDATA
						enmTagType = tagCDATA;
						nGTPos = strInput.Find(_T("]]>"), nLTPos);

						if (nGTPos == -1)
							return errType(eCDATAUnClosed, nLTPos);

						nLTPos += 8;
					}
					else
					{
						enmTagType = tagDTD;
						nCloseTagPos = nLTPos;

						do
						{
							nOpenTagPos1 = strInput.Find(_T("<!"), nCloseTagPos + 1);
							nCloseTagPos = strInput.Find(_T('>'), nCloseTagPos + 1);

							if (nOpenTagPos1 == -1)
								break;

							if (nCloseTagPos == -1)
								return errType(eDTDUnClosed, nLTPos);
						}
						while (nCloseTagPos > nOpenTagPos1);

						nGTPos = nCloseTagPos;

						nLTPos += 1;
					}
				}

				strTag = strInput.Mid(nLTPos + 1, nGTPos - nLTPos - 1);

				pTag = new xmlTag(_T(""), strTag, pRootTag, enmTagType);
				pRootTag->childs.Add(pTag);

				switch (enmTagType)
				{

				case tagComment:
					nGTPos += 2;
					break;

				case tagCDATA:
					nGTPos += 2;
					break;
				}

				break;

			case '?': // skip Processing Instructions
				enmTagType = tagPI;
				nGTPos = strInput.Find(_T("?>"), nLTPos);

				if (nGTPos == -1)
					return errType(eXSLTUnClosed, nLTPos);

				nLTPos += 1;

				break;

			default:
				strTag = strInput.Mid(nLTPos + 1, nGTPos - nLTPos - 1);

				nDelimeterPos = strTag.Find(' ');

				if (nDelimeterPos == -1)
					nDelimeterPos = strTag.Find('/');

				strTagName = (nDelimeterPos != -1) ? strTag.Mid(0, nDelimeterPos) : strTag;

				pTag = new xmlTag(strTagName, _T(""), pRootTag, enmTagType);

				pRootTag->childs.Add(pTag);

				while ( nDelimeterPos != -1 )
				{
					nCloseTagPos = strTag.Find('=', nDelimeterPos);

					if (nCloseTagPos == -1)
						break;

					i_name = strTag.Mid(nDelimeterPos, nCloseTagPos - nDelimeterPos);

					nDelimeterPos = find_first_of(strTag, _T("\"\'"), nCloseTagPos);

					nOpenTagPos1 = find_first_not_of(strTag, _T(" \n\t\r="), nCloseTagPos);

					if (nDelimeterPos == -1 || nOpenTagPos1 < nDelimeterPos)
						return errType(eAttrOpenQuote, nLTPos + nCloseTagPos);

					nCloseTagPos = strTag.Find(strTag[nDelimeterPos], nDelimeterPos + 1);

					if (nCloseTagPos == -1)
						return errType(eAttrCloseQuote, nLTPos + nDelimeterPos + 1);

					i_value = strTag.Mid(nDelimeterPos + 1, nCloseTagPos - nDelimeterPos - 1);

					nDelimeterPos = strTag.Find(' ', nCloseTagPos);

					pTag->items.Add(new Item(i_name, i_value));
				}

				if (strInput[nGTPos - 1] != '/' && enmTagType == tagXML)
				{
					strCloseTag = _T("</") + strTagName + _T(">");
					strOpenTag1 = _T("<") + strTagName + _T(" ");
					strOpenTag2 = _T("<") + strTagName + _T(">");

					nCloseTagGTPos = nGTPos;
					nOpenTagGTpos = nGTPos;

					do
					{
						// Find <pTag>...<pTag>...</pTag>...</pTag>
						nOpenTagPos1 = strInput.Find(strOpenTag1, nOpenTagGTpos);

						while (nOpenTagPos1 != -1)
						{
							// check <... xxx="zzz"/>
							// error : <x x="/>"><x x="/>"/></x>
							// change later
							nCloseTagPos = strInput.Find(_T("<"), nOpenTagPos1 + 1);
							nOpenTagPos2 = strInput.Find(_T("/>"), nOpenTagPos1 + 1);

							if ((nCloseTagPos == -1) || (nCloseTagPos > nOpenTagPos2))
							{
								nOpenTagPos1 = strInput.Find(strOpenTag1, nOpenTagPos2);
							}
							else
								break;
						}

						nOpenTagPos2 = strInput.Find(strOpenTag2, nOpenTagGTpos);

						nCloseTagPos = strInput.Find(strCloseTag, nCloseTagGTPos);

						if ((nOpenTagPos1 == -1) && (nOpenTagPos2 == -1))
							break;

						nOpenTagPos1 = min((unsigned)nOpenTagPos1, (unsigned)nOpenTagPos2);

						nOpenTagGTpos = nOpenTagPos1 + 1;

						nCloseTagGTPos = nCloseTagPos + 1;
					}
					while (nOpenTagPos1 < nCloseTagPos);

					if (nCloseTagPos == -1)
					{
						return errType(eTagUnClosed, nGTPos);
					}

					v_tag = strInput.Mid(nGTPos + 1, nCloseTagPos - nGTPos - 1);

					enmErrorCode = ParseXML(v_tag, pTag);

					if (pTag->childs.GetSize() <= 0)
					{
						pTag->setvalue(v_tag);
					}

					if (enmErrorCode.code() != eSuccess)
						return errType(enmErrorCode.code(), enmErrorCode.position() + nGTPos + 1);

					nGTPos = nCloseTagPos + strCloseTag.GetLength() - 1;
				}
				else
				{
					nGTPos += (enmTagType == tagXML ? 0 : 1);
				}
			}

			nLTPos = strInput.Find('<', nGTPos);

			if (nLTPos == -1 && xmlTag::parseRawValue)
			{
				strTag = strInput.Mid(nGTPos + 1, -1);

				if (strTag.GetLength() > 0)
				{
					pTag = new xmlTag("", strTag, pRootTag, tagRawValue);
					pRootTag->childs.Add(pTag);
				}
			}

			enmTagType = tagXML;
		}
	}

	//catch(...)
	//{
	//  _ASSERT(false);
	//  return errType(eUnknown);
	//}

	return errType(eSuccess);
};

