/* ======================================================================
 @brief Stores data for L
 @author Helmut Steeb
 @version $Id: Line.h 3853 2009-08-26 13:50:27Z helmut $

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
#ifndef _LINE_H_
#define _LINE_H_

#include "LosungObject.h"

/* ---------------------------
HS 2003-09-15:

defining _LINE_SAVE_SPACE_ saves about 200-300 KB of main memory
for one file which has been completely loaded (all days loaded)
in NT task manager:

With _LINE_SAVE_SPACE_ defined:

- one day loaded: ~ 2.8 MB
- year loaded   : ~ 3.2 - 3.3 MB

With _LINE_SAVE_SPACE_ undefined:

- one day loaded: ~ 2.8 MB
- year loaded   : ~ 3.5 MB

--------------------------- */

#define _LINE_SAVE_SPACE_ 1

// HS 2003-02-01: <!ENTITY % Line "(#PCDATA | X | EM)*">
// For now, just store #PCDATA...

class OLine : public OLosungObject
{
public:
#ifdef _LINE_SAVE_SPACE_
  OLine();
  OLine(const OLine &rhs);
  ~OLine();
#endif
  void Accept(OLosungObjectVisitor &vis);
  bool IsEmpty() const;
  wstring Content() const;
  void Content(const wstring &s);
  void Content(const wchar_t *pStart, const wchar_t *pEnd);
  const wchar_t *c_str() const;

private:
#ifdef _LINE_SAVE_SPACE_
  wchar_t *mpContent;
#else
  wstring mContent;
#endif
};

#endif
