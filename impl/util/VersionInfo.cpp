/* ======================================================================
  @brief C++ class for managing module version info
  @author Helmut Steeb
  $Id: VersionInfo.cpp 3011 2008-11-29 09:37:12Z helmut $

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
#include "VersionInfo.h"
#include <assert.h>
#include "../util/UTF8.h"

bool CVersionInfo::ReadVersionFrom(const tstring &filename)
{
  _TCHAR *pFilename = const_cast<_TCHAR*>(filename.c_str());
  DWORD dummyVersionInfo;
  DWORD ret = ::GetFileVersionInfoSize(pFilename, &dummyVersionInfo);
  bool res = false;
  if (!ret) {
    DWORD err = GetLastError();
    MSGERROR("GetFileVersionInfoSize from " << T28(filename) << " failed with error " << err);
  }
  else {
    char *pBuf = new char[ret];
    assert(pBuf);
    if (!::GetFileVersionInfo(pFilename, 0, ret, pBuf)) {
      MSGERROR("GetFileVersionInfo from " << pFilename << " failed");
    }
    else {
      VS_FIXEDFILEINFO *pFi;
      UINT size;
      if (!VerQueryValue(pBuf, _T("\\"), reinterpret_cast<void**>(&pFi), &size)) {
        MSGERROR("VerQueryValue from " << T28(filename) << " failed");
      }
      else {
        mMain  = HIWORD(pFi->dwFileVersionMS);
        mSub   = LOWORD(pFi->dwFileVersionMS);
        mDraft = HIWORD(pFi->dwFileVersionLS);
        res = true;
      }
/*
Betreff: Re: Accessing Version Info
Datum: Fri, 7 Jul 2000 12:20:10 -0400
Von: "Craig Klementowski" <none@sendmenomaill.com>
Foren: microsoft.public.vc.language,microsoft.public.vc.mfc
Referenzen: <uQyOKrC6$GA.88@cppssbbsa05>

  if ((p->dwFileFlags & VS_FF_SPECIALBUILD) && VerQueryValue(buff,
TEXT("\\StringFileInfo\\040904B0\\SpecialBuild"), (void
**)&chBuf, &len))
  {
   fileVersion += "  Build: " ;
   fileVersion += chBuf ;
  }
*/
    }
    delete pBuf;
  }
  return res;
}

tstring CVersionInfo::Version()
{
  std::tostringstream os;
  // HS 2005-04-11: use (int) cast for Unicode version in which unsigned short is a character!
  os << (int) mMain << _T(".") << (int) mSub << _T(".") << (int) mDraft;
  return os.str();
}

unsigned short CVersionInfo::Main()
{
  return mMain;
}
  
unsigned short CVersionInfo::Sub()
{
  return mSub;
}

unsigned short CVersionInfo::Draft()
{
  return mDraft;
}

CVersionInfo::operator<(const CVersionInfo &versionInfo)
{
  return mMain < versionInfo.mMain || 
    (mMain == versionInfo.mMain && mSub < versionInfo.mSub);
}

