/* ======================================================================
 @file LosFileStack.cpp
 @brief C++ stack for losfile xml files using expat and OXMLStack
 @author Helmut Steeb
 @version $Id: LosFileStack.cpp 3014 2008-11-29 10:19:16Z helmut $

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
#include "../util/XMLStack.h"
#include "../util/DateUtil.h"
#include "../util/UTF8.h"
#include "LosFileStack.h"
#include "LosFileConst.h"



OBaseHandler *CreateLosFileBaseHandler() 
{ 
  return new OLosFileBaseHandler;
}



// --- OLosFileBaseHandler ---

void OLosFileBaseHandler::PreStartTag(OXMLStack *pXMLStack)
{
#ifdef _DEBUG
  mpLosFileStack = dynamic_cast<OLosFileStack*>(pXMLStack);
  assert(mpLosFileStack);
#else
  mpLosFileStack = static_cast<OLosFileStack*>(pXMLStack);
#endif
}

OLosFileBaseHandler *OLosFileBaseHandler::GetParent()
{
  return mpLosFileStack->GetParent();
}

OLosFileStack &OLosFileBaseHandler::GetLosFileStack()
{
  assert(mpLosFileStack);
  return *mpLosFileStack;
}

void OLosFileBaseHandler::AcceptBeingParentOf(OLosFileBaseHandler &)
{
  // get here if you don't define AcceptBeingParentOf for specific handler
  if (mpLosFileStack && mpLosFileStack->GetName()) {
    MSGERROR("OLosFileBaseHandler::AcceptBeingParentOf: missing virtual AcceptBeingParentOf for specific handler");
  }
  else {
    MSGERROR("OLosFileBaseHandler::AcceptBeingParentOf:  missing virtual AcceptBeingParentOf for some handler");
  }
}

void OLosFileBaseHandler::UnhandledChild(const TCHAR *pTag)
{
  if (mpLosFileStack->IsStrict()) {
    mpLosFileStack->IncErrNo();
    MSGERROR("UnhandledChild: AcceptBeingParentOf must be overwritten at parent of " << T28(tstring(pTag)));
  }
  else {
    MSGDEBUG("UnhandledChild: skipping unhandled child element " << T28(tstring(pTag)));
  }
}

OBaseHandler *CreateBaseHandler() { return new OBaseHandler; }


// --- OLosFileStack ---


OLosFileStack::OLosFileStack(OLosFile &losFile)
: inherited(/* skippingHandlerCreator:*/ CreateLosFileBaseHandler), 
  mpLosung(0),
  mMonthToRead(0), mDayToRead(0),
  mLosFile(losFile)
{
}

OLosFileStack::~OLosFileStack()
{
  // be nice - release memory
  // (would be released later anyway...)
  NUTF8::Release();
}

void OLosFileStack::SetDayToRead(unsigned int monthToRead, unsigned int dayToRead)
{
  mMonthToRead = monthToRead;
  mDayToRead = dayToRead;
}
  
bool OLosFileStack::IsDayToRead(unsigned int monthToRead, unsigned int dayToRead)
{
  return !mMonthToRead || !mDayToRead 
    // if reading just one day, leave all other days unchanged
    || (mMonthToRead == monthToRead && mDayToRead == dayToRead);
}

OLosFileBaseHandler *OLosFileStack::GetParent()
{
  OBaseHandler *pParent = inherited::GetParent();
  OLosFileBaseHandler *pLosFileBaseHandler =
#ifdef _DEBUG
    dynamic_cast<OLosFileBaseHandler*>(pParent);
#else
    static_cast<OLosFileBaseHandler*>(pParent);
#endif
  assert(pLosFileBaseHandler);
  return pLosFileBaseHandler;
}

OLosFile &OLosFileStack::GetLosFile() 
{
  return mLosFile; 
}

const void OLosFileStack::ReportErrorAttNotFound(const TCHAR *pAtt)
{
  if (IsStrict()) {
    MSGERROR("OLosFileStack: attribute " << pAtt << " not found at element.");
    IncErrNo();
  }
  else {
    MSGDEBUG("OLosFileStack: attribute " << pAtt << " not found at element.");
  }
}

const void OLosFileStack::ReportErrorAttNotLong(const TCHAR *pAtt, const TCHAR *pValue)
{
  if (IsStrict()) {
    MSGERROR("OLosFileStack: attribute value " << pAtt << "=" << pValue << " should be number, ignoring.");
    IncErrNo();
  }
  else {
    MSGDEBUG("OLosFileStack: attribute value " << pAtt << "=" << pValue << " should be number, ignoring.");
  }
}

