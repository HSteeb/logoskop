/* ----------------------------------------------------------------------
  Helmut Steeb
  stdafx.h : Include-Datei für Standard-System-Include-Dateien,
    oder projektspezifische Include-Dateien, die häufig benutzt, aber
    in unregelmäßigen Abständen geändert werden.
  $Id: stdafx.h 3011 2008-11-29 09:37:12Z helmut $

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

#if !defined(AFX_STDAFX_H__130B4F24_B3E4_11D3_AFB9_865DDA2CCD25__INCLUDED_)
#define AFX_STDAFX_H__130B4F24_B3E4_11D3_AFB9_865DDA2CCD25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning(disable:4786) // avoid annoying STL warning
#pragma warning(push,3)
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#pragma warning(pop)

#include "../util/tstring.h"

using std::string;
using std::wstring;
using std::wostringstream;
using std::vector;


#include <winver.h>

#define __STR2__(x) #x   
#define __STR1__(x) __STR2__(x)
#define __TODO__ __FILE__ "("__STR1__(__LINE__)") : todo : "

#include "../messages/MsgSingleModule.h"

#define W32LOS_MSG_GROUP (1<<1)

#define MSGERROR(msg) MSGBUFWRITE(msg, MSGLEVEL_ERROR, W32LOS_MSG_GROUP)
#define MSGINFO(msg)  MSGBUFWRITE(msg, MSGLEVEL_INFO,  W32LOS_MSG_GROUP)
#define MSGDEBUG(msg) MSGBUFWRITE(msg, MSGLEVEL_DEBUG, W32LOS_MSG_GROUP)

#ifdef _DEBUG
#define _TEST
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_STDAFX_H__130B4F24_B3E4_11D3_AFB9_865DDA2CCD25__INCLUDED_)
