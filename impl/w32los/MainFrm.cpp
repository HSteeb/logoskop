/* ----------------------------------------------------------------------
  Helmut Steeb
  Main frame.
  $Id: MainFrm.cpp 3905 2009-09-12 11:30:16Z helmut $

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

#include "w32losDoc.h"
#include "w32losView.h"
#include "resource.h"

#include "MainFrm.h"
#include "LangNames.h"
#include "../filedesc/TwdFileDesc.h"
#include "../filedesc/TwdFileDescList.h"
#include "../filedesc/DirectoryFileDescReader.h"
#include "../util/IProfile.h"
#include "../util/AppPathManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NO_LANGBIBLE2 (_T("---"))

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
  ON_NOTIFY(DTN_DATETIMECHANGE,IDC_MAINBAR_DATETIME, OnDTNotify)
  ON_CBN_SELCHANGE(IDC_MAINBAR_LANGBOX , OnSelChangeLangBox)
  ON_CBN_SELCHANGE(IDC_MAINBAR_LANGBOX2, OnSelChangeLangBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// HS 2001-01-02: comment out ID_INDICATOR_* to suppress display of indicators.
// But why not display "NUM" etc.?
static UINT indicators[] =
{
	ID_SEPARATOR,           // Statusleistenanzeige
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Konstruktion/Zerstörung

CMainFrame::CMainFrame()
: mpLangComboFileDescList(0)
, mpProfile(0)
, mpDoc(0)
, mpView(0)
{
	// ZU ERLEDIGEN: Hier Code zur Member-Initialisierung einfügen
  MSGDEBUG(_T("CMainFrame construction."));
}

CMainFrame::~CMainFrame()
{
  MSGDEBUG(_T("CMainFrame destruction."));
  delete mpLangComboFileDescList;
  mpLangComboFileDescList = 0;
}

CComboBox &CMainFrame::_GetLangCombo1() 
{ 
  return m_wndToolBar.m_comboBox;
}

CComboBox &CMainFrame::_GetLangCombo2() 
{ 
  return m_wndToolBar.m_comboBox2;
}


// === Profile ===

IProfile &CMainFrame::_Profile()
{
  ASSERT(mpProfile);
  return *mpProfile;
}


// === AppPathManager ===

IAppPathManager &CMainFrame::_AppPathManager()
{
  ASSERT(mpAppPathManager);
  return *mpAppPathManager;
}


// === Document/view ===

CW32losDoc &CMainFrame::GetDoc()
{
  ASSERT(mpDoc);
  return *mpDoc;
}

CW32losView &CMainFrame::GetView()
{
  ASSERT(mpView);
  return *mpView;
}

/* separators are counted in index, too */
#define DATETIME_INDEX 11
#define COMBO_INDEX    12
#define COMBO2_INDEX   13

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  MSGDEBUG(_T("CMainFrame::OnCreate..."));
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
    MSGERROR(_T("CMainFrame::OnCreate - CFrameWnd::OnCreate returned -1, skipping."));
		return -1;
  }
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, 
      WS_CHILD | WS_VISIBLE | CBRS_TOP| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)) {
    MSGERROR(_T("CMainFrame::OnCreate - creating toolbar failed, skipping."));
		TRACE0("Could not create toolbar\n");
		return -1;      // Fehler bei Erstellung
	}
	if (!m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
    MSGERROR(_T("CMainFrame::OnCreate - LoadToolBar failed, skipping."));
		TRACE0("Could not LoadToolBar\n");
		return -1;      // Fehler bei Erstellung
	}


  _CreateDateTimePicker(DATETIME_INDEX, IDC_MAINBAR_DATETIME);

  _CreateLangCombo(COMBO_INDEX , IDC_MAINBAR_LANGBOX , m_wndToolBar.m_comboBox );
  _CreateLangCombo(COMBO2_INDEX, IDC_MAINBAR_LANGBOX2, m_wndToolBar.m_comboBox2);
  _SetFonts();

	if (!m_wndStatusBar.Create(this))	{
    MSGERROR(_T("CMainFrame::OnCreate - could not create status bar, skipping."));
		TRACE0("Could not create status bar\n");
		return -1;      // Fehler bei Erstellung
	}

	if (!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))) {
    MSGERROR(_T("CMainFrame::OnCreate - could not SetIndicators of status bar, skipping."));
		TRACE0("Could not SetIndicators of status bar\n");
		return -1;      // Fehler bei Erstellung
	}

	// ZU ERLEDIGEN: Löschen Sie diese drei Zeilen, wenn Sie nicht wollen, dass die Symbolleiste
	//  andockbar ist.
  // HS 2001-01-02: do not use CBRS_ALIGN_ANY - combo box becomes ugly 
  // with vertical tool bar!
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	EnableDocking             (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	DockControlBar(&m_wndToolBar);

  MSGDEBUG(_T("CMainFrame::OnCreate done."));
  return 0;
}

