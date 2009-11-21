/* ----------------------------------------------------------------------
  Helmut Steeb
  API for logging code that is not called via several DLLs.
  $Id: MsgSingleModule.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include <fstream>
#include <string>
#include <iomanip>
#include "MsgSingleModule.h"

typedef enum {
  stateUndef,
  stateOpen,
  stateError
} TOutputState;

static std::string sFilename;
static std::ofstream sOfstream;
static unsigned long sLevel = MSGLEVEL_INFO; // just a default
static TOutputState sOutputState;
static unsigned long sGroups;
static bool sbFileLineNoOutput;


void NMsgSingleModule::SetFilename(const char *filename)
{
  if (sOutputState == stateOpen) {
    sOfstream.close();
    sOutputState = stateUndef;
  }
  sFilename = filename;
}

const char *NMsgSingleModule::GetFilename()
{
  return sFilename.c_str();
}

void NMsgSingleModule::SetLevel(const unsigned long level)
{
  sLevel = level;
}

void NMsgSingleModule::SetGroups(const unsigned long groups)
{
  sGroups = groups;
}


void NMsgSingleModule::SetFileLineNoOutput(const unsigned long flag)
{
  sbFileLineNoOutput = flag != 0;
}


// === output ===

static std::ostringstream sBuf; // the buffer for messages

std::ostringstream &_NMsgSingleModule::GetStringBufferAtStart()
{
  sBuf.seekp(0, std::ostream::beg); 
  return sBuf;
}

std::ostringstream &_NMsgSingleModule::GetStringBuffer()
{
  return sBuf;
}


void _NMsgSingleModule::Write(
  const unsigned long level, 
  const unsigned long group, 
  const char *file, 
  const unsigned long lineNo
)
{
  if ( (level <= sLevel) && (sGroups == MSGALLGROUPS || group & sGroups) ) {
    if (sOutputState == stateUndef && !sFilename.empty()) {
      // HS 2004-11-03: using binary is probably more efficient (no "\n" to "\r\n" conversion)
      sOfstream.open(sFilename.c_str(), std::ios::trunc /* or ate*/ | std::ios::binary);
      sOutputState = sOfstream.is_open()? stateOpen : stateError;
    }
    if (sOutputState == stateOpen) {
      char t = '?';
      switch (level) {
      case MSGLEVEL_ERROR: t = 'E'; break;
      case MSGLEVEL_INFO:  t = 'I'; break;
      case MSGLEVEL_DEBUG: t = 'D'; break;
      default: ;
      }
      sOfstream << t << ": ";
      if (sbFileLineNoOutput && file) {
        sOfstream << file << ':' << lineNo << ' ';
      }
      // HS 2006-01-17: with std::endl, only \n is written.
      // Displaying such a logfile with Windows XP Editor messes up lines!
      sOfstream << sBuf.str() << "\r\n" << std::flush;
    }
  }
}

