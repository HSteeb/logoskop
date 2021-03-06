/* ======================================================================
  @brief Routines for Windows clipboard access.
  @author Helmut Steeb
  $Id: Clipboard.h 3905 2009-09-12 11:30:16Z helmut $

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
#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

namespace NClipboard {
  // Copy():
  // copies text as CF_UNICODETEXT and CF_TEXT.
  // Returns false on error.
  // If pbUsedDefaultChar is passed in, it is set accordingly:
  // it is set true iff. the conversion to CF_TEXT needs the "default character" 
  // (i.e. not all characters could be converted).
  bool Copy(const wstring &text, bool *pbUsedDefaultChar = 0);
}


#endif
