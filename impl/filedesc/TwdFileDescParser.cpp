/* ----------------------------------------------------------------------
  Helmut Steeb 
  Parse twd file list (from server) into FileDescList.

  $Id: TwdFileDescParser.cpp 3014 2008-11-29 10:19:16Z helmut $

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
---------------------------------------------------------------------- */

#include "stdafx.h"
#include "../util/StringUtil.h"
#include "../util/utf8.h"
#include "TwdFileDesc.h"
#include "TwdFileDescList.h"
#include "TwdInfo.h"
#include "TwdInfoList.h"
#include "TwdFileDescParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CATEGORY "category"
#define LANG  "lang"
#define BIBLE "bible"
#define YEAR  "year"
#define UPDATED "updated"
#define INFO  "info"
#define URL   "url"


OTwdFileDescParser::~OTwdFileDescParser()
{
}

void OTwdFileDescParser::Parse(const string& lines, OTwdFileDescList& fileList, OTwdInfoList& infoList)
{
  fileList.Clear();
  infoList.Clear();

  // parse file list: split into lines
  std::vector<tstring> Lines;
  NStringUtil::Split("\n", lines, Lines);

  std::vector<tstring>::const_iterator lineIter = Lines.begin();
  if (lineIter != Lines.end()) {
    const string& header = *lineIter;
    std::vector<string> Cells;
    NStringUtil::Split(";", header, Cells);
    
    // compute indexes of fields we need
    int iCategory = -1;
    int iLang = -1;
    int iBible = -1;
    int iYear = -1;
    int iUpdated = -1;
    int iInfo = -1;
    int iUrl = -1;
    for (unsigned int i = 0; i < Cells.size(); ++i) {
      const tstring& cell = Cells.at(i);
      if (cell == CATEGORY)     {  iCategory = i;  }
      else if (cell == LANG)    {  iLang = i;      }
      else if (cell == BIBLE)   {  iBible = i;     }
      else if (cell == YEAR)    {  iYear = i;      }
      else if (cell == UPDATED) {  iUpdated = i;   }
      else if (cell == INFO)    {  iInfo = i;      }
      else if (cell == URL)     {  iUrl = i;       }
    }
    // check that all required column names have been found
    int iMin = min(iCategory, min(iLang, min(iBible, min(iYear, min(iUpdated, min(iInfo, iUrl))))));
    if (iMin < 0) {
      MSGERROR("Splitting file list, header line does not contain all of " CATEGORY ", " LANG ", " BIBLE ", " YEAR ", " UPDATED ", " INFO ", " URL ": " << header.substr(0, 50));
      return;
    }
    int iMax = max(iCategory, max(iLang, max(iBible, max(iYear, max(iUpdated, max(iInfo, iUrl))))));
    
    for (++lineIter; lineIter != Lines.end(); ++lineIter) {
      const tstring& line = *lineIter;
      std::vector<tstring> Cells;
      NStringUtil::Split(";", line, Cells);

      // extract category
      if ( (unsigned int) iCategory >= Cells.size() ) {
        MSGERROR("Splitting file list, no cell (0-based index " << iCategory << ") for " CATEGORY " - skipping line: " << line);
        continue;
      }
      const tstring &category = Cells.at(iCategory);

      // switch on category
      if (category == "file") {
        // extract fields from data lines
        if ( (unsigned int) iMax >= Cells.size() ) {
          MSGERROR("Splitting file list, line with category '" << category << "' needs " << (iMax+1) << " fields but has fewer: " << line);
          continue;
        }
        const tstring &lang    = Cells.at(iLang);
        const tstring &bible   = Cells.at(iBible);
        const tstring &sYear   = Cells.at(iYear);

        // Beautify <updated> value for the user:
        //   2008-11-14T18:23:00Z
        // ==>
        //   2008-11-14 18:23
        tstring &updated = Cells.at(iUpdated);
        updated = updated.substr(0, 10) + " " + updated.substr(11, 5);

        // for Windows CEdit, need \r\n (do this here for simplicity)
        const wstring &info    = NUTF8::ToUTF16(NStringUtil::Replace(tstring(Cells.at(iInfo)), "\\n", "\r\n"));
        const tstring &url     = Cells.at(iUrl);

        const int year = atoi(sYear.c_str());

        ITwdFileDesc& fileDesc = NTwdFileDesc::MakeTwdFileDesc(lang, bible, year);
        fileDesc.SetUpdated(updated);
        fileDesc.SetInfo(info);
        fileDesc.SetUrl(url);
        fileList.Add(fileDesc);
      }
      else if (category == "info") {
        if ( (unsigned int) iLang >= Cells.size() || (unsigned int) iInfo >= Cells.size() || (unsigned int) iUpdated >= Cells.size() ) {
          MSGERROR("Splitting file list, line with category '" << category << "' lacks "
            LANG " (0-based index " << iLang << ") or "
            UPDATED " (0-based index " << iUpdated << ") or "
            INFO " (0-based index " << iInfo << "): " << line);
          continue;
        }
        const tstring &lang  = Cells.at(iLang);
        const tstring &updated  = Cells.at(iUpdated);
        // for Windows CEdit, need \r\n (do this here for simplicity)
        const wstring &info  = NUTF8::ToUTF16(NStringUtil::Replace(tstring(Cells.at(iInfo)), "\\n", "\r\n"));
        infoList.Add(lang, updated, info);
      }
      else {
        MSGERROR("Splitting file list, unhandled category " << category << " in line: " << line); 
      }
    }
  }
}

// === TEST ====================================================

#ifdef _TEST

bool NTwdFileDescParser::SelfTest()
{

  bool bRes = true;
  MSGDEBUG("--- Start NTwdFileDescParser::SelfTest ----------------------------");

  OTwdFileDescParser parser;
  OTwdFileDescList fileList;
  ITwdInfo* pInfo = 0;
  OTwdInfoList infoList;

  const string& lines1 = ""; // test empty string
  parser.Parse(lines1, fileList, infoList);
  bRes &= (0 == fileList.Size());
  bRes &= (0 == infoList.Size());

  const string& lines2 = CATEGORY ";" LANG ";" BIBLE ";" YEAR ";" UPDATED ";" INFO ";" URL "\n"
    "file;de;Schlachter2000;2008;2008-11-15T18:15:00Z;Info zu Testzwecken\\nFortgesetzt!;http://bible20.local/test";
  parser.Parse(lines2, fileList, infoList);
  bRes &= (1 == fileList.Size());
  bRes &= (0 == infoList.Size());

  const  string  Info1( "English info!"); // avoid \\n, comparison below would not match \r\n in result
  const wstring wInfo1(L"English info!");
  const string& lines3 = lines2 + "\n"
    "file;en;EnglishStandardVersion;2008;2008-11-14T18:23:00Z;Info for test\\nContinued on new line!;http://bible20.local/test2\n"
    "info;en;;;;" + Info1 + "\n"
    "info;de;;;;Deutsche info!\\nNeue Zeile!";
  parser.Parse(lines3, fileList, infoList);
  bRes &= (2 == fileList.Size());
  bRes &= (2 == infoList.Size());
  bRes &= ( (pInfo = infoList.Find("en")) && pInfo->GetInfo() == wInfo1);


  MSGDEBUG("--- End NTwdFileDescParser::SelfTest ----------------------------");
  return bRes;
}

#endif

