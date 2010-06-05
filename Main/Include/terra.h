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

#ifndef __TERRA_H__
#define __TERRA_H__

#include "typedef.h"

class character;
class festring;

class terrain
{
 public:
  virtual void StepOn(character*) { }
  virtual const char* SurviveMessage() const;
  virtual const char* MonsterSurviveMessage() const;
  virtual const char* DeathMessage() const;
  virtual const char* MonsterDeathVerb() const;
  virtual const char* ScoreEntry() const;
  virtual truth IsFatalToStay() const { return false; }
  virtual void SurviveEffect(character*) { };
};

class gterrain : public terrain
{
 public:
  virtual int GetEntryDifficulty() const = 0;
};

class oterrain : public terrain
{
 public:
  virtual truth Enter(truth) const = 0;
  virtual int GetRestModifier() const { return 1; }
  virtual void ShowRestMessage(character*) const { }
  virtual int GetWalkability() const = 0;
};

#endif
