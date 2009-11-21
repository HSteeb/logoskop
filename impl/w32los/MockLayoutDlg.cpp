/* ----------------------------------------------------------------------
  Helmut Steeb
  Mock class for layout dialog.
  $Id: MockLayoutDlg.cpp 3905 2009-09-12 11:30:16Z helmut $

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _TEST

// === OMockLayoutDlg ===========================================

#include "MockLayoutDlg.h"
#include "../util/ILayout.h"
#include "../util/IProfile.h"
#include "../util/Layout.h"
#include "ILayoutDlg.h"
#include "LayoutController.h"
#include <assert.h>

// OMockProfile

class OMockProfile : public IProfile {
public:
  OMockProfile() {};
  virtual ~OMockProfile() {};

  virtual unsigned long GetString  (const tstring &key, const tstring &defaultValue, const int maxSize, tstring &value, const tstring &section = tstring(_T(""))) { return 0; };
  virtual unsigned int  GetInt     (const tstring &key, const unsigned int defaultValue, const tstring &section = tstring(_T(""))) { return 0; };
  virtual bool          GetBool    (const tstring &key, const bool defaultValue, const tstring &section = tstring(_T(""))) { return false; };

  virtual bool          WriteString(const tstring &key, const tstring &value, const tstring &section = tstring(_T(""))) { return true; };
  virtual bool          WriteInt   (const tstring &key, const unsigned int value, const tstring &section = tstring(_T(""))) { return true; };
  virtual bool          WriteBool  (const tstring &key, const bool value, const tstring &section = tstring(_T(""))) { return true; };

  virtual bool          DeleteString(const tstring &key, const tstring &section = tstring(_T(""))) { return true; };
};

// OMockLayoutDlg
// represents data as seen in a dialog: langBible string + settings for one langBible

class OMockLayoutDlg : public ILayoutDlg
{
public:
  OMockLayoutDlg();
  ~OMockLayoutDlg();

  // === ILayoutDlg ===

  virtual void SetBkCol(COLORREF col);
  virtual void SetLangBibles(const std::vector<tstring> &langBibles);
  virtual void SetApplyToAllLangBibles(bool b);
  virtual void SetLangBible(const tstring &langBible);
  virtual void SetLangBibleAll();

  virtual void SetLineLayout(ILosungLayout::OLineType lineType, const ILineLayout &lineLayout);
  virtual void SetLosungLayout(const ILosungLayout &losungLayout);

  virtual void SetLinePasteButtonSens(ILosungLayout::OLineType lineType, bool b);
  virtual void SetLosungPasteButtonSens(bool b);
  virtual void SetUndoButtonSens(bool b);
  virtual void SetRedoButtonSens(bool b);

  // === Get ===

  COLORREF GetBkCol() const;
  tstring  GetLangBible() const;
  tstring  GetLineFontName(ILosungLayout::OLineType lineType);
  int      GetLineFontSize(ILosungLayout::OLineType lineType);

protected:
  ILosungLayout &_LosungLayout() const;
  ILineLayout &_LineLayout(ILosungLayout::OLineType lineType) const;

private:
  tstring mLangBible;
  COLORREF mBkCol;
  ILosungLayout *mpLosungLayout;
};

// === OMockLayoutDlg implementation ============================


OMockLayoutDlg::OMockLayoutDlg()
: mBkCol(0)
{
  mpLosungLayout = &NLayout::MakeLosungLayout();
  assert(mpLosungLayout);
}

OMockLayoutDlg::~OMockLayoutDlg()
{
  delete mpLosungLayout;
}


// === ILayoutDlg ===

void OMockLayoutDlg::SetBkCol(COLORREF col)
{
  mBkCol = col;
}

void OMockLayoutDlg::SetLangBibles(const std::vector<tstring> &langBibles)
{
}

void OMockLayoutDlg::SetApplyToAllLangBibles(bool b)
{
}


void OMockLayoutDlg::SetLangBible(const tstring &langBible)
{
  mLangBible = langBible;
}

void OMockLayoutDlg::SetLangBibleAll()
{
  mLangBible = "";
}

ILosungLayout &OMockLayoutDlg::_LosungLayout() const
{
  assert(mpLosungLayout);
  return *mpLosungLayout;
}

ILineLayout &OMockLayoutDlg::_LineLayout(ILosungLayout::OLineType lineType) const
{
  return mpLosungLayout->LineLayout(lineType);
}

void OMockLayoutDlg::SetLineLayout(ILosungLayout::OLineType lineType, const ILineLayout &lineLayout)
{
  _LineLayout(lineType).Assign(lineLayout);
}


void OMockLayoutDlg::SetLosungLayout(const ILosungLayout &losungLayout)
{
  mpLosungLayout->Assign(losungLayout);
}

void OMockLayoutDlg::SetLinePasteButtonSens(ILosungLayout::OLineType lineType, bool b)
{
}

void OMockLayoutDlg::SetLosungPasteButtonSens(bool b)
{
}

void OMockLayoutDlg::SetUndoButtonSens(bool b)
{
}

void OMockLayoutDlg::SetRedoButtonSens(bool b)
{
}


// === Get ===

COLORREF OMockLayoutDlg::GetBkCol() const
{
  return mBkCol;
}

tstring  OMockLayoutDlg::GetLangBible() const
{
  return mLangBible;
}

tstring  OMockLayoutDlg::GetLineFontName(ILosungLayout::OLineType lineType)
{
  return _LineLayout(lineType).GetName();
}

int      OMockLayoutDlg::GetLineFontSize(ILosungLayout::OLineType lineType)
{
  return _LineLayout(lineType).GetSize();
}



// === TEST =========================================================================


bool NMockLayoutDlg::SelfTest()
{
  // Test effect of calls to OLayoutController on ILayout and on ILayoutDlg

  bool bRes = true;
  MSGDEBUG("--- Start NMockLayoutDlg::SelfTest ----------------------------");

  const tstring S2K = "de_Schlachter2000";
  const tstring ESV = "en_EnglishStandardVersion";
  std::vector<tstring> langBibles;
  langBibles.push_back(S2K);
  langBibles.push_back(ESV);

  OMockLayoutDlg dlg;
  ILayout &layout = NLayout::MakeLayout(); // ***
  ILosungLayout &S2KLayout = layout.LosungLayout(S2K);

  OMockProfile profile;

  std::vector<OLayoutDlgCmd*> undoStack; // ***
  std::vector<OLayoutDlgCmd*> redoStack; // ***
  OLayoutController ctrl(profile, dlg, langBibles, layout, undoStack, redoStack);
  ctrl.Init();

  // *** SetAllLangBiblesMode(false)
  ctrl.SetAllLangBiblesMode(false);

  // check Init() result
  bRes &= (dlg.GetLangBible() == S2K);

  // set TL
  ctrl.SetLineFontName(ILosungLayout::ltTL, "test1");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test1");
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test1");

  ctrl.SetLineFontSize(ILosungLayout::ltTL, 11);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 11);
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltTL) == 11);
  
  // set IL
  ctrl.SetLineFontName(ILosungLayout::ltIL, "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test2");
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltIL) == "test2");

  ctrl.SetLineFontSize(ILosungLayout::ltIL, 22);
  bRes &= (layout.LosungLayout(S2K).LineLayout(ILosungLayout::ltIL).GetSize() == 22);
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 22);

  // copy IL into TL
  ctrl.CopyLine(ILosungLayout::ltIL);
  ctrl.PasteLine(ILosungLayout::ltTL);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 22);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 22);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test2");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltTL) == 22);

  // copy/paste Losung
  ctrl.CopyLosung();
  ctrl.SetLineFontName(ILosungLayout::ltTL, "test3");
  ctrl.SetLineFontSize(ILosungLayout::ltIL, 33);
  ctrl.PasteLosung();
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 22);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 22);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test2");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 22);

  // set langBible
  ctrl.SetLangBible(ESV);
  ILosungLayout *pESVLayout = layout.FindLosungLayout(ESV);
  ASSERT(pESVLayout);
  ILosungLayout &ESVLayout = *pESVLayout;
  bRes &= (dlg.GetLangBible() == ESV);

  ctrl.SetLineFontName(ILosungLayout::ltTL, "test4");
  ctrl.SetLineFontSize(ILosungLayout::ltIL, 44);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 22);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 22);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test4");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 44);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test4");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 44);

  ctrl.SetLangBible(S2K);
  bRes &= (dlg.GetLangBible() == S2K);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test2");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 22);

  // *** Undo/Redo

  ctrl.SetLangBible(ESV);
  ctrl.SetLineFontName(ILosungLayout::ltTL, "test8");
  ctrl.SetLineFontSize(ILosungLayout::ltIL, 88);
  ctrl.SetLangBible(S2K);
  ctrl.Undo();
  ctrl.Undo();
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 22);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 22);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test4");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 44);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test4");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 44);
  bRes &= (dlg.GetLangBible() == ESV); // switched back!

  ctrl.SetLangBible(S2K);
  ctrl.Redo();
  ctrl.Redo();
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 22);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test2");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 22);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test8");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 88);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test8");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 88);
  bRes &= (dlg.GetLangBible() == ESV); // switched back!


  // *** SetAllLangBiblesMode(true)

  ctrl.SetAllLangBiblesMode(true);

  // set TL
  ctrl.SetLineFontName(ILosungLayout::ltTL, "test5");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test5");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test2");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test5");
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test5");

  ctrl.SetLineFontSize(ILosungLayout::ltTL, 55);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 55);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 22);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 55);
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltTL) == 55);
  
  // set IL
  ctrl.SetLineFontName(ILosungLayout::ltIL, "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test5");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test5");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltIL) == "test6");

  // copy IL into TL
  ctrl.CopyLine(ILosungLayout::ltIL);
  ctrl.PasteLine(ILosungLayout::ltTL);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 22);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 88);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test6");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltTL) == 88);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltIL) == "test6");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 88);

  // copy/paste Losung
  ctrl.CopyLosung();
  ctrl.SetLineFontName(ILosungLayout::ltTL, "test3");
  ctrl.SetLineFontSize(ILosungLayout::ltIL, 33);
  ctrl.PasteLosung();
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 88);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 88);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test6");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltTL) == 88);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltIL) == "test6");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 88);

  // set langBible
  ctrl.SetLangBible(ESV);
  bRes &= (dlg.GetLangBible() == ESV);

  ctrl.SetLineFontName(ILosungLayout::ltTL, "test7");
  ctrl.SetLineFontSize(ILosungLayout::ltIL, 77);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test7");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 77);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test7");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 77);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test7");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 77);

  ctrl.SetLangBible(S2K);
  bRes &= (dlg.GetLangBible() == S2K);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test7");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 77);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test7");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 77);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test7");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 77);

  // *** Undo/Redo

  ctrl.SetLangBible(ESV);
  ctrl.SetLineFontName(ILosungLayout::ltTL, "test9");
  ctrl.SetLineFontSize(ILosungLayout::ltIL, 99);
  ctrl.SetLangBible(S2K);
  ctrl.Undo();
  ctrl.Undo();
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test7");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 77);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test7");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 77);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test7");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 77);
  bRes &= (dlg.GetLangBible() == ESV); // switched back!

  ctrl.SetLangBible(S2K);
  ctrl.Redo();
  ctrl.Redo();
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test9");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (S2KLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 99);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetName() == "test9");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltTL).GetSize() == 88);
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetName() == "test6");
  bRes &= (ESVLayout.LineLayout(ILosungLayout::ltIL).GetSize() == 99);
  bRes &= (dlg.GetLineFontName(ILosungLayout::ltTL) == "test9");
  bRes &= (dlg.GetLineFontSize(ILosungLayout::ltIL) == 99);
  bRes &= (dlg.GetLangBible() == ESV); // switched back!

  NLayoutDlgCmd::ClearStack(undoStack);
  NLayoutDlgCmd::ClearStack(redoStack);
  delete &layout;
  return bRes;
}

#endif //_TEST
