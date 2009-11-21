/* ======================================================================
  @brief Routines for Windows clipboard access.
  @author Helmut Steeb
  $Id: Clipboard.cpp 3905 2009-09-12 11:30:16Z helmut $

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
#include "Clipboard.h"

bool NClipboard::Copy(const wstring &text, bool *pbUsedDefaultChar) 
{
  if (pbUsedDefaultChar) {
    *pbUsedDefaultChar = false;
  }

  if (text.empty()) return true;

  bool bRes = true;
  CWaitCursor waitCursor;
  if (!::OpenClipboard(0)) { // hWndNewOwner = 0 => clipboard owned by current task
    MSGERROR("Error opening clipboard - Windows error code " << GetLastError());
    bRes = false;
  }
  else {
    ::EmptyClipboard();

    // transfer to clipboard in CF_UNICODETEXT
    const unsigned long len = text.length();
    HGLOBAL gmem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (1 + len) * sizeof(wchar_t)); // ***
    if (!gmem) {
      MSGERROR("Error getting global memory for unicode clipboard data");
      bRes = false;
    }
    else {
      wchar_t *p = (wchar_t*) GlobalLock(gmem);
      if (!p) {
        MSGERROR("Error locking unicode clipboard data");
        bRes = false;
      }
      else {
        wcscpy(p, text.c_str());
        ::GlobalUnlock(gmem);
        if (!::SetClipboardData(CF_UNICODETEXT, gmem)) {
          MSGERROR("Error setting unicode clipboard data");
          bRes = false;
        }
      }
      ::GlobalFree(gmem);
    }

    gmem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (1 + len) * sizeof(char)); // ***
    if (!gmem) {
      MSGERROR("Error getting global memory for character clipboard data");
      bRes = false;
    }
    else {
      char *p = (char*) GlobalLock(gmem);
      if (!p) {
        MSGERROR("Error locking character clipboard data");
        bRes = false;
      }
      else {
        unsigned int resLen;
        BOOL usedDefaultChar;
        resLen = ::WideCharToMultiByte(CP_ACP, /*dwFlags*/ 0, text.c_str(), 
          // using len results in trailing text!???
          // "-1" lets WideCharToMultiByte handle the text as 0 terminated
          // and compute the length itself.
          -1/*len*/, 
          p, (1 + len) * sizeof(char), NULL, &usedDefaultChar);
        if (usedDefaultChar && pbUsedDefaultChar) {
          *pbUsedDefaultChar = true;
        }
        if (!resLen) {
          MSGERROR("Error converting unicode data");
          bRes = false;
        }
        ::GlobalUnlock(gmem);
        if (!SetClipboardData(CF_TEXT, gmem)) {
          MSGERROR("Error setting character clipboard data");
          bRes = false;
        }
      }
      ::GlobalFree(gmem);
    }

    ::CloseClipboard();
  }
  return bRes;
}
