/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop application.
  $Id: w32los.cpp 3924 2009-09-26 17:37:12Z helmut $

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
#include "w32los.h"

#include "MainFrm.h"
#include "w32losDoc.h"
#include "w32losView.h"
#include "../online/ProxySession.h"
#include "../online/ServerDataCache.h"
#include "../filedesc/TwdFileDesc.h"
#include "../filedesc/TwdFileDescList.h"
#include "../filedesc/TwdInfo.h"
#include "../filedesc/TwdInfoList.h"
#include "../online/IUserPassDlg.h"
#include "../online/UserPassDlg.h"
#include "SignatureFormatter.h"
#include "SignatureManager.h"
#include "../util/AppPathManager.h"
#include "../util/FileManager.h"
#include "../filedesc/DirectoryFileDescReader.h"
#include "../online/IErrorMsgFormatter.h"
#include "../online/ErrorMsgFormatter.h"
#include "../encoding/Bidi.h"
#include "OptionsDlg.h"
#include "SignatureDlg.h"
#include "LangNames.h"
#include "../util/DateUtil.h"
#include "../util/tstring.h"
#include "../util/FileUtil.h"
#include "../util/PathUtil.h"
#include "../util/StringUtil.h"
#include "../util/Encoding.h"
#include "AboutDlg.h"
#include "../online/LangPackDlg.h"
#include "../online/OrganizeDlg.h"
#include "../online/PostPairDlg.h"
#include <algorithm> // find
#include "../util/ProfileDef.h"

#ifdef _TEST
// headers needed for selftest only
#include "../filedesc/TwdFileDescParser.h"
#include "../util/Layout.h"
#include "MockLayoutDlg.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Message that must be in exe since they report
// errors when loading ressource DLL:
#define _S_FAILED_ \
  "Logoskop starts with English user interface instead of '%s'\n" \
  "(as specified in the settings, see menu Edit - Options)!\n\n"
#define S_GET_VER_FAILED \
  _S_FAILED_ \
  "Reason: the required library %s is missing." 
#define S_DLL_VERSION_LT \
  _S_FAILED_ \
  "Reason: the library for the user interface language is too old!\n\n" \
  "You have installed a newer version of Logoskop, " \
  "you also need at least version %d.%d of the library '%s'."
#define S_DLL_VERSION_GT \
  _S_FAILED_ \
  "Reason: the library for the user interface language is too new!\n\n" \
  "You have installed a newer version of the library '%s', " \
  "to use it you also need at least version %d.%d of Logoskop."
#define S_LOAD_RES_DLL_FAILED \
  _S_FAILED_ \
  "Reason: loading data from %s failed."




/////////////////////////////////////////////////////////////////////////////
// CW32losApp

BEGIN_MESSAGE_MAP(CW32losApp, CWinApp)
  //{{AFX_MSG_MAP(CW32losApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  ON_COMMAND(ID_FILE_ACTUALIZE, OnFileActualize)
  ON_COMMAND(ID_FILE_ORGANIZE, OnFileOrganize)
  ON_COMMAND(ID_FILE_SIGNATURE, OnFileSignature)  

  ON_COMMAND(ID_EDIT_POST_PAIR, OnEditPostPair)
  ON_COMMAND(ID_EDIT_OPTIONS, OnEditOptions)

  ON_COMMAND(ID_HELP_PROGRAM, OnHelpProgram)
  ON_COMMAND(ID_HELP_PROJECT, OnHelpProject)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CW32losApp Konstruktion

