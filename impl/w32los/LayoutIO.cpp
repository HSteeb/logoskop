/* ----------------------------------------------------------------------
  Helmut Steeb
  Layout <=> Profile.
  $Id: LayoutIO.cpp 3905 2009-09-12 11:30:16Z helmut $

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

#include "LayoutIO.h"
#include "../util/ILayout.h"
#include "../util/Layout.h"
#include "../util/ProfileDef.h"
#include "../util/IProfile.h"
#include "../util/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SEP (_T(";"))

// === Get V2 (out-dated) ===

void _GetV2FontInfoProfileData(IProfile &profile, ILineLayout &lineLayout, char prefix)
{
  std::string s;
  std::string p = prefix ? string(1, prefix) : "";

  s = p + "FontFace"      ; tstring value;       profile.GetString(s.c_str(), DEFAULT_FONT_NAME, LF_FACESIZE, value);
  s = p + "FontSize"      ; int size           = profile.GetInt(s.c_str(), DEFAULT_POINT_SIZE);
  s = p + "CharSet"       ; int charset        = profile.GetInt(s.c_str(), DEFAULT_CHARSET);
  s = p + "PitchAndFamily"; int pitchAndFamily = profile.GetInt(s.c_str(), DEFAULT_PITCH);
  s = p + "FontColor"     ; int col            = profile.GetInt(s.c_str(), DEFAULT_FONT_COLOUR);
  s = p + "Italic"        ; int italic         = profile.GetInt(s.c_str(), 0);
  s = p + "Underline"     ; int underline      = profile.GetInt(s.c_str(), 0);
  s = p + "Weight"        ; int weight         = profile.GetInt(s.c_str(), FW_NORMAL);

  lineLayout.SetName(value);
  lineLayout.SetSize(size);
  lineLayout.SetCharset(charset);
  lineLayout.SetPitchAndFamily(pitchAndFamily);
  lineLayout.SetCol(col);
  lineLayout.SetItalic(italic);
  lineLayout.SetUnderline(underline);
  lineLayout.SetWeight(weight);
}

DWORD NLayoutIO::GetV2ProfileData(IProfile &profile, ILayout &layout)
{
  MSGDEBUG(_T("NLayoutIO::GetV2ProfileData..."));
  // read profile as of Logoskop v2 (for CFontInfo, pre ILayout)

  std::vector<tstring> langBibles;
  layout.GetLangBibles(langBibles);

  // ensure layout settings for "de" and "en" exist
  // (keep it simple: just add, trap [Duplicates] below)
  langBibles.push_back(_T("de_Schlachter2000"));
  langBibles.push_back(_T("en_EnglishStandardVersion"));

  // read INI settings into layout of all langBibles
  std::vector<tstring>::const_iterator it;
  ILosungLayout *pFirstLosungLayout = 0;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const tstring langBible = *it;
    MSGDEBUG(_T("NLayoutIO::GetV2ProfileData reading for langBible ") << langBible);
    ILosungLayout &losungLayout = layout.LosungLayout(langBible);
    if (!pFirstLosungLayout) {
      MSGINFO(_T("Reading layout settings from INI file of Logoskop version < 3 for ") << langBible);
      // read INI settings into layout of first langBible
      pFirstLosungLayout = &losungLayout;
      _GetV2FontInfoProfileData(profile, losungLayout.LineLayout(ILosungLayout::ltTL), 'H');
      _GetV2FontInfoProfileData(profile, losungLayout.LineLayout(ILosungLayout::ltIL), 'I');
      _GetV2FontInfoProfileData(profile, losungLayout.LineLayout(ILosungLayout::ltL ), '\0');
      _GetV2FontInfoProfileData(profile, losungLayout.LineLayout(ILosungLayout::ltSL), 'S');
      //  rc = _GetV2FontInfoProfileData(losungLayout.LineLayout(ILosungLayout::ltTL, 'C');
    }
    else if (pFirstLosungLayout != &losungLayout) { // trap [Duplicates]
      // clone INI settings from first langBible into a following langBible
      MSGINFO(_T("Copying layout settings from INI file of Logoskop version < 3 for ") << langBible);
      losungLayout.Assign(*pFirstLosungLayout);
    }
  }

  // read BkCol
  int color = profile.GetInt(PRF_BKCOLOUR_V2, DEFAULT_BKCOLOR);
  if (color != DEFAULT_BKCOLOR) {
    layout.SetBkCol(color);
  }
  MSGDEBUG(_T("NLayoutIO::GetV2ProfileData done."));
  return 0;
}

// === Get ===

BOOL _GetLineLayout(IProfile &profile, const tstring &key, ILineLayout &lineLayout)
{
  const int maxSize = 2048;
  tstring sValue;
  if (0 != profile.GetString(key, _T(""), maxSize, sValue, PRF_LAYOUT)) {
    int size;
    COLORREF col;
    int charset;
    int pitchAndFamily;
    int weight;
    int italic;
    int underline;
    int strikeOut;
    TCHAR name[LF_FACESIZE + 100]; 
    if (9 == sscanf(sValue.c_str(), "%d #%06x %d %d %d %d %d %d %[^\n]",
      &size, &col, &charset, &pitchAndFamily, &weight, &italic, &underline, &strikeOut, name)) {
      lineLayout.SetSize(size);
      lineLayout.SetCol(col);
      lineLayout.SetCharset(charset);
      lineLayout.SetPitchAndFamily(pitchAndFamily);
      lineLayout.SetWeight(weight);
      lineLayout.SetItalic(italic);
      lineLayout.SetUnderline(underline);
      lineLayout.SetStrikeOut(strikeOut);
      lineLayout.SetName(name);
    }
  }
  return 1;
}

BOOL _GetLosungLayout(IProfile &profile, const tstring &langBible, ILosungLayout &losungLayout)
{
  tstring prefix(langBible + _T("."));
  _GetLineLayout(profile, prefix + _T("TL"), losungLayout.LineLayout(ILosungLayout::ltTL));
  _GetLineLayout(profile, prefix + _T("IL"), losungLayout.LineLayout(ILosungLayout::ltIL));
  _GetLineLayout(profile, prefix + _T("L"),  losungLayout.LineLayout(ILosungLayout::ltL ));
  _GetLineLayout(profile, prefix + _T("SL"), losungLayout.LineLayout(ILosungLayout::ltSL));
  return 1;
}

DWORD NLayoutIO::GetProfileData(IProfile &profile, ILayout &layout)
{
  MSGDEBUG(_T("NLayoutIO::GetProfileData..."));
  DWORD rc = 0;

  const int maxSize = 2048;
  tstring sLangBibles;
  if (0 != profile.GetString(PRF_LAYOUT_LANGBIBLES, _T(""), maxSize, sLangBibles, PRF_LAYOUT)) {
    std::vector<tstring> langBibles;
    NStringUtil::Split(SEP, sLangBibles, langBibles);
    std::vector<tstring>::const_iterator it;
    for (it = langBibles.begin(); it != langBibles.end(); ++it) {
      const tstring &langBible = *it;
      MSGDEBUG(_T("NLayoutIO::GetProfileData - reading layout for langBible ") << langBible);
      ILosungLayout &losungLayout = layout.LosungLayout(langBible);
      _GetLosungLayout(profile, langBible, losungLayout);
    }
  }
  else {
    MSGDEBUG(_T("NLayoutIO::GetProfileData - no ") << PRF_LAYOUT_LANGBIBLES << _T(" read in section ") << PRF_LAYOUT);
  }
  int color = profile.GetInt(PRF_LAYOUT_BKCOLOUR, DEFAULT_BKCOLOR, PRF_LAYOUT);
  if (color != -1) {
    layout.SetBkCol(color);
  }
  MSGDEBUG(_T("NLayoutIO::GetProfileData done."));
  return rc;
}

// === Write ===

BOOL _WriteLineLayout(IProfile &profile, const tstring &key, const ILineLayout &lineLayout)
{
  TCHAR buf[LF_FACESIZE + 1000]; 
  sprintf(buf, "%d #%06x %d %d %d %d %d %d %s",
    lineLayout.GetSize(),
    lineLayout.GetCol(),
    lineLayout.GetCharset(),
    lineLayout.GetPitchAndFamily(),
    lineLayout.GetWeight(),
    lineLayout.GetItalic(),
    lineLayout.GetUnderline(),
    lineLayout.GetStrikeOut(),
    lineLayout.GetName().c_str());
  profile.WriteString(key, buf, PRF_LAYOUT);
  return 1;
}

BOOL _WriteLosungLayout(IProfile &profile, const tstring &langBible, const ILosungLayout &losungLayout)
{
  tstring prefix(langBible + _T("."));
  _WriteLineLayout(profile, prefix + _T("TL"), losungLayout.LineLayout(ILosungLayout::ltTL));
  _WriteLineLayout(profile, prefix + _T("IL"), losungLayout.LineLayout(ILosungLayout::ltIL));
  _WriteLineLayout(profile, prefix + _T("L"),  losungLayout.LineLayout(ILosungLayout::ltL ));
  _WriteLineLayout(profile, prefix + _T("SL"), losungLayout.LineLayout(ILosungLayout::ltSL));
  return 1;
}

BOOL NLayoutIO::WriteProfileData(IProfile &profile, const ILayout &layout)
{
  MSGDEBUG(_T("NLayoutIO::WriteProfileData..."));
  std::vector<tstring> langBibles;
  layout.GetLangBibles(langBibles);

  tstring sLangBibles = NStringUtil::Join(SEP, langBibles);
  profile.WriteString(PRF_LAYOUT_LANGBIBLES, sLangBibles, PRF_LAYOUT);
  profile.WriteString(PRF_LAYOUT_FORMAT, _T("Size Color Charset PitchAndFamily Weight Italic Underline StrikeOut Name"), PRF_LAYOUT);

  std::vector<tstring>::const_iterator it;
  for (it = langBibles.begin(); it != langBibles.end(); ++it) {
    const tstring &langBible = *it;
    MSGDEBUG(_T("NLayoutIO::WriteProfileData - writing langBible ") << langBible);
    ILosungLayout *pLosungLayout = layout.FindLosungLayout(langBible);
    assert(pLosungLayout);
    _WriteLosungLayout(profile, langBible, *pLosungLayout);
  }
  profile.WriteInt(PRF_LAYOUT_BKCOLOUR, layout.GetBkCol(), PRF_LAYOUT);
  MSGDEBUG(_T("NLayoutIO::WriteProfileData done."));
  return 1;
}
