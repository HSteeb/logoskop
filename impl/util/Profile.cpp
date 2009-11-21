/* ======================================================================
  @brief C++ class for managing profile
  @author Helmut Steeb
  $Id: Profile.cpp 3905 2009-09-12 11:30:16Z helmut $

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
#include <assert.h>
#include "Profile.h"
#include "../util/UTF8.h"

#define PRF_GENERAL _T("General")

const tstring localDefault = _T("~@~");


CProfile::CProfile()
: mpBuffer(0), mBufferSize(0)
{
}

CProfile::~CProfile()
{
  delete mpBuffer;
}

// copied from LosFileSaver.cpp
#ifdef UNICODE
#define UTF8_OR_CSTR(tstr) (NUTF8::FromUTF16nz(tstr))
#else
#define UTF8_OR_CSTR(tstr) (tstr.c_str())
#endif

void CProfile::SetWritableIniFileName(const tstring &filename)
{
  mWritableIniFileName = filename;
}

void CProfile::SetReadOnlyIniFileName(const tstring &filename)
{
  mReadOnlyIniFileName = filename;
}

tstring CProfile::_GetWritableIniFileName()
{
  return mWritableIniFileName;
}

tstring CProfile::_GetSection(const tstring &section)
{
  return section.empty() ? PRF_GENERAL : section;
}



// === Profile Get/Write ===

DWORD CProfile::_GetStringFromFile(
  const tstring &section,
  const tstring &key, 
  const tstring &defaultValue, 
  const int     maxSize,
  tstring       &value, 
  const tstring &filename)
{
  if (maxSize >= mBufferSize) {
    delete mpBuffer;
    // mBufferSize includes terminating \0
    mBufferSize = maxSize+1;
    mpBuffer = new TCHAR[mBufferSize];
    assert(mpBuffer);
  }
  // HS 2004-12-24:
  // Obviously, under Windows 95 GetPrivateProfileString returns an empty string if defaultValue='\n'.
  // 
  // MSDN says at GetPrivateProfileString:
  //   Windows 95: Although lpDefault is declared as a constant parameter, 
  //   the system strips any trailing blanks by inserting a null character 
  //   into the lpDefault string before copying it to the lpReturnedString buffer. 
  // -> obviously, the routine strips trailing *whitespace*!
  DWORD count = GetPrivateProfileString(_GetSection(section).c_str(), key.c_str(), defaultValue.c_str(), mpBuffer, maxSize, filename.c_str());

  // must assign empty defaultValue from caller!
  value = mpBuffer;
  MSGDEBUG("_GetStringFromFile(" << T28(key) << ", '" << T28(defaultValue) << "', " << T28(filename) << ", " << T28(section) << ")==" << value.substr(0, 10) << " ... (" << count << " chars)");
  return count;
}

unsigned int CProfile::_GetIntFromFile(
  const tstring &section,
  const tstring &key, 
  const unsigned int defaultValue, 
  const tstring &filename)
{
  unsigned int res = GetPrivateProfileInt(_GetSection(section).c_str(), key.c_str(), defaultValue, filename.c_str());
  MSGDEBUG("_GetIntFromFile(" << T28(key) << ", 0x" << std::hex << defaultValue << ", " << T28(filename) << ", " << T28(section) << ")==0x" << res << std::dec);
  return res;
}


// === user functions ===

unsigned long CProfile::GetString(
  const tstring &key, 
  const tstring &defaultValue,
  const int     maxSize,
  tstring       &value,
  const tstring &section)
{
  DWORD rc;
  // Read from local ini file into local buffer using local default value.
  // If a value different from the local default value was read, return it.
  // Else continue with reading from the program directory's ini file.
  rc = _GetStringFromFile(section, key, localDefault, maxSize, value, mWritableIniFileName);
  if (value != localDefault) {
    return rc;
  }
  return _GetStringFromFile(section, key, defaultValue, maxSize, value, mReadOnlyIniFileName);
}

bool CProfile::WriteString(const tstring &key, const tstring &value, const tstring &section)
{
  return value == localDefault
    || WritePrivateProfileString(
         _GetSection(section).c_str(), 
         key.c_str(), 
         value.c_str(),
         _GetWritableIniFileName().c_str()
       ) != 0;
}

bool CProfile::DeleteString(const tstring &key, const tstring &section)
{
  return 0 != WritePrivateProfileString(_GetSection(section).c_str(), key.c_str(), 0, 
    _GetWritableIniFileName().c_str());
}

unsigned int CProfile::GetInt(const tstring &key, const unsigned int defaultValue, const tstring &section)
{
  const unsigned int localDefault = 0xffffffff;
  int buf = _GetIntFromFile(section, key, 0xffffffff, mWritableIniFileName);
  if (buf != localDefault) {
    return buf;
  }
  return _GetIntFromFile(section, key, defaultValue, mReadOnlyIniFileName);
}

bool CProfile::GetBool(const tstring &key, const bool defaultValue, const tstring &section)
{
  const unsigned int localDefault = 0xffffffff;
  unsigned int v = GetInt(key, localDefault, section);
  if (v == localDefault) {
    return defaultValue;
  }
  return v != 0;
}

bool CProfile::WriteInt(const tstring &key, const unsigned int value, const tstring &section)
{
  _TCHAR buf[100];
  _stprintf(buf, _T("%d"), value);
  return WriteString(key, buf, section);
}

bool CProfile::WriteBool(const tstring &key, const bool value, const tstring &section)
{
  return WriteString(key, value ? "1" : "0", section);
}