CW32losApp::CW32losApp() 
: mpUserPassDlg(0)
, mpProxySession(0)
, mpServerDataCache(0)
, m_hInstResDLL(0)
, mpFrame(0)
, mpFileManager(0)
, mpAppPathManager(0)
, mDefaultLangNisoName("eng")
, mpDoc(0)
{
  // ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
  // Alle wichtigen Initialisierungen in InitInstance platzieren
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CW32losApp-Objekt

CW32losApp theApp;

void Usage()
{
  AfxMessageBox("Usage:\n\nLogoskop.exe [-autostart] [-i INIFILE]");
}


/////////////////////////////////////////////////////////////////////////////
// CW32losApp Initialisierung

BOOL CW32losApp::InitInstance()
{
#ifdef _AFXDLL
  Enable3dControls();      // Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
  Enable3dControlsStatic();  // Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif

  // create AppPathManager (must be deleted)
  // Needs: -
  // Sets: mpAppPathManager
  mpAppPathManager = &NAppPathManager::MakeAppPathManager();
  // Initialize APPDATA (appDataError reported below as soon as logging is available)
  tstring appDataError = mpAppPathManager->Init("bible2.net");

  // create FileManager (must be deleted - see ExitInstance)
  // Needs: mpAppPathManager
  // Sets: mpFileManager
  mpFileManager = &NFileManager::MakeFileManager(mpAppPathManager->TwdDir(), mProfile);


#ifdef _DEBUG
  long lBreakAlloc = 0; 
  // lBreakAlloc to be set in debugger
  // (see MSDN: "Detecting and Isolating Memory Leaks Using Microsoft Visual C++ ")
  if (lBreakAlloc) {
    _CrtSetBreakAlloc(lBreakAlloc); 
  }
#endif

  // process command line
  // Needs: -
  // Sets: bAutostart, iniPath
  bool bAutostart = false;
  tstring iniPath;
  if (!_ProcessCommandLine(bAutostart, iniPath)) {
    return FALSE;
  }
  
  // setup INI filenames
  // Needs: iniPath, mProfile, mpAppPathManager
  // Sets: mProfile (SetReadOnlyIniFileName, SetWritableIniFileName)
  _SetIniFileNames(iniPath);

  // setup logging
  // Needs: mProfile, mpAppPathManager
  // Sets: Logging
  _AdaptLogging();

  // report error from getting APPDATA
  // Needs: Logging
  if (!appDataError.empty()) {
    MSGERROR(appDataError);
  }

  // read mAppVersion from .exe resource, show message on failure
  // Needs: Logging
  // Sets: mAppVersion
  if (!_ReadAppVersion()) {
    return FALSE;
  }

  // check whether user wants to start only once per day
  // Needs: bAutostart, mProfile
  if (bAutostart && _MustLeaveForStartOnce()) {
    MSGINFO(_T("Autostart flag is set, and we must leave for 'start once' setting - exit application."));
    return FALSE;
  }
  // remember day of last start
  // (if started manually or by AutoStart)
  mProfile.WriteString(PRF_LAST_START, Today());

  // load dialog language setting + load ressource DLL if needed (via _SetDlgLang)
  // Needs: mProfile, mAppVersion
  // Sets: mDlgLang, eventually AfxSetResourceHandle(m_hInstResDLL)
  _DetermineDlgLang();

  // setup mpProxySession 
  // Needs: mAppVersion, mProfile
  // Sets: mpProxySession 
  _MakeProxySession();

  // create mpServerDataCache
  // Needs: mProfile, mpProxySession
  // Sets: mpServerDataCache
  {
    tstring baseurl;
    mProfile.GetString(PRF_BASEURL, DEFAULT_BASEURL, _MAX_PATH, baseurl);
    mpServerDataCache = &NServerDataCache::Make(*mpProxySession, baseurl);
  }

  // Ändern des Registrierungsschlüssels, unter dem unsere Einstellungen gespeichert sind.
  // HS 010103: used for setting "Installed" flag!
  SetRegistryKey(_T("Logoskop"));
  
  // at first start after installation (checked once per year), copy .twd from ProgDir to TwdDir
  // Needs: mAppVersion, mpAppPathManager, mProfile, mpFileManager
  _DoInitialInstall();

  // --- create document/view classes ---
  // Needs: mProfile, mpFileManager
  // Sets: mpDoc, mpFrame, initializes view
  {
    CSingleDocTemplate *pSingleDocTemplate;
    MSGDEBUG("CW32losApp::InitInstance - creating SingleDocTemplate.");
    pSingleDocTemplate = new CSingleDocTemplate(
      IDR_MAINFRAME,
      RUNTIME_CLASS(CW32losDoc), 
      RUNTIME_CLASS(CMainFrame), 
      RUNTIME_CLASS(CW32losView));
    AddDocTemplate(pSingleDocTemplate);
    MSGDEBUG("CW32losApp::InitInstance - CreateNewDocument.");
    mpDoc = (CW32losDoc*) pSingleDocTemplate->CreateNewDocument();
    {
      MSGDEBUG("CW32losApp::InitInstance - CreateNewFrame.");
      mpFrame = (CMainFrame*) pSingleDocTemplate->CreateNewFrame(mpDoc, NULL);
      CWinThread* pThread = AfxGetThread();
      if (pThread->m_pMainWnd == NULL) {
        // set as main frame (InitialUpdateFrame will show the window)
        pThread->m_pMainWnd = mpFrame;
      }
    }

    GetDoc().SetFileManager(*mpFileManager);
    GetDoc().SetProfile(mProfile);
    POSITION pos = GetDoc().GetFirstViewPosition();
    CW32losView * pView = static_cast<CW32losView*>(GetDoc().GetNextView(pos));
    if (pView) {
      MSGDEBUG("CW32losApp::InitInstance - init view and frame.");
      pView->Init(mProfile);
      mpFrame->Init(*pView, mProfile, *mpAppPathManager);
    }
    else {
      MSGERROR("InitInstance - no view, continuing.");
    }
  }

  // Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
  // Needs: mProfile
  // Sets: m_pMainWnd->SetWindowPlacement
  _RestoreWindowPlacement();

  // in document, activate set of langBibles from current or (as fallback with message box shown) next year 
  // Needs: mpFileManager, mProfile
  // Sets: langBibles and date in document
  _SetLangBiblesForCurOrNextYearWithMessage();

  // signature handling
  // Needs: langBibles and date in document
  // Sets: signature file in filesystem
  ISignatureFormatter &formatter = NSignatureFormatter::MakeSignatureFormatter();
  ISignatureManager &signatureManager = NSignatureManager::MakeSignatureManager(mProfile, mpAppPathManager->AppDataDir(), formatter);
  signatureManager.Exec(GetDoc().GetFirstLosung());
  delete &signatureManager;
  delete &formatter;

  // set focus to langCombo
  mpFrame->SetLangComboFocus();

  // finally show actualize dialog
  // Needs: langBibles from doc, mProfile, mpFileManager, mpProxySession, mpServerDataCache
  // Sets: eventually langBibles in document
  std::vector<string> langBiblesToActualize;
  GetDoc().SetLocked(true); // *** must reset!
  _CheckMustShowActualizeDialogFor(/*bInteractive=*/ false, langBiblesToActualize);
  // langBiblesToActualize is <= langBibles from doc
  if (!langBiblesToActualize.empty()) {
    // FileActualize has same effect as ShowWindows(SW_SHOW) + UpdateWindow()
    FileActualize(langBiblesToActualize);
  }
  GetDoc().SetLocked(false); // ***

#ifdef _TEST
  bool bDoSelfTest = true; // set false to skip self test in debugger
  if (bDoSelfTest) {
    bool bAllOk = true;
    bAllOk &= (!bDoSelfTest || GetDoc().SelfTest());
    bAllOk &= (!bDoSelfTest || FileManager().SelfTest());
    bAllOk &= (!bDoSelfTest || NStringUtil::SelfTest());
    bAllOk &= (!bDoSelfTest || NEncoding::SelfTest());
    bAllOk &= (!bDoSelfTest || NTwdFileDescParser::SelfTest());
    bAllOk &= (!bDoSelfTest || NTwdFileDescList::SelfTest());
    bAllOk &= (!bDoSelfTest || NTwdInfoList::SelfTest());

    bAllOk &= (!bDoSelfTest || NLayout::SelfTest());
    bAllOk &= (!bDoSelfTest || NMockLayoutDlg::SelfTest());

    // ... add more tests...
    ASSERT(bAllOk);
  }
#endif

  return TRUE;
}


CW32losDoc &CW32losApp::GetDoc()
{
  return *mpDoc;
}

CProfile &CW32losApp::Profile()
{
  return mProfile;
}

// === Helpers for InitInstance ===============================================
// (in order of call)

BOOL CW32losApp::_ProcessCommandLine(/*out:*/ bool bAutostart, tstring &iniPath)
{
  // -a[utostart]
  // -iINIFILE
  // -i INIFILE
  for (int i = 1; i < __argc; i++) {
    LPCTSTR pszParam = __targv[i];
    BOOL bLast = ((i + 1) == __argc);
    if (pszParam[0] == '-' || pszParam[0] == '/') {
      ++pszParam;
      switch (pszParam[0]) {
      case 'a':
        // -a[utostart]
        bAutostart = true;
        break;
      case 'i':
        if (pszParam[1]) {
          // -iINIFILE
          iniPath = pszParam + 1;
        }
        else if (!bLast) {
          // -i INIFILE
          ++i;
          iniPath = __targv[i];
        }
        else {
          // -i
          Usage();
          return FALSE;
        }
        break;
      default:
        // -x
        Usage();
        return FALSE;
      }
    }
    else {
      // blabla
      Usage();
      return FALSE;
    }
  }
  return TRUE;
}

void CW32losApp::_SetIniFileNames(const tstring &iniPath)
{
  OPathUtil pathUtil;

  // set secondary (read-only) INI path from dir of application (= default in pathUtil)
  pathUtil.SetDriveDir(mpAppPathManager->ProgDir()); // this is OPathUtil default, but be defensive
  mProfile.SetReadOnlyIniFileName(pathUtil.PathFromExt(_T("ini")));

  // set primary (read/write) INI path from .exe parameter "iniPath", or from <APPDATA>
  if (iniPath.empty()) {
    pathUtil.SetDriveDir(mpAppPathManager->DefaultIniDir());
    tstring usedIniPath = pathUtil.PathFromExt(_T("ini"));
    mProfile.SetWritableIniFileName(usedIniPath);
  }
  else {
    mProfile.SetWritableIniFileName(iniPath);
  }
}

void CW32losApp::_AdaptLogging(bool bIsStart)
{
  unsigned int level = mProfile.GetInt(PRF_LOGFILE_LEVEL, DEFAULT_LOGFILE_LEVEL);

  // set level also if level is 0 (= turn off)
  NMsgSingleModule::SetLevel(level);
  NBidi::ActivateLogging(level >= MSGLEVEL_DEBUG);

  if (level) {
    // retrieve INI file data before activating logging (avoid debug output if level excludes debug)
    tstring logfile;
    mProfile.GetString(PRF_LOGFILE, mpAppPathManager->LogPath(), /*adlib*/_MAX_PATH, logfile);
    // set filename
    NMsgSingleModule::SetFilename(logfile.c_str());
    if (bIsStart) {
      MSGINFO("--- Start logging " << Now() << " ---");
    }
  }
}

BOOL CW32losApp::_ReadAppVersion()
{
  TCHAR buf[_MAX_PATH];
  if (::GetModuleFileName(AfxGetInstanceHandle(), buf, sizeof(buf))) {
    // this is for display purposes only - don't bother if it fails...
    // ReadVersionFrom must be after start of logging
    if (!mAppVersion.ReadVersionFrom(buf)) {
      AfxMessageBox(IDS_GET_VER_FAILED, MB_OK);
      return FALSE;
    }
    // write app name to logfile
    MSGINFO(m_pszAppName << " " << mAppVersion.Version());
  }
  else {
    MSGERROR(_T("Failed to read version of application, continuing."));
  }
  return TRUE;
}

BOOL CW32losApp::_MustLeaveForStartOnce()
{
  MSGDEBUG("CW32losApp::_MustLeaveForStartOnce() - checking...");
  bool startOnce = mProfile.GetBool(PRF_START_ONCE, DEFAULT_START_ONCE);
  tstring lastStart;
  // CR005 "w32los starts twice on WindowsXP"
  if (startOnce) {
    int day;
    int month;
    int year;
    CTime now = CTime::GetCurrentTime();
    if (0 < mProfile.GetString(PRF_LAST_START, "", /*adlib*/_MAX_PATH, lastStart)
      && 3 == sscanf(lastStart.c_str(), "%d-%d-%d", &year, &month, &day)
      // CTime: 1970..2038 - be defensive...
      && year >= 1970 && year < 2039 && month >= 1  && month <= 12 && day >= 1 && day <= 31
      && (now.GetYear() == year && now.GetMonth() == month && now.GetDay() == day)) {
      MSGINFO("Leaving program - it has already been started today, and option " PRF_START_ONCE " is set");
      return TRUE;
    }
  }
  MSGDEBUG("CW32losApp::_MustLeaveForStartOnce() - FALSE.");
  return FALSE;
}

void CW32losApp::_DetermineDlgLang()
{
  MSGDEBUG("CW32losApp::_DetermineDlgLang().");
  tstring dlgLang; // local value
  mDlgLang = mDefaultLangNisoName; // *** init mDlgLang ***
  mProfile.GetString(PRF_DIALOGLANG, "", /*adlib*/_MAX_PATH, dlgLang);
  bool bHaveDlgLangFromIni = !dlgLang.empty();
  if (dlgLang.empty()) {
    // No language from INI file
    // => determine initial language from OS language
    // (not stored to INI - do this check at every start)
    // Move this to separate method (in LangNames.cpp?) 
    //   if there are more resource files some time.
    if (PRIMARYLANGID(GetUserDefaultLangID()) == LANG_GERMAN) {
      MSGDEBUG("CW32losApp::_DetermineDlgLang() - detected German.");
      dlgLang = "ger";
    }
    else {
      // fallback: default language (= built-in, no resource DLL to load)
      dlgLang = mDefaultLangNisoName;
    }
  }
  // === apply detected language ===
  if (dlgLang != mDefaultLangNisoName) {
    // Try to use the detected language for GUI
    if (!_SetDlgLang(dlgLang, /*report error: */ bHaveDlgLangFromIni)) { // *** sets mDlgLang ***
      if (bHaveDlgLangFromIni) {
        MSGERROR("CW32losApp::_DetermineDlgLang() - _SetDlgLang(" << dlgLang << ") failed, fallback to " << mDefaultLangNisoName);
      }
      // failed to use detected language for GUI
      // => use English
      mDlgLang = mDefaultLangNisoName; // *** resets mDlgLang ***
      mProfile.WriteString(PRF_DIALOGLANG, mDlgLang.c_str());
    }
  }
}

bool CW32losApp::_SetDlgLang(const string &dlgLang, bool bReportError)
{
  mDlgLang = dlgLang;
  if (mDlgLang == mDefaultLangNisoName) {
    // no need since SetDlgLang is done only at startup AfxSetResourceHandle(AfxGetInstanceHandle());
    if (m_hInstResDLL) {
      FreeLibrary(m_hInstResDLL);
      m_hInstResDLL = 0;
    }
    return true;
  }
  
  CString buf;
  string strResDLL = GetResDLLName();
  const char *resDLL = strResDLL.c_str();
  CVersionInfo resVersion;
  string nisoDlgLang = NLangNames::MakeNisoDisplayName(dlgLang);
  if (!resVersion.ReadVersionFrom(resDLL)) {
    MSGDEBUG(_T("CW32losApp::_SetDlgLang - resource DLL not read: ") << strResDLL);
    buf.Format(S_GET_VER_FAILED, nisoDlgLang.c_str(), resDLL); // msg taken from exe
  }
  // check version of language ressource dll
  // require identical main version
  // and res sub version >= app sub version
  else if (resVersion.Main() < mAppVersion.Main() || 
     (resVersion.Main() == mAppVersion.Main() && 
     (resVersion.Sub() < mAppVersion.Sub()))) {
    MSGDEBUG(_T("CW32losApp::_SetDlgLang - resource DLL too old: ") << strResDLL);
    // string taken from exe:
    buf.Format(S_DLL_VERSION_LT, nisoDlgLang.c_str(), mAppVersion.Main(), mAppVersion.Sub(), resDLL);
  }
  else if (resVersion.Main() > mAppVersion.Main()) {
    MSGDEBUG(_T("CW32losApp::_SetDlgLang - resource DLL too new: ") << strResDLL);
    // string taken from exe:
    buf.Format(S_DLL_VERSION_GT, nisoDlgLang.c_str(), resDLL, resVersion.Main(), resVersion.Sub());
  }
  else {
    // load language ressource dll
    m_hInstResDLL = LoadLibrary(resDLL); 
    if (!m_hInstResDLL) {
      MSGDEBUG(_T("CW32losApp::_SetDlgLang - resource DLL loading failed: ") << strResDLL);
      buf.Format(S_LOAD_RES_DLL_FAILED, nisoDlgLang.c_str(), resDLL);
    }
    else {
      MSGDEBUG(_T("CW32losApp::_SetDlgLang - using language '") << nisoDlgLang << _T("' with resource DLL ") << strResDLL);
      AfxSetResourceHandle(m_hInstResDLL); // ***
      return true;
    }
  }
  if (bReportError) {
    AfxMessageBox(buf);
  }
  return false;
}

void CW32losApp::_MakeProxySession()
{
  MSGDEBUG("CW32losApp::_MakeProxySession().");
  tstring appNameVersion = tstring(m_pszAppName) + " (" + mAppVersion.Version() + ")";
  mpUserPassDlg = &NUserPassDlg::MakeUserPassDlg();
  mpProxySession = &NProxySession::MakeProxySession(appNameVersion, *mpUserPassDlg);

  // handle proxy
  tstring useProxyStr;
  if (0 < mProfile.GetString(PRF_USEPROXY, DEFAULT_USEPROXY, /*max*/64, useProxyStr)) {
    // default: first char from inifile value
    tUseProxy useProxy = static_cast<tUseProxy>(useProxyStr[0]); 
    // check this char
    if (useProxy != proxyDirect && useProxy != proxyManual) {
      useProxy = proxyPreconfig;
    }
    // get proxy string from profile (in any case - don't lose the setting!)
    tstring proxy;
    mProfile.GetString(PRF_PROXY, DEFAULT_PROXY, _MAX_PATH, proxy);
    MSGDEBUG("CW32losApp::_MakeProxySession() - using proxy='" << proxy << "', useProxy=" << useProxy << " (P-reconfig, D-irect, M-anual)");
    mpProxySession->SetProxy(useProxy, proxy);
  }
  // allow user altering the default timeout
  unsigned int timeout = mProfile.GetInt(PRF_TIMEOUT, DEFAULT_TIMEOUT); 
  if (timeout) {
    MSGDEBUG("CW32losApp::_MakeProxySession() - using timeout=" << std::dec << timeout);
    mpProxySession->SetTimeout(timeout);
  }
}

void CW32losApp::_DoInitialInstall()
{
  /*
  Installer copies .twd files to .exe folder.
  Requirements:
  - Logoskop must copy them to the APPDATA folder, but 
  - not at each start (for performance)
  - not if user deleted a .twd file (via Organize dialog) (difficult to fulfill!)

  Concept:
  - in INI file, remember maximum year of any copied .twd file, =: MAX_YEAR_INSTALLED
  - in INI file, remember Logoskop version that was started the last time, =: LAST_RUN_VERSION

  IF ( !LAST_RUN_VERSION 
    || LAST_RUN_VERSION < current version
    || !MAX_YEAR_INSTALLED
    || MAX_YEAR_INSTALLED < today.GetYear()
    || MAX_YEAR_INSTALLED == today.GetYear() && today.GetMonth() >= TBD 10)
  THEN
    FOR all files FILE in .exe dir DO
      IF (FILE does not exist in APPDATA) 
      && (year Y of FILE in .exe dir > MAX_YEAR_INSTALLED 
        (i.e. install only once per FILE year, but possibly for year past)
      THEN 
        install FILE from .exe dir to APPDATA
        remember to adapt MAX_YEAR_INSTALLED in profile (after loop finished)
      END
    END
  END

  Resulting behaviour:
  - Initial install: 
    copy all files, OK.
  - Install 1 of installer 2009, install 2 of same installer 2009 in same year 2009, before month 10: 
    no action, OK (MAX_YEAR_INSTALLED = current year)
  - Install 1 of installer 2009, install 2 of same installer 2009 in same year 2009, after month 10: 
    no action, OK (month triggers entering loop, but files exist).
  - Install 1 of installer 2009, install 2 of installer 2010 in same year 2009, before month 10: 
    no action, OK (month prevents from entering loop; installer 2010 should be available only at month 10)
  - Install 1 of installer 2009, install 2 of installer 2010 in same year 2009, after month 10: 
    copy files, OK
  - Install 1 of installer 2009 in 2009, install 2 of installer 2010 in 2010:
    copy files, OK
  - Install 1 of installer 2009 in 2009, Logoskop start in 2009 before month 10:
    no action, OK (month < 10).
  - Install 1 of installer 2009 in 2009, Logoskop start in 2009 after month 10:
    no action, OK (files exist).
  - Install 1 of installer 2009 in 2009, Logoskop start in 2010:
    no action, OK (files exist).
  */
  MSGDEBUG("CW32losApp::_DoInitialInstall...");

  tstring lastRunVersion;
  bool bLastRunVersionOlder = 
    (0 == mProfile.GetString(PRF_LAST_RUN_VERSION, "", /*max*/64, lastRunVersion)
    || lastRunVersion < mAppVersion.Version());
  if (bLastRunVersionOlder) {
    // increment PRF_LAST_RUN_VERSION
    mProfile.WriteString(PRF_LAST_RUN_VERSION, mAppVersion.Version());
  }
  
  int maxYearInstalled = mProfile.GetInt(PRF_MAX_YEAR_INSTALLED, 0);
  int firstMonthForNextYear = mpFileManager->GetFirstMonthForNextYear();
  
  CTime today = CTime::GetCurrentTime();
  if (! 
    (bLastRunVersionOlder
     || !maxYearInstalled
     || maxYearInstalled < today.GetYear()
     || (maxYearInstalled == today.GetYear() && today.GetMonth() >= firstMonthForNextYear))) {
    MSGDEBUG(_T("CW32losApp::_DoInitialInstall: nothing to do: bLastRunVersionOlder=") << bLastRunVersionOlder
      << _T(", maxYearInstalled=") << std::dec << maxYearInstalled);
    return;    
  }
  // cf. -->COrganizeDlg::_FillInstalledList()

  ODirectoryFileDescReader reader;
  std::vector<string> fileList;
  if (!reader.Read(mpAppPathManager->ProgDir(), "", ".twd", fileList)) {
    MSGDEBUG(_T("CW32losApp::_DoInitialInstall: no The Word file found in ProgDir, skip."));
    return;
  }

  LANGID osLangID = GetUserDefaultLangID();
  int newMaxYearInstalled = maxYearInstalled;

  // process all paths
  OPathUtil pathUtil(mpAppPathManager->ProgDir());
  // cf. -->COrganizeDlg::OnUnpack()
  std::vector<string>::const_iterator it;
  ITwdFileDesc *pDesc = 0;
  for (it = fileList.begin(); it != fileList.end(); ++it) {
    // *it == e.g. "de_Schlachter2000_2008"
    delete pDesc;
    pDesc = &NTwdFileDesc::MakeTwdFileDesc(*it); // *** delete ***
    if (pDesc->GetYear() <= maxYearInstalled) {
      continue;
    }

    string tgtPath = mpFileManager->GetTwdFilePath(pDesc->GetYear(), pDesc->GetLangBible());
    if (FileUtil::Exists(tgtPath.c_str())) {
      continue;
    }

    if (pDesc->GetYear() > newMaxYearInstalled) {
      newMaxYearInstalled = pDesc->GetYear();
    }
    string srcPath = pathUtil.PathFromNameExt(*it, ".twd");
    string data = FileUtil::Read(srcPath);
    if (!data.length()) {
      MSGERROR(_T("InitialInstall: no data read from ") << srcPath << _T(", skip."));
    }
    else {
      // install one path
      if (!mpFileManager->Install(pDesc->GetYear(), pDesc->GetLangBible(), data)) {
        MSGERROR(_T("InitialInstall: failed to install ") << *it);
      }
    }
  }
  delete pDesc;
  MSGDEBUG(_T("CW32losApp::_DoInitialInstall: newMaxYearInstalled=") << newMaxYearInstalled);
  mProfile.WriteInt(PRF_MAX_YEAR_INSTALLED, newMaxYearInstalled);
}


void CW32losApp::_RestoreWindowPlacement()
{
  MSGDEBUG(_T("CW32losApp::_RestoreWindowPlacement..."));
  WINDOWPLACEMENT wp;
  RECT &r = wp.rcNormalPosition;
  const int NONE = -1;
  r.left   = mProfile.GetInt(_T("Left"),   NONE); 
  r.top    = mProfile.GetInt(_T("Top"),    NONE); 
  r.right  = mProfile.GetInt(_T("Right"),  NONE); 
  r.bottom = mProfile.GetInt(_T("Bottom"), NONE); 
  // HS 2004-06-19:
  // wp.showCmd = SW_HIDE
  // --> nothing is shown at SetWindowPlacement time,
  //     only the ShowWindow call displays the empty frame.
  wp.showCmd = mProfile.GetInt(_T("ShowCmd"), NONE); 
  wp.flags   = mProfile.GetInt(_T("Flags"),   NONE); 
  if (r.left != NONE && r.top != NONE && r.right != NONE && r.bottom != NONE
    && wp.showCmd != NONE && wp.flags != NONE) {
    wp.ptMinPosition = CPoint(0, 0);
    wp.ptMaxPosition = CPoint(-GetSystemMetrics(SM_CXBORDER), - GetSystemMetrics(SM_CYBORDER));
    // HS 2004-06-19: with wp.showCmd = mProfile.GetInt(...), 
    // SetWindowPlacement() displays empty frame: 
    // title, menu bar, window border, but inner area unchanged 
    // (= "look through" to previous desktop contents)
    MSGDEBUG(_T("CW32losApp::_RestoreWindowPlacement to ")
          << wp.rcNormalPosition.left  << ", " << wp.rcNormalPosition.top
          << wp.rcNormalPosition.right << ", " << wp.rcNormalPosition.bottom);
    m_pMainWnd->SetWindowPlacement(&wp);
  }
  else {
    MSGDEBUG(_T("CW32losApp::_RestoreWindowPlacement - no settings from INI file => just ShowWindow."));
    m_pMainWnd->ShowWindow(SW_SHOW);
  }
}

void CW32losApp::_CheckMustShowActualizeDialogFor(bool bInteractive, /*out*/std::vector<string> &langBiblesToActualize)
{
  MSGDEBUG(_T("CW32losApp::_CheckMustShowActualizeDialogFor..."));
  CTime &date = CTime::GetCurrentTime();
  int thisYear  = date.GetYear();
  int thisMonth = date.GetMonth();
  bool bTestShowForAll = false;

  string lastAutoLangPackDlg;
  mProfile.GetString(PRF_AUTOLANGPACKDLG, _T(""), 255, lastAutoLangPackDlg);
  bool bAutoLangpackOn = mProfile.GetBool(PRF_AUTOLANGPACKDLGON, DEFAULT_AUTOLANGPACKDLGON);

  int yearToCheck = (thisMonth < FileManager().GetFirstMonthForNextYear()) ? thisYear : (thisYear + 1);

  langBiblesToActualize.clear(); // be defensive
  const std::vector<string> &docLangBibles = GetDoc().GetLangBibles();
  std::vector<string>::const_iterator it;
  for (it = docLangBibles.begin(); it != docLangBibles.end(); ++it) {
    const string &docLangBible = *it;
    const string twdPath = FileManager().GetTwdFilePath(yearToCheck, docLangBible);
    bool filenameToActualizeExists = FileUtil::Exists(twdPath.c_str());

    if (bInteractive && !filenameToActualizeExists) {
      // do not record interactive call for PRF_AUTOLANGPACKDLG
      langBiblesToActualize.push_back(docLangBible);
    }
    else {
      string autoLangPackDlgMonth =
        // MustShowActualizeDialog() returns empty if filenameToActualizeExists or dialog has been shown this month...
        FileManager().MustShowActualizeDialog(thisYear, thisMonth, filenameToActualizeExists,
          lastAutoLangPackDlg, bAutoLangpackOn);
      if (bTestShowForAll || !autoLangPackDlgMonth.empty()) {
        if (langBiblesToActualize.empty()) {
          mProfile.WriteString(PRF_AUTOLANGPACKDLG, autoLangPackDlgMonth);
        }
        langBiblesToActualize.push_back(docLangBible);
      }
    }
  }
  MSGDEBUG(_T("CW32losApp::_CheckMustShowActualizeDialogFor done."));
}

void CW32losApp::_SetLangBiblesForCurOrNextYearWithMessage()
{
  MSGDEBUG(_T("CW32losApp::_SetLangBiblesForCurOrNextYearWithMessage..."));

  // read proposedLangBibles from INI
  std::vector<string> proposedLangBibles;
  {
    string sLangBibles;

    if (0 < mProfile.GetString(PRF_LANGBIBLES, _T(""), /*max*/2048, sLangBibles)) {
      MSGDEBUG("CW32losApp::InitInstance() - proposed langBibles from INI = " << sLangBibles);
      NStringUtil::Split(";", sLangBibles, proposedLangBibles);
    }
    else if (0 < mProfile.GetString(PRF_DOCLANG_V2, _T(""), /*max*/2048, sLangBibles)) {
      MSGDEBUG("CW32losApp::InitInstance() - proposed single langBible from Logoskop pre 3.0 INI = " << sLangBibles);
      proposedLangBibles.push_back(sLangBibles);
    }
    else {
      MSGDEBUG("CW32losApp::InitInstance() - no proposed langBibles from INI.");
    }
  }

  // from proposedLangBibles, collect list of existing files for current or (as fallback) next year
  std::vector<string> thisYearLangBibles;
  std::vector<string> nextYearLangBibles;
  const CTime date = CTime::GetCurrentTime();
  int thisYear = date.GetYear();
  {
    std::vector<string>::const_iterator it;
    for (it = proposedLangBibles.begin(); it != proposedLangBibles.end(); ++it) {
      const string &langBible = *it;
      int yearToRead = FileManager().ThisOrNextYearForThatFileExists(thisYear, date.GetMonth(), langBible);
      if (yearToRead == thisYear) {
        thisYearLangBibles.push_back(langBible);
      }
      else if (yearToRead == thisYear+1) {
        nextYearLangBibles.push_back(langBible);
      }
    }
  }

  // put list of existing files for current or (as fallback) next year into doc
  if (!thisYearLangBibles.empty()) {
    // use existing files for current year
    MSGDEBUG(_T("CW32losApp::_SetLangBiblesForCurOrNextYearWithMessage: got langBibles for current year."));
    GetDoc().SetLangBiblesDate(thisYearLangBibles, date);
  }
  else if (!nextYearLangBibles.empty()) {
    // as fallback, use existing files for next year
    MSGDEBUG(_T("CW32losApp::_SetLangBiblesForCurOrNextYearWithMessage: got langBibles for next year."));
    GetDoc().SetLangBiblesDate(nextYearLangBibles, CTime(thisYear+1, date.GetMonth(), date.GetDay(), 0, 0, 0));

    // display hint that next year is being used - but display it only once per year
    int yearQueried = mProfile.GetInt(PRF_NEXTYEAR_QUERIED, 0);
    if (yearQueried < thisYear) {
      mProfile.WriteInt(PRF_NEXTYEAR_QUERIED, thisYear);
      CString msg;
      msg.Format(IDS_USE_NEXT_YEAR, thisYear, thisYear+1, thisYear);
      AfxMessageBox(msg);
    }
  }
  else {
    // pass empty list to doc, let doc make a proposal
    MSGDEBUG(_T("CW32losApp::_SetLangBiblesForCurOrNextYearWithMessage: no langBibles."));
    GetDoc().SetLangBiblesDate(thisYearLangBibles, date);
  }
  MSGDEBUG(_T("CW32losApp::_SetLangBiblesForCurOrNextYearWithMessage done."));
}


// === Utility methods ==============================================================

const tstring CW32losApp::Today() const
{
  CTime now = CTime::GetCurrentTime();
  std::tostringstream s;
  s << now.GetYear() << "-"
    << std::setw(2) << std::setfill('0') << now.GetMonth() << "-"
    << std::setw(2) << std::setfill('0') << now.GetDay();
  return s.str();
}

const tstring CW32losApp::Now() const
{
  CTime now = CTime::GetCurrentTime();
  std::tostringstream s;
  s << now.GetYear() << "-"
    << std::setw(2) << std::setfill('0') << now.GetMonth() << "-"
    << std::setw(2) << std::setfill('0') << now.GetDay() <<  " "
    << std::setw(2) << std::setfill('0') << now.GetHour() << ":" 
    << std::setw(2) << std::setfill('0') << now.GetMinute();
  return s.str();
}


BOOL CW32losApp::DoPromptFileNames(
  CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog,
  CString& strFilterName, CString& strFilterExt, BOOL bAppendAllFiles,
  unsigned int nMaxFiles)
{
  CFileDialog dlgFile(bOpenFileDialog);

  CString title;
  VERIFY(title.LoadString(nIDSTitle));
  
  dlgFile.m_ofn.Flags |= lFlags;
  if (nMaxFiles > 1) {
    dlgFile.m_ofn.Flags |= OFN_ALLOWMULTISELECT | OFN_EXPLORER; // ensure 0 separated result (?)
  }
  // for "Install", there is no need to start in exe directory
  // dlgFile.m_ofn.lpstrInitialDir = mDriveDir.c_str();
  
  CString strFilter;
  CString strDefault;
  {
    // set the default extension
    strDefault = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
    dlgFile.m_ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(strDefault);
    dlgFile.m_ofn.nFilterIndex = dlgFile.m_ofn.nMaxCustFilter + 1;  // 1 based number
  }
  
  // add to filter
  strFilter += strFilterName;
  ASSERT(!strFilter.IsEmpty());  // must have a file type name
  strFilter += (TCHAR)'\0';  // next string please
  strFilter += (TCHAR)'*';
  strFilter += strFilterExt;
  strFilter += (TCHAR)'\0';  // next string please
  dlgFile.m_ofn.nMaxCustFilter++;
  
  if (bAppendAllFiles) {
     // append the "*.*" all files filter
    CString allFilter;
    VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
    strFilter += allFilter;
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.*");
    strFilter += (TCHAR)'\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;
  }
  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.lpstrTitle = title;
  // point into fileName (output parameter)
  dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH * nMaxFiles + 2);
  dlgFile.m_ofn.nMaxFile = _MAX_PATH * nMaxFiles;
  

  int nResult = dlgFile.DoModal();

  if (nResult == IDOK && nMaxFiles > 1) {
    // MSDN: 
    // If the OFN_ALLOWMULTISELECT flag is set and the user selects multiple files,
    // the buffer contains the current directory followed by the filenames of the selected files. 
    // For Explorer-style dialog boxes, the directory and filename strings are NULL separated, 
    // with an extra NULL character after the last filename. 
    //
    // 2009-01-03 HS: CR 025 SignatureTargetWithoutFilename
    // Even with single selection (= without flag OFN_ALLOWMULTISELECT),
    // dlgFile.m_ofn.nFileOffset is set, it points to the slash between folder and filename
    //
    //                                           !<-- here
    // S:\helmut\prjwin\Bible20\Logoskop\testdata\signatureOutputTestTarget.txt"
    //
    // In version 2.0, even without flag OFN_ALLOWMULTISELECT a \0 was inserted here,
    // to have consistent behaviour for single / multi selection,
    // but the only caller using single selection (signature target selection)
    // did not implement the special behaviour!
    // ==> in single selection case, drop the special behaviour,
    //     just return the complete path.
    //
    // For 3.0: ... but in multi-selection case, keep the special behaviour
    // even if user selects only one file, for the caller using multi-selection
    // (Organize dialog). 
    // Code copied from TheWordOrganizer w32los.cpp r3037,
    // was also contained in Logoskop w32los.cpp r3003.
    // But: added condition for nMaxFiles > 1!!!
    char *pFileName = dlgFile.m_ofn.lpstrFile;
    if (dlgFile.m_ofn.nFileOffset && pFileName[dlgFile.m_ofn.nFileOffset-1]) {
      // for single file, the buffer contains just one complete path.
      // for multiple files, it starts with drive+dir, 0, then filenames;
      //   nFileOffset points behind the 0.
      // By inserting the 0 for the single file case, 
      // the caller can handle both cases the same way.
      pFileName[dlgFile.m_ofn.nFileOffset-1] = 0;
      // be sure single filename is terminated by double 0
      char * f = pFileName + dlgFile.m_ofn.nFileOffset;
      f += strlen(f); // points to terminating 0
      if (f < dlgFile.m_ofn.lpstrFile + dlgFile.m_ofn.nMaxFile) {
        *(f+1) = 0; // must be ok, use "+ 2" above
      }
    }
  }
  fileName.ReleaseBuffer();
  return nResult == IDOK;
}

