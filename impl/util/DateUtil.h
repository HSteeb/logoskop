/* ----------------------------------------------------------------------
  Helmut Steeb 
  Date utility routines.
  $Id: DateUtil.h 3011 2008-11-29 09:37:12Z helmut $

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
#ifndef DATEUTIL_H
#define DATEUTIL_H

namespace DateUtil {
  
  /* --- utility date functions --------------------------------- */
  #define NO_DAYOFYEAR -1
  
  
  /* returns true iff year (e.g. 1996) is leap year, else 0 */
  bool IsLeapYear(const int year);
  
  /* returns true iff day/month/year is a valid date */
  bool IsValidDay(const int year, const int month, const int day);
  
  /* returns NO_DAYOFYEAR for error, 0..365 else */
  int DayOfYear(const int year, const int month, const int day);
  
  /* returns > 0 for error, 0 else with *pD set to 1..31 (if pD) and *pM set to 1..12 (if pM) */
  int DateOf(const int dayofyear, const int y, int *pM, int *pD);
  
  /* returns 0 if arguments invalid, 28..31 else */
  int MaxDayOfMonth(const int year, const int month);
  
  /* returns 365 or 366 */
  int MaxDayOfYear(int year);

  /* returns 0 for Sunday,... 6 for Saturday. year must be > 1900 */
  int DayOfWeek(const int year, const int month, const int day);
  
  /* corrects 2-digit year, i.e. returns: 0..49 => 2000..2049; 50..99 => 1950..1999; 100.. => 100.. */
  int CorrectYear(const int y);

  int MoveDay(int *pYear, int *pMonth, int *pDay, const int diff);
  int MoveMonth(int *pYear, int *pMonth, int *pDay, const int diff);

}

#endif

