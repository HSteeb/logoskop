/* ----------------------------------------------------------------------
 Helmut Steeb
 Module for "actualize" dialog.
  $Id: LangPackDlg.cpp 3853 2009-08-26 13:50:27Z helmut $

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
#include "../w32los/Resource.h"
#include "LangPackDlg.h"
#include "ProxyDlg.h"
#include "../util/Profile.h"
#include "../util/ProfileDef.h"
#include "../util/FileUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CLangPackDlg 


CLangPackDlg::CLangPackDlg(
  const tstring &langBibleString,
  int defaultYear,
  const tUseProxy useProxy, const string &proxy, CProfile &profile, 
  CWnd* pParent /*=NULL*/)
: CDialog(CLangPackDlg::IDD, pParent)
, mDefaultYear(defaultYear)
, mUseProxy(useProxy)
, mProxy(proxy)
, mProfile(profile)
{
  //{{AFX_DATA_INIT(CLangPackDlg)
  mYearChecked = -1;
  mCurLangBible = _T("");
  //}}AFX_DATA_INIT
  mCurLangBible = FileUtil::Moronize(FileUtil::DeMoronize(langBibleString)).c_str();
}


void CLangPackDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CLangPackDlg)
  DDX_Radio(pDX, IDC_YEAR1, mYearChecked);
  DDX_Text(pDX, IDC_CURRDOCLANG, mCurLangBible);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLangPackDlg, CDialog)
  //{{AFX_MSG_MAP(CLangPackDlg)
  ON_BN_CLICKED(IDC_LANGPACK_PROXY, OnLangpackProxy)
  ON_BN_CLICKED(IDC_AUTOLANGPACK, OnAutoLangpack)  
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CLangPackDlg 

BOOL CLangPackDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  CTime today = CTime::GetCurrentTime();
  const int currentMonth = today.GetMonth();
  mFirstYear  = today.GetYear();
  mSecondYear = mFirstYear + 1;
  
  {
    // create radio button for first year
    CButton *pButton = (CButton*) GetDlgItem(IDC_YEAR1);
    assert(pButton);
    std::ostringstream os;
    os << mFirstYear;
    pButton->SetWindowText(os.str().c_str());
//    pButton->SetCheck(1); // 1==checked
  }
  {
    // create radio button for second year
    CButton *pSecondYearButton = (CButton*) GetDlgItem(IDC_YEAR2);
    assert(pSecondYearButton);
    std::ostringstream os;
    os << mSecondYear;
    pSecondYearButton->SetWindowText(os.str().c_str());

    // Select one of the year radio buttons:
    if (mFirstYear == mDefaultYear) {
      // disable second year
      mYearChecked = 0;
      pSecondYearButton->EnableWindow(FALSE);
    }
    else {
      mYearChecked = 1;
    }
  }

  {
    // format year numbers into static hint string
    CStatic *pYearHint = (CStatic *) GetDlgItem(IDC_YEAR_HINT);
    assert(pYearHint);
    CString yearHintOld, yearHintNew;
    pYearHint->GetWindowText(yearHintOld);
    yearHintNew.Format(yearHintOld, mSecondYear, mFirstYear);
    pYearHint->SetWindowText(yearHintNew);
  }

  CButton *pAutoLangpack = (CButton *) GetDlgItem(IDC_AUTOLANGPACK);
  // HS 2005-11-05: why use assert, just use condition...
  if (pAutoLangpack) {
    // format year numbers into checkbox string
    CString autoLangpackOld, autoLangpackNew;
    pAutoLangpack->GetWindowText(autoLangpackOld);
    autoLangpackNew.Format(autoLangpackOld, mDefaultYear);
    pAutoLangpack->SetWindowText(autoLangpackNew);

    bool bAutoLangpackOn = mProfile.GetBool(PRF_AUTOLANGPACKDLGON, DEFAULT_AUTOLANGPACKDLGON);
    pAutoLangpack->SetCheck(bAutoLangpackOn);
  }

  UpdateData(FALSE);
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

int CLangPackDlg::GetSelectedYear() const
{
  return (mYearChecked == 0) ? mFirstYear : mSecondYear;
}

const tUseProxy CLangPackDlg::UseProxy() const
{
  return mUseProxy;
}

const string &CLangPackDlg::Proxy() const
{
  return mProxy;
}

void CLangPackDlg::OnLangpackProxy() 
{
  CProxyDlg proxyDlg(mUseProxy, mProxy);
  if (IDOK == proxyDlg.DoModal()) {
    mUseProxy = proxyDlg.UseProxy();
    mProxy = proxyDlg.Proxy();
  }
}

void CLangPackDlg::OnAutoLangpack() 
{
  CButton *pAutoLangpack = (CButton *) GetDlgItem(IDC_AUTOLANGPACK);
  if (pAutoLangpack) {
    bool bAutoLangpackOn = (0 != pAutoLangpack->GetCheck());
    mProfile.WriteBool(PRF_AUTOLANGPACKDLGON, bAutoLangpackOn);
  }
}
