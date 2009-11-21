/* ----------------------------------------------------------------------
  Helmut Steeb
  String utility routines.
  $Id: StringUtil.cpp 3853 2009-08-26 13:50:27Z helmut $

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
#include "StringUtil.h"

tstring NStringUtil::Pop(const tstring &sep, tstring &s)
{
  string::size_type pos = s.find(sep);
  if (pos == string::npos) {
    tstring res = s;
    s = "";
    return res;
  }
  tstring res = s.substr(0, pos);
  s.erase(0, pos+1);
  return res;
}

void NStringUtil::Split(const tstring &sep, const tstring &s, std::vector<tstring> &result)
{
  result.empty();
  tstring tmp(s);
  while (!tmp.empty()) {
    result.push_back(Pop(sep, tmp));
  }
}

tstring NStringUtil::Join(const tstring &sep, const std::vector<tstring> &src)
{
  tstring result;
  std::vector<tstring>::const_iterator it;
  for (it = src.begin(); it != src.end(); ++it) {
    if (!result.empty()) {
      result += sep;
    }
    result += *it;
  }
  return result;
}


tstring NStringUtil::Replace(const tstring &src, const tstring &oldVal, const tstring &newVal)
{
  // What a beauty in $src =~ s/$oldVal/$newVal/g :-(

  if (oldVal.empty() || oldVal == newVal) {
    return src;
  }
  string res(src);
  string::size_type pos = res.length() - 1;
  while (pos != string::npos) {
    // string("bbb").rfind("b", 2) => 2
    pos = res.rfind(oldVal, pos);
    if (pos != string::npos) {
      res.replace(pos, oldVal.length(), newVal);
      --pos;
    }
  }
  return res;
}

// === TEST ====================================================

#ifdef _TEST

bool NStringUtil::SelfTest()
{

  bool bRes = true;
  MSGDEBUG("--- Start NStringUtil::SelfTest ----------------------------");

  tstring orig = "lang;bible;url";

  tstring s = "lang;bible;url";
  bRes &= ("lang"  == NStringUtil::Pop(";", s));
  bRes &= ("bible" == NStringUtil::Pop(";", s));
  bRes &= ("url"   == NStringUtil::Pop(";", s));
  bRes &= (""      == NStringUtil::Pop(";", s));
  bRes &= (""      == NStringUtil::Pop(";", s));

  std::vector<tstring> vec;
  NStringUtil::Split(";", orig, vec);
  bRes &= (3 == vec.size());
  bRes &= ("lang"  == vec.at(0));
  bRes &= ("bible" == vec.at(1));
  bRes &= ("url"   == vec.at(2));

  bRes &= (NStringUtil::Replace("ab", "a", "c") == "cb");
  bRes &= (NStringUtil::Replace("bbb", "b", "c") == "ccc");
  bRes &= (NStringUtil::Replace("bbb", "", "c") == "bbb");
  bRes &= (NStringUtil::Replace("bbb", "b", "b") == "bbb");
  bRes &= (NStringUtil::Replace("", "b", "c") == "");
  // become shorter
  bRes &= (NStringUtil::Replace("ab", "ab", "c") == "c");
  bRes &= (NStringUtil::Replace("xaby", "ab", "c") == "xcy");
  // become longer
  bRes &= (NStringUtil::Replace("ab", "a", "ab") == "abb");
  bRes &= (NStringUtil::Replace("bbb", "b", "bb") == "bbbbbb");
  // strange \\ stuff
  bRes &= (NStringUtil::Replace("ab\\nNext", "\\n", "\r\n") == "ab\r\nNext");

  MSGDEBUG("--- End NStringUtil::SelfTest ----------------------------");
  return bRes;
}

#endif
