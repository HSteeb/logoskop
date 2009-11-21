/* ======================================================================
  @brief UTF-8 conversion
  @author Helmut Steeb
  $Id: utf8.cpp 3853 2009-08-26 13:50:27Z helmut $

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
#include "UTF8.h"
#include "MemBuf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT '?'
static OMemBuf sBuffer;

// --- Private buffer routines ---


void _utf8_toUtf16(const char **fromP, const char *fromLim,
                  unsigned short **toP, const unsigned short *toLim)
{
  unsigned short *to = *toP;
  const char *from = *fromP;
  while (from != fromLim && to != toLim) {
    if (unsigned char(*from) < 0xC0) {
      *to++ = *from++;
    }
    else if (unsigned char(*from) < 0xE0) {
    // case BT_LEAD2:
      *to++ = ((from[0] & 0x1f) << 6) | (from[1] & 0x3f);
      from += 2;
    }
    else if (unsigned char(*from) < 0xF0) {
    // case BT_LEAD3:
      *to++ = ((from[0] & 0xf) << 12) | ((from[1] & 0x3f) << 6) | (from[2] & 0x3f);
      from += 3;
    }
    else if (unsigned char(*from) < 0xF5) {
    // case BT_LEAD4:
      {
        unsigned long n;
        if (to + 1 == toLim)
          break;
        n = ((from[0] & 0x7) << 18) | ((from[1] & 0x3f) << 12) | ((from[2] & 0x3f) << 6) | (from[3] & 0x3f);
        n -= 0x10000;
        to[0] = (unsigned short)((n >> 10) | 0xD800);
        to[1] = (unsigned short)((n & 0x3FF) | 0xDC00);
        to += 2;
        from += 4;
      }
    }
    else {
      *to++ = *from++;
    }
  }
  *fromP = from;
  *toP = to;
}

long _WideCharToUTF8(const wchar_t &wide, char **ppDestStart, const char *pDestEnd)
{
  char *pDest = *ppDestStart;
  long res = 0;
  if (wide < 0x0080) {
    if (pDest + 1 > pDestEnd) {
      pDest += 1;
      res = -1;
    }
    else if (wide) {
      // 0001..007f
      *pDest++ = (unsigned char) wide;
    }
    else {
      // 0000
      *pDest++ = DEFAULT;
      res = 1;
    }
  }
  else if (wide <= 0x07ff) {
    // 0080..07ff
    if (pDest + 2 > pDestEnd) {
      pDest += 2;
      res = -1;
    }
    else {
      *pDest++ = ((unsigned char) (0xc0 | (wide >> 6)));
      *pDest++ = ((unsigned char) (0x80 | (wide  & 0x3f)));
    }
  }
  else {
    // 0800..ffff
    if (pDest + 3 > pDestEnd) {
      pDest += 3;
      res = -1;
    }
    else {
      *pDest++ = (unsigned char) (0xe0 | (wide >> 12));
      *pDest++ = (unsigned char) (0x80 | ((wide >> 6) & 0x3f));
      *pDest++ = ((unsigned char) (0x80 | (wide  & 0x3f)));
    }
  }
  *ppDestStart = pDest;
  return res;
}


long _FromUTF16(const wchar_t *pSrcStart, const wchar_t *pSrcEnd, char **ppDestStart, char *pDestEnd)
{
  long res = 0;
  long nErrors = 0;
  wchar_t wide;
  while (pSrcStart < pSrcEnd) {
    wide = *pSrcStart++;
    res = _WideCharToUTF8(wide, ppDestStart, pDestEnd);
    if (res > 0) nErrors += res; 
    // continue!
  }
  return (res < 0) ? res : nErrors;
}

// =========================================================================

wchar_t *NUTF8::ToUTF16(const char *pSrcStart, const char *pSrcEnd)
{
  // number of wide elements <= number of bytes in utf-8 string
  wchar_t *pWideBufferStart = sBuffer.GetWideBuffer(pSrcEnd - pSrcStart + 1);
  wchar_t *pDestRunning = pWideBufferStart;
  if (pDestRunning) {
    // convert into reallocated static buffer
    ::_utf8_toUtf16(&pSrcStart, pSrcEnd, &pDestRunning, sBuffer.GetWideBufferEnd());
    if (pSrcStart == pSrcEnd) {
      // success, let parameters point to static buffer
      *pDestRunning++ = 0;
      return pWideBufferStart;
    }
  }
  return 0;
}

wchar_t *NUTF8::ToUTF16(const std::string &s)
{
  const char *p = s.c_str();
  return ToUTF16(p, p + s.length());
}


char *NUTF8::FromUTF16(const wchar_t *pSrcStart, const wchar_t *pSrcEnd)
{
  // number of byte elements <= 3 * number of elements in wide string
  char *pByteBufferStart = sBuffer.GetByteBuffer(3 * (pSrcEnd - pSrcStart) + 1);
  char *pDestRunning = pByteBufferStart;
  if (pDestRunning) {
    int res = ::_FromUTF16(pSrcStart, pSrcEnd, &pDestRunning, sBuffer.GetByteBufferEnd());
    if (res >= 0) {
      *pDestRunning++ = 0;
      return pByteBufferStart;
    }
  }
  return 0;
}

const char *NUTF8::FromUTF16nz(const wchar_t *pSrcStart, const wchar_t *pSrcEnd)
{
  const char *p = FromUTF16(pSrcStart, pSrcEnd);
  return p ? p : "";
}

char *NUTF8::FromUTF16(const std::wstring &s)
{
  const wchar_t *p = s.c_str();
  return FromUTF16(p, p + s.length());
}

const char *NUTF8::FromUTF16nz(const std::wstring &s)
{
  const wchar_t *p = s.c_str();
  return FromUTF16nz(p, p + s.length());
}


void NUTF8::Release()
{
  sBuffer.Release();
}
