/* ----------------------------------------------------------------------
  Helmut Steeb
  Encoding utility routines.
  $Id: Encoding.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include "Encoding.h"

bool NEncoding::Widen(const tstring &s, wstring& res)
{
  UINT cp = CP_ACP;
  unsigned int resLen = MultiByteToWideChar(cp, /*dwFlags*/ 0, s.c_str(), s.length(), 0, 0);
  if (s.length() && !resLen) {
    MSGERROR("Widen conversion of " << s.length() << " chars failed with error " << GetLastError());
    res = L"";
    return false;
  }

  wchar_t* p = new wchar_t[resLen + 1];
  resLen = MultiByteToWideChar(cp, /*dwFlags*/ 0, s.c_str(), s.length(), p, resLen);
  if (s.length() && !resLen) {
    MSGERROR("Widen conversion of " << s.length() << " chars failed with error " << GetLastError());
    res = L"";
    return false;
  }
  res.assign(p, resLen);
  delete[] p;
  return true;
}

bool NEncoding::Narrow(const wstring &s, string &res)
{

  BOOL usedDefaultChar = 0;
  unsigned int resLen = WideCharToMultiByte(CP_ACP, 0, s.c_str(), s.length(), 0, 0, NULL, &usedDefaultChar);
  if (s.length() && !resLen) {
    MSGERROR("Narrow conversion of " << s.length() << " chars failed with error " << GetLastError());
    res = "";
    return false;
  }
  char* p = new char[resLen + 1];
  resLen = WideCharToMultiByte(CP_ACP, 0, s.c_str(), s.length(), p, resLen, NULL, &usedDefaultChar);
  if (usedDefaultChar) {
    MSGERROR("Narrow conversion to active codepage lost character(s), replaced by default character.");
  }
  res.assign(p, resLen);
  delete[] p;
  return true;
}


// === TEST ====================================================

#ifdef _TEST

bool NEncoding::SelfTest()
{

  bool bRes = true;
  MSGDEBUG("--- Start NEncoding::SelfTest ----------------------------");

  string orig = "abc";
  wstring wRes;
  bRes &= (NEncoding::Widen(orig, wRes) && L"abc" == wRes);

  wstring wOrig = L"abc";
  string res;
  bRes &= (NEncoding::Narrow(wOrig, res) && "abc" == res);

  MSGDEBUG("--- End NEncoding::SelfTest ----------------------------");
  return bRes;
}

#endif
