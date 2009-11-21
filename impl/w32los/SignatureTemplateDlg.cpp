/* ----------------------------------------------------------------------
  Helmut Steeb 
  Dialog for signature templates
  $Id: SignatureTemplateDlg.cpp 3827 2009-08-01 16:14:57Z helmut $

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
---------------------------------------------------------------------- */

#include "stdafx.h"
#include <assert.h>
#include "w32los.h"
#include "SignatureTemplateDlg.h"
#include "SignatureManager.h"
#include "SignatureFormatter.h" // TKEY_*
#include "../util/FileUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSignatureTemplateDlg 


CSignatureTemplateDlg::CSignatureTemplateDlg(
    ISignatureManager &signatureManager,
    CWnd* pParent /*=NULL*/)
	: CDialog(CSignatureTemplateDlg::IDD, pParent),
    mSignatureManager(signatureManager)
{
	//{{AFX_DATA_INIT(CSignatureTemplateDlg)
	m_SignatureTemplateCurrent  = mSignatureManager.GetTemplate().c_str();
	m_SignatureTargetFilename   = mSignatureManager.GetTargetFilename().c_str();
	m_SignatureCurrent          = _T("");
	//}}AFX_DATA_INIT
  Format();
}


void CSignatureTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSignatureTemplateDlg)
	DDX_Text(pDX, IDC_SIGNATURE_TEMPLATE_TEXT, m_SignatureTemplateCurrent);
	DDX_Text(pDX, IDC_SIGNATURE_PREVIEW, m_SignatureCurrent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSignatureTemplateDlg, CDialog)
	//{{AFX_MSG_MAP(CSignatureTemplateDlg)
	ON_BN_CLICKED(IDC_INSERT_SIGNATURE_KEYWORD, OnInsertSignatureKeyword)
	ON_BN_CLICKED(IDC_UPDATE_SIGNATURE,         OnUpdateSignature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSignatureTemplateDlg::Format()
{
  CW32losApp *pApp = static_cast<CW32losApp*>(AfxGetApp());
  assert(pApp);
  
  m_SignatureCurrent = FileUtil::Moronize(
    mSignatureManager.Format(
      pApp->GetDoc().GetFirstLosung(),
      (LPCTSTR) m_SignatureTemplateCurrent).c_str()
    ).c_str();
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSignatureTemplateDlg 

BOOL CSignatureTemplateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_SIGNATURE_KEYWORDS);
  assert(pComboBox);

  CString keyword;
  // HS 2004-12-24: assume IDs are ordered!
  for (unsigned int id = IDS_SIGNATURE_TEMPLATE_KW_LOSUNG; id <= IDS_SIGNATURE_TEMPLATE_KW_NT; ++id) {
    keyword.LoadString(id);
    pComboBox->AddString(keyword);
  }
  pComboBox->SetCurSel(0);


  CEdit *pEdit = (CEdit*) GetDlgItem(IDC_SIGNATURE_TEMPLATE_TEXT);
  assert(pEdit);
  pEdit->SetFocus();
  return FALSE;
  //return TRUE; // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CSignatureTemplateDlg::OnOK() 
{
  CWaitCursor wait;
	if (!UpdateData(TRUE)) {
		MSGERROR("UpdateData failed during Signature dialog termination");
		return;
	}
  if (!mSignatureManager.SetTemplate( (LPCTSTR) m_SignatureTemplateCurrent)) {
    tstring signatureTemplateFilename(mSignatureManager.GetTemplateFilename());
		MSGERROR("Writing template file failed: " << signatureTemplateFilename);
    CString msg;
    msg.Format(IDS_SIGNATURE_TEMPLATE_WRITE_FAILED, signatureTemplateFilename.c_str());
    AfxMessageBox(msg);
    return;
  }
	EndDialog(IDOK);
}

void CSignatureTemplateDlg::OnInsertSignatureKeyword()
{
  CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_SIGNATURE_KEYWORDS);
  assert(pComboBox);
  int sel = pComboBox->GetCurSel();
  if (sel == CB_ERR) {
		MSGERROR("Error from getting ComboBox selection");
    return;
  }
  CEdit *pEdit = (CEdit*) GetDlgItem(IDC_SIGNATURE_TEMPLATE_TEXT);
  assert(pEdit);

/*
Vollständiger Text
Datumszeile
Losung (AT)
Lehrtext (NT)
*/
  tstring ins;
  switch (sel) {
  case 0: ins = TKEY_LOSUNG;break;
  case 1: ins = TKEY_TL;    break;
  case 2: ins = TKEY_OT;    break;
  case 3: ins = TKEY_NT;    break;
  default:ins = _T("???"); break;
  }
  // for convenience, add linebreak after keyword!
  ins += _T("\r\n");

  pEdit->ReplaceSel(ins.c_str());
  pEdit->SetFocus();
    
  OnUpdateSignature();
}

void CSignatureTemplateDlg::OnUpdateSignature()
{
	UpdateData(TRUE);
  Format();
	UpdateData(FALSE);
}
