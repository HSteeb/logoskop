/* ======================================================================
 @file LosFileStack.h
 @brief C++ stack for losfile xml files using expat and OXMLStack
 @author Helmut Steeb
 @version $Id: LosFileStack.h 3014 2008-11-29 10:19:16Z helmut $

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
#ifndef __LOSFILESTACK_H__
#define __LOSFILESTACK_H__

#include "../Util/XMLStack.h"
#include "LosFile.h"


class OLosFileStack;

class OEncodingClient;

class OBibleNameHandler;
class OBibleCopyrightHandler;
class OCHandler;
class OEMHandler;
class OFileVersHandler;
class OHeadHandler;
class OILHandler;
class OLHandler;
class OLosFileHandler;
class OLosungHandler;
class OParolHandler;
class OSHandler;
class OSLHandler;
class OTLHandler;
class OXHandler;
class OXLatHandler;


// --- OLosFileBaseHandler ---

class OLosFileBaseHandler : public OBaseHandler {
  typedef OBaseHandler inherited;
public:
  OLosFileBaseHandler() : mpLosFileStack(0) {};
  virtual ~OLosFileBaseHandler() {};
  virtual void PreStartTag(OXMLStack *pXMLStack);
  OLosFileBaseHandler *GetParent();
  OLosFileStack &GetLosFileStack();

  virtual void AcceptBeingParentOf(OLosFileBaseHandler&);
  virtual void AcceptBeingParentOf(OBibleNameHandler&){ UnhandledChild(_T("biblename")); }
  virtual void AcceptBeingParentOf(OBibleCopyrightHandler&) { UnhandledChild(_T("biblecopyright")); }
  virtual void AcceptBeingParentOf(OCHandler&)        { UnhandledChild(_T("c")); }
  virtual void AcceptBeingParentOf(OEMHandler&)       { UnhandledChild(_T("em")); }
  virtual void AcceptBeingParentOf(OFileVersHandler&) { UnhandledChild(_T("updated")); }
  virtual void AcceptBeingParentOf(OHeadHandler&)     { UnhandledChild(_T("head")); }
  virtual void AcceptBeingParentOf(OILHandler&)       { UnhandledChild(_T("intro")); }
  virtual void AcceptBeingParentOf(OLHandler&)        { UnhandledChild(_T("text")); }
  virtual void AcceptBeingParentOf(OLosFileHandler&)  { UnhandledChild(_T("thewordfile")); }
  virtual void AcceptBeingParentOf(OLosungHandler&)   { UnhandledChild(_T("theword")); }
  virtual void AcceptBeingParentOf(OParolHandler&)       { UnhandledChild(_T("parol")); }
  virtual void AcceptBeingParentOf(OSHandler&)        { UnhandledChild(_T("S")); }
  virtual void AcceptBeingParentOf(OSLHandler&)       { UnhandledChild(_T("ref")); }
  virtual void AcceptBeingParentOf(OTLHandler&)       { UnhandledChild(_T("title")); }
  virtual void AcceptBeingParentOf(OXHandler&)        { UnhandledChild(_T("X")); }
  virtual void AcceptBeingParentOf(OXLatHandler&)     { UnhandledChild(_T("copyright")); }
  OLosFileStack *mpLosFileStack;
private:
  void UnhandledChild(const TCHAR *pTag);
};


// --- OLosFileStack ---

class OLosFileStack : public OXMLStack {
  typedef OXMLStack inherited;
public:
  OLosFileStack(OLosFile &losfile);
  ~OLosFileStack();

  OLosFileBaseHandler *GetParent(); // 0 if stacksize < 2, else pointer to parent

  virtual const void ReportErrorAttNotFound(const TCHAR *pAtt);
  virtual const void ReportErrorAttNotLong(const TCHAR *pAtt, const TCHAR *pValue);

  OLosFile &GetLosFile();

  OLosFile &mLosFile;

  // returns pointer to static buffer
  wchar_t *ToUTF16(const tstring &s);

  bool HandleUnknownEncoding(const XML_Char *name, XML_Encoding *info);

  // mpLosung:
  // pointer to current OLosung (= current day), 0 if outside LOSUNG.
  // Setting the pointer within OLosFileStack saves us 
  // copying it down all the element handlers.
  OLosung *mpLosung; 

  void SetDayToRead(unsigned int monthToRead, unsigned int dayToRead);
  bool IsDayToRead(unsigned int monthToRead, unsigned int dayToRead);

private:
  // prevent from using copy constructor / assignment
  OLosFileStack(const OLosFileStack&);
  OLosFileStack& operator=(const OLosFileStack&);

  unsigned int mMonthToRead; // 0 = none
  unsigned int mDayToRead;   // 0 = none
};



// --- Element handlers ---


// base classes:

class OCharDataHandler : public OLosFileBaseHandler {
public:
  virtual ~OCharDataHandler() {};
  virtual void CharData(OXMLStack*);
  tstring &CharData();
};

class OLineHandler : public OCharDataHandler {
public:
  OLineHandler();
  virtual ~OLineHandler() {};

  virtual void EndTag(OXMLStack*);
  virtual void AcceptBeingParentOf(OEMHandler&);
  virtual void AcceptBeingParentOf(OXHandler&);
  OLine *mpLine;
};

class OUnknownTagHandler : public OLosFileBaseHandler {
public:
  virtual ~OUnknownTagHandler() {};
  virtual void StartTag(OXMLStack*);
};


// alphabetically:

class OBibleNameHandler : public OCharDataHandler {
public:
  virtual ~OBibleNameHandler() {};
  virtual void EndTag(OXMLStack*);
};

class OBibleCopyrightHandler : public OCharDataHandler {
public:
  virtual ~OBibleCopyrightHandler() {};
  virtual void EndTag(OXMLStack*);
  virtual void VisitAsChildOfXLatHandler(OXLatHandler &);
};

class OCHandler : public OLineHandler {
public:
  virtual ~OCHandler() {};
  virtual void StartTag(OXMLStack*);
  virtual void VisitAsChildOfLosungHandler(OLosungHandler&);
};

class OEMHandler : public OCharDataHandler {
public:
  virtual ~OEMHandler() {};
  virtual void EndTag(OXMLStack*);
  virtual void VisitAsChildOfLineHandler(OLineHandler&);
};

class OFileVersHandler : public OCharDataHandler {
public:
  virtual ~OFileVersHandler() {};
  virtual void EndTag(OXMLStack*);
};

class OHeadHandler : public OLosFileBaseHandler {
public:
  virtual ~OHeadHandler() {};
  virtual void StartTag(OXMLStack*);
  virtual void VisitAsChildOfLosFileHandler(OLosFileHandler&);
};

class OILHandler : public OLineHandler {
public:
  virtual ~OILHandler() {};
  virtual void StartTag(OXMLStack*);
  virtual void VisitAsChildOfParolHandler(OParolHandler&);
};

class OLHandler : public OLineHandler {
public:
  virtual ~OLHandler() {};
  virtual void StartTag(OXMLStack*);
  virtual void VisitAsChildOfParolHandler(OParolHandler&);
};

class OLosFileHandler : public OLosFileBaseHandler {
public:
  virtual ~OLosFileHandler() {};

  virtual void StartTag(OXMLStack*);
  virtual void AcceptBeingParentOf(OHeadHandler&);
};

class OLosungHandler : public OLosFileBaseHandler {
public:
  OLosungHandler();
  virtual ~OLosungHandler() {};

  virtual void StartTag(OXMLStack*);
  virtual void EndTag(OXMLStack*);
  virtual void AcceptBeingParentOf(OCHandler&);
  virtual void AcceptBeingParentOf(OParolHandler&);
  virtual void AcceptBeingParentOf(OTLHandler&);
  int mMonth;
  int mDay;
  OLosung *mpLosung;
};

class OParolHandler : public OLosFileBaseHandler {
public:
  OParolHandler();
  virtual ~OParolHandler() {};

  virtual void StartTag(OXMLStack*);
  virtual void AcceptBeingParentOf(OILHandler&);
  virtual void AcceptBeingParentOf(OLHandler&);
  virtual void AcceptBeingParentOf(OSLHandler&);
  virtual void VisitAsChildOfLosungHandler(OLosungHandler&);
  OTestimony *mpTestimony;
};

class OSLHandler : public OLineHandler {
public:
  virtual ~OSLHandler() {};
  virtual void StartTag(OXMLStack*);
  virtual void VisitAsChildOfParolHandler(OParolHandler&);
};

class OTLHandler : public OLineHandler {
public:
  virtual ~OTLHandler() {};
  virtual void StartTag(OXMLStack*);
  virtual void VisitAsChildOfLosungHandler(OLosungHandler&);
};

class OXHandler : public OCharDataHandler {
public:
  virtual ~OXHandler() {};
  virtual void EndTag(OXMLStack*);
  virtual void VisitAsChildOfLineHandler(OLineHandler&);
};

class OXLatHandler : public OCharDataHandler {
public:
  virtual ~OXLatHandler() {};
  virtual void EndTag(OXMLStack*);
  virtual void AcceptBeingParentOf(OBibleCopyrightHandler&);
};

// --- register creators ---

namespace NLosFileStack {
  void RegisterLosFileHandlerCreators(OLosFileStack &losFileStack);

  extern "C"
  int PassThroughUnknownEncodingHandler(void *encodingHandlerData, const XML_Char *name, XML_Encoding *info);
  
  extern "C"
  int UnknownEncodingHandler(void *encodingHandlerData, const XML_Char *name, XML_Encoding *info);
}

#endif
