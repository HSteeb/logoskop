/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop view
  $Id: w32losView.cpp 3932 2009-09-28 21:58:25Z helmut $

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
#include <assert.h>

#include "w32losDoc.h"
#include "LosDoc.h"
#include "MainFrm.h"
#include "w32losView.h"
#include "Resource.h"
#include "../util/Layout.h"
#include "LayoutDlg.h"
#include "LayoutDlgCmd.h"
#include "LayoutController.h"
#include "LayoutIO.h"
#include "../util/ProfileDef.h"
#include "../util/IProfile.h"
#include "../util/Clipboard.h"
#include "../LosFile/LosFile.h"
#include "../LosFile/Losung.h"
#include "../util/LineDrawer.h"
#include "../util/LosungDrawer.h"
#include "../util/utf8.h"
#include "../util/DateUtil.h"
#include "../util/FileUtil.h"
#include "../encoding/Bidi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static wstring wlf = L"\n"; // \r added by code


/////////////////////////////////////////////////////////////////////////////
// CW32losView

IMPLEMENT_DYNCREATE(CW32losView, CScrollView)

BEGIN_MESSAGE_MAP(CW32losView, CScrollView)
	//{{AFX_MSG_MAP(CW32losView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()

  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_COMMAND(ID_EDIT_LAYOUT, OnEditLayout)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
  ON_UPDATE_COMMAND_UI(ID_EDIT_LAYOUT, OnUpdateEditLayout)

  ON_COMMAND(ID_VIEW_DAYBACK, OnViewDayback)
  ON_COMMAND(ID_VIEW_DAYFORTH, OnViewDayForth)
  ON_COMMAND(ID_VIEW_MONTHNEXT, OnViewMonthNext)
  ON_COMMAND(ID_VIEW_MONTHPREVIOUS, OnViewMonthPrevious)
  ON_COMMAND(ID_VIEW_TODAY, OnViewToday)
  ON_UPDATE_COMMAND_UI(ID_VIEW_DAYBACK, OnUpdateViewDayBack)
  ON_UPDATE_COMMAND_UI(ID_VIEW_DAYFORTH, OnUpdateViewDayForth)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MONTHNEXT, OnUpdateViewMonthNext)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MONTHPREVIOUS, OnUpdateViewMonthPrevious)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TODAY, OnUpdateViewToday)

  ON_COMMAND(ID_VIEW_XLAT, OnViewXlat)
  ON_UPDATE_COMMAND_UI(ID_VIEW_XLAT, OnUpdateViewXlat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CW32losView Konstruktion/Destruktion

CW32losView::CW32losView()
{
  MSGDEBUG("CW32losView construction...");
  mpLayout = &NLayout::MakeLayout();
  mpProfile = 0;
  mbIsShowingCopyright = false;

  mpBidi = NBidi::CreateBidi(OBidi::unrestricted);// unrestricted for now (HS 001007/2003-08-16)
  if (!mpBidi) {
    MSGERROR("CW32losView: CreateBidi failed");
  }
  mpLineDrawer = new CLineDrawer(*mpBidi);
  mpLosungDrawer = new CLosungDrawer(*mpLineDrawer);
  MSGDEBUG("CW32losView construction done.");
}

CW32losView::~CW32losView()
{
  MSGDEBUG("CW32losView destruction...");
  delete mpLosungDrawer;
  delete mpLineDrawer;
  delete mpBidi;
  _ClearLayoutStacks();
  delete mpLayout;
  MSGDEBUG("CW32losView destruction done.");
}

int CW32losView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
  mLastTimerDate = CTime::GetCurrentTime(); // initialize first!
  const UINT nElapse = 1000 * 60; // in milliseconds
  MSGDEBUG(_T("CW32losView::OnCreate - creating timer for every ") << nElapse << _T(" ms"));
	SetTimer(1, nElapse, NULL);
	return 0;
}

void CW32losView::Init(IProfile &profile)
{
  MSGDEBUG("CW32losView Init...");
  mpProfile = &profile;

  OnInitialUpdate(); // initialize scrollbar (avoid ViewScrl.cpp "Error: must call SetScrollSizes() or SetScaleToFitSize()...")
  MSGDEBUG("CW32losView Init done.");
}

