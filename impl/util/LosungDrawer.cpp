/* ----------------------------------------------------------------------
  Helmut Steeb
  LosungDrawer - draws losung using ILayout
  $Id: LosungDrawer.cpp 3853 2009-08-26 13:50:27Z helmut $

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
#include "LosungDrawer.h"
#include "ProfileDef.h"

#include "../util/ILineDrawer.h"
#include "../util/LineDrawer.h"
#include "../util/ILayout.h"
#include "../LosFile/Losung.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static wstring wlf = L"\r\n";


// === CLosungDrawer ===

CLosungDrawer::CLosungDrawer(ILineDrawer &lineDrawer)
: mLineDrawer(lineDrawer)
{
}

CLosungDrawer::~CLosungDrawer()
{
}

void CLosungDrawer::DrawLosung(CRect &rectClient, CDC &dc, OLosung &losung, const ILosungLayout &losungLayout)
{
  dc.SetBkColor(losungLayout.GetBkCol());
  // the following helps, without it (mode OPAQUE), the character background
  // differs from the window background when using e.g. bright yellow
  // (the text background is darker)
  dc.SetBkMode(TRANSPARENT);
  dc.SetTextAlign(TA_CENTER);

  mLineDrawer.DrawLines(dc, rectClient, losungLayout.LineLayout(ILosungLayout::ltTL), losung.TitleLine().Content() + wlf + wlf);
  
  for (OLosung::OIter it = losung.Begin(); it != losung.End(); ++it) {
    OTestimony &parol = **it;
    if (!parol.IntroLine().IsEmpty()) {
      mLineDrawer.DrawLines(dc, rectClient, losungLayout.LineLayout(ILosungLayout::ltIL), parol.IntroLine().Content());
    }
    mLineDrawer.DrawLines(dc, rectClient, losungLayout.LineLayout(ILosungLayout::ltL), parol.Lines().c_str());
    mLineDrawer.DrawLines(dc, rectClient, losungLayout.LineLayout(ILosungLayout::ltSL), parol.SourceLine().Content() + wlf + wlf);
  }
   
  if (!losung.CommentLines().IsEmpty()) {
    wstring comment = wlf + losung.CommentLines().Content();
    // 2009-08-26 HS - no separate comment line layout any more! IL is best fit.
    mLineDrawer.DrawLines(dc, rectClient, losungLayout.LineLayout(ILosungLayout::ltIL), comment);
  }
}

void CLosungDrawer::DrawText(CRect &rectClient, CDC &dc, const wstring &text, const ILineLayout &lineLayout, COLORREF bkCol)
{
  dc.SetBkColor(bkCol);
  // the following helps, without it (mode OPAQUE), the character background
  // differs from the window background when using e.g. bright yellow
  // (the text background is darker)
  dc.SetBkMode(TRANSPARENT);
  dc.SetTextAlign(TA_CENTER);

  mLineDrawer.DrawLines(dc, rectClient, lineLayout, text);
}


