/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop document.
  $Id: w32losDoc.cpp 3934 2009-09-30 17:02:31Z helmut $

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

#include "w32losDoc.h"
#include "LosDoc.h"
#include "LangNames.h"
#include "../filedesc/TwdFileDesc.h"
#include "../filedesc/DirectoryFileDescReader.h"
#include "../util/ProfileDef.h"
#include "../util/IProfile.h"
#include "../util/FileManager.h"
#include "../LosFile/LosFileStack.h"
#include "../util/DateUtil.h"
#include "../util/StringUtil.h"
#include "../util/xmlfile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === Util ===

// _OSLangIDMatchesLang:
//   returns true if at least 
//   - primary language (e.g. LANG_GERMAN or LANG_CHINESE) 
//     of osLangID (as from GetUserDefaultLangID(), e.g. MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN))
//   matches 
//   - language part (e.g. "zh") of lang (e.g. "de" or "zh-Hans")
// See _OSLangIDTest().

static bool _OSLangIDMatchesLang(LANGID osLangID, const string &lang);

static bool _OSLangIDMatchesLang(LANGID osLangID, const string &lang)
{
  // Lookup iso language code for osLangID
  const char *pIso = NLangNames::GetIsoForLangID(osLangID); // "de", "zh-Hans"
  // pIso maybe 0!
  if (!pIso) return false;

  string iso(pIso);

  // iso language code of osLangID matches as is
  if (iso == lang) {
    return true;
  }

  // compare primary primary language (language part) only
  string::size_type isoPos = iso.find("-"); 
  string isoPrimary = (isoPos == string::npos) ? iso : iso.substr(0, isoPos); // "de", "zh"

  string::size_type langPos = lang.find("-"); 
  string langPrimary = (langPos == string::npos) ? lang : lang.substr(0, langPos); // "de", "zh"

  return isoPrimary == langPrimary;
}




// === CW32losDoc ===

IMPLEMENT_DYNCREATE(CW32losDoc, CDocument)

static const string lf = "\r\n";
static const wstring wlf = L"\r\n";
static const NONMATCHING_LANGID = 0xFFFF;
static const IGNORE_LANGID = 0;


CW32losDoc::CW32losDoc()
: mpFileManager(0)
, mpProfile(0)
, mbLocked(false)
, mTestUserDefaultLangID(IGNORE_LANGID)
{
  mDate = CTime::GetCurrentTime();
  MSGDEBUG("CW32losDoc construction done.");
  mpDirectoryFileDescReader = new ODirectoryFileDescReader();
#ifdef _TEST
  mbTestRunning = false;
  mTestHint = 0;
#endif
}

CW32losDoc::~CW32losDoc()
{
  MSGDEBUG("CW32losDoc destruction...");
  std::vector<OLosDoc*>::iterator it;
  for (it = mLosDocCache.begin(); it != mLosDocCache.end(); ++it) {
    delete *it;
  }
  delete mpDirectoryFileDescReader;
  MSGDEBUG("CW32losDoc destruction done.");
}

// === Files ===

void CW32losDoc::SetFileManager(IFileManager &fileManager)
{
  mpFileManager = &fileManager;
}

IFileManager &CW32losDoc::_FileManager()
{
  ASSERT(mpFileManager);
  return *mpFileManager;
}

string CW32losDoc::_GetFilePath(const string &langBible)
{
  return _FileManager().GetTwdFilePath(mDate.GetYear(), langBible);
}

bool CW32losDoc::_Load(const string &langBible, OLosDoc &losDoc)
{
  MSGDEBUG("CW32losDoc::_Load " << langBible << " for date " << (LPCTSTR) mDate.Format("%Y-%m-%d"));
  OLosFile &losFile = losDoc.LosFile();

  string filename = _GetFilePath(langBible);
  OLosFileStack losFileStack(losFile);

  // store only one day
  losFileStack.SetDayToRead(mDate.GetMonth(), mDate.GetDay());

  bool bStrict = _Profile().GetBool(PRF_PARSE_STRICT, DEFAULT_PARSE_STRICT);
  MSGDEBUG("CW32losDoc::_Load - using strict parsing: " << bStrict);
  losFileStack.SetStrict(bStrict);

  NLosFileStack::RegisterLosFileHandlerCreators(losFileStack);
  XML_Parser parser;
  parser = XML_ParserCreate(NULL);
  NXMLStack::RegisterWithExpatParser(parser, losFileStack);
  NXMLStack::RegisterUnknownEncodingHandler(parser, NLosFileStack::UnknownEncodingHandler);

  unsigned int processFlags = XML_MAP_FILE;    
  int result = XML_ProcessFile(parser, filename.c_str(), processFlags);
  XML_ParserFree(parser);

  if (!result || losFileStack.GetErrNo() > 0) {
    if (GetLastError() == ERROR_FILE_NOT_FOUND) {
      // 2009-09-30 HS: may come here legally - emit info only.
      MSGINFO(_T("File does not exist: " << filename));
      return 0;
    }
    std::tostringstream stream;
    stream << "Loading file " << filename << ": ";
    if (result == 0) {
      int code = XML_GetStoredErrCode();
      const XML_Char *message = XML_ErrorString(code);
      stream << _T(" line ") << XML_GetStoredErrLine() << _T(", column ")
        << XML_GetStoredErrCol() << _T(" ") 
        << _T(" error ") << code << _T(" from xml parser: ")
        << (message ? message : _T("(unknown)"));
    }
    else {
      if (losFileStack.FirstUnknownElement().empty()) {
        stream << losFileStack.GetErrNo() << " semantic errors";
      }
      else {
        // assume losFileStack.GetTolerateUnknownElement() == true
        // - otherwise, unhandled tags would cause losFileStack().GetErrNo()>0 (checked above!)
        stream << "unknown file format containing XML tag <" << losFileStack.FirstUnknownElement() << ">";
      }
    }
    MSGERROR(stream.str());
    return 0;
  }
  else if (mDate.GetYear() != losFile.Year()) {
    MSGERROR(_T("_Load: filename indicates year ") << mDate.GetYear() << _T(", but from file contents got ") << losFile.Year() << _T(" - skipping ") << filename);
    return 0;
  }
  MSGINFO("Loading file " << filename << " done.");
  return 1;
}


// === Profile ===

void CW32losDoc::SetProfile(IProfile &profile)
{
  mpProfile = &profile;
}

IProfile &CW32losDoc::_Profile()
{
  ASSERT(mpProfile);
  return *mpProfile;
}


// === Date ===

CTime CW32losDoc::GetDate() const
{
  return mDate;
}


// === LangBibles ===

const std::vector<string> &CW32losDoc::GetLangBibles() const
{
  return mLangBibles;
}

tstring CW32losDoc::GetFirstLangBible() const
{
  return mLangBibles.empty() ? "" : *mLangBibles.begin();
}