void CW32losView::OnDestroy() 
{
  MSGDEBUG(_T("CW32losView::OnDestroy - killing timer."));
  KillTimer(1);
  CScrollView::OnDestroy();
}

void CW32losView::_ClearLayoutStacks() 
{
  MSGDEBUG(_T("CW32losView::_ClearLayoutStacks - clearing layout undo/redo history."));
  NLayoutDlgCmd::ClearStack(mUndoStack);
  NLayoutDlgCmd::ClearStack(mRedoStack);
}

CW32losDoc &CW32losView::_GetDoc() const
{
  return *((CW32losDoc*) GetDocument());
}

ILayout &CW32losView::_GetLayout()
{
  return *mpLayout;
}

void CW32losView::OnEditLayout() 
{
  MSGDEBUG("CW32losView OnEditLayout...");
  const std::vector<string> &langBibles = _GetDoc().GetLangBibles();
  if (_GetLayout().HasLangBibles() && !langBibles.empty()) {
    OLayoutDlg dlg;
    _GetDoc().SetLocked(true); // *** must reset!
    OLayoutController ctrl(_Profile(), dlg, langBibles, _GetLayout(), mUndoStack, mRedoStack, this);
    dlg.SetLayoutController(ctrl);

    ILayout *pLayoutCopy = &NLayout::MakeLayout();
    pLayoutCopy->Assign(_GetLayout());

    if (IDOK == dlg.DoModal()) {
      NLayoutIO::WriteProfileData(_Profile(), _GetLayout());
    }
    else {
      _GetLayout().Assign(*pLayoutCopy);
      // For [Cancel], 
      // - drive undo stack down or up to minimum position where user modified data
      unsigned int minExecUndoStackLen = ctrl.GetMinExecUndoStackLen();
      while (mUndoStack.size() > minExecUndoStackLen) {
        OLayoutDlgCmd *pLayoutDlgCmd = mUndoStack.back();
        assert(pLayoutDlgCmd);
        mUndoStack.pop_back();
        mRedoStack.push_back(pLayoutDlgCmd);
      }
      while (mUndoStack.size() < minExecUndoStackLen && !mRedoStack.empty()) {
        OLayoutDlgCmd *pLayoutDlgCmd = mRedoStack.back();
        assert(pLayoutDlgCmd);
        mRedoStack.pop_back();
        mUndoStack.push_back(pLayoutDlgCmd);
      }
      Invalidate(); // force final redraw for area under dialog (?!)
    }
    _GetDoc().SetLocked(false);
    delete pLayoutCopy;
  }
  else {
    MSGINFO(_T("No layout langBibles or no doc langBibles, skipping layout dialog."));
  }
  MSGDEBUG("CW32losView OnEditLayout done.");
}



// === Profile ===

IProfile &CW32losView::_Profile()
{
  ASSERT(mpProfile);
  return *mpProfile;
}




/////////////////////////////////////////////////////////////////////////////
// CW32losView Zeichnen

wstring ID2WString(int id)
{
  CString cstr;
  cstr.LoadString(id);
  wstring res(NUTF8::ToUTF16((LPCSTR) cstr));
  return res;
}


wstring a2w(CString &member) 
{
#define BUFLEN 65535
  wchar_t buffer[BUFLEN];
  int length = member.GetLength();
  assert(length < BUFLEN);
  int res = _mbstowcsz(buffer, member, BUFLEN);
  if (res >= length) {
    return wstring(buffer);
  }
  else if (!res) {
    DWORD err = GetLastError();
    MSGERROR("Error " << err << " from mbstowcsz for " << member);
  }
  return L"";
}