void CW32losApp::FileActualize(const std::vector<string> &langBiblesToActualize) 
{
  MSGDEBUG(_T("CW32losApp::FileActualize..."));
  // compute multi-line string with list of langBibles to actualize
  std::vector<tstring>::const_iterator it;
  tstring langBibleString;
  for (it = langBiblesToActualize.begin(); it != langBiblesToActualize.end(); ++it) {
    const string &langBible = *it;
    // get consistent string representation of lang/bible
    ITwdFileDesc &desc = NTwdFileDesc::MakeTwdFileDesc(langBible, 2000);// random year
    if (!langBibleString.empty()) {
      langBibleString += _T("\n");
    }
    langBibleString += desc.GetDisplayName();
    delete &desc;
  }

  CTime &date = CTime::GetCurrentTime();
  int thisYear  = date.GetYear();
  int thisMonth = date.GetMonth();
  int defaultYear = (thisMonth < FileManager().GetFirstMonthForNextYear()) ? thisYear : (thisYear + 1);

  // create and show dialog
  CLangPackDlg langPackDlg(
    langBibleString, defaultYear,
    mpProxySession->GetUseProxy(), mpProxySession->GetProxy(), mProfile);
  if (IDOK != langPackDlg.DoModal()) {
    return;
  }

  CWaitCursor wait;

  // save proxy configuration if changed
  if (langPackDlg.UseProxy() != mpProxySession->GetUseProxy()
      || langPackDlg.Proxy() != mpProxySession->GetProxy()) {
    mpProxySession->SetProxy(langPackDlg.UseProxy(), langPackDlg.Proxy());
  }
  ASSERT(mpServerDataCache);

  bool bFillOk = mpServerDataCache->Fill(); // *** HTTP GET ***
  OTwdFileDescList& serverFileList = mpServerDataCache->GetFileDescList();
  OTwdInfoList&     serverInfoList = mpServerDataCache->GetInfoList();

  // determine language of server info
  std::vector<tstring> infoLangFilterList;
  ComputeInfoLangFilterList(infoLangFilterList);

  // --- process files ---
  CString buf;
  tstring status;
  std::vector<string> installedLangBibles;
  for (it = langBiblesToActualize.begin(); it != langBiblesToActualize.end(); ++it) {
    const tstring &langBible = *it;
    tstring langBibleStatus;
    ITwdFileDesc *pTwdFileDesc = serverFileList.Find(langBible, langPackDlg.GetSelectedYear());
    if (!pTwdFileDesc) {
      MSGERROR(_T("FileActualize: no file in serverFileList for ") << langBible << _T(", skipping."));
      buf.LoadString(IDS_FILENOTAVAIL);
      langBibleStatus = (LPCSTR) buf;
    }
    else {
      // install file
      string data;
      ASSERT(mpProxySession);
      int res = mpProxySession->Get(pTwdFileDesc->GetUrl(), data); // *** HTTP GET ***
      if (res) {
        IErrorMsgFormatter& fmt = NErrorMsgFormatter::MakeErrorMsgFormatter(pTwdFileDesc->GetUrl());
        fmt.FormatDownloadError(IDS_GETFILE_FAILED, res, buf);
        delete &fmt;
        langBibleStatus = (LPCSTR) buf;
        MSGERROR("FileActualize: error from HTTP GET for " << langBible << ", skipping: " << langBibleStatus);
      }
      else if (!FileManager().Install(langPackDlg.GetSelectedYear(), langBible, data)) {
        buf.LoadString(IDS_WRITEERR_1);
        langBibleStatus = (LPCSTR) buf;
        MSGERROR("FileActualize: install failed for " << langBible << ", skipping: " << langBibleStatus);
      }
      else {
        installedLangBibles.push_back(langBible);

        buf.LoadString(IDS_INSTALLED_1);
        langBibleStatus = (LPCSTR) buf;

        // --- display info from server ---
        // Same in <--COrganizeDlg::OnDownload() !!
        //TODO low 2008-11-15 HS: duplicate code for displaying info from serverInfoList (Actualize vs. Organize)
        ITwdInfo* pInfo = serverInfoList.FindFirst(infoLangFilterList);
        if (pInfo && !pInfo->GetInfo().empty()) {
          tstring lastInfo;
          tstring key(tstring(PRF_LAST_LANG_INFO) + "_" + langBible);
          if (!Profile().GetString(key, "", 64, lastInfo)
              || lastInfo < pInfo->GetUpdated()) {
            Profile().WriteString(key, (LPCSTR) pInfo->GetUpdated().c_str());
            string info;
            if (NEncoding::Narrow(pInfo->GetInfo(), info)) {
              langBibleStatus += tstring(_T("\n")) + info.c_str();
            }
            // else :-(
          }
        }
      }
    }
    if (!status.empty()) {
      status += "\n\n";
    }
    status += langBible + _T(":\n") + langBibleStatus;
  }
  if (GetDoc().GetLangBibles().empty()) {
    MSGDEBUG(_T("CW32losApp::FileActualize: no langBibles in Doc so far => SetLangBibles to installed langBibles"));
    GetDoc().SetLangBiblesDate(installedLangBibles, langPackDlg.GetSelectedYear());
  }
  else {
    MSGDEBUG(_T("CW32losApp::FileActualize: having langBibles in Doc => update frame combo-boxes"));
    mpFrame->FillLangbox();
  }
  AfxMessageBox(status.c_str());
}