void CW32losDoc::_ProposeLangBibles(std::vector<string> &proposedLangBibles)
{
  string twdDir = _FileManager().GetTwdDir();
  MSGDEBUG(_T("CW32losDoc::_ProposeLangBibles for .twd folder ") << twdDir);
  std::vector<string> filelist; // list of e.g. "de_Schlachter2000"
  proposedLangBibles.clear(); // be defensive

  // fill filelist from files for currently selected year in TwdDir()
  int year = mDate.GetYear();
  std::ostringstream os;
  os << "_" << year << ".twd";
  assert(mpDirectoryFileDescReader);
  if (!mpDirectoryFileDescReader->Read(twdDir, /*nameprefix:*/ "", os.str(), filelist)) {
    // no files for current year - return with empty list
    MSGINFO("_ProposeLangBibles: no langBibles proposed for year " << year << " - returning with empty list.");
    return;
  }

  // process list of files read for currently selected year
  string osLangBible;
  string enLangBible;
  std::vector<string>::const_iterator it;
  LANGID osLangID = mTestUserDefaultLangID != IGNORE_LANGID ? mTestUserDefaultLangID : GetUserDefaultLangID();
  for (it = filelist.begin(); it != filelist.end(); ++it) {
    // *it e.g. "de_Schlachter2000"
    ITwdFileDesc &desc = NTwdFileDesc::MakeTwdFileDesc(*it, year);
    if (_OSLangIDMatchesLang(osLangID, desc.GetLang())) {
      osLangBible = desc.GetLangBible();
    }
    // additionally remember english langBible
    else if (desc.GetLang() == "en") {
      enLangBible = desc.GetLangBible();
    }
    delete &desc;
  }
  // first add bible for user language (if found)
  if (!osLangBible.empty()) {
    MSGDEBUG(_T("CW32losDoc::_ProposeLangBibles - proposing for OS language: ") << osLangBible);
    proposedLangBibles.push_back(osLangBible);
  }
  // then add English (if found)
  if (!enLangBible.empty()) {
    MSGDEBUG(_T("CW32losDoc::_ProposeLangBibles - proposing for English: ") << enLangBible);
    proposedLangBibles.push_back(enLangBible);
  }
  // if no user language nor English file found, use first one (if any)
  if (proposedLangBibles.empty()) {
    if (!filelist.empty()) {
      ITwdFileDesc &desc = NTwdFileDesc::MakeTwdFileDesc(*filelist.begin(), year);
      MSGDEBUG(_T("CW32losDoc::_ProposeLangBibles - no OS language, no English, using first langBible: ") << desc.GetLangBible());
      proposedLangBibles.push_back(desc.GetLangBible());
      delete &desc;
    }
    else {
      MSGDEBUG(_T("CW32losDoc::_ProposeLangBibles - no OS language, no English, no langBible from directory => propose empty list."));
    }
  }
  MSGDEBUG(_T("CW32losDoc::_ProposeLangBibles done."));
}

bool CW32losDoc::_SetDate( const CTime &date)
{
  if (date.GetYear() <= 1970) {
    MSGERROR("SetDate called with year " << date.GetYear());
    // HS 2005-11-22: could use ASSERTE, but be defensive...
    // Skipping the assignment in this case avoids setting date to 1970-01-01,
    // it remains on the current date!
    return false;
  }
  bool bChanged = !EqDate(mDate, date);
  mDate = date;
  MSGDEBUG(_T("CW32losDoc::_SetDate to ") << (LPCTSTR) mDate.Format("%Y-%m-%d") << _T(", bChanged=") << bChanged);
  return bChanged;
}

OLosDoc *CW32losDoc::_GetOrLoadLosDocForLangBibleDate(const string &langBible)
{
  // lookup langBible in cache
  OLosDoc *pLosDoc = GetLosDocForLangBible(langBible);

  if (!pLosDoc) {
    // not found in cache, try to load langBible
    OLosFile *pNewLosFile = new OLosFile();
    pLosDoc = new OLosDoc(*pNewLosFile, langBible); // ==> losFile owned by pLosDoc
    if (!_Load(langBible, *pLosDoc)) {
      // delete OLosDoc again (do not keep in cache)
      delete pLosDoc;
      return 0;
    }
    // _Load ok, store in cache and return
    mLosDocCache.push_back(pLosDoc);
    return pLosDoc;
  }

  // found OLosDoc in cache...
  if (!pLosDoc->GetLosungForDate(mDate)) {
    // ... but required day is missing: _Load to get that day
    if (!_Load(langBible, *pLosDoc)) {
      // load for required day failed - keep OLosDoc in cache
      return 0;
    }
  }
  // return OLosDoc that contains the required day
  return pLosDoc;
}

bool CW32losDoc::_AddLangBibleIfExists(const string &langBible)
{
#ifdef _TEST
  if (mbTestRunning) {
    // During test, do not filter langBible by file system access but use mock reader
    OMockLangBibleYearReader *pReader = (OMockLangBibleYearReader *) mpDirectoryFileDescReader;
    if (pReader->Has(mDate.GetYear(), langBible)) {
      mLangBibles.push_back(langBible);
      return true;
    }
    return false;
  }
#endif

  OLosDoc *pLosDoc = _GetOrLoadLosDocForLangBibleDate(langBible);
  if (!pLosDoc) {
    MSGDEBUG(_T("CW32losDoc::_AddLangBibleIfExists - file does not exist, skipping ") << langBible);
    return false;
  }
  pLosDoc->SetDate(mDate);


  MSGDEBUG(_T("CW32losDoc::_AddLangBibleIfExists - adding ") << langBible);
  // add to set of langBibles shown to user
  mLangBibles.push_back(langBible);
  return true;
}

