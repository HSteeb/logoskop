/* -------------------------------------------------------
 @file bidiLigatures.cpp
 @brief Bidirectional routines for ligatures
 @author Helmut Steeb
 @version $Id: bidiLigatures.cpp 3011 2008-11-29 09:37:12Z helmut $

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
#pragma warning(push,3)
#include "stdafx.h"
#pragma warning(pop)
#pragma warning(default:4786) 

#include <assert.h>
#include "bidiLigatures.h"




// === Data tables ==================================================

// Tables from arabjoint.pl http://www.czyborra.com

// sLigatures:
static wchar_t sLigatures[] = {
#include "bidiLigTable.h"
0
};


// === Ligatures ========================================================

wchar_t *NLigature::ReadLigature(wchar_t *pLigatureTable, TLigature &ligature)
{
  if (!pLigatureTable) pLigatureTable = sLigatures;
  if (!*pLigatureTable) return 0;

  ligature.pOriginal = pLigatureTable;
  while (*pLigatureTable++) {}  // advance behind next \0
  ligature.pReplacement = pLigatureTable;
  while (*pLigatureTable++) {}  // advance behind next \0
 
  // for in situ conversion, 
  // require no ligature replacement text is longer than its source
  assert(wcslen(ligature.pOriginal) >= wcslen(ligature.pReplacement));

  return pLigatureTable;
}

bool NLigature::Replace(
  const TLigatureVectorMap &ligatureVectorMap, 
  const wchar_t **ppSrc, const wchar_t *pSrcEnd, wchar_t**ppDest)
{
  TLigatureVectorMap::const_iterator it = ligatureVectorMap.find(**ppSrc);
  if (it != ligatureVectorMap.end()) {
    const TLigatureVector &ligatureVector = (*it).second;
    TLigatureVector::const_iterator vecit;
    for (vecit = ligatureVector.begin(); vecit != ligatureVector.end(); ++vecit) {
      const TLigature &ligature = *vecit;
      // compare source string starting at pSrc ending before pSrcEnd
      // to ligature.pOriginal which is zero terminated.
      // pSrc[0] is guaranteed to match because mLigatureVectorMap.find(*pSrc)
      //   returns a vector whose ligature.pOriginal's all start with *pSrc.
      assert(**ppSrc == ligature.pOriginal[0]);
      int n; // == number of matching chars in pOriginal
      for (n = 1; *ppSrc + n < pSrcEnd && (*ppSrc)[n] == ligature.pOriginal[n]; ++n) {
        ;
      }
      if (ligature.pOriginal[n] == 0) {
        // all characters in ligature.pOriginal matched => replace
        wcscpy(*ppDest, ligature.pReplacement);
        *ppSrc += n; // let *ppSrc point to next char
        *ppDest += wcslen(ligature.pReplacement);
        return true;
      }
    }
  }
  return false;
}