wchar_t *OLosFileStack::ToUTF16(const tstring &s)
{
#ifdef UNICODE
  return const_cast<wchar_t*>(s.c_str());
#else
  wchar_t *p = NUTF8::ToUTF16(s);
  return p ? p : L"";
#endif
}


// --- Element handlers ---

// base classes:

void OCharDataHandler::CharData(OXMLStack*)
{
  mCharData.append(mpLosFileStack->GetS(), mpLosFileStack->GetLen());
}

tstring &OCharDataHandler::CharData()
{
  return mCharData;
}

OLineHandler::OLineHandler()
: mpLine(0)
{
}
void OLineHandler::EndTag(OXMLStack*)
{
  if (mpLine) {
    mpLine->Content(mpLosFileStack->ToUTF16(mCharData));
  }
}
void OLineHandler::AcceptBeingParentOf(OEMHandler &handler)
{
  handler.VisitAsChildOfLineHandler(*this);
}
void OLineHandler::AcceptBeingParentOf(OXHandler &handler)
{
  handler.VisitAsChildOfLineHandler(*this);
}


void OUnknownTagHandler::StartTag(OXMLStack* pXMLStack)
{
  if (mpLosFileStack->FirstUnknownElement().empty()) {
    mpLosFileStack->FirstUnknownElement(pXMLStack->GetName());
  }
  if (mpLosFileStack->IsStrict()) {
    MSGERROR("OUnknownTagHandler::StartTag: unhandled start tag " << pXMLStack->GetName());
    mpLosFileStack->IncErrNo();
  }
  else {
    MSGDEBUG("OUnknownTagHandler::StartTag: skipping unhandled start tag " << pXMLStack->GetName());
  }
}
OBaseHandler *UnknownTagHandlerCreator() { return new OUnknownTagHandler; }



// alphabetically:

// --- OBibleNameHandler ---

void OBibleNameHandler::EndTag(OXMLStack*)
{
  mpLosFileStack->GetLosFile().BibleName(mpLosFileStack->ToUTF16(mCharData));
}
OBaseHandler *BibleNameHandlerCreator() { return new OBibleNameHandler; }


// --- OBibleCopyrightHandler ---

void OBibleCopyrightHandler::EndTag(OXMLStack*)
{
  // store separately
  mpLosFileStack->GetLosFile().BibleCopyright(mpLosFileStack->ToUTF16(mCharData));
  // add to parent string <copyright>
  GetParent()->AcceptBeingParentOf(*this);
}
void OBibleCopyrightHandler::VisitAsChildOfXLatHandler(OXLatHandler &handler)
{
  tstring &charData = handler.CharData();
  charData.append(CharData());
}
OBaseHandler *BibleCopyrightHandlerCreator() { return new OBibleCopyrightHandler; }


// --- OCHandler ---

void OCHandler::StartTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
}
void OCHandler::VisitAsChildOfLosungHandler(OLosungHandler &handler)
{
  if (handler.mpLosung) {
    mpLine = &handler.mpLosung->CommentLines();
  }
}
OBaseHandler *CHandlerCreator() { return new OCHandler; }


// --- OEMHandler ---

void OEMHandler::EndTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
}
void OEMHandler::VisitAsChildOfLineHandler(OLineHandler &handler)
{
  tstring &charData = handler.CharData();
  charData.append(_T(GUI_EM));
  charData.append(mCharData);
  charData.append(_T(GUI_EM));
}
OBaseHandler *EMHandlerCreator() { return new OEMHandler; }


// --- OFileVersHandler ---

void OFileVersHandler::EndTag(OXMLStack*)
{
  mpLosFileStack->GetLosFile().FileVers(mpLosFileStack->ToUTF16(mCharData));
}
OBaseHandler *FileVersHandlerCreator() { return new OFileVersHandler; }


// --- OHeadHandler ---

void OHeadHandler::StartTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
}
void OHeadHandler::VisitAsChildOfLosFileHandler(OLosFileHandler &handler)
{
  // HS 2003-06-19: no action
}
OBaseHandler *HeadHandlerCreator() { return new OHeadHandler; }


// --- OILHandler ---

void OILHandler::StartTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
}
void OILHandler::VisitAsChildOfParolHandler(OParolHandler &handler)
{
  if (handler.mpTestimony) {
    mpLine = &handler.mpTestimony->IntroLine();
  }
}
OBaseHandler *ILHandlerCreator() { return new OILHandler; }


// --- OLHandler ---