void CW32losView::OnDraw(CDC* pDC)
{
  CW32losDoc &doc = _GetDoc();
  const CTime &date = doc.GetDate();

  // 2009-08-01 HS: no more handle "UseUSP10" specific to langBible (how to control the setting?) See [UseUSP10]
  // Must check this for each OnDraw() - use may have set UseUSP10 for the first time now.
  bool bUseUSP10 = _Profile().GetBool(PRF_USE_USP10, DEFAULT_USE_USP10);
  mpLineDrawer->UseUSP10(bUseUSP10);
  if (bUseUSP10 && !mpLineDrawer->UseUSP10()) {
    MSGINFO("CW32losView::OnDraw: reset UseUSP10 since LineDrawer cannot use it.");
    _Profile().WriteBool(PRF_USE_USP10, false);
    bUseUSP10 = false;
  }
  MSGDEBUG("CW32losView::OnDraw: UseUSP10=" << mpLineDrawer->UseUSP10());

  CRect	rectClient;
  GetClientRect(&rectClient);

  // setup mpLineDrawer
  mpLineDrawer->Start(12); // start position vertical

  int nParols = 0;
  int nLineSize = 0;
  COLORREF bkCol = _GetLayout().GetBkCol();
  const std::vector<string> &langBibles = doc.GetLangBibles();
  std::vector<std::pair<LONG, COLORREF> > separatorYPositions;
  if (langBibles.empty()) {
    MSGDEBUG("CW32losView::OnDraw - langBibles empty");
    CString buf;
    buf.Format(IDS_FILEREAD_ERR, date.GetYear(), date.GetMonth(), date.GetDay());
    buf += "\n";

    ILineLayout *pLineLayout = 0;
    ILineLayout *pLineLayoutToDelete = 0;

    // try to get layout from first langBible in _GetLayout()
    std::vector<tstring> layoutLangBibles;
    _GetLayout().GetLangBibles(layoutLangBibles);
    std::vector<string>::const_iterator it = layoutLangBibles.begin();
    if (it != layoutLangBibles.end()) {
      const string &langBible = *it;
      ILosungLayout &losungLayout = _GetLayout().LosungLayout(langBible);
      pLineLayout = &losungLayout.LineLayout(ILosungLayout::ltL);
    }
    else  {
      pLineLayout = pLineLayoutToDelete = &NLayout::MakeLineLayout();
    }

    mpLosungDrawer->DrawText(rectClient, *pDC, a2w(buf), *pLineLayout, bkCol);
    delete pLineLayoutToDelete;
    nParols = 1;
  }
  else {
    MSGDEBUG("CW32losView::OnDraw - processing " << langBibles.size() << " langBibles...");
    std::vector<string>::const_iterator it;
    for (it = langBibles.begin(); it != langBibles.end(); ++it) {
      const string &langBible = *it;
      OLosDoc *pLosDoc = doc.GetLosDocForLangBible(langBible);
      if (!pLosDoc) {
        MSGDEBUG("CW32losView::OnDraw - continuing, no losDoc for langBible " << langBible);
        continue;
      }
      if (!pLosDoc->HasCurrentLosung()) {
        MSGDEBUG("CW32losView::OnDraw - continuing, !HasCurrentLosung for langBible " << langBible);
        continue;
      }
      ++nParols;

      ILosungLayout *pLosungLayout = _GetLayout().FindLosungLayout(langBible);
      if (!pLosungLayout) {
        MSGERROR("OnDraw - no layout for langBible " << langBible << ", creating + setting nice defaults.");
        // Could assert here, or just continue.
        // But be tolerant, just create the layout (but not saved to profile)
        pLosungLayout = &_GetLayout().LosungLayout(langBible); // see [NeedLayout]
        pLosungLayout->SetDefaults(); // set nice defaults

      }
      const ILineLayout &lineLayout = pLosungLayout->LineLayout(ILosungLayout::ltL);
      if (!nLineSize) {
        // use first size
        nLineSize = lineLayout.GetSize();
      }

      if (nParols > 1) {
        mpLineDrawer->IncY(10);
        // remember y pos for horizontal separator
        separatorYPositions.push_back(std::pair<LONG, COLORREF>(mpLineDrawer->GetY() - 10, lineLayout.GetCol()));
      }

      if (mbIsShowingCopyright) {
        MSGDEBUG("CW32losView::OnDraw - showing copyright for " << langBible);
        wstring text(ID2WString(IDS_XLAT_USED) + wlf
          // See [copyright]
          + pLosDoc->GetCopyright() + wlf 
          + L"(" + pLosDoc->GetFileVersion() + L")"
          // HS 2003-08-16: trailing wlf helps for bottom border, + for horizontal line
          + wlf + wlf); 
        mpLosungDrawer->DrawText(rectClient, *pDC, text, lineLayout, bkCol);
      }
      else {
        OLosung &losung = pLosDoc->CurrentLosung();
        MSGDEBUG("CW32losView::OnDraw - DrawLosung for " << langBible);
        mpLosungDrawer->DrawLosung(rectClient, *pDC, losung, *pLosungLayout);
      }
    }
    if (!nParols) nParols = 1; // avoid division by zero
  }

  // Check whether an error occurred in LineDrawer!
  // Do this here on exit of OnDraw, not only on next entry of OnDraw
  // so that UseUSP10 is really turned off, even in case no next OnDraw occurs
  // (the wrong value of UseUSP10 is visible to the user if he enters the options dialog)
  if (bUseUSP10 && !mpLineDrawer->UseUSP10()) {
    MSGINFO("CW32losView::OnDraw: reset " PRF_USE_USP10 " in INI file since CLineDrawer turned it off.");
    _Profile().WriteBool(PRF_USE_USP10, false);
  }

  long newY = mpLineDrawer->GetY();
  // HS 2003-12-20: add "20" for having small border!
  long newX = mpLineDrawer->GetMaxLen() + 20;

  std::vector<std::pair<LONG, COLORREF> >::const_iterator sepIt;
  MSGDEBUG("CW32losView::OnDraw - drawing " << separatorYPositions.size() << " separators");
  for (sepIt = separatorYPositions.begin(); sepIt != separatorYPositions.end(); ++sepIt) {
    LONG sepYPos = sepIt->first;
    COLORREF col = sepIt->second;
    CRect Rect(rectClient.left, sepYPos, rectClient.right, sepYPos+1);
    CBrush Brush(col);
    pDC->FillRect(&Rect, &Brush);
  }

  //  GetParentFrame()->RecalcLayout();
  //  SetScaleToFitSize(size);
  SetScrollSizes(MM_TEXT, 
    CSize(newX, newY), // total
    CSize(newX, newY / nParols), // page
    CSize(nLineSize/10, nLineSize/10)); // line

  WINDOWPLACEMENT wp;
  GetParentFrame()->GetWindowPlacement(&wp);
  MSGDEBUG("CW32losView::OnDraw - window placement showCmd=" << wp.showCmd);
  /* wp.showCmd see VC98\Include\WINUSER.H:
    #define SW_HIDE             0
    #define SW_SHOWNORMAL       1
    #define SW_NORMAL           1
    #define SW_SHOWMINIMIZED    2
    #define SW_SHOWMAXIMIZED    3
    #define SW_MAXIMIZE         3
    #define SW_SHOWNOACTIVATE   4
    #define SW_SHOW             5
    #define SW_MINIMIZE         6
    #define SW_SHOWMINNOACTIVE  7
    #define SW_SHOWNA           8
    #define SW_RESTORE          9
    #define SW_SHOWDEFAULT      10
    #define SW_FORCEMINIMIZE    11
  */
  if (wp.showCmd == SW_RESTORE || wp.showCmd == SW_SHOWNORMAL) {
    CRect rectWorkArea;
    if (0 != SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0)) {
      bool bDoSet = false;
      int frameHeight = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top - rectClient.Height();
      if (newY - rectClient.Height() <= rectWorkArea.bottom - wp.rcNormalPosition.bottom) {
        // exactly adjust height
        wp.rcNormalPosition.bottom += newY - rectClient.Height();
        bDoSet = true;
      }

      // extend width only if not wide enough
      // (adjusting width exactly results in flickering
      //  window when changing the day displayed)
      if (rectClient.right < newX) {
        wp.rcNormalPosition.right += newX - rectClient.right;
        bDoSet = true;
      }

      if (bDoSet) {
        MSGDEBUG("CW32losView::OnDraw - adapting frame size to "
          << wp.rcNormalPosition.left  << ", " << wp.rcNormalPosition.top << ", "
          << wp.rcNormalPosition.right << ", " << wp.rcNormalPosition.bottom);
        GetParentFrame()->SetWindowPlacement(&wp);
      }
    }
    else {
      MSGERROR("CW32losView::OnDraw - cannot adapt frame size due to error " << GetLastError() << " from SystemParametersInfo(SPI_GETWORKAREA...)");
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// CW32losView Nachrichten-Handler

// 2009-07-23 HS: OnInitialUpdate not called if mpFrame->InitialUpdateFrame bMakeVisible parameter is false!
void CW32losView::OnInitialUpdate() 
{
  MSGDEBUG("CW32losView::OnInitialUpdate...");
  //  GetParentFrame()->RecalcLayout();
  //  SetScaleToFitSize(size);

  // avoid error in CScrollView::OnPrepareDC:
  //    TRACE0("Error: must call SetScrollSizes() or SetScaleToFitSize()");
  //    TRACE0("\tbefore painting scroll view.\n");
  //    ASSERT(FALSE);
  SetScrollSizes(MM_TEXT, CSize(100, 100));
	CScrollView::OnInitialUpdate();
  MSGDEBUG("CW32losView::OnInitialUpdate done.");
}

void CW32losView::_CreateMissingLayouts() 
{
  MSGDEBUG("CW32losView::_CreateMissingLayouts...");
  const std::vector<tstring> &langBibles = _GetDoc().GetLangBibles();
  std::vector<tstring> langBiblesNoLayout;
  tstring langBibleForCopy;
  ILosungLayout *pLosungLayoutForCopy = 0;
  std::vector<tstring>::const_iterator it;

  // remember langBibles that have no layout, + layout of first langBible that has layout
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    ILosungLayout *pLosungLayout = _GetLayout().FindLosungLayout(*it);
    if (pLosungLayout) {
      if (!pLosungLayoutForCopy) {
        // remember layout of first langBible that has layout
        pLosungLayoutForCopy = pLosungLayout;
        langBibleForCopy = *it;
      }
    }
    else {
      // remember langBibles that have no layout
      langBiblesNoLayout.push_back(*it);
    }
  }

  // create missing layouts
  if (!langBiblesNoLayout.empty()) {
    for (it = langBiblesNoLayout.begin(); it != langBiblesNoLayout.end(); ++it) {
      // create layout for langBible, if it does not yet exist, cf. [NeedLayout]
      MSGINFO(_T("Creating missing layouts - layout for ") << (*it) << _T(" taken from ") << langBibleForCopy);
      ILosungLayout &layout = _GetLayout().LosungLayout(*it);
      if (pLosungLayoutForCopy) {
        // copy missing layout from layout of first langBible that has layout
        layout.Assign(*pLosungLayoutForCopy);
      }
      else {
        layout.SetDefaults(); // set nice defaults
      }
    }
    // ensure settings are remembered in profile - write now
    NLayoutIO::WriteProfileData(_Profile(), _GetLayout());
  }
  MSGDEBUG("CW32losView::_CreateMissingLayouts done.");
}

void CW32losView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  MSGDEBUG("CW32losView::OnUpdate...");

  bool bUpdateDateTime   = 0 != (lHint &  NW32losDoc::docDateChanged);
  bool bUpdateFrameTitle = 0 != (lHint &  NW32losDoc::docLangBiblesChanged);
  bool bFillLangbox      = 0 != (lHint & (NW32losDoc::docYearChanged | NW32losDoc::docLangBiblesChanged));
  bool bUpdateLayouts    = 0 != (lHint &  NW32losDoc::docLangBiblesChanged);

  CMainFrame *pFrameWnd = (CMainFrame*) GetParentFrame();

  if (pFrameWnd) {
    if (bUpdateDateTime) {
      pFrameWnd->UpdateDateTime();
    }

    if (bFillLangbox) {
      pFrameWnd->FillLangbox();
    }

    // assume frame contains doclang - update it
    if (bUpdateFrameTitle) {
      pFrameWnd->UpdateFrameTitle();
    }
  }

  if (bUpdateLayouts) {
    tstring lastRunVersion;
    if (0 < _Profile().GetString(PRF_LAST_RUN_VERSION, "", /*max*/64, lastRunVersion)
      && lastRunVersion >= _T("3")) {
      NLayoutIO::GetProfileData(_Profile(), _GetLayout());
    }
    else {
      // create langBibles in Layout for all languages in Doc
      NLayoutIO::GetV2ProfileData(_Profile(), _GetLayout());
    }
    _CreateMissingLayouts();
  }

  CScrollView::OnUpdate(pSender, lHint, pHint);
  MSGDEBUG("CW32losView::OnUpdate done.");
}

