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

#ifndef __STACK_H__
#define __STACK_H__

#include "lsquare.h"
#include "slot.h"

class felist;
class entity;

typedef std::vector<item*> itemvector;
typedef std::vector<itemvector> itemvectorvector;

/* Stack contains an arbitrary number of items in a linked list, which can
   be browsed using stackiterators like this:

   for(stackiterator i = Stack->GetBottom(); i.HasItem(); ++i)
   {
   item* Item = *i;
   i->ItemMemberFunction();
   }

   or using a temporary vector:

   itemvector ItemVector;
   Stack->FillItemVector(ItemVector);

   for(int c = 0; c < ItemVector.size(); ++c)
   {
   item* Item = ItemVector[c];
   ItemVector[c]->ItemMemberFunction();
   }

   The former is faster and should be used if items can't be removed nor
   added during the loop (at worst, this could cause a crash), otherwise
   the latter is necessary.

   Items are added to stack with Stack->AddItem(Item) and removed by
   Item->RemoveFromSlot(). A number of Stack->DrawContents() functions
   is provided as an easy item-selecting GUI. */

class stackiterator
{
 public:
  stackiterator(stackslot* Slot) : Slot(Slot) { }
  stackiterator& operator++() { Slot = Slot->Next; return *this; }
  stackiterator& operator--() { Slot = Slot->Last; return *this; }
  truth HasItem() const { return truth(Slot); }
  item* operator->() const { return Slot->Item; }
  item* operator*() const { return Slot->Item; }
  const stackslot& GetSlot() const { return *Slot; }
 private:
  stackslot* Slot;
};

class stack
{
 public:
  stack(square*, entity*, ulong = 0);
  ~stack();
  void Load(inputfile&);
  void Draw(const character*, blitdata&, int) const;
  void AddItem(item*, truth = true);
  void RemoveItem(stackslot*);
  item* GetItem(int) const;
  stackiterator GetBottom() const { return stackiterator(Bottom); }
  stackiterator GetTop() const { return stackiterator(Top); }
  int GetItems() const { return Items; }
  int GetSideItems(int) const;
  int GetVisibleItems(const character*) const;
  int GetNativeVisibleItems(const character*) const;
  int GetVisibleSideItems(const character*, int) const;
  void SetMotherSquare(square* What) { MotherSquare = What; }
  item* DrawContents(const character*, const festring&, int = 0, sorter = 0) const;
  int DrawContents(itemvector&, const character*, const festring&, int = 0, sorter = 0) const;
  int DrawContents(itemvector&, stack*, const character*, const festring&, const festring&, const festring&, const festring&, col16, int, sorter = 0) const;
  v2 GetPos() const;
  void Clean(truth = false);
  void Save(outputfile&) const;
  int SearchItem(item*) const;
  square* GetSquareUnder() const;
  lsquare* GetLSquareUnder() const { return static_cast<lsquare*>(GetSquareUnder()); }
  truth SortedItems(const character*, sorter) const;
  void BeKicked(character*, int, int);
  void Polymorph(character*);
  void CheckForStepOnEffect(character*);
  lsquare* GetLSquareTrulyUnder(int) const;
  void ReceiveDamage(character*, int, int, int = YOURSELF);
  void TeleportRandomly(uint = 0xFFFF);
  void FillItemVector(itemvector&) const;
  truth IsOnGround() const;
  truth RaiseTheDead(character*);
  truth TryKey(item*, character*);
  truth Open(character*);
  void SignalVolumeAndWeightChange();
  void CalculateVolumeAndWeight();
  long GetVolume() const { return Volume; }
  long GetWeight() const { return Weight; }
  long GetWeight(const character*, int) const;
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  area* GetArea() const { return GetSquareUnder()->GetArea(); }
  lsquare* GetNearLSquare(v2 Pos) const { return GetLSquareUnder()->GetLevel()->GetLSquare(Pos); }
  col24 GetEmitation() const { return Emitation; }
  void SignalEmitationIncrease(int, col24);
  void SignalEmitationDecrease(int, col24);
  void CalculateEmitation();
  col24 GetSideEmitation(int);
  truth CanBeSeenBy(const character*, int) const;
  truth IsDangerous(const character*) const;
  truth Duplicate(int, ulong = 0);
  void MoveItemsTo(stack*);
  void MoveItemsTo(slot*);
  item* GetBottomItem(const character*, truth) const;
  item* GetBottomVisibleItem(const character*) const;
  item* GetBottomSideItem(const character*, int, truth) const;
  void Pile(itemvectorvector&, const character*, int, sorter = 0) const;
  long GetTruePrice() const;
  static int GetSelected() { return Selected; }
  static void SetSelected(int What) { Selected = What; }
  truth TakeSomethingFrom(character*, const festring&);
  truth PutSomethingIn(character*, const festring&, long, ulong);
  truth IsVisible() const { return !(Flags & HIDDEN); }
  int GetSpoiledItems() const;
  void SortAllItems(const sortdata&) const;
  void Search(const character*, int);
  truth NeedDangerSymbol(const character*) const;
  void PreProcessForBone();
  void PostProcessForBone();
  void FinalProcessForBone();
  void AddElement(item*, truth = false);
  void SpillFluid(character*, liquid*, long);
  void AddItems(const itemvector&);
  void MoveItemsTo(itemvector&, int);
  void Freeze() { Flags |= FREEZED; }
  void UnFreeze() { Flags &= ~FREEZED; }
  void DropSideItems();
  truth DetectMaterial(const material*) const;
  void SetLifeExpectancy(int, int);
  truth Necromancy(character*);
  void CalculateEnchantments();
  const character* FindCarrier() const;
 private:
  void RemoveElement(stackslot*);
  void AddContentsToList(felist&, const character*, const festring&, int, int, sorter) const;
  int SearchChosen(itemvector&, const character*, int, int, int, int, sorter = 0) const;
  static truth AllowDamage(int, int);
  static int Selected;
  stackslot* Bottom;
  stackslot* Top;
  square* MotherSquare;
  entity* MotherEntity;
  long Volume;
  long Weight;
  col24 Emitation : 24;
  ulong Flags : 8;
  int Items;
};

#endif
