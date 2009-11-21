/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop ISignatureFormatter class.
  $Id: SignatureFormatter.h 3014 2008-11-29 10:19:16Z helmut $

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

#if !defined(AFX_SIGNATUREFORMATTER_H__INCLUDED_)
#define AFX_SIGNATUREFORMATTER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OLosung;

// === Keywords ===

#define KEY_LOSUNG L"%THEWORD;"
#define KEY_TL     L"%TL;"
#define KEY_OT     L"%PAROL1;"
#define KEY_NT     L"%PAROL2;"

#define TKEY_LOSUNG _T("%THEWORD;")
#define TKEY_TL     _T("%TL;")
#define TKEY_OT     _T("%PAROL1;")
#define TKEY_NT     _T("%PAROL2;")


class ISignatureFormatter {
public:
  virtual ~ISignatureFormatter();

  virtual SetAsUtf8(bool bAsUtf8) = 0;
  // Format:
  // replaces keywords by values from *pLosung; or by empty strings if pLosung==0.
  virtual tstring Format(OLosung *pLosung, const tstring &signatureTemplate, const tstring &lineSeparator) = 0;
  virtual bool ContainsAnyKeyword(const tstring &signatureTemplate) = 0;
};

namespace NSignatureFormatter {
  // resulting ISignatureFormatter must be freed by caller
  ISignatureFormatter &MakeSignatureFormatter();
};

#endif
