/* ----------------------------------------------------------------------
  Helmut Steeb
  LineDrawer - displays lines using ILineLayout passed in
  $Id: LineDrawer.cpp 3932 2009-09-28 21:58:25Z helmut $

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
#include <string>
#include "LineDrawer.h"
#include "../util/DspPlain.h"
#include "../LosFile/LosFileConst.h" // XML tags
#include <assert.h>
#include <algorithm> // find
#include "../encoding/Bidi.h"
#include "../encoding/fribidi.h" // fribidi_get_type
#include "ILayout.h"

static wstring wlf = L"\r\n";
#define LCR L'\r'
#define LLF L'\n'

static bool sbUSP10Initialized = false;

// === CLineDrawer ===

CLineDrawer::CLineDrawer(OBidi &bidi)
: mY(0)
, mMaxLen(0)
, mLeft(0)
, mBidi(bidi)
, mpFont(0)
, mbUseUSP10(false) // must be false - USP10 not usable without calling UseUSP10(true)
{
}

CLineDrawer::~CLineDrawer()
{
  delete mpFont;
}

bool CLineDrawer::UseUSP10() const
{
  return mbUseUSP10;
}

void CLineDrawer::UseUSP10(bool b)
{
  if (!b) {
    mbUseUSP10 = false;
  }
  else { 
    if (!sbUSP10Initialized) {
      dspplain::LoadUSP10();
      // reset flag - even if loading failed (avoid endless retry)
      sbUSP10Initialized = true;
      if (!dspplain::HasUSP10()) {
        MSGINFO("LineDrawer: USP10 not loaded.");
      }
      else {
        MSGINFO("LineDrawer: USP10 loaded.");
      }
    }
    // set false if dspplain failed
    mbUseUSP10 = dspplain::HasUSP10();
  }
}



void CLineDrawer::Start(long y)
{
  mY = y;
  mMaxLen = 0;
}

long CLineDrawer::GetY() const
{
  return mY;
}

void CLineDrawer::IncY(int y)
{
  if (y >= -mY) {
    mY += y;
  }
}

long CLineDrawer::GetMaxLen() const
{
  return mMaxLen;
}

bool CLineDrawer::_GetNextLine(const wchar_t *&pStart, const wchar_t *&pEnd, const wchar_t *&pTextEnd)
{
  assert(pStart);
  assert(pTextEnd);

  if (pStart >= pTextEnd) {
    return false;
  }
  // assert(pStart < pTextEnd);

  pEnd = std::find(pStart, pTextEnd, L'\n');
  // assert(pStart < pTextEnd && (pEnd == pTextEnd || pStart <= pEnd && pEnd < pTextEnd && pEnd[0]==L'\n'));

  if (pEnd < pTextEnd) {
    ++pEnd; // let end point behind \n
    // assert(pStart < pTextEnd && pStart < pEnd && pEnd <= pTextEnd && pEnd[-1]==L'\n');
  }
  // assert(pStart < pTextEnd && (pEnd == pTextEnd || pStart < pEnd && pEnd <= pTextEnd && pEnd[-1]==L'\n'));
  return true;
}

void Remove(wstring &s, const wstring &sub)
{
  wstring::size_type pos = 0;
  for (pos = s.find(sub, pos); pos != wstring::npos; pos = s.find(sub)) {
    s.erase(pos, sub.length());
  }
}

wstring Widen(wstring &s)
{
  // "abc" --> " a b c "
  wstring res = L" ";
  unsigned int len = s.length();
  for (unsigned int i = 0; i < len; ++i) {
    wchar_t ch = s[i];
    res.append(1, ch);
    res.append(1, L' ');
  }
  return res;
}

#define TEST_LOGTOVIS 0

// HS 2005-01-06: note - '#define TEST_LOGTOVIS 0' yields true for '#ifdef TEST_LOGTOVIS'!
#if TEST_LOGTOVIS
void DumpBuffer(const wstring &buffer)
{
  if (buffer.length()) { // write content as UCN
#ifdef _DEBUG
#define FILENAME "d:\\temp\\log2visDebug.txt"
#else
#define FILENAME "d:\\temp\\log2visRelease.txt"
#endif
    FILE *fp = fopen(FILENAME, "w");
    if (fp) {
      for (unsigned int i = 0; i < buffer.length(); ++i)  {
        fprintf(fp, "\\u%04x", (int)buffer[i]);
      }
      fclose(fp);
    }
  }
}
#define DUMPBUFFER(b) DumpBuffer(b)
#else
#define DUMPBUFFER(b) 
#endif



void CLineDrawer::_DrawLine(CDC &dc, CRect &clipRect, const wchar_t *pStart, const wchar_t *pEnd)
{
  // Concept:
  // - buffer contains the string which is manipulated as needed,
  // - pStart/pEnd are used for drawing and are redirected to point into buffer.
  wstring buffer(pStart, pEnd);

  {
    // --- process GUI_EM and GUI_X ---
    // - before log to vis conversion (avoid isolating ligature'd data)
    // - before checking line length (apply line length to widened data)

    // drop <X>
    ::Remove(buffer, sGUI_X_OPEN);
    ::Remove(buffer, sGUI_X_CLOSE);

    // add spaces in range between <EM> and </EM>
    unsigned int emLen = sGUI_EM.length();
    wstring::size_type pos = 0;
    for (pos = buffer.find(sGUI_EM, pos); pos != wstring::npos; pos = buffer.find(sGUI_EM, pos)) {
      wstring::size_type pos2 = buffer.find(sGUI_EM, pos + emLen);
      if (pos2 == wstring::npos) {
        // error - unmatched EM tag - drop
        buffer.erase(pos, emLen);
      }
      else {
        wstring::size_type end = pos + emLen;
        buffer = buffer.substr(0, pos) 
          + ::Widen(buffer.substr(end, pos2 - end))
          + buffer.substr(pos2 + emLen);
      }
    }
  }


  // --- redirect pointers ---
  pStart = buffer.c_str();
  pEnd   = pStart + buffer.length();

  // compute height of output (before removing \r, include \n!!!)
  // Leinecker/Archer (C++6 Bible, MITP, Ch. 8 pp. 215 german) say 
  //   that's not exact but usable for many cases,
  //   better: use tmHeight + tmExternalLeading from GetTextMetrics() 
  CSize size;
  // compute height of line 
  wchar_t lf[] = L"\n";
  GetTextExtentPoint32W(dc.m_hDC, lf, 1, &size);
 
  ASSERT(!mbUseUSP10 || dspplain::HasUSP10());
  // write text without \r\n (TextOutW tries to display them as glyph)
  if (mbUseUSP10) {
    // MSGDEBUG("DrawLine: UseUSP10");
    int y = mY;
    bool bidiLevelRTL = false;
    wchar_t firstRTLCh = 0;
    for (const wchar_t *p = pStart; p < pEnd; ++p) { 
      FriBidiCharType t = fribidi_get_type(*p);
      // For Losung display, use RTL as base direction as soon as there is any one RTL char
      if (t == FRIBIDI_TYPE_RTL || t == FRIBIDI_TYPE_WR || t == FRIBIDI_TYPE_AN) {
        bidiLevelRTL = true;
        firstRTLCh = *p;
        break;
      }
    }
    if (bidiLevelRTL) {
      //MSGDEBUG("DrawLine: detected RTL characters in line (first one is 0x" << std::hex << firstRTLCh << std::dec << "), using RTL for the line.");
    }

    // 2009-09-10 HS: hr == E_INVALIDARG returned if len == 0 (i.e. pEnd==pStart)
    // => skip dspplain for len == 0, and reset USP10 only if hr != E_INVALIDARG
    // 0x80004001 = E_NOTIMPL
    // 0x80070057 = E_INVALIDARG
    // (defined in VC98/Include/WinError.h)
    if (pEnd > pStart) {
      HRESULT hr = dspplain::PaintPlainTextLine(dc.m_hDC, &y, &clipRect, /*iFirst*/0, pEnd - pStart, size.cy, pStart, bidiLevelRTL);
      if (!SUCCEEDED(hr) && hr != E_INVALIDARG) {
        mbUseUSP10 = false;
        MSGERROR("CLineDrawer: PaintPlainTextLine failed, turning off use of MS Uniscribe Processor (USP10), using fribidi + Windows TextOutW instead.");
      }
      mY = y;
    }
    else {
      mY += size.cy;
    }
  }
  else {
    //MSGDEBUG("DrawLine: Log2Vis");
    // i.e. !mbUseUSP10()
    // we have a local copy, cast away constness
    wchar_t *pBuffer = const_cast<wchar_t*>(buffer.c_str());
    const wchar_t *pDestEnd;
    long res = mBidi.LogToVis(pBuffer, pBuffer + buffer.length(), &pDestEnd);
    if (res >= 0) {
      buffer.erase(pDestEnd - pBuffer);
      DUMPBUFFER(buffer);
    }
    //MSGDEBUG("DrawLine: TextOutW");
    TextOutW(dc.m_hDC, mLeft, mY, pStart, pEnd - pStart);
    // adjust y position by height of line 
    mY += size.cy;
  }

  // adapt mMaxLen
  GetTextExtentPoint32W(dc.m_hDC, pStart, pEnd - pStart, &size);
  if (size.cx > mMaxLen) {
    mMaxLen = size.cx;
  }
}

