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

#include <curses.h>

#include "whandler.h"

int globalwindowhandler::GetKey(truth EmptyBuffer)
{
  return getch();
}

int globalwindowhandler::ReadKey()
{
  return getch();
}
