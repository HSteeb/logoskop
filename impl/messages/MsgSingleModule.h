/* ----------------------------------------------------------------------
  Helmut Steeb
  API for logging code that is not called via several DLLs.
  $Id: MsgSingleModule.h 3014 2008-11-29 10:19:16Z helmut $

 <P>Objects:</P>
 <DL>
 <DT><A name="level">level</A></DT>
   <DD>
   Each message has one of the levels ERROR, INFO, DEBUG.
   </DD>
 <DT><A name="group">group</A></DT>
   <DD>The program modules using this DLL may be divided into several groups.
   Each group may be identified by a bit in a value of type unsigned long.
   Each message emitted anywhere in the program has some group value,
   typically 0 (any group) or the group of its module.
   The main program may selectively filter some message groups for output.
   </DD>
 </DL>

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

#if !defined(_MSGSINGLEMODULE_H__INCLUDED_)
#define _MSGSINGLEMODULE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MSGLEVEL_ERROR 1
#define MSGLEVEL_INFO  2
#define MSGLEVEL_DEBUG 3

#define MSGALLGROUPS 0



namespace NMsgSingleModule {

/** 
 SetFilename: specifies the filename for message output.
 @param filename the name of the file to write
*/
void SetFilename(const char *filename);

/** 
 GetFilename: 
 @return pointer to filename
*/
const char *GetFilename();

/** 
 SetLevel: specifies the level for message output.
 @param level the maximum level that a message may have in order to
   be written. Messages with a greater level will be suppressed.
   Should be one of MSG_LEVEL_{ERROR,INFO,DEBUG}.
*/
void SetLevel(const unsigned long level);

/** 
 SetGroups: specifies the group for message output.
 @param group bit mask, a message must have one of this bits
   set in its group in order to be written. 
   For value 0, all messages will be written without respect for their group.
*/
void SetGroups(const unsigned long groups);

/** 
 SetFileLineNoOutput: specifies whether the file and lineNo parameters
 of Write are written.
 @param flag - if nonzero, file and lineNo will be written (if the
   file parameter to Write is non 0).
*/
void SetFileLineNoOutput(const unsigned long flag);
}



namespace _NMsgSingleModule {

// === output ===

std::ostringstream &GetStringBufferAtStart();
std::ostringstream &GetStringBuffer();


/** 
Write: writes the contents of GetStringBuffer() to the logfile.
@param level of this message
@param group of this message
@param file name of the file emitting the message
@param lineNo location in the file emitting the message
*/
void Write(
    const unsigned long level, 
    const unsigned long group, 
    const char *file, 
    const unsigned long lineNo);


}

#define MSGBUFWRITE(msg, level, group) \
  _NMsgSingleModule::GetStringBufferAtStart() << msg << std::ends, \
  _NMsgSingleModule::Write(level, group, __FILE__, __LINE__)

#endif
