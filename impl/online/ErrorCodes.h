/* ----------------------------------------------------------------------
   Helmut Steeb 
  Error codes for download handling 
  $Id: ErrorCodes.h 2993 2008-11-22 18:19:36Z helmut $
---------------------------------------------------------------------- */

#if !defined(_ERRORCODES_INCLUDED_)
#define _ERRORCODES_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DOWNLOAD_SUCCESS 0
#define DOWNLOAD_PROTOCOL_NOT_SUPPORTED 1
#define DOWNLOAD_URL_ERROR 2
#define DOWNLOADMGR_CANCEL -1
#define DOWNLOADMGR_ERROR  -2
#define DOWNLOADMGR_RETRY_LIMIT 1

#endif
