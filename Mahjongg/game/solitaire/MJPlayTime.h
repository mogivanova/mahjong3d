#pragma once

/**
*
*/

class CMJGameTime
{

public:
	unsigned __int16 m_nDays;
	unsigned __int8 m_nHours, m_nMinutes, m_nSeconds;

public:
	/**
	*
	*/
	CMJGameTime()
	{
		m_nDays = m_nHours = m_nMinutes = m_nSeconds = 0;
	}

	/**
	*
	*/
	CMJGameTime(const CMJGameTime& t)
	{
		m_nDays = t.m_nDays;
		m_nHours = t.m_nHours;
		m_nMinutes = t.m_nMinutes;
		m_nSeconds = t.m_nSeconds;
	}

	/**
	*
	*/
	CMJGameTime(unsigned long nLong)
	{
		SetLong(nLong);
	}

	/**
	*
	*/
	CMJGameTime& operator +=(const CMJGameTime& tm)
	{
		unsigned long t = tm.GetLong();
		SetLong(t + GetLong());
		return *this;
	}

	/**
	*
	*/
	CMJGameTime& operator =(const CMJGameTime& tm)
	{
		unsigned long t = tm.GetLong();
		SetLong(t);
		return *this;
	}

	/**
	*
	*/
	void Clear()
	{
		m_nDays = 0;
		m_nHours = m_nMinutes = m_nSeconds = 0;
	}

	/**
	*
	*/
	void Advance()
	{
		m_nSeconds++;

		if (m_nSeconds == 60)
		{
			m_nSeconds = 0;
			m_nMinutes++;

			if (m_nMinutes == 60)
			{
				m_nMinutes = 0;
				m_nHours++;

				if (m_nHours == 24)
				{
					m_nHours = 0;
					m_nDays++;
				}
			}
		}
	}

	unsigned __int16 GetDays() const
	{
		return m_nDays;
	}

	unsigned __int8 GetHours() const
	{
		return m_nHours;
	}

	unsigned __int8 GetMinutes() const
	{
		return m_nMinutes;
	}

	unsigned __int8 GetSeconds() const
	{
		return m_nSeconds;
	}

	/**
	*
	*/
	unsigned long GetLong() const
	{
		return ((m_nDays * 24 + m_nHours) * 60 + m_nMinutes) * 60 + m_nSeconds;
	}

	/**
	*
	*/
	void SetLong(unsigned long nLong)
	{
		m_nSeconds = (__int8)nLong % 60;
		nLong /= 60;
		m_nMinutes = (__int8)nLong % 60;
		nLong /= 60;
		m_nHours = (__int8)nLong % 24;
		nLong /= 24;
		m_nDays = (__int16)nLong;
	}
};

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
inline bool operator ==(CMJGameTime& tm1, CMJGameTime& tm2)
{
	return tm1.GetDays() == tm2.GetDays() && tm1.GetHours() == tm2.GetHours() &&
				 tm1.GetMinutes() == tm2.GetMinutes() && tm1.GetSeconds() == tm2.GetSeconds();
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
inline bool operator <(CMJGameTime& tm1, CMJGameTime& tm2)
{
	__int32 ltm1 = (((tm1.GetDays() * 24) + tm1.GetHours()) * 60 + tm1.GetMinutes()) * 60 + tm1.GetSeconds();
	__int32 ltm2 = (((tm2.GetDays() * 24) + tm2.GetHours()) * 60 + tm2.GetMinutes()) * 60 + tm2.GetSeconds();

	return ltm1 < ltm2;
}

/*------------------------------------------------------------------------------>8-- Cut here -----
*
*/
inline bool operator <= (CMJGameTime& tm1, CMJGameTime& tm2)
{
	__int32 ltm1 = (((tm1.GetDays() * 24) + tm1.GetHours()) * 60 + tm1.GetMinutes()) * 60 + tm1.GetSeconds();
	__int32 ltm2 = (((tm2.GetDays() * 24) + tm2.GetHours()) * 60 + tm2.GetMinutes()) * 60 + tm2.GetSeconds();

	return ltm1 <= ltm2;
}
