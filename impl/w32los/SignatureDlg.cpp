/* ----------------------------------------------------------------------
  Helmut Steeb 
  Dialog for signatures
  $Id: SignatureDlg.cpp 3916 2009-09-21 18:38:20Z helmut $

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
#include "SignatureDlg.h"
#include "SignatureTemplateDlg.h"
#include "SignatureManager.h"
#include "../util/FileUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_TEMPLATE "--\r\n%THEWORD;\r\n"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSignatureDlg 


CSignatureDlg::CSignatureDlg(
    ISignatureManager &signatureManager,
    CWnd* pParent /*=NULL*/)
	: CDialog(CSignatureDlg::IDD, pParent),
    mSignatureManager(signatureManager)
{
  if (mSignatureManager.GetTemplate().empty()) {
    mSignatureManager.SetTemplate(DEFAULT_TEMPLATE);
  }

	//{{AFX_DATA_INIT(CSignatureDlg)
	m_SignatureOn               = mSignatureManager.IsSignatureOn();
	m_SignatureAsUtf8           = mSignatureManager.SignatureAsUtf8();
  m_SignatureTemplateFilename = mSignatureManager.GetTemplateFilename().c_str();
	m_SignatureTemplateCurrent  = mSignatureManager.GetTemplate().c_str();
	m_SignatureTargetFilename   = mSignatureManager.GetTargetFilename().c_str();
	m_SignatureCurrent          = mSignatureManager.GetTarget().c_str();
	//}}AFX_DATA_INIT
}


void CSignatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSignatureDlg)
	DDX_Check(pDX, IDC_SIGNATURE_ON, m_SignatureOn);
	DDX_Check(pDX, IDC_SIGNATURE_UTF8, m_SignatureAsUtf8);
	DDX_Text(pDX, IDC_SIGNATURE_TARGET, m_SignatureTargetFilename);
	DDX_Text(pDX, IDC_SIGNATURE_DLG_TEMPLATE, m_SignatureTemplateCurrent);
	DDX_Text(pDX, IDC_SIGNATURE_CURRENT, m_SignatureCurrent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSignatureDlg, CDialog)
	//{{AFX_MSG_MAP(CSignatureDlg)
	ON_BN_CLICKED(IDC_SIGNATURE_TARGET_SELECT, OnSelectTarget)
	ON_BN_CLICKED(IDC_SIGNATURE_TEMPLATE_HELP, OnSignatureTemplateHelp)
	ON_BN_CLICKED(IDC_EDIT_SIGNATURE_TEMPLATE, OnEditSignatureTemplate)
	ON_BN_CLICKED(IDC_WRITE_SIGNATURE, OnWriteSignature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSignatureDlg 

BOOL CSignatureDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  CButton *pButton = (CButton*) GetDlgItem(IDOK);
  assert(pButton);

  pButton->SetFocus();
  return FALSE;
//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CSignatureDlg::OnOK() 
{
  CWaitCursor wait;
	if (!UpdateData(TRUE)) {
		MSGERROR("UpdateData failed during Signature dialog termination.\n");
		return;
	}
  if (m_SignatureOn) {
    if (m_SignatureTargetFilename.IsEmpty()) {
      if (IDYES != AfxMessageBox(IDS_SIGNATURE_TARGET_EMPTY, MB_YESNO)) {
        return;
      }
      m_SignatureOn = FALSE;
    }
    else if (!FileUtil::Exists(m_SignatureTargetFilename)) {
      CString msg;
      msg.Format(IDS_SIGNATURE_TARGET_NOT_EXIST, m_SignatureTargetFilename);
      if (IDYES != AfxMessageBox(msg, MB_YESNO)) {
        return;
      }
      m_SignatureOn = FALSE;
    }
  }
  mSignatureManager.IsSignatureOn(m_SignatureOn != 0);
  mSignatureManager.SignatureAsUtf8(m_SignatureAsUtf8 != 0);
	mSignatureManager.SetTargetFilename( (LPCTSTR) m_SignatureTargetFilename);

	EndDialog(IDOK);
}

void CSignatureDlg::OnSelectTarget()
{
  CString fileName = m_SignatureTargetFilename;
  UINT nIDSTitle = IDS_SIGNATURE_TARGET_SELECT_TITLE;
  CString strFilterName;
  CString strFilterExt;

  strFilterName.LoadString(IDS_SIGNATURE_STR);
  strFilterExt.LoadString(IDS_SIGNATURE_EXT);
  CW32losApp *pApp = static_cast<CW32losApp*>(AfxGetApp());
  assert(pApp);
  if (pApp->DoPromptFileNames(
      fileName, nIDSTitle, 0/*NOT: OFN_FILEMUSTEXIST*/, /*bOpenFileDialog*/TRUE,
      strFilterName, strFilterExt, 
      // bAppendAllFiles: with "TRUE", missing AFX_IDS_ALLFILTER in ressource
      // results in VERIFY failing in DoPromptFileName()
      TRUE, /*#files:*/1)) {
    m_SignatureTargetFilename = fileName;
    mSignatureManager.SetTargetFilename( (LPCTSTR) m_SignatureTargetFilename);
    m_SignatureCurrent = mSignatureManager.GetTarget().c_str();
    UpdateData(FALSE);
  }
}

void CSignatureDlg::OnSignatureTemplateHelp()
{
  CW32losApp *pApp = static_cast<CW32losApp*>(AfxGetApp());
  assert(pApp);
  pApp->OpenFile(IDS_HELPFILE_PROGRAM, "readme.htm");  
}

void CSignatureDlg::OnEditSignatureTemplate()
{
  CSignatureTemplateDlg signatureTemplateDlg(mSignatureManager);
  if (IDOK == signatureTemplateDlg.DoModal()) {
    m_SignatureTemplateCurrent = mSignatureManager.GetTemplate().c_str();
    UpdateData(FALSE);
  }
}

void CSignatureDlg::OnWriteSignature()
{
  CWaitCursor wait;
	if (!UpdateData(TRUE)) {
		MSGERROR("UpdateData failed during OnWriteSignature.\n");
		return;
	}

  if (m_SignatureTargetFilename.IsEmpty()) {
    AfxMessageBox(IDS_SIGNATURE_TARGET_EMPTY_NOWRITE);
    return;
  }
  
  tstring signatureTemplateFilename(mSignatureManager.GetTemplateFilename());
  if (signatureTemplateFilename.empty()
    || mSignatureManager.GetTemplate(signatureTemplateFilename).empty()) {
    AfxMessageBox(IDS_SIGNATURE_TEMPLATE_EMPTY_NOWRITE);
    return;
  }

  //  update utf-8 setting
  mSignatureManager.SignatureAsUtf8(m_SignatureAsUtf8 != 0);

  CW32losApp *pApp = static_cast<CW32losApp*>(AfxGetApp());
  assert(pApp);
  // now Write - independent of current checkbox setting!
  mSignatureManager.Write(pApp->GetDoc().GetFirstLosung());
  m_SignatureCurrent = mSignatureManager.GetTarget().c_str();
	UpdateData(FALSE);
}

