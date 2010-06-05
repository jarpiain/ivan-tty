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

#ifndef __GODS_H__
#define __GODS_H__

#include "god.h"

GOD(valpurus, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual void Pray();
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(legifer, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(atavus, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual truth LikesMaterial(const materialdatabase*, const character*) const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(dulcis, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(seges, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual truth ForceGiveBodyPart() const { return true; }
  virtual truth HealRegeneratingBodyParts() const { return true; }
  virtual truth LikesMaterial(const materialdatabase*, const character*) const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(sophos, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(silva, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(loricatus, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(mellis, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(cleptia, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(nefas, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(scabies, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual truth PlayerVomitedOnAltar(liquid*);
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual truth LikesMaterial(const materialdatabase*, const character*) const;
  virtual truth MutatesBodyParts() const { return true; }
  virtual int GetSex() const { return FEMALE; }
  virtual truth LikesVomit() const { return true; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(infuscor, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(cruentus, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

GOD(mortifer, god)
{
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual void Pray();
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
};

#endif
