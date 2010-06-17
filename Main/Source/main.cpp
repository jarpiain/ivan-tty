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

#ifdef LINUX
#include <signal.h>
#endif

#include "game.h"
#include "database.h"
#include "feio.h"
#include "igraph.h"
#include "iconf.h"
#include "whandler.h"
#include "hscore.h"
#include "graphics.h"
#include "script.h"
#include "message.h"
#include "proto.h"

#ifdef LINUX
void catch_sighup(int Signum)
{
  game::Save();
  ABORT("SIGHUP");
}
#endif

int Main(int argc, char **argv)
{
  if(argc > 1 && festring(argv[1]) == "--version")
  {
    std::cout << "Iter Vehemens ad Necem version " << IVAN_VERSION << std::endl;
    return 0;
  }

  if(argc < 2)
  {
    std::cout << "Usage: ivan <username>" << std::endl;
    return 0;
  }

  festring UserName(argv[1]);

  femath::SetSeed(time(0));
  game::InitGlobalValueMap();
  scriptsystem::Initialize();
  databasesystem::Initialize();
  game::InitLuxTable();
  ivanconfig::Initialize(UserName);
  igraph::Init();
  msgsystem::Init();
  protosystem::Initialize();

#ifdef LINUX
  signal(SIGHUP, catch_sighup);
#endif

  if(game::Init(UserName))
  {
    game::Run();
    game::DeInit();
  }

  highscore HScore;
  HScore.Draw();

  return 0;
}
