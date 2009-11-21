/* ----------------------------------------------------------------------
  Helmut Steeb
  Layout dialog commands.
  $Id: LayoutDlgCmd.h 3905 2009-09-12 11:30:16Z helmut $

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
#if !defined(AFX_LAYOUTDLGCMD_H__INCLUDED_)
#define AFX_LAYOUTDLGCMD_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../util/ILayout.h"

class ILayoutDlgCmdVisitor;


// === OLayoutDlgCmd base class ===

class OLayoutDlgCmd
{
public:
  OLayoutDlgCmd(ILayout &oldLayout, const tstring &langBible, bool bApplyToAllLangBibles);
  virtual ~OLayoutDlgCmd();
  virtual void Accept(ILayoutDlgCmdVisitor &vis) = 0;

  ILayout &OldLayout() const;
  const tstring &LangBible() const;
  bool ApplyToAllLangBibles() const;

private:
  ILayout *mpOldLayout;
  const tstring mLangBible;
  bool mbApplyToAllLangBibles;
};


// === OLayoutDlgCmdBkCol ===

class OLayoutDlgCmdBkCol : public OLayoutDlgCmd
{
public:
  OLayoutDlgCmdBkCol(const tstring &langBible, ILayout &oldLayout, COLORREF newCol);
  virtual ~OLayoutDlgCmdBkCol();
  virtual void Accept(ILayoutDlgCmdVisitor &vis);

  COLORREF NewBkCol() const;
private:
  COLORREF mNewBkCol;
};


// === OLayoutDlgCmdLineName ===

class OLayoutDlgCmdLineName : public OLayoutDlgCmd
{
public:
  OLayoutDlgCmdLineName(
    const tstring &langBible, ILosungLayout::OLineType lineType, 
    ILayout &oldLayout, const tstring &newName,
    bool bApplyToAllLangBibles);
  virtual ~OLayoutDlgCmdLineName() {};
  virtual void Accept(ILayoutDlgCmdVisitor &vis);

  ILosungLayout::OLineType LineType();
  const tstring &NewName() const;
private:
  ILosungLayout::OLineType mLineType;
  tstring mNewName;
};


// === OLayoutDlgCmdLineSize ===

class OLayoutDlgCmdLineSize : public OLayoutDlgCmd
{
public:
  OLayoutDlgCmdLineSize(
    const tstring &langBible, ILosungLayout::OLineType lineType, 
    ILayout &oldLayout, int newSize,
    bool bApplyToAllLangBibles);
  virtual ~OLayoutDlgCmdLineSize() {};
  virtual void Accept(ILayoutDlgCmdVisitor &vis);

  ILosungLayout::OLineType LineType();
  int NewSize() const;
private:
  ILosungLayout::OLineType mLineType;
  int mNewSize;
};


// === OLayoutDlgCmdLineLayout ===

class OLayoutDlgCmdLineLayout : public OLayoutDlgCmd
{
public:
  OLayoutDlgCmdLineLayout(
    const tstring &langBible, ILosungLayout::OLineType lineType, 
    ILayout &oldLayout, ILineLayout &newLayout,
    bool bApplyToAllLangBibles);
  virtual ~OLayoutDlgCmdLineLayout();
  virtual void Accept(ILayoutDlgCmdVisitor &vis);

  ILosungLayout::OLineType LineType();
  const ILineLayout &NewLineLayout() const;
private:
  ILosungLayout::OLineType mLineType;
  ILineLayout *mpNewLineLayout;
};


// === OLayoutDlgCmdLosungLayout ===

class OLayoutDlgCmdLosungLayout : public OLayoutDlgCmd
{
public:
  OLayoutDlgCmdLosungLayout(
    const tstring &langBible, 
    ILayout &oldLayout, ILosungLayout &newLosungLayout,
  bool bApplyToAllLangBibles);
  virtual ~OLayoutDlgCmdLosungLayout();
  virtual void Accept(ILayoutDlgCmdVisitor &vis);

  const ILosungLayout &NewLosungLayout() const;
private:
  ILosungLayout *mpNewLosungLayout;
};


// === ILayoutDlgCmdVisitor ===

class ILayoutDlgCmdVisitor
{
public:
  virtual void VisitCmdBkCol       (OLayoutDlgCmdBkCol        &layoutDlgCmdBkCol) = 0;
  virtual void VisitCmdLineName    (OLayoutDlgCmdLineName     &layoutDlgCmdLineName) = 0;
  virtual void VisitCmdLineSize    (OLayoutDlgCmdLineSize     &layoutDlgCmdLineSize) = 0;
  virtual void VisitCmdLineLayout  (OLayoutDlgCmdLineLayout   &layoutDlgCmdLineLayout) = 0;
  virtual void VisitCmdLosungLayout(OLayoutDlgCmdLosungLayout &layoutDlgCmdLosungLayout) = 0;
};


// === NLayoutDlgCmd ===

namespace NLayoutDlgCmd {

  void ClearStack(std::vector<OLayoutDlgCmd*> &stack);

// === TEST ====================================================
#ifdef _TEST
  bool SelfTest();
#endif

};
#endif
