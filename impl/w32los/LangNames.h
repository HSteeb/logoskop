/* ----------------------------------------------------------------------
  Helmut Steeb
  Module for language names
  $Id: LangNames.h 3853 2009-08-26 13:50:27Z helmut $

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
#if !defined(_LANGNAMES_H__INCLUDED_)
#define _LANGNAMES_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace NLangNames {

  const char *GetLongForNiso(const char *nisoName);
  const char *GetNisoForLong(const char *longName);
  const char *GetLongForIso(const char *nisoName);
  const char *GetIsoForLong(const char *longName);
  const char *GetIsoForLangID(LANGID langID);

  // MakeNisoDisplayName("GER") = "Deutsch GER"
  // MakeNisoDisplayName("xyz") = "xyz"
  string MakeNisoDisplayName(const string &shortName);

  // MakeIsoDisplayName("de") = "Deutsch de"
  // MakeIsoDisplayName("xy") = "xy"
  string MakeIsoDisplayName(const string &shortName);

  // ExtractNisoFromNisoDisplayName("Deutsch GER") = "GER"
  // ExtractNisoFromNisoDisplayName("Deutsch ger") = "GER"
  // ExtractNisoFromNisoDisplayName("xyz") = "xyz"
  string ExtractNisoFromNisoDisplayName(const string &nisoDisplayName);

  // ExtractIsoFromIsoDisplayName("Deutsch de") = "de"
  // ExtractIsoFromIsoDisplayName("Deutsch DE") = "de"
  // ExtractIsoFromIsoDisplayName("xy") = "xy"
  string ExtractIsoFromIsoDisplayName(const string &isoDisplayName);

  // GetDefaultLangNisoName() == "eng"
  const string GetDefaultLangNisoName();

};


#endif // _LANGNAMES_H__INCLUDED_
