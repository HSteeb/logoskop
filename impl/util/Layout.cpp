/* ----------------------------------------------------------------------
  Helmut Steeb
  Data structure for layout.
  $Id: Layout.cpp 3905 2009-09-12 11:30:16Z helmut $

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
#include "ILayout.h"
#include "Layout.h"
#include "ProfileDef.h"
#include <assert.h>
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === OLineLayout ===

class OLineLayout : public ILineLayout
{
public:
  OLineLayout();

  virtual const tstring GetName() const;
  virtual void SetName(const tstring &name);

  virtual int GetSize() const;
  virtual void SetSize(int size);

  virtual COLORREF GetCol() const;
  virtual void SetCol(COLORREF col);

  virtual BYTE GetCharset() const;
  virtual void SetCharset(BYTE b);

  virtual BYTE GetPitchAndFamily() const;
  virtual void SetPitchAndFamily(BYTE b);

  virtual LONG GetWeight() const;
  virtual void SetWeight(LONG l);

  virtual BOOL GetItalic() const;
  virtual void SetItalic(BOOL b);

  virtual BOOL GetUnderline() const;
  virtual void SetUnderline(BOOL b);

  virtual BOOL GetStrikeOut() const;
  virtual void SetStrikeOut(BOOL b);

  virtual void Assign(const ILineLayout &lineLayout);

private:
  tstring mName;
  int mSize;
  COLORREF mCol;
  BYTE mCharset;
  BYTE mPitchAndFamily;
  LONG mWeight;
  BOOL mItalic;
  BOOL mUnderline;
  BOOL mStrikeOut;
};

OLineLayout::OLineLayout()
{
  mName = DEFAULT_FONT_NAME;
  mSize = DEFAULT_POINT_SIZE;
  mCol = DEFAULT_FONT_COLOUR;
  mCharset = DEFAULT_CHARSET;
  mPitchAndFamily = DEFAULT_PITCH || FF_DONTCARE;
  mWeight = FW_DONTCARE;
  mItalic = 0;
  mUnderline = 0;
  mStrikeOut = 0;
}

const tstring OLineLayout::GetName() const
{
  return mName;
}

void OLineLayout::SetName(const tstring &name)
{
  mName = name;
}

int OLineLayout::GetSize() const
{
  return mSize;
}

void OLineLayout::SetSize(int size)
{
  mSize = size;
}

COLORREF OLineLayout::GetCol() const
{
  return mCol;
}

void OLineLayout::SetCol(COLORREF col)
{
  mCol = col;
}

BYTE OLineLayout::GetCharset() const
{
  return mCharset;
}

void OLineLayout::SetCharset(BYTE b)
{
  mCharset = b;
}

BYTE OLineLayout::GetPitchAndFamily() const
{
  return mPitchAndFamily;
}

void OLineLayout::SetPitchAndFamily(BYTE b)
{
  mPitchAndFamily = b;
}

LONG OLineLayout::GetWeight() const
{
  return mWeight;
}

void OLineLayout::SetWeight(LONG l)
{
  mWeight = l;
}

BOOL OLineLayout::GetItalic() const
{
  return mItalic;
}

void OLineLayout::SetItalic(BOOL b)
{
  mItalic = b;
}

BOOL OLineLayout::GetUnderline() const
{
  return mUnderline;
}

void OLineLayout::SetUnderline(BOOL b)
{
  mUnderline = b;
}

BOOL OLineLayout::GetStrikeOut() const
{
  return mStrikeOut;
}

void OLineLayout::SetStrikeOut(BOOL b)
{
  mStrikeOut = b;
}

void OLineLayout::Assign(const ILineLayout &lineLayout)
{
//TODO Low 2009-08-15 HS - simpler/more performant default operator= ...?
  mName = lineLayout.GetName();
  mSize = lineLayout.GetSize();
  mCol  = lineLayout.GetCol();
  mCharset = lineLayout.GetCharset();
  mPitchAndFamily  = lineLayout.GetPitchAndFamily();
  mWeight  = lineLayout.GetWeight();
  mItalic  = lineLayout.GetItalic();
  mUnderline  = lineLayout.GetUnderline();
  mStrikeOut  = lineLayout.GetStrikeOut();
}


// === OLosungLayout ===

class OLosungLayout : public ILosungLayout
{
public:
  OLosungLayout::OLosungLayout();

  // SetDefaults:
  // set (implementation dependent) nice defaults.
  virtual void SetDefaults();

  virtual ILineLayout &LineLayout(OLineType lineType);
  virtual const ILineLayout &OLosungLayout::LineLayout(OLineType lineType) const;

  virtual COLORREF GetBkCol() const;
  virtual void SetBkCol(COLORREF col);

  virtual void Assign(const ILosungLayout &losungLayout);

private:
  OLineLayout mLineLayouts[4];
  COLORREF mBkCol;
};

OLosungLayout::OLosungLayout()
{
  mBkCol = DEFAULT_BKCOLOR;
}

void OLosungLayout::SetDefaults()
{
  ILineLayout *pLineLayout = 0;
#define _init(lineLayout, size, col)\
  pLineLayout = &lineLayout;\
  pLineLayout->SetSize(size);\
  pLineLayout->SetCol(col);

  // some nice defaults (distinguish parts)
  _init(mLineLayouts[ltTL], DEFAULT_POINT_SIZE_TL, DEFAULT_FONT_COLOUR_TL);
  _init(mLineLayouts[ltIL], DEFAULT_POINT_SIZE_IL, DEFAULT_FONT_COLOUR_IL);
  _init(mLineLayouts[ltL ], DEFAULT_POINT_SIZE_L , DEFAULT_FONT_COLOUR_L );
  _init(mLineLayouts[ltSL], DEFAULT_POINT_SIZE_SL, DEFAULT_FONT_COLOUR_SL);
}

ILineLayout &OLosungLayout::LineLayout(OLineType lineType)
{
  assert(0 <= lineType);
  assert(lineType <= 3);
  return mLineLayouts[lineType];
}

const ILineLayout &OLosungLayout::LineLayout(OLineType lineType) const
{
  assert(0 <= lineType);
  assert(lineType <= 3);
  return mLineLayouts[lineType];
}

COLORREF OLosungLayout::GetBkCol() const
{
  return mBkCol;
}

void OLosungLayout::SetBkCol(COLORREF col)
{
  mBkCol = col;
}

void OLosungLayout::Assign(const ILosungLayout &losungLayout)
{
  mLineLayouts[ltTL].Assign(losungLayout.LineLayout(ltTL));
  mLineLayouts[ltIL].Assign(losungLayout.LineLayout(ltIL));
  mLineLayouts[ltL ].Assign(losungLayout.LineLayout(ltL ));
  mLineLayouts[ltSL].Assign(losungLayout.LineLayout(ltSL));
  mBkCol = losungLayout.GetBkCol();
}


// === OLayout ===

class OLayout : public ILayout
{
public:
  OLayout();
  ~OLayout();

  // SetDefaults:
  // set (implementation dependent) nice defaults.
  virtual void SetDefaults();

  virtual const tstring &GetName() const;
  virtual void SetName(const tstring &name);

  virtual COLORREF GetBkCol() const;
  virtual void SetBkCol(COLORREF col);

  virtual ILosungLayout *FindLosungLayout(const tstring &langBible) const;
  virtual ILosungLayout &LosungLayout(const tstring &langBible);

  virtual bool HasLangBibles() const;
  virtual void GetLangBibles(std::vector<tstring> &langBibles) const;

  virtual void Assign(const ILayout &layout);

private:
  std::map<tstring, OLosungLayout*> mLosungLayouts;
  tstring mName;
  COLORREF mBkCol;
};

OLayout::OLayout()
{
  mBkCol = DEFAULT_BKCOLOR;
}

OLayout::~OLayout()
{
  std::map<tstring, OLosungLayout*>::iterator it;
  for (it = mLosungLayouts.begin(); it != mLosungLayouts.end(); ++it) {
    delete it->second;
  }
}

void OLayout::SetDefaults()
{
  std::map<tstring, OLosungLayout*>::const_iterator it;
  for (it = mLosungLayouts.begin(); it != mLosungLayouts.end(); ++it) {
    it->second->SetDefaults();
  }
}

const tstring &OLayout::GetName() const
{
  return mName;
}

void OLayout::SetName(const tstring &name)
{
  mName = name;
}

COLORREF OLayout::GetBkCol() const
{
  return mBkCol;
}
 
void OLayout::SetBkCol(COLORREF col)
{
  mBkCol = col;
  // BkCol in ILosungLayout is a duplicate from ILayout, see [BkCol]
  std::map<tstring, OLosungLayout*>::iterator it;
  for (it = mLosungLayouts.begin(); it != mLosungLayouts.end(); ++it) {
    it->second->SetBkCol(col);
  }
}

ILosungLayout *OLayout::FindLosungLayout(const tstring &langBible) const
{
  std::map<tstring, OLosungLayout*>::const_iterator it = mLosungLayouts.find(langBible);
  return (it != mLosungLayouts.end()) ? it->second : 0;
}

// LosungLayout:
// if an ILayout contains an ILosungLayout for langBible, returns it,
// otherwise creates a new one and returns it.

ILosungLayout &OLayout::LosungLayout(const tstring &langBible)
{
  ILosungLayout *pLosungLayout = FindLosungLayout(langBible);
  if (!pLosungLayout) {
    assert(!mLosungLayouts[langBible]);
    pLosungLayout = mLosungLayouts[langBible] = new OLosungLayout();
    assert(mLosungLayouts[langBible]);
    pLosungLayout->SetBkCol(mBkCol); // see [BkCol]
  }
  return *pLosungLayout;
}

bool OLayout::HasLangBibles() const
{
  return !mLosungLayouts.empty();
}

void OLayout::GetLangBibles(std::vector<tstring> &langBibles) const
{
  langBibles.clear();
  std::map<tstring, OLosungLayout*>::const_iterator it;
  for (it = mLosungLayouts.begin(); it != mLosungLayouts.end(); ++it) {
    langBibles.push_back(it->first);
  }
}

void OLayout::Assign(const ILayout &layout)
{
  std::vector<tstring> langBibles;
  layout.GetLangBibles(langBibles);
  std::vector<tstring>::const_iterator it;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const tstring &langBible = *it;
    ILosungLayout *pLosungLayout = layout.FindLosungLayout(langBible);
    assert(pLosungLayout);
    ILosungLayout &myLosungLayout = LosungLayout(langBible);
    myLosungLayout.Assign(*pLosungLayout);
  }
  mBkCol = layout.GetBkCol();
  mName = layout.GetName();
}


// === NLayout ===

ILineLayout &NLayout::MakeLineLayout()
{
  ILineLayout *pLineLayout = new OLineLayout();
  assert(pLineLayout);
  return *pLineLayout;
}

ILosungLayout &NLayout::MakeLosungLayout()
{
  ILosungLayout *pLosungLayout = new OLosungLayout();
  assert(pLosungLayout);
  return *pLosungLayout;
}

ILayout &NLayout::MakeLayout()
{
  ILayout *pLayout = new OLayout();
  assert(pLayout);
  return *pLayout;
}



// === TEST ====================================================

#ifdef _TEST

bool NLayout::SelfTest()
{

  bool bRes = true;
  MSGDEBUG("--- Start NLayout::SelfTest ----------------------------");

  ILayout &layout = MakeLayout();
  const tstring de("de_Schlachter2000");
  const tstring en("en_EnglishStanardVersion");
  const tstring arial("Arial");
  const tstring helv("Helvetica");
  const int size = 133;
  const COLORREF col = 0xffeeff;

  bRes &= (0 == layout.FindLosungLayout(de));

  ILosungLayout &losungLayout = layout.LosungLayout(de);
  ILosungLayout *pLosungLayout = layout.FindLosungLayout(de);
  bRes &= (0 != pLosungLayout);

  ILineLayout &lineLayoutTL = losungLayout.LineLayout(ILosungLayout::ltTL);
  ILineLayout &lineLayoutIL = losungLayout.LineLayout(ILosungLayout::ltIL);
  ILineLayout &lineLayoutL  = losungLayout.LineLayout(ILosungLayout::ltL);
  ILineLayout &lineLayoutSL = losungLayout.LineLayout(ILosungLayout::ltSL);
  bRes &= (!lineLayoutTL.GetName().empty()); // assume it is initialized
  bRes &= (0 != lineLayoutTL.GetSize());

  lineLayoutTL.SetName(arial);
  bRes &= (arial == lineLayoutTL.GetName());
  bRes &= (!lineLayoutIL.GetName().empty());
  bRes &= (!lineLayoutL.GetName().empty());
  bRes &= (!lineLayoutSL.GetName().empty());

  lineLayoutTL.SetSize(size);
  bRes &= (size == lineLayoutTL.GetSize());
  bRes &= (size != lineLayoutIL.GetSize());
  bRes &= (size != lineLayoutL.GetSize());
  bRes &= (size != lineLayoutSL.GetSize());

  lineLayoutTL.SetCol(col);
  bRes &= (col == lineLayoutTL.GetCol());
  bRes &= (col != lineLayoutIL.GetCol());
  bRes &= (col != lineLayoutL.GetCol());
  bRes &= (col != lineLayoutSL.GetCol());

  delete &layout;
  MSGDEBUG("--- End NLayout::SelfTest ----------------------------");
  return bRes;
}

#endif

