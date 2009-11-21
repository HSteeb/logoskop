/* ======================================================================
 @file OLosFile.h
 @brief C++ class for representing one losung file
 @author Helmut Steeb
 @version $Id: LosFile.h 3853 2009-08-26 13:50:27Z helmut $

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
#ifndef __OLOSFILE_H__
#define __OLOSFILE_H__

#include "LosungVector.h"
#include "LosungObject.h"


class OLosFile  : public OLosungObject
{
public:
  OLosFile();
  void Accept(OLosungObjectVisitor &vis);

  int DtdMainVers(); 
  int DtdSubVers();
  int Year();
  tstring XmlLang();
  wstring BibleName();
  wstring BibleCopyright();
  wstring FileVers();
  wstring Xlat();

  OLosungVector &LosungVector();

  void DtdMainVers(int dtdMainVers);
  void DtdSubVers(int dtdSubVers);
  void Year(int year);
  void XmlLang(const tstring &s);
  void BibleName(const wstring &s);
  void BibleCopyright(const wstring &s);
  void FileVers(const wstring &s);
  void Xlat(const wstring &s);
  
private:
  int mDtdMainVers;
  int mDtdSubVers;
  int mYear;
  tstring mXmlLang; // tstring!
  wstring mBibleName;
  wstring mBibleCopyright;
  wstring mFileVers;
  wstring mXlat;

  OLosungVector mLosungVector;
};

#endif
