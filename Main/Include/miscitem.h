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

#ifndef __MISCITEM_H__
#define __MISCITEM_H__

#include <set>

#include "item.h"
#include "game.h" /// check
#include "trap.h"

ITEM(materialcontainer, item)
{
 public:
  materialcontainer() { }
  materialcontainer(const materialcontainer&);
  virtual ~materialcontainer();
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material*, int = 0);
  virtual void ChangeSecondaryMaterial(material*, int = 0);
  void InitMaterials(material*, material*, truth = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetMaterials() const { return 2; }
  virtual void SignalSpoil(material*);
  virtual truth CanBePiledWith(const item*, const character*) const;
  virtual void Be();
  virtual int GetSpoilLevel() const;
  virtual material* GetMaterial(int) const;
  virtual int GetAttachedGod() const;
  virtual material* GetConsumeMaterial(const character*, materialpredicate = TrueMaterialPredicate) const;
  virtual material* RemoveMaterial(material*);
  material* RemoveMainMaterial();
  virtual material* RemoveSecondaryMaterial();
  virtual void CalculateEmitation();
  virtual void InitMaterials(const materialscript*, const materialscript*, truth);
  virtual int GetSparkleFlags() const;
 protected:
  virtual long GetMaterialPrice() const;
  virtual truth CalculateHasBe() const;
  virtual void GenerateMaterials();
  virtual col16 GetMaterialColorB(int) const;
  virtual alpha GetAlphaB(int) const;
  virtual int GetRustDataB() const;
  material* SecondaryMaterial;
};

ITEM(banana, materialcontainer)
{
 public:
  banana() : TimesUsed(0), Charges(6) { }
  virtual truth Zap(character*, v2, int);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { TimesUsed = 0; }
  virtual truth IsZappable(const character*) const { return true; }
  virtual truth IsChargeable(const character*) const { return true; }
  virtual void SignalSpoil(material*);
  virtual truth IsBanana() const { return true; }
  virtual material* RemoveSecondaryMaterial();
 protected:
  int TimesUsed;
  int Charges;
};

ITEM(holybanana, banana)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
  virtual truth Zap(character*, v2, int);
  virtual void Be() { }
  virtual int GetSpecialFlags() const;
  virtual void AddInventoryEntry(const character*, festring&, int, truth) const;
  virtual truth ReceiveDamage(character*, int, int, int);
};

ITEM(lantern, item)
{
 public:
  virtual void SignalSquarePositionChange(int);
  virtual truth AllowAlphaEverywhere() const { return true; }
  virtual int GetSpecialFlags() const;
  virtual truth IsLanternOnWall() const { return GetSquarePosition() != CENTER; }
 protected:
  virtual int GetClassAnimationFrames() const { return !IsBroken() ? 32 : 1; }
  virtual col16 GetMaterialColorA(int) const;
  virtual col16 GetMaterialColorB(int) const;
  virtual col16 GetMaterialColorC(int) const;
  virtual col16 GetMaterialColorD(int) const;
  virtual alpha GetAlphaA(int) const { return 255; }
  virtual alpha GetAlphaB(int) const;
  virtual alpha GetAlphaC(int) const;
  virtual alpha GetAlphaD(int) const;
  virtual v2 GetBitmapPos(int) const;
};

ITEM(can, materialcontainer)
{
 public:
  virtual item* BetterVersion() const;
  virtual void DipInto(liquid*, character*);
  virtual truth IsDippable(const character*) const { return !SecondaryMaterial; }
  virtual truth IsDipDestination(const character*) const;
  virtual liquid* CreateDipLiquid();
  virtual truth AllowSpoil() const { return false; } // temporary
  virtual truth WillSpoil() const { return false; } // temporary
  virtual truth HasBetterVersion() const { return !SecondaryMaterial; }
 protected:
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
  virtual truth AddAdjective(festring&, truth) const;
  virtual v2 GetBitmapPos(int) const;
};

