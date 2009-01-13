#pragma once

template<class T>

class CBufferT
{

public:
	CBufferT()
	{
		m_pBuffer = NULL;
		m_dwSize = 0;
	}

	CBufferT(DWORD dwSize)
	{
		m_pBuffer = NULL;
		m_dwSize = 0;
		Allocate(dwSize);
	}

	~CBufferT()
	{
		Destroy();
	}

	HRESULT Allocate(DWORD dwSize)
	{
		if (m_pBuffer != NULL)
			return E_FAIL;

		m_pBuffer = (T*)malloc(dwSize * sizeof(T));

		if (m_pBuffer == NULL)
			return E_OUTOFMEMORY;

		m_dwSize = dwSize;

#ifdef _DEBUG
		memset(m_pBuffer, 0, dwSize * sizeof(T));

#endif

		return S_OK;
	}

	HRESULT Destroy()
	{
		if (m_pBuffer != NULL)
		{
			free(m_pBuffer);
		}

		m_pBuffer = NULL;

		m_dwSize = 0;

		return S_OK;
	}

	HRESULT Resize(DWORD dwNewSize)
	{
		T* pNewBuffer = (T*)malloc(dwNewSize * sizeof(T));
#ifdef _DEBUG
		memset(pNewBuffer, 0, dwNewSize * sizeof(T));
#endif
		memcpy(pNewBuffer, m_pBuffer, min(dwNewSize, m_dwSize));

		free(m_pBuffer);

		m_pBuffer = pNewBuffer;
		m_dwSize = dwNewSize;

		return S_OK;
	}

	T* GetBuffer()
	{
		return m_pBuffer;
	}

	DWORD GetSize()
	{
		return m_dwSize;
	}

	HRESULT SetBuffer(T* pBuffer, DWORD dwSize)
	{
		Destroy();
		Allocate(dwSize);
		memcpy(m_pBuffer, pBuffer, dwSize * sizeof(T));

		return S_OK;
	}

protected:
	T* m_pBuffer;
	DWORD m_dwSize;
};

typedef CBufferT<char> CBuffer;