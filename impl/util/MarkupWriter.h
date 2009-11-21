/* ======================================================================
  @file MarkupWriter.h
  @brief Markup writer (XML etc.)
  @author Helmut Steeb
  $Id: MarkupWriter.h 3011 2008-11-29 09:37:12Z helmut $

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
#ifndef __MARKUPWRITER_H__
#define __MARKUPWRITER_H__

#include <TCHAR.H>

using std::string;


class OMarkupWriter {
public:
  OMarkupWriter(std::ostream &os);
  ~OMarkupWriter();

// routines writing basic xml output data ---

/** 
 MDO: writes markup declaration open, e.g. "<!"
*/
  void MDO();

/** 
 MDC: writes markup declaration close, e.g. ">"
*/
  void MDC(const bool withEndl = true);


/** 
 PIO: writes processing instruction open, e.g. "<?"
*/
  void PIO();

/** 
 PIC: writes processing instruction close, e.g. "?>"
*/
  void PIC(const bool withEndl = true);


/** 
 STagO: writes start tag open, e.g. "<pTag"
 @param pTag tag to use 
*/
  void STagO(const char *pTag);
  void STagO(const string &tag);

/** 
 Attr: writes attr with value, preceeded by blank, e.g. " pAttr='pValue'".
 At present (000711), validity for xml is not checked.
 @param pAttr name of attribute
 @param pValue value of attribute
*/
  void Attr(const char *pAttr, const char *pValue);
  void Attr(const string &attr, const string &value);

/** 
 LongAttr: writes attr with value, preceeded by blank, e.g. " pAttr='value'".
 At present (000711), validity for xml is not checked.
 @param pAttr name of attribute
 @param pValue value of attribute
*/
  void LongAttr(const char *pAttr, const long value);
  void LongAttr(const string &attr, const long value);

/** 
 TagC: writes closing ">".
 @param withEndl if given, a terminating linefeed is written
*/
  void TagC(const bool withEndl = true);

/** 
 Content: writes pContent.
 Should some time check for xml validity and mask xml special characters.
 @param pContent content to write (may be 0)
 @param pContentEnd pointer past last character to write
*/
  void Content(const char *pContent);
  void Content(const string &content);
  void Content(const char *pContent, const char *pContentEnd);

/** 
 ETag: writes closing tag, e.g. "</pTag>".
 @param pTag tag to use
*/
  void ETag(const char *pTag, const bool withEndl = true);
  void ETag(const string &tag, const bool withEndl = true);

/** 
 EmptyTagC: writes empty closing tag, e.g. "/>". This may be used after
 STagO and Attr to close an empty tag.
 @param withEndl if given, a terminating linefeed is written
*/
  void EmptyTagC(const bool withEndl = true);



// --- routines writing combination of several basics ---
  
/** 
 MDOName: writes markup declaration open with name e.g. "<?DOCTYPE"
 @param pName name to write
*/
  void MDOName(const char *pName);
  void MDOName(const string &name);

/** 
 PIOTarget: writes processing instruction open with target e.g. "<?xml"
 @param pTarget target to write (may be "xml")
*/
  void PIOTarget(const char *pTarget);
  void PIOTarget(const string &target);

/** 
 STag: writes start tag without attributes, e.g. "<pTag>"
 @param pTag tag to use 
 @param withEndl if given, a terminating linefeed is written
*/
  void STag(const char *pTag, const bool withEndl = true);
  void STag(const string &tag, const bool withEndl = true);

/** 
 Tagged: writes pContent, surrounded by start and end tag, e.g. <pTag>pContent</pTag>
 @param pTag tag to use 
 @param pContent content to write (may be 0)
 @param pContentEnd pointer past last character to write
 @param withEndl if given, a terminating linefeed is written
*/
  void Tagged(const char *pTag, const char *pContent, const bool withEndl = true);
  void Tagged(const string &tag, const string &content, const bool withEndl = true);
  void Tagged(const char *pTag, const char *pContent, const char *pContentEnd, const bool withEndl = true);


private:
  // exclude use of assignment/copy
  OMarkupWriter::operator=(const OMarkupWriter &);
  OMarkupWriter::OMarkupWriter(const OMarkupWriter &);

  std::ostream &mOs;
};


class ONestedElement {
public:
  typedef enum { noAttrs, withAttrs } TAttrMode;

/** 
 ONestedElement: immediately writes start tag open with pTag,
 on destruction writes ETag with pTag.
 @param attrMode for attrMode == noAttrs, immediately closes start tag, else leaves it open.
 @param withEndl for attrMode == noAttrs, defines whether end line is added
*/
  ONestedElement(OMarkupWriter &markupWriter, const char *pTag, const TAttrMode attrMode = noAttrs, const bool withEndl = true);
  ONestedElement(OMarkupWriter &markupWriter, const string &tag, const TAttrMode attrMode = noAttrs, const bool withEndl = true);

  ~ONestedElement();
private:
  // exclude use of assignment/copy
  ONestedElement::operator=(const ONestedElement &);
  ONestedElement::ONestedElement(const ONestedElement &);

  OMarkupWriter &mMarkupWriter;
  std::string mTag;
};


#endif
