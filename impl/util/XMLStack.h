/* ======================================================================
  @brief C++ stack manager for expat
  @author Helmut Steeb
  $Id: XMLStack.h 3011 2008-11-29 09:37:12Z helmut $

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
#ifndef __XMLSTACK_H__
#define __XMLSTACK_H__

#include <TCHAR.H>
#include "../xmlparse/xmlparse.h"
#pragma warning(push,3)
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include "../util/tstring.h"
#pragma warning(pop)


class OXMLStack;

class OBaseHandler {
public:
  virtual ~OBaseHandler() {};
  virtual void StartTag(OXMLStack*) {};
  virtual void EndTag(OXMLStack*) {};
  virtual void CharData(OXMLStack*) {};
  virtual void PreStartTag(OXMLStack*) {};
  const tstring& GetCharData() { return mCharData; }
protected:
  tstring mCharData;
};

struct ltstr {
  bool operator()(const TCHAR *s1, const TCHAR *s2) const {
    return _tcscmp(s1, s2) < 0;
  }
};



class OXMLStack {
public:
  typedef OBaseHandler *(*THandlerCreator)();
  // a skipping handler is created automatically in AddCreatorOfSkippingHandler()
  // and used to put a handler on the bottom of the stack
  OXMLStack(THandlerCreator skippingHandlerCreator);
  virtual ~OXMLStack();

  void SetXmlParser(XML_Parser pParser);

  int GetCurrentLineNumber();
  int GetCurrentColumnNumber();

  void AddHandlerCreator(const _TCHAR *pTag, THandlerCreator);
  void AddCreatorOfSkippingHandler(const _TCHAR *pTag);
  void SetUnknownTagHandlerCreator(THandlerCreator);

  // SetStrict: 
  // class default is "true".
  // If true, document type mismatches (unhandled start tag...)
  //   lead to MSGERROR and IncErrNo which lets load fail.
  // If false, document type mismatches lead to MSGDEBUG, no load fail
  //   (only severe errors will make load fail: IO error, not wellformed or the like...)
  void SetStrict(bool bStrict);
  bool IsStrict();

  void FirstUnknownElement(const tstring &s);
  tstring FirstUnknownElement() const;

  OBaseHandler *GetParent() const;

  const TCHAR *GetName() const { return mName; }
  const TCHAR **GetAtts() const { return mAtts; }

  const TCHAR *GetRequiredAtt(const TCHAR *pAtt);
  const TCHAR *GetAtt(const TCHAR *pAtt, bool &bFound);

  const long GetRequiredLongAtt(const TCHAR *pAtt);
  const long GetLongAtt(const TCHAR *pAtt, bool &bFound);

  // S and Len as from expat
  const TCHAR *GetS();
  const int GetLen();
  void AppendS(tstring &dest); // appends string from expat onto dest

  void IncErrNo();
  const long GetErrNo() const;

  // overloads of ReportErrorXX must call IncErrNo()
  virtual const void ReportErrorAttNotFound(const TCHAR *pAtt) = 0;
  virtual const void ReportErrorAttNotLong(const TCHAR *pAtt, const TCHAR *pValue) = 0;

  // routines called from expat
  void HandleStartTag(const XML_Char *pName, const XML_Char **pAtts);
  void HandleEndTag(const XML_Char *pName);
  void HandleCharData(const XML_Char *s, int len);

protected:
  // prevent from using copy constructor / assignment
  OXMLStack(const OXMLStack&);
  OXMLStack& operator=(const OXMLStack&);

private:
  const THandlerCreator GetHandlerCreatorForTag(const TCHAR *pTag) const;
  const THandlerCreator GetUnknownTagHandlerCreator() const { return mUnknownTagHandlerCreator; }
  THandlerCreator mUnknownTagHandlerCreator;

  const THandlerCreator GetSkippingHandlerCreator() const { return mSkippingHandlerCreator; }
  THandlerCreator mSkippingHandlerCreator;
  
  typedef std::vector<TCHAR*> TStringVector;
  typedef std::map<const TCHAR*, THandlerCreator, ltstr> TTagToHandlerCreatorMap;
  typedef std::vector<OBaseHandler*> TBaseHandlerVector;

  TStringVector mStringVector;
  TTagToHandlerCreatorMap mTagToHandlerCreatorMap;
  TBaseHandlerVector mBaseHandlerVector;

  bool mTolerateUnknownElement;
  tstring mFirstUnknownElement;

  // from expat:
  const TCHAR *mName;
  const TCHAR **mAtts;
  const TCHAR *mS;
  int mLen;

  long mErrNo;
  XML_Parser mpXmlParser;
  bool mbStrict;
};

namespace NXMLStack {
  void RegisterWithExpatParser(XML_Parser, OXMLStack &XMLStack);
  void RegisterUnknownEncodingHandler(XML_Parser parser, XML_UnknownEncodingHandler handler);
}


#endif
