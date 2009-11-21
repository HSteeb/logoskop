/* ----------------------------------------------------------------------
  Helmut Steeb
  Class for layout dialog.
  $Id: LayoutDlg.cpp 3853 2009-08-26 13:50:27Z helmut $

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
#include "Resource.h"
#include "LayoutDlg.h"
#include "LayoutController.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



OLayoutDlg::OLayoutDlg(
  CWnd* pParent /*= NULL*/)
: CDialog(OLayoutDlg::IDD, pParent)
, mpLayoutController(0)
{
  //{{AFX_DATA_INIT(OLayoutDlg)
  mLangBibleIndex = -1;

  mApplyToAllLangBibles = TRUE;

	m_strTLFont = "";
	m_strILFont = "";
	m_strLFont = "";
	m_strSLFont = "";

  m_fTLSize = 0;
  m_fILSize = 0;
  m_fLSize = 0;
  m_fSLSize = 0;

  //}}AFX_DATA_INIT

  LPTSTR COPY              = MAKEINTRESOURCE(IDB_COPY);
  LPTSTR COPY_PRESSED      = MAKEINTRESOURCE(IDB_COPYPRESSED);
  LPTSTR PASTE             = MAKEINTRESOURCE(IDB_PASTE);
  LPTSTR PASTE_PRESSED     = MAKEINTRESOURCE(IDB_PASTEPRESSED);
  LPTSTR PASTE_INSENSITIVE = MAKEINTRESOURCE(IDB_PASTEINSENSITIVE);
  LPTSTR FONTPROPS         = MAKEINTRESOURCE(IDB_FONTPROPERTIES);
  LPTSTR FONTPROPS_PRESSED = MAKEINTRESOURCE(IDB_FONTPROPERTIESPRESSED);
	if (
    // Losung
       !mBtnCopyLosung .LoadBitmaps(COPY,      COPY_PRESSED)
    || !mBtnPasteLosung.LoadBitmaps(PASTE,     PASTE_PRESSED) 
    || !mBtnPasteLosung.LoadBitmaps(PASTE,     PASTE_PRESSED, PASTE_PRESSED, PASTE_INSENSITIVE) 
    // TL
    || !mBtnFontPropTL .LoadBitmaps(FONTPROPS, FONTPROPS_PRESSED) 
    || !mBtnCopyPropTL .LoadBitmaps(COPY,      COPY_PRESSED) 
    || !mBtnPastePropTL.LoadBitmaps(PASTE,     PASTE_PRESSED, PASTE_PRESSED, PASTE_INSENSITIVE) 
    // IL
    || !mBtnFontPropIL .LoadBitmaps(FONTPROPS, FONTPROPS_PRESSED) 
    || !mBtnCopyPropIL .LoadBitmaps(COPY,      COPY_PRESSED) 
    || !mBtnPastePropIL.LoadBitmaps(PASTE,     PASTE_PRESSED, PASTE_PRESSED, PASTE_INSENSITIVE) 
    // L
    || !mBtnFontPropL  .LoadBitmaps(FONTPROPS, FONTPROPS_PRESSED) 
    || !mBtnCopyPropL  .LoadBitmaps(COPY,      COPY_PRESSED) 
    || !mBtnPastePropL .LoadBitmaps(PASTE,     PASTE_PRESSED, PASTE_PRESSED, PASTE_INSENSITIVE) 
    // SL
    || !mBtnFontPropSL .LoadBitmaps(FONTPROPS, FONTPROPS_PRESSED) 
    || !mBtnCopyPropSL .LoadBitmaps(COPY,      COPY_PRESSED) 
    || !mBtnPastePropSL.LoadBitmaps(PASTE,     PASTE_PRESSED, PASTE_PRESSED, PASTE_INSENSITIVE) 
    ) {
		TRACE0("Failed to load bitmaps for buttons\n");
		//AfxThrowResourceException();
	}
}

void OLayoutDlg::SetLayoutController(OLayoutController &layoutController)
{
  mpLayoutController = &layoutController;
}

OLayoutController &OLayoutDlg::GetLayoutController()
{
  assert(mpLayoutController);
  return *mpLayoutController;
}


void OLayoutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(OLayoutDlg)
  DDX_CBIndex(pDX, IDC_LANGBIBLE_COMBO, mLangBibleIndex);

  DDX_Check(pDX, IDC_LAYOUT_ALL, mApplyToAllLangBibles);

	DDX_Text(pDX, IDC_TL_FONT, m_strTLFont);
	DDX_Text(pDX, IDC_IL_FONT, m_strILFont);
	DDX_Text(pDX, IDC_L_FONT , m_strLFont);
	DDX_Text(pDX, IDC_SL_FONT, m_strSLFont);

	DDX_Text(pDX, IDC_TL_SIZE, m_fTLSize);
	DDX_Text(pDX, IDC_IL_SIZE, m_fILSize);
	DDX_Text(pDX, IDC_L_SIZE , m_fLSize);
	DDX_Text(pDX, IDC_SL_SIZE, m_fSLSize);

  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OLayoutDlg, CDialog)
  //{{AFX_MSG_MAP(OLayoutDlg)
  ON_BN_CLICKED(IDC_BKCOL_PROPS, OnSelectBkCol)
  ON_BN_CLICKED(IDC_LAYOUT_ALL, OnLayoutAllLangBibles)

  ON_CBN_SELCHANGE(IDC_LANGBIBLE_COMBO, OnSelchangeLangBibleCombo)

  ON_EN_KILLFOCUS(IDC_TL_FONT, OnKillFocusTLName)
  ON_EN_KILLFOCUS(IDC_IL_FONT, OnKillFocusILName)
  ON_EN_KILLFOCUS(IDC_L_FONT , OnKillFocusLName)
  ON_EN_KILLFOCUS(IDC_SL_FONT, OnKillFocusSLName)

  ON_EN_KILLFOCUS(IDC_TL_SIZE, OnKillFocusTLSize)
  ON_EN_KILLFOCUS(IDC_IL_SIZE, OnKillFocusILSize)
  ON_EN_KILLFOCUS(IDC_L_SIZE , OnKillFocusLSize)
  ON_EN_KILLFOCUS(IDC_SL_SIZE, OnKillFocusSLSize)

  ON_BN_CLICKED(IDC_TL_PROPS, OnProperties)
  ON_BN_CLICKED(IDC_IL_PROPS, OnProperties)
  ON_BN_CLICKED(IDC_L_PROPS , OnProperties)
  ON_BN_CLICKED(IDC_SL_PROPS, OnProperties)

  ON_BN_CLICKED(IDC_LOSUNG_COPY,  OnCopyLosung)
  ON_BN_CLICKED(IDC_LOSUNG_PASTE, OnPasteLosung)

  ON_BN_CLICKED(IDC_TL_COPY,  OnCopyLine)
  ON_BN_CLICKED(IDC_IL_COPY,  OnCopyLine)
  ON_BN_CLICKED(IDC_L_COPY ,  OnCopyLine)
  ON_BN_CLICKED(IDC_SL_COPY,  OnCopyLine)
  ON_BN_CLICKED(IDC_TL_PASTE, OnPasteLine)
  ON_BN_CLICKED(IDC_IL_PASTE, OnPasteLine)
  ON_BN_CLICKED(IDC_L_PASTE , OnPasteLine)
  ON_BN_CLICKED(IDC_SL_PASTE, OnPasteLine)

  ON_BN_CLICKED(IDC_LAYOUT_UNDO, OnUndo)
  ON_BN_CLICKED(IDC_LAYOUT_REDO, OnRedo)

  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten OLayoutDlg 


