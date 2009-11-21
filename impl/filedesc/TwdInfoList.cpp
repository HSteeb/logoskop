/* ----------------------------------------------------------------------
  Helmut Steeb 
  List of OTwdInfo

  $Id: TwdInfoList.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include "../util/StringUtil.h"
#include "TwdInfo.h"
#include "TwdInfoList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === OTwdInfoList ===

OTwdInfoList::~OTwdInfoList()
{
  Clear();
}


// === OTwdInfoList ===

TInfoList::iterator OTwdInfoList::Begin()
{
  return mList.begin();
}

TInfoList::iterator OTwdInfoList::End()
{
  return mList.end();
}

TInfoList::size_type OTwdInfoList::Size()
{
  return mList.size();
}

void OTwdInfoList::Add(const tstring& lang, const tstring& updated, const wstring& info)
{
  ITwdInfo& twdInfo = NTwdInfo::MakeTwdInfo(lang, updated, info);
  mList.push_back(&twdInfo);
}

ITwdInfo* OTwdInfoList::Find(const string& lang)
{
  TInfoList::const_iterator iter;
  for (iter = mList.begin(); iter != mList.end(); ++iter) {
    ITwdInfo* pInfo = *iter;
    if (pInfo->GetLang() == lang) {
      return pInfo;
    }
  }
  return 0;
}
  
ITwdInfo* OTwdInfoList::FindFirst(const std::vector<tstring>& langList)
{
  // try each of the languages passed (in order)
  std::vector<tstring>::const_iterator iter;
  for (iter = langList.begin(); iter != langList.end(); ++iter) {
    const tstring& curLang = *iter;
    ITwdInfo* pInfo = Find(curLang);
    if (pInfo) {
      return pInfo;
    }
  }
  return 0;
}
  
void OTwdInfoList::Clear()
{
  TInfoList::iterator iter;
  for (iter = mList.begin(); iter != mList.end(); ++iter) {
    delete *iter;
    *iter = 0;
  }

  mList.clear();
}


#ifdef _TEST

// === TEST =========================================================================================


bool NTwdInfoList::SelfTest()
{

  bool bRes = true;
  MSGDEBUG("--- Start NTwdInfoList::SelfTest ----------------------------");

  OTwdInfoList list;
  ITwdInfo* pInfo = 0;

  list.Add("de", "2008-11-05T18:23:00Z", L"INFO");
  bRes &= (1 == list.Size());
  bRes &= ( (pInfo = list.Find("de")) && pInfo->GetInfo() == L"INFO");

  list.Clear();
  bRes &= (0 == list.Size());

  list.Add("de", "2008-11-05T18:23:00Z", L"INFO");
  list.Add("en", "2008-11-05T18:23:00Z", L"INFO en");
  bRes &= (2 == list.Size());
  bRes &= ( (pInfo = list.Find("de")) && pInfo->GetInfo() == L"INFO");
  bRes &= ( (pInfo = list.Find("en")) && pInfo->GetInfo() == L"INFO en");

  std::vector<tstring> langList;
  langList.push_back("en");
  langList.push_back("de");
  bRes &= ( (pInfo = list.FindFirst(langList)) && pInfo->GetInfo() == L"INFO en");

  list.Clear();
  bRes &= (0 == list.Size());

  MSGDEBUG("--- End NTwdInfoList::SelfTest ----------------------------");
  return bRes;
}

#endif

