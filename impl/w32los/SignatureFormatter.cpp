/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop ISignatureFormatter class.
  $Id: SignatureFormatter.cpp 3932 2009-09-28 21:58:25Z helmut $

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
#include <assert.h>
#include "SignatureFormatter.h"
#include "../LosFile/LosFile.h"
#include "../util/utf8.h"
#include "../util/Encoding.h"
#include "../util/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// === ISignatureFormatter ===

ISignatureFormatter::~ISignatureFormatter()
{
}

// === CSignatureFormatter ===

class CSignatureFormatter : public ISignatureFormatter 
{
public:
  CSignatureFormatter();
  ~CSignatureFormatter();
  SetAsUtf8(bool bAsUtf8);
  virtual tstring Format(OLosung *pLosung, const tstring &signatureTemplate, const tstring &lineSeparator);
  virtual bool ContainsAnyKeyword(const tstring &signatureTemplate);
protected:
  virtual bool Replace(wstring &s, const wstring &key, const wstring &value);
  virtual wstring Widen(const tstring &s);
  virtual tstring Narrow(const wstring &s);
private:
  bool mbAsUtf8;
};



CSignatureFormatter::CSignatureFormatter()
: mbAsUtf8(false)
{
}


CSignatureFormatter::~CSignatureFormatter()
{
}


CSignatureFormatter::SetAsUtf8(bool bAsUtf8)
{
  mbAsUtf8 = bAsUtf8;
}



#define BUFSIZE 65535

wstring CSignatureFormatter::Widen(const tstring &s)
{
  if (mbAsUtf8) {
    return NUTF8::ToUTF16(s);
  }
  wstring res;
  NEncoding::Widen(s, res);
  return res;
}

tstring CSignatureFormatter::Narrow(const wstring &s)
{
  if (mbAsUtf8) {
    return NUTF8::FromUTF16nz(s);
  }
  string res;
  NEncoding::Narrow(s, res);
  return res;
}

bool CSignatureFormatter::ContainsAnyKeyword(const tstring &signatureTemplate)
{
  wstring result(Widen(signatureTemplate));
  return 
      wstring::npos != result.find(KEY_LOSUNG)
   || wstring::npos != result.find(KEY_TL)
   || wstring::npos != result.find(KEY_OT)
   || wstring::npos != result.find(KEY_NT)
   ;
}

bool CSignatureFormatter::Replace(wstring &s, const wstring &key, const wstring &value)
{
  unsigned int pos;
  bool bModified = false;
  while (wstring::npos != (pos = s.find(key))) {
    s.erase(pos, key.length());
    s.insert(pos, value);
    bModified = true;
  }
  return bModified;
}

tstring CSignatureFormatter::Format(OLosung *pLosung, const tstring &signatureTemplate, const tstring &lineSeparator)
{
  wstring result(Widen(signatureTemplate));

  OTestimony* pParol1 = 0;
  OTestimony* pParol2 = 0;
  if (pLosung) {
    OLosung::OIter it = pLosung->Begin();
    if (it != pLosung->End()) {
      pParol1 = *it;
      ++it;
      if (it != pLosung->End()) {
        pParol2 = *it;
      }
    }
  }

  // avoid unnecessary computation of AsString etc. - first check whether keyword is contained
  if (pLosung) {
    if (wstring::npos != result.find(KEY_LOSUNG)) { Replace(result, KEY_LOSUNG, pLosung->AsString()                ); }
    if (wstring::npos != result.find(KEY_TL))     { Replace(result, KEY_TL,     pLosung->TitleLine().c_str()       ); }
    if (wstring::npos != result.find(KEY_OT))     { Replace(result, KEY_OT,     pParol1 ? pParol1->AsString() : L""); }
    if (wstring::npos != result.find(KEY_NT))     { Replace(result, KEY_NT,     pParol2 ? pParol2->AsString() : L""); }
  }
  else {
    Replace(result, KEY_LOSUNG, L"");
    Replace(result, KEY_TL,     L"");
    Replace(result, KEY_OT,     L"");
    Replace(result, KEY_NT,     L"");
  }
  tstring nresult(Narrow(result));
  if (lineSeparator != "\n") {
    nresult = NStringUtil::Replace(nresult, "\n", lineSeparator);
  }
  return nresult;
}



// === NSignatureFormatter ===

ISignatureFormatter &NSignatureFormatter::MakeSignatureFormatter()
{
  ISignatureFormatter *pSignatureFormatter = new CSignatureFormatter();
  assert(pSignatureFormatter);
  return *pSignatureFormatter;
}