void OLHandler::StartTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
}
void OLHandler::VisitAsChildOfParolHandler(OParolHandler &handler)
{
  if (handler.mpTestimony) {
    mpLine = &handler.mpTestimony->Lines();
  }
}
OBaseHandler *LHandlerCreator() { return new OLHandler; }


// --- OLosFileHandler ---

void OLosFileHandler::StartTag(OXMLStack*) 
{
  OLosFileStack &losFileStack = GetLosFileStack();
  OLosFile &losFile = losFileStack.GetLosFile();

  const TCHAR *pDtdVers = losFileStack.GetRequiredAtt(_T(A_DTDVERS));
  if (pDtdVers) {
    long mainVers;
    long subVers;
    if (scanf("%d.%d", pDtdVers, &mainVers, &subVers)) {
      losFile.DtdMainVers(mainVers);
      losFile.DtdMainVers(mainVers);
    }
  }

  const long year = losFileStack.GetRequiredLongAtt(_T(A_YEAR));
  losFile.Year(year);

  const TCHAR *xmlLang = losFileStack.GetRequiredAtt(_T(A_XMLLANG));
  losFile.XmlLang(xmlLang);
}
void OLosFileHandler::AcceptBeingParentOf(OHeadHandler &handler)
{
  handler.VisitAsChildOfLosFileHandler(*this);
}
OBaseHandler *LosFileHandlerCreator() { return new OLosFileHandler; }


// --- OLosungHandler ---

OLosungHandler::OLosungHandler() 
: mMonth(0), mDay(0), mpLosung(0)
{
}  
void OLosungHandler::StartTag(OXMLStack*)
{
  const TCHAR *pDate = mpLosFileStack->GetRequiredAtt(_T(A_DATE));
  unsigned int year;
  unsigned int month;
  unsigned int day;
  if (3 == sscanf(pDate, "%4u-%2u-%2u", &year, &month, &day)) {
    mMonth = month;
    mDay = day;
    if (mpLosFileStack->IsDayToRead(mMonth, mDay)) {
      // set mpLosung for all handlers within LOSUNG (may be 0!)
      int dayOfYear = DateUtil::DayOfYear(mpLosFileStack->GetLosFile().Year(), mMonth, mDay);
      // if day has already been read, skip it!
      // (HS 2003-11-11 - fix for w32los)
      // 2007-12-11 HS: dayOfYear is -1 when reading a 2008 file with 02-29 
      // that has just been copied to be used in 2007.
      if (dayOfYear >= 0 && dayOfYear <= 365) {
        OLosung *pLosung = mpLosFileStack->mLosFile.LosungVector().Get(dayOfYear+1);
        if (!pLosung) {
          // papa mama b d s g tabea
          mpLosung = &mpLosFileStack->mLosFile.LosungVector().GetOrMake(dayOfYear+1);
        }
      }
    }
  }
}
void OLosungHandler::AcceptBeingParentOf(OCHandler &handler)
{
  handler.VisitAsChildOfLosungHandler(*this);
}
void OLosungHandler::AcceptBeingParentOf(OParolHandler &handler)
{
  handler.VisitAsChildOfLosungHandler(*this);
}
void OLosungHandler::AcceptBeingParentOf(OTLHandler &handler)
{
  handler.VisitAsChildOfLosungHandler(*this);
}
void OLosungHandler::EndTag(OXMLStack*)
{
  // reset mpLosung when leaving LOSUNG
  mpLosung = 0;
}
OBaseHandler *LosungHandlerCreator() { return new OLosungHandler; }


// --- OParolHandler ---

OParolHandler::OParolHandler() 
: mpTestimony(0) 
{
}
void OParolHandler::StartTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
  if (mpTestimony) {
    bool bFound;
    const TCHAR *s = mpLosFileStack->GetAtt(_T(A_ID), bFound);
    if (bFound) {
      mpTestimony->ParolID(s);
    }
  }
}
void OParolHandler::AcceptBeingParentOf(OILHandler &handler)
{
  handler.VisitAsChildOfParolHandler(*this);
}
void OParolHandler::AcceptBeingParentOf(OLHandler &handler)
{
  handler.VisitAsChildOfParolHandler(*this);
}
void OParolHandler::AcceptBeingParentOf(OSLHandler &handler)
{
  handler.VisitAsChildOfParolHandler(*this);
}
void OParolHandler::VisitAsChildOfLosungHandler(OLosungHandler &handler)
{
  if (handler.mpLosung) {
    mpTestimony = &handler.mpLosung->AddParol();
  }
}
OBaseHandler *ParolHandlerCreator() { return new OParolHandler; }