bool CW32losDoc::_SetLangBibles(const std::vector<string> *pLangBibles)
{
  MSGDEBUG(_T("CW32losDoc::_SetLangBibles..."));
  // initialize lists accessed by clients
  const std::vector<string> origLangBibles = mLangBibles;
  mLangBibles.clear();
  const std::vector<string> *pInputLangBibles = pLangBibles ? pLangBibles : &origLangBibles;

  // check each langBible passed in
  std::vector<string>::const_iterator it;
  for (it = pInputLangBibles->begin(); it != pInputLangBibles->end(); ++it) {
    _AddLangBibleIfExists(*it);
  }

  if (mLangBibles.empty()) {
    // fallback: use lang bibles previously displayed (e.g. after user navigated to year for which no files exist)
    if (!mLastNonEmptyLangBibles.empty()) {
      for (it = mLastNonEmptyLangBibles.begin(); it != mLastNonEmptyLangBibles.end(); ++it) {
        _AddLangBibleIfExists(*it);
      }
    }
    // mLangBibles may still be empty, e.g. if mLastNonEmptyLangBibles was filled for a different year!
    if (mLangBibles.empty()) {
      // fallback: propose lang bibles (based on OS language etc.), use these
      std::vector<string> proposedLangBibles;
      _ProposeLangBibles(proposedLangBibles);
      for (it = proposedLangBibles.begin(); it != proposedLangBibles.end(); ++it) {
        _AddLangBibleIfExists(*it);
      }
    }
  }
  // mLangBibles may still be empty

  bool bRes = false;
  if (origLangBibles != mLangBibles) {
    const tstring sLangBibles = NStringUtil::Join(";", mLangBibles);
    MSGDEBUG(_T("CW32losDoc::_SetLangBibles - set of langBibles changed to ") << sLangBibles);
#ifdef _TEST
    if (!mbTestRunning) {
#endif
      _Profile().WriteString(PRF_LANGBIBLES, sLangBibles.c_str());
#ifdef _TEST
    }
#endif
    bRes = true;
    if (!mLangBibles.empty()) {
      mLastNonEmptyLangBibles = mLangBibles;
    }
  }
  MSGDEBUG(_T("CW32losDoc::_SetLangBibles done."));
  return bRes;
}


void CW32losDoc::SetLangBibles(const std::vector<string> &langBibles)
{
  MSGDEBUG(_T("CW32losDoc::SetLangBibles"));
  LPARAM lHint = 0;
  if (_SetLangBibles(&langBibles)) {
    lHint = lHint | NW32losDoc::docLangBiblesChanged;
  }
  UpdateAllViews(0, lHint);
}

void CW32losDoc::SetLangBiblesDate(const std::vector<string> &langBibles, const CTime &date)
{
  MSGDEBUG(_T("CW32losDoc::SetLangBiblesDate"));
  LPARAM lHint = 0;
  bool bYearChanged = mDate.GetYear() != date.GetYear();
  if (_SetDate(date)) {
    lHint = lHint | NW32losDoc::docDateChanged;
    if (bYearChanged) {
      lHint = lHint | NW32losDoc::docYearChanged;
    }
  }
  if (_SetLangBibles(&langBibles)) {
    lHint = lHint | NW32losDoc::docLangBiblesChanged;
  }
  UpdateAllViews(0, lHint);
}

void CW32losDoc::SetDate(const CTime &date)
{
  MSGDEBUG(_T("CW32losDoc::SetDate"));
  LPARAM lHint = 0;
  bool bYearChanged = mDate.GetYear() != date.GetYear();
  if (_SetDate(date)) {
    lHint = lHint | NW32losDoc::docDateChanged;
    if (bYearChanged) {
      lHint = lHint | NW32losDoc::docYearChanged;
    }
    if (_SetLangBibles(0)) {
      lHint = lHint | NW32losDoc::docLangBiblesChanged;
    }
  }
  UpdateAllViews(0, lHint);
}

