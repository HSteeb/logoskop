/* ----------------------------------------------------------------------
  Helmut Steeb 
  Internet session handling including proxy with user/pass input.
  $Id: ProxySession.h 3014 2008-11-29 10:19:16Z helmut $

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

#if !defined(_PROXYSESSION_INCLUDED_)
#define _PROXYSESSION_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#define DOWNLOAD_SUCCESS 0
#define DOWNLOAD_PROTOCOL_NOT_SUPPORTED 1
#define DOWNLOAD_URL_ERROR 2

class IUserPassDlg;

enum tUseProxy {
  proxyPreconfig = 'P',  // cf. DEFAULT_USEPROXY in ProfileDef.h!
  proxyDirect    = 'D',
  proxyManual    = 'M'
};

class IProxySession {
public:
  virtual ~IProxySession();

  virtual void SetProxy(tUseProxy useProxy, const string &proxy = "", bool bForceNewSession = false) = 0;
  virtual tUseProxy GetUseProxy() = 0;
  virtual string GetProxy() = 0;
  virtual void SetTimeout(const unsigned int timeout) = 0;

  virtual void CloseSession() = 0;

  // Get:
  // returns 0 on success, or error code from windows API.
  virtual int Get(const string &url, string &result) = 0;

  // Post:
  // returns 0 on success, or error code from windows API.
  virtual int Post(const string &url, const string &data, string &result) = 0;
};

namespace NProxySession {
  // resulting IProxySession must be freed by caller
  IProxySession &MakeProxySession(const string &appNameVersion, IUserPassDlg &userPassDlg);
};


#endif
