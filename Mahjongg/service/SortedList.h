#pragma once

#include "List.h"

/**
* \author Thinking Thing Software
* \version 1.0
* \date 2004
* \todo implement this class
*/
template <class T>

class CSortedList : public CList<T>
{

public:
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

			while (true)
			{
				if (t < pChunk->m_Item)
				{
					InsertBefore(pChunk, t);
					break;
				}

				if (pChunk->m_pNext == NULL)
				{
					InsertAfter(pChunk, t);
					break;
				}

				pChunk = pChunk->m_pNext;
			}
		}

		m_nSize++;

		return S_OK;
	}

};
