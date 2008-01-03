/*
This file is distributed "as is", e.g. there are no warranties
and obligations and you could use it in your applications on your
own risk. Although your comments and questions are welcome.

Source: QProfile.cpp
Author: (c) Dan Kozub, 1999
URL : http://members.tripod.com/~DanKozub
Email : Dan_Kozub@usa.net, Dan_Kozub@pemail.net
Last Modified: Feb 7, 1999
Version: 1.3

Modified by Andrew Ovcharov, 2002

*/

#include "stdafx.h"

#include "QProfile.h"

#ifdef DEBUG

#define QPROFILE_MIN_MAX
//comment above line to exclude min-max info collection
//this can speed up profiling a bit

// Initializing static variables
QProfile* QProfile::m_pChainHead = NULL;
int QProfile::m_nMaxFileNameLen;
LONGLONG QProfile::m_lProgramTime;
LONGLONG QProfile::m_lFrequency;
TCHAR QProfile::m_pszStrBuffer[QPROFILE_NAMELEN];
bool QProfile::m_bStopProfiling = false;

LPCTSTR QProfile::m_pszOutputFileName = _T("QProfile.txt");

// after 20K output file will be truncated
int QProfile::m_nOutFileMaxSize = 20000;

// by default time in ms is excluded from report
int QProfile::OutputFilter = QProfile_OutputFilters_None;

QProfile_Sorting QProfile::SortBy = QProfile_Sort_Count;

// write output to all destinations
int QProfile::Output = QProfile_Out_All | QProfile_Out_File_Append | QProfile_Out_Add_Source;


#ifdef UNICODE
#define _wsprintf swprintf
#else
#define _wsprintf sprintf
#endif

// This is the main object to measure total time
QProfile QProfile_Program(_T("Mahjongg3D Total time"));

