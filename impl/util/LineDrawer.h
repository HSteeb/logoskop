/* ----------------------------------------------------------------------
  Helmut Steeb
  LineDrawer - displays lines using ILineLayout passed in
  $Id: LineDrawer.h 3932 2009-09-28 21:58:25Z helmut $

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

#if !defined(_LINEDRAWER_H__INCLUDED_)
#define _LINEDRAWER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../util/ILineDrawer.h"

class OBidi;

class CLineDrawer : public ILineDrawer
{
public:
	CLineDrawer(OBidi &bidi);
	virtual ~CLineDrawer();
  
  virtual void Start(long y);

  virtual void DrawLines(CDC &dc, CRect &clipRect, const ILineLayout &lineLayout, const wstring &text);

  virtual long GetY() const;
  virtual void IncY(int y);
  virtual long GetMaxLen() const;

  // UseUSP10:
  bool UseUSP10() const;
  void UseUSP10(bool b);

protected:
  // GetNextLine:
  //
  // Use like:
  //
  //   wchar_t *pStart = text;
  //   wchar_t *pEnd = 0;
  //   wchar_t *pPrintableEnd;
  //   while (GetNextLine(pStart, pPrintableEnd, pEnd, text + strlen(text)) { };
  //
  // On first call, 
  // - pStart must be set to the start of the respective string.
  // Returns true if the indexes are valid (i.e. there is text left, even \n only).
  //   pStart is first printable character, 
  //   pEnd past (optional \r) and \n (or at end of text).
  // On subsequent calls, the indexes must be as from the previous call.
  bool _GetNextLine(const wchar_t *&pStart, const wchar_t *&pEnd, const wchar_t *&pTextEnd);

  void _DrawLine(CDC &dc, CRect &clipRect, const wchar_t *pStart, const wchar_t *pEnd);
  CFont *_SelectInto(CDC &dc, const ILineLayout &lineLayout);

private:
  long mMaxLen;
  long mLeft;
  long mY;
  OBidi &mBidi;
  CFont *mpFont;

  // mbUseUSP10==true triggers use of USP10.dll (ScriptStringOut).
  // mbUseUSP10==false triggers use TextOutW with OBidi reordering.
  bool mbUseUSP10;
};


#endif
