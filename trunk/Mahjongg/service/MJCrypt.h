#pragma once

#include "service/Buffer.h"

class CMJCryptStream
{
public:
	CMJCryptStream();

	void* GetBuffer();
	DWORD GetSize();

	HRESULT SetBuffer(void* pBuffer, size_t nSize);

public:
	HRESULT Write(char val);
	HRESULT Write(unsigned char val);
	HRESULT Write(short val);
	HRESULT Write(unsigned short val);
	HRESULT Write(int val);
	HRESULT Write(unsigned int val);
	HRESULT Write(long val);
	HRESULT Write(unsigned long val);
	HRESULT Write(__int64 val);
	HRESULT Write(unsigned __int64 val);

	HRESULT Write(CString str);

	HRESULT Write(void* pBuffer, size_t nLength);

	HRESULT Read(char* pval);
	HRESULT Read(unsigned char* pval);
	HRESULT Read(short* pval);
	HRESULT Read(unsigned short* pval);
	HRESULT Read(int* pval);
	HRESULT Read(unsigned int* pval);
	HRESULT Read(long* pval);
	HRESULT Read(unsigned long* pval);
	HRESULT Read(__int64* pval);
	HRESULT Read(unsigned __int64* pval);

	HRESULT Read(CString* pstr);

	HRESULT Read(void* pBuffer, size_t nLength);

protected:
	CBuffer m_objBuffer;

	size_t m_dwSize;
	LPBYTE m_pCurrent;
};