/* ----------------------------------------------------------------------
  Helmut Steeb
  Interfaces for layout data structures.
  $Id: ILayout.h 3905 2009-09-12 11:30:16Z helmut $

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
#if !defined(AFX_ILAYOUT_H__INCLUDED_)
#define AFX_ILAYOUT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// === ILineLayout ===

class ILineLayout
{
public:
  virtual ~ILineLayout() {};

  virtual const tstring GetName() const = 0;
  virtual void SetName(const tstring &name) = 0;

  // Recommendation for implementations of GetSize()/SetSize():
  // size should be in tenths of a point.
  // Cf. CFontDialog::GetSize which handles tenths of a point,
  // on Windows NT, the dialog does not allow input of eg. 10.5 pt
  // (but probably on newer versions?).
  virtual int GetSize() const = 0;
  virtual void SetSize(int size) = 0;

  virtual COLORREF GetCol() const = 0;
  virtual void SetCol(COLORREF col) = 0;

  virtual BYTE GetCharset() const = 0;
  virtual void SetCharset(BYTE b) = 0;

  virtual BYTE GetPitchAndFamily() const = 0;
  virtual void SetPitchAndFamily(BYTE b) = 0;

  virtual LONG GetWeight() const = 0;
  virtual void SetWeight(LONG l) = 0;

  virtual BOOL GetItalic() const = 0;
  virtual void SetItalic(BOOL b) = 0;

  virtual BOOL GetUnderline() const = 0;
  virtual void SetUnderline(BOOL b) = 0;

  virtual BOOL GetStrikeOut() const = 0;
  virtual void SetStrikeOut(BOOL b) = 0;

  virtual void Assign(const ILineLayout &lineLayout) = 0;
};


// === ILosungLayout ===

class ILosungLayout
{
public:
  virtual ~ILosungLayout() {};

  // SetDefaults:
  // set (implementation dependent) nice defaults.
  virtual void SetDefaults() = 0;

  typedef enum { ltTL, ltIL, ltL, ltSL } OLineType;

  virtual ILineLayout &LineLayout(OLineType lineType) = 0;
  virtual const ILineLayout &LineLayout(OLineType lineType) const = 0;

  virtual COLORREF GetBkCol() const = 0;
  virtual void SetBkCol(COLORREF col) = 0;

  virtual void Assign(const ILosungLayout &losungLayout) = 0;
};


// === ILayout ===

class ILayout
{
public:
  virtual ~ILayout() {};

  // SetDefaults:
  // set (implementation dependent) nice defaults.
  virtual void SetDefaults() = 0;

  virtual const tstring &GetName() const = 0;
  virtual void SetName(const tstring &name) = 0;

  virtual COLORREF GetBkCol() const = 0;
  virtual void SetBkCol(COLORREF col) = 0;

  virtual ILosungLayout *FindLosungLayout(const tstring &langBible) const = 0;
  virtual ILosungLayout &LosungLayout(const tstring &langBible) = 0;

  virtual bool HasLangBibles() const = 0;
  virtual void GetLangBibles(std::vector<tstring> &langBibles) const = 0;

  // Assign:
  // for each LosungLayout in layout, puts it into this (creates if necessary),
  // and assigns all other properties.
  virtual void Assign(const ILayout &layout) = 0;
};

#endif
