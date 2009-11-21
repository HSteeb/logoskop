/* ----------------------------------------------------------------------
  Helmut Steeb
  Interface for layout dialog.
  $Id: ILayoutDlg.h 3853 2009-08-26 13:50:27Z helmut $

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
#if !defined(AFX_ILAYOUTDLG_H__INCLUDED_)
#define AFX_ILAYOUTDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../util/ILayout.h"

class ILayoutDlg
{
public:
  virtual ~ILayoutDlg() {};

  virtual void SetBkCol(COLORREF col) = 0;
  virtual void SetLangBibles(const std::vector<tstring> &langBibles) = 0;
  virtual void SetApplyToAllLangBibles(bool b) = 0;
  virtual void SetLangBible(const tstring &langBible) = 0;
  virtual void SetLangBibleAll() = 0;
  virtual void SetLineLayout(ILosungLayout::OLineType lineType, const ILineLayout &lineLayout) = 0;
  virtual void SetLosungLayout(const ILosungLayout &losungLayout) = 0;

  // not needed (copy is possible always):
  // virtual void SetLineCopyButtonSens(ILosungLayout::OLineType lineType, bool b) = 0;
  // virtual void SetLosungCopyButtonSens(bool b) = 0;
  virtual void SetLinePasteButtonSens(ILosungLayout::OLineType lineType, bool b) = 0;
  virtual void SetLosungPasteButtonSens(bool b) = 0;
  virtual void SetUndoButtonSens(bool b) = 0;
  virtual void SetRedoButtonSens(bool b) = 0;
};

#endif
