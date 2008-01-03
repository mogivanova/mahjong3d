/*=================================================================================================
*
*  Copyright (c) 1998-2003 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg CE
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 16.02.05 20:16 $
*
*================================================================================================*/

#pragma once

class CFileStorage
{

protected:
	HANDLE m_hFile;

public:
	CFileStorage()
	{
		m_hFile = INVALID_HANDLE_VALUE;
	}

	CFileStorage(HANDLE hFile)
	{
		m_hFile = hFile;
	}

	~CFileStorage()
	{
		Close();
	}

	bool IsOpen()
	{
		return m_hFile != INVALID_HANDLE_VALUE;
	}

	HANDLE Create(LPCTSTR lpFileName, DWORD dwDesiredAccess = 0, DWORD dwShareMode = 0,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, DWORD dwCreationDisposition = 0,
								DWORD dwFlagsAndAttributes = 0, HANDLE hTemplateFile = NULL)
	{
		m_hFile = ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
													 dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

		return m_hFile;
	}

	void Close()
	{
		if (m_hFile == INVALID_HANDLE_VALUE)
			return;

		::CloseHandle(m_hFile);

		m_hFile = INVALID_HANDLE_VALUE;
	}

	DWORD GetSize()
	{
		return GetFileSize(m_hFile, NULL);
	}

	DWORD Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPOVERLAPPED lpOverlapped = NULL)
	{
		if (m_hFile == INVALID_HANDLE_VALUE)
			return 0;

		DWORD nNumberOfBytesRead = 0;

		if (!::ReadFile(m_hFile, lpBuffer, nNumberOfBytesToRead, &nNumberOfBytesRead, lpOverlapped))
			return 0;

		return nNumberOfBytesRead;
	}

	DWORD Read(unsigned char* pchValue, LPOVERLAPPED lpOverlapped = NULL)
	{
		return Read(pchValue, sizeof(unsigned char), lpOverlapped);
	}

	DWORD Read(unsigned int* pnValue, LPOVERLAPPED lpOverlapped = NULL)
	{
		return Read(pnValue, sizeof(unsigned int), lpOverlapped);
	}

	DWORD Read(DWORD* pdwValue, LPOVERLAPPED lpOverlapped = NULL)
	{
		return Read(pdwValue, sizeof(DWORD), lpOverlapped);
	}

	DWORD Read(CString& strValue, LPOVERLAPPED lpOverlapped = NULL)
	{
		DWORD dwStrLength;

		if (Read(&dwStrLength, lpOverlapped) == 0)
			return 0;

		if (dwStrLength == 0)
		{
			strValue = _T("");
			return 1;
		}

		TCHAR* pBuffer = new TCHAR[dwStrLength + 1];

		DWORD dwSize = Read(pBuffer, dwStrLength * sizeof(TCHAR), lpOverlapped);

		if (dwSize != 0)
		{
			pBuffer[dwStrLength] = _T('\0');
			strValue = pBuffer;
		}
		else
		{
			strValue = _T("");
		}

		if (pBuffer != NULL)
			delete [] pBuffer;

		return dwSize > 0 ? dwSize : 1;
	}

	DWORD Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped = NULL)
	{
		if (m_hFile == INVALID_HANDLE_VALUE)
			return 0;

		DWORD nNumberOfBytesWritten = 0;

		if (!::WriteFile(m_hFile, lpBuffer, nNumberOfBytesToWrite, &nNumberOfBytesWritten, lpOverlapped))
			return 0;

		return nNumberOfBytesWritten;
	}

	DWORD Write(unsigned char chValue, LPOVERLAPPED lpOverlapped = NULL)
	{
		return Write(&chValue, sizeof(unsigned char), lpOverlapped);
	}

	DWORD Write(unsigned int nValue, LPOVERLAPPED lpOverlapped = NULL)
	{
		return Write(&nValue, sizeof(unsigned int), lpOverlapped);
	}

	DWORD Write(DWORD dwValue, LPOVERLAPPED lpOverlapped = NULL)
	{
		return Write(&dwValue, sizeof(DWORD), lpOverlapped);
	}

	DWORD Write(CString strValue, LPOVERLAPPED lpOverlapped = NULL)
	{
		if (Write((DWORD)strValue.GetLength(), lpOverlapped) == 0)
			return 0;

		return Write((LPCTSTR)strValue, strValue.GetLength() * sizeof(TCHAR), lpOverlapped);
	}
};