void CW32losView::OnViewDayback() 
{
  const CTime &date = _GetDoc().GetDate();
  int d = date.GetDay();
  int m = date.GetMonth();
  int y = date.GetYear();
  if (0 == DateUtil::MoveDay(&y, &m, &d, -1)) {
    _GetDoc().SetDate(CTime(y, m, d, 0, 0, 0)); // h:m:s == 0:0:0 is ok because we use EqDate()
  }
}

void CW32losView::OnViewDayForth() 
{
  const CTime &date = _GetDoc().GetDate();
  int d = date.GetDay();
  int m = date.GetMonth();
  int y = date.GetYear();
  if (0 == DateUtil::MoveDay(&y, &m, &d, +1)) {
    _GetDoc().SetDate(CTime(y, m, d, 0, 0, 0)); // h:m:s == 0:0:0 is ok because we use EqDate()
  }
}

void CW32losView::OnViewMonthPrevious() 
{
  const CTime &date = _GetDoc().GetDate();
  int d = date.GetDay();
  int m = date.GetMonth();
  int y = date.GetYear();
  if (0 == DateUtil::MoveMonth(&y, &m, &d, -1)) {
    _GetDoc().SetDate(CTime(y, m, d, 0, 0, 0)); // h:m:s == 0:0:0 is ok because we use EqDate()
  }
}

