/* ======================================================================
  @brief C++ class for managing application paths
  @author Helmut Steeb
  $Id: AppPathManager.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include <direct.h>
#include "AppPathManager.h"
#include "../util/PathUtil.h"
#include "../util/FileUtil.h"


#define LIBNAME_SHELL32       "Shell32.dll"
#define LIBNAME_SHFOLDER      "SHFolder.dll"

// Exclude unicode version since PROCNAME_* are using just "*A" APIs
#ifdef _UNICODE
#error Ooooops!
#endif

#define PROCNAME_SHELLGETSFP  "SHGetSpecialFolderPathA"
#define PROCNAME_SHELLGETFP   "SHGetFolderPathA"

typedef WINSHELLAPI BOOL    (APIENTRY *fSHGetSpecialFolderPath) (HWND hwndOwner, LPTSTR lpszPath, int nFolder, BOOL fCreate);
typedef WINSHELLAPI HRESULT (APIENTRY *fSHGetFolderPath)        (HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath);


class OAppPathManager : public IAppPathManager {
public:
  virtual ~OAppPathManager();
  virtual tstring Init(const tstring &appDataSubfolder);
  virtual tstring AppDataDir();
  virtual tstring DefaultIniDir();
  virtual tstring LogPath();
  virtual tstring ProgDir();
  virtual tstring TwdDir();

protected:
  tstring _PrepareAppData(const tstring& appDataSubfolder);

private:
  tstring error;
  tstring mAppDataDir;
  tstring mLogPath;
  tstring mProgDir;
};


OAppPathManager::~OAppPathManager()
{
}

tstring OAppPathManager::Init(const tstring& appDataSubfolder)
{
  OPathUtil pathUtil;
  // mProgDir == <path to .exe>
  mProgDir = pathUtil.DriveDir();
  const tstring filename = pathUtil.Filename();

  const tstring error = _PrepareAppData(appDataSubfolder);
  if (!error.empty()) {
    mAppDataDir = mProgDir; // what to do??
  }

  // mLogPath == <APPDATA>/<appDataSubfolder>/<filename>.log (if writable), or in temp dir
  pathUtil.SetDriveDir(mAppDataDir);
  mLogPath = pathUtil.PathFromNameExt(filename, "log");
  if (!FileUtil::Write(mLogPath, "\n")) {
    // guess a writable temp dir
    char *pTemp = _tempnam("c:\\temp", filename.c_str());
    if (pTemp) {
      mLogPath = pTemp;
      free(pTemp);
    }
  }
  return error;
}

tstring OAppPathManager::AppDataDir()
{
  return mAppDataDir;
}

tstring OAppPathManager::DefaultIniDir()
{
  return mAppDataDir;
}

tstring OAppPathManager::LogPath()
{
  return mLogPath;
}

tstring OAppPathManager::ProgDir()
{
  return mProgDir;
}

tstring OAppPathManager::TwdDir()
{
  return mAppDataDir;
}

tstring OAppPathManager::_PrepareAppData(const tstring& appDataSubfolder)
{
  // *** This is called early in application start, logging is not yet available here!!! ***
  // *** Put info for logging into error!!! ***

  TCHAR buf[MAX_PATH];
  tstring error;

  HINSTANCE hShell32 = LoadLibrary(LIBNAME_SHELL32);
  if (!hShell32) {
    // What to do if even loading Shell32 fails? Do not even try SHFolder in this case.
    error = "Failed to load " LIBNAME_SHELL32 " for computing APPDATA, using exe folder";
  }
  else {
    HRESULT res = NOERROR + 1; // default value != NOERROR

    // in the following, 
    // a) either get ProcAddress and set res code, 
    // b) or     fill error()

    // HS: getting SHGetSpecialFolderPathA fails on my NT 4 with IE5 installed :-(
    fSHGetSpecialFolderPath pGet = (fSHGetSpecialFolderPath) GetProcAddress(hShell32, PROCNAME_SHELLGETSFP);
    if (pGet) {
      res = (pGet(NULL, buf, CSIDL_APPDATA, /*bCreate:*/ true) ? NOERROR : NOERROR + 1); // returns BOOL
      if (res != NOERROR) {
        error = "SHGetSpecialFolderPath failed for CSIDL_APPDATA";
      }
      // if we have SHGetSpecialFolderPath, but res != NOERROR, do not try SHFolder (why should we?)
    }
    else {
      // Shell32::SHGetSpecialFolderPath not gotten, try SHFolder::SHGetFolderPath
      HINSTANCE hSHFolder = LoadLibrary(LIBNAME_SHFOLDER);
      if (!hSHFolder) {
        // What to do if getting SHGetSpecialFolderPath and loading SHFolder fails?
        error = "Failed to load " LIBNAME_SHELL32 " and " LIBNAME_SHFOLDER " for computing APPDATA, using exe folder";
      }
      else {
        // Shell32::SHGetSpecialFolderPath not gotten, SHFolder loaded
        fSHGetFolderPath pGet = (fSHGetFolderPath) GetProcAddress(hSHFolder, PROCNAME_SHELLGETFP);
        if (pGet) {
          // Shell32::SHGetSpecialFolderPath not gotten, SHFolder::SHGetFolderPath gotten
          // HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath
          res = pGet(NULL, CSIDL_APPDATA, 0, 0, buf); // returns HRESULT
          if (res != NOERROR) {
            error = "SHGetFolderPath failed for CSIDL_APPDATA";
          }
        }
        else {
          error = "Failed to locate " PROCNAME_SHELLGETFP " in  " LIBNAME_SHFOLDER " for computing APPDATA, using exe folder";
        }
        FreeLibrary(hSHFolder);
      } 
    }
    FreeLibrary(hShell32);

    if (NOERROR == res) {
      mAppDataDir = buf;
      mAppDataDir += "\\" + appDataSubfolder + "\\"; // use trailing "\\" - avoid filename="bible2", ext=".net"!

      // Create directory
      // _mkdir returns 0 on success / -1 with errno == EEXIST if dir already exists
      int mdres = _mkdir(mAppDataDir.c_str());
      if (mdres != 0 && errno != EEXIST) {
        error = "Failed to create APPDATA sub-folder: " + mAppDataDir;
        mAppDataDir = "";
      }
    }
  }
  return error;
}



// === NAppPathManager ===

IAppPathManager &NAppPathManager::MakeAppPathManager()
{
  IAppPathManager *pAppPathManager = new OAppPathManager();
  assert(pAppPathManager);
  return *pAppPathManager;
}
