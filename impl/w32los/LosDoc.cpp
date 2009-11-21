/* ----------------------------------------------------------------------
  Helmut Steeb
  $Id: LosDoc.cpp 3932 2009-09-28 21:58:25Z helmut $

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

#include "LosDoc.h"
#include "../losfile/LosFile.h"
#include "../util/DateUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OLosDoc::OLosDoc(OLosFile &losFile, const tstring &langBible)
: mLosFile(losFile)
, mLangBible(langBible)
, mpCurrentLosung(0)
{
}

OLosDoc::~OLosDoc()
{
  delete &mLosFile;
}


OLosFile &OLosDoc::LosFile()
{
  return mLosFile;
}

const tstring OLosDoc::LangBible() const
{
  return mLangBible;
}

int OLosDoc::Year() const
{
  return mLosFile.Year();
}

void OLosDoc::SetDate(CTime &date)
{
  mpCurrentLosung = GetLosungForDate(date);
}

OLosung *OLosDoc::GetLosungForDate(const CTime &date) const
{
  int dayOfYear = DateUtil::DayOfYear(date.GetYear(), date.GetMonth(), date.GetDay());
  ASSERT(dayOfYear != NO_DAYOFYEAR);
  return mLosFile.LosungVector().Get(dayOfYear + 1);
}

bool OLosDoc::HasCopyright() const
{
  return !mLosFile.Xlat().empty();
}

wstring OLosDoc::GetCopyright() const
{
  return mLosFile.Xlat();
}

wstring OLosDoc::GetText() const
{
  if (!mpCurrentLosung) {
    return L"";
  }
  return mpCurrentLosung->AsString();
}

wstring OLosDoc::GetFileVersion() const
{
  return mLosFile.FileVers();
}

// === CurrentLosung ===

bool OLosDoc::HasCurrentLosung() const
{
  return 0 != mpCurrentLosung;
}

OLosung &OLosDoc::CurrentLosung() const
{
  ASSERT(mpCurrentLosung);
  return *mpCurrentLosung;
}

