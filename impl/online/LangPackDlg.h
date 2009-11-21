/* ----------------------------------------------------------------------
 Helmut Steeb
 Module for "actualize" dialog.
  $Id: LangPackDlg.h 3853 2009-08-26 13:50:27Z helmut $

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
#if !defined(AFX_LANGPACKDLG_H__INCLUDED_)
#define AFX_LANGPACKDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProxySession.h" // tUseProxy

class CProfile;


class CLangPackDlg : public CDialog
{
// Konstruktion
public:
  CLangPackDlg(
    const tstring &langBibleString,
    int defaultYear,
    const tUseProxy useProxy, const string &proxy, CProfile &profile, 
    CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
  //{{AFX_DATA(CLangPackDlg)
  enum { IDD = IDD_LANGPACK };
  int    mYearChecked;
  CString  mCurLangBible;
  //}}AFX_DATA

  int GetSelectedYear() const;
  const tUseProxy UseProxy() const;
  const string &Proxy() const;
  bool GetLangPackDlgOn() const;

// Überschreibungen
  // Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
  //{{AFX_VIRTUAL(CLangPackDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  //}}AFX_VIRTUAL

// Implementierung
protected:

  // Generierte Nachrichtenzuordnungsfunktionen
  //{{AFX_MSG(CLangPackDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnLangpackProxy();
  afx_msg void OnAutoLangpack();  
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:
  bool mbHasCurrentLosung;
  int mFirstYear; // the real year, e.g. 2002
  int mSecondYear;
  int mDefaultYear;
  tUseProxy mUseProxy;
  string mProxy;
  CProfile &mProfile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif
