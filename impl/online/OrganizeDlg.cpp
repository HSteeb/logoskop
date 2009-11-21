/* ----------------------------------------------------------------------
  Helmut Steeb
  Dialog for organizing The Word files
  $Id: OrganizeDlg.cpp 3853 2009-08-26 13:50:27Z helmut $

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
#include "../w32los/Resource.h"
#include <assert.h>
#include <afxinet.h> // AfxParseURL

#include "../util/ProfileDef.h"
#include "../filedesc/TwdFileDesc.h"
#include "../filedesc/TwdFileDescList.h"
#include "../filedesc/TwdInfo.h"
#include "../filedesc/TwdInfoList.h"
#include "../filedesc/TwdFileDescParser.h"
#include "OrganizeDlg.h"
#include "DownloadDlg.h"
#include "ProxyDlg.h"
#include "ProxySession.h"
#include "ServerDataCache.h"
#include "../util/FileManager.h"
#include "../filedesc/DirectoryFileDescReader.h"
#include "../filedesc/TwdFileDescListBoxHelper.h"
#include "IAppInterface.h"
#include "IErrorMsgFormatter.h"
#include "ErrorMsgFormatter.h"
#include "../util/FileUtil.h"
#include "../util/StringUtil.h"
#include "../util/Encoding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === COrganizeDlg declaration ===

class COrganizeDlg : public CDialog, public IOrganizeDlg
{
  typedef CDialog inherited;
public:
  COrganizeDlg(
    IAppInterface& appInterface,
    const tstring& driveDir, 
    IProxySession& proxySession, 
    IServerDataCache& serverDataCache,
    IFileManager& fileManager,
    std::vector<tstring>& infoLangFilterList,
    const tstring& listUrl, 
    const std::vector<string> &langBibles, 
    int            year      = -1, 
    CWnd*          pParent   = NULL);

  ~COrganizeDlg();

  // Dialogfelddaten
  //{{AFX_DATA(COrganizeDlg)
  enum { IDD = IDD_ORGANIZE };
  int mYearIndex;
  CString mDownloadProgress;
  //}}AFX_DATA

  int DoModal();

  const std::vector<tstring> &GetViewLangBibles() const;

  // GetYearSelected:
  // reads mYearIndex, returns -1 if no year selected, else year (e.g. 2001)
  int GetYearSelected() const; 

  // Überschreibungen
  // Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
  //{{AFX_VIRTUAL(COrganizeDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  //}}AFX_VIRTUAL

// Implementierung
protected:
  // Generierte Nachrichtenzuordnungsfunktionen
  //{{AFX_MSG(COrganizeDlg)
  afx_msg void OnSelchangeYearCombo();
  virtual BOOL OnInitDialog();
  afx_msg void OnDelete();
  afx_msg void OnDownload();
  afx_msg void OnView();
  afx_msg void OnUnpack();
  afx_msg void OnOrganizeProxy();
#ifdef THEWORDORGANIZER
  afx_msg void OnAppAbout();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
#else
  afx_msg void OnDblclkListInstalled();
#endif
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:
#ifdef THEWORDORGANIZER
  HICON             m_hIcon;
#endif
  // _SetYearSelected:
  // sets mYearIndex
  void _SetYearSelected(const int year);

  string _GetStringYear(const int year) const;
  
  // _GetFilenameSuffix:
  // returns the filename suffix after the language code and bible
  // e.g. "_2008.twd"
  string _GetFilenameSuffix(const int year) const; 

  void _FillInstalledList(const int year);

  void _SetButtonSensForInstalledList(const int buttonID);
  void _DisplayProgress(const string &s);


private:
  IAppInterface& mAppInterface;
  tstring mDriveDir;
  tstring mListUrl;
  const std::vector<string> &mLangBiblesFromCaller;
  int mYearFromCaller; // -1 if none
  IProxySession &mProxySession;
  IServerDataCache& mServerDataCache;
  IFileManager& mFileManager;
  std::vector<tstring>& mInfoLangFilterList;

  OTwdFileDescListBoxHelper *mpInstalledListHelper;

  OTwdFileDescList mInstalledList;

  string mServerName;

  std::vector<string> mViewLangBibles; // for caller to switch view to these langBibles
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.


// === COrganizeDlg implementation ===

#define FIRST_THEWORDFILE_YEAR 2008
#define PROGRESS_SEP ": "

COrganizeDlg::COrganizeDlg(
    IAppInterface& appInterface,
    const tstring& driveDir,
    IProxySession& proxySession, 
    IServerDataCache& serverDataCache,
    IFileManager& fileManager,
    std::vector<tstring>& infoLangFilterList,
    const tstring& listUrl, 
    const std::vector<string> &langBibles, 
    int year, 
    CWnd* pParent)
: CDialog(COrganizeDlg::IDD, pParent)
, mAppInterface(appInterface)
, mDriveDir(driveDir)
, mListUrl(listUrl)
, mLangBiblesFromCaller(langBibles)
, mYearFromCaller(year)
, mProxySession(proxySession)
, mServerDataCache(serverDataCache)
, mFileManager(fileManager)
, mInfoLangFilterList(infoLangFilterList)
, mpInstalledListHelper(0)
{
  //{{AFX_DATA_INIT(COrganizeDlg)
  mYearIndex = -1;
  mDownloadProgress = _T("");
  //}}AFX_DATA_INIT

  // compute mServerName from listUrl
  CString server;
  DWORD serviceType;
  CString strServerName;
  CString strObject;
  INTERNET_PORT nPort;
  if (!AfxParseURL(mListUrl.c_str(), serviceType, strServerName, strObject, nPort)) {
    mServerName = "???";
  }
  else if (strServerName.GetLength() == 0 && serviceType == AFX_INET_SERVICE_FILE) {
    mServerName = "localhost";
  }
  else {
    mServerName = (LPCTSTR) strServerName;
  }

#ifdef THEWORDORGANIZER
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
#endif
}

COrganizeDlg::~COrganizeDlg()
{
  mProxySession.CloseSession();

  delete mpInstalledListHelper;
  mpInstalledListHelper = 0;
}

void COrganizeDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COrganizeDlg)
  DDX_CBIndex(pDX, IDC_YEARCOMBO, mYearIndex);
  DDX_Text(pDX, IDC_DOWNLOAD_PROGRESS, mDownloadProgress);
  //}}AFX_DATA_MAP
  if (pDX->m_bSaveAndValidate) {
  }
  else {
    mpInstalledListHelper->FillListbox(mInstalledList);
  
    // button sensitivity
    _SetButtonSensForInstalledList(IDC_DELETE);
#ifndef THEWORDORGANIZER
    _SetButtonSensForInstalledList(IDC_ORGANIZE_VIEW);
#endif
  }
}

BEGIN_MESSAGE_MAP(COrganizeDlg, CDialog)
  //{{AFX_MSG_MAP(COrganizeDlg)
  ON_CBN_SELCHANGE(IDC_YEARCOMBO, OnSelchangeYearCombo)
  ON_BN_CLICKED(IDC_DELETE, OnDelete)
  ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
  ON_BN_CLICKED(IDC_UNPACK, OnUnpack)
  ON_BN_CLICKED(IDC_ORGANIZE_PROXY, OnOrganizeProxy)
#ifdef THEWORDORGANIZER
  ON_BN_CLICKED(ID_APP_ABOUT, OnAppAbout)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
#else
  ON_BN_CLICKED(IDC_ORGANIZE_VIEW, OnView)
  ON_LBN_DBLCLK(IDC_LISTINSTALLED, OnDblclkListInstalled)
#endif
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef THEWORDORGANIZER

void COrganizeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    mAppInterface.AppAbout();
  }
  else {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COrganizeDlg::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COrganizeDlg::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

#endif


void COrganizeDlg::_SetYearSelected(const int year)
{
  mYearIndex = year - FIRST_THEWORDFILE_YEAR;
}

string COrganizeDlg::_GetStringYear(const int year) const
{
  std::ostringstream os;
  os << std::setw(4) << std::setfill('0') << year;
  return os.str();
}

string COrganizeDlg::_GetFilenameSuffix(const int year) const
{
  std::ostringstream os;
  os << "_" << _GetStringYear(year) << ".twd";
  return os.str();
}

void COrganizeDlg::_FillInstalledList(const int year)
{
  // Cf. <--CW32losApp::DoInitialInstall()

  // Initially clear the list!
  // Otherwise, after deleting the last file, the list will still contain that file!
  mInstalledList.Clear();

  ODirectoryFileDescReader reader;
  std::vector<string> fileList;
  if (!reader.Read(mDriveDir, "", _GetFilenameSuffix(year), fileList)) {
    MSGDEBUG("COrganizeDlg::_FillInstalledList: no The Word file found");
  }
  else {
    std::vector<string>::const_iterator fileIter;
    for (fileIter = fileList.begin(); fileIter != fileList.end(); ++fileIter) {
      mInstalledList.Add(NTwdFileDesc::MakeTwdFileDesc(*fileIter, year));
    }
  }
}

void COrganizeDlg::_SetButtonSensForInstalledList(const int buttonID)
{
  CButton *pButton = (CButton *) GetDlgItem(buttonID);
  assert(pButton);
  const bool isEmpty = (0 == mInstalledList.Size());
  if (isEmpty && GetFocus() == pButton) {
    // if list becomes empty, button becomes insensitive
    // - let not the focused button become insensitive,
    //   using the keyboard, you would not be able 
    //   to move the focus to another button.
    //   So put it anywhere in advance - e.g. to the OK button.
    CButton *pOKButton = (CButton *) GetDlgItem(IDOK);
    if (pOKButton) {
      pOKButton->SetFocus();
    }
  }
  pButton->EnableWindow(!isEmpty);
}

void COrganizeDlg::_DisplayProgress(const string &s)
{
  CStatic *pDownloadProgress = (CStatic *) GetDlgItem(IDC_DOWNLOAD_PROGRESS);
  if (pDownloadProgress) {
    pDownloadProgress->SetWindowText(s.c_str());
  }
}

// === public ===

int COrganizeDlg::DoModal()
{
  return inherited::DoModal();
}

const std::vector<tstring> &COrganizeDlg::GetViewLangBibles() const
{
  return mViewLangBibles;
}

int COrganizeDlg::GetYearSelected() const
{
  return (mYearIndex < 0) ? -1 : (mYearIndex + FIRST_THEWORDFILE_YEAR);
}



/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten COrganizeDlg 

BOOL COrganizeDlg::OnInitDialog() 
{
  // mpInstalledListHelper cannot be set in constructor
  // (*this does not yet have a valid handle, which lets access for ID fail),
  // but must be set before OnInitDialog() (which calls DoDataExchange)
  mpInstalledListHelper = new OTwdFileDescListBoxHelper(*this, IDC_LISTINSTALLED);

  CDialog::OnInitDialog();

  // fill year combobox depending on current year
  int currentYear = CTime::GetCurrentTime().GetYear();

  CComboBox *pCombo = (CComboBox *) GetDlgItem(IDC_YEARCOMBO);
  assert(pCombo);
  pCombo->ResetContent();
  for (int y = FIRST_THEWORDFILE_YEAR; y <= currentYear + 1; ++y) {
    std::ostringstream os;
    os << y;
    pCombo->AddString(os.str().c_str());
  }

  _SetYearSelected(mYearFromCaller > 0 ? mYearFromCaller : currentYear);

  _FillInstalledList(GetYearSelected());
  UpdateData(FALSE);

  CListBox *pList = (CListBox *) GetDlgItem(IDC_LISTINSTALLED);
  assert(pList);
  pList->SetFocus(); // set focus to list - allow immediate selection

  // set selection to language/bible currently displayed
  {
    OTwdFileDescList sel;
    std::vector<string>::const_iterator it;
    for (it = mLangBiblesFromCaller.begin(); it != mLangBiblesFromCaller.end(); ++it) {
      const string &langBible = *it;
      sel.Add(NTwdFileDesc::MakeTwdFileDesc(langBible, currentYear));
    }
    mpInstalledListHelper->SetSelection(sel);
  }

#ifdef THEWORDORGANIZER
  {
    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
      CString strAboutMenu;
      strAboutMenu.LoadString(ID_APP_ABOUT);
      if (!strAboutMenu.IsEmpty())
      {
        pSysMenu->AppendMenu(MF_SEPARATOR);
        pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
      }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);      // Set big icon
    SetIcon(m_hIcon, FALSE);    // Set small icon
  }  
#endif

  return FALSE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void COrganizeDlg::OnSelchangeYearCombo() 
{
  UpdateData(TRUE);   // updates selected year
  _FillInstalledList(GetYearSelected());
  UpdateData(FALSE);  // refresh file list
}


void COrganizeDlg::OnDelete() 
{
  OTwdFileDescList sel;
  mpInstalledListHelper->GetSelection(sel);
  if (0 != sel.Size()) {
    if (IDOK == AfxMessageBox(IDS_REALLYDELETE, MB_OKCANCEL)) {
      TFileDescList::const_iterator it;
      for (it = sel.Begin(); it != sel.End(); ++it) {
        string filename = mDriveDir + (*it)->GetKey() + ".twd";
        if (unlink(filename.c_str())) {
          CString s;
          switch (errno) {
          case EACCES:
            MSGERROR("File " << filename << " is read-only");
            s.Format(IDS_DELETE_EACCES, filename.c_str()); 
            AfxMessageBox(s, MB_OK);
            break;
          case ENOENT:
            MSGERROR("File " << filename << " not found");
            // Want to delete the file, and it's not there?
            // Good! Don't bother user with error message...
            break;
          default:
            ; // nop
          }
        }        
      }
      _FillInstalledList(GetYearSelected()); // read files for selected year
      UpdateData(FALSE);  // refresh file list
    }
  }
}

void COrganizeDlg::OnDownload() 
{
  CWaitCursor wait;

  CString getting;
  getting.LoadString(IDS_GETTING_FILELIST);
  _DisplayProgress(LPCTSTR(getting));

  bool bFillOk = mServerDataCache.Fill();
  _DisplayProgress("");

  OTwdFileDescList& serverFileList = mServerDataCache.GetFileDescList();
  OTwdInfoList&     serverInfoList = mServerDataCache.GetInfoList();

  // compute list of server files for currently selected year
  OTwdFileDescList yearFileList;
  TFileDescList::const_iterator it;
  for (it = serverFileList.Begin(); it != serverFileList.End(); ++it) {
    ITwdFileDesc *pTwdFileDesc = *it;
    // filter for year
    if (pTwdFileDesc->GetYear() == GetYearSelected()) {
      yearFileList.Add(pTwdFileDesc->Clone());
    }
  }

  // Same in -->CW32losApp::OnFileActualize() !!
  // Show info, but only once 
  ITwdInfo* pInfo = serverInfoList.FindFirst(mInfoLangFilterList);
  if (pInfo && !pInfo->GetInfo().empty()) {
    tstring lastInfo;
    // TODO 2009-08-01 HS: why was this stored individually per langBibleFromCaller?
    tstring key(tstring(PRF_LAST_LANG_INFO));
    if (!mAppInterface.GetString(key, "", 64, lastInfo)
        || lastInfo < pInfo->GetUpdated()) {
      mAppInterface.WriteString(key, (LPCSTR) pInfo->GetUpdated().c_str());
      string info;
      if (NEncoding::Narrow(pInfo->GetInfo(), info)) {
        AfxMessageBox(info.c_str());
      }
    }
  }

  // show download dialog
  OTwdFileDescList resultFileList;
  IDownloadDlg& downloadDlg = NDownloadDlg::MakeDownloadDlg(mServerName, yearFileList, GetYearSelected(), mInstalledList, resultFileList);

  bool ok = (IDOK == downloadDlg.DoModal() && 0 != resultFileList.Size());
  delete &downloadDlg;

  if (ok) {

    // process files selected in download dialog
    CWaitCursor wait;
    int nErrFiles = 0;
    int nOkFiles = 0;
    
    // prepare status strings
    CString gettingFile;
    gettingFile.LoadString(IDS_GETTING_FILE);
    CString installingFile;
    installingFile.LoadString(IDS_INSTALLING_FILE);
   
    // loop over files selected in download dialog
    TFileDescList::const_iterator it;
    for (it = resultFileList.Begin(); it != resultFileList.End(); ++it) {
      ITwdFileDesc* pTwdFileDesc = *it;
      assert(pTwdFileDesc);

      // show status
      string displayName = pTwdFileDesc->GetDisplayName();
      _DisplayProgress(displayName + PROGRESS_SEP + LPCSTR(gettingFile));

      string url = pTwdFileDesc->GetUrl();

      // download one file and store it
      string data;
      int res = mProxySession.Get(url, data);

      if (0 != res) {
        IErrorMsgFormatter& fmt = NErrorMsgFormatter::MakeErrorMsgFormatter(url);
        CString buf;
        fmt.FormatDownloadError(IDS_GETFILE_FAILED, res, buf);
        delete &fmt;
        AfxMessageBox(buf);
        ++nErrFiles;
      }
      else {
        _DisplayProgress(displayName + PROGRESS_SEP + LPCSTR(installingFile));
        if (!mFileManager.Install(pTwdFileDesc->GetYear(), pTwdFileDesc->GetLangBible(), data)) {
          AfxMessageBox(IDS_WRITEERR);
          ++nErrFiles;
        }
        else {
          ++nOkFiles;
        }
      }
    }
    _DisplayProgress("");
    MSGDEBUG("OrganizeDlg: " << nOkFiles << " ok, " << nErrFiles << " failed.");

    // update listbox
    _FillInstalledList(GetYearSelected()); // read files for selected year
    UpdateData(FALSE);  // refresh file list
    mpInstalledListHelper->SetSelection(resultFileList);
  }
}

void COrganizeDlg::OnUnpack() 
{
  std::vector<string> resList;
  mAppInterface.PromptImportFiles(resList);// show dialog with multi-selection, return path list

  // process all paths
  // cf. <--CW32losApp::DoInitialInstall()
  std::vector<string>::const_iterator it;
  OTwdFileDescList imported;
  for (it = resList.begin(); it != resList.end(); ++it) {
    // read file
    string data = FileUtil::Read(*it);
    if (data.length()) {
      ITwdFileDesc& desc = NTwdFileDesc::MakeTwdFileDesc(*it);

      // install one path
      bool ok = mFileManager.Install(desc.GetYear(), desc.GetLangBible(), data);
      if (!ok) {
        AfxMessageBox(IDS_WRITEERR);
        delete &desc;
      }
      else {
        imported.Add(desc);
      }
    }
  }

  // if at least one path has been installed, update list, show info...
  // Note: the path may be relevant for a different year than the currently selected one!

  if (0 != imported.Size()) {
    _FillInstalledList(GetYearSelected()); // read files for selected year
    UpdateData(FALSE);  // refresh file list

    CListBox *pList = (CListBox *) GetDlgItem(IDC_LISTINSTALLED);
    assert(pList);
    pList->SetFocus(); // set focus to list - allow immediate selection

    mpInstalledListHelper->SetSelection(imported, /*doMatchYear*/ true);

    CString str;
    str.Format(IDS_INSTALLED, imported.Size());
    AfxMessageBox(str);
  }
}

