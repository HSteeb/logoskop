/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop CSignatureManager class.

  For Windows edit-fields, provides "\r\n" in GetTemplate() and GetTarget().
  Saves template (in SetTemplate()) without "\r\n".
  Saves signature (in Exec()) *with* "\r\n" (for Windows programs using the signature).


   $Id: SignatureManager.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include "SignatureManager.h"
#include "SignatureFormatter.h"
#include "SignatureWriter.h"
#include "../util/Profile.h"
#include "../util/FileUtil.h"
#include "../util/ProfileDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// === ISignatureManager ===

ISignatureManager::~ISignatureManager()
{
}

// === CSignatureManager ===

class CSignatureManager : public ISignatureManager 
{
public:
  CSignatureManager::CSignatureManager(CProfile &profile, const tstring &baseDir, ISignatureFormatter &formatter);
  CSignatureManager::~CSignatureManager();


  virtual bool IsSignatureOn();
  virtual void IsSignatureOn(bool bIsOn);

  virtual bool SignatureAsUtf8();
  virtual void SignatureAsUtf8(bool bIsOn);

  virtual tstring GetTemplateFilename();
  virtual tstring GetTemplate(const tstring &filename = _T(""));
  virtual bool SetTemplate(const tstring &templateString);

  virtual tstring GetTargetFilename();
  virtual tstring GetTarget(const tstring &filename = _T(""));
  virtual void SetTargetFilename(const tstring &target);

  virtual tstring Format(OLosung *pLosung, const tstring &signatureTemplate);
  virtual bool Write(OLosung *pLosung);

  virtual bool Exec(OLosung *pLosung);
protected:
  virtual bool Replace(tstring &s, const tstring &key, const tstring &value);

private:
  CProfile &mProfile;
  tstring mSignatureTemplate;
  tstring mBaseDir;
  bool mbIsSignatureOn;
  bool mbSignatureAsUtf8;
  ISignatureFormatter &mFormatter;
};



CSignatureManager::CSignatureManager(CProfile &profile, const tstring &baseDir, ISignatureFormatter &formatter)
: mProfile(profile),
  mBaseDir(baseDir),
  mbIsSignatureOn(false),
  mbSignatureAsUtf8(false),
  mFormatter(formatter)
{
}


CSignatureManager::~CSignatureManager()
{
}

bool CSignatureManager::IsSignatureOn()
{
  return mProfile.GetBool(PRF_SIGNATURE_ON, DEFAULT_SIGNATURE_ON);
}
 
void CSignatureManager::IsSignatureOn(bool bIsOn)
{
  mProfile.WriteBool(PRF_SIGNATURE_ON, bIsOn);
}

bool CSignatureManager::SignatureAsUtf8()
{
  return mProfile.GetBool(PRF_SIGNATURE_AS_UTF8, DEFAULT_SIGNATURE_AS_UTF8);
}
 
void CSignatureManager::SignatureAsUtf8(bool bIsOn)
{
  mProfile.WriteBool(PRF_SIGNATURE_AS_UTF8, bIsOn);
}

tstring CSignatureManager::GetTemplateFilename()
{
  tstring signatureTemplateFilename;
  if (0 >= mProfile.GetString(PRF_SIGNATURE_TEMPLATE, _T("signatureTemplate.txt"), /*adlib*/_MAX_PATH, signatureTemplateFilename)) {
    signatureTemplateFilename = _T("");
  }
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  char ext[_MAX_EXT];
  char fname[_MAX_FNAME];
  _splitpath(signatureTemplateFilename.c_str(), drive, dir, fname, ext);  
  tstring driveDir = tstring(drive) + dir;
  if (driveDir.empty()) {
    signatureTemplateFilename = mBaseDir + signatureTemplateFilename;
  }
  return signatureTemplateFilename;
}

tstring CSignatureManager::GetTemplate(const tstring &signatureTemplateFilename)
{
  tstring filename;
  // supply default argument
  if (signatureTemplateFilename.empty()) {
    filename = GetTemplateFilename();
  }
  else {
    filename = signatureTemplateFilename;
  }

  // check filename
  if (filename.empty()) {
    MSGERROR("Reading signature template file failed - no signature template filename");
    return _T("");
  }
  return FileUtil::Moronize(FileUtil::Read(filename));
}

bool CSignatureManager::SetTemplate(const tstring &templateString)
{
  tstring signatureTemplateFilename(GetTemplateFilename());
  if (signatureTemplateFilename.empty()) {
    MSGERROR("Writing signature template file failed - no signature template filename");
    return false;
  }
  return FileUtil::Write(signatureTemplateFilename, FileUtil::DeMoronize(templateString));
}

tstring CSignatureManager::GetTargetFilename()
{
  tstring signatureTargetFilename;
  if (0 >= mProfile.GetString(PRF_SIGNATURE_TARGET, "", /*adlib*/_MAX_PATH, signatureTargetFilename)) {
    signatureTargetFilename = _T("");
  }
  return signatureTargetFilename;
}

