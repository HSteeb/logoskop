/* ----------------------------------------------------------------------
  Helmut Steeb 
  List of OTwdFileDesc

  $Id: TwdFileDescList.cpp 3011 2008-11-29 09:37:12Z helmut $

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
#include "../util/StringUtil.h"
#include "TwdFileDesc.h"
#include "TwdFileDescList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === OTwdFileDescList ===

OTwdFileDescList::~OTwdFileDescList()
{
  Clear();
}


// === OTwdFileDescList ===

TFileDescList::iterator OTwdFileDescList::Begin()
{
  return mList.begin();
}

TFileDescList::iterator OTwdFileDescList::End()
{
  return mList.end();
}

TFileDescList::size_type OTwdFileDescList::Size()
{
  return mList.size();
}

void OTwdFileDescList::Add(ITwdFileDesc &fileDesc)
{
  mList.push_back(&fileDesc);
}

ITwdFileDesc *OTwdFileDescList::Find(const string& lang, const string& bible, const int year)
{
  TFileDescList::iterator iter;
  for (iter = mList.begin(); iter != mList.end(); ++iter) {
    ITwdFileDesc *pTwdFileDesc = *iter;
    if (pTwdFileDesc->GetLang()     == lang
        && pTwdFileDesc->GetBible() == bible
        && pTwdFileDesc->GetYear()  == year) {
      return pTwdFileDesc;
    }
  }
  return 0;
}
  
ITwdFileDesc *OTwdFileDescList::Find(const string& langBible, const int year)
{
  string bible(langBible);
  string lang = NStringUtil::Pop("_", bible);
  return Find(lang, bible, year);
}


void OTwdFileDescList::Clear()
{
  TFileDescList::iterator iter;
  for (iter = mList.begin(); iter != mList.end(); ++iter) {
    delete *iter;
    *iter = 0;
  }

  mList.clear();
}


#ifdef _TEST

// === TEST =========================================================================================


bool NTwdFileDescList::SelfTest()
{

  bool bRes = true;
  MSGDEBUG("--- Start NTwdFileDescList::SelfTest ----------------------------");

  OTwdFileDescList list;

  ITwdFileDesc &fileDesc = NTwdFileDesc::MakeTwdFileDesc("de", "Luther1912", 2007);
  list.Add(fileDesc);
  bRes &= (1 == list.Size());

  list.Clear();
  bRes &= (0 == list.Size());

  ITwdFileDesc &fileDesc2 = NTwdFileDesc::MakeTwdFileDesc("de", "Schlachter2000", 2008);
  list.Add(fileDesc2);
  ITwdFileDesc &fileDesc3 = NTwdFileDesc::MakeTwdFileDesc("zh-TW", "ChineseUnionVersion", 2007);
  list.Add(fileDesc3);
  bRes &= (2 == list.Size());

  list.Clear();
  bRes &= (0 == list.Size());

  MSGDEBUG("--- End NTwdFileDescList::SelfTest ----------------------------");
  return bRes;
}

#endif

