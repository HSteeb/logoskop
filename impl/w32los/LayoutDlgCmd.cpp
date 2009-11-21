/* ----------------------------------------------------------------------
  Helmut Steeb
  Layout dialog commands.
  $Id: LayoutDlgCmd.cpp 3905 2009-09-12 11:30:16Z helmut $

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
#include "LayoutDlgCmd.h"
#include "../util/Layout.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === OLayoutDlgCmd base class ===

OLayoutDlgCmd::OLayoutDlgCmd(ILayout &oldLayout, const tstring &langBible, bool bApplyToAllLangBibles)
: mLangBible(langBible)
, mbApplyToAllLangBibles(bApplyToAllLangBibles)
{
  mpOldLayout = &NLayout::MakeLayout();
  mpOldLayout->Assign(oldLayout);
}

OLayoutDlgCmd::~OLayoutDlgCmd()
{
  delete mpOldLayout;
}

ILayout &OLayoutDlgCmd::OldLayout() const
{
  return *mpOldLayout;
}

const tstring &OLayoutDlgCmd::LangBible() const
{
  return mLangBible;
}

bool OLayoutDlgCmd::ApplyToAllLangBibles() const
{
  return mbApplyToAllLangBibles;
}


// === OLayoutDlgCmdBkCol ===

OLayoutDlgCmdBkCol::OLayoutDlgCmdBkCol(
  const tstring &langBible, ILayout &oldLayout, COLORREF newCol)
: OLayoutDlgCmd(oldLayout, langBible, true)
, mNewBkCol(newCol)
{
}

OLayoutDlgCmdBkCol::~OLayoutDlgCmdBkCol()
{
}

void OLayoutDlgCmdBkCol::Accept(ILayoutDlgCmdVisitor &vis)
{
  vis.VisitCmdBkCol(*this);
}

COLORREF OLayoutDlgCmdBkCol::NewBkCol() const
{
  return mNewBkCol;
}


// === OLayoutDlgCmdLineName ===

OLayoutDlgCmdLineName::OLayoutDlgCmdLineName(
  const tstring &langBible, ILosungLayout::OLineType lineType, 
  ILayout &oldLayout, const tstring &newName,
  bool bApplyToAllLangBibles)
: OLayoutDlgCmd(oldLayout, langBible, bApplyToAllLangBibles)
, mLineType(lineType)
, mNewName(newName)
{
}

void OLayoutDlgCmdLineName::Accept(ILayoutDlgCmdVisitor &vis)
{
  vis.VisitCmdLineName(*this);
}

ILosungLayout::OLineType OLayoutDlgCmdLineName::LineType()
{
  return mLineType;
}

const tstring &OLayoutDlgCmdLineName::NewName() const
{
  return mNewName;
}


// === OLayoutDlgCmdLineSize ===

OLayoutDlgCmdLineSize::OLayoutDlgCmdLineSize(
  const tstring &langBible, ILosungLayout::OLineType lineType, 
  ILayout &oldLayout, int newSize,
  bool bApplyToAllLangBibles)
: OLayoutDlgCmd(oldLayout, langBible, bApplyToAllLangBibles)
, mLineType(lineType)
, mNewSize(newSize)
{
}

void OLayoutDlgCmdLineSize::Accept(ILayoutDlgCmdVisitor &vis)
{
  vis.VisitCmdLineSize(*this);
}

ILosungLayout::OLineType OLayoutDlgCmdLineSize::LineType()
{
  return mLineType;
}

int OLayoutDlgCmdLineSize::NewSize() const
{
  return mNewSize;
}


// === OLayoutDlgCmdLineLayout ===

OLayoutDlgCmdLineLayout::OLayoutDlgCmdLineLayout(
  const tstring &langBible, ILosungLayout::OLineType lineType, 
  ILayout &oldLayout, ILineLayout &newLayout,
  bool bApplyToAllLangBibles)
: OLayoutDlgCmd(oldLayout, langBible, bApplyToAllLangBibles)
, mLineType(lineType)
{
  mpNewLineLayout = &NLayout::MakeLineLayout();
  mpNewLineLayout->Assign(newLayout);
}

OLayoutDlgCmdLineLayout::~OLayoutDlgCmdLineLayout()
{
  delete mpNewLineLayout;
}

void OLayoutDlgCmdLineLayout::Accept(ILayoutDlgCmdVisitor &vis)
{
  vis.VisitCmdLineLayout(*this);
}

ILosungLayout::OLineType OLayoutDlgCmdLineLayout::LineType()
{
  return mLineType;
}

const ILineLayout &OLayoutDlgCmdLineLayout::NewLineLayout() const
{
  return *mpNewLineLayout;
}


// === OLayoutDlgCmdLosungLayout ===

OLayoutDlgCmdLosungLayout::OLayoutDlgCmdLosungLayout(
  const tstring &langBible, 
  ILayout &oldLayout, ILosungLayout &newLosungLayout,
  bool bApplyToAllLangBibles)
: OLayoutDlgCmd(oldLayout, langBible, bApplyToAllLangBibles)
{
  mpNewLosungLayout = &NLayout::MakeLosungLayout();
  mpNewLosungLayout->Assign(newLosungLayout);
}

OLayoutDlgCmdLosungLayout::~OLayoutDlgCmdLosungLayout()
{
  delete mpNewLosungLayout;
}

void OLayoutDlgCmdLosungLayout::Accept(ILayoutDlgCmdVisitor &vis)
{
  vis.VisitCmdLosungLayout(*this);
}

const ILosungLayout &OLayoutDlgCmdLosungLayout::NewLosungLayout() const
{
  return *mpNewLosungLayout;
}


// === NLayoutDlgCmd ===

void NLayoutDlgCmd::ClearStack(std::vector<OLayoutDlgCmd*> &stack)
{
  std::vector<OLayoutDlgCmd*>::iterator it;
  for (it = stack.begin(); it != stack.end(); ++it) {
    OLayoutDlgCmd *pLayoutDlgCmd = *it;
    delete pLayoutDlgCmd;
  }
  stack.clear();
}


// === TEST ====================================================

#ifdef _TEST

bool NLayoutDlgCmd::SelfTest()
{

  bool bRes = true;
  MSGDEBUG("--- Start NLayoutDlgCmd::SelfTest ----------------------------");
  MSGDEBUG("--- End NLayoutDlgCmd::SelfTest ----------------------------");
  return bRes;
}

#endif

