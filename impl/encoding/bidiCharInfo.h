/* ======================================================================
 @file bidiCharInfo.h
 @brief Bidirectional routines for shape+transparency info
 @author Helmut Steeb
 @version $Id: bidiCharInfo.h 3011 2008-11-29 09:37:12Z helmut $
 
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

#if !defined(__BIDICHARINFO_H_INCLUDED__)
#define __BIDICHARINFO_H_INCLUDED__

typedef enum { 
  nonJoining = 'U', 
    rightJoining = 'R', 
    leftJoining = 'L', 
    dualJoining = 'D', 
    joinCausing = 'C', 
    transparent = 'T'
} TJoiningClass;

namespace NCharInfo {

  // mapping functions char (as in Unicode 3.0) <=> TJoiningClass
  wchar_t JoiningClass2Char(TJoiningClass joiningClass);
  TJoiningClass Char2JoiningClass(wchar_t ch);
}

class TCharInfo {
public:
  
  TCharInfo();
  void Reset();
  void SetJoiningClass(TJoiningClass joiningClass);// sets precomputed data!
  
  wchar_t canonical;
  wchar_t isolated;
  wchar_t final;
  wchar_t medial;
  wchar_t initial;
  // joiningClass:
  TJoiningClass joiningClass;
  // precomputed from joiningClass;
  bool isRightJoinCausing;
  bool isLeftJoinCausing;
};


namespace NCharInfo {
  
  // TCharInfo: contains 
  // - special shapes (or 0 if none)
  // - transparency flag
  
  // ReadShape:
  // Assumes pShapeTable 
  //   a.) is 0 (to start reading), or
  //   b.) was returned from previous call
  // If there is no data left, return 0.
  // Else read data, set the values in charInfo, 
  //   and return a pointer behind the data read.
  const wchar_t *ReadShape(const wchar_t *pShapeTable, TCharInfo &charInfo);

  // ReadJoiningClass:
  // Assumes pJoiningClassTable 
  //   a.) is 0 (to start reading), or
  //   b.) was returned from previous call
  // If there is no data left, return 0.
  // Else read data, set ch and joiningClass, 
  //   and return a pointer behind the data read.
  const wchar_t *ReadJoiningClass(const wchar_t *pJoiningClassTable, wchar_t &ch, TJoiningClass& joiningClass);
  
  // ReadTransparentChar:
  // Assumes pTransparentChars 
  //   a.) is 0 (to start reading), or
  //   b.) was returned from previous call
  // If there is no data left, return 0.
  // Else read data, set ch, 
  //   and return a pointer behind the data read.
  const wchar_t *ReadTransparentChar(const wchar_t *pTransparentChars, wchar_t &ch);
  
  // TCharInfoMap: 
  //   maps the canonical character c to the TCharInfo C that has C.canonical==c.
  typedef std::map<wchar_t, TCharInfo> TCharInfoMap;
  
  // GetCharInfo:
  //   for ch, returns the CharInfo C from charInfoMap with C.canonical==ch,
  //   or sEmptyCharInfo if there is no such C.
  const TCharInfo &GetCharInfo(const wchar_t ch, const TCharInfoMap &charInfoMap);

  const TCharInfo &GetEmptyCharInfo();
  
}

#endif
