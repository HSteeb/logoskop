/* -------------------------------------------------------
 @file bidi.cpp
 @brief Bidirectional routines
 @author Helmut Steeb steeb@losung.de
 @version $Id: bidi.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#pragma warning(push,3)
#include "stdafx.h"
#pragma warning(pop)

#include "common.h"
#include <assert.h>
#include "bidi.h"
#include "bidiCharInfo.h"
#include "bidiLigatures.h"
#include "fribidi.h" // logical to visual conversion

#pragma warning(disable:4786) // avoid annoying STL warning
#pragma warning(disable:4514) // " "

#ifdef _DEBUG
#define DEBUG_BIDI
#else
// define this to get Bidi logging info in Release version, too.
#undef DEBUG_BIDI
#endif

#ifdef DEBUG_BIDI
  static bool sbLogging = false; // normally false, to be changed in debugger...
# define DEBUGONLY(s) s
#else
# define DEBUGONLY(s) 
#endif

// === UnicodeBitSet routines ========================================

namespace NBitSet {

  // _IsBitSet: internal function without argument check
  bool _IsBitSet(wchar_t ch, const unsigned char *pUnicodeBitSetStart)
  {
    static unsigned char bits[] = "\x1\x2\x4\x8\x10\x20\x40\x80";
    return 0 != (pUnicodeBitSetStart[ch >> 8] & bits[ch & 7]);
  }
  // IsBitSet:
  //   true iff the bit in *pUnicodeBitSetStart addressed by 0 <= ch < 65536 is set.
  //   NOTE: querying bit 0 does not make sense (caller uses it for string termination).
  bool IsBitSet(wchar_t ch, const unsigned char *pUnicodeBitSetStart)
  {
    assert(pUnicodeBitSetStart);
    return _IsBitSet(ch, pUnicodeBitSetStart);
  }
  
  // AllContained:
  //   true iff IsBitSet is true for all (possibly 0) chars in *pString.
  bool AllContained(const wchar_t *pString, const unsigned char *pUnicodeBitSetStart)
  {
    assert(pString);
    assert(pUnicodeBitSetStart);
    for ( ; *pString && _IsBitSet(*pString, pUnicodeBitSetStart); ++pString) {
      ;
    }
    return 0 == *pString;
  }

}

// === OBidiImpl declaration ========================================

class OBidiImpl : public OBidi {
public:
  OBidiImpl(TMode mode);
  OBidiImpl(const unsigned char *pUnicodeBitSetStart, const unsigned char *pUnicodeBitSetEnd);
  virtual ~OBidiImpl();

  // Shape: leaves size unchanged
  virtual void Shape(wchar_t *pSrcStart, const wchar_t *pSrcEnd);

  // Revert: leaves size unchanged
  virtual void Reorder(wchar_t *pSrcStart, wchar_t *pSrcEnd);

  // Join: result may be shorter, *pDestEnd will point past last character
  virtual void Join(wchar_t *pSrcStart, const wchar_t *pSrcEnd, const wchar_t **pDestEnd);

  virtual int LogToVis(wchar_t *pSrcStart, wchar_t *pSrcEnd, const wchar_t **pDestEnd);

private:
  // ComputeCharInfo: 
  // to be called for any mode,
  // computes character info data.
  void ComputeCharInfo(const unsigned char *pUnicodeBitSetStart);

  // ComputeLigatures: 
  // to be called for any mode.
  // computes ligature data.
  void ComputeLigatures(const unsigned char *pUnicodeBitSetStart);

  void SetJoiningClass(const wchar_t ch, TJoiningClass joiningClass);

private:
  TMode mMode;

  // mCharInfoMap:
  // contains shaping and joining class data
  NCharInfo::TCharInfoMap mCharInfoMap; 

  // TLigatureVectorMap:
  // contains pointers into sLigatures 
  // (ligatures in ligatures that contain an undesired replacement char
  //  are not referenced from mLigatureVectorMap)
  NLigature::TLigatureVectorMap mLigatureVectorMap;
}; 


// === OBidiImpl definition =========================================


OBidiImpl::OBidiImpl(TMode mode)
: mMode(mode)
{
  ComputeLigatures(0);
  ComputeCharInfo(0);
}

OBidiImpl::OBidiImpl(const unsigned char *pUnicodeBitSetStart, const unsigned char *pUnicodeBitSetEnd) 
: mMode(restricted)
{
  assert(pUnicodeBitSetStart + 8192 == pUnicodeBitSetEnd);
  ComputeCharInfo(pUnicodeBitSetStart);
  ComputeLigatures(pUnicodeBitSetStart);
}

OBidiImpl::~OBidiImpl()
{
}

void OBidiImpl::SetJoiningClass(const wchar_t ch, TJoiningClass joiningClass)
{
  NCharInfo::TCharInfoMap::iterator it = mCharInfoMap.find(ch);
  if (it == mCharInfoMap.end()) {
    // no CharInfo for the char, add a new entry for joiningClass
    TCharInfo charInfo;
    charInfo.canonical = ch;
    charInfo.SetJoiningClass(joiningClass);
    mCharInfoMap[ch] = charInfo;
  }
  else {
    (*it).second.SetJoiningClass(joiningClass);
  }
}

void OBidiImpl::ComputeCharInfo(const unsigned char *pUnicodeBitSetStart) 
{
  assert( (mMode == restricted) == (pUnicodeBitSetStart != 0));
  if (mMode == canonical) return;

  // set shape info
  TCharInfo charInfo;
  const wchar_t *pShape = 0;
  do {
    pShape = NCharInfo::ReadShape(pShape, charInfo);
    if (mMode == restricted) {
      // Got charInfo, now restrict to glyphs available 
      // pUnicodeBitSet is non 0 only in mMode==restricted!
      if (!NBitSet::IsBitSet(charInfo.isolated,  pUnicodeBitSetStart)) charInfo.isolated = 0;
      if (!NBitSet::IsBitSet(charInfo.final,     pUnicodeBitSetStart)) charInfo.final = 0;
      if (!NBitSet::IsBitSet(charInfo.medial,    pUnicodeBitSetStart)) charInfo.medial = 0;
      if (!NBitSet::IsBitSet(charInfo.initial,   pUnicodeBitSetStart)) charInfo.initial = 0;
    }
    mCharInfoMap[charInfo.canonical] = charInfo;
  } while (pShape);
 
  // set joining class info from shape list (does not contain transparent characters!)
  const wchar_t *pJoiningClass = 0;
  do {
    wchar_t ch;
    TJoiningClass joiningClass;
    pJoiningClass = NCharInfo::ReadJoiningClass(pJoiningClass, ch, joiningClass);
    SetJoiningClass(ch, joiningClass);
  } while (pJoiningClass);

  // set joining class info from transparent characters list
  const wchar_t *pTransparentChars = 0;
  do {
    wchar_t ch;
    pTransparentChars = NCharInfo::ReadTransparentChar(pTransparentChars, ch);
    SetJoiningClass(ch, transparent);
  } while (pTransparentChars);

  // U+200C ZERO WIDTH NON-JOINER
  SetJoiningClass(0x200c, nonJoining);

  // U+200D ZERO WIDTH JOINER
  SetJoiningClass(0x200d, dualJoining); // OK as dualJoining??
}

void OBidiImpl::ComputeLigatures(const unsigned char *pUnicodeBitSetStart)
{
  assert( (mMode == restricted) == (pUnicodeBitSetStart != 0));
  if (mMode == canonical) return;

  NLigature::TLigature ligature;
  wchar_t *pLig = NLigature::ReadLigature(0, ligature);
  for ( ; pLig; pLig = NLigature::ReadLigature(pLig, ligature)) {
    // pUnicodeBitSet is non 0 only in mMode restricted!
    if (mMode == unrestricted 
      || NBitSet::AllContained(ligature.pReplacement, pUnicodeBitSetStart)) {
      mLigatureVectorMap[ligature.pOriginal[0]].push_back(ligature);
    }
  }
}


void OBidiImpl::Shape(wchar_t *pSrcStart, const wchar_t *pSrcEnd)
{
  if (mMode == canonical) return;

  // Shaping algorithm from Unicode 3.0 p. 192ff.
  // implementation by Helmut Steeb 001001.
  // The algorithm assumes to be called AFTER reordering, 
  // i.e. any data which come to the right in Unicode 3.0
  // are denoted by pNextCharInfo here, and v.v.
  //
  // For implementation purposes, read the Unicode algorithm as follows:
  //
  // Rules are shown in terms of a sequence of a + [b] + c (in visual order) where
  //    a, c are derived joining classes (table 8-3), 
  //      e.g. "RJC" is "Right join-causing"
  //      !a is the complement of a, e.g. !LJC is {R,D,U,T}
  //    [b] is the joining class of the current character
  //    right(i) is the final    glyph for i
  //    left(i)  is the initial  glyph for i
  //    med(i)   is the medial   glyph for i (if available)
  //    isol(i)  is the isolated glyph for i (if available)
  // 
  // R1. In the sequence a + [b] + c, 
  //     use the "nearest" non-transparent characters for a and c;
  //     if there are none, use class U (non-joining)
  // R2.         [R] + RJC  ==>        right(R) + RJC
  // R3.  LJC  + [L]        ==>  LJC + left(L)
  // R4.  LJC  + [D] + RJC  ==>  LJC + med(D)   + RJC
  // R5.  !LJC + [D] + RJC  ==>  !LJC+ right(D) + RJC
  // R6.  LJC  + [D] + !RJC ==>  LJC + left(D)  + !RJC
  // R7.  else:  [D]        ==>        isol(D)

  const TCharInfo *pPrevCharInfo = &NCharInfo::GetEmptyCharInfo();
  const TCharInfo *pNextCharInfo = pPrevCharInfo;

  for (wchar_t *pSrc = pSrcStart; pSrc < pSrcEnd; ++pSrc) {
    const TCharInfo &currCharInfo = NCharInfo::GetCharInfo(*pSrc, mCharInfoMap);

    // R1.
    for (const wchar_t *pNext = pSrc + 1; pNext < pSrcEnd; ++pNext){
      pNextCharInfo = &NCharInfo::GetCharInfo(*pNext, mCharInfoMap);
      if (pNextCharInfo->joiningClass != transparent) {
        break;
      }
    }
    if (pNext >= pSrcEnd) {
      pNextCharInfo = &NCharInfo::GetEmptyCharInfo();
    }

    wchar_t rpl = *pSrc; // default
#ifdef DEBUG_BIDI
    std::string choice;
#endif
    switch (currCharInfo.joiningClass) {
    case rightJoining:
      // R2.
      if (currCharInfo.final && pNextCharInfo->isRightJoinCausing) {
        rpl = currCharInfo.final;
#ifdef DEBUG_BIDI
        choice = "final";
#endif
      }
      break;
    case leftJoining:
      // R3.
      if (currCharInfo.initial && pPrevCharInfo->isLeftJoinCausing) {
        rpl = currCharInfo.initial;
#ifdef DEBUG_BIDI
        choice = "initial";
#endif
      }
      break;
    case dualJoining:
      // R4.
      if (currCharInfo.medial && pNextCharInfo->isRightJoinCausing && pPrevCharInfo->isLeftJoinCausing) {
        rpl = currCharInfo.medial;
#ifdef DEBUG_BIDI
        choice = "medial";
#endif
      }
      // R5.
      else if (currCharInfo.final && pNextCharInfo->isRightJoinCausing && !pPrevCharInfo->isLeftJoinCausing) {
        rpl = currCharInfo.final;
#ifdef DEBUG_BIDI
        choice = "final";
#endif
      }
      // R6.
      else if (currCharInfo.initial && !pNextCharInfo->isRightJoinCausing && pPrevCharInfo->isLeftJoinCausing) {
        rpl = currCharInfo.initial;
#ifdef DEBUG_BIDI
        choice = "initial";
#endif
      }
      break;
    default:
      ; // nop
    }
    if (rpl == *pSrc && currCharInfo.isolated) {
      // R7.
      rpl = currCharInfo.isolated;
#ifdef DEBUG_BIDI
      DEBUGONLY(choice = "isolated";)
#endif
    }
    wchar_t JoiningClass2Char(TJoiningClass joiningClass);

#ifdef DEBUG_BIDI
    if (sbLogging && (currCharInfo.isolated || currCharInfo.initial || currCharInfo.medial || currCharInfo.final)) {
      MSGDEBUG("Shape(): ["
        << static_cast<char>(NCharInfo::JoiningClass2Char(pPrevCharInfo->joiningClass))
        << "] 0x" << std::hex << *pSrc << "["
        << static_cast<char>(NCharInfo::JoiningClass2Char(currCharInfo.joiningClass))
        << "] ["
        << static_cast<char>(NCharInfo::JoiningClass2Char(pNextCharInfo->joiningClass))
        << "] ==> " << choice << " 0x" << std::hex << rpl
        << " (Is,In,Med,Fin) = ("
        << std::hex << currCharInfo.isolated << ","
        << std::hex << currCharInfo.initial << ","
        << std::hex << currCharInfo.medial << ","
        << std::hex << currCharInfo.final << ")");
    }
#endif
    *pSrc = rpl;
    if (currCharInfo.joiningClass != transparent) {
      // R1.
      pPrevCharInfo = &currCharInfo;
    }
  }
}

void ReorderLine(wchar_t *pSrcStart, wchar_t *pSrcEnd)
{
  /*
  wchar_t *pLo;
  wchar_t *pHi;
  for (pLo = pSrcStart, pHi = pSrcEnd - 1; pLo < pHi; ++pLo, --pHi) {
    wchar_t tmp = *pLo;
    *pLo = *pHi;
    *pHi = tmp;
  }
  */
