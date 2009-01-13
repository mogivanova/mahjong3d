/*=================================================================================================
*
*  Copyright (c) 1998-2003 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg CE
*  $Workfile: Debug.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.08.05 0:53 $
*
*================================================================================================*/

#pragma once

inline void TraceLastErrorDebug()
{
	LPVOID lpMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								 NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								 (LPTSTR) &lpMsgBuf, 0, NULL);
	ATLTRACE((LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	ATLASSERT(FALSE);
}

inline void _cdecl TraceLog(LPCTSTR lpszFormat, ...)
{
	USES_CONVERSION;

	HANDLE hFile = ::CreateFile(_T("mahjongg.log"), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		::SetFilePointer(hFile, 0L, NULL, FILE_END);

		int nBuf;
		TCHAR szBuffer[4096] = {0};

		va_list args;
		va_start(args, lpszFormat);

#ifndef UNICODE
		nBuf = _vsnprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), lpszFormat, args);
#else
		nBuf = _vsnwprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), lpszFormat, args);
#endif
		ATLASSERT(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

		DWORD dwWritten;
		::WriteFile(hFile, szBuffer, lstrlen(szBuffer), &dwWritten, NULL);

		::CloseHandle(hFile);
	}
}

#ifdef _DEBUG

#define _TRACE ATLTRACE
#define _TRACE_LAST_ERROR(Expr) {if(!(Expr)) TraceLastErrorDebug();}

#define _VERIFY(Expr) _ASSERT(Expr)

#define RETURN(_hRes) {if(FAILED(_hRes)) _TRACE(_T("Error: %s:%d\n"), __FILE__, __LINE__); return _hRes;}

#else

/*
#define _TRACE TraceLog
#define _TRACE_LAST_ERROR __noop
*/

inline void _cdecl DebugTrace(const char* pszFormat, ...) {}

#define _TRACE __noop
#define _TRACE_LAST_ERROR __noop

#define _VERIFY(Expr) ((void)(Expr))

#define RETURN(_hRes) return _hRes

#endif