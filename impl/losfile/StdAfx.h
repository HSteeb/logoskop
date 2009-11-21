/* ----------------------------------------------------------------------
 Helmut Steeb
  stdafx.h : Include-Datei für Standard-System-Include-Dateien,
    oder projektspezifische Include-Dateien, die häufig benutzt, aber
    in unregelmäßigen Abständen geändert werden.
  $Id: StdAfx.h 3014 2008-11-29 10:19:16Z helmut $

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

#if !defined(AFX_STDAFX_H__681D76C2_BC41_11D7_82C3_EC74B972C82B__INCLUDED_)
#define AFX_STDAFX_H__681D76C2_BC41_11D7_82C3_EC74B972C82B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning(disable:4786) // avoid annoying STL warning
#pragma warning(push,3)
#include <vector>
#include <set>
#include <string>
#ifndef _USE_MSG_BUF
#include <sstream> // ostringstream
#endif
#include <fstream> // ofstream
#include <iomanip>
#include "../util/tstring.h"
#pragma warning(pop)



using std::string;
using std::wstring;
using std::wostringstream;
using std::vector;


#define __STR2__(x) #x   
#define __STR1__(x) __STR2__(x)
#define __TODO__ __FILE__ "("__STR1__(__LINE__)") : todo : "

#ifdef _USE_MSG_BUF
#include "../messages/msgbuf.h"
#include "../messages/messages.h"
#else
#include "../messages/MsgSingleModule.h"
#endif

#define LOSFILE_MSG_GROUP (1<<2)

#define MSGERROR(msg) MSGBUFWRITE(msg, MSGLEVEL_ERROR, LOSFILE_MSG_GROUP)
#define MSGINFO(msg)  MSGBUFWRITE(msg, MSGLEVEL_INFO,  LOSFILE_MSG_GROUP)
#define MSGDEBUG(msg) MSGBUFWRITE(msg, MSGLEVEL_DEBUG, LOSFILE_MSG_GROUP)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__681D76C2_BC41_11D7_82C3_EC74B972C82B__INCLUDED_)
