/* ----------------------------------------------------------------------
  Helmut Steeb 
  Read directory into list of strings.

  $Id: DirectoryFileDescReader.cpp 3924 2009-09-26 17:37:12Z helmut $

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
#include "DirectoryFileDescReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === IDirectoryFileDescReader ===

IDirectoryFileDescReader::~IDirectoryFileDescReader()
{
}

// === ODirectoryFileDescReader ===

ODirectoryFileDescReader::~ODirectoryFileDescReader()
{
}


bool ODirectoryFileDescReader::Read(
  const string& dir, 
  const string& prefix, 
  const string& suffix, 
  std::vector<string>& reslist)
{
  MSGDEBUG("Reading directory " + dir + " for files " + prefix + "*" + suffix);
  reslist.clear();

  // e.g.
  // prefix  = ""
  // result infix = "de"
  // suffix  = "_los07.xml"
  // return list of infixes = language codes
  // Grep for [LANGUAGE]
  WIN32_FIND_DATA findData;
  string completeprefix = dir + "\\" + prefix;
  string mask = completeprefix + "*" + suffix;
  HANDLE hFiles = FindFirstFile(mask.c_str(), &findData);
  if (hFiles == INVALID_HANDLE_VALUE) {
    // this occurs if no file found e.g. after initial install without .twd files
    MSGDEBUG("No match from FindFirstFile when reading directory " + dir + " for files " + prefix + "*" + suffix);
    return false;
  }
  do {
    string tmp = findData.cFileName;
#ifdef _DEBUG
    // funny debugging...
    int a = prefix.length();
    int b = tmp.length();
    int c = suffix.length();
    int d = b-c;
    string su = tmp.substr(a, d-a);
    reslist.push_back(su);
#else
    reslist.push_back(
        tmp.substr(prefix.length(), tmp.length()-suffix.length()-prefix.length()));
#endif
  } while (FindNextFile(hFiles, &findData));
  FindClose(hFiles);
  return true;
}

#ifdef _TEST

// === OMockLangBibleYearReader ===

typedef std::multimap<int, string>::const_iterator I; // Stroustrup p. 521

OMockLangBibleYearReader::~OMockLangBibleYearReader()
{
}

void OMockLangBibleYearReader::Clear()
{
  mLangBiblesPerYear.clear();
}

void OMockLangBibleYearReader::Add(int year, const string &langBible)
{
  mLangBiblesPerYear.insert(std::make_pair(year, langBible));
}

bool OMockLangBibleYearReader::Has(int year, const string &langBible) const
{
  std::pair<I, I> matches = mLangBiblesPerYear.equal_range(year);
  for (I i = matches.first; i != matches.second; ++i) {
    if (i->second == langBible) {
      return true;
    }
  }
  return false;
}

bool OMockLangBibleYearReader::Read(
  const string& dir, 
  const string& prefix, 
  const string& suffix, 
  std::vector<string>& reslist)
{
  reslist.clear();

  string sYear = suffix.substr(1, 4);
  int year = atoi(sYear.c_str());
  std::pair<I, I> matches = mLangBiblesPerYear.equal_range(year);
  bool bRes = false;
  for (I i = matches.first; i != matches.second; ++i) {
    reslist.push_back(i->second);
    bRes = true;
  }
  return bRes;
}

#endif
