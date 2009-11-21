/* ----------------------------------------------------------------------
  Helmut Steeb
  Controller class for layout dialog.
  $Id: LayoutController.h 3905 2009-09-12 11:30:16Z helmut $

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
#if !defined(AFX_LAYOUTCONTROLLER_H__INCLUDED_)
#define AFX_LAYOUTCONTROLLER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../util/ILayout.h"
#include "LayoutDlgCmd.h"

class IProfile;
class ILayoutDlg;

class OLayoutController : public ILayoutDlgCmdVisitor
{
public:
  OLayoutController(
    IProfile &profile, 
    ILayoutDlg &layoutDlg, 
    const std::vector<string> &langBibles, 
    ILayout &layout, 
    std::vector<OLayoutDlgCmd*> &undoStack,
    std::vector<OLayoutDlgCmd*> &redoStack,
    CView *pView = 0);
  ~OLayoutController();

  // Init:
  // setup of controller and dialog.
  // mLayout must contain at least one langBible (asserted).
  void Init();
  unsigned int GetMinExecUndoStackLen() const;

  // --- ILayoutDlgCmdVisitor ---

  virtual void VisitCmdBkCol       (OLayoutDlgCmdBkCol        &layoutDlgCmdBkCol);
  virtual void VisitCmdLineName    (OLayoutDlgCmdLineName     &layoutDlgCmdLineName);
  virtual void VisitCmdLineSize    (OLayoutDlgCmdLineSize     &layoutDlgCmdLineSize);
  virtual void VisitCmdLineLayout  (OLayoutDlgCmdLineLayout   &layoutDlgCmdLineLayout);
  virtual void VisitCmdLosungLayout(OLayoutDlgCmdLosungLayout &layoutDlgCmdLosungLayout);


  // --- Interface for dialog ---

  void SetLangBible(const tstring &langBible);
  void SetAllLangBiblesMode(bool b);

  void SelectBkCol();

  void SetLineFontName(ILosungLayout::OLineType lineType, const tstring &name);
  // SetLineFontSize:
  // size in tenths of a point
  void SetLineFontSize(ILosungLayout::OLineType lineType, int size);

  void CopyLine(ILosungLayout::OLineType lineType);
  void PasteLine(ILosungLayout::OLineType lineType);

  void CopyLosung();
  void PasteLosung();

  void Properties(ILosungLayout::OLineType lineType);

  void Undo();
  void Redo();

protected:

  void _UpdateView();
  void _SetButtonSens();
  void _GetOneOrAllLangBibles(const OLayoutDlgCmd &layoutDlgCmd, std::vector<tstring> &langBibles);

  // --- Cmd handling ---

  void _ClearStack(std::vector<OLayoutDlgCmd*> &stack);
  void _Exec(OLayoutDlgCmd *pLayoutDlgCmd);

private:
  IProfile &mProfile;
  CView *mpView;
  const std::vector<string> &mLangBibles;
  ILayout &mLayout;
  ILayoutDlg &mLayoutDlg;
  ILosungLayout *mpLosungLayout;
  ILineLayout *mpLineLayoutCopy;
  ILosungLayout *mpLosungLayoutCopy;
  tstring mLangBible;
  bool mApplyToAllLangBibles;
  std::vector<OLayoutDlgCmd*> &mUndoStack;
  std::vector<OLayoutDlgCmd*> &mRedoStack;
  unsigned int mMinExecUndoStackLen;
};

#endif // AFX_LAYOUTCONTROLLER_H__INCLUDED_