bool CW32losDoc::EqDate(const CTime &t1, const CTime &t2)
{
  return t1.GetDay()   == t2.GetDay() 
      && t1.GetMonth() == t2.GetMonth()
      && t1.GetYear()  == t2.GetYear();
}



OLosDoc *CW32losDoc::GetLosDocForLangBible(const string &langBible) const
{
  std::vector<OLosDoc*>::const_iterator it;
  int year = mDate.GetYear();
  for (it = mLosDocCache.begin(); it != mLosDocCache.end(); ++it) {
    if ((*it)->Year() == year && (*it)->LangBible() == langBible) {
      return *it;
    }
  }
  return 0;
}

OLosung *CW32losDoc::GetFirstLosung() const
{
  std::vector<string>::const_iterator it;
  for (it = mLangBibles.begin(); it != mLangBibles.end(); ++it) {
    const string &langBible = *it;
    const OLosDoc *pLosDoc = GetLosDocForLangBible(langBible);
    if (pLosDoc && pLosDoc->HasCurrentLosung()) {
      return &pLosDoc->CurrentLosung();
    }
  }
  return 0;
}

// === Lock ===

void CW32losDoc::SetLocked(bool bLocked)
{
  mbLocked = bLocked;
}

bool CW32losDoc::IsLocked() const
{
  return mbLocked;
}


#ifdef _TEST

// === TEST =========================================================================================

void CW32losDoc::UpdateAllViews(CView* pSender, LPARAM lHint, CObject* pHint)
{
  if (mbTestRunning) {
    mTestHint = lHint;
  }
  else {
    CDocument::UpdateAllViews(pSender, lHint, pHint);
  }
}

bool CW32losDoc::_OSLangIDTest()
{
  bool bRes = true;
  MSGDEBUG("--- Start CW32losDoc::_OSLangIDTest ----------------------------");

  bRes &=  _OSLangIDMatchesLang(LANG_GERMAN, "de");
  bRes &=  _OSLangIDMatchesLang(LANG_GERMAN, "de-CH");
  bRes &= !_OSLangIDMatchesLang(LANG_ENGLISH, "de-CH");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN_SWISS), "de");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN_SWISS), "de-AT");
  bRes &= !_OSLangIDMatchesLang(MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN_SWISS), "en");

  bRes &=  _OSLangIDMatchesLang(LANG_CHINESE, "zh");
  bRes &=  _OSLangIDMatchesLang(LANG_CHINESE, "zh-Hans");
  bRes &=  _OSLangIDMatchesLang(LANG_CHINESE, "zh-Hant");
  bRes &= !_OSLangIDMatchesLang(LANG_ENGLISH, "zh-Hans");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), "zh");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), "zh-Hans");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), "zh-Hant");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL), "zh");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL), "zh-Hant");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL), "zh-Hans");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_HONGKONG), "zh");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SINGAPORE), "zh");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_MACAU), "zh");
  bRes &= !_OSLangIDMatchesLang(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_MACAU), "es");

  bRes &=  _OSLangIDMatchesLang(LANG_SPANISH, "es");
  bRes &=  _OSLangIDMatchesLang(MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_MODERN), "es");

  MSGDEBUG("--- End CW32losDoc::_OSLangIDTest ----------------------------");
  return bRes;
}


void CW32losDoc::_InitTest(const CTime &initialDate, OMockLangBibleYearReader *pReader)
{
  mDate = initialDate;
  mLangBibles.clear();
  mLastNonEmptyLangBibles.clear();
  pReader->Clear();
}

