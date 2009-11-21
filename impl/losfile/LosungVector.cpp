/* ======================================================================
 @file OLosung.h
 @brief C++ class for representing one daily losung 
 @author Helmut Steeb
 @version $Id: LosungVector.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include "Losung.h"
#include "LosungVector.h"



// === OLosungVector ===

OLosungVector::OLosungVector()
{
  mLosungVector.resize(366);
}

OLosungVector::~OLosungVector()
{
  OLosungVector::OIter it;
  for (it = mLosungVector.begin(); it != mLosungVector.end(); ++it) {
    delete *it;
  }
}

OLosungVector::OIter OLosungVector::Begin()
{
  return mLosungVector.begin();
}
 
OLosungVector::OIter OLosungVector::Next(const OIter &it)
{
  OLosungVector::OIter curIt = it;
  OLosungVector::OIter end = mLosungVector.end();
  ++curIt;
  while (curIt != end && !*curIt) {
    ++curIt;
  }
  return curIt;
}
 
OLosungVector::OIter OLosungVector::End()
{
  return mLosungVector.end();
}

OLosung *OLosungVector::Get(unsigned int dayOfYear)
{
  assert(dayOfYear > 0);
  assert(dayOfYear <= 366);
  return mLosungVector[dayOfYear-1];
}

OLosung &OLosungVector::GetOrMake(unsigned int dayOfYear)
{
  OLosung *pLosung = Get(dayOfYear);
  if (!pLosung) {
    pLosung = new OLosung;
    mLosungVector[dayOfYear-1] = pLosung;
  }
  return *pLosung;
}

void OLosungVector::Delete(unsigned int dayOfYear)
{
  assert(dayOfYear > 0);
  assert(dayOfYear <= 366);
  delete mLosungVector[dayOfYear-1];
  mLosungVector[dayOfYear-1] = 0;
}