BOOL OLayoutDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  // now our controls are initialized, allow controller to fill them
  GetLayoutController().Init();

	VERIFY(mBtnCopyLosung.SubclassDlgItem(IDC_LOSUNG_COPY, this));
  mBtnCopyLosung.SizeToContent();
	VERIFY(mBtnPasteLosung.SubclassDlgItem(IDC_LOSUNG_PASTE, this));

	VERIFY(mBtnFontPropTL .SubclassDlgItem(IDC_TL_PROPS, this));
	VERIFY(mBtnCopyPropTL .SubclassDlgItem(IDC_TL_COPY , this));
	VERIFY(mBtnPastePropTL.SubclassDlgItem(IDC_TL_PASTE, this));

	VERIFY(mBtnFontPropIL .SubclassDlgItem(IDC_IL_PROPS, this));
	VERIFY(mBtnCopyPropIL .SubclassDlgItem(IDC_IL_COPY , this));
	VERIFY(mBtnPastePropIL.SubclassDlgItem(IDC_IL_PASTE, this));

	VERIFY(mBtnFontPropL  .SubclassDlgItem(IDC_L_PROPS , this));
	VERIFY(mBtnCopyPropL  .SubclassDlgItem(IDC_L_COPY  , this));
	VERIFY(mBtnPastePropL .SubclassDlgItem(IDC_L_PASTE , this));

	VERIFY(mBtnFontPropSL .SubclassDlgItem(IDC_SL_PROPS, this));
	VERIFY(mBtnCopyPropSL .SubclassDlgItem(IDC_SL_COPY , this));
	VERIFY(mBtnPastePropSL.SubclassDlgItem(IDC_SL_PASTE, this));

  /* 2009-08-22 - SetBitmap does not work
  // cf. http://www.tutorials.de/forum/visualstudio-mfc/230727-cbutton-setbitmap-funktioniert-nicht.html
  // SetBitmap
  //   ASSERT(::IsWindow(m_hWnd))
  // => must be called after .SubclassDlgItem
  // HBITMAP mhCopy;
  mhCopy = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_LOSUNG_COPY));
	mBtnCopyPropIL .SetBitmap(mhCopy);
	*/

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

// --- conversions ---

COLORREF OLayoutDlg::_String2Col(const string &col)
{
  if (col[0] != _T('#')) {
    return 0;
  }
  std::istringstream istr(col.substr(1));
  COLORREF lCol;
  istr >> std::hex >> lCol;
  return lCol;
}

CString OLayoutDlg::_Col2String(COLORREF col)
{
  char buf[10];
  sprintf(buf, "#%06x", col);
  return buf;
}

int OLayoutDlg::_Control2Size(FLOAT f)
{
  return (int) (f*10);
}

FLOAT OLayoutDlg::_Size2Control(int size)
{
  return ((float) size) / 10;
}

// --- handlers ---

void OLayoutDlg::OnSelectBkCol() 
{
  GetLayoutController().SelectBkCol();  
}

// LangBibles combo

void OLayoutDlg::OnSelchangeLangBibleCombo() 
{
  UpdateData(TRUE);// updates mLangBibleIndex
  if (mLangBibleIndex > -1) {
    CComboBox *pCombo = (CComboBox *) GetDlgItem(IDC_LANGBIBLE_COMBO);
    assert(pCombo);
    CString langBible;
    pCombo->GetLBText(mLangBibleIndex, langBible);
    GetLayoutController().SetLangBible((LPCTSTR) langBible);
  }
}

void OLayoutDlg::OnLayoutAllLangBibles() 
{
  UpdateData(TRUE);// updates mApplyToAllLangBibles
  GetLayoutController().SetAllLangBiblesMode(mApplyToAllLangBibles != 0);
}

// Name

void OLayoutDlg::OnKillFocusTLName()
{
	UpdateData();
  GetLayoutController().SetLineFontName(ILosungLayout::ltTL, (LPCTSTR) m_strTLFont);
}

void OLayoutDlg::OnKillFocusILName()
{
	UpdateData();
  GetLayoutController().SetLineFontName(ILosungLayout::ltIL, (LPCTSTR) m_strILFont);
}

void OLayoutDlg::OnKillFocusLName()
{
	UpdateData();
  GetLayoutController().SetLineFontName(ILosungLayout::ltL, (LPCTSTR) m_strLFont);
}

void OLayoutDlg::OnKillFocusSLName()
{
	UpdateData();
  GetLayoutController().SetLineFontName(ILosungLayout::ltSL, (LPCTSTR) m_strSLFont);
}

// Size

void OLayoutDlg::OnKillFocusTLSize()
{
	UpdateData();
  GetLayoutController().SetLineFontSize(ILosungLayout::ltTL, _Control2Size(m_fTLSize));
}

void OLayoutDlg::OnKillFocusILSize()
{
	UpdateData();
  GetLayoutController().SetLineFontSize(ILosungLayout::ltIL, _Control2Size(m_fILSize));
}

