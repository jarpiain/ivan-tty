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

/* Compiled through trapset.cpp */

trapprototype::trapprototype(trapspawner Spawner, const char* ClassID) : Spawner(Spawner), ClassID(ClassID) { Index = protocontainer<trap>::Add(this); }

trap::trap() : entity(HAS_BE), Next(0) { }
square* trap::GetSquareUnderEntity(int) const { return LSquareUnder; }

trap::~trap()
{
}

void trap::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
}

void trap::Load(inputfile&)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
}

outputfile& operator<<(outputfile& SaveFile, const trapdata* Data)
{
  SaveFile << *Data;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, trapdata*& Data)
{
  Data = new trapdata;
  SaveFile >> *Data;
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, const trapdata& Data)
{
  SaveFile << Data.TrapID << Data.VictimID << Data.BodyParts;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, trapdata& Data)
{
  SaveFile >> Data.TrapID >> Data.VictimID >> Data.BodyParts;
  return SaveFile;
}

trap* trapprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  trap* Trap = Spawner();
  Trap->Load(SaveFile);
  return Trap;
}
