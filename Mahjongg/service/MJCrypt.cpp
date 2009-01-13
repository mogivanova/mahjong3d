#include "stdafx.h"
#include "MJCrypt.h"

#define ALLOC_SIZE 2048

/**
 * @brief CMJCryptStream
 *
 *
 * 
 * @return 
 */
CMJCryptStream::CMJCryptStream()
{
	m_dwSize = 0;
	m_pCurrent = 0;
}

/**
 * @brief GetBuffer
 *
 *
 * 
 * @return void*
 */
void* CMJCryptStream::GetBuffer()
{
	return m_objBuffer.GetBuffer();
}

/**
 * @brief GetSize
 *
 *
 * 
 * @return DWORD
 */
DWORD CMJCryptStream::GetSize()
{
	return m_dwSize;
}

/**
 * @brief SetBuffer
 *
 *
 * 
 * @param void * pBuffer
 * @param size_t nSize
 * @return HRESULT
 */
HRESULT CMJCryptStream::SetBuffer(void* pBuffer, size_t nSize)
{
	m_objBuffer.Destroy();
	m_objBuffer.SetBuffer((char*)pBuffer, nSize);
	m_dwSize = nSize;
	m_pCurrent = (LPBYTE)m_objBuffer.GetBuffer();
	return S_OK;
}

/**
 * @brief Write
 *
 *
 * 
 * @param char val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(char val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param unsigned char val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(unsigned char val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param short val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(short val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param unsigned short val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(unsigned short val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param int val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(int val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param unsigned int val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(unsigned int val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param long val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(long val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param unsigned long val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(unsigned long val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param __int64 val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(__int64 val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param unsigned __int64 val
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(unsigned __int64 val)
{
	return Write(&val, sizeof(val));
}

/**
 * @brief Write
 *
 *
 * 
 * @param CString str
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(CString str)
{
	DWORD dwLen = str.GetLength();
	Write(dwLen);
	Write(str.GetBuffer(), dwLen);
	return S_OK;
}

/**
 * @brief Write
 *
 *
 * 
 * @param void * pBuffer
 * @param size_t nLength
 * @return HRESULT
 */
HRESULT CMJCryptStream::Write(void* pBuffer, size_t nLength)
{
	if (m_dwSize + nLength > m_objBuffer.GetSize())
	{
		m_objBuffer.Resize(m_objBuffer.GetSize() + ALLOC_SIZE);
	}

	LPVOID pWrite = &((m_objBuffer.GetBuffer())[m_dwSize]);

	memcpy(pWrite, pBuffer, nLength);

	m_dwSize += nLength;

	return S_OK;
}

/**
 * @brief Read
 *
 *
 * 
 * @param char * pval
 * @return HRESULT
 */
HRESULT CMJCryptStream::Read(char* pval)
{
	return Read(pval, sizeof(*pval));
}

/**
 * @brief Read
 *
 *
 * 
 * @param unsigned char * pval
 * @return HRESULT
 */
HRESULT CMJCryptStream::Read(unsigned char* pval)
{
	return Read(pval, sizeof(*pval));
}

/**
 * @brief Read
 *
 *
 * 
 * @param short * pval
 * @return HRESULT
 */
HRESULT CMJCryptStream::Read(short* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(unsigned short* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(int* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(unsigned int* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(long* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(unsigned long* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(__int64* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(unsigned __int64* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(CString* pstr)
{
	DWORD dwLen;
	Read(&dwLen);

	CBufferT<TCHAR> objStrBuffer(dwLen + 1);
	Read(objStrBuffer.GetBuffer(), dwLen);

	objStrBuffer.GetBuffer()[dwLen] = _T('\0');

	*pstr = objStrBuffer.GetBuffer();

	return S_OK;
}

HRESULT CMJCryptStream::Read(void* pBuffer, size_t nLength)
{
	if(m_pCurrent == NULL)
		return E_FAIL;

	memcpy(pBuffer, m_pCurrent, nLength);

	m_pCurrent += nLength;

	return S_OK;
}
