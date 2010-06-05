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

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#ifdef USE_SDL
#include "SDL.h"
#endif

#include "v2.h"

#define DOUBLE_BUFFER graphics::GetDoubleBuffer()
#define RES graphics::GetRes()
#define FONT graphics::GetDefaultFont()

class bitmap;
class rawbitmap;
class festring;

class graphics
{
 public:
  friend class bitmap;
  static void Init();
  static void DeInit();
#ifdef USE_SDL
  static void SwitchMode();
#endif
  static void SetMode(const char*, const char*, v2, truth);
  static void BlitDBToScreen();
  static v2 GetRes() { return Res; }
  static bitmap* GetDoubleBuffer() { return DoubleBuffer; }
  static void LoadDefaultFont(const festring&);
  static rawbitmap* GetDefaultFont() { return DefaultFont; }
  static void SetSwitchModeHandler(void (*What)())
  { SwitchModeHandler = What; }
 private:
  static void (*SwitchModeHandler)();
#ifdef USE_SDL
  static SDL_Surface* Screen;
#endif
  static bitmap* DoubleBuffer;
  static v2 Res;
  static int ColorDepth;
  static rawbitmap* DefaultFont;
};

#endif
