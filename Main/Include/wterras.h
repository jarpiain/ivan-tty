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

#ifndef __WTERRAS_H__
#define __WTERRAS_H__

#include "wterra.h"

GWTERRAIN(ocean, gwterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual truth UsesLongArticle() const { return true; }
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 10; }
  virtual const char* SurviveMessage() const;
  virtual const char* MonsterSurviveMessage() const;
  virtual const char* DeathMessage() const;
  virtual const char* MonsterDeathVerb() const;
  virtual const char* ScoreEntry() const;
  virtual truth IsFatalToStay() const { return true; }
  virtual int GetWalkability() const;
  virtual int GetGlyph() const { return '~'; }
  virtual int GetAttr() const { return BLUE; }
};

GWTERRAIN(glacier, gwterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 90; }
  virtual int GetGlyph() const { return '.'; }
  virtual int GetAttr() const { return CYAN; }
};

GWTERRAIN(desert, gwterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 20; }
  virtual int GetGlyph() const { return '.'; }
  virtual int GetAttr() const { return YELLOW; }
};

GWTERRAIN(snow, gwterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 80; }
  virtual int GetGlyph() const { return '.'; }
  virtual int GetAttr() const { return WHITE; }
};

GWTERRAIN(jungle, gwterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 50; }
  virtual int GetGlyph() const { return '&'; }
  virtual int GetAttr() const { return GREEN; }
};

GWTERRAIN(leafyforest, gwterrain)
{
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 60; }
  virtual int GetGlyph() const { return '%'; }
  virtual int GetAttr() const { return GREEN; }
};

GWTERRAIN(evergreenforest, gwterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual truth UsesLongArticle() const { return true; }
  virtual int GetPriority() const { return 70; }
  virtual int GetGlyph() const { return '^'; }
  virtual int GetAttr() const { return GREEN; }
};

GWTERRAIN(steppe, gwterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 30; }
  virtual int GetGlyph() const { return '.'; }
  virtual int GetAttr() const { return GREEN; }
};

OWTERRAIN(attnam, owterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual int GetGlyph() const { return '\\'; }
  virtual int GetAttr() const { return WHITE; }
};

OWTERRAIN(elpuricave, owterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual int GetGlyph() const { return '>'; }
  virtual int GetAttr() const { return YELLOW; }
};

OWTERRAIN(newattnam, owterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual int GetGlyph() const { return '\\'; }
  virtual int GetAttr() const { return YELLOW; }
};

OWTERRAIN(underwatertunnel, owterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual truth UsesLongArticle() const { return true; }
  virtual int GetGlyph() const { return '>'; }
  virtual int GetAttr() const { return YELLOW; }
};

OWTERRAIN(underwatertunnelexit, owterrain)
{
 public:
  virtual const char* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual truth UsesLongArticle() const { return true; }
  virtual int GetAttachedArea() const { return 2; }
  virtual int GetGlyph() const { return '>'; }
  virtual int GetAttr() const { return YELLOW; }
};

#endif
