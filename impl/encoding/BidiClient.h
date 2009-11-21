/* ======================================================================
 @file BidiClient.h
 @brief C++ base class for bidi functionality, calling encoding.dll.
 @author Helmut Steeb
 @version $Id: BidiClient.h 3011 2008-11-29 09:37:12Z helmut $

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

#if !defined(__BIDICLIENT_H_INCLUDED__)
#define __BIDICLIENT_H_INCLUDED__

// you need <vector> and <string> in your stdafx.h!!!
#include "Encoding.h"
#include "TCHAR.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class OBidiClient
{
public:
  OBidiClient(bool bUnrestricted = true);
  OBidiClient(unsigned char *pUnicodeBitSetStart, const unsigned char *pUnicodeBitSetEnd);
  ~OBidiClient();



/** 
 SetMessageCallback: sets a callback that will be called for reporting messages.
 @param messageCallback will be called (may be 0 to reset callback)
 @return 
   0 = success
   -2 = internal error
   -4 = encoding.dll not found 
   -5 = function in encoding.dll not found 
*/
  static long SetMessageCallback(TMessageCallback messageCallback);

/** 
 Reorder: in situ reorders from logical to visual order.
 @param str string to reorder
 @return 
   0 = success.
   -2 = internal error
   -4 = encoding.dll not found (str unchanged)
   -5 = function in encoding.dll not found (str unchanged)
*/
  long Reorder(wstring &str);

/** 
 Shape: in situ shapes characters of the U+0600 block.
 @param str string to shape
 @return 
   0 = success
   -2 = internal error
   -4 = encoding.dll not found (str unchanged)
   -5 = function in encoding.dll not found (str unchanged)
*/
  long Shape(wstring &str);

/** 
 Join: joins ligatures.
 @param str string to join
 @return 
   0 = success.  *ppDestEnd points one past last element written.
   -2 = internal error
   -4 = encoding.dll not found (str unchanged)
   -5 = function in encoding.dll not found (str unchanged)
*/
  long Join(wstring &str);

/** 
 LogToVis: does Reorder, Shape, Join.
 @param str string to convert
 @return 
   0 = success. *ppDestEnd points one past last element written.
   -2 = internal error
   -4 = encoding.dll not found (str unchanged)
   -5 = function in encoding.dll not found (str unchanged)
*/
  long LogToVis(wstring &str);

private:
  GLYPHSET_HANDLE mGlyphSetHandle;
};


#endif
