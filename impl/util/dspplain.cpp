/* ----------------------------------------------------------------------
  Helmut Steeb
  Uniscribe processor (USP) interface file, modified by HS
  $Id: dspplain.cpp 3905 2009-09-12 11:30:16Z helmut $
====================================================================== */

#include "stdafx.h"
////    DspPlain.CPP - Display plaintext and handle caret placement
//
//      Demonstrates use of the ScriptString APIs
//
//      ScriptStringAnalyse          - Calls ScriptItemise, Shape, Place etc.
//      ScriptStringGetLogicalWidths - Returns logical widths for the entire line
//      ScriptStringXtoCP            - Pixel position to character index
//      ScriptStringCPtoX            - Character index to pixel position
//      ScriptString_pSize           - Gets points to SIZE structure for the line
//      ScriptStringOut              - Render line to device
//      ScriptStringFree             - All analyses must be freed


//#include <windows.h>
//#include <commdlg.h>

#include "usp10.h"
#include "dspplain.h"

//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <string.h>

//#include "resource.h"

//#include "global.h"


// begin HS
static HINSTANCE hUniscribe = 0;

typedef HRESULT (WINAPI *fScriptStringAnalyse)
(
    HDC                      hdc,
    const void              *pString,
    int                      cString,
    int                      cGlyphs,
    int                      iCharset,
    DWORD                    dwFlags,
    int                      iReqWidth,
    SCRIPT_CONTROL          *psControl,
    SCRIPT_STATE            *psState,
    const int               *piDx,
    SCRIPT_TABDEF           *pTabdef,
    const BYTE              *pbInClass,
    SCRIPT_STRING_ANALYSIS  *pssa);
typedef HRESULT (WINAPI *fScriptStringOut)
(
    SCRIPT_STRING_ANALYSIS ssa,         //In  Analysis with glyphs
    int              iX,                //In
    int              iY,                //In
    UINT             uOptions,          //In  ExtTextOut options
    const RECT      *prc,               //In  Clipping rectangle (iff ETO_CLIPPED)
    int              iMinSel,           //In  Logical selection. Set iMinSel>=iMaxSel for no selection
    int              iMaxSel,           //In
    BOOL             fDisabled);        //In  If disabled, only the background is highlighted.
typedef HRESULT (WINAPI *fScriptStringFree)
(
    SCRIPT_STRING_ANALYSIS *pssa);


static fScriptStringAnalyse pScriptStringAnalyse  = 0;
static fScriptStringOut     pScriptStringOut      = 0;
static fScriptStringFree    pScriptStringFree     = 0;

bool dspplain::HasUSP10()
{
  return hUniscribe != 0 && pScriptStringAnalyse != 0 && pScriptStringOut != 0 && pScriptStringFree != 0;
}

bool dspplain::LoadUSP10()
{
  hUniscribe = LoadLibrary(_T("usp10.dll"));
  if (hUniscribe) {
    pScriptStringAnalyse  = (fScriptStringAnalyse) GetProcAddress(hUniscribe, "ScriptStringAnalyse");
    pScriptStringOut      = (fScriptStringOut)     GetProcAddress(hUniscribe, "ScriptStringOut");
    pScriptStringFree     = (fScriptStringFree)    GetProcAddress(hUniscribe, "ScriptStringFree");
    return HasUSP10();
  }
  return false;
}

// end HS


////    PaintPlainTextLine
//
//      Use the ScriptString APIs to analyse and display a line
//      as plain text.