void COrganizeDlg::OnOrganizeProxy() 
{
  CProxyDlg proxyDlg(mProxySession.GetUseProxy(), mProxySession.GetProxy());
  if (IDOK == proxyDlg.DoModal()) {
    mProxySession.SetProxy(proxyDlg.UseProxy(), proxyDlg.Proxy());
  }
}

void COrganizeDlg::OnView() 
{
  OTwdFileDescList sel;
  mpInstalledListHelper->GetSelection(sel);
  mViewLangBibles.clear();
  TFileDescList::const_iterator it;
  for (it = sel.Begin(); it != sel.End(); ++it) {
    mViewLangBibles.push_back((*it)->GetLangBible());
  }
  EndDialog(IDOK);
}

#ifdef THEWORDORGANIZER

void COrganizeDlg::OnAppAbout() 
{
  mAppInterface.AppAbout();
}

#else

void COrganizeDlg::OnDblclkListInstalled() 
{
  OnView();
}

#endif

// === NOrganizeDlg ===

IOrganizeDlg &NOrganizeDlg::MakeOrganizeDlg(
    IAppInterface& appInterface,
    const tstring& driveDir, 
    IProxySession& proxySession, 
    IServerDataCache& serverDataCache,
    IFileManager& fileManager,
    std::vector<tstring>& infoLangFilterList,
    const tstring& listUrl, 
    const std::vector<string> &langBibles, 
    int year, 
    CWnd* pParent)
{
  IOrganizeDlg *pOrganizeDlg = new COrganizeDlg(
    appInterface,
    driveDir, 
    proxySession, 
    serverDataCache, 
    fileManager,
    infoLangFilterList,
    listUrl, 
    langBibles, 
    year, 
    pParent);
  assert(pOrganizeDlg);
  return *pOrganizeDlg;
}
