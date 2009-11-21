/* ----------------------------------------------------------------------
  Helmut Steeb
  Stores document for one language/bible.
  $Id: LosDoc.h 3932 2009-09-28 21:58:25Z helmut $

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

#if !defined(_LOSDOC_H_)
#define _LOSDOC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OLosFile;
class OLosung;

class OLosDoc : public CObject
{
public:
  OLosDoc(OLosFile &losFile, const tstring &langBible);
  virtual ~OLosDoc();

  OLosFile &LosFile();

  const tstring LangBible() const;
  int Year() const; // year from the LosFile

  // SetDate:
  // HasCurrentLosung()/CurrentLosung will provide the Losung for date (if it exists)
  void SetDate(CTime &date);

  OLosung *GetLosungForDate(const CTime &date) const;
  bool HasCopyright() const;
  wstring GetCopyright() const;
  wstring GetFileVersion() const;
  // GetText:
  // line-breaks are L"\n"
  wstring GetText() const;

  // === CurrentLosung ===

  bool HasCurrentLosung() const;
  OLosung &CurrentLosung() const;

private:

  // text members
  OLosFile &mLosFile;
  OLosung *mpCurrentLosung; // non 0 if file loaded, and OLosung for current day is not empty

  tstring mFilename; // name of mpLosFile (empty if mpLosFile==0)
  tstring mLangBible;
};

#endif
