/* ----------------------------------------------------------------------
  Helmut Steeb
  Module for "proxy configuration" dialog.
  $Id: ProxyDlg.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include "ProxyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CProxyDlg::CProxyDlg(const tUseProxy useProxy, const string &proxy, CWnd* pParent /*=NULL*/)
: CDialog(CProxyDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CProxyDlg)
  m_UseProxy = -1;
  m_Proxy = _T("");
  //}}AFX_DATA_INIT
  switch (useProxy) {
  default:
  case proxyPreconfig:  m_UseProxy = 0; break;
  case proxyDirect:     m_UseProxy = 1; break;
  case proxyManual:     m_UseProxy = 2; break;
  }
  m_Proxy = proxy.c_str();
}


void CProxyDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CProxyDlg)
  DDX_Radio(pDX, IDC_PROXY_PRECONFIG, m_UseProxy);
  DDX_Text(pDX, IDC_PROXY, m_Proxy);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProxyDlg, CDialog)
  //{{AFX_MSG_MAP(CProxyDlg)
  ON_BN_CLICKED(IDC_PROXY_PRECONFIG, OnProxy)
  ON_BN_CLICKED(IDC_PROXY_DIRECT, OnProxy)
  ON_BN_CLICKED(IDC_PROXY_MANUAL, OnProxy)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CProxyDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  CEdit *pEdit = (CEdit*) GetDlgItem(IDC_PROXY);
  pEdit->SetReadOnly(m_UseProxy != 2); // proxy edit field enabled if radio 2 selected
  if (m_UseProxy == 2) {
    pEdit->SetFocus();
    return FALSE;
  }
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

const tUseProxy CProxyDlg::UseProxy() const
{
  switch (m_UseProxy) {
  default:
  case 0: return proxyPreconfig;
  case 1: return proxyDirect;
  case 2: return proxyManual;
  }
}

void CProxyDlg::OnProxy() 
{
  UpdateData(TRUE);
  CEdit *pEdit = (CEdit*) GetDlgItem(IDC_PROXY);
  pEdit->SetReadOnly(m_UseProxy != 2); // proxy edit field enabled if radio 2 selected
  if (m_UseProxy == 2) {
    pEdit->SetFocus();
  }
}

const string CProxyDlg::Proxy() const
{
  return (LPCTSTR) m_Proxy;
}