/*
  fribidi_log2vis(// input 
		     FriBidiChar *str,
		     gint len,
		     FriBidiCharType *pbase_dir,
		     // output 
		     FriBidiChar *visual_str,
		     guint16     *position_L_to_V_list,
		     guint16     *position_V_to_L_list,
		     guint8      *embedding_level_list
		     );
*/
  const int len = pSrcEnd - pSrcStart;

  // alloc buffer of FriBidiChar (UCS4 chars!)
  FriBidiChar *pFriBidiBuf = new FriBidiChar[len+1];
  int i;

  // copy wchar_t source string to dword buffer
  for (i = 0; i < len; ++i) pFriBidiBuf[i] = pSrcStart[i];

  // reorder using FriBidi
  // HS 2005-01-06: do NOT use FRIBIDI_TYPE_LTR!
  FriBidiCharType base_dir = FRIBIDI_TYPE_N;
  fribidi_log2vis(pFriBidiBuf, len, 
    /* pbase_dir 
      is the input and output base direction. If 
      base == FRIBIDI_TYPE_N then fribidi_log2vis 
      calculates the base direction on its own
      according to the BiDi algorithm. 
     */
    &base_dir, 
    /* visual_str             
      The reordered output unicode string.
      HS 000825: the first use of this parameter is
        if (visual_str) for (i=0; i<len+1; i++) visual_str[i] = str[i];
        and behind this code, the input string "str" is no longer used.
        Hence str may be passed for visual_str to reorder in situ.
     */
    pFriBidiBuf, 
    /* n any of the output pointers == NULL, then that information is not calculated. */
    0, 0, 0);

  // copy dword reordering result back to wchar_t string 
  for (i = 0; i < len; ++i) pSrcStart[i] = static_cast<wchar_t>(pFriBidiBuf[i]);
  delete[] pFriBidiBuf;
}

