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

#ifdef USE_SDL
#include <vector>
#include "SDL.h"
#endif

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
#ifdef USE_SDL
  static void Init();
  static void SetQuitMessageHandler(truth (*What)())
  { QuitMessageHandler = What; }
  static void UpdateTick() { Tick = SDL_GetTicks() / 40; }
#endif
 private:
#ifdef USE_SDL
  static void ProcessMessage(SDL_Event*);
  static std::vector<int> KeyBuffer;
  static truth (*QuitMessageHandler)();
#endif
  static truth (*ControlLoop[MAX_CONTROLS])();
  static int Controls;
  static ulong Tick;
  static truth ControlLoopsEnabled;
};

#endif
