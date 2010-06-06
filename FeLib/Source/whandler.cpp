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

#include "whandler.h"
#include "graphics.h"
#include "error.h"
#include "bitmap.h"
#include "festring.h"

truth (*globalwindowhandler::ControlLoop[MAX_CONTROLS])();
int globalwindowhandler::Controls = 0;
ulong globalwindowhandler::Tick;
truth globalwindowhandler::ControlLoopsEnabled = true;

void globalwindowhandler::InstallControlLoop(truth (*What)())
{
  if(Controls == MAX_CONTROLS)
    ABORT("Animation control frenzy!");

  ControlLoop[Controls++] = What;
}

void globalwindowhandler::DeInstallControlLoop(truth (*What)())
{
  int c;

  for(c = 0; c < Controls; ++c)
    if(ControlLoop[c] == What)
      break;

  if(c != Controls)
  {
    --Controls;

    for(; c < Controls; ++c)
      ControlLoop[c] = ControlLoop[c + 1];
  }
}

int globalwindowhandler::GetKey(truth EmptyBuffer)
{
  return -1;
}

int globalwindowhandler::ReadKey()
{
  return -1;
}