ITEM(lump, item)
{
 protected:
  virtual void AddPostFix(festring& String) const { AddLumpyPostFix(String); }
  virtual truth ShowMaterial() const { return false; }
  virtual truth WeightIsIrrelevant() const { return true; }
};

ITEM(potion, materialcontainer)
{
 public:
  virtual item* BetterVersion() const;
  virtual void DipInto(liquid*, character*);
  virtual liquid* CreateDipLiquid();
  virtual truth IsDippable(const character*) const { return !SecondaryMaterial; }
  virtual void Break(character*, int);
  virtual truth IsDipDestination(const character*) const;
  virtual truth IsExplosive() const;
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth HasBetterVersion() const { return !SecondaryMaterial; }
  virtual truth EffectIsGood() const;
  virtual truth IsKamikazeWeapon(const character*) const { return IsExplosive(); }
 protected:
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
  virtual truth AddAdjective(festring&, truth) const;
};

ITEM(bananapeels, item)
{
 public:
  virtual item* BetterVersion() const;
  virtual truth HasBetterVersion() const { return true; }
  virtual void StepOnEffect(character*);
  virtual truth IsBananaPeel() const { return true; }
  virtual truth IsDangerous(const character*) const;
  virtual truth RaiseTheDead(character*);
};

ITEM(brokenbottle, item)
{
 public:
  virtual truth IsBroken() const { return true; }
  virtual item* BetterVersion() const;
  virtual truth HasBetterVersion() const { return true; }
  virtual void StepOnEffect(character*);
  virtual item* Fix();
  virtual truth IsDangerous(const character*) const;
};

ITEM(scroll, item)
{
 public:
  virtual truth CanBeRead(character*) const;
  virtual truth IsReadable(const character*) const { return true; }
  virtual truth ReceiveDamage(character*, int, int, int);
};

