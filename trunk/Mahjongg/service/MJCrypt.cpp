#include "stdafx.h"
#include "MJCrypt.h"

#define ALLOC_SIZE 2048

CMJCryptStream::CMJCryptStream()
{
	m_dwSize = 0;
	m_pCurrent = 0;
}

void* CMJCryptStream::GetBuffer()
{
	return m_objBuffer.GetBuffer();
}

DWORD CMJCryptStream::GetSize()
{
	return m_dwSize;
}

HRESULT CMJCryptStream::SetBuffer(void* pBuffer, size_t nSize)
{
	m_objBuffer.Destroy();
	m_objBuffer.SetBuffer((char*)pBuffer, nSize);
	m_dwSize = nSize;
	m_pCurrent = (LPBYTE)m_objBuffer.GetBuffer();
	return S_OK;
}

HRESULT CMJCryptStream::Write(char val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(unsigned char val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(short val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(unsigned short val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(int val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(unsigned int val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(long val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(unsigned long val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(__int64 val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(unsigned __int64 val)
{
	return Write(&val, sizeof(val));
}

HRESULT CMJCryptStream::Write(CString str)
{
	DWORD dwLen = str.GetLength();
	Write(dwLen);
	Write(str.GetBuffer(), dwLen);
	return S_OK;
}

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

HRESULT CMJCryptStream::Read(char* pval)
{
	return Read(pval, sizeof(*pval));
}

HRESULT CMJCryptStream::Read(unsigned char* pval)
{
	return Read(pval, sizeof(*pval));
}

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
	memcpy(pBuffer, m_pCurrent, nLength);

	m_pCurrent += nLength;

	return S_OK;
}
