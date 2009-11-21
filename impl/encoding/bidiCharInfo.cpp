/* -------------------------------------------------------
 @file bidiCharInfo.cpp
 @brief Bidirectional routines for shape+transparency info
 @author Helmut Steeb
 @version $Id: bidiCharInfo.cpp 3011 2008-11-29 09:37:12Z helmut $

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
-------------------------------------------------------*/

#pragma warning(disable:4786) // avoid annoying STL warning
#pragma warning(disable:4514) 

#pragma warning(push,3)
#include "stdafx.h"
#pragma warning(pop)


#include <assert.h>
#include "common.h"
#include "bidiCharInfo.h"


// === Data tables ==================================================

// sShapeTable:
// Last line is terminating zero.
static const wchar_t sShapes[] = {
#include "bidiShapeTable.h"
0
};

// sJoiningClasses:
// Last line is terminating zero.
static const wchar_t sJoiningClasses[] = {
#include "bidiArabicShaping.h"
0
};

// sTransparentChars:
// Last line is terminating zero.
static const wchar_t sTransparentChars[] = {
#include "bidiTransparent.h"
0
};


// === CharInfo (shape+transparency) ========================================

TCharInfo::TCharInfo() 
{
  Reset();
}
void TCharInfo::Reset() 
{
  memset(this, 0, sizeof(TCharInfo));
  joiningClass = nonJoining;
}

void TCharInfo::SetJoiningClass(TJoiningClass _joiningClass)
{
  joiningClass = _joiningClass;
  isRightJoinCausing = joiningClass == dualJoining || joiningClass == leftJoining || joiningClass == joinCausing;
  isLeftJoinCausing = joiningClass == dualJoining || joiningClass == rightJoining || joiningClass == joinCausing;
}

wchar_t NCharInfo::JoiningClass2Char(TJoiningClass joiningClass)
{
  return static_cast<wchar_t>(joiningClass);
}

TJoiningClass NCharInfo::Char2JoiningClass(wchar_t ch)
{
  return static_cast<TJoiningClass>(ch);
  /*
  TJoiningClass joiningClass;
  switch (ch) {
  default: 
    MSGERROR("Char2JoiningClass(): invalid joining class 0x" << std::hex << static_cast<unsigned int>(ch));
    // run into nonJoining...
  case 'U': joiningClass = nonJoining; break;
  case 'R': joiningClass = rightJoining; break;
  case 'L': joiningClass = leftJoining; break;
  case 'D': joiningClass = dualJoining; break;
  case 'C': joiningClass = joinCausing; break;
  case 'T': joiningClass = transparent; break;
  }
  return joiningClass;
  */
}

static TCharInfo sEmptyCharInfo;

const wchar_t *NCharInfo::ReadShape(const wchar_t *pShapeTable, TCharInfo& charInfo)
{
  charInfo.Reset();;
  if (!pShapeTable) pShapeTable = sShapes; // initialize
  if (!*pShapeTable) return 0;
  
  charInfo.canonical = *pShapeTable++;
  if (*pShapeTable) {
    charInfo.isolated = *pShapeTable++;
    if (*pShapeTable) {
      charInfo.final = *pShapeTable++;
      if (*pShapeTable) {
        charInfo.medial = *pShapeTable++;
        if (*pShapeTable) {
          charInfo.initial = *pShapeTable++;
        }
      }
    }
  }

  return pShapeTable + 1; // next "line"
}

const wchar_t *NCharInfo::ReadJoiningClass(const wchar_t *pJoiningClassTable, wchar_t &ch, TJoiningClass& joiningClass)
{
  if (!pJoiningClassTable) pJoiningClassTable = sJoiningClasses; // initialize
  if (!*pJoiningClassTable) return 0;

  ch = *pJoiningClassTable++;
  wchar_t cl = *pJoiningClassTable++;
  joiningClass = Char2JoiningClass(cl);
  return pJoiningClassTable;
}

const wchar_t *NCharInfo::ReadTransparentChar(const wchar_t *pTransparentChars, wchar_t &ch)
{
  if (!pTransparentChars) pTransparentChars = sTransparentChars; // initialize
  if (!*pTransparentChars) return 0;

  ch = *pTransparentChars++;
  return pTransparentChars;
}



const TCharInfo &NCharInfo::GetCharInfo(const wchar_t ch, const TCharInfoMap &charInfoMap)
{
  TCharInfoMap::const_iterator it = charInfoMap.find(ch);
  assert(it == charInfoMap.end() || (*it).first == ch);
  return (it == charInfoMap.end()) ? sEmptyCharInfo : (*it).second;
}

const TCharInfo &NCharInfo::GetEmptyCharInfo()
{
  return sEmptyCharInfo;
}
