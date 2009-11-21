/* ----------------------------------------------------------------------
  Helmut Steeb 
  String utility routines.
  $Id: StringUtil.h 3853 2009-08-26 13:50:27Z helmut $

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
#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include "../util/tstring.h"

namespace NStringUtil {  
  
  // prefix = Pop(sep, s):
  // - if sep is substring of s:
  //   returns the part before the first occurrence of sep,
  //   with the part + sep removed from s
  // - otherwise:
  //   returns the value of the input parameter s,
  //   with output parameter s made empty.
  // 
  // s = tstring("a;b;c"); Pop(";", s) => tstring("a")     && s = tstring("b;c")
  // s = tstring("a;b;c"); Pop("-", s) => tstring("a;b;c") && s = tstring("")
  tstring Pop(const tstring &sep, tstring &s);
  
  // Vector = Split(sep, s):
  // s = tstring("a;b;c"); Split(";", s) => [tstring("a"), tstring("b"), tstring("c")]
  // s = tstring("a;b;c"); Split("-", s) => [tstring("a;b;c")]
  void Split(const tstring &sep, const tstring &s, std::vector<tstring> &result);

  tstring Join(const tstring &sep, const std::vector<tstring> &result);

  tstring Replace(const tstring &src, const tstring &oldVal, const tstring &newVal);
}


// === TEST ====================================================

#ifdef _TEST
namespace NStringUtil {
  bool SelfTest();
}
#endif


#endif
