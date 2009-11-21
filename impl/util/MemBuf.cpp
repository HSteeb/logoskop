/* ======================================================================
  @file MemBuf.cpp
  @brief Memory buffer
  @author Helmut Steeb
  $Id: MemBuf.cpp 3011 2008-11-29 09:37:12Z helmut $

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
#include "MemBuf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OMemBuf::OMemBuf()
: mpWideBufferStart(0),
  mpWideBufferEnd(0),
  mpByteBufferStart(0),
  mpByteBufferEnd(0)
{
}

OMemBuf::~OMemBuf()
{
  // avoid messages about memory leaks in debugger...
  Release();
}

wchar_t *OMemBuf::GetWideBuffer(const long nElements)
{
  if (mpWideBufferEnd - mpWideBufferStart < nElements) {
    mpWideBufferStart = (wchar_t*) realloc(mpWideBufferStart, nElements * sizeof(wchar_t));
    if (mpWideBufferStart) {
      mpWideBufferEnd = mpWideBufferStart + nElements;
    }
    else {
      MSGERROR("GetWideBuffer: failed to get buffer for " << nElements << " wide characters");
    }
  }
  return mpWideBufferStart;
}

char *OMemBuf::GetByteBuffer(const long nElements)
{
  if (mpByteBufferEnd - mpByteBufferStart < nElements) {
    mpByteBufferStart = (char*) realloc(mpByteBufferStart, nElements);
    if (mpByteBufferStart) {
      mpByteBufferEnd = mpByteBufferStart + nElements;
    }
    else {
      MSGERROR("GetByteBuffer: failed to get buffer for " << nElements << " bytes");
    }
  }
  return mpByteBufferStart;
}

void OMemBuf::Release()
{
  delete mpWideBufferStart;
  mpWideBufferStart = 0;
  mpWideBufferEnd = 0;

  delete mpByteBufferStart;
  mpByteBufferStart = 0;
  mpByteBufferEnd = 0;
}

wchar_t *OMemBuf::GetWideBufferEnd() const
{
  return mpWideBufferEnd;
}

char *OMemBuf::GetByteBufferEnd() const
{
  return mpByteBufferEnd;
}
