/* ----------------------------------------------------------------------
  Helmut Steeb
  $Id: bidiLigTable.h 3011 2008-11-29 09:37:12Z helmut $

  contains "lines", each line describes one ligature.
  The last "line" contains just 0.
  The ligature lines contain
  - a sequence of characters to be replaced (in visual order!)
  - separator 0
  - a sequence of characters to insert (in visual order!)
  - separator 0.
  NOTE (HS 000807):
  These ligatures are 1:1 from arabjoin.
  They could well be updated with data from the Unicode server.

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

// The following 8 obligatory LAM+ALEF ligatures are encoded in the
// U+FE70 Arabic Presentation Forms-B block in Unicode's
// compatibility zone:
0xfe82, 0xfedf, 0, 0xfef5, 0, 
0xfe82, 0xfee0, 0, 0xfef6, 0, 
0xfe84, 0xfedf, 0, 0xfef7, 0, 
0xfe84, 0xfee0, 0, 0xfef8, 0, 
0xfe88, 0xfedf, 0, 0xfef9, 0, 
0xfe88, 0xfee0, 0, 0xfefa, 0, 
0xfe8e, 0xfedf, 0, 0xfefb, 0, 
0xfe8e, 0xfee0, 0, 0xfefc, 0, 

// Bitstream's Cyberbit font offers 57 of the other 466 optional
// ligatures in the U+FB50 Arabic Presentation Forms-A block:

0xfee2, 0xfe97, 0, 0xfc0e, 0, 
0xfef2, 0xfed3, 0, 0xfc32, 0, 
0xfe9e, 0xfedf, 0, 0xfc3f, 0, 
0xfea2, 0xfedf, 0, 0xfc40, 0, 
0xfea6, 0xfedf, 0, 0xfc41, 0, 
0xfee2, 0xfedf, 0, 0xfc42, 0, 
0xfef0, 0xfedf, 0, 0xfc43, 0, 
0xfef2, 0xfedf, 0, 0xfc44, 0, 
0xfee2, 0xfee7, 0, 0xfc4e, 0, 
0x064c, 0x0651, 0, 0xfc5e, 0, 
0x064d, 0x0651, 0, 0xfc5f, 0, 
0x064e, 0x0651, 0, 0xfc60, 0, 
0x064f, 0x0651, 0, 0xfc61, 0, 
0x0650, 0x0651, 0, 0xfc62, 0, 
0xfeae, 0xfe92, 0, 0xfc6a, 0, 
0xfee6, 0xfe92, 0, 0xfc6d, 0, 
0xfef2, 0xfe92, 0, 0xfc6f, 0, 
0xfeae, 0xfe98, 0, 0xfc70, 0, 
0xfee6, 0xfe98, 0, 0xfc73, 0, 
0xfef2, 0xfe98, 0, 0xfc75, 0, 
0xfef2, 0xfee8, 0, 0xfc8f, 0, 
0xfeae, 0xfef4, 0, 0xfc91, 0, 
0xfee6, 0xfef4, 0, 0xfc94, 0, 
0xfea0, 0xfe91, 0, 0xfc9c, 0, 
0xfea4, 0xfe91, 0, 0xfc9d, 0, 
0xfea8, 0xfe91, 0, 0xfc9e, 0, 
0xfee4, 0xfe91, 0, 0xfc9f, 0, 
0xfea0, 0xfe97, 0, 0xfca1, 0, 
0xfea4, 0xfe97, 0, 0xfca2, 0, 
0xfea8, 0xfe97, 0, 0xfca3, 0, 
0xfee4, 0xfe97, 0, 0xfca4, 0, 
0xfee4, 0xfe9b, 0, 0xfca6, 0, 
0xfee4, 0xfe9f, 0, 0xfca8, 0, 
0xfee4, 0xfea3, 0, 0xfcaa, 0, 
0xfee4, 0xfea7, 0, 0xfcac, 0, 
0xfee4, 0xfeb3, 0, 0xfcb0, 0, 
0xfea0, 0xfedf, 0, 0xfcc9, 0, 
0xfea4, 0xfedf, 0, 0xfcca, 0, 
0xfea8, 0xfedf, 0, 0xfccb, 0, 
0xfee4, 0xfedf, 0, 0xfccc, 0, 
0xfeec, 0xfedf, 0, 0xfccd, 0, 
0xfea0, 0xfee3, 0, 0xfcce, 0, 
0xfea4, 0xfee3, 0, 0xfccf, 0, 
0xfea8, 0xfee3, 0, 0xfcd0, 0, 
0xfee4, 0xfee3, 0, 0xfcd1, 0, 
0xfea0, 0xfee7, 0, 0xfcd2, 0, 
0xfea4, 0xfee7, 0, 0xfcd3, 0, 
0xfea8, 0xfee7, 0, 0xfcd4, 0, 
0xfee4, 0xfee7, 0, 0xfcd5, 0, 
0xfea0, 0xfef3, 0, 0xfcda, 0, 
0xfea4, 0xfef3, 0, 0xfcdb, 0, 
0xfea8, 0xfef3, 0, 0xfcdc, 0, 
0xfee4, 0xfef3, 0, 0xfcdd, 0, 
0xfea4, 0xfee4, 0xfedf, 0, 0xfd88, 0, 
0xfeea, 0xfee0, 0xfedf, 0xfe8d, 0, 0xfdf2, 0, 
0xfee2, 0xfee0, 0xfeb3, 0xfeed, 0, 0xfeea, 0xfef4, 0xfee0, 0xfecb, 0, 
0xfeea, 0xfedf, 0xfe8e, 0xfee0, 0xfe9f, 0, 0xfede, 0xfe9f, 0, 
