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

#ifndef __TRAP_H__
#define __TRAP_H__

#include "entity.h"

class trap;
class lsquare;
class character;
class bitmap;
class outputfile;
class inputfile;
struct blitdata;

typedef trap* (*trapspawner)();

struct trapdata
{
  trapdata(ulong TrapID, ulong VictimID, ulong BodyParts) : Next(0), TrapID(TrapID), VictimID(VictimID), BodyParts(BodyParts) { }
  trapdata() : Next(0) { }
  trapdata* Next;
  ulong TrapID;
  ulong VictimID;
  ulong BodyParts;
};

outputfile& operator<<(outputfile&, const trapdata*);
inputfile& operator>>(inputfile&, trapdata*&);
outputfile& operator<<(outputfile&, const trapdata&);
inputfile& operator>>(inputfile&, trapdata&);

class trapprototype
{
 public:
  trapprototype(trapspawner truth, const char*);
  trap* SpawnAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  trapspawner Spawner;
  const char* ClassID;
};

class trap : public entity
{
 public:
  /* Come To The Dark Side */
  trap* Next;
 public:
  typedef trapprototype prototype;
  trap();
  virtual ~trap();
  virtual square* GetSquareUnderEntity(int = 0) const;
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  virtual truth IsOnGround() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual void AddDescription(festring&) const = 0;
  virtual const prototype* GetProtoType() const = 0;
  virtual void StepOnEffect(character*) = 0;
  virtual void Draw(blitdata&) const = 0;
  virtual void ReceiveDamage(character*, int, int, int) { }
  virtual truth IsDangerous(const character*) const { return false; }
  virtual void PreProcessForBone() { }
  virtual void PostProcessForBone() { }
 protected:
  lsquare* LSquareUnder;
};

#ifdef __FILE_OF_STATIC_TRAP_PROTOTYPE_DEFINITIONS__
#define TRAP_PROTO(name)\
template<> const trapprototype\
  name##sysbase::ProtoType((trapspawner)(&name##sysbase::Spawn), #name);
#else
#define TRAP_PROTO(name)
#endif

#define TRAP(name, base)\
class name;\
typedef simplesysbase<name, base, trapprototype> name##sysbase;\
TRAP_PROTO(name)\
class name : public name##sysbase

#endif
