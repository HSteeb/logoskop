/* ----------------------------------------------------------------------
  Helmut Steeb 
  Implementation of ITwdFileDesc interface, 
  represents one .twd file.

  $Id: TwdFileDesc.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include "TwdFileDesc.h"
#include "../util/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// === OTwdFileDesc declaration ===

class OTwdFileDesc : public ITwdFileDesc
{
public:
  OTwdFileDesc(const string& lang, const string& bible, const int year);
  virtual ~OTwdFileDesc();

  virtual ITwdFileDesc& Clone();

  virtual string GetKey() const;
  virtual string GetDisplayName() const;
  virtual bool IsEqual(const ITwdFileDesc& rhs) const;

  virtual void SetLang(const string &lang);
  virtual void SetBible(const string &bible);
  virtual void SetYear(const int year);
  virtual void SetUpdated(const string &updated);
  virtual void SetInfo(const wstring &info);
  virtual void SetUrl(const string &url);

  virtual string GetLang() const;
  virtual string GetBible() const;
  virtual string GetLangBible() const;
  virtual int    GetYear() const;
  virtual string GetUpdated() const;
  virtual wstring GetInfo() const;
  virtual string GetUrl() const;

private:
  void _Update(); // redundant mKey, mDisplayName

private:
  string mLang;
  string mBible;
  int    mYear;
  string mUpdated;
  wstring mInfo;
  string mUrl;
  string mKey;
  string mDisplayName;
};



// === OTwdFileDesc implementation ===

OTwdFileDesc::OTwdFileDesc(const string& lang, const string& bible, const int year)
: mLang(lang)
, mBible(bible)
, mYear(year)
{
  _Update();
}

OTwdFileDesc::~OTwdFileDesc()
{
}

// --- private ---

void OTwdFileDesc::_Update()
{
  std::ostringstream os;
  os << mLang << "_" << mBible << "_" << mYear;
  mKey = os.str();

  // mDisplayName = mLang + ": " + mBible;
  mDisplayName = mLang + "_" + mBible;
  // mDisplayName = mBible + " (" + mLang + ")";
  //mDisplayName = mBible + " (" + NLangNames::MakeIsoDisplayName(mLang) + ")";
}


// --- OTwdFileDesc ---

ITwdFileDesc& OTwdFileDesc::Clone()
{
  OTwdFileDesc *pTwdFileDesc = new OTwdFileDesc(*this);
  assert(pTwdFileDesc);
  return *pTwdFileDesc;
}

void OTwdFileDesc::SetLang(const string &lang)
{
  mLang = lang;
  _Update();
}

void OTwdFileDesc::SetBible(const string &bible)
{
  mBible = bible;
  _Update();
}

void OTwdFileDesc::SetYear(const int year)
{
  mYear = year;
  _Update();
}

void OTwdFileDesc::SetUpdated(const string &updated)
{
  mUpdated = updated;
}

void OTwdFileDesc::SetInfo(const wstring &info)
{
  mInfo = info;
}

void OTwdFileDesc::SetUrl(const string &url)
{
  mUrl = url;
}

string OTwdFileDesc::GetLang() const
{
  return mLang;
}

string OTwdFileDesc::GetBible() const
{
  return mBible;
}

string OTwdFileDesc::GetLangBible() const
{
  return mLang + "_" + mBible;
}

int OTwdFileDesc::GetYear() const
{
  return mYear;
}

string OTwdFileDesc::GetUpdated() const
{
  return mUpdated;
}

wstring OTwdFileDesc::GetInfo() const
{
  return mInfo;
}

string OTwdFileDesc::GetUrl() const
{
  return mUrl;
}

string OTwdFileDesc::GetKey() const
{
  return mKey;
}

string OTwdFileDesc::GetDisplayName() const
{
  return mDisplayName;
}

bool OTwdFileDesc::IsEqual(const ITwdFileDesc& rhs) const
{
  return mKey == rhs.GetKey();
}




// === NTwdFileDesc ===

ITwdFileDesc& NTwdFileDesc::MakeTwdFileDesc(const string& lang, const tstring& bible, const int year)
{
  OTwdFileDesc *pTwdFileDesc = new OTwdFileDesc(lang, bible, year);
  assert(pTwdFileDesc);
  return *pTwdFileDesc;
}

ITwdFileDesc& NTwdFileDesc::MakeTwdFileDesc(const string& langBible, const int year)
{
  string tmp = langBible;
  string lang = NStringUtil::Pop("_", tmp);
  string bible = tmp;
  return MakeTwdFileDesc(lang, bible, year);
}

ITwdFileDesc& NTwdFileDesc::MakeTwdFileDesc(const string& path)
{
  string tmp(path);

  // remove folder prefix (if any)
  string::size_type pos = tmp.rfind("\\");
  if (pos == string::npos) {
    // accept "\" and "/" for path separator
    pos = tmp.rfind("/");
  }
  if (pos != string::npos) {
    tmp.erase(0, pos + 1); // param 2: char count
  }

  // "de_Schlachter2000_2008.twd"
  string lang  = NStringUtil::Pop("_", tmp);
  string bible = NStringUtil::Pop("_", tmp);
  // atoi would also extract "2008" from tmp=="2008.twd"
  // (with my current language settings at least),
  // but be defensive, extract language string first:
  string year  = NStringUtil::Pop(".", tmp);
  int nYear = atoi(year.c_str());

  return MakeTwdFileDesc(lang, bible, nYear);
}