void OLayoutDlg::OnKillFocusLSize()
{
	UpdateData();
  GetLayoutController().SetLineFontSize(ILosungLayout::ltL, _Control2Size(m_fLSize));
}

void OLayoutDlg::OnKillFocusSLSize()
{
	UpdateData();
  GetLayoutController().SetLineFontSize(ILosungLayout::ltSL, _Control2Size(m_fSLSize));
}

// Font selection dialog

void OLayoutDlg::OnProperties() 
{
  CWnd *pFocusWnd = GetFocus();
  if (!pFocusWnd) return;

  int ID = pFocusWnd->GetDlgCtrlID();
  ILosungLayout::OLineType lineType;
  switch (ID) {
  case IDC_TL_PROPS: lineType = ILosungLayout::ltTL; break;
  case IDC_IL_PROPS: lineType = ILosungLayout::ltIL; break;
  case IDC_L_PROPS : lineType = ILosungLayout::ltL;  break;
  case IDC_SL_PROPS: lineType = ILosungLayout::ltSL; break;
  default:
    MSGERROR(_T("LayoutDlg::OnProperties: unhandled id ") << ID);
    return;
  }

  GetLayoutController().Properties(lineType);  
}


// Clipboard operations

void OLayoutDlg::OnCopyLosung() 
{
  GetLayoutController().CopyLosung();
}

void OLayoutDlg::OnPasteLosung() 
{
  GetLayoutController().PasteLosung();
}

void OLayoutDlg::OnCopyLine() 
{
  CWnd *pFocusWnd = GetFocus();
  if (!pFocusWnd) return;

  int ID = pFocusWnd->GetDlgCtrlID();
  ILosungLayout::OLineType lineType;
  switch (ID) {
  case IDC_TL_COPY: lineType = ILosungLayout::ltTL; break;
  case IDC_IL_COPY: lineType = ILosungLayout::ltIL; break;
  case IDC_L_COPY : lineType = ILosungLayout::ltL;  break;
  case IDC_SL_COPY: lineType = ILosungLayout::ltSL; break;
  default:
    MSGERROR(_T("LayoutDlg::OnCopyLine: unhandled id ") << ID);
    return;
  }
  GetLayoutController().CopyLine(lineType);  
}

void OLayoutDlg::OnPasteLine() 
{
  CWnd *pFocusWnd = GetFocus();
  if (!pFocusWnd) return;

  int ID = pFocusWnd->GetDlgCtrlID();
  ILosungLayout::OLineType lineType;
  switch (ID) {
  case IDC_TL_PASTE: lineType = ILosungLayout::ltTL; break;
  case IDC_IL_PASTE: lineType = ILosungLayout::ltIL; break;
  case IDC_L_PASTE : lineType = ILosungLayout::ltL;  break;
  case IDC_SL_PASTE: lineType = ILosungLayout::ltSL; break;
  default:
    MSGERROR(_T("LayoutDlg::OnPasteLine: unhandled id ") << ID);
    return;
  }
  GetLayoutController().PasteLine(lineType);  
}

void OLayoutDlg::OnUndo()
{
  GetLayoutController().Undo();  
}

void OLayoutDlg::OnRedo()
{
  GetLayoutController().Redo();  
}

// === ILayoutDlg ===

void OLayoutDlg::SetBkCol(COLORREF col)
{
  // NOP (edit field for BkCol has been dropped)
}

void OLayoutDlg::SetLangBibles(const std::vector<tstring> &langBibles)
{
  CComboBox *pCombo = (CComboBox *) GetDlgItem(IDC_LANGBIBLE_COMBO);
  assert(pCombo);
  pCombo->ResetContent();

  std::vector<string>::const_iterator it;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const string &langBible = *it;
    // add only if not yet contained - list from Document may contain duplicates
    // e.g. if INI file edited manually
    if (pCombo->FindStringExact(-1, langBible.c_str()) == CB_ERR) {
      pCombo->AddString(langBible.c_str());
    }
  }
  if (!langBibles.empty()) {
    mLangBibleIndex = 0;
    UpdateData(FALSE);  
    //why does this not work? pCombo->SetCurSel(0);
  }
}