bool CW32losDoc::_EvalTest(string testID, 
  const CTime &expectedDate,
  unsigned int expectedNoLangBibles, string expectedLangBibles, 
  long expectedHint)
{
  bool bRes = true;
  if (!EqDate(mDate, expectedDate)) {
    MSGERROR("Test " << testID << ": DATE expected: " << (LPCTSTR) expectedDate.FormatGmt("%Y-%m-%d") << ", got: " << (LPCTSTR) mDate.FormatGmt("%Y-%m-%d"));
    bRes = false;
  }
  if (mTestHint != expectedHint) {
    MSGERROR("Test " << testID << ": HINT expected: " << expectedHint << ", got: " << mTestHint);
    bRes = false;
  }
  if (mLangBibles.size() != expectedNoLangBibles) {
    MSGERROR("Test " << testID << ": #LangBibles expected: " << expectedNoLangBibles << ", got: " << mLangBibles.size());
    bRes = false;
  }
  if (!expectedLangBibles.empty()) {
    tstring langBibles = NStringUtil::Join(";", mLangBibles);
    if (langBibles != expectedLangBibles) {
      MSGERROR("Test " << testID << ": LangBibles expected: " << expectedLangBibles << ", got: " << langBibles);
      bRes = false;
    }
  }
  return bRes;
}

