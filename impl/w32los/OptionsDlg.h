/* ----------------------------------------------------------------------
  Helmut Steeb
  Module for options dialog.
  $Id: OptionsDlg.h 3827 2009-08-01 16:14:57Z helmut $

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
#if !defined(AFX_OPTIONSDLG_H__INCLUDED_)
#define AFX_OPTIONSDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OOptionsDlg : public CDialog
{
// Konstruktion
public:
  OOptionsDlg(
    const string &driveDir,
    const string &dialogLang, 
    const string& defaultLangNisoName,
    const bool startOnce, 
    const bool bUseUSP10,
    const bool bWriteLogfile,
    CWnd* pParent = NULL);

// Dialogfelddaten
  //{{AFX_DATA(OOptionsDlg)
  enum { IDD = IDD_OPTIONS };
  CString  m_DialogLangList; // [longName ]shortName
  BOOL  m_StartOnce;
  BOOL  m_UseUSP10; // 4.0.9
  BOOL  m_WriteLogfile; // 4.0.9
  //}}AFX_DATA

  const string mDriveDir;
  string mLangShortName;
  const string& mDefaultLangNisoName;

// Überschreibungen
  // Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
  //{{AFX_VIRTUAL(OOptionsDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  //}}AFX_VIRTUAL

// Implementierung
protected:

  // Generierte Nachrichtenzuordnungsfunktionen
  //{{AFX_MSG(OOptionsDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnDblclkDlglanglist();
  afx_msg void OnOpenLogfile();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OOptionsDlg_H__INCLUDED_
