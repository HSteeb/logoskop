/* ----------------------------------------------------------------------
 Helmut Steeb
 Module for storing one testament parol.
  $Id: Testimony.cpp 3932 2009-09-28 21:58:25Z helmut $

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
#include "LosungObjectVisitor.h"
#include "Testimony.h"

OTestimony::~OTestimony()
{
}

void OTestimony::Accept(OLosungObjectVisitor &vis)
{
  vis.Visit(*this);
}

bool OTestimony::IsEmpty()
{
  return mIntroLine.IsEmpty() 
    && mLines.IsEmpty();
}

tstring OTestimony::ParolID() const
{
  return mParolID;
}

void OTestimony::ParolID(const tstring &s)
{
  mParolID = s;
}

OLine &OTestimony::IntroLine()
{
  return mIntroLine;
}

OLine &OTestimony::Lines()
{
  return mLines;
}

OLine &OTestimony::SourceLine()
{
  return mSource;
}


// convenience functions

wstring OTestimony::AsString()
{
  wstring res;
  if (!IntroLine().IsEmpty()) {
    res = IntroLine().Content();
  }
  if (!Lines().IsEmpty()) {
    if (!res.empty()) {
      res += L"\n";
    }
    res += Lines().Content();
  }
  if (!SourceLine().IsEmpty()) {
    if (!res.empty()) {
      res += L"\n";
    }
    res += SourceLine().Content();
  }
  return res;
}
