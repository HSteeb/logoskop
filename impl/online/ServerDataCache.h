/* ----------------------------------------------------------------------
  Helmut Steeb
  Cache for server file list.
  $Id: ServerDataCache.h 3014 2008-11-29 10:19:16Z helmut $

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
#if !defined(AFX_SERVERDATACACHE_H__INCLUDED_)
#define AFX_SERVERDATACACHE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IProxySession;
class OTwdFileDescList;
class OTwdInfoList;

class IServerDataCache {
public:
  virtual ~IServerDataCache() {};

  virtual bool Fill() = 0;
  virtual OTwdFileDescList& GetFileDescList() = 0;
  virtual OTwdInfoList& GetInfoList() = 0;
  virtual void Clear() = 0;
};

namespace NServerDataCache {
  IServerDataCache& Make(IProxySession& proxySession, const tstring& listUrl);
}

#endif
