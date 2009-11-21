/* ======================================================================
  @brief C++ class for managing profile.
  @author Helmut Steeb
  $Id: Profile.h 3853 2009-08-26 13:50:27Z helmut $

  The class supports
  - progDirIniFile: a required INI file in the program directory.
    This must be given, but may be write protected.
  - systemDirIniFile: an optional INI file e.g. in the system directory.
    When this is given, it will be tried first for reading and writing.

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
#ifndef __PROFILE_H__
#define __PROFILE_H__

#include "../util/tstring.h"
#include "../util/IProfile.h"

class CProfile : public IProfile {
public:

  CProfile();
  virtual ~CProfile();

  void SetWritableIniFileName(const tstring &filename); // write, primary read
  void SetReadOnlyIniFileName(const tstring &filename); // secondary read

  // === Profile Get/Write ===

  // GetString:
  // returns number of characters read.
  // NOTE: under Windows 95, returns an empty string if defaultValue='\n'!
  virtual unsigned long GetString  (const tstring &key, const tstring &defaultValue, const int maxSize, tstring &value, const tstring &section = tstring(_T("")));
  virtual unsigned int  GetInt     (const tstring &key, const unsigned int defaultValue, const tstring &section = tstring(_T("")));
  virtual bool          GetBool    (const tstring &key, const bool defaultValue, const tstring &section = tstring(_T("")));

  virtual bool          WriteString(const tstring &key, const tstring &value, const tstring &section = tstring(_T("")));
  virtual bool          WriteInt   (const tstring &key, const unsigned int value, const tstring &section = tstring(_T("")));
  virtual bool          WriteBool  (const tstring &key, const bool value, const tstring &section = tstring(_T("")));

  virtual bool          DeleteString(const tstring &key, const tstring &section = tstring(_T("")));

protected:

  // === File name handling ===
  tstring _GetWritableIniFileName();
  tstring _GetSection(const tstring &section);

  DWORD _GetStringFromFile(
    const tstring &section,
    const tstring &key, 
    const tstring &defaultValue, 
    const int     maxSize,
    tstring       &value, 
    const tstring &filename);

  unsigned int _GetIntFromFile(
    const tstring &section,
    const tstring &key, 
    const unsigned int defaultValue, 
    const tstring &filename);

private:
  tstring mWritableIniFileName;
  tstring mReadOnlyIniFileName;
  TCHAR *mpBuffer;
  int mBufferSize;
};


#endif
