/* ======================================================================
  @brief C++ class for managing application paths
  @author Helmut Steeb
  $Id: PathUtil.h 3904 2009-09-12 10:35:50Z helmut $

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
#ifndef __PATHUTIL_H__
#define __PATHUTIL_H__

#include "../util/tstring.h"

class OPathUtil {
public:
  // stores driveDir and filename component of path in member variables.
  // If path is empty, GetModuleFileName() is used as default,
  // so that DriveDir() will return drive+directory of .exe,
  // Filename() will return name of .exe (without .exe extension).
  OPathUtil(const tstring &path = "");

  // Sets result of DriveDir() and Filename() from path
  void SetPath(const tstring &path);

  // Sets result of DriveDir() from path
  void SetDriveDir(const tstring &path);

  // === member data ===

  tstring DriveDir() const;
  tstring Filename() const;

  // === computation ===

  // PathFromExt:
  // returns ext given + stored dir and filename
  tstring PathFromExt       (const tstring ext) const;

  // PathFromNameExt:
  // returns fname and ext given + stored dir
  tstring PathFromNameExt   (const tstring fname, const tstring ext) const;
  tstring PathFromNameExt   (const tstring fnameExt) const;

  // PathFromDirExt:
  // returns dir and ext given + stored filename
  tstring PathFromDirExt    (const tstring dir, const tstring ext) const;

  // PathFromDirNameExt:
  // returns dir, name and ext given.
  tstring PathFromDirNameExt(const tstring dir, const tstring fname, const tstring ext) const;
  tstring PathFromDirNameExt(const tstring dir, const tstring fnameExt) const;

protected:
  
  tstring _BuildPath(const tstring &drivedir, const tstring &fname, const tstring &ext) const;
  tstring _BuildPath(const tstring &drivedir, const tstring &fnameExt) const;

private:
  // mDriveDir, mFName = components of executable
  tstring mDriveDir;
  tstring mFName;

};


#endif
