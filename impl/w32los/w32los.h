/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop application
  $Id: w32los.h 3905 2009-09-12 11:30:16Z helmut $

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

#if !defined(AFX_W32LOS_H__INCLUDED_)
#define AFX_W32LOS_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

// (this include effects many w32los cpp files)
#include "resource.h"
#include "w32losDoc.h"
#include "../util/Profile.h"
#include "../util/VersionInfo.h"
#include "../online/IAppInterface.h"

class CMainFrame;
class IAppPathManager;
class IUserPassDlg;
class IProxySession;
class IServerDataCache;
class IFileManager;
class OTwdFileDescList;

class CW32losApp : public CWinApp, public IAppInterface
{
public:
  CW32losApp();
  HINSTANCE m_hInstResDLL;

  CW32losDoc &GetDoc();
  CProfile &Profile();


  // === IAppInterface ===

  // PromptImportFiles:
  // displays open file dialog, allows multiselect,
  // returns list of selected file paths.
  void AppAbout();
  void PromptImportFiles(std::vector<string>& resList);
  unsigned long GetString  (const tstring &key, const tstring &defaultValue, const int maxSize, tstring &value, const tstring prefix = tstring(_T("")));
  bool          WriteString(const tstring &key, const tstring &value, const tstring prefix = tstring(_T("")));

private:

  IAppPathManager   *mpAppPathManager;
  CProfile          mProfile;
  CVersionInfo      mAppVersion;

  string mDlgLang; // NISO name
  string mDocLang;
  CW32losDoc *mpDoc;

  bool mbIsInitialized;

  IUserPassDlg *mpUserPassDlg;
  IProxySession *mpProxySession;
  IServerDataCache* mpServerDataCache;
  IFileManager *mpFileManager;

  const string mDefaultLangNisoName;

protected:
  CMainFrame *mpFrame;

  IFileManager &FileManager();


  void ComputeInfoLangFilterList(std::vector<tstring>& infoLangFilterList);

  // helper methods called in InitInstance()
  BOOL _ProcessCommandLine(/*out:*/ bool bAutostart, tstring &iniPath);
  void _SetIniFileNames(const tstring &iniPath);
  void _AdaptLogging(bool bIsStart = true);
  BOOL _ReadAppVersion();
  BOOL _MustLeaveForStartOnce();
  void _DetermineDlgLang();
  bool _SetDlgLang(const string &dlgLang, bool bReportError);
  void _MakeProxySession();
  void _DoInitialInstall();
  void _RestoreWindowPlacement();
  void _CheckMustShowActualizeDialogFor(bool bInteractive, /*out*/std::vector<string> &langBiblesToActualize);
  // _SetLangBiblesForCurOrNextYearWithMessage:
  // based on current date and langBibles specified in mProfile,
  // checks existence of files for current or next year,
  // sets these in document,
  // eventually displays hint that next year has been read.
  void _SetLangBiblesForCurOrNextYearWithMessage();



  const tstring Today() const;
  const tstring Now() const;


public:
  string GetResDLLName() const;


protected:

  // CheckMustShowActualizeDialogFor:
  // returns true if OnActualizeDialog() should be called,
  //   based on date and mProfile.
  // Expects that OnActualizeDialog() will be called afterwards,
  //   hence writes to mProfile to record this.

  void FileActualize(const std::vector<string> &langBiblesToActualize);


public:
  // DoPromptFileNames:
  // if nMaxFiles > 1, adds OFN_ALLOWMULTISELECT to lFlags,
  // on return, fileName contains the directory, followed by the
  // files selected (0 separated), terminated by two 0's.
  // If nMaxFiles == 1, fileName contains one complete path.

  BOOL DoPromptFileNames(
    CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog,
    CString& strFilterName, CString& strFilterExt, BOOL bAppendAllFiles,
    unsigned int nMaxFiles);

  // reads filename from resource entry for nID and opens that file
  // If that fails, tries pFilename2. If that fails, error message.
  void OpenFile(UINT nID, const char *pFilename2);

  void OpenURL(const char *pURL);

  // Open logfile in application associated in Windows.
  void OpenLogfile();


// Überladungen
  // Vom Klassenassistenten generierte Überladungen virtueller Funktionen
  //{{AFX_VIRTUAL(CW32losApp)
  public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  //}}AFX_VIRTUAL

// Implementierung
  //{{AFX_MSG(CW32losApp)
  afx_msg void OnFileActualize();
  afx_msg void OnFileOrganize();
  afx_msg void OnFileSignature();

  afx_msg void OnEditPostPair();
  afx_msg void OnEditOptions();

  afx_msg void OnHelpProgram();
  afx_msg void OnHelpProject();
  afx_msg void OnAppAbout();

  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_W32LOS_H__INCLUDED_)
