/* ----------------------------------------------------------------------
  Helmut Steeb
  File utility routines.
  $Id: FileUtil.cpp 3931 2009-09-28 16:32:34Z helmut $

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
#include "FileUtil.h"

bool FileUtil::Exists(const TCHAR *pFilename)
{
  FILE *fp = _tfopen(pFilename, _T("r"));
  if (fp) {
    fclose(fp);
    return true;
  }
  return false;
}

tstring FileUtil::Read(const tstring &filename)
{
  tstring result;
  // read file (binary - avoid messing up utf-8 files)

  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    MSGERROR("Loading file failed: " << filename);
    return _T("");
  }

  MSGINFO("Reading file " << filename);
#define BSIZE 32768
  char buffer[BSIZE];
  int nRead;
  while (nRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) {
    result.append(buffer, nRead);
  }
  fclose(fp);
  return result;
}

bool FileUtil::Write(const tstring &filename, const tstring &content)
{
  // write file (binary - avoid messing up utf-8 files)
  FILE *fp = fopen(filename.c_str(), "wb");
  if (!fp) {
    MSGERROR("Writing file failed: " << filename);
    return false;
  }

  MSGINFO("Writing file " << filename);
  bool bOk = (content.length() == fwrite(content.data(), sizeof(TCHAR), content.length(), fp));
  fclose(fp);
  return bOk;
}

wstring FileUtil::MoronizeW(const wstring &s)
{
  wstring result(s);
  unsigned int pos = result.find(L"\n");
  while (wstring::npos != pos) {
    if (pos == 0 || result[pos-1] != L'\r') {
      result.insert(pos++, 1, L'\r');
    }
    pos = result.find(L"\n", pos+1);
  }
  return result;
}


tstring FileUtil::Moronize(const tstring &s)
{
  tstring result(s);
  unsigned int pos = result.find(_T("\n"));
  while (tstring::npos != pos) {
    if (pos == 0 || result[pos-1] != _T('\r')) {
      result.insert(pos++, 1, _T('\r'));
    }
    pos = result.find(_T("\n"), pos+1);
  }
  return result;
}

tstring FileUtil::DeMoronize(const tstring &s)
{
  tstring result(s);
  unsigned int pos = result.find(_T("\r\n"));
  while (tstring::npos != pos) {
    result.erase(pos, 1);
    pos = result.find(_T("\r\n"), pos+1);
  }
  return result;
}