void CW32losView::OnViewMonthNext() 
{
  const CTime &date = _GetDoc().GetDate();
  int d = date.GetDay();
  int m = date.GetMonth();
  int y = date.GetYear();
  if (0 == DateUtil::MoveMonth(&y, &m, &d, +1)) {
    _GetDoc().SetDate(CTime(y, m, d, 0, 0, 0)); // h:m:s == 0:0:0 is ok because we use EqDate()
  }
}


void CW32losView::OnViewToday() 
{
  if (mbIsShowingCopyright) {
    OnViewXlat();
  }
  else {
    _GetDoc().SetDate(CTime::GetCurrentTime());
  }
}

void CW32losView::OnUpdateViewDayBack(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!mbIsShowingCopyright);
}

void CW32losView::OnUpdateViewDayForth(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!mbIsShowingCopyright);
}

void CW32losView::OnUpdateViewMonthPrevious(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!mbIsShowingCopyright);
}

void CW32losView::OnUpdateViewMonthNext(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!mbIsShowingCopyright);
}

void CW32losView::OnUpdateViewToday(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(mbIsShowingCopyright || !CW32losDoc::EqDate(_GetDoc().GetDate(), CTime::GetCurrentTime()));
}

void CW32losView::OnUpdateEditLayout(CCmdUI* pCmdUI) 
{
  const std::vector<string> &langBibles = _GetDoc().GetLangBibles();
  pCmdUI->Enable(_GetLayout().HasLangBibles() && !langBibles.empty());
}

