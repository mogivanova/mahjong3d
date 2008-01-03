#include "stdafx.h"

#include "HttpConnection.h"

HINTERNET CGLHttpRequest::s_hInternet = NULL;

/**
*
*/
CGLHttpRequest::~CGLHttpRequest()
{
	Close();
}

/**
*
*/
HRESULT CGLHttpRequest::Initialize(LPCTSTR pszApplication)
{
	if (s_hInternet != NULL)
		return S_FALSE; // already initialized

	s_hInternet = ::InternetOpen(pszApplication, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (s_hInternet == NULL)
		RETURN(E_FAIL);

	return S_OK;
}

/**
*
*/
HRESULT CGLHttpRequest::Uninitialize()
{
	if (s_hInternet == NULL)
		return S_FALSE;

	::InternetCloseHandle(s_hInternet);

	s_hInternet = NULL;

	return S_OK;
}

//HRESULT CGLHttpConnection::Connect(LPCTSTR pszURL, LPCTSTR pszUser /* = NULL */, LPCTSTR pszPassword /* = NULL */)
//{
//  if(s_hInternet == NULL)
//    RETURN(E_FAIL);
//
//  memset(m_pszHost, 0, sizeof(m_pszHost));
//  memset(m_pszPath, 0, sizeof(m_pszPath));
//
//  URL_COMPONENTS url = {0};
//  url.dwStructSize = sizeof(URL_COMPONENTS);
//  url.lpszHostName = m_pszHost;
//  url.dwHostNameLength = MAX_PATH;
//  url.lpszUrlPath = m_pszPath;
//  url.dwUrlPathLength = MAX_PATH;
//  if(::InternetCrackUrl(pszURL, lstrlen(pszURL), ICU_DECODE, &url) == FALSE)
//    RETURN(E_FAIL);
//
//  m_hInternet = ::InternetConnect(s_hInternet, url.lpszHostName, url.nPort, url.lpszUserName, url.lpszPassword, INTERNET_SERVICE_HTTP, 0, 0);
//  if(m_hInternet == NULL)
//    RETURN(E_FAIL);
//
//  return S_OK;
//}
//
///**
// *
// */
//CGLHttpRequest CGLHttpConnection::CreateRequest(LPCTSTR pszMethod, LPCTSTR pszResource)
//{
//  CGLHttpRequest objRequest(::HttpOpenRequest(m_hInternet, pszMethod, pszResource, NULL, NULL, NULL, 0, 0));
//
//  return objRequest;
//}

CString GenerateBoundary()
{
	CString boundaryPrefix('-', 27);

	// We now need 12 hex digits.
	int r0 = rand() & 0xffff;
	int r1 = rand() & 0xffff;
	int r2 = rand() & 0xffff;

	char temp[13];
	sprintf(temp, "%04X%04X%04X", r0, r1, r2);

	return boundaryPrefix + temp;
}

/**
*
*/
HRESULT CGLHttpRequest::Post(LPCTSTR lpszURL, TRequestParams& params, CString& strResponse)
{
	USES_CONVERSION;

	TCHAR pszHost[MAX_PATH] = {0};
	TCHAR pszPath[MAX_PATH] = {0};

	URL_COMPONENTS url = {0};
	url.dwStructSize = sizeof(URL_COMPONENTS);
	url.lpszHostName = pszHost;
	url.dwHostNameLength = MAX_PATH;
	url.lpszUrlPath = pszPath;
	url.dwUrlPathLength = MAX_PATH;

	if (::InternetCrackUrl(lpszURL, lstrlen(lpszURL), ICU_DECODE, &url) == FALSE)
		RETURN(E_FAIL);

	CGLInternetHandle hConnection = ::InternetConnect(s_hInternet, url.lpszHostName, url.nPort, url.lpszUserName, url.lpszPassword, INTERNET_SERVICE_HTTP, 0, 0);

	if (hConnection == NULL)
		RETURN(E_FAIL);

	LPCTSTR pszAcceptTypes[] = {_T("text/*"), NULL};

	DWORD dwFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
									INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
									INTERNET_FLAG_KEEP_CONNECTION |
									INTERNET_FLAG_NO_AUTH |
									INTERNET_FLAG_NO_AUTO_REDIRECT |
									INTERNET_FLAG_NO_COOKIES |
									INTERNET_FLAG_NO_UI |
									INTERNET_FLAG_RELOAD;

	CGLInternetHandle hRequest = ::HttpOpenRequest(hConnection, _T("POST"), url.lpszUrlPath, NULL, NULL, pszAcceptTypes, dwFlags, 0);

	if (hRequest == NULL)
		RETURN(E_FAIL);

	CString strBoundary = GenerateBoundary();

	ATL::CString strRequest;

	for (POSITION pos = params.GetStartPosition(); pos != NULL; params.GetNext(pos))
	{
		CString strVar = params.GetKeyAt(pos);
		CString strVal = params.GetValueAt(pos);

		strRequest += _T("--") + strBoundary + _T("\r\n");
		strRequest += _T("Content-Disposition: form-data; name=\"") + strVar + _T("\"\r\n\r\n");
		strRequest += strVal + _T("\r\n");
	}

	strRequest += _T("--") + strBoundary + _T("--\r\n");

	TCHAR chLength[20];
	wsprintf(chLength, _T("%d"), strRequest.GetLength());

	CString strContentType = _T("Content-Type: multipart/form-data; boundary=") + strBoundary;
	::HttpAddRequestHeaders(hRequest, strContentType, -1, HTTP_ADDREQ_FLAG_ADD);

	CString strContentLength = _T("Content-Length: ") + CString(chLength);
	::HttpAddRequestHeaders(hRequest, strContentLength, -1, HTTP_ADDREQ_FLAG_ADD);

	if (!::HttpSendRequest(hRequest, NULL, 0, strRequest.GetBuffer(), strRequest.GetLength()))
		RETURN(E_FAIL);

	strResponse = _T("");

	DWORD dwBytesAvailable = 0;

	while (::InternetQueryDataAvailable(hRequest, &dwBytesAvailable, 0, 0) && dwBytesAvailable > 0)
	{
		char *pMessageBody = (char*)malloc(dwBytesAvailable + 1);

		DWORD dwBytesRead = 0;

		if (!::InternetReadFile(hRequest, pMessageBody, dwBytesAvailable, &dwBytesRead))
		{
			RETURN(E_FAIL);
		}

		if (dwBytesRead == 0)
			break;	// End of File.

		pMessageBody[dwBytesRead] = '\0';

		strResponse += CString(pMessageBody);

		free(pMessageBody);
	}

	return S_OK;
}

/**
 *
 */
HRESULT CGLHttpRequest::Parse(CString strResponse, TResponse& mapResponse)
{
	mapResponse.RemoveAll();

	while (!strResponse.IsEmpty())
	{
		ATL::CString strPart;
		int nDelimeter = strResponse.Find(_T("&"));

		if (nDelimeter == -1)
		{
			strPart = strResponse;
			strResponse.Empty();
		}
		else
		{
			strPart = strResponse.Left(nDelimeter);
			strResponse = strResponse.Mid(nDelimeter + 1);
		}

		int nEq = strPart.Find(_T("="));

		if (nEq == -1)
		{
			return E_FAIL;
		}

		CString strVar = strPart.Left(nEq);

		CString strVal = strPart.Mid(nEq + 1);

		mapResponse[strVar] = strVal;
	}

	return S_OK;
}