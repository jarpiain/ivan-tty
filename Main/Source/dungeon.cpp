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

#include "dungeon.h"
#include "level.h"
#include "script.h"
#include "error.h"
#include "game.h"
#include "save.h"
#include "femath.h"

dungeon::dungeon() { }

dungeon::dungeon(int Index) : Index(Index)
{
  Initialize();

  for(int c = 0; c < GetLevels(); ++c)
    Generated[c] = false;
}

dungeon::~dungeon()
{
  for(int c = 0; c < GetLevels(); ++c)
    delete Level[c];

  delete [] Level;
  delete [] Generated;
}

void dungeon::Initialize()
{
  std::map<int, dungeonscript>::const_iterator DungeonIterator = game::GetGameScript()->GetDungeon().find(Index);

  if(DungeonIterator != game::GetGameScript()->GetDungeon().end())
    DungeonScript = &DungeonIterator->second;
  else
  {
    ABORT("Unknown dungeon #%d requested!", int(Index));
    return;
  }

  Level = new level*[GetLevels()];
  Generated = new truth[GetLevels()];

  for(int c = 0; c < GetLevels(); ++c)
    Level[c] = 0;
}

const levelscript* dungeon::GetLevelScript(int I)
{
  std::map<int, levelscript>::const_iterator LevelIterator = DungeonScript->GetLevel().find(I);

  const levelscript* LevelScript;

  if(LevelIterator != DungeonScript->GetLevel().end())
    LevelScript = &LevelIterator->second;
  else
    LevelScript = DungeonScript->GetLevelDefault();

  return LevelScript;
}

/* Returns whether the level has been visited before */

truth dungeon::PrepareLevel(int Index, truth Visual)
{
  if(Generated[Index])
  {
    level* NewLevel = LoadLevel(game::SaveName(), Index);
    game::SetCurrentArea(NewLevel);
    game::SetCurrentLevel(NewLevel);
    game::SetCurrentLSquareMap(NewLevel->GetMap());
    return true;
  }
  else
  {
    level* NewLevel = Level[Index] = new level;
    NewLevel->SetDungeon(this);
    NewLevel->SetIndex(Index);
    NewLevel->SetLevelScript(GetLevelScript(Index));

    if(Visual)
      game::TextScreen(CONST_S("Entering ") + GetLevelDescription(Index) + CONST_S("...\n\nThis may take some time, please wait."), WHITE, false, &game::BusyAnimation);

    NewLevel->Generate(Index);
    game::SetCurrentLSquareMap(NewLevel->GetMap());
    Generated[Index] = true;
    game::BusyAnimation();

    if(*NewLevel->GetLevelScript()->GenerateMonsters())
      NewLevel->GenerateNewMonsters(NewLevel->GetIdealPopulation(), false);

    return false;
  }
}

void dungeon::SaveLevel(const festring& SaveName, int Number, truth DeleteAfterwards)
{
  outputfile SaveFile(SaveName + '.' + Index + Number);
  SaveFile << Level[Number];

  if(DeleteAfterwards)
  {
    delete Level[Number];
    Level[Number] = 0;
  }
}

level* dungeon::LoadLevel(const festring& SaveName, int Number)
{
  inputfile SaveFile(SaveName + '.' + Index + Number);
  return LoadLevel(SaveFile, Number);
}

void dungeon::Save(outputfile& SaveFile) const
{
  SaveFile << Index << WorldMapPos;

  for(int c = 0; c < GetLevels(); ++c)
    SaveFile << Generated[c];
}

void dungeon::Load(inputfile& SaveFile)
{
  SaveFile >> Index >> WorldMapPos;
  Initialize();

  for(int c = 0; c < GetLevels(); ++c)
    SaveFile >> Generated[c];
}

int dungeon::GetLevels() const
{
  return *DungeonScript->GetLevels();
}

festring dungeon::GetLevelDescription(int I)
{
  if(GetLevel(I)->GetLevelScript()->GetDescription())
    return *GetLevel(I)->GetLevelScript()->GetDescription();
  else
    return *DungeonScript->GetDescription() + " level " + (I + 1);
}

festring dungeon::GetShortLevelDescription(int I)
{
  if(GetLevel(I)->GetLevelScript()->GetShortDescription())
    return *GetLevel(I)->GetLevelScript()->GetShortDescription();
  else
    return *DungeonScript->GetShortDescription() + " level " + (I + 1);
}

outputfile& operator<<(outputfile& SaveFile, const dungeon* Dungeon)
{
  if(Dungeon)
  {
    SaveFile.Put(1);
    Dungeon->Save(SaveFile);
  }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, dungeon*& Dungeon)
{
  if(SaveFile.Get())
  {
    Dungeon = new dungeon;
    Dungeon->Load(SaveFile);
  }

  return SaveFile;
}

level* dungeon::LoadLevel(inputfile& SaveFile, int Number)
{
  SaveFile >> Level[Number];
  Level[Number]->SetDungeon(this);
  Level[Number]->SetIndex(Number);
  Level[Number]->SetLevelScript(GetLevelScript(Number));
  return Level[Number];
}

int dungeon::GetLevelTeleportDestination(int From) const
{
  int To;

  if(Index == ELPURI_CAVE)
  {
    if(RAND_2)
    {
      To = From + RAND_2 + RAND_2 + RAND_2 + RAND_2 + 1;

      if(To > DARK_LEVEL)
	To = From;
    }
    else
    {
      To = From - RAND_2 - RAND_2 - RAND_2 - RAND_2 - 1;

      if(To < 0)
	To = 0;
    }

    return To;
  }

  if(Index == UNDER_WATER_TUNNEL)
    return RAND_N(3);

  return From;
}
