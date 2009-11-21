/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop CSignatureWriter class.
  $Id: SignatureWriter.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include <assert.h>
#include "SignatureWriter.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// === ISignatureWriter ===

ISignatureWriter::~ISignatureWriter()
{
}

// === CSignatureWriter ===

class CSignatureWriter : public ISignatureWriter 
{
public:
  CSignatureWriter::CSignatureWriter();
  CSignatureWriter::~CSignatureWriter();
  virtual bool Write(const tstring &signature, const tstring &filename, const bool bAsUTF8 = true);
};



CSignatureWriter::CSignatureWriter()
{
}


CSignatureWriter::~CSignatureWriter()
{
}

bool CSignatureWriter::Write(const tstring &signature, const tstring &filename, const bool bAsUTF8)
{
  std::ofstream os;
  os.open(filename.c_str(), std::ios::trunc | std::ios::binary);
  if (!os.is_open()) {
    MSGERROR("Failed to write signature to file " << filename);
    return false;
  }
  os << signature;
  os.close();
  return true;
}


// === NSignatureWriter ===

ISignatureWriter &NSignatureWriter::MakeSignatureWriter()
{
  ISignatureWriter *pSignatureWriter = new CSignatureWriter();
  assert(pSignatureWriter);
  return *pSignatureWriter;
}