ITEM(scrollofteleportation, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(scrollofcharging, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(nut, item)
{
};

ITEM(leftnutofpetrus, nut)
{
 public:
  virtual void Be() { }
  virtual truth IsPetrussNut() const { return true; }
  virtual truth IsConsumable() const { return false; }
};

ITEM(bone, item)
{
 public:
  virtual truth DogWillCatchAndConsume(const character*) const;
};

ITEM(loaf, item)
{
 protected:
  virtual void AddPostFix(festring& String) const { AddLumpyPostFix(String); }
  virtual truth ShowMaterial() const { return false; }
};

ITEM(scrollofwishing, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(copyofleftnutofpetrus, nut)
{
};

ITEM(wand, item)
{
 public:
  virtual truth Apply(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { TimesUsed = 0; }
  virtual truth IsAppliable(const character*) const { return true; }
  virtual truth IsZappable(const character*) const { return true; }
  virtual truth IsChargeable(const character*) const { return true; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth Zap(character*, v2, int);
  virtual void AddInventoryEntry(const character*, festring&, int, truth) const;
  virtual long GetPrice() const;
  virtual truth IsExplosive() const { return true; }
 protected:
  virtual void PostConstruct();
  void BreakEffect(character*, const festring&);
  ulong GetSpecialParameters() const;
  int Charges;
  int TimesUsed;
};

ITEM(scrollofchangematerial, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(avatarofvalpurus, item)
{
 public:
  virtual void Be() { }
  virtual truth IsTheAvatar() const { return true; }
  virtual truth IsConsumable() const { return false; }
};

ITEM(kiwi, item)
{
};

ITEM(pineapple, item)
{
};

ITEM(palmbranch, item)
{
 public:
  virtual truth IsShield(const character*) const { return true; }
};

ITEM(backpack, materialcontainer)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(const character*) const { return true; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth IsExplosive() const;
  virtual long GetTotalExplosivePower() const;
  virtual void SpillFluid(character*, liquid*, int = 0);
 protected:
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
};

ITEM(holybook, item)
{
 public:
  virtual truth CanBeRead(character*) const;
  virtual truth IsReadable(const character*) const { return true; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual void FinishReading(character*);
 protected:
  virtual col16 GetMaterialColorA(int) const;
  virtual truth ShowMaterial() const { return false; }
};

ITEM(fiftymillionroubles, item)
{
};

ITEM(oillamp, item)
{
 public:
  oillamp();
  oillamp(const oillamp&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth GetInhabitedByGenie() const { return InhabitedByGenie; }
  virtual void SetInhabitedByGenie(truth What) { InhabitedByGenie = What; }
  virtual truth Apply(character*);
  virtual truth IsAppliable(const character*) const { return true; }
 protected:
  truth InhabitedByGenie;
};

ITEM(stone, item)
{
 public:
  virtual long GetTruePrice() const;
 protected:
  virtual truth WeightIsIrrelevant() const { return true; }
};

ITEM(scrolloftaming, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(mine, materialcontainer)
{
 public:
  mine() : Active(false) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void StepOnEffect(character*);
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth IsActive() const { return Active; }
  virtual void SetIsActive(truth);
  virtual truth CanBeSeenBy(const character*) const;
  virtual truth Apply(character* User);
  virtual truth IsAppliable(const character*) const { return true; }
  virtual truth IsDangerous(const character* Stepper) const { return WillExplode(Stepper); }
  virtual truth WillExplode(const character*) const;
  virtual int GetTeam() const { return Team; }
  virtual void SetTeam(int What) { Team = What; }
  virtual truth CheckPickUpEffect(character*);
  virtual void Search(const character*, int);
  virtual truth NeedDangerSymbol() const { return IsActive(); }
  virtual void FinalProcessForBone();
  virtual void TeleportRandomly();
 protected:
  virtual truth AddAdjective(festring&, truth) const;
  truth Active;
  int Team;
  std::set<int> DiscoveredByTeam;
};

ITEM(key, item)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(const character*) const { return true; }
  virtual truth CanOpenDoors() const { return true; }
  virtual truth CanOpenLockType(int AnotherLockType) const { return GetConfig() == AnotherLockType; }
};

ITEM(headofelpuri, item) // can't wear equipment, so not "head"
{
 public:
  virtual truth IsHeadOfElpuri() const { return true; }
  virtual truth IsConsumable() const { return false; }
  virtual void Be() { }
};

ITEM(whistle, item)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(const character*) const { return true; }
  virtual void BlowEffect(character*);
 protected:
  virtual col16 GetMaterialColorB(int) const;
};

ITEM(magicalwhistle, whistle)
{
 public:
  magicalwhistle() : LastUsed(0) { }
  virtual void BlowEffect(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void FinalProcessForBone();
 protected:
  ulong LastUsed;
};

ITEM(itemcontainer, item)
{
 public:
  itemcontainer();
  itemcontainer(const itemcontainer&);
  virtual ~itemcontainer();
  virtual truth Open(character*);
  virtual truth IsOpenable(const character*) const { return true; }
  virtual truth TryKey(item*, character*);
  virtual truth HasLock(const character*) const { return true; }
  virtual void Lock() { Locked = true; }
  virtual truth IsLocked() const { return Locked; }
  virtual void SetIsLocked(truth What) { Locked = What; }
  virtual stack* GetContained() const { return Contained; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual truth Polymorph(character*, stack*);
  virtual void CalculateVolumeAndWeight();
  virtual truth ContentsCanBeSeenBy(const character*) const;
  virtual long GetTruePrice() const;
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual void DrawContents(const character*);
  virtual truth Apply(character* Applier) { return Open(Applier); }
  virtual truth IsAppliable(const character*) const { return true; }
  virtual void SetItemsInside(const fearray<contentscript<item> >&, int);
  virtual truth AllowContentEmitation() const { return false; }
  virtual truth IsDestroyable(const character*) const;
  virtual int GetOfferValue(int) const;
  virtual void SortAllItems(const sortdata&) const;
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone();
  virtual material* RemoveMaterial(material*);
  virtual void SetLifeExpectancy(int, int);
  virtual void CalculateEnchantment();
  virtual int GetTeleportPriority() const;
  virtual void SetParameters(int);
  virtual void Disappear();
 protected:
  virtual col16 GetMaterialColorB(int) const;
  virtual void PostConstruct();
  stack* Contained;
  truth Locked;
};

ITEM(beartrap, item)
{
 public:
  beartrap();
  beartrap(const beartrap&);
  virtual ~beartrap();
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void StepOnEffect(character*);
  virtual truth CheckPickUpEffect(character*);
  virtual truth IsPickable(character*) const;
  virtual truth IsActive() const { return Active; }
  virtual void SetIsActive(truth);
  virtual truth CanBeSeenBy(const character*) const;
  virtual truth Apply(character*);
  virtual v2 GetBitmapPos(int) const;
  virtual truth IsAppliable(const character*) const { return true; }
  virtual truth IsDangerous(const character*) const { return Active; }
  virtual int GetTeam() const { return Team; }
  virtual void SetTeam(int What) { Team = What; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual void Search(const character*, int);
  virtual truth NeedDangerSymbol() const { return IsActive(); }
  virtual void Fly(character*, int, int);
  virtual void FinalProcessForBone();
  virtual void TeleportRandomly();
  virtual ulong GetTrapID() const { return TrapData.TrapID; }
  virtual ulong GetVictimID() const { return TrapData.VictimID; }
  virtual void UnStick() { TrapData.VictimID = 0; }
  virtual void UnStick(int I) { TrapData.BodyParts &= ~(1 << I); }
  virtual truth TryToUnStick(character*, v2);
  virtual void RemoveFromSlot();
  virtual int GetTrapType() const { return GetType() | ITEM_TRAP; }
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void DonateSlotTo(item*);
 protected:
  virtual truth AddAdjective(festring&, truth) const;
  truth IsStuck() const { return TrapData.VictimID; }
  int GetBaseTrapDamage() const;
  int Team;
  std::set<int> DiscoveredByTeam;
  trapdata TrapData;
  truth Active;
};

ITEM(stethoscope, item)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(const character*) const { return true; };
};

ITEM(scrollofenchantweapon, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(scrollofenchantarmor, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(skull, item)
{
};

ITEM(scrollofrepair, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(encryptedscroll, scroll)
{
 public:
  virtual void Be() { }
  virtual truth Read(character*);
  virtual truth ReceiveDamage(character*, int, int, int) { return false; }
  virtual truth IsEncryptedScroll() const { return true; }
};

ITEM(horn, item)
{
 public:
  horn() : LastUsed(0) { }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual truth Apply(character*);
  virtual truth IsAppliable(const character*) const { return true; }
  virtual void FinalProcessForBone();
 protected:
  ulong LastUsed;
};

ITEM(carrot, item)
{
 public:
  virtual truth BunnyWillCatchAndConsume(const character*) const;
 protected:
  virtual col16 GetMaterialColorB(int) const;
};

ITEM(charmlyre, item)
{
 public:
  charmlyre();
  virtual truth Apply(character*);
  virtual truth IsAppliable(const character*) const { return true; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void FinalProcessForBone();
 protected:
  virtual col16 GetMaterialColorB(int) const;
  ulong LastUsed;
};

ITEM(scrollofdetectmaterial, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(stick, item)
{
 protected:
  virtual void AddPostFix(festring& String) const { AddLumpyPostFix(String); }
  virtual truth ShowMaterial() const { return false; }
  virtual truth WeightIsIrrelevant() const { return true; }
};

ITEM(scrollofhardenmaterial, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(scrollofgolemcreation, scroll)
{
 public:
  virtual void FinishReading(character*);
};

#endif
