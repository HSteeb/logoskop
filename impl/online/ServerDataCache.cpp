/* ----------------------------------------------------------------------
  Helmut Steeb
  Cache for server file list and server info list.
  $Id: ServerDataCache.cpp 3924 2009-09-26 17:37:12Z helmut $

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
====================================================================== */

#include "stdafx.h"
#include "../w32los/Resource.h"
#include <assert.h>

#include "../filedesc/TwdFileDesc.h"
#include "../filedesc/TwdFileDescList.h"
#include "../filedesc/TwdInfoList.h"
#include "../filedesc/TwdFileDescParser.h"
#include "ProxySession.h"
#include "IErrorMsgFormatter.h"
#include "ErrorCodes.h"
#include "ErrorMsgFormatter.h"
#include "ServerDataCache.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === OServerDataCache declaration ===

class OServerDataCache : public IServerDataCache
{
public:
  OServerDataCache(
    IProxySession& proxySession, 
    const tstring& listUrl
  );
  ~OServerDataCache();

  virtual bool Fill();
  virtual OTwdFileDescList& GetFileDescList();
  virtual OTwdInfoList& GetInfoList();
  virtual void Clear();

protected:
  bool _ProvideServerData();

private:
  IProxySession&   mProxySession;
  tstring          mListUrl;
  OTwdFileDescList mServerFileDescList;
  OTwdInfoList     mServerInfoList;
  CTime            mServerDataTime;
};

// === OServerDataCache implementation ===

OServerDataCache::OServerDataCache(
  IProxySession& proxySession, 
  const tstring& listUrl
)
: mProxySession(proxySession)
, mListUrl(listUrl)
{
}

OServerDataCache::~OServerDataCache()
{
  Clear();
}

// === public ===

bool OServerDataCache::Fill()
{
  // cache mServerFileDescList + mServerInfoList: read only once a day
  if (0 != mServerFileDescList.Size() || 0 != mServerInfoList.Size()) {
    CTime now = CTime::GetCurrentTime();
    if  (mServerDataTime.GetDay()   == now.GetDay()
      && mServerDataTime.GetMonth() == now.GetMonth()
      && mServerDataTime.GetYear()  == now.GetYear()) {
      // now is same date as when file lists were filled 
      // -> use existing lists.
      return true;
    }
  }

  // get data from server (for all years)
  MSGDEBUG("Reading data from server " + mListUrl);
  string serverResult;

  int res = mProxySession.Get(mListUrl, serverResult);
  if (!res) {
    CWaitCursor wait; // redisplay in case Get queried for password

    OTwdFileDescParser parser;
    parser.Parse(serverResult, mServerFileDescList, mServerInfoList);
  
    if (0 != mServerFileDescList.Size() || 0 != mServerInfoList.Size()) {
      // remember date of getting lists
      mServerDataTime = CTime::GetCurrentTime();
      return true;
    }
    // list was empty -> error
    res = DOWNLOADMGR_ERROR;
  }
  // assert(res)

  IErrorMsgFormatter& fmt = NErrorMsgFormatter::MakeErrorMsgFormatter(mListUrl);
  CString buf;
  fmt.FormatDownloadError(IDS_GETLIST_FAILED, res, buf);
  MSGERROR(buf);
  AfxMessageBox(buf);
  delete &fmt;

  return false;
}


OTwdFileDescList& OServerDataCache::GetFileDescList()
{
  return mServerFileDescList;
}

OTwdInfoList& OServerDataCache::GetInfoList()
{
  return mServerInfoList;
}

void OServerDataCache::Clear()
{
  mServerFileDescList.Clear();
  mServerInfoList.Clear();
  // no need to reset mServerDataTime since list is empty now
}


// === NServerDataCache ===

IServerDataCache &NServerDataCache::Make(
  IProxySession& proxySession, 
  const tstring& listUrl)
{
  IServerDataCache *pServerDataCache = new OServerDataCache(proxySession, listUrl);
  assert(pServerDataCache);
  return *pServerDataCache;
}
