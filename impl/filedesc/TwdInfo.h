/* ----------------------------------------------------------------------
  Helmut Steeb 
  ITwdInfo interface represents an info line from the server Twd11List
  $Id: TwdInfo.h 3011 2008-11-29 09:37:12Z helmut $

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

#if !defined(_ITWDINFO_INCLUDED_)
#define _ITWDINFO_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class ITwdInfo {
public:
  virtual ~ITwdInfo() {};

  virtual ITwdInfo& Clone() = 0;

  virtual void SetLang(const string &lang) = 0;
  virtual void SetUpdated(const string &updated) = 0;
  virtual void SetInfo(const wstring &info) = 0;

  virtual string GetLang() const = 0;
  virtual string GetUpdated() const = 0;
  virtual wstring GetInfo() const = 0;
};

namespace NTwdInfo {
  ITwdInfo& MakeTwdInfo(const string& lang, const string& updated, const wstring& info);
}

#endif
