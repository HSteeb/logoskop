/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop profile defines
  $Id: ProfileDef.h 3905 2009-09-12 11:30:16Z helmut $

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

#if !defined(AFX_PROFILEDEF_H__INCLUDED_)
#define AFX_PROFILEDEF_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define PRF_GENERAL                   _T("General")

#define PRF_DIALOGLANG                _T("DialogLang")
#define PRF_DOCLANG_V2                _T("Lang")
#define PRF_LANGBIBLES                _T("LangBibles")
#define PRF_LOGFILE                   _T("Logfile")
#define PRF_LOGFILE_LEVEL             _T("LogfileLevel")
#define PRF_PARSE_STRICT              _T("ParseStrict")
#define PRF_USE_USP10                 _T("UseUSP10")
#define PRF_START_ONCE                _T("StartOnce")
#define PRF_LAST_START                _T("LastStart")
#define PRF_LAST_LANG_INFO            _T("LastLangInfo")
#define PRF_MAX_YEAR_INSTALLED        _T("MaxYearInstalled")
#define PRF_LAST_RUN_VERSION          _T("LastRunVersion")
#define PRF_SIGNATURE_AS_UTF8         _T("SignatureAsUtf8")
#define PRF_SIGNATURE_ON              _T("SignatureOn")
#define PRF_SIGNATURE_LINESEP         _T("SignatureLineSeparator")
#define PRF_SIGNATURE_TARGET          _T("SignatureTarget")
#define PRF_SIGNATURE_TEMPLATE        _T("SignatureTemplate")
#define PRF_BKCOLOUR_V2               _T("BkColor") // out-dated (v2)
#define PRF_LAYOUT                    _T("Layout")
#define PRF_LAYOUT_FORMAT             _T("Format")
#define PRF_LAYOUT_LANGBIBLES         _T("LangBibles")
#define PRF_LAYOUT_BKCOLOUR           _T("BkColor")
#define PRF_LAYOUT_ALL_LANGBIBLES     _T("LayoutAllLangBibles")
#define PRF_BASEURL                   _T("BaseURL")
#define PRF_BASEPOSTPAIRURL           _T("BasePostPairURL")
#define PRF_HOMEPAGEURL               _T("HomepageURL")
#define PRF_TIMEOUT                   _T("Timeout")
#define PRF_USEPROXY                  _T("UseProxy")
#define PRF_PROXY                     _T("Proxy")
#define PRF_NEXTYEAR_QUERIED          _T("NextYearQueried")
#define PRF_AUTOLANGPACKDLG           _T("AutoLangPackDlg")
#define PRF_AUTOLANGPACKDLGON         _T("AutoLangPackDlgOn")


#define DEFAULT_LOGFILE_LEVEL             2
#define DEFAULT_PARSE_STRICT              false
// HS 2006-01-06: DEFAULT_USE_USP10 was true in 4.0.7, but must be false (as stated in ENreadme.htm)!
// HS 2006-01-16: DEFAULT_USE_USP10 must be true,
//   e.g. Tamil is not displayed correctly otherwise (ligature and alternative glyphs are not correct).
#define DEFAULT_USE_USP10                 true
#define DEFAULT_START_ONCE                false
#define DEFAULT_SIGNATURE_ON              false
#define DEFAULT_SIGNATURE_TARGET          _T("Signature.txt")
#define DEFAULT_SIGNATURE_TEMPLATE        _T("SignatureTemplate.txt")
#define DEFAULT_SIGNATURE_AS_UTF8         false
#define DEFAULT_LAYOUT_ALL_LANGBIBLES     true
// HS 2005-01-06: include 'he' in DEFAULT_LANGSPECFONTS
#define DEFAULT_FONT_NAME                 _T("Arial Unicode MS")
#define DEFAULT_POINT_SIZE                120
#define DEFAULT_POINT_SIZE_TL             120
#define DEFAULT_POINT_SIZE_IL             110
#define DEFAULT_POINT_SIZE_L              140
#define DEFAULT_POINT_SIZE_SL             110
#define DEFAULT_FONT_COLOUR               0x008000 // green
#define DEFAULT_FONT_COLOUR_TL            0x008000
#define DEFAULT_FONT_COLOUR_IL            0x008000
#define DEFAULT_FONT_COLOUR_L             0x008000
#define DEFAULT_FONT_COLOUR_SL            0x008000
#define DEFAULT_BKCOLOR                   (0xffffff) 
#define DEFAULT_BASEURL                   _T("http://bible2.net/service/TheWord/twd11/?format=csv")
#define DEFAULT_BASEPOSTPAIRURL           _T("http://bible2.net/service/Pair/Suggestion/List")
#define DEFAULT_HOMEPAGEURL               _T("http://bible2.net/")
#define DEFAULT_TIMEOUT                   15000 // in msec
#define DEFAULT_USEPROXY                  "P" // cf. tUseProxy in online/ProxySession.h!
#define DEFAULT_PROXY                     ""
#define DEFAULT_AUTOLANGPACKDLGON         true

#endif
