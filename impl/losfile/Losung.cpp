/* ======================================================================
 @file OLosung.h
 @brief C++ class for representing one daily losung 
 @author Helmut Steeb
 @version $Id: Losung.cpp 3932 2009-09-28 21:58:25Z helmut $

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
#include "Losung.h"
#include "LosungObjectVisitor.h"
#include "Testimony.h"


// --- OLosung (one day) ---

OLosung::~OLosung()
{
  for (OIter it = mTestimonyVector.begin(); it != mTestimonyVector.end(); ++it) {
    delete (*it);
  }
}

void OLosung::Accept(OLosungObjectVisitor &vis)
{
  vis.Visit(*this);
}

bool OLosung::IsEmpty()
{
  return mTitleLine.IsEmpty()
    && mTestimonyVector.empty()
    && mCommentLines.IsEmpty();
}

// TitleLine

OLine &OLosung::TitleLine()
{
  return mTitleLine;
}


// Testimony*

OLosung::OIter OLosung::Begin()
{
  return mTestimonyVector.begin();
}
 
OLosung::OIter OLosung::Next(const OIter &it)
{
  OIter _it = it;
  if (_it != mTestimonyVector.end()) {
    ++_it;
  }
  return _it;
}
 
OLosung::OIter OLosung::End()
{
  return mTestimonyVector.end();
}

OTestimony &OLosung::AddParol()
{
  OTestimony *pTestimony = new OTestimony;
  mTestimonyVector.push_back(pTestimony);
  return *pTestimony;
}



// CommentLines

OLine &OLosung::CommentLines()
{
  return mCommentLines;
}

// convenience functions

wstring OLosung::AsString()
{
  wstring res;

  wstring &titleLine = TitleLine().Content();
  if (!titleLine.empty()) {
    res = titleLine;
  }

  OIter it;
  for (it = mTestimonyVector.begin(); it != mTestimonyVector.end(); ++it) {
    wstring parol = (**it).AsString();
    if (!parol.empty()) {
      // trap case that titleLine is empty
      if (!res.empty()) {
        res += L"\n\n";
      }
      res += parol;
    }
  }

  wstring comment = CommentLines().Content();
  if (!comment.empty()) {
    res += L"\n\n" + comment;
  }
  // add trailing line break
  // HS 2003-08-15: for compatibility with w32los 3.1.1 Edit-Copy functionality
  // HS 2003-08-15: no trailing line break for display!
  // HS 2009-09-28: no trailing line break directly after keyword for Edit-Copy!
  return res;
}

