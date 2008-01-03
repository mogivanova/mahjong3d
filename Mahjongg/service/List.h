/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: GAPI Library
*  $Workfile: Array.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 3.01.06 14:38 $
*
*================================================================================================*/

#pragma once

/**
* Array
* \author Thinking Thing Software
* \version 1.0
* \date 2004
* \todo implement this class
*/
template <class T>

class CList
{

protected:

	struct SChunk
	{
		T m_Item;
		SChunk* m_pNext;
		SChunk* m_pPrev;
		SChunk()
		{
			m_pNext = NULL;
			m_pPrev = NULL;
		}
	};

	SChunk* m_pHead;
	unsigned int m_nSize;

public:
	typedef SChunk* TIterator;

public:
	// Construction/destruction
	CList() : m_pHead(NULL), m_nSize(0)
	{
	}

	//
	~CList()
	{
		RemoveAll();
	}

	// Operations
	unsigned int GetSize() const
	{
		return m_nSize;
	}

	///
	HRESULT Add(T& t)
	{
		if (m_pHead == NULL)
		{
			m_pHead = new SChunk();

			if (m_pHead == NULL)
				RETURN(E_OUTOFMEMORY);

			new(&m_pHead->m_Item) Wrapper(t);
		}
		else
		{
			SChunk* pChunk = m_pHead;

			while (pChunk->m_pNext != NULL)
				pChunk = pChunk->m_pNext;

			InsertAfter(pChunk, t);
		}

		m_nSize++;

		return S_OK;
	}

	/**
	*
	*/
	HRESULT InsertBefore(TIterator it, T& t)
	{
		SChunk* pChunk = (SChunk*) it;
		SChunk* pNewChunk = new SChunk();

		if (pNewChunk == NULL)
			RETURN(E_OUTOFMEMORY);

		new(&pNewChunk->m_Item) Wrapper(t);

		pNewChunk->m_pPrev = pChunk->m_pPrev;

		pNewChunk->m_pNext = pChunk;

		pChunk->m_pPrev = pNewChunk;

		if (pNewChunk->m_pPrev != NULL)
			pNewChunk->m_pPrev->m_pNext = pNewChunk;

		// move head if needed
		while (m_pHead->m_pPrev != NULL)
			m_pHead = m_pHead->m_pPrev;

		return S_OK;
	}

	/**
	 *
	 */
	HRESULT InsertAfter(TIterator it, T& t)
	{
		SChunk* pChunk = (SChunk*) it;
		SChunk* pNewChunk = new SChunk();

		if (pNewChunk == NULL)
			RETURN(E_OUTOFMEMORY);

		new(&pNewChunk->m_Item) Wrapper(t);

		pNewChunk->m_pPrev = pChunk;

		pNewChunk->m_pNext = pChunk->m_pNext;

		pChunk->m_pNext = pNewChunk;

		if (pNewChunk->m_pNext != NULL)
			pNewChunk->m_pNext->m_pPrev = pNewChunk;

		return S_OK;
	}

	///
	HRESULT Remove(T& t)
	{
		RETURN(E_NOTIMPL);
	}

	/**
	 *
	 */
	HRESULT RemoveAt(unsigned int nIndex)
	{
		RETURN(E_NOTIMPL);
	}

	/**
	 *
	 */
	HRESULT RemoveAll()
	{
		SChunk* pChunk = m_pHead;

		while (pChunk != NULL)
		{
			SChunk* pCurrent = pChunk;
			pChunk = pChunk->m_pNext;
			delete pCurrent;
		}

		return S_OK;
	}

	// Implementation

	class Wrapper
	{

	public:
		Wrapper(T& _t) : t(_t)
		{
		}

		template <class _Ty>
		void *operator new(size_t, _Ty* p)
		{
			return p;
		}

		T t;
	};


	TIterator Find(T& t) const
	{
		SChunk* pChunk = m_pHead;

		while (pChunk != NULL)
		{
			if (pChunk->m_Item == t)
				return pChunk;

			pChunk = pChunk->m_pNext;
		}

		return NULL;  // not found
	}

	TIterator GetHeadPosition()
	{
		return m_pHead;
	}

	T& GetNext(TIterator& iterator)
	{
		SChunk* pChunk = (SChunk*)iterator;
		iterator = pChunk->m_pNext;
		return pChunk->m_Item;
	}
};
