/* ----------------------------------------------------------------------
  Helmut Steeb 
  Internet session handling including proxy with user/wpass input.

  Authentication with HTTP-header:
  cf. RFC 2616 5.1.2 Request-URI
  cf. RFC 2617 2 Basic Authentication Scheme

  $Id: ProxySession.cpp 3014 2008-11-29 10:19:16Z helmut $

The contents of this file are subject to the Mozilla Public License Version 1.1
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
the specific language governing rights and limitations under the License.

The Original Code is Logoskop.

The Initial Developer of the Original Code is Helmut Steeb.
Portions created by Helmut Steeb are Copyright (C) 2008 Helmut Steeb.
All Rights Reserved.

Contributor(s):

Alternatively, the contents of this file may be used under the terms of the
GNU General Public License (the "GPL"), in which case the provisions of
GPL are applicable instead of those above. If you wish to allow use
of your version of this file only under the terms of the GPL and not
to allow others to use your version of this file under the MPL, indicate your
decision by deleting the provisions above and replace them with the notice and
other provisions required by the GPL. If you do not delete the
provisions above, a recipient may use your version of this file under either the
MPL or the GPL.
---------------------------------------------------------------------- */

#include "stdafx.h"
#include <assert.h>
#include <afxinet.h>
#include "IUserPassDlg.h"
#include "ProxySession.h"
#include "ErrorCodes.h"
#include "../util/FileUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === IProxySession ===

IProxySession::~IProxySession()
{
}

// === OProxySession ===

class OProxySession : public IProxySession 
{
public:
  OProxySession::OProxySession(const string &appNameVersion, IUserPassDlg &userPassDlg);
  virtual OProxySession::~OProxySession();
  
  // IProxySession
  virtual void SetProxy(tUseProxy useProxy, const string &proxy = "", bool bForceNewSession = false);
  virtual tUseProxy GetUseProxy();
  virtual string GetProxy();
  virtual void SetTimeout(const unsigned int timeout);

  virtual void CloseSession();
  
  int Get(const string &url, string &result);
  int Post(const string &url, const string &data, string &result);

protected:
  int _ProvideSession();
  int _Request       (const string &url, int nVerb, const string &data, string &result);
  int _ProxiedRequest(const string &url, int nVerb, const string &data, string &result);

private:
  string mAppNameVersion;
  IUserPassDlg &mUserPassDlg;
  CInternetSession *mpInternetSession;
  tUseProxy mUseProxy;
  string mProxy; // address of http proxy (optional)
  string mProxyUsername;  // username for proxy login (optional; session only)
  string mProxyPassword;  // password for proxy login (optional; session only)

  unsigned int mTimeout; // 1/1000 sec.
};



OProxySession::OProxySession(const string &appNameVersion, IUserPassDlg &userPassDlg)
: mAppNameVersion(appNameVersion)
, mUserPassDlg(userPassDlg)
, mpInternetSession(0)
, mUseProxy(proxyPreconfig)
, mTimeout(0)
{
}

OProxySession::~OProxySession()
{
  CloseSession();
}

int OProxySession::_ProvideSession()
{
  if (!mpInternetSession) {
    try {
      DWORD dwAccessType = 0;
      const char *pProxy = 0;

      switch (mUseProxy) {
      case proxyPreconfig:
        dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
        break;
      case proxyDirect:
        dwAccessType = INTERNET_OPEN_TYPE_DIRECT;
        break;
      case proxyManual:
        dwAccessType = INTERNET_OPEN_TYPE_PROXY;
        pProxy = mProxy.c_str();
        break;
      default:
        assert(0);
      }
      // CInternetSession( 
      //   LPCTSTR pstrAgent = NULL, 
      //   DWORD dwContext = 1, 
      //   DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG, 
      //   LPCTSTR pstrProxyName = NULL, 
      //   LPCTSTR pstrProxyBypass = NULL, 
      //   DWORD dwFlags = 0 );
      mpInternetSession = new CInternetSession(mAppNameVersion.c_str(), 
          /*dwContext*/1, dwAccessType, pProxy);

      // HS 010525: standard timeout seems 300000 msec. = 5min. as from:
      // DWORD timeout; mpSession->QueryOption(INTERNET_OPTION_CONNECT_TIMEOUT, timeout);
      // Since during the timeout, the user only sees a waiting mouse cursor,
      // allow caller to shorten the timeout:
      if (mTimeout) {
        mpInternetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, mTimeout);
      }
    }
    catch (CInternetException* pEx)
    {
      MSGERROR("Provide session: Internet error " << pEx->m_dwError);
      return pEx->m_dwError;
    }
  }
  return 0;
}