CFont *CLineDrawer::_SelectInto(CDC &dc, const ILineLayout &lineLayout)
{
  // these defaults are overridden by GetProfileXXX below!
  LOGFONT logfont;
  memset(&logfont, 0, sizeof(logfont));

 	_tcscpy(logfont.lfFaceName, lineLayout.GetName().c_str());
  // CWindowDC dc(NULL);
	logfont.lfHeight         = -::MulDiv(lineLayout.GetSize(), dc.GetDeviceCaps(LOGPIXELSY), 720);
	logfont.lfPitchAndFamily = lineLayout.GetPitchAndFamily();
  logfont.lfWeight         = lineLayout.GetWeight();
  logfont.lfItalic         = lineLayout.GetItalic();
  logfont.lfUnderline      = lineLayout.GetUnderline();
  logfont.lfStrikeOut      = lineLayout.GetStrikeOut();

  // 2009-08-15 HS: would it be ok to re-use mpFont? No idea, be defensive...
  delete mpFont;
  mpFont = new CFont;
	mpFont->CreateFontIndirect(&logfont);
  CFont *pOldFont = dc.SelectObject(mpFont);
  dc.SetTextColor(lineLayout.GetCol());
  return pOldFont;
}

void CLineDrawer::DrawLines(CDC &dc, CRect &clipRect, const ILineLayout &lineLayout, const wstring &text)
{
  if (text.empty()) return;

  mLeft = clipRect.CenterPoint().x;        // left - center lines horizontally

  // setup text pointers
  const wchar_t *pStart = text.c_str();
  const wchar_t *pEnd   = 0;
  const wchar_t *pTextEnd = pStart + text.length();

  CFont *pOldFont = _SelectInto(dc, lineLayout);
  if (!pOldFont) return;

  while (_GetNextLine(pStart, pEnd, pTextEnd)) {
    // assert(pStart < pEnd);
    const wchar_t *pPrintableEnd = pEnd;
    if (pPrintableEnd[-1] == L'\n') { 
      --pPrintableEnd; // point to \n 
    }
    if (pPrintableEnd > pStart && pPrintableEnd[-1] == L'\r') {
      --pPrintableEnd; // point to \r 
    }
    // assert(*pPrintableEnd==L'\r' || *pPrintableEnd==L'\n');
    _DrawLine(dc, clipRect, pStart, pPrintableEnd);
    pStart = pEnd;
  }

  dc.SelectObject(pOldFont);
}