void CW32losView::OnViewXlat() 
{
  MSGDEBUG(_T("CW32losView::OnViewXlat: mbIsShowingCopyright=") << mbIsShowingCopyright);
  mbIsShowingCopyright = !mbIsShowingCopyright;
  CMainFrame *pFrameWnd = (CMainFrame*) GetParentFrame();
  if (pFrameWnd) {
    pFrameWnd->EnableInput(!mbIsShowingCopyright);
  }
  Invalidate();
}

void CW32losView::OnUpdateViewXlat(CCmdUI* pCmdUI) 
{
  if (!_GetDoc().GetLangBibles().empty()) {
    pCmdUI->Enable(true);
    pCmdUI->SetCheck(mbIsShowingCopyright);
  }
  else {
    pCmdUI->SetCheck(false);
    pCmdUI->Enable(false);  
  }
}

void CW32losView::OnTimer(UINT nIDEvent) 
{
  // avoid unnecessary noise each minute: MSGDEBUG(_T("CW32losView::OnTimer"));
  // timer works on CWnd - not possible in App
  const CTime now = CTime::GetCurrentTime();
  // If doc showed current day at the time of the last check,
  // and now we're at a different day,
  // switch day in doc
  CW32losDoc &doc = _GetDoc();
  if (!CW32losDoc::EqDate(mLastTimerDate, now) 
    && CW32losDoc::EqDate(mLastTimerDate, doc.GetDate())
    // Timer is called even if modal dialog is open (via e.g. OnLayoutEdit)!
    // Prevent from modifying langBible set in document 
    //   while open dialog has "locked" the document!
    && !doc.IsLocked()) {
    MSGDEBUG(_T("CW32losView::OnTimer... (date is different from date of last timer event)"));

    MSGDEBUG(_T("CW32losView::OnTimer - updating document date"));
    doc.SetDate(now);

    // some guys keep their computers running all their life
    // => clear layout stacks at the end of the day at least.
    MSGDEBUG(_T("CW32losView::OnTimer - clearing layout undo/redo history once a day"));
    _ClearLayoutStacks();

    MSGDEBUG(_T("CW32losView::OnTimer done."));
  }
  mLastTimerDate = now;

	CScrollView::OnTimer(nIDEvent);
}



