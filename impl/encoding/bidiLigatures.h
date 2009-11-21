/* ======================================================================
 @file bidiLigatures.h
 @brief Bidirectional routines for ligatures
 @author Helmut Steeb
 @version $Id: bidiLigatures.h 3011 2008-11-29 09:37:12Z helmut $
 
 Needs vector and map from STL!

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

#if !defined(__BIDILIGATURES_H_INCLUDED__)
#define __BIDILIGATURES_H_INCLUDED__

namespace NLigature {

  struct TLigature {
    const wchar_t *pOriginal;
    const wchar_t *pReplacement;
  };
  typedef std::vector<TLigature> TLigatureVector;
  
  // TLigatureVectorMap:
  // maps a wchar_t c to all TLigature's L with L.pOriginal[0]==c.
  typedef std::map<wchar_t, TLigatureVector> TLigatureVectorMap;
  
  // ReadLigature:
  // Assumes pLigatureTable 
  //   a.) is 0, then starts at beginning of internal table, or
  //   b.) was returned from previous call
  // If there is no data left, return 0.
  // Else read data, set the pointers in ligature, 
  //   and return a pointer behind the data read.
  wchar_t *ReadLigature(wchar_t *pLigatureTable, TLigature &ligature);

  // Replace:
  //   if ligatureVectorMap contains a ligature whose pOriginal
  //   matches the string beginning at *ppSrc before *pSrcEnd,
  //     Replace() copies the pReplacement to *ppDest,
  //     advances *ppSrc to the first non-matching character, 
  //     advances *ppDest behind the pReplacement,
  //     returns true.
  //   Else returns false, nothing changed.
  bool Replace(const TLigatureVectorMap &ligatureVectorMap, 
    const wchar_t **ppSrc, const wchar_t *pSrcEnd, wchar_t **ppDest);

}

#endif
