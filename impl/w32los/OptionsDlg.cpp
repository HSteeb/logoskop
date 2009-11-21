/* ----------------------------------------------------------------------
  Helmut Steeb
  Module for options dialog.
  $Id: OptionsDlg.cpp 3890 2009-08-28 14:32:40Z helmut $

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
#include "OptionsDlg.h"
#include "LangNames.h"
#include "../filedesc/DirectoryFileDescReader.h"
#include <algorithm> // find
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



OOptionsDlg::OOptionsDlg(
  const string &driveDir, 
  const string &dialogLang, 
  const string& defaultLangNisoName,
  const bool startOnce,
  const bool bUseUSP10,
  const bool bWriteLogfile,
  CWnd* pParent /*= NULL*/)
: CDialog(OOptionsDlg::IDD, pParent)
, mDriveDir(driveDir)
, mDefaultLangNisoName(defaultLangNisoName)
{
  //{{AFX_DATA_INIT(OOptionsDlg)
  m_DialogLangList = _T("");
  m_StartOnce = FALSE;
  m_UseUSP10 = TRUE;
  m_WriteLogfile = FALSE;
  //}}AFX_DATA_INIT
  mLangShortName = dialogLang;
  m_DialogLangList = NLangNames::MakeNisoDisplayName(mLangShortName).c_str();
  m_StartOnce = startOnce;
  m_UseUSP10 = bUseUSP10;
  m_WriteLogfile = bWriteLogfile;

}


void OOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(OOptionsDlg)
  DDX_LBString(pDX, IDC_DLGLANGLIST, m_DialogLangList);
  DDX_Check(pDX, IDC_START_ONCE, m_StartOnce);
  DDX_Check(pDX, IDC_USEUSP10, m_UseUSP10);
  DDX_Check(pDX, IDC_WRITELOGFILE, m_WriteLogfile);
  //}}AFX_DATA_MAP
  if (pDX->m_bSaveAndValidate) {
    mLangShortName = NLangNames::ExtractNisoFromNisoDisplayName(string(m_DialogLangList));
  }
}


BEGIN_MESSAGE_MAP(OOptionsDlg, CDialog)
  //{{AFX_MSG_MAP(OOptionsDlg)
  ON_LBN_DBLCLK(IDC_DLGLANGLIST, OnDblclkDlglanglist)
  ON_BN_CLICKED(IDC_VIEWLOGFILE, OnOpenLogfile)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten OOptionsDlg 


BOOL OOptionsDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  CListBox *pList = (CListBox*) GetDlgItem(IDC_DLGLANGLIST);
  if (!pList) {
    MSGERROR("OOptionsDlg::OnInitDialog: listbox not found");
    return FALSE;
  }
  pList->SendMessage(LB_RESETCONTENT, 0, 0); 

  ODirectoryFileDescReader reader;
  std::vector<string> fileList;
  // For filename, see also [ResDLLName] in CW32losApp::GetResDLLName()
  if (!reader.Read(mDriveDir, "Logoskop_", ".dll", fileList)) {
    MSGDEBUG("OOptionsDlg::OnInitDialog: no resource file found");
  }
  // if default language ("eng", typically) not yet contained, add it (is builtin in exe)
  if (fileList.end() == std::find(fileList.begin(), fileList.end(), mDefaultLangNisoName)) {
    fileList.push_back(mDefaultLangNisoName);
  }
  std::vector<string>::const_iterator it;
  for (it = fileList.begin(); it != fileList.end(); ++it) {
    string displayName = NLangNames::MakeNisoDisplayName(*it);
    pList->SendMessage(LB_ADDSTRING, 0, (LPARAM) displayName.c_str());
  }
  pList->SelectString(-1, m_DialogLangList);
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void OOptionsDlg::OnDblclkDlglanglist() 
{
  UpdateData(TRUE);
  mLangShortName = NLangNames::ExtractNisoFromNisoDisplayName(string(m_DialogLangList));
  EndDialog(IDOK);
}

void OOptionsDlg::OnOpenLogfile() 
{
  CW32losApp *pApp = static_cast<CW32losApp*>(AfxGetApp());
  assert(pApp);
  pApp->OpenLogfile();
}