void OLayoutDlg::SetApplyToAllLangBibles(bool b)
{
  if (::IsWindow(m_hWnd)) {
    mApplyToAllLangBibles = b;
    UpdateData(FALSE);  
  }
}

void OLayoutDlg::SetLangBible(const tstring &langBible)
{
  if (::IsWindow(m_hWnd)) {
    CComboBox *pCombo = (CComboBox *) GetDlgItem(IDC_LANGBIBLE_COMBO);
    assert(pCombo);
    int pos = pCombo->FindStringExact(-1, langBible.c_str());
    if (pos != CB_ERR) {
      mLangBibleIndex = pos;
      UpdateData(FALSE);  
    //why does this not work? pCombo->SetCurSel(pos);
    }
  }
}

void OLayoutDlg::SetLangBibleAll()
{
  if (::IsWindow(m_hWnd)) {
    UpdateData(FALSE);
  }
}

bool OLayoutDlg::_SetLineLayout(ILosungLayout::OLineType lineType, const ILineLayout &lineLayout)
{
  switch (lineType) {
  case ILosungLayout::ltTL:
    m_strTLFont = lineLayout.GetName().c_str();
    m_fTLSize  = _Size2Control(lineLayout.GetSize());
    break;
  case ILosungLayout::ltIL:
    m_strILFont = lineLayout.GetName().c_str();
    m_fILSize  = _Size2Control(lineLayout.GetSize());
    break;
  case ILosungLayout::ltL:
    m_strLFont = lineLayout.GetName().c_str();
    m_fLSize  = _Size2Control(lineLayout.GetSize());
    break;
  case ILosungLayout::ltSL:
    m_strSLFont = lineLayout.GetName().c_str();
    m_fSLSize  = _Size2Control(lineLayout.GetSize());
    break;
  default:
    return false;
    break;
  }
  if (::IsWindow(m_hWnd)) {
    UpdateData(FALSE);
  }
  return true;
}

void OLayoutDlg::SetLineLayout(ILosungLayout::OLineType lineType, const ILineLayout &lineLayout)
{
  if (_SetLineLayout(lineType, lineLayout)) {
    if (::IsWindow(m_hWnd)) {
      UpdateData(FALSE);
    }
  }
}

void OLayoutDlg::SetLosungLayout(const ILosungLayout &losungLayout)
{
  _SetLineLayout(ILosungLayout::ltTL, losungLayout.LineLayout(ILosungLayout::ltTL));
  _SetLineLayout(ILosungLayout::ltIL, losungLayout.LineLayout(ILosungLayout::ltIL));
  _SetLineLayout(ILosungLayout::ltL , losungLayout.LineLayout(ILosungLayout::ltL ));
  _SetLineLayout(ILosungLayout::ltSL, losungLayout.LineLayout(ILosungLayout::ltSL));

  if (::IsWindow(m_hWnd)) {
    UpdateData(FALSE);
  }
}

void OLayoutDlg::_SetButtonSens(int nID, bool b)
{
  CWnd *pWnd = GetDlgItem(nID);
  if (pWnd) {
    pWnd->EnableWindow(b);
  }
}

void OLayoutDlg::SetLinePasteButtonSens(ILosungLayout::OLineType lineType, bool b)
{
  int nID;
  switch (lineType) {
  case ILosungLayout::ltTL: nID = IDC_TL_PASTE; break;
  case ILosungLayout::ltIL: nID = IDC_IL_PASTE; break;
  case ILosungLayout::ltL : nID = IDC_L_PASTE ; break;
  case ILosungLayout::ltSL: nID = IDC_SL_PASTE; break;
  default:
    return;
  }
  _SetButtonSens(nID, b);
}

void OLayoutDlg::SetLosungPasteButtonSens(bool b)
{
  _SetButtonSens(IDC_LOSUNG_PASTE, b);
}

void OLayoutDlg::SetUndoButtonSens(bool b)
{
  _SetButtonSens(IDC_LAYOUT_UNDO, b);
}

void OLayoutDlg::SetRedoButtonSens(bool b)
{
  _SetButtonSens(IDC_LAYOUT_REDO, b);
}
