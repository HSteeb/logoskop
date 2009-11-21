/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop CFileManager class.
  $Id: FileManager.cpp 3934 2009-09-30 17:02:31Z helmut $

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
#include "FileManager.h"
#include "../util/Profile.h"
#include "../util/FileUtil.h"
#include "../util/PathUtil.h"
#include "../util/ProfileDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FIRST_MONTH_FOR_NEXT_YEAR (9)

// --- for internal regression tests only ---
// P-re P-revious month
#define _sPP "07"
// P-revious month
#define _P (FIRST_MONTH_FOR_NEXT_YEAR-1)
#define _sP "08"
// F-irst month
#define _F (FIRST_MONTH_FOR_NEXT_YEAR)
#define _sF "09"

// === IFileManager ===

IFileManager::~IFileManager()
{
}

// === CFileManager ===

class CFileManager : public IFileManager 
{
public:
  CFileManager(const tstring &driveDir, CProfile &profile);
  virtual ~CFileManager();

  virtual string GetTwdDir() const;
  virtual string GetTwdFilePath(int year, const string &docLang) const;
  virtual int GetFirstMonthForNextYear() const;

  virtual string MustShowActualizeDialog(int year, int month, bool bFileExists, 
    const string &lastAutoLangPackDlgMonth, bool bAutoLangPackDlgOn) const;

  virtual int ThisOrNextYearForThatFileExists(int thisYear, int thisMonth, const string &docLang);

  virtual bool Install(int year, const string &docLang, const string& data) const;


#ifdef _TEST
  virtual bool SelfTest() const;
#endif

private:
#ifdef _TEST
  bool _Test1(
    int year, int month, bool bFileExists, 
    const string &lastAutoLangPackDlgMonth, bool bAutoLangPackDlgOn, 
    bool bMustShowExpected, const string &resultAutoLangPackDlgMonthExpected) const;
#endif

  OPathUtil mPathUtil;
  CProfile &mProfile;
};



CFileManager::CFileManager(const tstring &driveDir, CProfile &profile)
: mProfile(profile)
{
  mPathUtil.SetDriveDir(driveDir);
}


CFileManager::~CFileManager()
{
}

string CFileManager::GetTwdDir() const
{
  return mPathUtil.DriveDir();
}

string CFileManager::GetTwdFilePath(int year, const string &docLang) const
{
  // standard filename e.g. "de_los00.xml", "zh-TW_los99.xml"
  std::ostringstream os;
  // year    = 2008
  // docLang = "de_Schlachter2000"
  // standard filename e.g. "de_Schlachter2000_2008.twd"
  // Grep for [LANGUAGE]
  os << docLang << "_" << std::dec << year;
  return mPathUtil.PathFromNameExt(os.str().c_str(), _T("twd"));
}

int CFileManager::GetFirstMonthForNextYear() const
{
  return FIRST_MONTH_FOR_NEXT_YEAR;
}


/* -------------------------------------------------------------------------------------------
HS 2005-11-01:

w32los 4.0.7 (= Losung 2005) behaviour:

  ...................................................................
  date     this/next ex. Actualize  year to read  show next year hint
  ...................................................................
        01  this not     this 1st/y NOP
            this ex.     no         this
  ...................................................................
    02..09  this not     no         NOP
            this ex.     no         this
  ...................................................................
  _F..12.0  none         no         NOP
            this         no         this
            both         no         this
            next         no         next           1st/y
  ...................................................................
      12.5  none         no         NOP
            this         next 1st/y this
            both         no         this
            next         no         next           1st/y
  ...................................................................

w32los > 4.0.7 (= Losung 2006) behaviour:

  .............................................................................................
      date    this/next ex. Actualize what, if                year to read
  .............................................................................................
  (A)  01..09  this not     this, 1st/m && ALO                none
  (B)          this ex.     no                                this
  .............................................................................................
  (C)  10..12  none         next, 1st/m && (ALO || 1st/next)  none
  (D)          this         next, 1st/m && (ALO || 1st/next)  this
  (E)          both         no                                this
  (F)          next         no                                next
  .............................................................................................

Legend: 
  1st/m = if PRF_AUTOLANGPACKDLG indicates it is the first call in the current month;
  ALO = option [General]AutoLangPackDlgOn is true;
  1st/next = PRF_AUTOLANGPACKDLG indicates it is the first call for the next year text 
    (i.e. month in existing PRF_AUTOLANGPACKDLG is < 10)
  this/next ex. = text of this or next year exists on the computer.
------------------------------------------------------------------------------------------- */

