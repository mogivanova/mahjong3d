/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: QProfile.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:43 $
*
*================================================================================================*/

#pragma once

#ifdef _DEBUG

#define LOG_PATH _TEXT("C:\\Work\\DevProj\\Mahjongg3D\\_bin\\")

typedef __int64 LONGLONG;

#define GET_TIMER_VALUE(val) QueryPerformanceCounter((LARGE_INTEGER*)&val)
#define GET_TIMER_FREQ(val) QueryPerformanceFrequency((LARGE_INTEGER*)&val)

#define QPROFILE_NAMELEN 80

//usage: QProfile::SortBy = ONE of the following values
enum QProfile_Sorting
{
	QProfile_Sort_None, // No sort
	QProfile_Sort_Time, // sorting by total time
	QProfile_Sort_PureTime, // sorting by "pure time", i.e. except time in subfunctions
	QProfile_Sort_TimePerCall, // sorting by average time per call
	QProfile_Sort_Count // sorting by count
};


//usage: QProfile::Output = combination of the flags below
enum QProfile_OutputFlags
{
	QProfile_Out_None = 0, // no output at all
	QProfile_Out_File = 1, // output to file
	QProfile_Out_Consol = 2, // output to console
	QProfile_Out_DebugWindow = 4, // output to VC debug window
	QProfile_Out_All = 7, // output to file, consol, debug window
	QProfile_Out_File_Append = 8, // when writing to file - append new data
	QProfile_Out_Add_Source = 16, // add source line info to debug window
	QProfile_Out_DrawBar = 32 // add a bar after each line
};

//usage: QProfile::OutputFilter = combination of the following flags
//aim: to exclude data, you do not need for your analysis.
enum QProfile_OutputFilters
{
	QProfile_OutputFilters_None = 0,
	QProfile_OutputFilters_Time = 1,
	QProfile_OutputFilters_PureTime = 2,
	QProfile_OutputFilters_TimePerCall = 4,
	QProfile_OutputFilters_Count = 8
};


class QProfile
{

	friend class QProfileStarter;
//static
	static QProfile* m_pChainHead; //the pointer to the first class in the chain
	static LONGLONG m_lFrequency; //m_lFrequency of the timer
	static LONGLONG m_lProgramTime; //total time of the programm
	static bool m_bStopProfiling; //stops profiling
	static int m_nMaxFileNameLen; //is used to store max length of the source file

public:
	static QProfile_Sorting SortBy; // by time, count, ...
	static int Output; // to file, consol, debug window
	static LPCTSTR m_pszOutputFileName; // by default = "QProfile.txt"
	static int m_nOutFileMaxSize; // append output until file size < m_nOutFileMaxSize
	static int OutputFilter; // excludes some data e.g. time per call
	static TCHAR m_pszStrBuffer[QPROFILE_NAMELEN];

	static void PrintSummary(); //you can call this function directly

private:
	static TCHAR* PrintBar(double val, double max, int lenght);
	static bool Out(LPCTSTR string, bool last = false, LPCTSTR debug_only = NULL);

	double ElapsedInMS();
	void PrintReport(int level = 0);
	void Add(LONGLONG& add, LONGLONG& in_children);
	bool operator>(QProfile& to_compare);
	QProfile * FindNextChild(QProfile * find_after);

private:
	bool DeleteAfterReport; //this object is constructed by new and should be deleted
	LONGLONG Elapsed; //time in function
	LONGLONG TimeInChildren; //time in children
	LONGLONG LastStart; //last start time
	LONGLONG MaxTime; //max time per call
	LONGLONG MinTime; //min time per call
	LONGLONG Counter; //count of calls
	QProfile * Next; //pointer to next class
	int Running; //Start() was called
	int AutoStarterActive; //
	QProfile * FirstParentFunction;//Pointer to the parent function ifit is UNIQUE!
	bool ReportPrinted; //this function report is already printed
	LPCTSTR FileName; //name of the source file this objects was constructed
	int LineNumber; //line of the source file this objects was constructed

public:
	TCHAR Name[QPROFILE_NAMELEN]; //name of the function

public:
	QProfile(LPCTSTR name, bool delete_after_report = false, LPCTSTR file_name = _T(""), int line_num = 0);
	~QProfile();
	void Start();
	void Stop();
	void Reset();
	bool IsRunning()
	{
		return Running != 0;
	}
};


extern QProfile QProfile_Program;


inline QProfile::~QProfile()
{
	if (m_pChainHead)
	{
		PrintSummary();
	}
};


inline void QProfile::Start()
{
	if (!Running)
	{
		GET_TIMER_VALUE(LastStart);
	}

	Running++;
};


inline void QProfile::Stop()
{
	if (!Running)
		return;

	Counter++;

	Running--;

	if (Running)
	{
		return;
	}

	LONGLONG now;

	GET_TIMER_VALUE(now);
	Elapsed += now - LastStart;
};


inline void QProfile::Add(LONGLONG& add, LONGLONG& in_children)
{
	Elapsed += add;
	TimeInChildren += in_children;
	Counter++;
};


inline double QProfile::ElapsedInMS()
{
	return (double) Elapsed / m_lFrequency * 1000.;
}

class QProfileStarter
{
	QProfile* m_pProfile;
	QProfileStarter* m_pParent;
	LONGLONG StartTime;
	LONGLONG TimeInChildren;
	static DWORD m_dwTLSID;

public:
	QProfileStarter(QProfile *pProfile)
	{
		m_pProfile = pProfile;
		GET_TIMER_VALUE(StartTime);

		if (m_dwTLSID == (DWORD) - 1L)
		{
			m_dwTLSID = TlsAlloc();
		}

		m_pParent = (QProfileStarter*)TlsGetValue(m_dwTLSID);

		TlsSetValue(m_dwTLSID, this);
		TimeInChildren = 0;
		m_pProfile->AutoStarterActive++;
	}

	~QProfileStarter();

private:
	QProfileStarter * RecursiveCallFrom();
};
/*********************************************************************************************************************/
/*                                                                                                                   */
/*********************************************************************************************************************/
#define QPROFILE_DECLARE(var, name) \
 static QProfile var(name, false, _T(__FILE__), __LINE__);

#define QPROFILE_DECLARE1(var, name, param) \
 static QProfile var(QProfile::m_pszStrBuffer+\
 !sprintf(QProfile::m_pszStrBuffer, name, param), false, __FILE__, __LINE__);
#define QPROFILE_DECLARE2(var, name, param, param2) \
 static QProfile var(QProfile::m_pszStrBuffer+\
 var.FileName=__FILE__; var.LineNumber=__LINE__; \
 !sprintf(QProfile::m_pszStrBuffer, param, param2), false, __FILE__, __LINE__);
#define QPROFILE_DECLARE3(var, name, param, param2, param3) \
 static QProfile var(QProfile::m_pszStrBuffer+\
 var.FileName=__FILE__; var.LineNumber=__LINE__; \
 !sprintf(QProfile::m_pszStrBuffer, param, param2, param3), false, __FILE__, __LINE__);


#define QPROFILE_MT_DECLARE(var, name) \
 _declspec(thread) static QProfile* var = NULL; \
 if(!var) var = new QProfile(name, true, _T(__FILE__), __LINE__);
#define QPROFILE_MT_DECLARE1(var, name, param) \
 _declspec(thread) static QProfile * var=NULL; \
 if(!var){ var = new QProfile(name, true, __FILE__, __LINE__); \
 sprintf(var->Name, name, param);}
#define QPROFILE_MT_DECLARE2(var, name, param, param2) \
 _declspec(thread) static QProfile * var=NULL; \
 if(!var){ var = new QProfile(name, true, __FILE__, __LINE__); \
 sprintf(var->Name, name, param, param2);}
#define QPROFILE_MT_DECLARE3(var, name, param, param2, param3) \
 _declspec(thread) static QProfile * var=NULL; \
 if(!var){ var = new QProfile(name, true, __FILE__, __LINE__); \
 sprintf(var->Name, name, param, param2, param3);}


#define QPROFILE_FUN(name) \
 QPROFILE_DECLARE(static_profile, name) \
 QProfileStarter static_profile_auto(&static_profile);
#define QPROFILE_FUN1(name, param) \
 QPROFILE_DECLARE1(static_profile1, name, param) \
 QProfileStarter static_profile_auto1(&static_profile1);
#define QPROFILE_FUN2(name, param, param2) \
 QPROFILE_DECLARE2(static_profile2, name, param, param2) \
 QProfileStarter static_profile_auto2(&static_profile2);
#define QPROFILE_FUN3(name, param, param2, param3) \
 QPROFILE_DECLARE3(static_profile3, name, param, param2, param3) \
 QProfileStarter static_profile_auto3(&static_profile3);


#define QPROFILE_MT_FUN(name) \
 QPROFILE_MT_DECLARE(profile_ptr, name) \
 QProfileStarter profile_ptr_auto(profile_ptr);
#define QPROFILE_MT_FUN1(name, param) \
 QPROFILE_MT_DECLARE1(profile_ptr, name, param) \
 QProfileStarter profile_ptr_auto(profile_ptr);
#define QPROFILE_MT_FUN2(name, param, param2) \
 QPROFILE_MT_DECLARE2(profile_ptr, name, param, param2) \
 QProfileStarter profile_ptr_auto(profile_ptr);
#define QPROFILE_MT_FUN3(name, param, param2, param3) \
 QPROFILE_MT_DECLARE3(profile_ptr, name, param, param2, param3) \
 QProfileStarter profile_ptr_auto(profile_ptr);

#else

#define QPROFILE_FUN(name)

#define QPROFILE_MT_FUN(name)

#endif
