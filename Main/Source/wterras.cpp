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

/* Compiled through wmapset.cpp */

const char* ocean::GetNameStem() const { return "ocean"; }
v2 ocean::GetBitmapPos(int Frame) const
{ return v2(48 + ((Frame << 3)&~8), 48); }
const char* ocean::SurviveMessage() const
{ return "you manage to reach the shore"; }
const char* ocean::MonsterSurviveMessage() const
{ return "manages to reach the shore"; }
const char* ocean::DeathMessage() const { return "you drown"; }
const char* ocean::MonsterDeathVerb() const { return "drowns"; }
const char* ocean::ScoreEntry() const { return "drowned"; }

const char* glacier::GetNameStem() const { return "glacier"; }
v2 glacier::GetBitmapPos(int) const { return v2(16, 16); }

const char* desert::GetNameStem() const { return "desert"; }
v2 desert::GetBitmapPos(int) const { return v2(64, 16); }

const char* snow::GetNameStem() const { return "tundra"; }
v2 snow::GetBitmapPos(int) const { return v2(112, 16); }

const char* jungle::GetNameStem() const { return "jungle"; }
v2 jungle::GetBitmapPos(int) const { return v2(208, 16); }

const char* leafyforest::GetNameStem() const { return "leafy forest"; }
v2 leafyforest::GetBitmapPos(int) const { return v2(304, 16); }

const char* evergreenforest::GetNameStem() const
{ return "evergreen forest"; }
v2 evergreenforest::GetBitmapPos(int) const { return v2(352, 16); }

const char* steppe::GetNameStem() const { return "steppe"; }
v2 steppe::GetBitmapPos(int) const { return v2(160, 16); }

const char* attnam::GetNameStem() const
{ return "migthy cathedral reaching the clouds"; }
v2 attnam::GetBitmapPos(int) const { return v2(0, 48); }
int attnam::GetAttachedDungeon() const { return ATTNAM; }

const char* elpuricave::GetNameStem() const
{ return "hideous cave entry radiating pure evil"; }
v2 elpuricave::GetBitmapPos(int) const { return v2(16, 48); }
int elpuricave::GetAttachedDungeon() const { return ELPURI_CAVE; }

const char* newattnam::GetNameStem() const { return "primitive village"; }
v2 newattnam::GetBitmapPos(int) const { return v2(16, 64); }
int newattnam::GetAttachedDungeon() const { return NEW_ATTNAM; }

const char* underwatertunnel::GetNameStem() const
{ return "entrance to an underwater tunnel"; }
v2 underwatertunnel::GetBitmapPos(int) const { return v2(32, 64); }
int underwatertunnel::GetAttachedDungeon() const
{ return UNDER_WATER_TUNNEL; }

const char* underwatertunnelexit::GetNameStem() const
{ return "exit from an underwater tunnel"; }
v2 underwatertunnelexit::GetBitmapPos(int) const { return v2(32, 64); }
int underwatertunnelexit::GetAttachedDungeon() const
{ return UNDER_WATER_TUNNEL; }

int ocean::GetWalkability() const { return ANY_MOVE&~WALK; }
