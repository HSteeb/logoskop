/* ----------------------------------------------------------------------
  Helmut Steeb
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
// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//

#if !defined(AFX_STDAFX_H__3C1C969F_A569_45BB_8E62_1CCC95913F95__INCLUDED_)
#define AFX_STDAFX_H__3C1C969F_A569_45BB_8E62_1CCC95913F95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Fügen Sie hier Ihre Header-Dateien ein
#define WIN32_LEAN_AND_MEAN		// Selten benutzte Teile der Windows-Header nicht einbinden

#include <windows.h> // for DLL creation stuff only


// ZU ERLEDIGEN: Verweisen Sie hier auf zusätzliche Header-Dateien, die Ihr Programm benötigt
// Helper for user defined warnings in build window
#define __STR2__(x) #x   
#define __STR1__(x) __STR2__(x)
#define __TODO__ __FILE__ "("__STR1__(__LINE__)") : todo : "
#define __TEST__ __FILE__ "("__STR1__(__LINE__)") : test : "

#pragma warning(disable:4786) // avoid annoying STL warning

#pragma warning(push,3)
#include <vector>
#include <string>
#include <map>
#ifndef _USE_MSG_BUF
#include <sstream> // ostringstream
#endif
#pragma warning(pop)

using std::string;
using std::wstring;

#ifdef _USE_MSG_BUF
#include "../messages/msgbuf.h"
#include "../messages/messages.h"
#else
#include "../messages/MsgSingleModule.h"
#endif

// HS 000722: for now, just use an ad hoc selected bit as group for encoding messages
#define ENCODING_MSG_GROUP (1<<15)

#define MSGERROR(msg) MSGBUFWRITE(msg, MSGLEVEL_ERROR, ENCODING_MSG_GROUP)
#define MSGINFO(msg)  MSGBUFWRITE(msg, MSGLEVEL_INFO,  ENCODING_MSG_GROUP)
#define MSGDEBUG(msg) MSGBUFWRITE(msg, MSGLEVEL_DEBUG, ENCODING_MSG_GROUP)


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt zusätzliche Deklarationen unmittelbar vor der vorherigen Zeile ein.

#endif // !defined(AFX_STDAFX_H__3C1C969F_A569_45BB_8E62_1CCC95913F95__INCLUDED_)
