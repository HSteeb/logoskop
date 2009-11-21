/* ======================================================================
  @brief UTF-8 conversion
  @author Helmut Steeb
  $Id: utf8.h 3011 2008-11-29 09:37:12Z helmut $

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
#ifndef _UTF8_H_
#define _UTF8_H_


namespace NUTF8 
{
  // returning pointer to static buffer

  wchar_t *ToUTF16(const char *pSrcStart, const char *pSrcEnd);
  wchar_t *ToUTF16(const std::string &s);

  // FromUTF16: returns 0 on error
  char *FromUTF16(const wchar_t *pSrcStart, const wchar_t *pSrcEnd);
  char *FromUTF16(const std::wstring &s);

  // FromUTF16nz: returns "" on error
  const char *FromUTF16nz(const wchar_t *pSrcStart, const wchar_t *pSrcEnd);
  const char *FromUTF16nz(const std::wstring &s);

  // Release():
  // releases static buffer (may be allocated again)
  void Release();

}

// T28:
// tstring to 8-bit string (ACP for 8 bit application, UTF-8 for 16 bit application)
#ifdef UNICODE
#define T28(tstr) NUTF8::FromUTF16nz(tstr)
#else
#define T28(tstr) (const_cast<char*>(tstr.c_str()))
#endif

#endif
