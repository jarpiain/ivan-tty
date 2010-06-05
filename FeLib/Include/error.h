/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

#ifndef __ERROR_H__
#define __ERROR_H__

#include "felibdef.h"

#define ABORT globalerrorhandler::Abort

class globalerrorhandler
{
 public:
  static void Install();
  static void DeInstall();
  static void NO_RETURN LIKE_PRINTF(1, 2) Abort(const char*, ...);
  static const char* GetBugMsg() { return BugMsg; }
 private:
  static const char* BugMsg;
  static void NewHandler();
  static void (*OldNewHandler)();
};

#endif
