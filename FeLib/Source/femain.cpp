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

#include <iostream>

#include <cstdlib>

#include "error.h"

int Main(int, char**);

int main(int argc, char* argv[])
{
  try
  {
    return Main(argc, argv);
  }
  catch(...)
  {
    const char* Msg = "Fatal Error: Unknown exception thrown.";
#ifdef LINUX
    std::cerr << Msg << globalerrorhandler::GetBugMsg() << std::endl;
#endif
    exit(3);
  }

  exit(0);
}