// ----------------

string upper(const string &s)
{
  string res;
  for (const char *p = s.c_str(); p && *p; ++p) {
    char ch = *p;
    if (ch >= 'a' && ch <= 'z') ch -= ('a' - 'A');
    res.append(1, ch);
  }
  return res;
}


BOOL CW32losView::OnEraseBkgnd(CDC* pDC) 
{
  MSGDEBUG(_T("CW32losView::OnEraseBkgnd."));
  assert(pDC);
  CDC &dc = *pDC;
  COLORREF col = _GetLayout().GetBkCol();
	CBrush Brush(col);
  Brush.UnrealizeObject();
  CBrush *pOldBrush = dc.SelectObject(&Brush);
  dc.SetBkColor(col);
  RECT Rect;
  GetClientRect(&Rect);
  dc.FillRect(&Rect, &Brush);
  dc.SelectObject(pOldBrush);
  return 1;
}

// === Text ===

bool CW32losView::_HasText() const
{
  const std::vector<string> &langBibles = _GetDoc().GetLangBibles();
  std::vector<string>::const_iterator it;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const string &langBible = *it;
    const OLosDoc *pLosDoc = _GetDoc().GetLosDocForLangBible(langBible);
    if (pLosDoc && (mbIsShowingCopyright || pLosDoc->HasCurrentLosung())) {
      return true;
    }
  }
  return false;
}

wstring CW32losView::_GetText() const
{
  wstring text;
  const std::vector<string> &langBibles = _GetDoc().GetLangBibles();
  std::vector<string>::const_iterator it;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const string &langBible = *it;
    const OLosDoc *pLosDoc = _GetDoc().GetLosDocForLangBible(langBible);
    if (pLosDoc) {
      wstring s;
      if (mbIsShowingCopyright) {
        // See [copyright]
        s = pLosDoc->GetCopyright() + wlf 
          + L"(" + pLosDoc->GetFileVersion() + L")";
      }
      else if (pLosDoc->HasCurrentLosung()) {
        s = pLosDoc->GetText();
      }
      if (!s.empty()) {
        if (!text.empty()) {
          text += wlf + wlf + wlf; // separator (first wlf terminates typical copyright paragraph)
        }
        text += s;
      }
    }
  }
  return text;
}

void CW32losView::OnEditCopy() 
{
  bool bUsedDefaultChar;
  // convert \n to \r\n (\n alone is not shown correctly e.g. in Notepad)
  wstring text = FileUtil::MoronizeW(_GetText());
  if (!NClipboard::Copy(text, &bUsedDefaultChar)) {
    AfxMessageBox(IDS_CLIPBOARDERR);
  }
  else if (bUsedDefaultChar) {
    // 2009-08-28 HS: no more show annoying message box.
    MSGDEBUG("CW32losView::OnEditCopy: not all characters can be displayed in the current codepage! "
      "The text is now in the clipboard in two formats: "
      "in your current codepage and in Unicode format. "
      "A Unicode aware application may use it correctly.");
  }
}

void CW32losView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(_HasText());
}

