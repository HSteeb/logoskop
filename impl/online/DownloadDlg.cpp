/* ----------------------------------------------------------------------
  Helmut Steeb
  Module for "download language packs" dialog.
  $Id: DownloadDlg.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include <afxinet.h>
#include <assert.h>
#include <algorithm>
#include "../w32los/Resource.h"
#include "../filedesc/TwdFileDesc.h"
#include "../filedesc/TwdFileDescList.h"
#include "../filedesc/TwdFileDescListBoxHelper.h"
#include "DownloadDlg.h"
#include "../util/Encoding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === CDownloadDlg declaration ===


class CDownloadDlg : public CDialog, public IDownloadDlg
{
  typedef CDialog inherited;
public:
  CDownloadDlg(
    const string& serverName,
    OTwdFileDescList& serverFileList,
    const int year,
    OTwdFileDescList& installedFileList,
    OTwdFileDescList& resultFileList,
    CWnd* pParent = NULL);

  // Dialogfelddaten
  //{{AFX_DATA(CDownloadDlg)
  enum { IDD = IDD_DOWNLOAD };
  //}}AFX_DATA

  virtual int DoModal();

  // Überschreibungen
  // Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
  //{{AFX_VIRTUAL(CDownloadDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  //}}AFX_VIRTUAL


// Implementierung
protected:
  // Generierte Nachrichtenzuordnungsfunktionen
  //{{AFX_MSG(CDownloadDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnSelectAll();
  afx_msg void OnSelectMissing();
  afx_msg void OnDblclkDownloadList();
  afx_msg void OnSelchange();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  void SetButtonSens();
  void UpdateInfo();
  tstring _Narrow(const wstring &s);

private:
  int mYearFromCaller;
  OTwdFileDescList& mInstalledFileList;
  OTwdFileDescList& mServerFileList;
  const std::string& mServerName;
  OTwdFileDescList& mResultFileList; // passed back if dialog not cancelled
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.



// === CDownloadDlg implementation ===

CDownloadDlg::CDownloadDlg(
  const string& serverName,
  OTwdFileDescList& serverFileList,
  const int year,
  OTwdFileDescList& installedFileList,
  OTwdFileDescList& resultFileList,
  CWnd* pParent /*=NULL*/
)
: CDialog(CDownloadDlg::IDD, pParent)
, mServerName(serverName)
, mServerFileList(serverFileList)
, mYearFromCaller(year)
, mInstalledFileList(installedFileList)
, mResultFileList(resultFileList)
{
  //{{AFX_DATA_INIT(CDownloadDlg)
  //}}AFX_DATA_INIT
  mResultFileList.Clear(); // be defensive
}


void CDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDownloadDlg)
  //}}AFX_DATA_MAP
#ifdef _DEBUG
  if (!pDX->m_bSaveAndValidate) {
    CListBox *pList = (CListBox *) GetDlgItem(IDC_DOWNLOAD);
    assert(pList);
    int count = pList->GetSelCount();
  }
#endif
}


BEGIN_MESSAGE_MAP(CDownloadDlg, CDialog)
  //{{AFX_MSG_MAP(CDownloadDlg)
  ON_BN_CLICKED(IDC_DOWNLOAD_SELECT_ALL, OnSelectAll)
  ON_BN_CLICKED(IDC_DOWNLOAD_SELECT_MISSING, OnSelectMissing)
  ON_LBN_DBLCLK(IDC_DOWNLOAD, OnDblclkDownloadList)
  ON_LBN_SELCHANGE(IDC_DOWNLOAD, OnSelchange)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CDownloadDlg 

BOOL CDownloadDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  // set static text for year
  {
    CString staticText;
    staticText.Format(IDS_DOWNLOAD_TEXTS_YEAR, mServerName.c_str(), mYearFromCaller);
    CStatic *pStatic = (CStatic*) GetDlgItem(IDC_STATIC_TEXT_YEAR);
    assert(pStatic);
    pStatic->SetWindowText(staticText);
  }

  // fill listbox of downloadable files
  CListBox *pListBox = (CListBox *) GetDlgItem(IDC_DOWNLOAD);
  assert(pListBox);
  pListBox->SetFocus(); // set focus to list - allow immediate selection

  OTwdFileDescListBoxHelper helper(*pListBox);
  helper.FillListbox(mServerFileList);

  UpdateData(FALSE);
  SetButtonSens();
  UpdateInfo();

  return FALSE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CDownloadDlg::OnOK() 
{
  CWaitCursor wait;

  OTwdFileDescListBoxHelper helper(*this, IDC_DOWNLOAD);
  helper.GetSelection(mResultFileList);
  if (0 != mResultFileList.Size()) {
    CDialog::OnOK();
  }
}

