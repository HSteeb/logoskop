/* ======================================================================
  @brief C++ class for managing application paths
  @author Helmut Steeb
  $Id: PathUtil.cpp 3904 2009-09-12 10:35:50Z helmut $

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
#include <stdlib.h> // _splitpath
#include "PathUtil.h"

OPathUtil::OPathUtil(const tstring &path)
{
  if (!path.empty()) {
    SetPath(path);
  }
  else {
    TCHAR programName[_MAX_PATH];
    if (GetModuleFileName(NULL, programName, sizeof(programName))) {
      SetPath(tstring(programName));
    }
  }
}

void OPathUtil::SetPath(const tstring &path)
{
  TCHAR drive[_MAX_DRIVE];
  TCHAR dir[_MAX_DIR];
  TCHAR ext[_MAX_EXT];
  TCHAR fname[_MAX_FNAME];
  _tsplitpath(path.c_str(), drive, dir, fname, ext);  
  mDriveDir = tstring(drive) + dir;
  mFName = tstring(fname);
}

void OPathUtil::SetDriveDir(const tstring &path)
{
  TCHAR drive[_MAX_DRIVE];
  TCHAR dir[_MAX_DIR];
  TCHAR _ext[_MAX_EXT];
  TCHAR _fname[_MAX_FNAME];
  _tsplitpath(path.c_str(), drive, dir, _fname, _ext);
  mDriveDir = tstring(drive) + dir;
}


// === member data ===

tstring OPathUtil::DriveDir() const
{
  return mDriveDir;
}

tstring OPathUtil::Filename() const
{
  return mFName;
}


// === computation ===

tstring OPathUtil::_BuildPath(const tstring &drivedir, const tstring &fname, const tstring &ext) const
{
  TCHAR  buf  [_MAX_PATH];
  TCHAR  drive[_MAX_DRIVE];
  TCHAR  dir  [_MAX_DIR];
  TCHAR _fn   [_MAX_FNAME];
  TCHAR _e    [_MAX_EXT];

  _tsplitpath(drivedir.c_str(), drive, dir, _fn,            _e);
  _tmakepath (buf,              drive, dir,  fname.c_str(),  ext.c_str());
  return buf;
}

tstring OPathUtil::_BuildPath(const tstring &drivedir, const tstring &fnameExt) const
{
  TCHAR  buf  [_MAX_PATH];
  TCHAR  drive[_MAX_DRIVE];
  TCHAR _drive[_MAX_DRIVE];
  TCHAR  dir  [_MAX_DIR];
  TCHAR _dir  [_MAX_DIR];
  TCHAR  fn   [_MAX_FNAME];
  TCHAR _fn   [_MAX_FNAME];
  TCHAR  e    [_MAX_EXT];
  TCHAR _e    [_MAX_EXT];

  _tsplitpath(drivedir.c_str(),  drive,  dir,  _fn,  _e);
  _tsplitpath(fnameExt.c_str(), _drive, _dir,   fn,   e);
  _tmakepath (buf,               drive,  dir,   fn,   e);
  return buf;
}

tstring OPathUtil::PathFromExt(const tstring ext) const
{
  return _BuildPath(mDriveDir, mFName, ext);
}

tstring OPathUtil::PathFromNameExt(const tstring fname, const tstring ext) const
{
  return _BuildPath(mDriveDir, fname, ext);
}

tstring OPathUtil::PathFromNameExt(const tstring fnameExt) const
{
  return _BuildPath(mDriveDir, fnameExt);
}

tstring OPathUtil::PathFromDirExt(const tstring dir, const tstring ext) const
{
  return _BuildPath(dir, mFName, ext);
}

tstring OPathUtil::PathFromDirNameExt(const tstring dir, const tstring fname, const tstring ext) const
{
  return _BuildPath(dir, fname, ext);
}

tstring OPathUtil::PathFromDirNameExt(const tstring dir, const tstring fnameExt) const
{
  return _BuildPath(dir, fnameExt);
}
