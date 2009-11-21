/* ======================================================================
 @file LosFileConst.h
 @brief String constants for DTD
 @author Helmut Steeb
 @version $Id: LosFileConst.h 3014 2008-11-29 10:19:16Z helmut $

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
#ifndef _LOSFILECONST_H_
#define _LOSFILECONST_H_

#include "../util/tstring.h"

// --- element definitions ---

#define E_BIBLENAME         "biblename"
#define E_BIBLECOPYRIGHT    "biblecopyright"
#define E_C                 "c"
#define E_DAY               "DAY"
#define E_DOW               "DOW"
#define E_EM                "em"
#define E_EM_w              L"em"
#define E_FILEVERS          "updated"
#define E_HEAD              "head"
#define E_IL                "intro"
#define E_L                 "text"
#define E_LOSFILE           "thewordfile"
#define E_LOSUNG            "theword"
#define E_MONTH             "MONTH"
#define E_MONTHS            "MONTHS"
#define E_PAROL             "parol"
#define E_NT                "parol"
#define E_OT                "parol"
#define E_SL                "ref"
#define E_SUBLANG           "SUBLANG"
#define E_TL                "title"
#define E_TT                "TT"
#define E_TLPATTERN         "TLPATTERN"
#define E_X                 "X"
#define E_X_w               L"X"
#define E_XLAT              "copyright"

#define GUI_EM              "\"\"" // GUI representation of <EM> - cf. below sGUI_EM etc.!
#define GUI_EM_LEN          2
#define GUI_X_OPEN          "{{" // GUI representation of <X>
#define GUI_X_OPEN_LEN      2
#define GUI_X_CLOSE         "}}" // GUI representation of </X>
#define GUI_X_CLOSE_LEN     2

const wstring sGUI_EM       (L"\"\""); // - cf. above GUI_EM etc.!
const wstring sGUI_X_OPEN   (L"{{");
const wstring sGUI_X_CLOSE  (L"}}");


// --- attribute definitions ---

// new for Logoskop
#define A_BIBLE             "bible"
#define A_BOOK              "book"
#define A_CHAPTER           "chapter"
#define A_VERSE             "verse"
#define A_DATE              "date"
#define A_DTDVERS           "dtdvers"
#define A_ID                "id"
#define A_REV               "rev"
#define A_YEAR              "year"


#define A_B                 "b"
#define A_CH                "ch"
#define A_CH2               "ch"
#define A_CH3               "ch"
#define A_CH4               "ch"
#define A_DAY               "d"
#define A_DTDMAINVERS       "dtdmainvers"
#define A_DTDSUBVERS        "dtdsubvers"
#define A_MONTH             "m"
#define A_N                 "n"
#define A_R2                "r2"
#define A_R3                "r3"
#define A_R4                "r4"
#define A_TR                "tr"
#define A_V                 "v"
#define A_V2                "v2"
#define A_V3                "v3"
#define A_V4                "v4"
#define A_XMLLANG           "xml:lang"


#endif
