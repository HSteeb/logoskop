/* ----------------------------------------------------------------------
  Helmut Steeb 
  Dialog for username + password
  $Id: UserPassDlg.cpp 3014 2008-11-29 10:19:16Z helmut $

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
------------------------------------------------------------------------- */

#include "stdafx.h"
#include <assert.h>
#include "../w32los/Resource.h"
#include "IUserPassDlg.h"
#include "UserPassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CUserPassDlg : public IUserPassDlg, public CDialog
{
// Konstruktion
public:
  CUserPassDlg(CWnd* pParent = NULL);   // Standardkonstruktor

  // --- IUserPassDlg ---
  virtual void SetUser(const string &user);
  virtual void SetPassword(const string &password);

  virtual string GetUser();
  virtual string GetPassword();

  virtual bool AskUserPass();


// Dialogfelddaten
  //{{AFX_DATA(CUserPassDlg)
  enum { IDD = IDD_USERPASS };
  CString  m_Username;
  CString  m_Password;
  //}}AFX_DATA


// Überschreibungen
  // Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
  //{{AFX_VIRTUAL(CUserPassDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  //}}AFX_VIRTUAL

// Implementierung
protected:

  // Generierte Nachrichtenzuordnungsfunktionen
  //{{AFX_MSG(CUserPassDlg)
    // HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CUserPassDlg 


CUserPassDlg::CUserPassDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CUserPassDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CUserPassDlg)
  m_Username = _T("");
  m_Password = _T("");
  //}}AFX_DATA_INIT
}


void CUserPassDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CUserPassDlg)
  DDX_Text(pDX, IDC_USERNAME, m_Username);
  DDX_Text(pDX, IDC_PASSWORD, m_Password);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserPassDlg, CDialog)
  //{{AFX_MSG_MAP(CUserPassDlg)
    // HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// --- IUserPassDlg ---

void CUserPassDlg::SetUser(const string &user)
{
  m_Username = user.c_str();
}

void CUserPassDlg::SetPassword(const string &password)
{
  m_Password = password.c_str();
}

string CUserPassDlg::GetUser()
{
  return m_Username;
}

string CUserPassDlg::GetPassword()
{
  return m_Password;
}

bool CUserPassDlg::AskUserPass()
{
  return IDOK == DoModal();
}


// === NUserPassDlg ===

IUserPassDlg& NUserPassDlg::MakeUserPassDlg()
{
  IUserPassDlg *pUserPassDlg = new CUserPassDlg();
  assert(pUserPassDlg);
  return *pUserPassDlg;
}
