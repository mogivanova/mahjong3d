/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MJTime.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#pragma once

class CMJTime
{

protected:
	unsigned char m_cDay;
	unsigned char m_cHour;
	unsigned char m_cMinute;
	unsigned char m_cSecond;

public:
	CMJTime();
	~CMJTime();

	void Tick();
	void Reset();

	unsigned char GetDay()
	{
		return m_cDay;
	}

	unsigned char GetHour()
	{
		return m_cHour;
	}

	unsigned char GetMinute()
	{
		return m_cMinute;
	}

	unsigned char GetSecond()
	{
		return m_cSecond;
	}

	bool IsOverflow()
	{
		return m_cDay == 0xFF;
	}
};