void CMainFrame::_CreateDateTimePicker(int nIndex, int nID)
{
  MSGDEBUG(_T("CMainFrame::_CreateDateTimePicker index ") << nIndex);
  // Code from CmnCtrl1
  // MSDN says:
  // Note Date and time picker controls are implemented in version 4.70 and later of Comctl32.dll
  // Source: 
  //   MSDN | Plattform-SDK | User Interface Services | 
  //   Shell and Common Controls | Common Controls | Common Controls
  INITCOMMONCONTROLSEX icc;
  icc.dwICC = ICC_DATE_CLASSES;
  icc.dwSize = sizeof(icc);
  if (InitCommonControlsEx(&icc)) {
    m_wndToolBar.SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, /*width*/ 90);
    m_wndToolBar.SetButtonInfo(nIndex+1, ID_SEPARATOR, TBBS_SEPARATOR, 12);
  	CRect rect;
    m_wndToolBar.GetItemRect(DATETIME_INDEX, &rect);
    m_DateTime.Create(WS_CHILD|WS_VISIBLE|DTS_SHORTDATEFORMAT, rect, &m_wndToolBar, nID);
  }
  else {
    MSGERROR("InitCommonControlsEx failed for ICC_DATE_CLASSES - you seem to have Comctl32.dll version < 4.70, cannot use date and time picker control");
  }
}

void CMainFrame::_CreateLangCombo(int nIndex, int nID, CComboBox &combo)
{
  MSGDEBUG(_T("CMainFrame::_CreateLangCombo index ") << nIndex);
  // code from CtrlBars sample

  // the following determines the combobox width
  m_wndToolBar.SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, /* width*/ 160);

	// Design guide advises 12 pixel gap between combos and buttons
  // m_wndToolBar.SetButtonInfo(nIndex+1, ID_SEPARATOR, TBBS_SEPARATOR, 120);
	CRect rect;
	m_wndToolBar.GetItemRect(nIndex, &rect);
	rect.bottom = rect.top + /*drop height*/ 200; // reasonable default to avoid scrolling (about 8 lines?);
	if (!combo.Create(
			CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|/*!*/WS_VSCROLL|CBS_SORT,
			rect, &m_wndToolBar, nID))
	{
    MSGERROR("Failed to create langBible combo-box at index " << nIndex);
		TRACE0("Failed to create combo-box\n");
	}
}

