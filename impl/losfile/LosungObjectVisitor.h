/* ======================================================================
 @brief Abstract base visitor for losung data.
 @author Helmut Steeb
 @version $Id: LosungObjectVisitor.h 3849 2009-08-26 12:45:48Z helmut $

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
#ifndef _LOSUNG_OBJECT_VISITOR_H_
#define _LOSUNG_OBJECT_VISITOR_H_

class OLosungObject;
class OLosFile;
class OLosung;
class OTestimony;
class OLineVector;
class OLine;


class OLosungObjectVisitor
{
public:
  virtual void Visit(OLosFile &losObject) = 0;
  virtual void Visit(OLosung &losObject) = 0;
  virtual void Visit(OTestimony &losObject) = 0;
  virtual void Visit(OLineVector &losObject) = 0;
  virtual void Visit(OLine &losObject) = 0;
  virtual void Visit(OLosungObject &losObject) = 0;
};

#endif
