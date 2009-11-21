/* ----------------------------------------------------------------------
  Helmut Steeb
  Controller class for layout dialog.
  $Id: LayoutController.cpp 3905 2009-09-12 11:30:16Z helmut $

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
#include "LayoutController.h"
#include "ILayoutDlg.h"
#include "../util/Layout.h"
#include "../util/IProfile.h"
#include "../util/ProfileDef.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



OLayoutController::OLayoutController(
  IProfile &profile, 
  ILayoutDlg &layoutDlg, 
  const std::vector<string> &langBibles, 
  ILayout &layout, 
  std::vector<OLayoutDlgCmd*> &undoStack,
  std::vector<OLayoutDlgCmd*> &redoStack,
  CView *pView)
: mProfile(profile)
, mLayoutDlg(layoutDlg)
, mpLosungLayout(0)
, mLangBibles(langBibles)
, mLayout(layout)
, mpLineLayoutCopy(0)
, mpLosungLayoutCopy(0)
, mUndoStack(undoStack)
, mRedoStack(redoStack)
, mpView(pView)
, mApplyToAllLangBibles(true)
{
  mMinExecUndoStackLen = mUndoStack.size();
}

OLayoutController::~OLayoutController()
{
  delete mpLineLayoutCopy;
  delete mpLosungLayoutCopy;
}

void OLayoutController::Init()
{
  MSGDEBUG(_T("OLayoutController::Init"));
  mLayoutDlg.SetLangBibles(mLangBibles);
  assert(!mLangBibles.empty()); // ***
  const tstring &langBible = *mLangBibles.begin();
  SetLangBible(langBible);
  mApplyToAllLangBibles = mProfile.GetBool(PRF_LAYOUT_ALL_LANGBIBLES, DEFAULT_LAYOUT_ALL_LANGBIBLES);
  mLayoutDlg.SetApplyToAllLangBibles(mApplyToAllLangBibles);
  MSGDEBUG(_T("OLayoutController::Init - mApplyToAllLangBibles=") << mApplyToAllLangBibles);
  _SetButtonSens();
}

unsigned int OLayoutController::GetMinExecUndoStackLen() const
{
  return mMinExecUndoStackLen;
}

void OLayoutController::_UpdateView()
{
  if (mpView) {
    mpView->Invalidate();
  }
}

void OLayoutController::_SetButtonSens()
{
  mLayoutDlg.SetLinePasteButtonSens(ILosungLayout::ltTL, mpLineLayoutCopy != 0);
  mLayoutDlg.SetLinePasteButtonSens(ILosungLayout::ltIL, mpLineLayoutCopy != 0);
  mLayoutDlg.SetLinePasteButtonSens(ILosungLayout::ltL , mpLineLayoutCopy != 0);
  mLayoutDlg.SetLinePasteButtonSens(ILosungLayout::ltSL, mpLineLayoutCopy != 0);
  mLayoutDlg.SetLosungPasteButtonSens(mpLosungLayoutCopy != 0);
  mLayoutDlg.SetUndoButtonSens(!mUndoStack.empty());
  mLayoutDlg.SetRedoButtonSens(!mRedoStack.empty());
}


// --- Cmd handling ---

void OLayoutController::_GetOneOrAllLangBibles(const OLayoutDlgCmd &layoutDlgCmd, std::vector<tstring> &langBibles)
{
  if (layoutDlgCmd.ApplyToAllLangBibles()) {
    mLayout.GetLangBibles(langBibles);
  }
  else {
    langBibles.clear();
    langBibles.push_back(layoutDlgCmd.LangBible());
  }
}

void OLayoutController::_Exec(OLayoutDlgCmd *pLayoutDlgCmd)
{
  assert(pLayoutDlgCmd);
  MSGDEBUG(_T("OLayoutController::_Exec - langBible=") << pLayoutDlgCmd->LangBible());
  // for [Cancel], remember position down/up to which undo stack must be cleared/filled
  // (remember position before push_back)
  if (mMinExecUndoStackLen > mUndoStack.size()) {
    mMinExecUndoStackLen = mUndoStack.size();
  }

  mUndoStack.push_back(pLayoutDlgCmd);
  pLayoutDlgCmd->Accept(*this);
  NLayoutDlgCmd::ClearStack(mRedoStack);
  _SetButtonSens(); // ***
}

void OLayoutController::Undo()
{
  if (!mUndoStack.empty()) {
    MSGDEBUG(_T("OLayoutController::Undo"));
    OLayoutDlgCmd *pLayoutDlgCmd = mUndoStack.back();
    assert(pLayoutDlgCmd);

    // adapt stacks
    mUndoStack.pop_back();
    mRedoStack.push_back(pLayoutDlgCmd);

    // undo
    mLayout.Assign(pLayoutDlgCmd->OldLayout());
    SetLangBible(pLayoutDlgCmd->LangBible()); // for user convenience, navigate to original langBible
    _UpdateView(); // ***
    _SetButtonSens(); // call this for Undo/Redo buttons; Undo() may optimize this away if no changes (?)
  }
}

void OLayoutController::Redo()
{
  if (!mRedoStack.empty()) {
    MSGDEBUG(_T("OLayoutController::Redo"));
    OLayoutDlgCmd *pLayoutDlgCmd = mRedoStack.back();
    assert(pLayoutDlgCmd);

    // adapt stacks
    mRedoStack.pop_back();
    mUndoStack.push_back(pLayoutDlgCmd);

    // redo
    pLayoutDlgCmd->Accept(*this);
    _SetButtonSens(); // call this for Undo/Redo buttons; Redo() may optimize this away if no changes (?)
  }
}


// --- ILayoutDlgCmdVisitor ---

void OLayoutController::VisitCmdBkCol(OLayoutDlgCmdBkCol &layoutDlgCmd)
{
  COLORREF col = layoutDlgCmd.NewBkCol();
  MSGDEBUG(_T("OLayoutController::VisitCmdBkCol ") << col);
  // check for change (avoid screen flickering when tabbing through dialog)
  if (col != mLayout.GetBkCol()) {
    mLayout.SetBkCol(col);
    mLayoutDlg.SetBkCol(col);
    _UpdateView(); // ***
  }
}

void OLayoutController::VisitCmdLineName(OLayoutDlgCmdLineName &layoutDlgCmd)
{
  MSGDEBUG(_T("OLayoutController::VisitCmdLineName ") << layoutDlgCmd.NewName());
  std::vector<tstring> langBibles;
  _GetOneOrAllLangBibles(layoutDlgCmd, langBibles);
  std::vector<string>::const_iterator it;
  bool bChanged = false;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const string &curLangBible = *it;
    mpLosungLayout = &mLayout.LosungLayout(curLangBible);
    ILineLayout &lineLayout = mpLosungLayout->LineLayout(layoutDlgCmd.LineType());
    // check for change (avoid screen flickering when tabbing through dialog)
    if (layoutDlgCmd.NewName() != lineLayout.GetName()) {
      lineLayout.SetName(layoutDlgCmd.NewName());
      bChanged = true;
    }
  }
  SetLangBible(layoutDlgCmd.LangBible()); // obsoletes mLayoutDlg.SetLineLayout(lineType, lineLayout);
  if (bChanged) {
    _UpdateView(); // ***
  }
}

void OLayoutController::VisitCmdLineSize(OLayoutDlgCmdLineSize &layoutDlgCmd)
{
  MSGDEBUG(_T("OLayoutController::VisitCmdLineSize ") << layoutDlgCmd.NewSize());
  std::vector<tstring> langBibles;
  _GetOneOrAllLangBibles(layoutDlgCmd, langBibles);
  std::vector<string>::const_iterator it;
  bool bChanged = false;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const string &curLangBible = *it;
    mpLosungLayout = &mLayout.LosungLayout(curLangBible);
    ILineLayout &lineLayout = mpLosungLayout->LineLayout(layoutDlgCmd.LineType());
    // check for change (avoid screen flickering when tabbing through dialog)
    if (layoutDlgCmd.NewSize() != lineLayout.GetSize()) {
      bChanged = true;
      lineLayout.SetSize(layoutDlgCmd.NewSize());
    }
  }
  SetLangBible(layoutDlgCmd.LangBible()); // obsoletes mLayoutDlg.SetLineLayout(lineType, lineLayout);
  if (bChanged) {
    _UpdateView(); // ***
  }
}

void OLayoutController::VisitCmdLineLayout(OLayoutDlgCmdLineLayout &layoutDlgCmd)
{
  MSGDEBUG(_T("OLayoutController::VisitCmdLineLayout"));
  std::vector<tstring> langBibles;
  _GetOneOrAllLangBibles(layoutDlgCmd, langBibles);
  std::vector<string>::const_iterator it;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const string &curLangBible = *it;
    mpLosungLayout = &mLayout.LosungLayout(curLangBible);
    ILineLayout &myLineLayout = mpLosungLayout->LineLayout(layoutDlgCmd.LineType());
    myLineLayout.Assign(layoutDlgCmd.NewLineLayout());
  }
  SetLangBible(layoutDlgCmd.LangBible()); // obsoletes mLayoutDlg.SetLineLayout(lineType, lineLayout);
  _UpdateView(); // ***
}

void OLayoutController::VisitCmdLosungLayout(OLayoutDlgCmdLosungLayout &layoutDlgCmd)
{
  MSGDEBUG(_T("OLayoutController::VisitCmdLosungLayout"));
  std::vector<tstring> langBibles;
  _GetOneOrAllLangBibles(layoutDlgCmd, langBibles);
  std::vector<string>::const_iterator it;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const string &curLangBible = *it;
    mpLosungLayout = &mLayout.LosungLayout(curLangBible);
    mpLosungLayout->Assign(layoutDlgCmd.NewLosungLayout());
  }
  SetLangBible(layoutDlgCmd.LangBible());
  mLayoutDlg.SetLosungLayout(*mpLosungLayout); // use mpLosungLayout from last iteration
  _UpdateView(); // ***
}


// --- Interface for dialog ---
// These methods are called by the dialog.

void OLayoutController::SetLangBible(const tstring &langBible) 
{
  MSGDEBUG(_T("OLayoutController::SetLangBible ") << langBible);
  mLangBible = langBible;
  mpLosungLayout = &mLayout.LosungLayout(langBible);

  mLayoutDlg.SetLangBible(langBible);
  mLayoutDlg.SetLosungLayout(*mpLosungLayout);
  mLayoutDlg.SetBkCol(mpLosungLayout->GetBkCol());
  // no need to _UpdateView() - no GUI change
}

void OLayoutController::SetAllLangBiblesMode(bool b) 
{
  mApplyToAllLangBibles = b;
  MSGDEBUG(_T("OLayoutController::SetAllLangBiblesMode b=") << b);
  mProfile.WriteBool(PRF_LAYOUT_ALL_LANGBIBLES, mApplyToAllLangBibles);
}

void OLayoutController::SelectBkCol()
{
  MSGDEBUG(_T("OLayoutController::SelectBkCol"));
  CColorDialog dlg;
  dlg.m_cc.rgbResult = mLayout.GetBkCol();
  dlg.m_cc.Flags |= CC_ANYCOLOR | CC_RGBINIT;
  if (IDOK == dlg.DoModal()) {
    COLORREF col = dlg.GetColor();
    if (col != mLayout.GetBkCol()) {
      _Exec(new OLayoutDlgCmdBkCol(mLangBible, mLayout, col));
    }
  }
}

void OLayoutController::SetLineFontName(ILosungLayout::OLineType lineType, const tstring &name)
{
  assert(mpLosungLayout);
  ILineLayout &lineLayout = mpLosungLayout->LineLayout(lineType);
  // check for change (avoid screen flickering when tabbing through dialog)
  if (mApplyToAllLangBibles || name != lineLayout.GetName()) {
    _Exec(new OLayoutDlgCmdLineName(mLangBible, lineType, mLayout, name, mApplyToAllLangBibles));
  }
}

void OLayoutController::SetLineFontSize(ILosungLayout::OLineType lineType, int size)
{
  assert(mpLosungLayout);
  ILineLayout &lineLayout = mpLosungLayout->LineLayout(lineType);
  // check for change (avoid screen flickering when tabbing through dialog)
  if (mApplyToAllLangBibles || size != lineLayout.GetSize()) {
    _Exec(new OLayoutDlgCmdLineSize(mLangBible, lineType, mLayout, size, mApplyToAllLangBibles));
  }
}

void OLayoutController::CopyLine(ILosungLayout::OLineType lineType)
{
  MSGDEBUG(_T("OLayoutController::CopyLine"));
  assert(mpLosungLayout);
  ILineLayout &lineLayout = mpLosungLayout->LineLayout(lineType);
  if (!mpLineLayoutCopy) {
    mpLineLayoutCopy = &NLayout::MakeLineLayout();
  }
  mpLineLayoutCopy->Assign(lineLayout);
  _SetButtonSens(); // ***
}

void OLayoutController::PasteLine(ILosungLayout::OLineType lineType)
{
  MSGDEBUG(_T("OLayoutController::PasteLine"));
  if (mpLosungLayout && mpLineLayoutCopy) {
    ILineLayout &lineLayout = mpLosungLayout->LineLayout(lineType);
    _Exec(new OLayoutDlgCmdLineLayout(mLangBible, lineType, mLayout, *mpLineLayoutCopy, mApplyToAllLangBibles));
  }
}

void OLayoutController::CopyLosung()
{
  MSGDEBUG(_T("OLayoutController::CopyLosung"));
  assert(mpLosungLayout);
  if (!mpLosungLayoutCopy) {
    mpLosungLayoutCopy = &NLayout::MakeLosungLayout();
  }
  mpLosungLayoutCopy->Assign(*mpLosungLayout);
  _SetButtonSens(); // ***
}

void OLayoutController::PasteLosung()
{
  MSGDEBUG(_T("OLayoutController::PasteLosung"));
  if (mpLosungLayout && mpLosungLayoutCopy) {
    _Exec(new OLayoutDlgCmdLosungLayout(mLangBible, mLayout, *mpLosungLayoutCopy, mApplyToAllLangBibles));
  }
}

void OLayoutController::Properties(ILosungLayout::OLineType lineType) 
{
  MSGDEBUG(_T("OLayoutController::Properties"));
  assert(mpLosungLayout);
  ILineLayout &lineLayout = mpLosungLayout->LineLayout(lineType);

  LOGFONT  logfont;
  memset(&logfont, 0, sizeof(LOGFONT));
  CWindowDC dc(NULL);

   _tcscpy(logfont.lfFaceName, lineLayout.GetName().c_str());
  logfont.lfHeight         = -::MulDiv(lineLayout.GetSize(), dc.GetDeviceCaps(LOGPIXELSY), 720);
  logfont.lfPitchAndFamily = lineLayout.GetPitchAndFamily();
  logfont.lfCharSet        = lineLayout.GetCharset();
  logfont.lfWeight         = lineLayout.GetWeight();
  logfont.lfItalic         = lineLayout.GetItalic();
  logfont.lfUnderline      = lineLayout.GetUnderline();
  logfont.lfStrikeOut      = lineLayout.GetStrikeOut();

  CFontDialog dlg(&logfont);
  dlg.m_cf.Flags |= CF_FORCEFONTEXIST;// | CF_FIXEDPITCHONLY;
  dlg.m_cf.rgbColors = lineLayout.GetCol();

  if (dlg.DoModal() == IDOK) {
    ILineLayout &newLineLayout = NLayout::MakeLineLayout();
    newLineLayout.SetName          ((LPCTSTR) dlg.GetFaceName());
    newLineLayout.SetSize          (dlg.GetSize());
    newLineLayout.SetPitchAndFamily(logfont.lfPitchAndFamily);
    newLineLayout.SetCharset       (logfont.lfCharSet);
    newLineLayout.SetWeight        (dlg.GetWeight());
    newLineLayout.SetItalic        (dlg.IsItalic());
    newLineLayout.SetUnderline     (dlg.IsUnderline());
    newLineLayout.SetStrikeOut     (dlg.IsStrikeOut());
    newLineLayout.SetCol           (dlg.GetColor());
    _Exec(new OLayoutDlgCmdLineLayout(mLangBible, lineType, mLayout, newLineLayout, mApplyToAllLangBibles));
    delete &newLineLayout;
  }
}

