/* ----------------------------------------------------------------------
 Helmut Steeb
 @brief Stores data for L
 @author Helmut Steeb
 @version $Id: Line.cpp 3853 2009-08-26 13:50:27Z helmut $

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
#include "Line.h"
#include "LosungObjectVisitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _LINE_SAVE_SPACE_ 
// --- _LINE_SAVE_SPACE_ ---

OLine::OLine()
: mpContent(0)
{
}

OLine::OLine(const OLine &rhs)
{
  mpContent = 0;
  Content(rhs.Content());
}

OLine::~OLine()
{
  if (mpContent) {
    free(mpContent);
  }
}

void OLine::Accept(OLosungObjectVisitor &vis)
{
  vis.Visit(*this);
}


bool OLine::IsEmpty() const
{
  return !mpContent || !*mpContent;
}

wstring OLine::Content() const
{
  return wstring(mpContent ? mpContent : L"");
}

void OLine::Content(const wstring &s)
{
  const wchar_t *p = s.c_str();
  Content(p, p + s.length());
}

void OLine::Content(const wchar_t *pStart, const wchar_t *pEnd)
{
  if (mpContent) {
    delete mpContent;
  }
  mpContent = (wchar_t*) malloc(sizeof(wchar_t) * (pEnd - pStart + 1));
  //assert(mpContent);strcpy
  std::copy(pStart, pEnd, mpContent);
  *(mpContent + (pEnd - pStart)) = 0;
}

const wchar_t *OLine::c_str() const
{
  return mpContent ? mpContent : L"";
}

#else 
// --- _LINE_SAVE_SPACE_ ---


bool OLine::IsEmpty() const
{
  return mpContent.empty();
}

wstring OLine::Content() const
{
  return mpContent;
}

void OLine::Content(const wstring &s)
{
  mpContent = s;
}

void OLine::Content(const wchar_t *pStart, const wchar_t *pEnd)
{
  mpContent.assign(pStart, pEnd);
}

const wchar_t *OLine::c_str() const
{
  return mpContent.c_str();
}


// --- _LINE_SAVE_SPACE_ ---
#endif
