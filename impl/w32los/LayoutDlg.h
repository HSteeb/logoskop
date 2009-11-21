/* ----------------------------------------------------------------------
  Helmut Steeb
  Class for layout dialog.
  $Id: LayoutDlg.h 3853 2009-08-26 13:50:27Z helmut $

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
#if !defined(AFX_LAYOUTDLG_H__INCLUDED_)
#define AFX_LAYOUTDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ILayoutDlg.h"

class OLayoutController;

class OLayoutDlg : public CDialog, public ILayoutDlg
{
  // Konstruktion
public:
  OLayoutDlg(
    CWnd* pParent = NULL);

  void SetLayoutController(OLayoutController &layoutController);

  // === ILayoutDlg ===

  virtual void SetBkCol(COLORREF col);
  virtual void SetLangBibles(const std::vector<tstring> &langBibles);
  virtual void SetApplyToAllLangBibles(bool b);
  virtual void SetLangBible(const tstring &langBible);
  virtual void SetLangBibleAll();
  virtual void SetLineLayout(ILosungLayout::OLineType lineType, const ILineLayout &lineLayout);
  virtual void SetLosungLayout(const ILosungLayout &losungLayout);

  // not needed (copy is possible always):
  // virtual void SetLineCopyButtonSens(ILosungLayout::OLineType lineType, bool b);
  // virtual void SetLosungCopyButtonSens(bool b);
  virtual void SetLinePasteButtonSens(ILosungLayout::OLineType lineType, bool b);
  virtual void SetLosungPasteButtonSens(bool b);
  virtual void SetUndoButtonSens(bool b);
  virtual void SetRedoButtonSens(bool b);

  
protected:
  OLayoutController &GetLayoutController();

  // conversions
  COLORREF _String2Col(const string &col);
  CString _Col2String(COLORREF col);
  int _Control2Size(FLOAT f);
  FLOAT _Size2Control(int size);

  void OLayoutDlg::OnSelchangeCombo();
  void OnLayoutAllLangBibles();
  bool _SetLineLayout(ILosungLayout::OLineType lineType, const ILineLayout &lineLayout);
  void _SetButtonSens(int nID, bool b);

private:
  OLayoutController *mpLayoutController;

	CBitmapButton mBtnCopyLosung;
	CBitmapButton mBtnPasteLosung;

	CBitmapButton mBtnFontPropTL;
	CBitmapButton mBtnCopyPropTL;
	CBitmapButton mBtnPastePropTL;

	CBitmapButton mBtnFontPropIL;
	CBitmapButton mBtnCopyPropIL;
	CBitmapButton mBtnPastePropIL;

	CBitmapButton mBtnFontPropL;
	CBitmapButton mBtnCopyPropL;
	CBitmapButton mBtnPastePropL;

	CBitmapButton mBtnFontPropSL;
	CBitmapButton mBtnCopyPropSL;
	CBitmapButton mBtnPastePropSL;


  // === MFC stuff ===================================================
public:

  // Dialogfelddaten
  //{{AFX_DATA(OLayoutDlg)
  enum { IDD = IDD_LAYOUT };
  int mLangBibleIndex;

  BOOL mApplyToAllLangBibles;

	CString m_strTLFont;
	CString m_strILFont;
	CString m_strLFont;
	CString m_strSLFont;

  FLOAT m_fTLSize;
  FLOAT m_fILSize;
  FLOAT m_fLSize;
  FLOAT m_fSLSize;
  //}}AFX_DATA


  // Überschreibungen
  // Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
  //{{AFX_VIRTUAL(OLayoutDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
  //}}AFX_VIRTUAL

// Implementierung
protected:

  // Generierte Nachrichtenzuordnungsfunktionen
  //{{AFX_MSG(OLayoutDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnSelectBkCol();
  afx_msg void OnSelchangeLangBibleCombo();
  afx_msg void OnKillFocusTLName();
  afx_msg void OnKillFocusILName();
  afx_msg void OnKillFocusLName();
  afx_msg void OnKillFocusSLName();
  afx_msg void OnKillFocusTLSize();
  afx_msg void OnKillFocusILSize();
  afx_msg void OnKillFocusLSize();
  afx_msg void OnKillFocusSLSize();
  afx_msg void OnCopyLosung();
  afx_msg void OnPasteLosung();
  afx_msg void OnCopyLine();
  afx_msg void OnPasteLine();
  afx_msg void OnProperties();
  afx_msg void OnUndo();
  afx_msg void OnRedo();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_LAYOUTDLG_H__INCLUDED_
