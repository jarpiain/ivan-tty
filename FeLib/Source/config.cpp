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

#include "config.h"
#include "save.h"
#include "felist.h"
#include "feio.h"

configoption* configsystem::Option[MAX_CONFIG_OPTIONS];
festring configsystem::ConfigFileName;
int configsystem::Options;

void configsystem::AddOption(configoption* O) { Option[Options++] = O; }

configoption::configoption(const char* Name, const char* Description)
: Name(Name), Description(Description) { }

stringoption::stringoption(const char* Name, const char* Desc,
                           const festring& Value)
: configoption(Name, Desc),
  Value(Value) { }

numberoption::numberoption(const char* Name, const char* Desc, long Value)
: configoption(Name, Desc),
  Value(Value) { }

scrollbaroption::scrollbaroption(const char* Name,
				 const char* Desc, long Value)
: numberoption(Name, Desc, Value) { }

truthoption::truthoption(const char* Name, const char* Desc, truth Value)
: configoption(Name, Desc),
  Value(Value) { }

truth configsystem::Load()
{
  inputfile SaveFile(ConfigFileName, 0, false);

  if(!SaveFile.IsOpen())
    return false;

  festring Word;

  for(SaveFile.ReadWord(Word, false);
      !SaveFile.Eof();
      SaveFile.ReadWord(Word, false))
  {
    /* Inefficient, but speed is probably not an issue here */

    for(int c = 0; c < Options; ++c)
      if(Word == Option[c]->Name)
        Option[c]->LoadValue(SaveFile);
  }

  return true;
}

void stringoption::LoadValue(inputfile& SaveFile)
{
  SaveFile.ReadWord();
  SaveFile.ReadWord(Value);
}

void numberoption::LoadValue(inputfile& SaveFile)
{ Value = SaveFile.ReadNumber(); }
void truthoption::LoadValue(inputfile& SaveFile)
{ Value = SaveFile.ReadNumber(); }