string CFileManager::MustShowActualizeDialog(int year, int month, bool bFileExists, 
    const string &lastAutoLangPackDlgMonth, bool bAutoLangPackDlgOn) const
{
  const string empty("");
  if (bFileExists) {
    // (B), (E), (F)
    return empty;
  }
  // (A), (C), (D)

  _TCHAR sMonth[100];
  _stprintf(sMonth, _T("%04d-%02d"), year, month);

  // show actualize dialog only once per month
  if (lastAutoLangPackDlgMonth >= sMonth) {
    // actualize dialog has already been shown, do not show again
    // 1st/m is not true
    return empty;
  }

  if (!bAutoLangPackDlgOn) {
    // ! ALO
    if (month < FIRST_MONTH_FOR_NEXT_YEAR) {
      // (A)
      return empty;
    }
    _TCHAR sFirstMonthNextYear[100];
    _stprintf(sFirstMonthNextYear, _T("%04d-%02d"), year, FIRST_MONTH_FOR_NEXT_YEAR);
    if (lastAutoLangPackDlgMonth >= sFirstMonthNextYear) {
      // (C), (D)
      // 1st/next is false
      return empty;
    }
  }
  
  return sMonth;
}

int CFileManager::ThisOrNextYearForThatFileExists(int thisYear, int thisMonth, const string &docLang)
{
  int yearToRead = 0;
  string &filename = GetTwdFilePath(thisYear, docLang);

  MSGDEBUG("CFileManager::ThisOrNextYearForThatFileExists(thisYear==" << std::dec << thisYear << ", thisMonth==" << thisMonth
    << ", docLang==" << docLang << ")");
  if (FileUtil::Exists(filename.c_str())) {
    // (B), (D), (E)
    MSGDEBUG("CFileManager::ThisOrNextYearForThatFileExists: file exists: "  << filename);
    yearToRead = thisYear;
  }
  else if (thisMonth < FIRST_MONTH_FOR_NEXT_YEAR) {
    // (A)
    MSGDEBUG("CFileManager::ThisOrNextYearForThatFileExists: file " << filename << " does not exist, month=" << std::dec << thisMonth << ", no file to load.");
  }
  else {
    // (C), (F)
    MSGDEBUG("CFileManager::ThisOrNextYearForThatFileExists: file " << filename << " does not exist, but month=" << std::dec << thisMonth
      << ", checking for file of next year...");
    // special handling: 
    // if the file for the current doc lang is missing in month >= FIRST_MONTH_FOR_NEXT_YEAR,
    // and the file for the year to come is present,
    // assume they installed the packet for the year to come
    // without the text of the current year.
    // Switch to next year just now (let them change date as usual).
    string nextYearName(GetTwdFilePath(thisYear+1, docLang));
    if (!FileUtil::Exists(nextYearName.c_str())) {
      // (C)
      MSGINFO("CFileManager::ThisOrNextYearForThatFileExists: this year file " << filename << " and next year file " << nextYearName << " do not exist, no file to load!");
    }
    else {
      // (F)
      MSGDEBUG("CFileManager::ThisOrNextYearForThatFileExists: this year file " << filename << " does not exist, but next year file " << nextYearName << " exists, using this to load.");
      yearToRead = thisYear+1;
    }
  }
  MSGDEBUG("CFileManager::ThisOrNextYearForThatFileExists: returning " << std::dec << yearToRead);
  return yearToRead;
}


bool CFileManager::Install(int year, const string &docLang, const string& data) const
{
  string path = GetTwdFilePath(year, docLang);
  return FileUtil::Write(path, data);
}


#ifdef _TEST

// === TEST =========================================================================================

