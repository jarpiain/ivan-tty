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

#ifndef __WHANDLER_H__
#define __WHANDLER_H__

#include "felibdef.h"

#define GET_KEY globalwindowhandler::GetKey
#define READ_KEY globalwindowhandler::ReadKey
#define GET_TICK globalwindowhandler::GetTick

class globalwindowhandler
{
 public:
  static int GetKey(truth = true);
  static int ReadKey();
  static void InstallControlLoop(truth (*)());
  static void DeInstallControlLoop(truth (*)());
  static ulong GetTick() { return Tick; }
  static truth ControlLoopsInstalled() { return Controls; }
  static void EnableControlLoops() { ControlLoopsEnabled = true; }
  static void DisableControlLoops() { ControlLoopsEnabled = false; }
 private:
  static truth (*ControlLoop[MAX_CONTROLS])();
  static int Controls;
  static ulong Tick;
  static truth ControlLoopsEnabled;
};

#endif