/* 2008-10-31 HS: not needed - use OpenURL instead

// _FileDownload:
// returns true iff. url contains file:/// prefix.
// In this case, the rest of url is assumed to be a file path (for fopen),
// and its contents is read into result.

bool OProxySession::_FileDownload(const string &url, string &result)
{
  const string filePrefix("file:///");
  if (filePrefix == url.substr(0, filePrefix.length())) {
    const string path = url.substr(filePrefix.length());
    result = FileUtil::Read(path);
    return true;
  }
  return false;
}
*/

int OProxySession::_Request(const string &url, int nVerb, const string &data, string &result)
{
  int res = _ProvideSession();
  if (res) {
    return res;
  }

  // HS 2005-11-05: must delete allocated pointers outside try/catch,
  // otherwise there are memory leaks in case an exception is thrown.
  CHttpConnection* pConnection = 0;
  CFile* pFile = 0;
  DWORD dwRet = 0;

  try
  {
    // "http://bible2.net/service/TheWord/twd11"
    const char *p = url.c_str();
    DWORD serviceType;
    CString strServerName;
    INTERNET_PORT nPort;
    CString strObject;
    // AfxParseURL: 
    // returns "0 if it is empty or does not contain a known Internet service type"
    if (!AfxParseURL(p, serviceType, strServerName, strObject, nPort)) {
      MSGERROR("OProxySession::_Request: URL " << url << " is empty or does not contain a known Internet service type");
      return DOWNLOAD_URL_ERROR;
    }
    if (serviceType != AFX_INET_SERVICE_HTTP && serviceType != AFX_INET_SERVICE_FILE) {
      MSGERROR("OProxySession::_Request: Unsupported protocol in URL " << url << " - need http:// or file:///");
      return DOWNLOAD_PROTOCOL_NOT_SUPPORTED;
    }


#define BUFSIZE 1024
    unsigned long len = BUFSIZE;
    char szBuf[BUFSIZE+1];

    if (serviceType == AFX_INET_SERVICE_FILE) {
      pFile = mpInternetSession->OpenURL(url.c_str(), 1, INTERNET_FLAG_TRANSFER_BINARY);
      MSGINFO("Reading file " << url);
    }
    else {
      pConnection = mpInternetSession->GetHttpConnection(
        strServerName, nPort, 
        mProxyUsername.empty()? 0 : mProxyUsername.c_str(), 
        mProxyPassword.empty()? 0 : mProxyPassword.c_str());
      
      //  OpenRequest(nVerb, pstrObjectName, pstrReferer, dwContext, ppstrAcceptTypes, pstrVersion, dwFlags)
      CHttpFile* pHttpFile = pConnection->OpenRequest(nVerb, strObject, 0, 1, 0, "HTTP/1.1",
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
      if (pHttpFile) {
        string headers;
        if (nVerb == CHttpConnection::HTTP_VERB_POST && data.length() == 0) {
          // 2008-11-22 HS:
          // with my ParolEdit Firefox extension,
          // HTTP POST with data length 0 was blocked with HTTP 411 by some proxy.
          // Adding header "Content-Length: 0" helped :-(
          // => be defensive: add header "Content-Length" here.
          // I have not tested the case data.length()==0, 
          // but hard-coded setting of Content-Length for data.length() > 0 worked fine (without proxy).
          headers = "Content-Length: 0";
        }
        pHttpFile->SendRequest(headers.c_str(), headers.length(), (void*) data.c_str(), data.length());

        // from here on, should pHttpFile->Close() to avoid "Warning: destroying an open CInternetFile with handle ..."
        pHttpFile->QueryInfoStatusCode(dwRet);
        MSGDEBUG("Response: HTTP status code " << dwRet);
        // do not use QueryInfo(dwInfoLevel, CString, ...) - it asserts 
        //   ASSERT(dwInfoLevel <= HTTP_QUERY_MAX && dwInfoLevel >= 0);
        // instead of
        //   ASSERT((HTTP_QUERY_HEADER_MASK & dwInfoLevel) <= HTTP_QUERY_MAX && dwInfoLevel != 0);
        // as in QueryInfo(DWORD dwInfoLevel, LPVOID lpvBuffer...).
        // HTTP Response Status Codes: VC98\Include\WinInet.h
        if (pHttpFile->QueryInfo(HTTP_QUERY_RAW_HEADERS, szBuf, &len)) {
          MSGDEBUG("Response headers:\n" << szBuf);
        }
        if (dwRet == HTTP_STATUS_OK) {
          dwRet = 0; // success
          pFile = pHttpFile; // remember for deletion
        }
        else {
          pHttpFile->Close();
          delete pHttpFile;
        }
      }
      // must delete pFile (if != 0)
    }        

    if (dwRet == 0 && pFile) {
      // read data (twd files are about 200K...)
      result.erase();
      UINT nRead; 
      while (0 < (nRead = pFile->Read(szBuf, BUFSIZE-1))) {
        result += string(szBuf, nRead);
      }
    }
  }
  catch (CInternetException* pEx)
  {
    TCHAR szErr[1024];
    pEx->GetErrorMessage(szErr, 1024);
    MSGERROR("OProxySession::_Request: Internet error (" << pEx->m_dwError << ") " << szErr);
    dwRet = pEx->m_dwError;
    pEx->Delete();
  }
  catch (CFileException* pEx)
  {
    TCHAR szErr[1024];
    pEx->GetErrorMessage(szErr, 1024);
    MSGERROR("OProxySession::_Request: CFileException error (" << pEx->m_cause << ") " << szErr);
    dwRet = pEx->m_lOsError; // avoid conflicts with CInternetException::m_dwError (?)
    pEx->Delete();
  }
  catch (...)
  {
    MSGERROR("OProxySession::_Request: other error!");
    dwRet = 1;
  }


  // finally:
  if (pFile) {
    pFile->Close(); // avoid TRACE message in destructor (inet.cpp)
    delete pFile;
  }

  if (pConnection) {
    pConnection->Close(); // avoid TRACE message in destructor (inet.cpp)
    delete pConnection;
  }

  return dwRet;
}

// === OProxySession public methods ===

void OProxySession::SetProxy(tUseProxy useProxy, const string &proxy, bool bForceNewSession)
{
  if (bForceNewSession || useProxy != mUseProxy || proxy != mProxy) {
    // also erase username and password when proxy changed
    // (otherwise, user has no chance to change it)
    mProxyUsername.erase();
    mProxyPassword.erase();
  
    // close session to force re-open with new proxy
    // (on initial SetProxy, mpInternetSession is still 0 -> no special handling)
    CloseSession();
  }

  mUseProxy = useProxy;
  mProxy = proxy;
}
  
tUseProxy OProxySession::GetUseProxy()
{
  return mUseProxy;
}

string OProxySession::GetProxy()
{
  return mProxy;
}

void OProxySession::SetTimeout(const unsigned int timeout)
{
  mTimeout = timeout;
}

void OProxySession::CloseSession()
{
  if (mpInternetSession) {
    mpInternetSession->Close();
    delete mpInternetSession;
    mpInternetSession = 0;
    //mRetryCount = 0;
  }
}

int OProxySession::_ProxiedRequest(const string &url, int nVerb, const string &data, string &result)
{
  MSGDEBUG("Requesting " << url);
  int res = _Request(url, nVerb, data, result);
  // HS 010720: changing "if" to "while" here
  //   suppresses the "error 407" message
  //   (password dialog comes up without notice until password correct or cancel).
  while (res == HTTP_STATUS_PROXY_AUTH_REQ) { // 407
    // skip user/pass dialog if already got (successful) user/pass
    if (mProxyUsername.empty() || mProxyPassword.empty()) {
      mUserPassDlg.SetUser(mProxyUsername);
      mUserPassDlg.SetPassword("");
      if (IDOK != mUserPassDlg.AskUserPass()) {
        return DOWNLOADMGR_CANCEL; // return code for "cancel" <--OErrorMsgFormatter()
      }
      // store as session-wide member variables, but not persistent (for security)
      mProxyUsername = mUserPassDlg.GetUser();
      mProxyPassword = mUserPassDlg.GetPassword();
    }
    CWaitCursor wait; // redisplay in case we queried for password
    res = _Request(url, nVerb, data, result);
    if (res) {
      // oops, operation failed - reset user/pass to give them a new chance
      // after they read the error message.
      // Cannot decide here whether user/pass was ok (so it should be kept),
      // or whether user/pass caused the operation to fail,
      // so reset not only for (res==HTTP_STATUS_PROXY_AUTH_REQ) but any failure.
      mProxyUsername.erase();
      mProxyPassword.erase();
    }
  }
  return res;
}

int OProxySession::Get(const string &url, string &result)
{
  return _ProxiedRequest(url, CHttpConnection::HTTP_VERB_GET, "", result);
}

int OProxySession::Post(const string &url, const string &data, string &result)
{
  return _ProxiedRequest(url, CHttpConnection::HTTP_VERB_POST, data, result);
}



// === NProxySession ===

IProxySession &NProxySession::MakeProxySession(const string &appNameVersion, IUserPassDlg &userPassDlg)
{
  IProxySession *pProxySession = new OProxySession(appNameVersion, userPassDlg);
  assert(pProxySession);
  return *pProxySession;
}