void CMainFrame::_SetFonts()
{
  MSGDEBUG(_T("CMainFrame::_SetFonts"));
  //  Create a font for the combobox
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(logFont));

	if (!::GetSystemMetrics(SM_DBCSENABLED)) {
    // Since design guide says toolbars are fixed height so is the font.
    logFont.lfHeight = -12;
    logFont.lfWeight = FW_NORMAL; // FW_BOLD;
    logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
    CString strDefaultFont;
    strDefaultFont.LoadString(IDS_DEFAULT_FONT);
    
    lstrcpy(logFont.lfFaceName, strDefaultFont);
    if (!m_wndToolBar.m_font.CreateFontIndirect(&logFont)) {
      MSGERROR("Failed to create font for date time control and langBible combo-boxes");
      TRACE0("Could Not create font for combo\n");
    }
    else {
      MSGDEBUG(_T("CMainFrame::_SetFonts - setting fixed font for date time control and langBible combo-boxes"));
      m_wndToolBar.m_comboBox.SetFont(&m_wndToolBar.m_font);
      m_wndToolBar.m_comboBox2.SetFont(&m_wndToolBar.m_font);
      m_DateTime.SetFont(&m_wndToolBar.m_font);
    }
  }
  else {
    MSGDEBUG(_T("CMainFrame::_SetFonts - setting system font for date time control and langBible combo-boxes"));
    m_wndToolBar.m_font.Attach(::GetStockObject(SYSTEM_FONT));
    m_wndToolBar.m_comboBox.SetFont(&m_wndToolBar.m_font);
    m_wndToolBar.m_comboBox2.SetFont(&m_wndToolBar.m_font);
    m_DateTime.SetFont(&m_wndToolBar.m_font);
	}
  m_wndToolBar.GetToolBarCtrl().AutoSize();
}

void CMainFrame::Init(CView &view, IProfile &profile, IAppPathManager &appPathManager)
{
  MSGDEBUG(_T("CMainFrame::Init..."));
  mpView = (CW32losView*) &view;
  mpDoc  = (CW32losDoc*) mpView->GetDocument();

  mpProfile = &profile;
  mpAppPathManager = &appPathManager;

  // register as active view to enable forwarding of OnUpdateCmdUI messages
  SetActiveView(&view, /*bNotify*/ FALSE);
  // 011 LargeDateControlFont
  // HS 2005-11-25: using the toolbar font to set the MonthCalFont.
  {
    bool bSetMonthCalFont = true;

    bSetMonthCalFont = _Profile().GetBool(_T("SetMonthCalFont"), true);
    if (bSetMonthCalFont) {
      m_DateTime.SetMonthCalFont( (HFONT) m_wndToolBar.m_font);
    }
  }

  {
    int nToolBar   = _Profile().GetInt(_T("ToolBar"), 1);
    int nStatusBar = _Profile().GetInt(_T("StatusBar"), -1);
    if (nToolBar   == -1 || !nToolBar  ) m_wndToolBar  .ShowWindow(SW_HIDE);
    if (nStatusBar == -1 || !nStatusBar) m_wndStatusBar.ShowWindow(SW_HIDE);
  }
  MSGDEBUG(_T("CMainFrame::Init done."));
}

void CMainFrame::SetLangComboFocus()
{
  _GetLangCombo1().SetFocus();
}