void CDownloadDlg::SetButtonSens() 
{
  CListBox *pList = (CListBox *) GetDlgItem(IDC_DOWNLOAD);
  assert(pList);
  int count = pList->GetSelCount();

  CButton *pButton = (CButton *) GetDlgItem(IDOK);
  assert(pButton);
  pButton->EnableWindow(count > 0);
  if (!count) {
    CButton *pCancelButton = (CButton *) GetDlgItem(IDCANCEL);
    assert(pCancelButton);
    pCancelButton->SetFocus();
  }
}

void CDownloadDlg::UpdateInfo() 
{
  CEdit *pEdit = (CEdit*) GetDlgItem(IDC_INFO);
  assert(pEdit);

  OTwdFileDescListBoxHelper helper(*this, IDC_DOWNLOAD);
  helper.GetSelection(mResultFileList);
  TFileDescList::iterator it = mResultFileList.Begin();
  if (it != mResultFileList.End()) {
    ITwdFileDesc *pTwdFileDesc = *it;
    string allInfo(pTwdFileDesc->GetUpdated());
    if (!pTwdFileDesc->GetInfo().empty()) {
      string info;
      if (NEncoding::Narrow(pTwdFileDesc->GetInfo(), info)) {
        allInfo += "\r\n" + info;
      }
    }
    pEdit->SetWindowText(allInfo.c_str());
  }
}

void CDownloadDlg::OnDblclkDownloadList() 
{
  OnOK();
}

void CDownloadDlg::OnSelectAll() 
{
  CListBox *pList = (CListBox *) GetDlgItem(IDC_DOWNLOAD);
  assert(pList);
  int count = pList->GetCount();
  if (count > 0) {
    if (count == 1) {
      // CListBox::SelItemRange 
      // If you need to select only one item in a multiple-selection list box
      // - that is, if nFirstIem is equal to nLastItem -
      // call the SetSel member function instead.
      // !"§$%&!!
      pList->SetSel(0);
    }
    else {
      pList->SelItemRange(TRUE, 0, count - 1);
    }
  }
  SetButtonSens();
  UpdateInfo();
}


void CDownloadDlg::OnSelectMissing() 
{
  OTwdFileDescList notInstalledFileList;

  // for each file on server (for year selected)...
  for (TFileDescList::const_iterator serverIt = mServerFileList.Begin(); serverIt != mServerFileList.End(); ++serverIt) {
    const string key = (*serverIt)->GetKey();
 
    TFileDescList::const_iterator installedIt;
    for (installedIt = mInstalledFileList.Begin(); 
      installedIt != mInstalledFileList.End() && key != (*installedIt)->GetKey(); 
      ++installedIt) {
      ;
    }
    // ... if file is not in mInstalledFileList...
    if (installedIt == mInstalledFileList.End()) {
      // ... add file to notInstalledFileList
      notInstalledFileList.Add((*serverIt)->Clone());
    }
  }
  OTwdFileDescListBoxHelper helper(*this, IDC_DOWNLOAD);
  helper.SetSelection(notInstalledFileList);
  SetButtonSens();
  UpdateInfo();
}

void CDownloadDlg::OnSelchange() 
{
  SetButtonSens();
  UpdateInfo();
}

// === public ===

int CDownloadDlg::DoModal()
{
  return inherited::DoModal();
}


// === NDownloadDlg ===

IDownloadDlg& NDownloadDlg::MakeDownloadDlg(
    const string& serverName,
    OTwdFileDescList& serverFileList,
    const int year,
    OTwdFileDescList& installedFileList,
    OTwdFileDescList& resultFileList,
    CWnd* pParent)
{
  IDownloadDlg *pDownloadDlg = new CDownloadDlg(
    serverName,
    serverFileList,
    year,
    installedFileList,
    resultFileList,
    pParent);
  assert(pDownloadDlg);
  return *pDownloadDlg;
}