void OBidiImpl::Reorder(wchar_t *pSrcStart, wchar_t *pSrcEnd)
{
  int icpLineStart = 0;     // First character of line
  int icpLineEnd;       // End of line (end of buffer or index of CR character)
  int iTextLen = pSrcEnd - pSrcStart;
  while (icpLineStart < iTextLen) {
    // Find end of line or end of buffer
    icpLineEnd = icpLineStart;
    while (icpLineEnd < iTextLen  &&  pSrcStart[icpLineEnd] != 0x0D) {
      icpLineEnd++;
    }
    ReorderLine(pSrcStart + icpLineStart, pSrcStart + icpLineEnd);
    if (icpLineEnd < iTextLen) {
      icpLineEnd++;
    }
    if (icpLineEnd < iTextLen  &&  pSrcStart[icpLineEnd] == 0x0A) {
      icpLineEnd++;
    }
    icpLineStart = icpLineEnd;
  }
}

void OBidiImpl::Join(wchar_t *pSrcStart, const wchar_t *pSrcEnd, const wchar_t **ppDestEnd)
{
  if (mMode == canonical) {
    // leave unchanged
    *ppDestEnd = pSrcEnd;
    return;
  }

  wchar_t *pDest = pSrcStart;
  for (const wchar_t *pSrc = pSrcStart; pSrc < pSrcEnd; ) {
    if (!NLigature::Replace(mLigatureVectorMap, &pSrc, pSrcEnd, &pDest)) {
      *pDest++ = *pSrc++;
    }
#ifdef DEBUG_BIDI
    else if (sbLogging) {
      MSGDEBUG("Join(): replaced by ligature, end at 0x" << std::hex << *(pDest-1));
    }
#endif
  }
  *ppDestEnd = pDest;
}

int OBidiImpl::LogToVis(wchar_t *pSrcStart, wchar_t *pSrcEnd, const wchar_t **ppDestEnd)
{
  Reorder(pSrcStart, pSrcEnd);
  Shape(pSrcStart, pSrcEnd);
  Join(pSrcStart, pSrcEnd, ppDestEnd);
  return 0;
}


// === NBidi =====================================================

OBidi *NBidi::CreateBidi(OBidi::TMode mode)
{
  return new OBidiImpl(mode);
}

OBidi *NBidi::CreateBidi(const unsigned char *pUnicodeBitSetStart, const unsigned char *pUnicodeBitSetEnd)
{
  return new OBidiImpl(pUnicodeBitSetStart, pUnicodeBitSetEnd);
}

void NBidi::ActivateLogging(bool b)
{
#ifdef DEBUG_BIDI
  sbLogging = b;
  fribidi_set_debug(b);
#endif
}

