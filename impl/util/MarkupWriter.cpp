/* ======================================================================
  @file MarkupWriter.h
  @brief Markup writer (XML etc.)
  @author Helmut Steeb
 $Id: MarkupWriter.cpp 3011 2008-11-29 09:37:12Z helmut $

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
#include "MarkupWriter.h"


OMarkupWriter::OMarkupWriter(std::ostream &os)
: mOs(os)
{
}


OMarkupWriter::~OMarkupWriter()
{
}


// routines writing basic xml output data ---

void OMarkupWriter::MDO()
{
  mOs << "<!";
}

void OMarkupWriter::MDC(const bool withEndl)
{
  mOs << ">";
  if (withEndl) mOs << std::endl;
}

void OMarkupWriter::PIO()
{
  mOs << "<?";
}

void OMarkupWriter::PIC(const bool withEndl)
{
  mOs << "?>";
  if (withEndl) mOs << std::endl;
}


void OMarkupWriter::STagO(const char *pTag)
{
  mOs << "<" << pTag;
}
void OMarkupWriter::STagO(const string &tag)
{
  mOs << "<" << tag;
}
 
void OMarkupWriter::Attr(const char *pAttr, const char *pValue)
{
  mOs << " " << pAttr << "='" << pValue << "'";
}
void OMarkupWriter::Attr(const string &attr, const string &value)
{
  mOs << " " << attr << "='" << value << "'";
}

void OMarkupWriter::LongAttr(const char *pAttr, const long value)
{
  mOs << " " << pAttr << "='" << value << "'";
}
void OMarkupWriter::LongAttr(const string &attr, const long value)
{
  mOs << " " << attr << "='" << value << "'";
}

void OMarkupWriter::TagC(const bool withEndl)
{
  mOs << ">";
  if (withEndl) mOs << std::endl;
}
 
void OMarkupWriter::Content(const char *pContent)
{
  if (!pContent) return;
  for (const char *p = pContent; *p; ++p) {
    switch (*p) {
    case '<':
      mOs << "&lt;";
      break;
    case '>':
      mOs << "&gt;";
      break;
    case '&':
      mOs << "&amp;";
      break;
    default:
      mOs.put(*p);
    }
  }
}
void OMarkupWriter::Content(const string &content)
{
  Content(content.c_str());
}
void OMarkupWriter::Content(const char *pContent, const char *pContentEnd)
{
  if (!pContent || pContentEnd <= pContent) return;
  mOs.write(pContent, pContentEnd - pContent);
}


void OMarkupWriter::ETag(const char *pTag, const bool withEndl)
{
  mOs << "</" << pTag << ">";
  if (withEndl) mOs << std::endl;
}
void OMarkupWriter::ETag(const string &tag, const bool withEndl)
{
  mOs << "</" << tag << ">";
  if (withEndl) mOs << std::endl;
}
 
void OMarkupWriter::EmptyTagC(const bool withEndl)
{
  mOs << "/>";
  if (withEndl) mOs << std::endl;
}


// --- routines writing combination of several basics ---

void OMarkupWriter::MDOName(const char *pName)
{
  MDO();
  mOs << pName;
}
void OMarkupWriter::MDOName(const string &name)
{
  MDO();
  mOs << name;
}

void OMarkupWriter::PIOTarget(const char *pTarget)
{
  PIO();
  mOs << pTarget;
}
void OMarkupWriter::PIOTarget(const string &target)
{
  PIO();
  mOs << target;
}


void OMarkupWriter::STag(const char *pTag, const bool withEndl)
{
  STagO(pTag);
  TagC(withEndl);
}
void OMarkupWriter::STag(const string &tag, const bool withEndl)
{
  STagO(tag);
  TagC(withEndl);
}

void OMarkupWriter::Tagged(const char *pTag, const char *pContent, const bool withEndl)
{
  STag(pTag, false); // no endl
  Content(pContent);
  ETag(pTag, withEndl);
}
void OMarkupWriter::Tagged(const string &tag, const string &content, const bool withEndl)
{
  STag(tag, false); // no endl
  Content(content);
  ETag(tag, withEndl);
}
void OMarkupWriter::Tagged(const char *pTag, const char *pContent, const char *pContentEnd, const bool withEndl)
{
  STag(pTag, false); // no endl
  Content(pContent, pContentEnd);
  ETag(pTag, withEndl);
}



// --- ONestedElement

ONestedElement::ONestedElement(OMarkupWriter &markupWriter, const char *pTag, const TAttrMode attrMode, const bool withEndl)
: mMarkupWriter(markupWriter), mTag(pTag)
{
  if (attrMode == withAttrs) {
    mMarkupWriter.STagO(mTag);
  }
  else {
    mMarkupWriter.STag(mTag, withEndl);
  }
}

ONestedElement::ONestedElement(OMarkupWriter &markupWriter, const string &tag, const TAttrMode attrMode, const bool withEndl)
: mMarkupWriter(markupWriter), mTag(tag)
{
  if (attrMode == withAttrs) {
    mMarkupWriter.STagO(mTag);
  }
  else {
    mMarkupWriter.STag(mTag, withEndl);
  }
}

ONestedElement::~ONestedElement()
{
  mMarkupWriter.ETag(mTag);
}
