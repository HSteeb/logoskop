/* ----------------------------------------------------------------------
  Helmut Steeb 
  Dialog for signature template
  $Id: SignatureTemplateDlg.h 3014 2008-11-29 10:19:16Z helmut $

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

#if !defined(_SIGNATURETEMPLATEDLG_INCLUDED_)
#define _SIGNATURETEMPLATEDLG_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class ISignatureManager;

class CSignatureTemplateDlg : public CDialog
{
// Konstruktion
public:
	CSignatureTemplateDlg(
    ISignatureManager &signatureManager,
    CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSignatureTemplateDlg)
	enum { IDD = IDD_SIGNATURE_TEMPLATE };
	CString	m_SignatureTemplateCurrent;
	CString	m_SignatureTargetFilename;
	CString	m_SignatureCurrent;
	//}}AFX_DATA

  ISignatureManager &mSignatureManager;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CUserPassDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
  
  bool SelectTarget();
  void Format();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSignatureTemplateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnInsertSignatureKeyword();
	afx_msg void OnUpdateSignature();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif
