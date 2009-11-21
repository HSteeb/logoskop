/* ======================================================================
  @brief C++ stack manager for expat
  @author Helmut Steeb
  $Id: XMLStack.cpp 3011 2008-11-29 09:37:12Z helmut $

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
#include "XMLStack.h"


OXMLStack::OXMLStack(THandlerCreator skippingHandlerCreator) 
: mName(0), mAtts(0), mS(0), mLen(0), mErrNo(0),
  mUnknownTagHandlerCreator(0),
  mSkippingHandlerCreator(skippingHandlerCreator),
  mpXmlParser(0),
  mbStrict(true)
{
  mBaseHandlerVector.push_back(skippingHandlerCreator());
}

OXMLStack::~OXMLStack()
{
  TStringVector::iterator it;
  for (it = mStringVector.begin(); it != mStringVector.end(); ++it) {
    TCHAR *pTag = *it;
    if (pTag) free(pTag);
  }

  while (!mBaseHandlerVector.empty()) {
    OBaseHandler *pHandler = mBaseHandlerVector.back();
    mBaseHandlerVector.pop_back();
    delete pHandler;
  }
}

void OXMLStack::SetXmlParser(XML_Parser pParser)
{
  mpXmlParser = pParser;
}

void OXMLStack::SetStrict(bool bStrict)
{
  mbStrict = bStrict;
}

bool OXMLStack::IsStrict()
{
  return mbStrict;
}



// handler creation

void OXMLStack::AddHandlerCreator(const _TCHAR *pTag, THandlerCreator handlerCreator)
{
  assert(pTag);
  assert(handlerCreator);
  TCHAR *pCopy = _tcsdup(pTag);
  assert(pCopy);

  std::pair<TTagToHandlerCreatorMap::const_iterator, bool> res;
  res = mTagToHandlerCreatorMap.insert(TTagToHandlerCreatorMap::value_type(pCopy, handlerCreator));
  assert(res.second);
  mStringVector.push_back(pCopy); // for deleting string
}

void OXMLStack::AddCreatorOfSkippingHandler(const _TCHAR *pTag)
{
  AddHandlerCreator(pTag, mSkippingHandlerCreator);
}

void OXMLStack::SetUnknownTagHandlerCreator(THandlerCreator handlerCreator)
{
  mUnknownTagHandlerCreator = handlerCreator;
}

const OXMLStack::THandlerCreator OXMLStack::GetHandlerCreatorForTag(const TCHAR *pTag) const
{
  assert(pTag);
  TTagToHandlerCreatorMap::const_iterator it = mTagToHandlerCreatorMap.find(pTag);
  return (it == mTagToHandlerCreatorMap.end()) ? 0: (*it).second;
}

int OXMLStack::GetCurrentLineNumber()
{
 return mpXmlParser ? XML_GetCurrentLineNumber(mpXmlParser) : -1;
}

int OXMLStack::GetCurrentColumnNumber()
{
  return mpXmlParser ? XML_GetCurrentColumnNumber(mpXmlParser) : -1;
}

void OXMLStack::FirstUnknownElement(const tstring &s)
{
  mFirstUnknownElement = s;
}

tstring OXMLStack::FirstUnknownElement() const
{
  return mFirstUnknownElement;
}

OBaseHandler *OXMLStack::GetParent() const
{
  const long size = mBaseHandlerVector.size();
  return (size > 1) ? mBaseHandlerVector[size-2] : 0;
}


const TCHAR *OXMLStack::GetRequiredAtt(const TCHAR *pAtt)
{
  bool bFound;
  const TCHAR *pValue = GetAtt(pAtt, bFound);
  if (!bFound) {
    ReportErrorAttNotFound(pAtt);
  }
  return pValue;
}

const TCHAR *OXMLStack::GetAtt(const TCHAR *pAtt, bool &bFound)
{
  if (mAtts) {
    const TCHAR **pAtts = mAtts;
    while (*pAtts && _tcscmp(*pAtts, pAtt)) {
      ++pAtts;
    }
    if (*pAtts) {
      bFound = true;
      return *++pAtts;
    }
  }
  bFound = false;
  return 0;
}

const long OXMLStack::GetRequiredLongAtt(const TCHAR *pAtt)
{
  bool bFound;
  const TCHAR *pValue = GetAtt(pAtt, bFound);
  if (bFound) {
    std::tistringstream istr(pValue);
    long value;
    if (istr >> std::dec >> value) {
      return value;
    }
    ReportErrorAttNotLong(pAtt, pValue);
  }
  else {
    ReportErrorAttNotFound(pAtt);
  }
  return 0;
}

const long OXMLStack::GetLongAtt(const TCHAR *pAtt, bool &bFound)
{
  const TCHAR *pValue = GetAtt(pAtt, bFound);
  if (bFound) {
    std::tistringstream istr(pValue);
    long value;
    if (istr >> std::dec >> value) {
      return value;
    }
    ReportErrorAttNotLong(pAtt, pValue);
  }
  return 0;
}

void OXMLStack::AppendS(tstring &dest)
{
  dest.append(GetS(), GetLen());
}

const TCHAR *OXMLStack::GetS() 
{ 
  return mS; 
}

const int OXMLStack::GetLen() 
{ 
  return mLen; 
}

void OXMLStack::IncErrNo() 
{ 
  ++mErrNo; 
}

const long OXMLStack::GetErrNo() const 
{ 
  return mErrNo; 
}



// handle callbacks from expat

void OXMLStack::HandleStartTag(const XML_Char *pName, const XML_Char **pAtts)
{
  THandlerCreator handlerCreator = GetHandlerCreatorForTag(pName);
  if (!handlerCreator) {
    handlerCreator = GetUnknownTagHandlerCreator();
    if (!handlerCreator) {
      handlerCreator = GetSkippingHandlerCreator();
    }
  }
#if 0
  fprintf(stderr, _T("<%s>"), pName);
#endif

  OBaseHandler *pHandler = handlerCreator();
  assert(pHandler);
  mBaseHandlerVector.push_back(pHandler);

  mName = pName;
  mAtts = pAtts;
  pHandler->PreStartTag(this);
  pHandler->StartTag(this);
  mAtts = 0;
  mName = 0;
}

void OXMLStack::HandleEndTag(const XML_Char *pName)
{
  assert(!mBaseHandlerVector.empty());
  OBaseHandler *pHandler = mBaseHandlerVector.back();
  assert(pHandler);
#if 0
  fprintf(stderr, _T("<%s>"), pName);
#endif

  mName = pName;
  pHandler->EndTag(this);
  mName = 0;
  mBaseHandlerVector.pop_back();
  delete pHandler;
}

void OXMLStack::HandleCharData(const XML_Char *s, int len)
{
  assert(!mBaseHandlerVector.empty());
  OBaseHandler *pHandler = mBaseHandlerVector.back();
  assert(pHandler);

  mS = s;
  mLen = len;
  pHandler->CharData(this);
  mS = 0;
  mLen = 0;
}

OXMLStack *GetXMLStack(void *parseData)
{
  assert(parseData);
  OXMLStack *pXMLStack = static_cast<OXMLStack*>(parseData);
  assert(pXMLStack); // && magic data valid...
  return pXMLStack;
}

// expat callback functions

extern "C"
void expatHandleStart(void *parsedata, const XML_Char *name, const XML_Char **atts)
{
  GetXMLStack(parsedata)->HandleStartTag(name, atts);
}

extern "C"
void expatHandleEnd(void *parsedata, const XML_Char *name)
{
  GetXMLStack(parsedata)->HandleEndTag(name);
}

extern "C"
void expatHandleCharData(void *parsedata, const XML_Char *s, int len)
{
  GetXMLStack(parsedata)->HandleCharData(s, len);
}


// register expat callback functions with expat

void NXMLStack::RegisterWithExpatParser(XML_Parser parser, OXMLStack &XMLStack)
{
  XML_SetUserData(parser, &XMLStack);
  XML_SetElementHandler(parser, expatHandleStart, expatHandleEnd);
  XML_SetCharacterDataHandler(parser, expatHandleCharData);
  XMLStack.SetXmlParser(parser);
}

void NXMLStack::RegisterUnknownEncodingHandler(XML_Parser parser, XML_UnknownEncodingHandler handler)
{
  XML_SetUnknownEncodingHandler(parser, handler, 0);
}
