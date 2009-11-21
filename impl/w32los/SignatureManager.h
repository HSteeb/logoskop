/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop CSignatureManager class.

  $Id: SignatureManager.h 3014 2008-11-29 10:19:16Z helmut $

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

#if !defined(AFX_SIGNATUREMANAGER_H__INCLUDED_)
#define AFX_SIGNATUREMANAGER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OLosung;
class CProfile;

class ISignatureManager {
public:
  virtual ~ISignatureManager();

  // --- IsSignatureOn ---
  // IsSignatureOn reflects the global INI file setting.
  virtual bool IsSignatureOn() = 0;
  virtual void IsSignatureOn(bool bIsOn) = 0;

  // --- SignatureAsUtf8 ---
  // SignatureAsUtf8 reflects the global INI file setting.
  virtual bool SignatureAsUtf8() = 0;
  virtual void SignatureAsUtf8(bool bIsOn) = 0;


  // --- Template ---

  // GetTemplateFilename:
  // returns the name of the template file
  virtual tstring GetTemplateFilename() = 0;

  // GetTemplate:
  // returns the content of the template file (ready for Windows - containing "\r\n").
  // If no or empty filename passed in, uses result from GetTemplateFilename().
  virtual tstring GetTemplate(const tstring &filename = _T("")) = 0;

  // SetTemplate:
  // stores templateString into the file named GetTemplateFilename
  // returns true iff. sucessful.
  // Expects templateString in Windows editfield convention (containing "\r\n"),
  //   but tolerates sole "\n" also.
  virtual bool SetTemplate(const tstring &templateString) = 0;


  // --- Target ---

  // GetTargetFilename:
  // returns the name of the target file
  virtual tstring GetTargetFilename() = 0;

  // GetTarget:
  // returns the contents of the target file (ready for Windows - containing "\r\n").
  // If no or empty filename passed id, uses result from GetTargetFilename().
  virtual tstring GetTarget(const tstring &filename = _T("")) = 0;

  virtual void SetTargetFilename(const tstring &target) = 0;



  // Format:
  // fetches PRF_SIGNATURE_LINESEP and formats pLosung into signatureTemplate,
  // returns the result.
  // Formatting actually done by a formatter 
  // created using NSignatureFormatter::MakeSignatureFormatter().
  virtual tstring Format(OLosung *pLosung, const tstring &signatureTemplate) = 0;


  // Write:
  // if GetTargetFilename set and GetTemplate() is not empty, 
  // calls Format() and writes signature into GetTargetFilename().
  // Tolerates pLosung==0 (behaviour to be defined by implementation).
  virtual bool Write(OLosung *pLosung) = 0;

  // Exec:
  // if IsSignatureOn(), does Write() and Write() result,
  // otherwise returns false.
  virtual bool Exec(OLosung *pLosung) = 0;

};

class ISignatureFormatter;

namespace NSignatureManager {
  // resulting ISignatureManager must be freed by caller
  ISignatureManager &MakeSignatureManager(CProfile &profile, const tstring &baseDir, ISignatureFormatter &formatter);
};

#endif