tstring CSignatureManager::GetTarget(const tstring &signatureTargetFilename)
{
  tstring filename;
  // supply default argument
  if (signatureTargetFilename.empty()) {
    filename = GetTargetFilename();
  }
  else {
    filename = signatureTargetFilename;
  }

  // check filename
  tstring signatureTarget;
  if (filename.empty()) {
    MSGERROR("Reading signature target file failed - no signature target filename");
    return _T("");
  }
  return FileUtil::Moronize(FileUtil::Read(filename));
}

void CSignatureManager::SetTargetFilename(const tstring &target)
{
  mProfile.WriteString(PRF_SIGNATURE_TARGET, target);
}

bool CSignatureManager::Exec(OLosung *pLosung)
{
  // Algorithm (numbers) see "003 Create EMail signature.sxw"

  // 1. SignatureOn turned off -> skip
  if (!IsSignatureOn()) {
    MSGINFO("Skipping email signature generation due to profile setting " PRF_SIGNATURE_ON);
    return false;
  }
  return Write(pLosung);
}

bool CSignatureManager::Replace(tstring &s, const tstring &key, const tstring &value)
{
  unsigned int pos;
  bool bModified = false;
  while (tstring::npos != (pos = s.find(key))) {
    s.erase(pos, key.length());
    s.insert(pos, value);
    bModified = true;
  }
  return bModified;
}

tstring CSignatureManager::Format(OLosung *pLosung, const tstring &signatureTemplate)
{
  // get separator for formatter
  tstring lineSeparator;
  // NOTE:
  // - line separator setting not given in profile        --> lineSeparator = "\n"
  // - line separator setting given in profile, but empty --> lineSeparator = ""
  // NOTE (HS 2004-12-24):
  // a default of "\n" is suppressed under Windows 95 
  // (MSDN about "GetPrivateProfileString": trailing "blanks", actually *whitespace*, is dropped! See GetString().)
  mProfile.GetString(PRF_SIGNATURE_LINESEP, "\\n", /*adlib*/_MAX_PATH, lineSeparator);

  // accept literal "\r", "\n", "\t" in string from profile (i.e. 2 characters '\' + 'r' etc.)
  Replace(lineSeparator, _T("\\r"), _T("\r"));
  Replace(lineSeparator, _T("\\n"), _T("\n"));
  Replace(lineSeparator, _T("\\t"), _T("\t"));
  
  // be defensive
  lineSeparator = FileUtil::Moronize(lineSeparator);

  // 5. format Losung into SignatureTemplate
  MSGINFO("Formatting email signature...");
  mFormatter.SetAsUtf8(SignatureAsUtf8());
  tstring signature = mFormatter.Format(pLosung, signatureTemplate, lineSeparator);

  return signature;
}

bool CSignatureManager::Write(OLosung *pLosung)
{
  // Algorithm (numbers) see "003 Create EMail signature.sxw"

  // 2. SignatureTarget undefined -> skip
  tstring signatureTargetFilename(GetTargetFilename());
  if (signatureTargetFilename.empty()) {
    MSGINFO("Skipping email signature generation due to empty profile setting " PRF_SIGNATURE_TARGET);
    return false;
  }

  // 3. SignatureTemplate undefined -> skip
  tstring signatureTemplateFilename(GetTemplateFilename());
  if (signatureTemplateFilename.empty()) {
    MSGINFO("Skipping email signature generation due to empty profile setting " PRF_SIGNATURE_TEMPLATE);
    return false;
  }

  // 4. if SignatureTemplate does not exist -> skip
  tstring signatureTemplate(GetTemplate());
  if (signatureTemplate.empty()) {
    MSGINFO("Skipping email signature generation due to missing template file " << signatureTemplateFilename);
    return false;
  }

  // continue even is signature is empty
  tstring signature(Format(pLosung, signatureTemplate));

  // 7. write to file (even if signature is empty - guarantee to refresh file that contains old text!)
  MSGINFO("Writing email signature to file " << signatureTargetFilename << "...");
  ISignatureWriter &writer = NSignatureWriter::MakeSignatureWriter();
  bool bWritten = writer.Write(signature, signatureTargetFilename, /*asUTF8:*/ false);
  delete &writer;

  if (!bWritten) {
    MSGERROR("Failed to write email signature to file " << signatureTargetFilename);
  }
  return bWritten;
}


// === NSignatureManager ===

ISignatureManager &NSignatureManager::MakeSignatureManager(CProfile &profile, const tstring &baseDir, ISignatureFormatter &formatter)
{
  ISignatureManager *pSignatureManager = new CSignatureManager(profile, baseDir, formatter);
  assert(pSignatureManager);
  return *pSignatureManager;
}
