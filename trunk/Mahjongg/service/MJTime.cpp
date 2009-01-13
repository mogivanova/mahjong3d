/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: MJTime.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:46 $
*
*================================================================================================*/

#include "stdafx.h"

#include "MJTime.h"


CMJTime::CMJTime()
{
	m_cDay = 0;
	m_cHour = 0;
	m_cMinute = 0;
	m_cSecond = 0;
}


CMJTime::~CMJTime()
{

}


void CMJTime::Reset()
{
	m_cDay = 0;
	m_cHour = 0;
	m_cMinute = 0;
	m_cSecond = 0;
}


void CMJTime::Tick()
{
	if (m_cDay == 0xFF) // overflow
		return;

	m_cSecond++;

	if (m_cSecond == 60)
	{
		m_cSecond = 0;
		m_cMinute++;
	}

	if (m_cMinute == 60)
	{
		m_cMinute = 0;
		m_cHour++;
	}

	if (m_cHour == 24)
	{
		m_cHour = 0;
		m_cDay++;
	}

	if (m_cDay == 31)
	{
		m_cDay = m_cHour = m_cMinute = m_cSecond = 0xFF;
	}
}