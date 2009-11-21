/* ----------------------------------------------------------------------
  Helmut Steeb 
  Implementation of ITwdInfo interface, 
  represents one .twd file.

  $Id: TwdInfo.cpp 3011 2008-11-29 09:37:12Z helmut $

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
---------------------------------------------------------------------- */

#include "stdafx.h"
#include <assert.h>
#include "TwdInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// === OTwdInfo declaration ===

class OTwdInfo : public ITwdInfo
{
public:
  OTwdInfo(const string& lang, const string& updated, const wstring& info);
  virtual ~OTwdInfo();

  virtual ITwdInfo& Clone();

  virtual void SetLang(const string &lang);
  virtual void SetUpdated(const string &updated);
  virtual void SetInfo(const wstring &info);

  virtual string GetLang() const;
  virtual string GetUpdated() const;
  virtual wstring GetInfo() const;

private:
  string mLang;
  string mUpdated;
  wstring mInfo;
};



// === OTwdInfo implementation ===

OTwdInfo::OTwdInfo(const string& lang, const string& updated, const wstring& info)
: mLang(lang)
, mUpdated(updated)
, mInfo(info)
{
}

OTwdInfo::~OTwdInfo()
{
}

// --- private ---


// --- OTwdInfo ---

ITwdInfo& OTwdInfo::Clone()
{
  OTwdInfo *pTwdInfo = new OTwdInfo(*this);
  assert(pTwdInfo);
  return *pTwdInfo;
}

void OTwdInfo::SetLang(const string &lang)
{
  mLang = lang;
}

void OTwdInfo::SetUpdated(const string &updated)
{
  mUpdated = updated;
}

void OTwdInfo::SetInfo(const wstring &info)
{
  mInfo = info;
}

string OTwdInfo::GetLang() const
{
  return mLang;
}

string OTwdInfo::GetUpdated() const
{
  return mUpdated;
}

wstring OTwdInfo::GetInfo() const
{
  return mInfo;
}




// === NTwdInfo ===

ITwdInfo& NTwdInfo::MakeTwdInfo(const string& lang, const string& updated, const wstring& info)
{
  OTwdInfo *pTwdInfo = new OTwdInfo(lang, updated, info);
  assert(pTwdInfo);
  return *pTwdInfo;
}

