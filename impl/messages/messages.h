/* ======================================================================
 @file messages.h
 @brief API of messages DLL
 @author Helmut Steeb
 @version $Id: messages.h 3014 2008-11-29 10:19:16Z helmut $

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

#ifdef MESSAGES_EXPORTS
#define MESSAGES_API __declspec(dllexport)
#else
#define MESSAGES_API __declspec(dllimport)
#endif

#define MSGLEVEL_ERROR 1
#define MSGLEVEL_INFO  2
#define MSGLEVEL_DEBUG 3

#define MSGALLGROUPS 0

#ifdef __cplusplus
extern "C" {
#endif

/** 
 MsgSetFilename: specifies the filename for message output.
 @param filename the name of the file to write
*/
MESSAGES_API void MsgSetFilename(const char *filename);

/** 
 MsgGetFilename: 
 @return pointer to filename
*/
MESSAGES_API const char *MsgGetFilename();

/** 
 MsgSetLevel: specifies the level for message output.
 @param level the maximum level that a message may have in order to
   be written. Messages with a greater level will be suppressed.
   Should be one of MSG_LEVEL_{ERROR,INFO,DEBUG}.
*/
MESSAGES_API void MsgSetLevel(const unsigned long level);

/** 
 MsgSetGroups: specifies the group for message output.
 @param group bit mask, a message must have one of this bits
   set in its group in order to be written. 
   For value 0, all messages will be written without respect for their group.
*/
MESSAGES_API void MsgSetGroups(const unsigned long groups);

/** 
 MsgSetFileLineNoOutput: specifies whether the file and lineNo parameters
 of MsgWrite are written.
 @param flag - if nonzero, file and lineNo will be written (if the
   file parameter to MsgWrite is non 0).
*/
MESSAGES_API void MsgSetFileLineNoOutput(const unsigned long flag);



/** 
 MsgWrite: reads a losung file into memory.
 @param msg is the message to write (0 is tolerated). 
 MsgWrite adds a linefeed so msg should not contain one.
 @param level is the level of the message
 @param group is the group of the message
 @param file is the name of the file emitting the message (e.g. C macro __FILE__).
 If 0, output of file and lineNo is suppressed.
 @param lineNo is the location in the file where the message was emitted (e.g. C macro __LINE__)
*/
MESSAGES_API void MsgWrite(
  const char *msg, 
  const unsigned long level, 
  const unsigned long group, 
  const char *file, 
  const unsigned long lineNo
  );

#ifdef __cplusplus
}
#endif