// --- OSLHandler ---

void OSLHandler::StartTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
}
void OSLHandler::VisitAsChildOfParolHandler(OParolHandler &handler)
{
  if (handler.mpTestimony) {
    mpLine = &handler.mpTestimony->SourceLine();
  }
}
OBaseHandler *SLHandlerCreator() { return new OSLHandler; }


// --- OTLHandler ---

void OTLHandler::StartTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
}
void OTLHandler::VisitAsChildOfLosungHandler(OLosungHandler &handler)
{
  if (handler.mpLosung) {
    mpLine = &handler.mpLosung->TitleLine();
  }
}
OBaseHandler *TLHandlerCreator() { return new OTLHandler; }


// --- OXHandler ---

void OXHandler::EndTag(OXMLStack*)
{
  GetParent()->AcceptBeingParentOf(*this);
}
void OXHandler::VisitAsChildOfLineHandler(OLineHandler &handler)
{
  tstring &charData = handler.CharData();
  // represent X literally
  charData.append(_T(GUI_X_OPEN));
  charData.append(mCharData);
  charData.append(_T(GUI_X_CLOSE));
}
OBaseHandler *XHandlerCreator() { return new OXHandler; }


// --- OXLatHandler ---

void OXLatHandler::EndTag(OXMLStack*)
{
  mpLosFileStack->mLosFile.Xlat(mpLosFileStack->ToUTF16(mCharData));
}
void OXLatHandler::AcceptBeingParentOf(OBibleCopyrightHandler &handler)
{
  handler.VisitAsChildOfXLatHandler(*this);
}
OBaseHandler *XLatHandlerCreator() { return new OXLatHandler; }


// --- register creators ---

void NLosFileStack::RegisterLosFileHandlerCreators(OLosFileStack &losFileStack)
{
  losFileStack.SetUnknownTagHandlerCreator(UnknownTagHandlerCreator);

  // HS 2004-12-07:
  // By using AddCreatorOfSkippingHandler for some tag,
  //   this tag is silently skipped in any case, it is NEVER reported as an error.
  // To emit an error for an unknown tag (if SetStrict(true)), 
  //   do not AddCreatorOfSkippingHandler for the tag.

  losFileStack.AddHandlerCreator(_T(E_BIBLENAME), BibleNameHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_BIBLECOPYRIGHT), BibleCopyrightHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_C),         CHandlerCreator);
  losFileStack.AddCreatorOfSkippingHandler(_T(E_DAY)); // HS 2004-12-06: be defensive for Petr Herman's DAY
  losFileStack.AddCreatorOfSkippingHandler(_T(E_DOW));
  losFileStack.AddHandlerCreator(_T(E_EM),        EMHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_FILEVERS),  FileVersHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_HEAD),      HeadHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_IL),        ILHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_L),         LHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_LOSFILE),   LosFileHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_LOSUNG),    LosungHandlerCreator);
  losFileStack.AddCreatorOfSkippingHandler(_T(E_MONTH)); // HS 2004-12-06: be defensive for Petr Herman's MONTH
  losFileStack.AddCreatorOfSkippingHandler(_T(E_MONTHS));
  losFileStack.AddHandlerCreator(_T(E_PAROL),     ParolHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_SL),        SLHandlerCreator);
  losFileStack.AddCreatorOfSkippingHandler(_T(E_SUBLANG));
  losFileStack.AddHandlerCreator(_T(E_TL),        TLHandlerCreator);
  losFileStack.AddCreatorOfSkippingHandler(_T(E_TT)); // HS 2003-09-23: be defensive for Petr Herman's TT
  losFileStack.AddCreatorOfSkippingHandler(_T(E_TLPATTERN));
  losFileStack.AddHandlerCreator(_T(E_X),         XHandlerCreator);
  losFileStack.AddHandlerCreator(_T(E_XLAT),      XLatHandlerCreator);
}

extern "C"
int NLosFileStack::PassThroughUnknownEncodingHandler(void *encodingHandlerData, const XML_Char *name, XML_Encoding *info)
{
  // If the application requests "no conversion", 
  // pass any encoding through 1:1
  int i;
  for (i = 0; i < 256; ++i) {
    info->map[i] = i;
  }
  return true;
}



extern "C"
int NLosFileStack::UnknownEncodingHandler(void *encodingHandlerData, const XML_Char *name, XML_Encoding *info)
{
  // 2008-11-14 HS:
  // support UTF-8 only for The Word, dropped includes for enc*.h!
  return false;
}

