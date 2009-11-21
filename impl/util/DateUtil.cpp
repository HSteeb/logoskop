/* ----------------------------------------------------------------------
  Helmut Steeb
  Date utility routines.
  $Id: DateUtil.cpp 3011 2008-11-29 09:37:12Z helmut $

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

#include "dateutil.h"
#include "assert.h"


/* --- utility date functions --------------------------------- */

bool DateUtil::IsLeapYear(int year)
/* returns 1 iff year is leap year, else 0 */
{
  return (year % 4 == 0 && year % 100 != 0)
         || year % 400 == 0;
}

int DateUtil::MaxDayOfMonth(int year, int month)
{
  static int daytab[13] = 
    { 0, 31,  28,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31 };
      /* Jan  Feb  Mar  Apr  May  Jun  Jul  Aug  Sep  Oct  Nov  Dec */
  return (month < 1 || month > 12) ?
    0 : (daytab[month] + ( (month == 2 && IsLeapYear(year)) ? 1 : 0));
}

int DateUtil::MaxDayOfYear(int year)
{
  return IsLeapYear(year)? 366 : 365;
}

bool DateUtil::IsValidDay(int year, int month, int day)
{
  return day > 0 && day <= MaxDayOfMonth(year, month);
    /* returns 0 if maxDayOfMonth returns 0 */
}

static int monthStart[13] = 
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
/* Jan Feb Mar Apr  May  Jun  Jul  Aug  Sep  Oct  Nov  Dec */

int DateUtil::DayOfYear(int year, int month, int day)
/* returns NO_DAYOFYEAR for error, 0..365 else */
{
  if (!IsValidDay(year, month, day)) {
    return NO_DAYOFYEAR;
  }
  return monthStart[month - 1] + day - 1 + 
            ( (month > 2 && IsLeapYear(year)) ? 1 : 0);
         /* add 1 for leap year if month > 2 */
}


int DateUtil::DateOf(int dayofyear, int y, int *pM, int *pD)
{
  int m;
  if (dayofyear < 0 || dayofyear > 365) return 1;
  for (m = 1; m < 12 && dayofyear >= monthStart[m]; ) {
    ++m;
    if (m == 3 && IsLeapYear(y)) {
      if (dayofyear == 59) {
        m = 2; 
        break; // special treatment for 29 Feb.: break to get it into Feb.
      }
      // else shift rest one day back to get correct difference from monthStart[]
      --dayofyear;
    }
  }
  if (pM) *pM = m;
  if (pD) *pD = dayofyear - monthStart[m-1] + 1;
  return 0;  
}

#if 0
int testDateOf(int year) 
{
  int limit = IsLeapYear(year)? 366 : 365;
  for (int i = 0; i < limit; ++i) {
    int d, m;
    if (0 == dateOf(i, year, &m, &d)) {
      fprintf(stdout, "%3d %2d.%2d\n", i, d, m);
    }
    else {
      fprintf(stdout, "Error for %d\n", i);
    }
  }
#endif

int DateUtil::DayOfWeek(int year, int month, int day)
{
  int year1, year2, res;
  if (month < 3) {
    month += 10;
    --year;
  }
  else {
    month -= 2;
  }
  year1 = year / 100;
  year2 = year % 100;
  // the orginal formula may return negative numbers
  // since "2*year1 is subtracted.
  // Avoid that by adding a big multiple of 7 constant before.
  res = (700 + year2/4 + year1/4 + (month*13-1)/5 + year2 + day - 2*year1) % 7;
  assert (0 <= res && res <= 6);
  return res;
}

int DateUtil::CorrectYear(int y)
{
  if (y < 50) {
    return y + 2000;
  }
  if (y < 100) {
    return y + 1900;
  }
  return y;
}

// --- date manipulation

int DateUtil::MoveDay(int *pYear, int *pMonth, int *pDay, const int diff)
{
  int doy = DayOfYear(*pYear, *pMonth, *pDay) + diff;
  int m = MaxDayOfYear(*pYear);
  while (doy >= m) {
    doy -= m;
    m = MaxDayOfYear(++*pYear);
  }
  while (doy < 0) {
    doy += MaxDayOfYear(--*pYear);
  }
  return DateOf(doy, *pYear, pMonth, pDay);  
}

int DateUtil::MoveMonth(int *pYear, int *pMonth, int *pDay, const int diff)
{
  //_ASSERTE(diff >= -1 && diff < 1); // hack for now
  *pMonth += diff;
  while (*pMonth < 1) {
    *pMonth += 12;
    --*pYear;
  }
  while (*pMonth > 12) {
    *pMonth -= 12;
    ++*pYear;
  }
  {
    int md = MaxDayOfMonth(*pYear, *pMonth);
    if (*pDay > md) {
      *pDay = md;
    }
  }
  return 0;
}

