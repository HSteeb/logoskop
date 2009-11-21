/* ----------------------------------------------------------------------
  Helmut Steeb 
  Helper for data exchange OTwdFileDescList <-> Listbox

  $Id: TwdFileDescListBoxHelper.cpp 3014 2008-11-29 10:19:16Z helmut $

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
---------------------------------------------------------------------- */

#include "stdafx.h"
#include <assert.h>
#include "TwdFileDesc.h"
#include "TwdFileDescList.h"
#include "TwdFileDescListBoxHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


OTwdFileDescListBoxHelper::OTwdFileDescListBoxHelper(CListBox& listbox)
{
  mpListBox = &listbox;
}

OTwdFileDescListBoxHelper::OTwdFileDescListBoxHelper(CDialog &dlg, int listboxID)
{
  mpListBox = (CListBox *) dlg.GetDlgItem(listboxID);
  // mpListBox may be 0
}

OTwdFileDescListBoxHelper::~OTwdFileDescListBoxHelper()
{
}


bool OTwdFileDescListBoxHelper::FillListbox(OTwdFileDescList &fileList)
{
  if (!mpListBox) {
    return false;
  }
  mpListBox->ResetContent();
  TFileDescList::const_iterator it;
  for (it = fileList.Begin(); it != fileList.End(); ++it) {
    string displayName = (*it)->GetDisplayName();
    int index = mpListBox->AddString(displayName.c_str());
    mpListBox->SetItemDataPtr(index, *it);
  }
  if (0 != fileList.Size()) {
    mpListBox->SetSel(0);
  }
  return true;
}

void OTwdFileDescListBoxHelper::GetSelection(OTwdFileDescList &sel)
{
  if (!mpListBox) return;

  sel.Clear();

  int count = mpListBox->GetSelCount();
  int *pArr = new int[count];
  mpListBox->GetSelItems(count, pArr);
  for (int i = 0; i < count; ++i) {
    ITwdFileDesc *pTwdFileDesc = (ITwdFileDesc*) mpListBox->GetItemDataPtr(pArr[i]);
    assert(pTwdFileDesc);
    sel.Add(pTwdFileDesc->Clone());
  }
  delete[] pArr;
}

void OTwdFileDescListBoxHelper::SetSelection(OTwdFileDescList &sel, bool doMatchYear)
{
  if (!mpListBox) return;

  // Note:SelectString() does not work for multiselect listbox :-(

  // reset selection
  mpListBox->SelItemRange(FALSE, 0, mpListBox->GetCount()-1);

  for (TFileDescList::const_iterator it = sel.Begin(); it != sel.End(); ++it) {
    const string& displayName = (*it)->GetDisplayName();
    int prevPos = -1;
    int pos;
    do {
      pos = mpListBox->FindString(prevPos, displayName.c_str());
      //  NOTE CListBox::FindString:
      //   When the search reaches the bottom of the list box, 
      //   it continues from the top of the list box
      if (pos > prevPos && pos != LB_ERR) {
        prevPos = pos;
        if (doMatchYear) {
          ITwdFileDesc* pTwdFileDesc = (ITwdFileDesc*) mpListBox->GetItemDataPtr(pos);
          if (!pTwdFileDesc || pTwdFileDesc->GetYear() != (*it)->GetYear()) continue;
        }
        mpListBox->SetSel(pos);
      }
      else {
        pos = LB_ERR; // terminate (in case we wrapped around with pos <= prevPos)
      }
    } while (pos != LB_ERR);
  }
}