HRESULT dspplain::PaintPlainTextLine(
                        HDC      hdc,               // In   Device
                        int     *piY,               // In   Y position
                        RECT    *prc,               // In   Clipping rectange
                        int      iFirst,            // In   First character of line
                        int      iLen,
                        int      iLineHeight,       // In   Height to advance per line, in pixels
                    const wchar_t *str,
                        bool bidiLevelRTL
                    )
{
  int      iX;
  HRESULT  hr;
//HS  int      iBuf[200];
  int      iClipWidth;
//HS  int      iTrailing;         // ScriptStringXtoCP result - offset to nearest cluster edge
  RECT     rc;
  int      iFrom = 0;
  int      iTo = 0;
  DWORD    dwFlags;
//HS  int      i;
  
  SCRIPT_CONTROL         ScriptControl;

//HS    ScriptControl = g_ScriptControl;
  // HS 2004-11-20: language codes - see definition of "LANG_NEUTRAL"
  ScriptControl.uDefaultLanguage = 1; // TODO!!! 0=default, 01=ar, 07=de

  SCRIPT_STATE           ScriptState;
//HS    ScriptState   = g_ScriptState;
  ScriptState.uBidiLevel = bidiLevelRTL ? 1 : 0;
  ScriptState.fOverrideDirection = FALSE;
  // Unicode 3.0 p.320: Deprecated Format Characters
  // -> set defaults from Unicode.
  ScriptState.fInhibitSymSwap = FALSE;
  ScriptState.fCharShape = FALSE;
  ScriptState.fDigitSubstitute = FALSE;

  ScriptState.fInhibitLigate = FALSE;
  ScriptState.fDisplayZWG = FALSE;
  ScriptState.fArabicNumContext = FALSE;
  ScriptState.fGcpClusters = FALSE;
  ScriptState.fReserved = FALSE;

  if (iLen) {
    // There's at least one character to display
//HS    SelectObject(hdc, g_style[0].hf);       // Select plaintext font
    
/*HS    if (g_fRight) {
      
      iX = prc->right - 1;
      SetTextAlign(hdc, TA_RIGHT);
      
    } else {
*/      
    int align = GetTextAlign(hdc);
    switch (align) {
    case TA_CENTER:
      iX = prc->left + (prc->right - prc->left) / 2;
      break;
    default:
    case TA_LEFT:
      iX = prc->left;
      break;
    case TA_RIGHT:
      iX = prc->right - 1;
      break;
    }
  }
    
    
    dwFlags = /*HS g_dwSSAflags | */ SSA_GLYPHS;    // Require glyph generation for ScriptStringOut
    
    // usp10.h docs for ScriptStringAnalyse() says:
    //      Note that the uBidiLevel field in the initial SCRIPT_STATE value
    //      is ignored - the uBidiLevel used is derived from the SSA_RTL
    //      flag in combination with the layout of the hdc.

    if (ScriptState.uBidiLevel & 1) {       // ScriptStringAnalyse requires SSA_RTL for rtl reading order
      dwFlags |= SSA_RTL;
    }
    
    // Draw a line to indicate clipping position
    
    iClipWidth = prc->right - prc->left;//HS2004-11-20:  - 50;
    
    // Analyse the string.
    //
    // ScriptStringAnalyse calls the ScriptItemize, ScriptShape,
    // ScriptPlace and ScriptBreak APIs and returns a pointer to a block
    // of memory containing all these results.
    
    SCRIPT_STRING_ANALYSIS ssa;
    hr = (*pScriptStringAnalyse)(
      hdc,
      str + iFirst,
      iLen, 0, -1,
      dwFlags,
      iClipWidth,
      //HS g_fNullState ? NULL : &ScriptControl,
      //HS g_fNullState ? NULL : &ScriptState,
#pragma message(__TODO__"HS 2004-11-22: not using &ScriptControl avoids RTL display of numbers in bible references!")
      0, // &ScriptControl,
      &ScriptState,
      NULL,
      NULL, NULL, &ssa);
/* HS    
    if (    SUCCEEDED(hr)
      &&  g_fOverrideDx
      &&  !(dwFlags & SSA_FIT)) {
      
      // Demonstrate use of pidx parameter by widening every character by 25%.
      // This demo doesn't work with SSA_FIT justification, so we don't even try.
      
      ScriptStringGetLogicalWidths(ssa, iBuf);
      
      for (i=0; i<iLen; i++) {
        iBuf[i] = iBuf[i] * 125 / 100;
      }
      
      // Reanalyse with the updated widths
      
      ScriptStringFree(&ssa);
      hr = ScriptStringAnalyse(
        hdc,
        g_wcBuf + iFirst,
        iLen, 0, -1,
        dwFlags,
        iClipWidth,
        g_fNullState ? NULL : &ScriptControl,
        g_fNullState ? NULL : &ScriptState,
        iBuf,
        NULL, NULL, &ssa);
    }
*/    
    
    if (SUCCEEDED(hr)) {
/*HS      
      // Check whether mouse clicks in this line are waiting to be processed
      
      if (g_fMouseDown  &&  g_iMouseDownY > *piY  &&  g_iMouseDownY < *piY+iLineHeight) {
        
        // Record char pos at left button down
        
        ScriptStringXtoCP(ssa, g_iMouseDownX-prc->left, &g_iFrom, &iTrailing);
        g_iFrom += iTrailing + iFirst; // Snap to closest cluster edge and make relative to buffer
        g_fMouseDown = FALSE;
      }
      
      
      if (g_fMouseUp  &&  g_iMouseUpY > *piY  &&  g_iMouseUpY < *piY+iLineHeight) {
        
        // Complete selection processing
        
        ScriptStringXtoCP(ssa, g_iMouseUpX-prc->left, &g_iTo, &iTrailing);
        g_iTo += iTrailing + iFirst; // Snap to closest cluster edge and make relative to buffer
        
        // Caret is where mouse was raised
        
        g_iCurChar = g_iTo;
        g_fMouseUp = FALSE;     // Signal that the mouse up is processed
        g_fUpdateCaret = TRUE;  // Request caret update
        g_iCaretSection = CARET_SECTION_PLAINTEXT;    // in plaintext section
      }
*/      
      
      // Display this line
      
      rc        = *prc;          // Clip to line height (important for selection marking)
      rc.top    = *piY;
      rc.bottom = *piY + iLineHeight;
      
/*HS      
      // Support selection range specified in either direction
      
      if (g_iFrom <= g_iTo) {
        iFrom = g_iFrom - iFirst;
        iTo   = g_iTo   - iFirst;
      } else {
        iFrom = g_iTo   - iFirst;
        iTo   = g_iFrom - iFirst;
      }
*/      
      
      hr = (*pScriptStringOut)(
        ssa,
        iX,//HS2004-11-10 prc->left,
        *piY,
        ETO_CLIPPED,
        &rc,
        iFrom,   // ScriptStringOut will ignore From/To that are outside this line
        iTo,
        FALSE);
      if (!SUCCEEDED(hr)) {
        MSGERROR("dspplain::PaintPlainTextLine: ScriptStringOut failed with code " << std::hex << hr << std::dec);
      }
      
/*HS      
      // Now position the caret if it has moved, either because of a mouse click
      // detected above, or because of an editing function such as a cursor key.
      
      if (    g_fUpdateCaret
        &&  g_iCaretSection == CARET_SECTION_PLAINTEXT
        &&  g_iCurChar >= iFirst
        &&  g_iCurChar <= iFirst+iLen) {
        
        // Display caret at coordinate corresponding to g_iCurChar
        
        g_fUpdateCaret = FALSE;
        if (g_iCurChar < iFirst + iLen) {
          
          ScriptStringCPtoX(ssa, g_iCurChar - iFirst, FALSE, &g_iCaretX);
          
        } else {
          
          // represent end of line position by placing caret at right edge
          g_iCaretX = ScriptString_pSize(ssa)->cx;
        }
        
        ResetCaret(prc->left + g_iCaretX, *piY, iLineHeight);
      }
*/      
      (*pScriptStringFree)(&ssa);

    }
    else {
/*HS      
      // Although the line is empty we may still need to display the caret
      
      if (    g_fUpdateCaret
        &&  g_iCaretSection == CARET_SECTION_PLAINTEXT
        &&  g_iCurChar == iFirst) {
        
        g_fUpdateCaret = FALSE;
        ResetCaret(prc->left, *piY, iLineHeight);
      }
*/    
      // E_NOTIMPL defined in VC98/Include/WinError.h
      if (hr == E_NOTIMPL) {
        MSGERROR("dspplain::PaintPlainTextLine: USP10 ScriptStringAnalyse not implemented (this may occur e.g. when running under Linux using Wine)!");
      }
      else {
        MSGERROR("dspplain::PaintPlainTextLine: USP10 ScriptStringAnalyse failed with code " << std::hex << hr << std::dec);
      }
    }
    *piY += iLineHeight;
    return hr;
}






////    PaintPlainText - Demonstrate ScriptString APIs
//
//


void dspplain::PaintPlainText(
                    HDC   hdc,
                    int  *piY,
                    RECT *prc,
                    int   iLineHeight,
                    wstring str)
{
  int   icpLineStart;     // First character of line
  int   icpLineEnd;       // End of line (end of buffer or index of CR character)
  int iTextLen = str.length();
  
  icpLineStart = 0;
//HS  SelectObject(hdc, g_style[0].hf);
  
  // Display line by line
  
  while (icpLineStart < iTextLen) {
    
    // Find end of line or end of buffer
    
    icpLineEnd = icpLineStart;
    while (icpLineEnd < iTextLen  &&  str[icpLineEnd] != 0x0D) {
      icpLineEnd++;
    }
    
    PaintPlainTextLine(
      hdc,
      piY,
      prc,
      icpLineStart,
      icpLineEnd-icpLineStart,
      iLineHeight, 
      str.c_str());
    
    
    // Advance to next line
    
    if (icpLineEnd < iTextLen) {
      icpLineEnd++;
    }
    if (icpLineEnd < iTextLen  &&  str[icpLineEnd] == 0x0A) {
      icpLineEnd++;
    }
    icpLineStart = icpLineEnd;
  }
}
