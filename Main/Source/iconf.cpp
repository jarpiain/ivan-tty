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

#include "iconf.h"
#include "game.h"
#include "feio.h"
#include "area.h"
#include "graphics.h"
#include "igraph.h"

stringoption ivanconfig::DefaultPetName(  "DefaultPetName",
					  "starting pet's default name",
					  CONST_S("Kenny"));
truthoption ivanconfig::WarnAboutDanger(	  "WarnAboutVeryDangerousMonsters",
						  "Warn about very dangerous monsters",
						  true);
truthoption ivanconfig::AutoDropLeftOvers( "AutoDropLeftOvers",
					   "drop food leftovers automatically",
					   true);
truthoption ivanconfig::UseAlternativeKeys("UseAlternativeKeys",
					   "use alternative direction keys",
					   false);
col24 ivanconfig::ContrastLuminance;
void ivanconfig::CalculateContrastLuminance()
{
  ContrastLuminance = MakeRGB24(128, 128, 128);
}

void ivanconfig::Initialize(const festring& UserName)
{
  configsystem::AddOption(&DefaultPetName);
  configsystem::AddOption(&WarnAboutDanger);
  configsystem::AddOption(&AutoDropLeftOvers);
  configsystem::AddOption(&UseAlternativeKeys);
  configsystem::SetConfigFileName(CONST_S(LOCAL_STATE_DIR "/config/") + UserName + ".ivanrc");
  configsystem::Load();
  CalculateContrastLuminance();
}
