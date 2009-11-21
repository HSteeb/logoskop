/* ----------------------------------------------------------------------
  Helmut Steeb
  $Id: glib.h 3011 2008-11-29 09:37:12Z helmut $
  Hack to get fribidi compiled - don't have glib.h under Win32

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
#ifndef _GLIB_H_
#define _GLIB_H_

#define gboolean  int
#define FALSE     0
#define TRUE      1

#define gint      int
#define guchar    unsigned char
#define gchar     signed char
#define guint16   unsigned short
#define gint16    signed short
#define guint8    unsigned char
#define gint8     signed char
#define guint32   unsigned long

/* HS 000825: is that ok...? */
#include <stdlib.h> /* malloc, free */
#define g_new(TYPE, size) (TYPE*) malloc(sizeof(TYPE)*size)
#define g_free(p) free(p)

//typedef 
typedef struct _GList {
  void *data;
  struct _GList *prev;
  struct _GList *next;
} GList;

GList *g_list_append(GList *elem, void *data);
void g_list_free_1(GList *elem);
#define g_list_next(p) ((p)->next)

#endif