void CMainFrame::EnableInput(bool bEnable)
{
  MSGDEBUG(_T("CMainFrame::EnableInput: bEnable=") << bEnable);
  _GetLangCombo1().EnableWindow(bEnable);
  _GetLangCombo2().EnableWindow(bEnable);
  m_DateTime.EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Nachrichten-Handler

void CMainFrame::WriteSettings()
{
  MSGDEBUG("CMainFrame::WriteSettings...");
  // HS 2005-11-21: see ->OnClose().
  if (!::IsWindow(m_hWnd)) {
    MSGDEBUG("CMainFrame::WriteSettings !IsWindow");
  }
  else {
    IProfile &profile = _Profile();
    WINDOWPLACEMENT wp;
    GetWindowPlacement(&wp);
    RECT &r = wp.rcNormalPosition;
    profile.WriteInt(_T("Left"),   r.left);
    profile.WriteInt(_T("Top"),    r.top);
    profile.WriteInt(_T("Right"),  r.right);
    profile.WriteInt(_T("Bottom"), r.bottom);
    profile.WriteInt(_T("ShowCmd"),wp.showCmd);
    profile.WriteInt(_T("Flags"),  wp.flags);
    profile.WriteInt(_T("ToolBar"),   (m_wndToolBar.GetStyle() & WS_VISIBLE) != 0);
    profile.WriteInt(_T("StatusBar"), (m_wndStatusBar.GetStyle() & WS_VISIBLE) != 0);
    MSGDEBUG("CMainFrame::WriteSettings saved window settings.");
  }
}

void CMainFrame::OnClose() 
{
  /* -----------------------------------------------------------
  010 WindowPosNotSaved
  HS 2005-11-21: 
  1. Closing window via "X" at top right:
    D: CMainFrame::OnClose
    D: CMainFrame::WriteSettings...
    D: CMainFrame::WriteSettings saved window.
    D: CW32losApp::ExitInstance
    D: CMainFrame::WriteSettings...
    D: CMainFrame::WriteSettings !IsWindow

  2. Windows user logout:
    D: CW32losApp::ExitInstance
    D: CMainFrame::WriteSettings...
    D: CMainFrame::WriteSettings saved window.

  Neither OnDestroy nor OnNcDestroy get called!
  ----------------------------------------------------------- */
  MSGDEBUG("CMainFrame::OnClose");
  WriteSettings();	
	CFrameWnd::OnClose();
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
  UpdateFrameTitle();
}

CTime CMainFrame::_GetDate()
{
  SYSTEMTIME sel; 
  if (GDT_VALID == m_DateTime.GetTime(&sel)) {
    // maybe comparison to GDT_VALID traps creation failure of control
    if (sel.wYear < 70) { // for CTime (aka mktime aka time_t) can hold only >=1970
      sel.wYear += 2000;
    }
    else if (sel.wYear < 100) {
      sel.wYear += 1900;
    }
    // range of time_t: 1970-01-01 .. 2038-01-18
    if (sel.wYear >= 1970 && sel.wYear <= 2037) {
      return CTime(sel);
    }
    else {
      MSGERROR("Getting date from main frame - cannot represent year " << sel.wYear << " in time_t, ignoring.");
    }
  }
  else {
    MSGERROR("Getting date from main frame - date of date control is invalid, ignoring.");
  }

  return CTime::GetCurrentTime();
}

void CMainFrame::OnDTNotify(NMHDR *pHdr, LRESULT * pRes)
{
  switch( pHdr->code)
  {
  case DTN_DATETIMECHANGE:
    GetDoc().SetDate(_GetDate());
    break;
  }
}

afx_msg void CMainFrame::OnSelChangeLangBox()
{
  MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox..."));
  CWnd *pFocusWnd = GetFocus();
  if (!pFocusWnd) {
    MSGERROR("Selection changed in langBible combo-box - GetFocus returns 0, skipping.");
    return;
  }
  int nID = pFocusWnd->GetDlgCtrlID();

  CComboBox &comboBox   = (nID == IDC_MAINBAR_LANGBOX2) ? _GetLangCombo2() : _GetLangCombo1();
  unsigned int tgtIndex = (nID == IDC_MAINBAR_LANGBOX2) ? 1 : 0;
  MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox - tgtIndex=") << tgtIndex);

  int selIndex = comboBox.GetCurSel();
  if (selIndex == CB_ERR) return;

  std::vector<string> langBibles = GetDoc().GetLangBibles();
  ITwdFileDesc* pTwdFileDesc = (ITwdFileDesc*) comboBox.GetItemDataPtr(selIndex);
  if (!pTwdFileDesc) {
    MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox - no fileDesc"));
    // if no data attached, this is the entry for "no langBible", see [NoLangBible2]
    if (langBibles.size() > tgtIndex) {
      MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox - erase index ") << tgtIndex);
      // drop element (if more follow, the next one will be shown in the combobox)
      langBibles.erase(langBibles.begin()+tgtIndex);
    }
  }
  else {
    const string langBible = pTwdFileDesc->GetLangBible();
    MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox - fileDesc ") << langBible);
    if (langBibles.size() <= tgtIndex) {
      // append
      MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox - appending."));
      langBibles.push_back(langBible);
    }
    else {
      if (langBibles[tgtIndex] == langBible) {
        MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox - unchanged."));
        return;
      }
      // replace existing element (if different)
      MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox - replacing."));
      langBibles[tgtIndex] = langBible;
    }
  }
  GetDoc().SetLangBibles(langBibles);
  MSGDEBUG(_T("CMainFrame::OnSelChangeLangBox done."));
}

void CMainFrame::UpdateDateTime()
{
  MSGDEBUG(_T("CMainFrame::UpdateDateTime."));
  CTime docDate = GetDoc().GetDate();
  if (!GetDoc().EqDate(docDate, _GetDate())) {
    m_DateTime.SetTime(&docDate);
  }
}

void CMainFrame::UpdateFrameTitle()
{
  MSGDEBUG(_T("CMainFrame::UpdateFrameTitle."));
  CString title;
  // HS 010630: don't use NLangNames::MakeIsoDisplayName() here,
  //   to avoid "complicated" title (Hanno Priebe complained)
  string langBible = GetDoc().GetFirstLangBible();
  title.Format(IDS_FRAME_TITLE, langBible.c_str());
  SetWindowText(title);
}

void CMainFrame::FillLangbox()
{
  MSGDEBUG(_T("CMainFrame::FillLangbox..."));
  if (!mpLangComboFileDescList) {
    mpLangComboFileDescList = new OTwdFileDescList();
    ASSERT(mpLangComboFileDescList);
  }

  CComboBox &combo1 = _GetLangCombo1();
  CComboBox &combo2 = _GetLangCombo2();
  combo1.ResetContent();
  combo2.ResetContent();

  int year = GetDoc().GetDate().GetYear();
  std::ostringstream os;
  // Set file name pattern for text files!
  // w32los  : de_los07.xml
  // Logoskop: de_Schlachter2000_2008.twd
  os << "_" << year << ".twd";

  ODirectoryFileDescReader reader;
  std::vector<string> filelist; // list of e.g. "de_Schlachter2000"
  if (!reader.Read(_AppPathManager().TwdDir(), /*nameprefix:*/ "", os.str(), filelist)) {
    MSGDEBUG("CMainFrame::FillLangbox: no text file found for driveDir='" 
      << _AppPathManager().TwdDir() << "', no nameprefix, name='" << os.str() << "'");
    // leave error handling to caller (not to langbox)!
    return;
  }

  mpLangComboFileDescList->Clear();

  // get first langBible (to select it in combo)
  const std::vector<string> &langBibles = GetDoc().GetLangBibles();
  const string docLangBible1 = langBibles.empty() ? "" : *langBibles.begin();
  const string docLangBible2 = langBibles.size() < 2 ? "" : langBibles[1];

  // allow second combo to specify "no langBible",
  // with no data attached, cf. [NoLangBible2]
  combo2.AddString(NO_LANGBIBLE2);

  std::vector<string>::const_iterator it;
  string selection1;
  string selection2;
  for (it = filelist.begin(); it != filelist.end(); ++it) {
    // *it e.g. "de_Schlachter2000"
    ITwdFileDesc& desc = NTwdFileDesc::MakeTwdFileDesc(*it, year);
    mpLangComboFileDescList->Add(desc);

    string displayName = desc.GetDisplayName();
    int index1 = combo1.AddString(displayName.c_str());
    combo1.SetItemDataPtr(index1, &desc);
    int index2 = combo2.AddString(displayName.c_str());
    combo2.SetItemDataPtr(index2, &desc);

    if (*it == docLangBible1) {
      selection1 = displayName; // select matching item
    }
    if (*it == docLangBible2) {
      selection2 = displayName; // select matching item
    }
  }

  // Set selection.
  // Note: remembering index + calling combo.SetCurSel() does not work,
  // because sorted combo does not preserve the order of inserted elements!
  if (!selection1.empty()) {
    combo1.SelectString(-1, selection1.c_str());
  }
  combo2.SelectString(-1, selection2.empty() ? NO_LANGBIBLE2 : selection2.c_str());
  MSGDEBUG(_T("CMainFrame::FillLangbox done."));
}