////////////////////////////////////////////////////////////////////////////////////////////////////
//
QProfile::QProfile(LPCTSTR name, bool delete_after_report, LPCTSTR file_name, int line_num)
{
	if (name != NULL)
		::lstrcpyn(Name, name, QPROFILE_NAMELEN);
	else
		Name[0] = '\0';

	DeleteAfterReport = delete_after_report;

	Elapsed = 0;

	LastStart = 0;

	TimeInChildren = 0;

	Counter = 0;

	Running = 0;

	Next = m_pChainHead;

	m_pChainHead = this;

	AutoStarterActive = 0;

	if (!QProfile_Program.IsRunning())
		QProfile_Program.Start();

	FirstParentFunction = NULL;

	ReportPrinted = false;

#ifdef QPROFILE_MIN_MAX
	MaxTime = 0;

	MinTime = 0x7FFFFFFFFFFFFFFF;

#endif
	FileName = file_name;

	if (lstrlen(file_name) > m_nMaxFileNameLen)
		m_nMaxFileNameLen = lstrlen(file_name);

	LineNumber = line_num;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
void QProfile::Reset()
{
	Elapsed = 0;
	LastStart = 0;
	Counter = 0;
	Running = 0;
#ifdef QPROFILE_MIN_MAX
	MaxTime = 0;
	MinTime = 0x7FFFFFFFFFFFFFFF;
#endif
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
void QProfile::PrintSummary()
{
	//Summary should be printed only once
	static bool SummaryAlreadyPrinted = false;

	if (SummaryAlreadyPrinted)
		return;
	else
		SummaryAlreadyPrinted = true;

	m_bStopProfiling = true;

	QProfile_Program.Stop();

	GET_TIMER_FREQ(m_lFrequency);

	m_lProgramTime = QProfile_Program.Elapsed;

	TCHAR buff[255];

	Out(_T("\r\n----------------- Profiling results -----------------\r\n"));

	SYSTEMTIME time;

	GetLocalTime(&time);

	_wsprintf( buff, _T("Date: %02d.%02d.%02d, Time: %02d:%02d.%02d\r\n"),
						 time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);

	Out(buff);

	while (true)
	{
		QProfile * max = m_pChainHead;
		QProfile * cur = m_pChainHead;

		while (cur)
		{
			if (*cur > *max)
				max = cur;

			cur = cur->Next;
		}

		if (max->ReportPrinted)
			break;

		max->PrintReport();
	}

	Out(_T("\r\n------------------------------------------------------\r\n"), true, _T(""));

	//let's go through all objects once more to delete some
	QProfile* cur = m_pChainHead;

	while (cur)
	{
		QProfile* next = cur->Next;

		if (cur->DeleteAfterReport)
			delete cur;

		cur = next;
	}

	return;
}

void QProfile::PrintReport(int level)
{
	_ASSERT(TimeInChildren >= 0);

	if (ReportPrinted)
		return;

	ReportPrinted = true;

	TCHAR buff[255];

	TCHAR buff2[255];

	double elapsed = (double)Elapsed / m_lFrequency * 1000;

	double share = (double)Elapsed / m_lProgramTime * 100;

	double no_children = (double)(Elapsed - TimeInChildren) / m_lFrequency * 1000;

	double no_children_share = (double)(Elapsed - TimeInChildren) / m_lProgramTime * 100;

	static bool first_line = true;

	if (first_line)
	{
		Out(_T("------------------------------------------------------\r\n"), false, _T(""));
		Out(_T("|-Child|Total "), false, _T(""));

		if ((OutputFilter&QProfile_OutputFilters_Time) == 0)
			Out(_T("|Time (ms) "));

		if ((OutputFilter&QProfile_OutputFilters_Count) == 0)
			Out(_T("|  Hits  "));

		if ((OutputFilter&QProfile_OutputFilters_TimePerCall) == 0)
			Out(_T("|Time/call "));

#ifdef QPROFILE_MIN_MAX
		Out(_T("|   MIN   |   MAX   "));

#endif
		Out(_T("| Function \r\n"));

		Out(_T("------------------------------------------------------\r\n"), false, _T(""));
	}

	if (no_children_share == share)
	{
		if (SortBy == QProfile_Sort_Time)
			_wsprintf(buff, _T("|     |%6.2f"), no_children_share);
		else
			_wsprintf(buff, _T("|%6.2f|      "), no_children_share);
	}
	else
		_wsprintf(buff, _T("|%6.2f|%6.2f"), no_children_share, share);

	if (FileName)
		_wsprintf(buff2, _T("%-*.*s(%3d) :"), m_nMaxFileNameLen, m_nMaxFileNameLen, FileName, (WORD)LineNumber);
	else
		_wsprintf(buff2, _T(""));

	Out(buff, false, buff2);

	if ((OutputFilter&QProfile_OutputFilters_Time) == 0)
	{
		_wsprintf(buff, _T("|%10.2f"), elapsed);
		Out(buff);
	}

	if ((OutputFilter&QProfile_OutputFilters_Count) == 0)
	{
		_wsprintf(buff, _T("|%8I64d"), Counter);
		Out(buff);
	}

	if ((OutputFilter&QProfile_OutputFilters_TimePerCall) == 0)
	{
		_wsprintf(buff, _T("|%10.3lf"), elapsed / Counter);
		Out(buff);
	}

	Out(_T("|"));

#ifdef QPROFILE_MIN_MAX

	if (MinTime == 0x7FFFFFFFFFFFFFFF) MinTime = 0;

	_wsprintf(buff, _T("%9.3lf|%9.3lf|"),
						(double)MinTime / m_lFrequency*1000,
						(double)MaxTime / m_lFrequency*1000);

	Out(buff);

#endif
	for (int l = 0; l < level; l++)
		Out(_T(" "));

	Out(Name);

	if (FirstParentFunction != NULL)
	{
		_wsprintf(buff, _T("(%3.1lf%%)"), (double) Elapsed / FirstParentFunction->Elapsed * 100);
		Out(buff);
	}

	Out(_T("\r\n"));

	if (Output & QProfile_Out_DrawBar)
	{
		double bar_share = no_children_share;

		if (SortBy == QProfile_Sort_Time)
			bar_share = share;

		Out(PrintBar(bar_share, 100, 80), false, _T(""));

		Out(_T("\r\n"));
	}

	first_line = false;

	if (SortBy == QProfile_Sort_Time)
	{
		QProfile * child = FindNextChild(NULL);

		while (child)
		{
			child->PrintReport(level + 1);
			child = FindNextChild(child);
		}
	}

	return;
}

/*********************************************************************************************************************/
/*                                                                                                                   */
/*********************************************************************************************************************/
TCHAR* QProfile::PrintBar(double val, double max, int length)
{
	static TCHAR buff[256] = {0};

	if (length > 255)
		length = 255;

	double to_print = (double)length * val / max;

	int to_print_int = (int)to_print;

	int i = 0;

	for (i = 0; i < length; i++)
	{
		while (true)
		{
			if (i == to_print_int)
			{
				buff[i] = '#';
				break;
			}

			if (i == to_print_int*10)
			{
				buff[i] = '#';
				break;
			}

			if (i < to_print_int)
			{
				buff[i] = '>';
				break;
			}

			if (i < to_print_int*10)
			{
				buff[i] = '=';
				break;
			}

			buff[i] = '.';

			break;
		}
	}

	buff[i] = 0;

	return buff;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool QProfile::Out(LPCTSTR string, bool last, LPCTSTR debug_only)
{
	if (Output & QProfile_Out_DebugWindow)
	{
		OutputDebugString(string);
	}

	if (Output & QProfile_Out_Consol)
	{
#ifdef UNICODE
		wprintf(string);
#else
		printf(string);
#endif

		if (last)
			printf("\n");
	}

	if ((Output & QProfile_Out_File) == 0)
	{
		return true;
	}

	static HANDLE Handle = NULL;

	if (Handle == NULL)
	{
		SYSTEMTIME tm;
		GetLocalTime(&tm);

		TCHAR szReportFile[MAX_PATH];
		_wsprintf(szReportFile, _T("%sProfile %04d-%02d-%02d %02d-%02d-%02d.txt"), LOG_PATH,
							tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, m_pszOutputFileName);

		Handle = ::CreateFile(szReportFile, GENERIC_WRITE, 0, 0L, OPEN_ALWAYS, 0, 0);
	}

	if (Handle != INVALID_HANDLE_VALUE)
	{
		DWORD written = 0;
		DWORD dwSize = lstrlen(string) * sizeof(TCHAR);
		BOOL ok = ::WriteFile(Handle, string, dwSize, &written, NULL);

		if (last)
			::CloseHandle(Handle);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
QProfile* QProfile::FindNextChild(QProfile * find_after)
{
	QProfile* cur = m_pChainHead;

	if (find_after)
		cur = find_after->Next;

	while (cur)
	{
		if (cur->FirstParentFunction == this)
			return cur;

		cur = cur->Next;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool QProfile::operator>(QProfile& to_compare)
{
	if (to_compare.ReportPrinted)
		return true;

	if (ReportPrinted)
		return false;

	if (SortBy == QProfile_Sort_Time)
	{
		if (to_compare.FirstParentFunction != FirstParentFunction)
			return FirstParentFunction == NULL;
	}

	switch (SortBy)
	{

	case QProfile_Sort_Time:

		if (Elapsed > to_compare.Elapsed)
			return true;
		else
			return false;

	case QProfile_Sort_PureTime:
		if (Elapsed - TimeInChildren > to_compare.Elapsed - to_compare.TimeInChildren)
			return true;
		else
			return false;

	case QProfile_Sort_Count:
		if (Counter > to_compare.Counter)
			return true;
		else
			return false;

	case QProfile_Sort_TimePerCall:
		if ((double)Elapsed / Counter > (double)to_compare.Elapsed / to_compare.Counter )
			return true;
		else
			return false;

	default:
		return false;
	}

	return false;
}

/*********************************************************************************************************************/
/*                                                                                                                   */
/*********************************************************************************************************************/
DWORD QProfileStarter::m_dwTLSID = -1L;

QProfileStarter* QProfileStarter::RecursiveCallFrom()
{
	QProfileStarter* pCurrent = m_pParent;

	while (pCurrent != NULL)
	{
		if (pCurrent->m_pProfile == m_pProfile)
			return pCurrent;

		pCurrent = pCurrent->m_pParent;
	}

	return NULL;
}

QProfileStarter::~QProfileStarter()
{
	if (QProfile::m_bStopProfiling)
		return;

	LONGLONG now;

	GET_TIMER_VALUE(now);

	LONGLONG elapsed = now - StartTime;

	TlsSetValue(m_dwTLSID, m_pParent);

	bool maybe_recursive = (--m_pProfile->AutoStarterActive) > 0;

#ifdef QPROFILE_MIN_MAX
	if (elapsed > m_pProfile->MaxTime)
		m_pProfile->MaxTime = elapsed;

	if (elapsed < m_pProfile->MinTime)
		m_pProfile->MinTime = elapsed;

#endif
	if (!m_pParent)
	{
		m_pProfile->Add(elapsed, TimeInChildren);
		return;
	}

	// checking calling function to be unique caller
	if (!m_pProfile->FirstParentFunction)
	{
		m_pProfile->FirstParentFunction = m_pParent->m_pProfile;
	}
	else
	{
		if (m_pProfile->FirstParentFunction != m_pParent->m_pProfile)
			m_pProfile->FirstParentFunction = NULL;
	}

	QProfileStarter* recursive = NULL;

	if (maybe_recursive)
		recursive = RecursiveCallFrom();

	if (!recursive)
	{
		m_pProfile->Add(elapsed, TimeInChildren);
		m_pParent->TimeInChildren += elapsed;
	}
	else
	{
		m_pProfile->Counter++;

		if (recursive == m_pParent)
		{
			m_pParent->TimeInChildren += TimeInChildren;
		}
		else
		{
			m_pParent->TimeInChildren += elapsed;
			//time in recursive call should be excluded
			recursive->TimeInChildren -= elapsed - TimeInChildren;
		}
	}

	return;
}

#endif