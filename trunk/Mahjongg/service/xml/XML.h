/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: XML.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 17.01.05 22:24 $
*
*================================================================================================*/

#pragma once

/*--------------------------------------------------------------------------------->8-Cut here-----
*
*/
enum tagType { tagUnknown = 0, tagXML = 1, tagPI, tagDTD, tagCDATA, tagComment, tagRawValue };

#define TRIM(str) {str.TrimLeft();str.TrimRight();}

/*--------------------------------------------------------------------------------->8-Cut here-----
*
*/

class Item
{

protected:
	CString m_strName;
	CString m_strValue;

public:
	Item() {};

	Item(CString nm, CString vl)
	{
		TRIM(nm);
		TRIM(vl);
		m_strName = nm;
		m_strValue = vl;
	};

	Item(const Item& a)
	{
		m_strName = a.m_strName;
		m_strValue = a.m_strValue;
	};

	~Item()
	{
	};

	CString getname() const
	{
		return m_strName;
	};

	CString getvalue() const
	{
		return m_strValue;
	};

	void setvalue(CString strValue)
	{
		TRIM(strValue);
		m_strValue = "";

		for (int i = 0; i < strValue.GetLength(); i++)
		{
			if (strValue.GetAt(i) == '\n' || strValue.GetAt(i) == '\t' || strValue.GetAt(i) == '\r')
			{
				m_strValue += ' ';
			}
			else if (strValue.GetAt(i) == '\\')
			{
				i++;
				TCHAR ch = strValue.GetAt(i);

				if (ch == 'n')
					m_strValue += '\n';

				if (ch == 't')
					m_strValue += '\t';
			}
			else if (strValue.GetAt(i) == '&')
			{
				// &amp; tags
				i++;
				CString strChar;

				while (strValue.GetAt(i) != ';')
				{
					strChar += strValue.GetAt(i++);
				}

				if (strChar == "amp")
					m_strValue += '&';
				else if (strChar == "lt")
					m_strValue += '<';
				else if (strChar == "gt")
					m_strValue += '>';
				else if (strChar == "nbsp")
					m_strValue += ' ';
				else
					m_strValue += ' ';
			}
			else
			{
				m_strValue += strValue.GetAt(i);
			}
		}
	};

	Item& operator=(const Item& a)
	{
		m_strValue = a.m_strValue;
		m_strName = a.m_strName;
		return *this;
	}

	friend bool operator<(const Item& lv, const Item& rv)
	{
		return lv.m_strName < rv.m_strName;
	}

	friend bool operator==(const Item& lv, const Item& rv)
	{
		return lv.m_strName == rv.m_strName;
	}

	//friend std::ostream& operator<<(std::ostream& os, const Item& n)
	//{
	//  return os << n.m_strName << '=' << '"' << n.m_strValue << '"';
	//}
};

/*--------------------------------------------------------------------------------->8-Cut here-----
*
*/

class xmlTag : public Item
{

protected:
	xmlTag* parent;
	tagType type;

public:
	static bool parseRawValue;
	CSimpleArray<Item*> items;
	CSimpleArray<xmlTag*> childs;

	xmlTag()
	{
		parent = NULL;
		type = tagUnknown;
	};

	xmlTag(const xmlTag& a) : Item(a.m_strName, a.m_strValue)
	{
		parent = a.parent;
		type = a.type;
	};

	xmlTag(CString nm, CString vl, xmlTag *p, tagType tt = tagXML) : Item(nm, vl), parent(p), type(tt)
	{
	};

	~xmlTag()
	{
		int i;

		for (i = 0; i < items.GetSize(); i++)
			delete items[i];

		items.RemoveAll();

		for (i = 0; i < childs.GetSize(); i++)
			delete childs[i];

		childs.RemoveAll();
	};

	xmlTag *getparent() const
	{
		return parent;
	}

	tagType gettype() const
	{
		return type;
	}

	xmlTag& operator=(const xmlTag& a)
	{
		m_strValue = a.m_strValue;
		m_strName = a.m_strName;
		parent = a.parent;
		type = a.type;
		return *this;
	}

	CString get_attribute(CString strAttribute)
	{
		for (int i = 0; i < items.GetSize(); i++)
		{
			Item* pItem = items[i];

			if (pItem->getname() == strAttribute)
			{
				return pItem->getvalue();
			}
		}

		return _T("");
	}
};

/*--------------------------------------------------------------------------------->8-Cut here-----
*
*/
enum errCode
{
	eSuccess = 0,
	eDTDUnClosed = 1,
	eXSLTUnClosed,
	eAttrOpenQuote,
	eAttrCloseQuote,
	eTagUnClosed,
	eClosedOrphan,
	eMissingGt,
	eCDATAUnClosed,
	eUnknown
};

/*--------------------------------------------------------------------------------->8-Cut here-----
*
*/

class errType
{

protected:
	errCode errcode;
	int index;

public:
	errType(errCode ec = eUnknown, int pos = 0 ): errcode(ec), index(pos)
	{
		_ASSERT(ec == eSuccess);
	};

	errCode code() const
	{
		return errcode;
	}

	int position() const
	{
		return index;
	}
};

/*--------------------------------------------------------------------------------->8-Cut here-----
*
*/
errType ParseXML(const CString&, xmlTag*);
