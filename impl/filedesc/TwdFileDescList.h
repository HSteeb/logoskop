/* ----------------------------------------------------------------------
  Helmut Steeb 
  List of OTwdFileDesc

  $Id: TwdFileDescList.h 3011 2008-11-29 09:37:12Z helmut $

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

#if !defined(_TWDFILEDESCLIST_INCLUDED_)
#define _TWDFILEDESCLIST_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ITwdFileDesc;

typedef std::vector<ITwdFileDesc*> TFileDescList;

class OTwdFileDescList {
public:

  virtual ~OTwdFileDescList();

  TFileDescList::iterator Begin();
  TFileDescList::iterator End();

  TFileDescList::size_type Size();

  // Add: consumes fileDesc (will be deleted by OFileDescList)
  void Add(ITwdFileDesc& fileDesc);

  ITwdFileDesc *Find(const string& lang, const string& bible, const int year);
  ITwdFileDesc *Find(const string& langBible, const int year);

  void Clear();

private:
  TFileDescList mList;  
};


// === TEST ====================================================

#ifdef _TEST
namespace NTwdFileDescList {
  bool SelfTest();
}
#endif


#endif
