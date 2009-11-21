/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop CFileManager class.

  Encapsulates decisions
  - whether to show ActualizeDialog
  - which year to show

  $Id: FileManager.h 3905 2009-09-12 11:30:16Z helmut $

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

#if !defined(AFX_FILEMANAGER_H__INCLUDED_)
#define AFX_FILEMANAGER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProfile;

class IFileManager {
public:
  virtual ~IFileManager();

  // GetTwdDir:
  // returns dir passed to constructor
  virtual string GetTwdDir() const = 0;

  // GetTwdFilePath: 
  // computes the path for the given year and docLang.
  virtual string GetTwdFilePath(int year, const string &docLang) const = 0;

  // GetFirstMonthForNextYear: 
  // used to determine when the file for the year to come 
  // should be proposed for the actualize dialog.
  virtual int GetFirstMonthForNextYear() const = 0;

  // MustShowActualizeDialog:
  // returns non-empty string iff. the ActualizeDialog should be shown for given year, month. 
  // The string denotes the new month (which should e.g. be stored in a profile).
  // - bFileExists = whether the GetTwdFilePathToActualize(year, month, ...) exists,
  // - lastAutoLangPackDlgMonth (e.g. "2005-11") = the month where MustShowActualizeDialog()
  //   specified to show ActualizeDialog the last time,
  // - bAutoLangPackDlgOn = whether user disabled automatic display of
  //   ActualizeDialog the last time it was shown (= lastAutoLangPackDlgMonth)
  //   (which does not prevent to show it for a new year)
  virtual string MustShowActualizeDialog(int year, int month, bool bFileExists, 
    const string &lastAutoLangPackDlgMonth, bool bAutoLangPackDlgOn) const = 0;

  // ThisOrNextYearForThatFileExists:
  // returns 
  // - thisYear (if the file for docLang in thisYear exists)
  // - or thisYear+1 (if the file for docLang in thisYear+1 exists)
  // - otherwise 0
  virtual int ThisOrNextYearForThatFileExists(int thisYear, int thisMonth, const string &docLang) = 0;

  virtual bool Install(int year, const string &docLang, const string& data) const = 0;


#ifdef _TEST
  virtual bool SelfTest() const = 0;
#endif
};

namespace NFileManager {
  // resulting IFileManager must be freed by caller
  IFileManager &MakeFileManager(const tstring &driveDir, CProfile &profile);
};

#endif
