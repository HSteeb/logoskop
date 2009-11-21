/* ----------------------------------------------------------------------
  Helmut Steeb
  Uniscribe processor (USP) interface.
  $Id: dspplain.h 3903 2009-09-10 20:03:50Z helmut $
====================================================================== */

namespace dspplain {

HRESULT PaintPlainTextLine(
                        HDC      hdc,               // In   Device
                        int     *piY,               // In   Y position
                        RECT    *prc,               // In   Clipping rectange
                        int      iFirst,            // In   First character of line
                        int      iLen,
                        int      iLineHeight,       // In   Height to advance per line, in pixels
                        const wchar_t *str,
                        bool bidiLevelRTL = false   // HS 2004-11-20: In  true for RTL line
                        );
                    
void PaintPlainText(
                    HDC   hdc,
                    int  *piY,
                    RECT *prc,
                    int   iLineHeight,
                    wstring str)
;

// begin HS
bool HasUSP10();
bool LoadUSP10();

// end HS

}
