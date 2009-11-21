/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop main frame.
  $Id: MainFrm.h 3853 2009-08-26 13:50:27Z helmut $

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

#if !defined(AFX_MAINFRM_H__INCLUDED_)
#define AFX_MAINFRM_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OTwdFileDescList;
class IAppPathManager;
class IProfile;
class CW32losDoc;
class CW32losView;

class CComboBar : public CToolBar
{
public:
	CComboBox   m_comboBox;
	CComboBox   m_comboBox2;
	CFont       m_font;
};


class CMainFrame : public CFrameWnd
{
	
protected: // Nur aus Serialisierung erzeugen
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attribute
public:

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CMainFrame();

  void Init(CView &view, IProfile &profile, IAppPathManager &appPathManager);
  void SetLangComboFocus();
  void EnableInput(bool bEnable);

  void OnDTNotify(NMHDR *pHdr, LRESULT *pRes);

  void UpdateDateTime();
  void UpdateFrameTitle();

  void FillLangbox();

  void WriteSettings();


protected:  // Eingebundene Elemente der Steuerleiste
	CStatusBar  m_wndStatusBar;
	CComboBar   m_wndToolBar;
	CDateTimeCtrl m_DateTime;

  CComboBox &_GetLangCombo1();
  CComboBox &_GetLangCombo2();
  CTime _GetDate();

  void _CreateDateTimePicker(int nIndex, int nID);
  void _CreateLangCombo(int nIndex, int nID, CComboBox &combo);
  void _SetFonts();

  // === Profile ===

  IProfile &_Profile();

  // === AppPathManager ===

  IAppPathManager &_AppPathManager();


  // === Document/view ===

  CW32losDoc &GetDoc();
  CW32losView &GetView();

private:
  OTwdFileDescList *mpLangComboFileDescList;

  // === Profile ===

  IProfile *mpProfile;
  
  // === AppPathManager ===

  IAppPathManager *mpAppPathManager;

  // === Document/view ===

  CW32losDoc  *mpDoc;
  CW32losView *mpView;

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
  afx_msg void OnUpdateFrameTitle(BOOL bAddToTitle);
  afx_msg void OnSelChangeLangBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_MAINFRM_H__INCLUDED_)
