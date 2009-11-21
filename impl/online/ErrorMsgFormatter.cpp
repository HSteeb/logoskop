/* ----------------------------------------------------------------------
  Helmut Steeb 
  Format numeric error code to message.

  $Id: ErrorMsgFormatter.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include "../w32los/Resource.h"
#include <afxinet.h>
#include <assert.h>
#include "IErrorMsgFormatter.h"
#include "ErrorMsgFormatter.h"
#include "ErrorCodes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === OErrorMsgFormatter ===

class OErrorMsgFormatter : public IErrorMsgFormatter {
public:
  OErrorMsgFormatter(const string &baseUrl);
  virtual ~OErrorMsgFormatter();

  virtual void FormatDownloadError(UINT msgID, const int err, CString &buf);
  virtual void FormatInstallError(const int err, CString &buf);

private:
  const tstring mBaseUrl;
};


OErrorMsgFormatter::OErrorMsgFormatter(const string &baseUrl)
: mBaseUrl(baseUrl)
{
}

OErrorMsgFormatter::~OErrorMsgFormatter()
{
}

void OErrorMsgFormatter::FormatDownloadError(UINT msgID, const int err, CString &buf)
{
  // Error codes see:
  // MSDN Library Visual Studio 6.0
  //   Visual InterDev-Dokumentation
  //     Referenz
  //       Hyperlinkansicht-Referenz
  // "Weitere Informationen unter http://www.microsoft.com/intdev/sdk/docs/wininet"
  buf.LoadString(msgID);

  UINT errID = 0;
  CString errMsg;
  bool bAuthorHint = false;

  switch (err) {
  case DOWNLOADMGR_CANCEL: // return code for "cancel" -->OProxySession::Get()
    return;

  case DOWNLOAD_PROTOCOL_NOT_SUPPORTED:
  case DOWNLOAD_URL_ERROR:
    errMsg.Format(IDS_INVALID_URL, mBaseUrl.c_str());
    break;

  case DOWNLOADMGR_ERROR:
    // HS 010616: maybe format error, or interspersing HTML page from provider, or...
    // But I just sent the ressource texts to the translators 
    // => use message in English + author hint
    errMsg = "Format error!\n\n"; 
    bAuthorHint = true;
    break;

  case ERROR_INTERNET_TIMEOUT: // 12002: The request has timed out. (HS 010625 possible reason: wrong proxy!)
  case ERROR_INTERNET_CANNOT_CONNECT: // 12029: Der Versuch, eine Verbindung zum Server herzustellen, schlug fehl.
    errID = IDS_NOSESSION;
    break;

  case ERROR_INTERNET_NAME_NOT_RESOLVED: // 12007: "Der Servername konnte nicht aufgelöst werden."
    errMsg.Format(IDS_SERVER_NOT_FOUND, mBaseUrl.c_str());
    bAuthorHint = true;
    break;

  case HTTP_STATUS_NOT_FOUND: // 404
    // (HS 011001: 404 occurs if correct proxy address but wrong port entered (?))
    errMsg.Format(IDS_URL_NOT_FOUND, mBaseUrl.c_str());
    bAuthorHint = true;
    break;

  default:
    if (err >= HTTP_STATUS_AMBIGUOUS /*300*/ && err <= HTTP_STATUS_LAST /*505*/) {
      errMsg.Format(IDS_HTTP_STATUS, err);
      bAuthorHint = true;
    }
    else {
      errMsg.Format(IDS_INTERNET_ERROR);
      bAuthorHint = true;
    }
    break;
  }
  if (errMsg.IsEmpty()) { // allow formatting in switch above
    errMsg.LoadString(errID);
  }

  buf += errMsg;

  if (bAuthorHint) {
    CString authorHint;
    authorHint.LoadString(IDS_AUTHOR);
    buf += authorHint;
  }

  MSGERROR("CDownloadManager: error " << err << " from Download: " << LPCTSTR(buf));
}

void OErrorMsgFormatter::FormatInstallError(const int err, CString &buf)
{
  buf = "";
}


// === NErrorMessageFormatter ===

IErrorMsgFormatter &NErrorMsgFormatter::MakeErrorMsgFormatter(const string &baseUrl)
{
  IErrorMsgFormatter *pErrorMsgFormatter = new OErrorMsgFormatter(baseUrl);
  assert(pErrorMsgFormatter);
  return *pErrorMsgFormatter;
}

