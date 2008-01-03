#pragma once

#include <Wininet.h>

/**
 *
 */

class CGLInternetHandle
{

public:
	/**
	 *
	 */
	CGLInternetHandle()
	{
		m_hInternet = NULL;
	}

	/**
	 *
	 */
	CGLInternetHandle(HINTERNET hInternet)
	{
		m_hInternet = hInternet;
	}

	/**
	 *
	 */
	CGLInternetHandle(const CGLInternetHandle& handle)
	{
		m_hInternet = handle.m_hInternet;
	}

	/**
	 *
	 */
	CGLInternetHandle& operator=(const CGLInternetHandle& handle)
	{
		m_hInternet = handle.m_hInternet;
		return *this;
	}

	/**
	 *
	 */
	void Close()
	{
		if (m_hInternet != NULL)
		{
			::InternetCloseHandle(m_hInternet);
			m_hInternet = NULL;
		}
	}

	/**
	 *
	 */
	HINTERNET GetHandle()
	{
		return m_hInternet;
	}

	/**
	 *
	 */
	operator HINTERNET()
	{
		return m_hInternet;
	}

protected:
	///
	HINTERNET m_hInternet;
};

/**
 *
 */

class CGLHttpRequest : public CGLInternetHandle
{

public:
	enum
	{
		eEventConnectionStarted = 0x2000, ///< click the button
		eEventConnectionFinished, ///< click the button
		eEventConnectionFailed ///< click the button
	};

public:
	~CGLHttpRequest();

public:
	/// call initialize prior to operations with internet
	static HRESULT Initialize(LPCTSTR pszApplication);
	/// call to free resources
	static HRESULT Uninitialize();

public:
	/// request parameters
	typedef ATL::CAtlMap<CString, CString> TRequestParams;
	typedef ATL::CAtlMap<CString, CString> TResponse;

public:
	///
	HRESULT Get(LPCTSTR lpszURL, TRequestParams& params, CString& strResponse);
	/// process request
	HRESULT Post(LPCTSTR lpszURL, TRequestParams& params, CString& strResponse);
	///
	HRESULT Parse(CString strResponse, TResponse& mapResponse);

protected:
	/// global inet handle
	static HINTERNET s_hInternet;
};
