/* ----------------------------------------------------------------------
  Helmut Steeb
  Logoskop document.
  $Id: w32losDoc.h 3924 2009-09-26 17:37:12Z helmut $

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

#if !defined(_W32LOSDOC_H_)
#define _W32LOSDOC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IFileManager;
class IProfile;
class IDirectoryFileDescReader;
#ifdef _TEST
class OMockLangBibleYearReader;
#endif
class OLosung;
class OLosDoc;

namespace NW32losDoc {
  typedef enum { 
    // enum values must be multiple of 2 for bitwise OR
    docDateChanged = 1, 
    docYearChanged = 2, 
    docLangBiblesChanged = 4 
  } tDocChange;
};

class CW32losDoc : public CDocument
{
public:
//protected: // Nur aus Serialisierung erzeugen
  CW32losDoc();
	DECLARE_DYNCREATE(CW32losDoc)
  virtual ~CW32losDoc();

  // === Files ===

  void SetFileManager(IFileManager &fileManager);

  // === Profile ===

  void SetProfile(IProfile &profile);

  // === Date ===

  CTime GetDate() const;


  // === LangBibles ===

  const std::vector<string> &GetLangBibles() const;
  tstring GetFirstLangBible() const;

  // SetLangBibles:
  // tries to provide OLosDoc for each element of langBibles,
  // GetLangBibles() and GetLosDocForLangBible() 
  // will subsequently return data for all available elements of langBibles.
  // Updates all views.
  void SetLangBibles(const std::vector<string> &langBibles);

  // SetLangBiblesDate:
  // sets date as date for displaying document data, 
  // + same effect as SetLangBibles(), based on new date.
  void SetLangBiblesDate(const std::vector<string> &langBibles, const CTime &date);

  // SetDate:
  // sets date as date for displaying document data, 
  // + tries to keep list of lang bibles.
  void SetDate(const CTime &date);

  OLosDoc *GetLosDocForLangBible(const string &langBible) const;

  OLosung *GetFirstLosung() const;

  static bool EqDate(const CTime &t1, const CTime &t2);

  // === Lock ===

  // SetLocked():
  // bLocked becomes future return value of IsLocked().
  // No other effect - just enables clients (modal dialogs, timer) to get out of their way.
  void SetLocked(bool bLocked = true);
  bool IsLocked() const;

#ifdef _TEST
  bool _OSLangIDTest();
  void _InitTest(const CTime &initialDate, OMockLangBibleYearReader *pReader);
  // _EvalTest:
  // check expectedLangBibles against mLangBibles if expectedLangBibles is non-empty
  bool _EvalTest(string testID, 
    const CTime &expectedDate,
    unsigned int expectedNoLangBibles, string expectedLangBibles, 
    long expectedHint);
  bool _LangBiblesTest();
  bool SelfTest();
  void UpdateAllViews( CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL );
#endif

protected:


  // === Files ===

  IFileManager &_FileManager();

  // _GetFilePath() constructs a complete path for langBible + mDate 
  string _GetFilePath(const string &langBible);

  bool _Load(const string &langBible, OLosDoc &losDoc);

  void _ProposeLangBibles(std::vector<string> &proposedLangBibles);

  // _SetDate:
  // returns true if mDate has changed
  bool _SetDate( const CTime &date);
  
  // uses mDate
  OLosDoc *_GetOrLoadLosDocForLangBibleDate(const string &langBible);

  // _UpdateLangBibles
  // leaves only elements in mLangBibles in which OLosung exists for mDate;
  // returns true iff. mLangBibles has been changed.
  bool _UpdateLangBibles();

  // _AddLangBibleIfExists
  // adds langBible into mLangBibles if OLosung exists for mDate
  // returns true iff. if langBible added into mLangBible
  bool _AddLangBibleIfExists(const string &langBible);

  // _SetLangBibles
  // sets only elements in pLangBibles (default: mLangBibles)
  // in which OLosung exists for mDate into mLangBibles
  // returns true iff. mLangBibles has been changed.
  bool _SetLangBibles(const std::vector<string> *pLangBibles);

  // === Profile ===

  IProfile &_Profile();

private:
#ifdef _TEST
  bool mbTestRunning;
  LPARAM mTestHint;
#endif

  // === Files ===

  IFileManager *mpFileManager;
  IDirectoryFileDescReader *mpDirectoryFileDescReader;


  // === Profile ===

  IProfile *mpProfile;

  // === LangBibles ===

  std::vector<string> mLangBibles;
  std::vector<string> mLastNonEmptyLangBibles;
  CTime mDate;
  std::vector<OLosDoc*> mLosDocCache; // owned OLosDoc objects
  LANGID mTestUserDefaultLangID; // use for _TEST only, ignored if value is IGNORE_LANGID

  // === Lock ===

  bool mbLocked;
};

#endif