bool CFileManager::_Test1(
  int year, int month, bool bFileExists, 
  const string &lastAutoLangPackDlgMonth, bool bAutoLangPackDlgOn, 
  bool bMustShowExpected, const string &resultAutoLangPackDlgMonthExpected) const
{
  string resultAutoLangPackDlgMonth =
    MustShowActualizeDialog(year, month, bFileExists, lastAutoLangPackDlgMonth, bAutoLangPackDlgOn);
  bool bMustShow = !resultAutoLangPackDlgMonth.empty();

  MSGDEBUG("CFileManager::_Test1(y=" << year << ", m=" << month << ", exists=" << bFileExists
    << ", last='" << lastAutoLangPackDlgMonth << "', autoOn=" << bAutoLangPackDlgOn 
    << ", bMustShow/Expected=" << bMustShow << "/" << bMustShowExpected 
    << ", Month/Expected='" << resultAutoLangPackDlgMonth << "'/'" << resultAutoLangPackDlgMonthExpected 
    << "')");

  bool bTestOk = true;
  if (bMustShowExpected != bMustShow) {
    MSGERROR("  bMustShowExpected != bMustShow");
    bTestOk = false;
  }
  if (resultAutoLangPackDlgMonthExpected != resultAutoLangPackDlgMonth) {
    MSGERROR("  MonthExpected != Month");
    bTestOk = false;
  }
  return bTestOk;
}

