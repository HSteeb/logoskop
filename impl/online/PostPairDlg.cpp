/* ----------------------------------------------------------------------
  Helmut Steeb
  Dialog for posting bible word pairs to the server
  $Id: PostPairDlg.cpp 3014 2008-11-29 10:19:16Z helmut $

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

#include "ProxyDlg.h"
#include "ProxySession.h"
#include "IErrorMsgFormatter.h"
#include "ErrorMsgFormatter.h"
#include "../util/StringUtil.h"
#include "../util/Encoding.h"
#include "../LosFile/Losung.h"
#include "PostPairDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === CPostPairDlg declaration ===

class CPostPairDlg : public CDialog, public IPostPairDlg
{
  typedef CDialog inherited;
public:
  CPostPairDlg(
    IProxySession& proxySession,
    const string& postUrl, 
    OLosung* pLosung,
    string langBible = "", 
    CWnd* pParent = NULL);
  ~CPostPairDlg();

  // Dialogfelddaten
  //{{AFX_DATA(CPostPairDlg)
  enum { IDD = IDD_POST_PAIR };
  CString mParol1;
  CString mParolContext1;
  CString mParol2;
  CString mParolContext2;
  CString mBibleEdition;
  int mOrderIndex;
  int mTypeIndex;
  CString mTypeOther;
  BOOL mThirdPartyFree;
  //}}AFX_DATA

  virtual int DoModal();

  // Überschreibungen
  // Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
  //{{AFX_VIRTUAL(CPostPairDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  //}}AFX_VIRTUAL

// Implementierung
protected:
  // Generierte Nachrichtenzuordnungsfunktionen
  //{{AFX_MSG(CPostPairDlg)
  afx_msg void OnClose();
  afx_msg void OnInsertParol1();
  afx_msg void OnInsertParol2();
  virtual BOOL OnInitDialog();
  afx_msg void OnSwap();
  afx_msg void OnDelete();
  afx_msg void OnSend();
  afx_msg void OnProxy();  
  afx_msg void OnTypeOther();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  string _GetOrderString(int index);
  string _GetTypeString(int index);
  string _FormatParol(const CString& ref, const CString& context);
  string _FormatPair();

private:
  tstring mPostUrl;
  string mLangBibleFromCaller; // "" if none
  IProxySession &mProxySession;
  OLosung* mpLosung;
  OTestimony* mpTestament1;
  OTestimony* mpTestament2;
  string mServerName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.


// === CPostPairDlg implementation ===

#define ORDER_DONT_KNOW_INDEX 2
#define TYPE_OTHER_INDEX      7
#define TYPE_DONT_KNOW_INDEX  8

CPostPairDlg::CPostPairDlg(
    IProxySession& proxySession,
    const string& postUrl, 
    OLosung* pLosung,
    string langBible,
    CWnd* pParent)
: CDialog(CPostPairDlg::IDD, pParent)
, mPostUrl(postUrl)
, mLangBibleFromCaller(langBible)
, mProxySession(proxySession)
, mpLosung(pLosung)
, mpTestament1(0)
, mpTestament2(0)
{
  NStringUtil::Pop("_", mLangBibleFromCaller); // cut off leading language
  //{{AFX_DATA_INIT(CPostPairDlg)
  mParol1 = _T("");
  mParolContext1 = _T("");
  mParol2 = _T("");
  mParolContext2 = _T("");
  mBibleEdition = mLangBibleFromCaller.c_str();
  mOrderIndex = ORDER_DONT_KNOW_INDEX;
  mTypeIndex = TYPE_DONT_KNOW_INDEX;
  mTypeOther = _T("");
  mThirdPartyFree = FALSE;
  //}}AFX_DATA_INIT

  // compute testaments
  if (mpLosung) {
    OLosung::OIter it = mpLosung->Begin();
    if (it != mpLosung->End()) {
      mpTestament1 = *it;
      ++it;
      if (it != mpLosung->End()) {
        mpTestament2 = *it;
      }
    }
  }
  // disable insertion buttons after construction only!
  
  // compute mServerName from postUrl
  CString server;
  DWORD serviceType;
  CString strServerName;
  CString strObject;
  INTERNET_PORT nPort;
  if (!AfxParseURL(mPostUrl.c_str(), serviceType, strServerName, strObject, nPort)) {
    mServerName = "???";
  }
  else {
    mServerName = (LPCTSTR) strServerName;
  }
}

CPostPairDlg::~CPostPairDlg()
{
  mProxySession.CloseSession();
}

void CPostPairDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPostPairDlg)
  DDX_Text(pDX, IDC_PAROL1, mParol1);
  DDX_Text(pDX, IDC_CONTEXT_PAROL1, mParolContext1);
  DDX_Text(pDX, IDC_PAROL2, mParol2);
  DDX_Text(pDX, IDC_CONTEXT_PAROL2, mParolContext2);
  DDX_Text(pDX, IDC_BIBLEEDITION, mBibleEdition);
  DDX_Radio(pDX, IDC_ORDER_THIS, mOrderIndex);
  DDX_Radio(pDX, IDC_TYPE_EXAMPLE, mTypeIndex);
  DDX_Text(pDX, IDC_TYPE_TEXT_OTHER, mTypeOther);
  DDX_Check(pDX, IDC_THIRDPARTYFREE, mThirdPartyFree);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPostPairDlg, CDialog)
  //{{AFX_MSG_MAP(CPostPairDlg)
  ON_BN_CLICKED(IDC_INSERT_PAROL1, OnInsertParol1)
  ON_BN_CLICKED(IDC_INSERT_PAROL2, OnInsertParol2)
  ON_BN_CLICKED(IDC_SWAP, OnSwap)
  ON_BN_CLICKED(IDC_RESET, OnDelete)
  ON_BN_CLICKED(IDC_SEND, OnSend)
  ON_BN_CLICKED(IDC_POST_PAIR_PROXY, OnProxy)
  ON_BN_CLICKED(IDC_TYPE_EXAMPLE,     OnTypeOther)
  ON_BN_CLICKED(IDC_TYPE_OPPOSITE,    OnTypeOther)
  ON_BN_CLICKED(IDC_TYPE_FULFILLMENT, OnTypeOther)
  ON_BN_CLICKED(IDC_TYPE_EXPLANATION, OnTypeOther)
  ON_BN_CLICKED(IDC_TYPE_EXTENSION,   OnTypeOther)
  ON_BN_CLICKED(IDC_TYPE_CITATION,    OnTypeOther)
  ON_BN_CLICKED(IDC_TYPE_PARALLEL,    OnTypeOther)
  ON_BN_CLICKED(IDC_TYPE_OTHER,       OnTypeOther)
  ON_BN_CLICKED(IDC_TYPE_DONTKNOW,    OnTypeOther)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


string CPostPairDlg::_GetOrderString(int index)
{
  string res;
  switch (index) {
    case 0: res = _T("this"); break;
    case 1: res = _T("both"); break;
    default:
    case 2: res = _T("dontknow"); break;
  }
#if ORDER_DONT_KNOW_INDEX != 2
#error Adapt ORDER_DONT_KNOW_INDEX!
#endif

  return res;
}

string CPostPairDlg::_GetTypeString(int index)
{
  string res;
  switch (index) {
    case 0: res = _T("example"); break;
    case 1: res = _T("opposite"); break;
    case 2: res = _T("fulfillment"); break;
    case 3: res = _T("explanation"); break;
    case 4: res = _T("extension"); break;
    case 5: res = _T("citation"); break;
    case 6: res = _T("parallel"); break;
    case TYPE_OTHER_INDEX:
      res = mTypeOther; // assume data has been fetched from control
      break;
    default:
    case 8: res = _T("dontknow"); break;
  }
#if TYPE_OTHER_INDEX != 7
#error Adapt TYPE_OTHER_INDEX!
#endif
#if TYPE_DONT_KNOW_INDEX != 8
#error Adapt TYPE_DONT_KNOW_INDEX!
#endif

  return res;
}

string CPostPairDlg::_FormatParol(const CString& ref, const CString& context) 
{
  string parol("<parol");
  // Note: Content() returns wstring, which is supported by CString
  if (mpTestament1 && ref == mpTestament1->SourceLine().Content().c_str()) {
    parol += " ParolID='" + mpTestament1->ParolID() + "'";
  }
  else if (mpTestament2 && ref == mpTestament2->SourceLine().Content().c_str()) {
    parol += " ParolID='" + mpTestament2->ParolID() + "'";
  }  
  parol += ">\n";
  if (!context.IsEmpty()) {
    parol += "<text>" + context + "</text>\n";
  }
  parol += "<ref>" + ref + "</ref>\n";
  parol += "</parol>\n";
  return parol;
}

string CPostPairDlg::_FormatPair() 
{
  string pair =
    "<pair "
    "order='" + _GetOrderString(mOrderIndex) + "' "
    "type='"  + _GetTypeString(mTypeIndex) + "'>\n";
  pair += _FormatParol(mParol1, mParolContext1);
  pair += _FormatParol(mParol2, mParolContext2);
  pair += "<bibleedition>" + mBibleEdition + "</bibleedition>\n";
  pair += "</pair>\n";
  return pair;
}


// === public ===

int CPostPairDlg::DoModal()
{
  return inherited::DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CPostPairDlg 

BOOL CPostPairDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if (!mpLosung) {
    // disable insertion buttons
    CButton *pButton = 0;

    pButton = (CButton *) GetDlgItem(IDC_INSERT_PAROL1);
    if (pButton) {
      pButton->EnableWindow(FALSE);
    }

    pButton = (CButton *) GetDlgItem(IDC_INSERT_PAROL2);
    if (pButton) {
      pButton->EnableWindow(FALSE);
    }
  }

  CEdit *pEdit = (CEdit*) GetDlgItem(IDC_TYPE_TEXT_OTHER);
  pEdit->SetReadOnly(mTypeIndex != TYPE_OTHER_INDEX); // other edit field enabled if its radio selected

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

//TODO low 2008-11-21 HS: if user clicks into "Type other" edit control, select its radio button

void CPostPairDlg::OnInsertParol1() 
{
  if (mpTestament1) {
    mParol1 = mpTestament1->SourceLine().Content().c_str();
    mParolContext1 = _T("");
  }
  UpdateData(FALSE); // refresh controls
}

void CPostPairDlg::OnInsertParol2() 
{
  if (mpTestament2) {
    mParol1 = mpTestament2->SourceLine().Content().c_str();
    mParolContext1 = _T("");
  }
  UpdateData(FALSE); // refresh controls
}

void CPostPairDlg::OnSwap() 
{
  UpdateData(TRUE);

  string tmpParol(mParol1);
  string tmpParolContext(mParolContext1);

  mParol1 = mParol2;
  mParolContext1 = mParolContext2;
  
  mParol2 = tmpParol.c_str();
  mParolContext2 = tmpParolContext.c_str();

  UpdateData(FALSE); // refresh controls
}

void CPostPairDlg::OnDelete() 
{
  mParol1 = _T("");
  mParolContext1 = _T("");
  mParol2 = _T("");
  mParolContext2 = _T("");
  mOrderIndex = ORDER_DONT_KNOW_INDEX;
  mTypeIndex = TYPE_DONT_KNOW_INDEX;
  UpdateData(FALSE); // refresh controls
}

void CPostPairDlg::OnSend() 
{
  UpdateData(TRUE);

  // === Check ===
  if (mParol1.IsEmpty() || mParol2.IsEmpty() || mBibleEdition.IsEmpty()) {
    AfxMessageBox(IDS_POSTPAIR_DATA_MISSING);
    return;
  }

  if (!mThirdPartyFree) {
    AfxMessageBox(IDS_PLEASE_THIRDPARTYFREE);
    return;
  }

  // === Compute data to send ===
  string pair = _FormatPair();

  // convert to UTF-8 (be defensive: if it fails, keep input as is)
  wstring wide;
  string utf8;
  if (NEncoding::Widen(pair, wide) && NEncoding::Narrow(wide, utf8)) {
    pair = utf8;
  }

  // AfxMessageBox(pair.c_str());

  MSGDEBUG("Sending suggestion to server " + mPostUrl);
  string serverResult;

  int res = mProxySession.Post(mPostUrl, pair, serverResult);
  if (res) {
    IErrorMsgFormatter& fmt = NErrorMsgFormatter::MakeErrorMsgFormatter(mPostUrl);
    CString buf;
    fmt.FormatDownloadError(IDS_POSTPAIR_FAILED, res, buf);
    delete &fmt;
    AfxMessageBox(buf);
  }
  else {
    AfxMessageBox(IDS_POSTPAIR_OK);
  }


  // === Set focus to first input control
  CEdit *pEdit = (CEdit *) GetDlgItem(IDC_PAROL1);
  assert(pEdit);
  pEdit->SetFocus();
}

void CPostPairDlg::OnProxy() 
{
  CProxyDlg proxyDlg(mProxySession.GetUseProxy(), mProxySession.GetProxy());
  if (IDOK == proxyDlg.DoModal()) {
    mProxySession.SetProxy(proxyDlg.UseProxy(), proxyDlg.Proxy());
  }
}

void CPostPairDlg::OnTypeOther() 
{
  UpdateData(TRUE);
  CEdit *pEdit = (CEdit*) GetDlgItem(IDC_TYPE_TEXT_OTHER);
  pEdit->SetReadOnly(mTypeIndex != TYPE_OTHER_INDEX); // other edit field enabled if its radio selected
  // do not set focus - allow navigating to the subsequent radio button via cursor keys
  //if (mTypeIndex == TYPE_OTHER_INDEX) {
  //  pEdit->SetFocus();
  //  return true;
  //}
}

// === NPostPairDlg ===

IPostPairDlg &NPostPairDlg::MakePostPairDlg(
    IProxySession& proxySession,
    const string& postUrl,
    OLosung* pLosung,
    string langBible,
    CWnd* pParent)
{
  IPostPairDlg *pPostPairDlg = new CPostPairDlg(proxySession, postUrl, pLosung, langBible, pParent);
  assert(pPostPairDlg);
  return *pPostPairDlg;
}
