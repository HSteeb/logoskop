/* ======================================================================
 @file BidiClient.h
 @brief C++ base class for bidi functionality, calling encoding.dll.
 @author Helmut Steeb
 @version $Id: BidiClient.cpp 3014 2008-11-29 10:19:16Z helmut $

NOTE: Using this file also needs MsgBuf.cpp (for linking MsgBuf::Write)
NOTE: the project's stdafx.h should contain (if not, MSGERROR etc. will be empty):
#include "../messages/msgBuf.h"
#include "../messages/messages.h"
#define BIT ... (e.g. 1 - for identifying messages emitted here)
#define MSGERROR(msg) MSGBUFWRITE(msg, MSGLEVEL_ERROR, BIT)
#define MSGINFO(msg)  MSGBUFWRITE(msg, MSGLEVEL_INFO,  BIT)
#define MSGDEBUG(msg) MSGBUFWRITE(msg, MSGLEVEL_DEBUG, BIT)

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
#include "BidiClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef MSGERROR
#define MSGERROR(s) 
#define MSGINFO(s)
#define MSGDEBUG(s)
#endif

typedef long (*fSetMessageCallback)       (TMessageCallback messageCallback);
typedef long (*fReorder)                  (const GLYPHSET_HANDLE handle, wchar_t *pSrcStart, wchar_t *pSrcEnd);
typedef long (*fShape)                    (const GLYPHSET_HANDLE handle, wchar_t *pSrcStart, wchar_t *pSrcEnd);
typedef long (*fJoin)                     (const GLYPHSET_HANDLE handle, wchar_t *pSrcStart, wchar_t *pSrcEnd, const wchar_t **ppDestEnd);
typedef long (*fLogToVis)                 (const GLYPHSET_HANDLE handle, wchar_t *pSrcStart, wchar_t *pSrcEnd, const wchar_t **ppDestEnd);
typedef GLYPHSET_HANDLE (*fPrepareCanonicalGlyphSet) ();
typedef GLYPHSET_HANDLE (*fPrepareUnrestrictedGlyphSet)();
typedef GLYPHSET_HANDLE (*fPrepareGlyphSetFor)       (const unsigned char *pUnicodeBitSetStart, const unsigned char *pUnicodeBitSetEnd);
typedef long (*fReleaseGlyphSet)           (const GLYPHSET_HANDLE handle);


static HINSTANCE hEncoding = 0;
static bool mustInit = true;

static fSetMessageCallback        pSetMessageCallback   = 0;
static fReorder                   pReorder   = 0;
static fShape                     pShape   = 0;
static fJoin                      pJoin   = 0;
static fLogToVis                  pLogToVis   = 0;
static fPrepareCanonicalGlyphSet    pPrepareCanonicalGlyphSet   = 0;
static fPrepareUnrestrictedGlyphSet pPrepareUnrestrictedGlyphSet   = 0;
static fPrepareGlyphSetFor          pPrepareGlyphSetFor = 0;
static fReleaseGlyphSet             pReleaseGlyphSet   = 0;

static FARPROC GetProc(HINSTANCE handle, char *pProcName, long &res)
{
  FARPROC proc = GetProcAddress(handle, pProcName);
  if (!proc) {
    DWORD err = GetLastError();
    MSGERROR("BidiClient: could not get procedure " << pProcName << " - error " << err);
    res = -5;
  }
  else {
    res = 0;
  }
  return proc;
}

static long Init() 
{
  if (hEncoding) return 0;

  hEncoding = LoadLibrary(_T("encoding.dll"));
  if (!hEncoding) {
    return -4;
  }

  long res = 0;
#define GET(here, enc) if (0 == (p##here = (f##here) GetProc(hEncoding, enc, res))) return res
#ifdef _USE_MSG_BUF
  GET(SetMessageCallback,       "encodingSetMessageCallback");
#endif
  GET(Reorder,                    "Reorder");
  GET(Shape,                      "Shape");
  GET(Join,                       "Join");
  GET(LogToVis,                   "LogToVis");
  GET(PrepareCanonicalGlyphSet,   "PrepareCanonicalGlyphSet");
  GET(PrepareUnrestrictedGlyphSet,"PrepareUnrestrictedGlyphSet");
  GET(PrepareGlyphSetFor,         "PrepareGlyphSetFor");
  GET(ReleaseGlyphSet,            "ReleaseGlyphSet");
  return res;
}


OBidiClient::OBidiClient(bool bUnrestricted) : mGlyphSetHandle(0)
{
  long res = Init();
  if (0 == res) {
    mGlyphSetHandle = bUnrestricted ? pPrepareUnrestrictedGlyphSet() : pPrepareCanonicalGlyphSet();
  }
}

OBidiClient::OBidiClient(unsigned char *pUnicodeBitSetStart, const unsigned char *pUnicodeBitSetEnd)
: mGlyphSetHandle(0)
{
  long res = Init();
  if (0 == res) {
    mGlyphSetHandle = pPrepareGlyphSetFor(pUnicodeBitSetStart, pUnicodeBitSetEnd);
  }
}

OBidiClient::~OBidiClient()
{
  if (pReleaseGlyphSet && mGlyphSetHandle) {
    pReleaseGlyphSet(mGlyphSetHandle);
  }
}

long OBidiClient::SetMessageCallback(TMessageCallback messageCallback)
{
  long res;
  if (0 != (res = Init())) return res;
  pSetMessageCallback(messageCallback);
  return res;
}

long OBidiClient::Reorder(wstring &str)
{
  long res;
  if (0 != (res = Init())) return res;
  res = pReorder(mGlyphSetHandle, str.begin(), str.end());
  return res;
}

long OBidiClient::Shape(wstring &str)
{
  long res;
  if (0 != (res = Init())) return res;
  res = pShape(mGlyphSetHandle, str.begin(), str.end());
  return res;
}

long OBidiClient::Join(wstring &str)
{
  long res;
  if (0 != (res = Init())) return res;
  const wchar_t *pDestEnd;
  res = pJoin(mGlyphSetHandle, str.begin(), str.end(), &pDestEnd);
  if (res >= 0) {
    str = str.substr(0, pDestEnd - str.begin());
  }
  return res;
}

long OBidiClient::LogToVis(wstring &str)
{
  long res;
  if (0 != (res = Init())) return res;
  const wchar_t *pDestEnd;
  res = pLogToVis(mGlyphSetHandle, str.begin(), str.end(), &pDestEnd);
  if (res >= 0) {
#pragma message(__TODO__"OBidiClient::Join AND LogToVis: shorten wstring!!!")
    str = str.substr(0, pDestEnd - str.begin());
  }
  return res;
}