bool CFileManager::SelfTest() const
{
  bool bRes = true;
  MSGDEBUG("--- Start CFileManager::SelfTest ----------------------------");

  // Test1(year, month, bFileExists, lastAutoLangPackDlgMonth, bAutoLangPackDlgOn, bMustShowExpected, resultAutoLangPackDlgMonthExpected) 

  // (A)  01..08  this not     this, 1st/m && ALO
  // month=1
  bRes &= _Test1(2005,  1, false, "2004-12", false,  false, "");        // 1st/m && !ALO
  bRes &= _Test1(2005,  1, false, "2004-12", true,   true,  "2005-01"); // 1st/m && ALO
  bRes &= _Test1(2005,  1, false, "2005-01", false,  false, "");        // !1st/m && !ALO
  bRes &= _Test1(2005,  1, false, "2005-01", true,   false, "");        // !1st/m && ALO
  // month=4
  bRes &= _Test1(2005,  4, false, "2005-03", false,  false, "");        // 1st/m && !ALO
  // month=9
  bRes &= _Test1(2005, _P, false, "2005-"_sPP, false,  false, "");        // 1st/m && !ALO
  bRes &= _Test1(2005, _P, false, "2005-"_sPP, true,   true,  "2005-"_sP); // 1st/m && ALO
  bRes &= _Test1(2005, _P, false, "2005-"_sP, false,  false, "");        // !1st/m && !ALO
  bRes &= _Test1(2005, _P, false, "2005-"_sP, true,   false, "");        // !1st/m && ALO

  // (B)          this ex.     no                
  // month=1
  bRes &= _Test1(2005,  1, true,  "2004-12", false,  false, "");
  bRes &= _Test1(2005,  1, true,  "2004-12", true,   false, "");
  bRes &= _Test1(2005,  1, true,  "2005-01", false,  false, "");
  bRes &= _Test1(2005,  1, true,  "2005-01", true,   false, "");
  // month=4
  bRes &= _Test1(2005,  4, true,  "2005-03", false,  false, "");
  // month=9
  bRes &= _Test1(2005, _P, true,  "2005-"_sPP, false,  false, "");
  bRes &= _Test1(2005, _P, true,  "2005-"_sPP, true,   false, "");
  bRes &= _Test1(2005, _P, true,  "2005-"_sP, false,  false, "");
  bRes &= _Test1(2005, _P, true,  "2005-"_sP, true,   false, "");

  // (C)  10..12  none         next, 1st/m && (ALO || 1st/next)
  // (D)          this         next, 1st/m && (ALO || 1st/next)
  // month=10
  bRes &= _Test1(2005, _F, false,  "2005-"_sP, false, true, "2005-"_sF); // 1st/m && !ALO && 1st/next
  bRes &= _Test1(2005, _F, false,  "2005-"_sP, true,  true, "2005-"_sF); // 1st/m && ALO && 1st/next
  bRes &= _Test1(2005, _F, false,  "2005-"_sF, false, false, "");       // !1st/m && !ALO && !1st/next
  bRes &= _Test1(2005, _F, false,  "2005-"_sF, true,  false, "");       // !1st/m && ALO && 1st/next
  // month=11
  bRes &= _Test1(2005, 11, false,  "2005-"_sP, false, true, "2005-11"); // 1st/m && !ALO && 1st/next
  bRes &= _Test1(2005, 11, false,  "2005-"_sF, false, false,"");        // 1st/m && !ALO && !1st/next
  bRes &= _Test1(2005, 11, false,  "2005-"_sP, true,  true, "2005-11"); // 1st/m && ALO && 1st/next
  bRes &= _Test1(2005, 11, false,  "2005-"_sF, true,  true, "2005-11"); // 1st/m && ALO && !1st/next
  bRes &= _Test1(2005, 11, false,  "2005-11", false, false, "");       // !1st/m && !ALO && !1st/next
  bRes &= _Test1(2005, 11, false,  "2005-11", true,  false, "");       // !1st/m && ALO && !1st/next
  // month=12
  bRes &= _Test1(2005, 12, false,  "2005-"_sP, false, true, "2005-12"); // 1st/m && !ALO && 1st/next
  bRes &= _Test1(2005, 12, false,  "2005-"_sF, false, false, "");       // 1st/m && !ALO && !1st/next
  bRes &= _Test1(2005, 12, false,  "2005-11", false, false, "");       // 1st/m && !ALO && !1st/next
  bRes &= _Test1(2005, 12, false,  "2005-"_sP, true,  true, "2005-12"); // 1st/m && ALO && 1st/next
  bRes &= _Test1(2005, 12, false,  "2005-"_sF, true,  true, "2005-12"); // 1st/m && ALO && !1st/next
  bRes &= _Test1(2005, 12, false,  "2005-11", true,  true, "2005-12"); // 1st/m && ALO && !1st/next
  bRes &= _Test1(2005, 12, false,  "2005-12", false, false, "");       // !1st/m && !ALO && !1st/next
  bRes &= _Test1(2005, 12, false,  "2005-12", true,  false, "");       // !1st/m && ALO && !1st/next

  // (E)          both         no                              
  // (F)          next         no                              
  // month=10
  bRes &= _Test1(2005, _F, true,   "2005-"_sP, false, false, "");       // 1st/m && !ALO
  bRes &= _Test1(2005, _F, true,   "2005-"_sP, true,  false, "");       // 1st/m && ALO
  bRes &= _Test1(2005, _F, true,   "2005-"_sF, false,  false, "");       // !1st/m && !ALO
  bRes &= _Test1(2005, _F, true,   "2005-"_sF, true,  false, "");       // !1st/m && ALO
  // month=11
  bRes &= _Test1(2005, 11, true,   "2005-"_sP, false, false, "");       // 1st/m && !ALO
  bRes &= _Test1(2005, 11, true,   "2005-"_sF, false, false, "");       // 1st/m && !ALO
  bRes &= _Test1(2005, 11, true,   "2005-"_sP, true,  false, "");       // 1st/m && ALO
  bRes &= _Test1(2005, 11, true,   "2005-"_sF, true,  false, "");       // 1st/m && ALO
  bRes &= _Test1(2005, 11, true,   "2005-11", false,  false, "");      // !1st/m && !ALO
  bRes &= _Test1(2005, 11, true,   "2005-11", true,  false, "");       // !1st/m && ALO
  // month=12
  bRes &= _Test1(2005, 12, true,   "2005-"_sP, false, false, "");       // 1st/m && !ALO
  bRes &= _Test1(2005, 12, true,   "2005-"_sF, false, false, "");       // 1st/m && !ALO
  bRes &= _Test1(2005, 12, true,   "2005-11", false, false, "");       // 1st/m && !ALO
  bRes &= _Test1(2005, 12, true,   "2005-"_sP, true,  false, "");       // 1st/m && ALO
  bRes &= _Test1(2005, 12, true,   "2005-"_sF, true,  false, "");       // 1st/m && ALO
  bRes &= _Test1(2005, 12, true,   "2005-11", true,  false, "");       // 1st/m && ALO
  bRes &= _Test1(2005, 12, true,   "2005-12", false,  false, "");      // !1st/m && !ALO
  bRes &= _Test1(2005, 12, true,   "2005-12", true,  false, "");       // !1st/m && ALO

  MSGDEBUG("--- End CFileManager::SelfTest ----------------------------");
  return bRes;
}
#endif


// === NFileManager ===

IFileManager &NFileManager::MakeFileManager(const tstring &driveDir, CProfile &profile)
{
  IFileManager *pFileManager = new CFileManager(driveDir, profile);
  assert(pFileManager);
  return *pFileManager;
}
