/* ======================================================================
 @file OLosFile.h
 @brief C++ class for representing one losung file
 @author Helmut Steeb
 @version $Id: LosFile.cpp 3853 2009-08-26 13:50:27Z helmut $

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
#include "LosFile.h"
#include "LosungObjectVisitor.h"
#include "../util/DateUtil.h"


// --- OLosFile (one year) ---

OLosFile::OLosFile()
{
}

void OLosFile::Accept(OLosungObjectVisitor &vis)
{
  vis.Visit(*this);
}


// === access ===

int OLosFile::DtdMainVers()
{
  return mDtdMainVers;
}

int OLosFile::DtdSubVers()
{
  return mDtdSubVers;
}

int OLosFile::Year()
{
  return mYear;
}

tstring OLosFile::XmlLang()
{
  return mXmlLang;
}

wstring OLosFile::BibleName()
{
  return mBibleName;
}

wstring OLosFile::BibleCopyright()
{
  return mBibleCopyright;
}

wstring OLosFile::FileVers()
{
  return mFileVers;
}

wstring OLosFile::Xlat()
{
  return mXlat;
}


// === modify ===

OLosungVector &OLosFile::LosungVector()
{
  return mLosungVector;
}

void OLosFile::DtdMainVers(int dtdMainVers)
{
  mDtdMainVers = dtdMainVers;
}

void OLosFile::DtdSubVers(int dtdSubVers)
{
  mDtdSubVers = dtdSubVers;
}

void OLosFile::Year(int year)
{
  mYear = year;
}

void OLosFile::XmlLang(const tstring &s)
{ 
  mXmlLang = s;
}

void OLosFile::BibleName(const wstring &s)
{
  mBibleName = s;
}

void OLosFile::BibleCopyright(const wstring &s)
{
  mBibleCopyright = s;
}

void OLosFile::FileVers(const wstring &s)
{
  mFileVers = s;
}

void OLosFile::Xlat(const wstring &s)
{
  mXlat = s;
}

