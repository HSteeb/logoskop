/* ======================================================================
 @file OLosungVector.h
 @brief C++ class for representing set of daily losung 
 @author Helmut Steeb
 @version $Id: LosungVector.h 3014 2008-11-29 10:19:16Z helmut $

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
#ifndef __OLOSUNGVECTOR_H__
#define __OLOSUNGVECTOR_H__

#include "Losung.h"


// === OLosungVector ===

class OLosungVector {
private:
  typedef std::vector<OLosung*> _OLosungVector;
  typedef _OLosungVector::iterator OIter;
public:
  OLosungVector();
  ~OLosungVector();

  OIter Begin();
  OIter Next(const OIter &it);
  OIter End();

  // Get:
  // accepts dayOfYear 1..366 (asserted)
  // If OLosung exists for dayOfYear, returns it,
  // else returns 0.
  OLosung *Get(unsigned int dayOfYear);

  // GetOrMake:
  // accepts dayOfYear 1..366 (asserted)
  // If OLosung exists for dayOfYear, returns it,
  // else creates one and returns that.
  OLosung &GetOrMake(unsigned int dayOfYear);

  // Delete:
  // accepts dayOfYear 1..366 (asserted)
  // If OLosung exists for dayOfYear, deletes it.
  // else no action.
  void Delete(unsigned int dayOfYear);

private:
  _OLosungVector mLosungVector;
};


#endif
