/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop view
  $Id: w32losView.h 3932 2009-09-28 21:58:25Z helmut $

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

#if !defined(AFX_W32LOSVIEW_H__INCLUDED_)
#define AFX_W32LOSVIEW_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OBidi;
class ILineDrawer;
class CLosungDrawer;
class IProfile;
class ILayout;
class OLayoutDlgCmd;

class CW32losView : public CScrollView
{
protected: // Nur aus Serialisierung erzeugen
	CW32losView();
	DECLARE_DYNCREATE(CW32losView)

public:


// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CW32losView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
  virtual void OnInitialUpdate();
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	protected:
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CW32losView();

  void Init(IProfile &profile);

protected:
  CW32losDoc &_GetDoc() const;
  ILayout &_GetLayout();

  void _CreateMissingLayouts();
  void _ClearLayoutStacks();

  // === Profile ===

  IProfile &_Profile();

  // === Text ===

  bool _HasText() const;
  // _GetText:
  // line-breaks are \n (not \r\n)
  wstring _GetText() const;


// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CW32losView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewFont();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

  afx_msg void OnEditCopy();
  afx_msg void OnEditLayout();
  afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditLayout(CCmdUI* pCmdUI);

  afx_msg void OnViewDayback();
  afx_msg void OnViewDayForth();
  afx_msg void OnViewMonthNext();
  afx_msg void OnViewMonthPrevious();
  afx_msg void OnViewToday();
  afx_msg void OnUpdateViewDayBack(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewDayForth(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewMonthNext(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewMonthPrevious(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewToday(CCmdUI* pCmdUI);

  afx_msg void OnViewXlat();
  afx_msg void OnUpdateViewXlat(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  ILayout *mpLayout;
  bool mbLayoutStacksLocked; // by OnEditLayout to prevent concurrent access from timer
  std::vector<OLayoutDlgCmd*> mUndoStack;
  std::vector<OLayoutDlgCmd*> mRedoStack;

  CLosungDrawer *mpLosungDrawer;
  OBidi *mpBidi;
  ILineDrawer *mpLineDrawer;
  CTime mLastTimerDate;

  // === Profile ===

  IProfile *mpProfile;

  // === Copyright ===

  bool mbIsShowingCopyright;
};


#endif
