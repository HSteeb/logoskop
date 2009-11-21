/* ======================================================================
 @file msgBuf.cpp
 @brief Buffer that forwards streamed messages to a C string based callback
 @author Helmut Steeb
 @version $Id: msgBuf.cpp 3014 2008-11-29 10:19:16Z helmut $

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

#include "stdafx.h"
#include <string>
#include "msgBuf.h"

#pragma warning(push,3)
#include <fstream> // ofstream
#pragma warning(pop)

#ifndef _T
#ifdef UNICODE
#define _T(s) L##s
#else
#define _T(s) (s)
#endif
#endif

// --- General routines

static TMessageCallback sMessageCallback = 0;
static std::ostringstream sBuf; // the buffer for messages

void msgBuf::SetMessageCallback(TMessageCallback messageCallback)
{
  sMessageCallback = messageCallback;
}

void msgBuf::Write(std::ostringstream &ostr,
  const unsigned long level, 
  const unsigned long group, 
  const char *file, 
  const unsigned long lineNo
)
{
  if (sMessageCallback) {
    sMessageCallback(ostr.str().c_str(), level, group, file, lineNo); // str() returns a copy!
  }
}

std::ostringstream &msgBuf::GetStringBufferAtStart()
{
  sBuf.seekp(0, std::ostream::beg); 
  return sBuf;
}

std::ostringstream &msgBuf::GetStringBuffer()
{
  return sBuf;
}

// === predefined message callback for forwarding to messages.dll ===

typedef void    (*fMsgSetFilename) (const char *filename);
typedef const char *(*fMsgGetFilename) ();
typedef HRESULT (*fMsgWrite) (const char *msg, const unsigned long level, const unsigned long group, const char *file, const unsigned long lineNo);
typedef void    (*fMsgSetLevel) (const unsigned long level);

static HINSTANCE        hMessages = 0;

static fMsgSetFilename  pMsgSetFilename  = 0;
static fMsgGetFilename  pMsgGetFilename  = 0;
static fMsgWrite        pMsgWrite  = 0;
static fMsgSetLevel     pMsgSetLevel = 0;
static bool             mustInit = true;

void Init()
{
  // simply forward to messages DLL
  hMessages = LoadLibrary(_T("messages.dll"));
  if (hMessages) {
    
    // MsgWrite
    pMsgWrite  = (fMsgWrite) GetProcAddress(hMessages, _T("MsgWrite"));
    if (!pMsgWrite) {
      DWORD lastError = GetLastError(); // for debugger
    }
    
    // MsgSetFilename
    pMsgSetFilename  = (fMsgSetFilename) GetProcAddress(hMessages, _T("MsgSetFilename"));
    if (!pMsgSetFilename) {
      DWORD lastError = GetLastError(); // for debugger
    }
    
    // MsgGetFilename
    pMsgGetFilename  = (fMsgGetFilename) GetProcAddress(hMessages, _T("MsgGetFilename"));
    if (!pMsgGetFilename) {
      DWORD lastError = GetLastError(); // for debugger
    }
    
    // MsgSetLevel
    pMsgSetLevel = (fMsgSetLevel) GetProcAddress(hMessages, _T("MsgSetLevel"));
    if (!pMsgSetLevel) {
      DWORD lastError = GetLastError(); // for debugger
    }
    else {
      pMsgSetLevel(MSGLEVEL_DEBUG);
    }
    
  }
}

const char *msgBuf::RetrieveMessageFilename()
{
  if (mustInit) {
    Init();
    mustInit = false;
  }
  if (pMsgGetFilename) {
    return pMsgGetFilename();
  }
  return 0;
}

void msgBuf::SetLevel(const unsigned long level)
{
  if (pMsgSetLevel) {
    pMsgSetLevel(level);
  }
}

void msgBuf::TransmitMessageFilename(const char *filename)
{
  if (mustInit) {
    Init();
    mustInit = false;
  }
  if (pMsgSetFilename) {
    pMsgSetFilename(filename);
  }
}

void DLLForwarder(
  const char *msg,
  const unsigned long level, 
  const unsigned long group, 
  const char *file, 
  const unsigned long lineNo)
{
  if (mustInit) {
    Init();
    mustInit = false;
  }
  if (pMsgWrite) {
    pMsgWrite(msg, level, group, file, lineNo);
  }

}

TMessageCallback msgBuf::GetDLLForwarder()
{
  return DLLForwarder;
}