void CW32losApp::ComputeInfoLangFilterList(std::vector<tstring>& infoLangFilterList) 
{
  tstring tmp(GetDoc().GetFirstLangBible());
  tstring lang = NStringUtil::Pop("_", tmp);
  if (lang == "de") {
    infoLangFilterList.push_back(lang);
  }
  infoLangFilterList.push_back("en");
}


// === IAppInterface ===

unsigned long CW32losApp::GetString(
  const tstring &key, 
  const tstring &defaultValue,
  const int     maxSize,
  tstring       &value,
  const tstring prefix)
{
  return mProfile.GetString(key, defaultValue, maxSize, value, prefix);
}

bool CW32losApp::WriteString(const tstring &key, const tstring &value, const tstring prefix)
{
  return mProfile.WriteString(key, value, prefix);
}


void CW32losApp::PromptImportFiles(std::vector<string>& resList)
{
  resList.clear();

  CString fileName;
  UINT nIDSTitle = IDS_INSTALL_TITLE;
  CString strFilterName;
  CString strFilterExt;

  strFilterName.LoadString(IDS_INSTALL_STR);
  strFilterExt.LoadString(IDS_INSTALL_EXT);

  if (DoPromptFileNames(
    fileName, nIDSTitle, OFN_FILEMUSTEXIST, /*bOpenFileDialog*/TRUE,
    strFilterName, strFilterExt, 
    // bAppendAllFiles: with "TRUE", missing AFX_IDS_ALLFILTER in ressource
    // results in VERIFY failing in DoPromptFileName()
    FALSE, 
    /*nMaxFiles*/ 20)) {
    const char *p = (LPCTSTR) fileName;

    CWaitCursor wait;
    // loop over all selected files (p is a structured string if multiple files selected)
    for (p += strlen(p) + 1; *p; p += strlen(p) + 1) {
      string path = string((LPCTSTR) fileName) + "\\" + p;
      resList.push_back(path);
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// CW32losApp-Nachrichtenbehandlungsroutinen


int CW32losApp::ExitInstance() 
{
  MSGDEBUG("CW32losApp::ExitInstance");
  if (m_hInstResDLL) {
    FreeLibrary(m_hInstResDLL);
  }

  if (mpProxySession) {
    // Write proxy data only if changed from default!
    // Otherwise, this may be the only entry in the INI file written!
    string useProxy(1, mpProxySession->GetUseProxy());
    if (useProxy != DEFAULT_USEPROXY) {
      mProfile.WriteString(PRF_USEPROXY, useProxy.c_str());
    }
    if (mpProxySession->GetProxy() != DEFAULT_PROXY) {
      mProfile.WriteString(PRF_PROXY, mpProxySession->GetProxy().c_str());
    }

    delete mpProxySession;
    mpProxySession = 0;
  }

  delete mpUserPassDlg;
  mpUserPassDlg = 0;

  delete mpFileManager;
  mpFileManager = 0;

  delete mpServerDataCache;
  mpServerDataCache = 0;

  delete mpAppPathManager;
  mpAppPathManager = 0;

  // 010 WindowPosNotSaved
  if (mpFrame) {
    mpFrame->WriteSettings();
  }
  return CWinApp::ExitInstance();
}

IFileManager &CW32losApp::FileManager()
{
  ASSERT(mpFileManager);
  return *mpFileManager;
}

string CW32losApp::GetResDLLName() const 
{
  if (mDlgLang == mDefaultLangNisoName) {
    return mDlgLang;
  }
  else {
    // For filename, see also [ResDLLName] in OOptionsDlg::OnInitDialog()
    return string("Logoskop_") + mDlgLang + ".dll"; 
  }
}


#define LIBNAME_SHELL32       "Shell32.dll"

// Exclude unicode version since PROCNAME_* are using just "*A" APIs
#ifdef _UNICODE
#error Ooooops!
#endif

#define PROCNAME_SHELLEXECUTE "ShellExecuteA"
typedef WINSHELLAPI HINSTANCE (APIENTRY *fShellExecute)         (HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);

fShellExecute LoadShellExecute()
{
  static bool init = false;
  static HINSTANCE hShell32 = 0;
  static fShellExecute pShellExecute = 0;


  // provide function pointer for displaying help file etc.
  if (!init) {
    hShell32 = LoadLibrary(LIBNAME_SHELL32);
    if (!hShell32) {
      MSGERROR("LoadShellExecute: could not load library " << LIBNAME_SHELL32);
      AfxMessageBox(IDS_LOADSHELL32_FAILED, MB_OK);
    }
    else {
      init = true;
      pShellExecute = (fShellExecute) GetProcAddress(hShell32, PROCNAME_SHELLEXECUTE);
      if (!pShellExecute) {
        DWORD err = GetLastError();
        MSGERROR("LoadShellExecute: could not get procedure " PROCNAME_SHELLEXECUTE " - error " << err);
        CString buf;
        buf.Format(IDS_GETSHELLEXECUTE_FAILED, err);
        AfxMessageBox(buf, MB_OK);
        FreeLibrary(hShell32); // no use...
      }
    }
  }
  return pShellExecute;
}

void CW32losApp::OpenLogfile()
{
  fShellExecute pShellExecute = LoadShellExecute();
  if (!pShellExecute) return; // no chance

  tstring logfile;
  mProfile.GetString(PRF_LOGFILE, mpAppPathManager->LogPath(), /*adlib*/_MAX_PATH, logfile);
  if (!logfile.empty()) {
    CString dir(mpAppPathManager->AppDataDir().c_str());
    HINSTANCE res = pShellExecute(m_pMainWnd->m_hWnd, "open", logfile.c_str(), 0, dir, SW_RESTORE/*SW_SHOW*/);
    if ((int) res > 32) return; // ok
  }
  // fall through if empty or if fail on non-empty
  CString buf;
  buf.Format(IDS_OPENFILE_FAILED, logfile.c_str());
  AfxMessageBox(buf, MB_OK);
}

void CW32losApp::OpenFile(UINT nID, const char *pFilename2)
{
  fShellExecute pShellExecute = LoadShellExecute();
  if (!pShellExecute) return; // no chance

  string tries[3];
  unsigned int end = 0;

  // prepare filenames to try
  // Grep for [LANGUAGE]
  string docLangBible(GetDoc().GetFirstLangBible());
  if (!docLangBible.empty()) {
    string::size_type pos = docLangBible.find("_"); 
    if (pos != string::npos) {
      docLangBible.erase(pos);
      // filename for docLangBible
      tries[end++] = docLangBible + pFilename2;
    }
  }
  {
    CString guiFilename;
    guiFilename.LoadString(nID);
    if (0 != stricmp(tries[0].c_str(), guiFilename)) {
      // filename from GUI (language ressource)
      tries[end++] = guiFilename;
    }
    if (0 != stricmp(docLangBible.c_str(), "en") && 0 != stricmp(guiFilename.Left(2), "en")) {
      // add english
      tries[end++] = string("EN") + pFilename2;
    }
  }

  // try all filenames prepared, leave as soon as one succeeded
  {
    CString dir(mpAppPathManager->ProgDir().c_str());
    for (unsigned int cur = 0; cur < end; ++cur) {
      HINSTANCE res = pShellExecute(m_pMainWnd->m_hWnd, "open", tries[cur].c_str(), 0, dir, SW_RESTORE/*SW_SHOW*/);
      if ((int) res > 32) return; // ok
    }
  }

  // all filenames failed, report them
  string filesTried;
  filesTried = tries[0];
  for (unsigned int cur = 1; cur < end; ++cur) {
    filesTried += string(" / ") + tries[cur];
  }  
  CString buf;
  buf.Format(IDS_OPENFILE_FAILED, filesTried.c_str());
  AfxMessageBox(buf, MB_OK);
}

void CW32losApp::OpenURL(const char *pURL)
{
  fShellExecute pShellExecute = LoadShellExecute();
  if (!pShellExecute) return; // no chance

  CString dir(mpAppPathManager->ProgDir().c_str());
  HINSTANCE res = pShellExecute(m_pMainWnd->m_hWnd, "open", pURL, 0, dir, SW_RESTORE/*SW_SHOW*/);
  if ((int) res > 32) return; // ok

  CString buf;
  buf.Format(IDS_OPENFILE_FAILED, pURL);
  AfxMessageBox(buf, MB_OK);
}


// === Menu actions ===========================================================

void CW32losApp::OnAppAbout()
{
  CAboutDlg aboutDlg(GetResDLLName(), mAppVersion);
  aboutDlg.DoModal();
}

void CW32losApp::AppAbout()
{
  OnAppAbout();
}

void CW32losApp::OnFileActualize() 
{
  MSGDEBUG(_T("CW32losApp::OnFileActualize..."));
  GetDoc().SetLocked(true); // *** must reset!
  std::vector<string> langBiblesToActualize;
  _CheckMustShowActualizeDialogFor(/*bInteractive=*/ true, langBiblesToActualize);
  if (langBiblesToActualize.empty()) {
    AfxMessageBox(IDS_NO_FILE_TO_ACTIVATE);
  }
  else {
    FileActualize(langBiblesToActualize);
  }
  GetDoc().SetLocked(false); // *** reset
  MSGDEBUG(_T("CW32losApp::OnFileActualize done."));
}


void CW32losApp::OnEditPostPair()
{
  tstring baseurl;
  mProfile.GetString(PRF_BASEPOSTPAIRURL, DEFAULT_BASEPOSTPAIRURL, _MAX_PATH, baseurl);

  GetDoc().SetLocked(true); // *** must reset! (dialog may access set of langBibles)

  OLosung* pLosung = GetDoc().GetFirstLosung();
  tstring langBible = GetDoc().GetFirstLangBible();
  IPostPairDlg& postPairDlg = NPostPairDlg::MakePostPairDlg(
    *mpProxySession, baseurl, pLosung, langBible);

  postPairDlg.DoModal();

  GetDoc().SetLocked(false);

  delete &postPairDlg;
}

void CW32losApp::OnFileOrganize() 
{
  tstring baseurl;
  mProfile.GetString(PRF_BASEURL, DEFAULT_BASEURL, _MAX_PATH, baseurl);

  GetDoc().SetLocked(true); // *** must reset! (dialog may access set of langBibles)

  // determine language of server info
  std::vector<tstring> infoLangFilterList;
  ComputeInfoLangFilterList(infoLangFilterList);

  IOrganizeDlg& organizeDlg = NOrganizeDlg::MakeOrganizeDlg(
    *this,
    mpAppPathManager->TwdDir(),
    *mpProxySession, *mpServerDataCache,
    FileManager(), 
    infoLangFilterList,
    baseurl,
    GetDoc().GetLangBibles(),
    GetDoc().GetDate().GetYear());
  // must delete organizeDlg! 

  organizeDlg.DoModal();
  
  const std::vector<string> viewLangBibles = organizeDlg.GetViewLangBibles();
  int newYear = organizeDlg.GetYearSelected();

  // no conditional processing here 
  // - even when ESC pressed, user may have deleted file 
  CWaitCursor wait;

  if (!viewLangBibles.empty()) {
    CTime date = GetDoc().GetDate();
    if (newYear > 0 && date.GetYear() != newYear) {
      // user selected text from another year - switch to the year,
      // try to display same day as in currently displayed year
      int d = date.GetDay();
      int m = date.GetMonth();
      if (!DateUtil::IsValidDay(newYear, m, d)) {
        --d; // this may happen for 29th of February
      }
      if (DateUtil::IsValidDay(newYear, m, d)) {
        date = CTime(newYear, m, d, 0, 0, 0); // h:m:s == 0:0:0 is ok because we use EqDate()
      }
    }
    // change langBibles/date
    GetDoc().SetLangBiblesDate(viewLangBibles, date);
  }
  else {
    // doc unchanged, but probably more files available
    mpFrame->FillLangbox();
  }

  // finally delete dialog (viewLangBibles has reference to its member!)
  delete &organizeDlg;

  GetDoc().SetLocked(false);
}

void CW32losApp::OnFileSignature() 
{
  ISignatureFormatter &formatter = NSignatureFormatter::MakeSignatureFormatter();
  ISignatureManager &signatureManager = 
    NSignatureManager::MakeSignatureManager(mProfile, mpAppPathManager->AppDataDir(), formatter);
  CSignatureDlg signatureDlg(signatureManager);
  GetDoc().SetLocked(true); // *** must reset! (dialog may access set of langBibles)
  signatureDlg.DoModal();
  GetDoc().SetLocked(false);
  delete &signatureManager;
  delete &formatter;
}

void CW32losApp::OnHelpProgram() 
{
  OpenFile(IDS_HELPFILE_PROGRAM, "readme.htm");  
}

void CW32losApp::OnHelpProject() 
{
  tstring baseurl;
  mProfile.GetString(PRF_HOMEPAGEURL, DEFAULT_HOMEPAGEURL, _MAX_PATH, baseurl);
  OpenURL(baseurl.c_str());    
}



void CW32losApp::OnEditOptions() 
{
  // 2009-08-01 HS: no more handle "UseUSP10" specific to langBible (how to control the setting?) See [UseUSP10]
  bool bUseUSP10 = mProfile.GetBool(PRF_USE_USP10, DEFAULT_USE_USP10);
  int oldLevel = mProfile.GetInt(PRF_LOGFILE_LEVEL, DEFAULT_LOGFILE_LEVEL);
  bool bWriteLogfile = (oldLevel != 0);

  bool startOnce = mProfile.GetBool(PRF_START_ONCE, DEFAULT_START_ONCE);
  OOptionsDlg options(mpAppPathManager->ProgDir(),
    mDlgLang,
    mDefaultLangNisoName,
    startOnce ? 1:0,
    bUseUSP10,
    bWriteLogfile);  

  if (IDOK == options.DoModal()) {
    mProfile.WriteBool(PRF_START_ONCE, options.m_StartOnce!=0);
    mProfile.WriteString(PRF_DIALOGLANG, (LPCSTR)options.mLangShortName.c_str());
    mProfile.WriteBool(PRF_USE_USP10, options.m_UseUSP10 != 0);

    // Levels are 0, 1, 2, 3
    // Dialog contains checkbox only.
    // How these are mapped:
    // - level = 0 => checkbox off 
    // - level > 0 => checkbox on
    // - checkbox off => level 0
    // - checkbox on  => 
    //   if initial level > 0: use initial level
    //   otherwise use DEFAULT_LOGFILE_LEVEL

    int newLevel = (options.m_WriteLogfile == 0) ? 0 : (oldLevel ? oldLevel : DEFAULT_LOGFILE_LEVEL);
    mProfile.WriteInt(PRF_LOGFILE_LEVEL, newLevel);
    _AdaptLogging(/*bIsStart=*/ !oldLevel && newLevel != 0);
  }
}


