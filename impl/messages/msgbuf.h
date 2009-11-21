/* ======================================================================
 @file msgBuf.h
 @brief Buffer that forwards streamed messages to a C string based callback
 @author Helmut Steeb
 @version $Id: msgbuf.h 3014 2008-11-29 10:19:16Z helmut $

NOTE: the project's stdafx.h must contain

#include "../messages/msgBuf.h"
#include "../messages/messages.h"
#define BIT ... (e.g. 1 - for identifying messages emitted here)
#define MSGERROR(msg) MSGBUFWRITE(msg, MSGLEVEL_ERROR, BIT)
#define MSGINFO(msg)  MSGBUFWRITE(msg, MSGLEVEL_INFO,  BIT)
#define MSGDEBUG(msg) MSGBUFWRITE(msg, MSGLEVEL_DEBUG, BIT)

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
#ifndef _MSGBUF_H_
#define _MSGBUF_H_

#include "messages.h"

#pragma warning(push,3)
#include <sstream> // ostringstream
#pragma warning(pop)

typedef void (*TMessageCallback)(
  const char *msg,
  const unsigned long level, 
  const unsigned long group, 
  const char *file, 
  const unsigned long lineNo);


namespace msgBuf {

/** 
SetMessageCallback: sets a callback that will be called for reporting messages.
@param messageCallback will be called (may be 0 to reset callback)
@return  
- a FILEHANDLE on success
- <= 0 on error, 
*/
void SetMessageCallback(TMessageCallback messageCallback);
  
/** 
Write: writes the contents of ostringstream ostr to the callback
set by SetMessageCallback (if any).
@param ostr contains message to write (end of line will be added)
@param level of this message (see messages dll)
@param group of this message (see messages dll)
@param file name of the file emitting the message
@param lineNo location in the file emitting the message
*/
void Write(std::ostringstream &ostr,
    const unsigned long level, 
    const unsigned long group, 
    const char *file, 
    const unsigned long lineNo);
std::ostringstream &GetStringBufferAtStart();
std::ostringstream &GetStringBuffer();

/** 
GetDLLForwarder: 
@return returns a function pointer that, when called,
will simply forward messages to the messages.dll (if available).
Use like:
  TMessageCallback dllForwarder = msgBuf::GetDLLForwarder();
  msgBuf::SetMessageCallback(dllForwarder);
  TransmitMessageFilename("xx.log");
  MsgBox("Your logfile is " + RetrieveMessageFilename());
*/
TMessageCallback GetDLLForwarder();

const char *RetrieveMessageFilename();

void SetLevel(const unsigned long level);

void TransmitMessageFilename(const char *filename);


}

#define MSGBUFWRITE(msg, level, group) \
  msgBuf::GetStringBufferAtStart() << msg << std::ends, \
  msgBuf::Write(msgBuf::GetStringBuffer(), level, group, __FILE__, __LINE__)


#endif
