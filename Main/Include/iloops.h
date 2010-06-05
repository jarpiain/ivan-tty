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

#ifndef __ILOOPS_H__
#define __ILOOPS_H__

#include "feloops.h"
#include "char.h"

struct combinebodypartpredicates : public combinepredicates<const character, bodypart>
{
  typedef combinepredicates<const character, bodypart> base;
  truth operator()(const character* C, base::routine F, truth OrBit) const
  {
    return base::operator()(C, &character::GetBodyPart, F, C->GetBodyParts(), OrBit);
  }
};

template <class param>
struct combinebodypartpredicateswithparam : public combinepredicateswithparam<const character, bodypart, param>
{
  typedef combinepredicateswithparam<const character, bodypart, param> base;
  truth operator()(const character* C, typename base::routine F, param P, truth OrBit) const
  {
    return base::operator()(C, &character::GetBodyPart, F, P, C->GetBodyParts(), OrBit);
  }
};

struct combineequipmentpredicates : public combinepredicates<const character, item>
{
  typedef combinepredicates<const character, item> base;
  truth operator()(const character* C, base::routine F, truth OrBit) const
  {
    return base::operator()(C, &character::GetEquipment, F, C->GetEquipments(), OrBit);
  }
};

template <class param>
struct combineequipmentpredicateswithparam : public combinepredicateswithparam<const character, item, param>
{
  typedef combinepredicateswithparam<const character, item, param> base;
  truth operator()(const character* C, typename base::routine F, param P, truth OrBit) const
  {
    return base::operator()(C, &character::GetEquipment, F, P, C->GetEquipments(), OrBit);
  }
};

struct doforbodyparts : public doforelements<const character, bodypart>
{
  typedef doforelements<const character, bodypart> base;
  void operator()(const character* C, base::routine F) const
  {
    base::operator()(C, &character::GetBodyPart, F, C->GetBodyParts());
  }
};

template <class param>
struct doforbodypartswithparam : public doforelementswithparam<const character, bodypart, param>
{
  typedef doforelementswithparam<const character, bodypart, param> base;
  void operator()(const character* C, typename base::routine F, param P) const
  {
    base::operator()(C, &character::GetBodyPart, F, P, C->GetBodyParts());
  }
};

struct doforequipments : public doforelements<const character, item>
{
  typedef doforelements<const character, item> base;
  void operator()(const character* C, base::routine F) const
  {
    base::operator()(C, &character::GetEquipment, F, C->GetEquipments());
  }
  void operator()(const character* C, void (item::*F)() const) const
  {
    base::operator()(C, &character::GetEquipment, base::routine(F), C->GetEquipments());
  }
};

template <class param>
struct doforequipmentswithparam : public doforelementswithparam<const character, item, param>
{
  typedef doforelementswithparam<const character, item, param> base;
  void operator()(const character* C, typename base::routine F, param P) const
  {
    base::operator()(C, &character::GetEquipment, F, P, C->GetEquipments());
  }
  void operator()(const character* C, void (item::*F)(param) const, param P) const
  {
    base::operator()(C, &character::GetEquipment, (typename base::routine)(F), P, C->GetEquipments());
  }
};

struct sumbodypartproperties : public sumproperties<const character, bodypart>
{
  typedef sumproperties<const character, bodypart> base;
  int operator()(const character* C, base::routine F) const
  {
    return base::operator()(C, &character::GetBodyPart, F, C->GetBodyParts());
  }
};

template <class param>
struct findequipment : public findelement<const character, item, param>
{
  typedef findelement<const character, item, param> base;
  item* operator()(const character* C, typename base::routine F, param P) const
  {
    return base::operator()(C, &character::GetEquipment, F, P, C->GetEquipments());
  }
};

#endif