bool CW32losDoc::_LangBiblesTest()
{
  bool bRes = true;
  MSGDEBUG("--- Start CW32losDoc::_LangBiblesTest ----------------------------");

  // === constants ===
  static const tstring ANV = "af_AfrikaanseNuweVertaling";
  static const tstring S2K = "de_Schlachter2000";
  static const tstring ESV = "en_EnglishStandardVersion";
  static const CTime Date2008_12_31(2008, 12, 31, 0, 0, 0);
  static const CTime Date2008_12_30(2008, 12, 30, 0, 0, 0);
  static const CTime Date2009_12_31(2009, 12, 31, 0, 0, 0);
  static LANGID langDe = MAKELANGID(LANG_GERMAN,    SUBLANG_DEFAULT);
  static LANGID langAf = MAKELANGID(LANG_AFRIKAANS, SUBLANG_DEFAULT);

  // === setup ===
  assert(mpDirectoryFileDescReader);
  IDirectoryFileDescReader *pOriginalDirectoryFileDescReader = mpDirectoryFileDescReader;
  OMockLangBibleYearReader *pReader = new OMockLangBibleYearReader();
  mpDirectoryFileDescReader = pReader;
  CTime origDate(mDate);
  mTestUserDefaultLangID = NONMATCHING_LANGID; // match nothing
  std::vector<string> origLangBibles(mLangBibles);
  std::vector<string> origLastNonEmptyLangBibles(mLastNonEmptyLangBibles);
  mbTestRunning = true;

  // === run ===

  // 2009-09-26:
  // lang bibles handling:
  // if for current year no file exists 
  // for intersection of current mLangBibles and new langBibles,
  // 1. try to use mLastNonEmptyLangBibles
  // 2. if still none exists: use each existing one of a) OS lang, b) english
  // 3. if still none exists: use first existing one


  // change date in year with single non-English non OS-lang text
  _InitTest(Date2008_12_31, pReader);
  pReader->Add(2008, ANV);
  SetDate(Date2008_12_30);
  bRes &= _EvalTest("1", Date2008_12_30, 1, ANV, NW32losDoc::docDateChanged | NW32losDoc::docLangBiblesChanged);

  // change date to year without text
  _InitTest(Date2008_12_31, pReader);
  pReader->Add(2008, ANV);
  SetDate(Date2009_12_31);
  bRes &= _EvalTest("2", Date2009_12_31, 0, "", NW32losDoc::docDateChanged | NW32losDoc::docYearChanged);

  // change date to year keeping text (despite OS-lang)
  _InitTest(Date2008_12_31, pReader);
  mTestUserDefaultLangID = langAf;
  pReader->Add(2009, ANV);
  pReader->Add(2009, ESV);
  pReader->Add(2009, S2K); // preferred - kept
  mLangBibles.push_back(S2K);
  SetDate(Date2009_12_31);
  bRes &= _EvalTest("3.1", Date2009_12_31, 1, S2K, NW32losDoc::docDateChanged | NW32losDoc::docYearChanged);

  // change date to year with text using mLastNonEmptyLangBibles
  _InitTest(Date2008_12_31, pReader);
  mLastNonEmptyLangBibles.push_back(S2K);
  pReader->Add(2009, ESV); // first
  pReader->Add(2009, S2K); // preferred due to mLastNonEmptyLangBibles
  SetDate(Date2009_12_31);
  bRes &= _EvalTest("3.2", Date2009_12_31, 1, S2K, NW32losDoc::docDateChanged | NW32losDoc::docYearChanged | NW32losDoc::docLangBiblesChanged);

  // change date to year with OS-lang
  _InitTest(Date2008_12_31, pReader);
  mTestUserDefaultLangID = langDe;
  pReader->Add(2009, ANV); // first
  pReader->Add(2009, ESV); // preferred English
  pReader->Add(2009, S2K); // preferred due to OS-lang
  SetDate(Date2009_12_31);
  bRes &= _EvalTest("3.3", Date2009_12_31, 2, S2K + ";" + ESV, NW32losDoc::docDateChanged | NW32losDoc::docYearChanged | NW32losDoc::docLangBiblesChanged);

  // change date to year with OS-lang
  _InitTest(Date2008_12_31, pReader);
  mTestUserDefaultLangID = langAf;
  pReader->Add(2009, S2K); // first
  pReader->Add(2009, ANV); // preferred due to OS-lang
  pReader->Add(2009, ESV); // preferred English
  SetDate(Date2009_12_31);
  bRes &= _EvalTest("3.4", Date2009_12_31, 2, ANV + ";" + ESV, NW32losDoc::docDateChanged | NW32losDoc::docYearChanged | NW32losDoc::docLangBiblesChanged);

  mTestUserDefaultLangID = IGNORE_LANGID;

  // change date to year, prefer English
  _InitTest(Date2008_12_31, pReader);
  pReader->Add(2009, ANV); // first
  pReader->Add(2009, ESV);
  SetDate(Date2009_12_31);
  bRes &= _EvalTest("3.5", Date2009_12_31, 1, ESV, NW32losDoc::docDateChanged | NW32losDoc::docYearChanged | NW32losDoc::docLangBiblesChanged);

  // change date, having different set of languages, keep intersection
  _InitTest(Date2008_12_31, pReader);
  pReader->Add(2009, ANV);
  pReader->Add(2009, S2K);
  mLangBibles.push_back(ESV);
  mLangBibles.push_back(S2K);
  SetDate(Date2009_12_31);
  bRes &= _EvalTest("3.6", Date2009_12_31, 1, S2K, NW32losDoc::docDateChanged | NW32losDoc::docYearChanged | NW32losDoc::docLangBiblesChanged);

  // === SetLangBibles ===

  std::vector<string> S2KLangBibles;
  S2KLangBibles.push_back(S2K);

  // set langBibles, having different set of languages, keep intersection
  _InitTest(Date2008_12_31, pReader);
  mLastNonEmptyLangBibles.push_back(ESV); // must be ignored
  pReader->Add(2008, ANV);
  pReader->Add(2008, S2K);
  mLangBibles.push_back(ESV);
  mLangBibles.push_back(S2K);
  SetLangBibles(S2KLangBibles);
  bRes &= _EvalTest("4.1", Date2008_12_31, 1, S2K, NW32losDoc::docLangBiblesChanged);

  
  // === restore ===
  mbTestRunning = false;
  mDate = origDate;
  delete pReader;
  mpDirectoryFileDescReader = pOriginalDirectoryFileDescReader;
  mTestUserDefaultLangID = 0;
  mLangBibles = origLangBibles;
  mLastNonEmptyLangBibles = origLastNonEmptyLangBibles;

  MSGDEBUG("--- End CW32losDoc::_LangBiblesTest ----------------------------");
  return bRes;
}

bool CW32losDoc::SelfTest()
{
  bool bRes = true;
  MSGDEBUG("--- Start CW32losDoc::SelfTest ----------------------------");
  bool bDoSelfTest = true; // set false to skip self test in debugger
  bRes &= (!bDoSelfTest || _OSLangIDTest());
  bRes &= (!bDoSelfTest || _LangBiblesTest());
  MSGDEBUG("--- End CW32losDoc::SelfTest ----------------------------");
  return bRes;
}

#endif

