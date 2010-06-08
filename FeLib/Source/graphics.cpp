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

#include <curses.h>
#include <iostream>
#include <cstdio>
#include <cstdarg>

#include "graphics.h"
#include "error.h"

void graphics::Init()
{
  static truth AlreadyInstalled = false;

  if(!AlreadyInstalled)
  {
    AlreadyInstalled = true;

    (void) initscr();
    keypad(stdscr, TRUE);
    (void) nonl();
    (void) cbreak();
    (void) noecho();
    ESCDELAY = 25;

    if(has_colors())
    {
      start_color();
      init_pair(1, COLOR_RED,     COLOR_BLACK);
      init_pair(2, COLOR_GREEN,   COLOR_BLACK);
      init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
      init_pair(4, COLOR_BLUE,    COLOR_BLACK);
      init_pair(5, COLOR_CYAN,    COLOR_BLACK);
      init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
      init_pair(7, COLOR_BLACK,   COLOR_BLACK);
    }

    atexit(graphics::DeInit);
  }
}

void graphics::DeInit()
{
  endwin();
}

void graphics::BlitDBToScreen()
{
  refresh();
}

void graphics::MoveCursor(v2 Pos)
{
  move(Pos.Y, Pos.X);
}

void graphics::ClearScreen()
{
  clear();
}

void graphics::ClearRect(int x1, int y1, int x2, int y2)
{
  for(int Row = y1; Row < y2; ++Row)
  {
    move(Row, x1);
    for(int Col = x1; Col < x2; ++Col)
      addch(' ');
  }
}

void graphics::PutChar(int Glyph, int Attr)
{
  int Color = Attr & 0x7;
  int Bright = (Attr & 0x8) ? A_BOLD : 0;
  int Blink = (Attr & 0x10) ? A_BLINK : 0;
  addch(Glyph | COLOR_PAIR(Color) | Bright | Blink);
}

void graphics::PutStr(const char* Text, int Attr)
{
  int Color = Attr & 0x7;
  int Bright = (Attr & 0x8) ? A_BOLD : 0;
  int Blink = (Attr & 0x10) ? A_BLINK : 0;
  while(*Text)
  {
    addch(*Text++ | COLOR_PAIR(Color) | Bright | Blink);
  }
}

void graphics::PutStrf(int Attr, const char* Fmt, ...)
{
  va_list Argp;
  va_start(Argp, Fmt);
  char Linebuf[80];
  vsprintf(Linebuf, Fmt, Argp);
  PutStr(Linebuf, Attr);
  va_end(Argp);
}
