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

#ifndef __ICONF_H__
#define __ICONF_H__

#include "config.h"

class ivanconfig
{
 public:
  static const festring& GetDefaultPetName() { return DefaultPetName.Value; }
  static truth GetWarnAboutDanger() { return WarnAboutDanger.Value; }
  static truth GetAutoDropLeftOvers() { return AutoDropLeftOvers.Value; }
  static truth GetUseAlternativeKeys() { return UseAlternativeKeys.Value; }
  static void Load() { configsystem::Load(); }
  static void CalculateContrastLuminance();
  static col24 GetContrastLuminance() { return ContrastLuminance; }
  static void Initialize();
 private:
  static stringoption DefaultPetName;
  static truthoption WarnAboutDanger;
  static truthoption AutoDropLeftOvers;
  static truthoption UseAlternativeKeys;
  static col24 ContrastLuminance;
};

#endif
