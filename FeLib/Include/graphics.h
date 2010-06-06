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

#include <curses.h>
#include "v2.h"

#define DOUBLE_BUFFER stdscr
#define RES graphics::GetRes()

class festring;

class graphics
{
 public:
  static void Init();
  static void DeInit();
  static void BlitDBToScreen();
  static v2 GetRes() { return v2(80,24); }

  static void MoveCursor(v2);
  static void ClearScreen();
  static void ClearGrid() { PutChar(' ', WHITE); }
  static void ClearRect(int x1, int y1, int x2, int y2);
  static void PutChar(int glyph, col16 attr);
  static void PutStr(const char* text, col16 attr);
  static void PutStrf(col16 attr, const char* format, ...);
};

#endif
