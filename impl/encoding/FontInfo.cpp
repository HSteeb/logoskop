/* -------------------------------------------------------
 @file FontInfo.cpp
 @brief Read glyph info from font
 @author Helmut Steeb
 @version $Id: FontInfo.cpp 3011 2008-11-29 09:37:12Z helmut $

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
//#pragma warning(push,3)
#include "stdafx.h"
//#pragma warning(pop)

#include <assert.h>

#pragma warning(disable:4786) // avoid annoying STL warning
#pragma warning(disable:4514) // " "

#ifndef MSGERROR 
#define MSGERROR(s) 
#endif

void SwapShort (PUSHORT p)
{
  SHORT temp;
  
  temp =(SHORT)( HIBYTE (*p) + (LOBYTE(*p) << 8));
  *p = temp;
}



void SwapULong (PULONG p)
{
  ULONG temp;
  
  temp = (LONG) ((BYTE) *p);
  temp <<= 8;
  *p >>=8;
  
  temp += (LONG) ((BYTE) *p);
  temp <<= 8;
  *p >>=8;
  
  temp += (LONG) ((BYTE) *p);
  temp <<= 8;
  *p >>=8;
  
  temp += (LONG) ((BYTE) *p);
  *p = temp;
}
			

bool ReadFontInfoAux(HDC hdc, unsigned char *fontinfo)
{
// HS: font accessing code borrowed in large from MS example code GridFont (grid.cpp)
  memset(fontinfo, 0, 0x2000); // 64k/8

#define CMAPHEX	0x70616d63 // = "cmap" (reversed)
#define NBYTES   256
#define OFFSETERROR 0
  
  typedef struct tagTABLE{
    USHORT platformID;
    USHORT encodingID;
    ULONG  offset;
  } TABLE, *PTABLE;
  
  typedef struct tagSUBTABLE{
    USHORT format;
    USHORT length;
    USHORT version;
    USHORT segCountX2;
    USHORT searchRange;
    USHORT entrySelector;
    USHORT rangeShift;
  } SUBTABLE, *PSUBTABLE;
  
  DWORD       cbData;
  USHORT      aShort[2];
  DWORD       nBytes;
  USHORT      i, nTables;
  PTABLE      pTable;
  PSUBTABLE   pSubTable;
  ULONG       offset,offsetFormat4;
  BYTE        buffer[NBYTES];
  
  // find number of encoding tables, second long in cmap
  nBytes = GetFontData(hdc, CMAPHEX, 0, aShort, 4);
  if (nBytes == GDI_ERROR || nBytes == 0)	{
    MSGERROR("GetFontData: GDI error or no cmap table");
    return false;
  }
  nTables = aShort[1];
  SwapShort (&nTables);
  
  // limit ourself to 32 encoding tables (largely enough)
  cbData = nTables * sizeof(TABLE);
  if (cbData >NBYTES) {
    MSGERROR("GetFontData: cbData > " << NBYTES);
    return false;
  }
  
  // get array of encoding tables.
  // Check each one for PlatformId = 3, Encoding ID = 1.
  nBytes=GetFontData (hdc, CMAPHEX, 4, buffer, cbData);
  pTable = (PTABLE)buffer;
  offsetFormat4 = OFFSETERROR;
  for (i = 0; i< nTables; i++) {
    SwapShort (&(pTable->encodingID));
    SwapShort (&(pTable->platformID));
    if ((pTable->platformID == 3) && (pTable->encodingID == 1)) {
      offsetFormat4 = pTable->offset;
      SwapULong (&offsetFormat4);
      break;
    }
    pTable++;
  }
  if (offsetFormat4 == OFFSETERROR) {//Can not find 3,1 subtable 
    MSGERROR("GetFontData: This font does not contain Unicode information!");
    return false;
  }
  
  /* Get the beginning of the subtable, especially the segment count */
  nBytes=GetFontData (hdc, CMAPHEX, offsetFormat4, buffer, sizeof(SUBTABLE));
  pSubTable = (PSUBTABLE) buffer;
  SwapShort (&(pSubTable->format));
  SwapShort (&(pSubTable->segCountX2));
  if (pSubTable->format != 4) {
    MSGERROR("GetFontData: font format != 4!");
    return false;
  }
  int vSegCount = pSubTable->segCountX2 / 2;
  
  /* Now that we know how many segments that the font contains,
  *  free up the old memory, and realloc. the two global arrays.
  */
  USHORT *vStartCount = new USHORT[vSegCount];
  USHORT *vEndCount = new USHORT[vSegCount];
  if (vStartCount == NULL || vEndCount == NULL) {
    delete[] vStartCount;
    delete[] vEndCount;
    MSGERROR("GetFontData: alloc failed!");
    return false;
  }
  
  /* read in the array of endCount values */
  offset = offsetFormat4 + (7 * sizeof(USHORT));  /* skip constant # bytes in subtable */
  cbData = vSegCount * sizeof (USHORT);
  nBytes = GetFontData(hdc, CMAPHEX, offset, vEndCount, cbData);
  for (i = 0; i < vSegCount; i++) SwapShort(&vEndCount[i]);
  
  /* read in the array of startCount values */
  offset = offsetFormat4
    + (7 * sizeof (USHORT))   /* skip constant # bytes in subtable */
    + (vSegCount * sizeof (USHORT)) /* skip endCount array */
    + sizeof (USHORT);             /* skip reservedPad */
  cbData = vSegCount * sizeof(USHORT);
  nBytes = GetFontData (hdc, CMAPHEX, offset, vStartCount, cbData);
  for (i = 0; i < vSegCount; i++) SwapShort(&vStartCount[i]);

  for (int seg = 0; seg < vSegCount; ++seg) {
    for (unsigned int ch = vStartCount[seg]; ch <= vEndCount[seg]; ++ch) {
       fontinfo[ch >> 3] |= (1 << (ch & 7));
     }
  }
  delete[] vStartCount;
  delete[] vEndCount;
  
  return true;
}

bool ReadFontInfo(unsigned char *fontinfo)
{
// HS: font accessing code borrowed in large from MS example code GridFont (grid.cpp)
  //CFont*	pOldFont = pDC->SelectObject(mHeadlineFontInfo.m_pFont);
  
  HDC hdc = CreateCompatibleDC(GetDC(GetDesktopWindow()));
  //HGDIOBJ gdiobj = SelectObject(hdc, font);
  bool res = ReadFontInfoAux(hdc, fontinfo);
  DeleteDC(hdc);
  //pDC->SelectObject(pOldFont);
  return res;
}

