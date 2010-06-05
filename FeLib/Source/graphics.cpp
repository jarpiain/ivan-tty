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

#ifdef USE_SDL
#include "SDL.h"
#endif

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "error.h"
#include "rawbit.h"

void (*graphics::SwitchModeHandler)();

#ifdef USE_SDL
SDL_Surface* graphics::Screen;
#endif

bitmap* graphics::DoubleBuffer;
v2 graphics::Res;
int graphics::ColorDepth;
rawbitmap* graphics::DefaultFont = 0;

void graphics::Init()
{
  static truth AlreadyInstalled = false;

  if(!AlreadyInstalled)
  {
    AlreadyInstalled = true;

#ifdef USE_SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE))
      ABORT("Can't initialize SDL.");
#endif

    atexit(graphics::DeInit);
  }
}

void graphics::DeInit()
{
  delete DefaultFont;
  DefaultFont = 0;

#ifdef USE_SDL
  SDL_Quit();
#endif
}

#ifdef USE_SDL

void graphics::SetMode(const char* Title, const char* IconName,
		       v2 NewRes, truth FullScreen)
{
  if(IconName)
  {
    SDL_Surface* Icon = SDL_LoadBMP(IconName);
    SDL_SetColorKey(Icon, SDL_SRCCOLORKEY,
		    SDL_MapRGB(Icon->format, 255, 255, 255));
    SDL_WM_SetIcon(Icon, NULL);
  }

  ulong Flags = SDL_SWSURFACE;

  if(FullScreen)
  {
    SDL_ShowCursor(SDL_DISABLE);
    Flags |= SDL_FULLSCREEN;
  }

  Screen = SDL_SetVideoMode(NewRes.X, NewRes.Y, 16, Flags);

  if(!Screen)
    ABORT("Couldn't set video mode.");

  SDL_WM_SetCaption(Title, 0);
  globalwindowhandler::Init();
  DoubleBuffer = new bitmap(NewRes);
  Res = NewRes;
  ColorDepth = 16;
}

void graphics::BlitDBToScreen()
{
  if(SDL_MUSTLOCK(Screen) && SDL_LockSurface(Screen) < 0)
    ABORT("Can't lock screen");

  packcol16* SrcPtr = DoubleBuffer->GetImage()[0];
  packcol16* DestPtr = static_cast<packcol16*>(Screen->pixels);
  ulong ScreenYMove = (Screen->pitch >> 1);
  ulong LineSize = Res.X << 1;

  for(int y = 0; y < Res.Y; ++y, SrcPtr += Res.X, DestPtr += ScreenYMove)
    memcpy(DestPtr, SrcPtr, LineSize);

  if(SDL_MUSTLOCK(Screen))
    SDL_UnlockSurface(Screen);

  SDL_UpdateRect(Screen, 0, 0, Res.X, Res.Y);
}

void graphics::SwitchMode()
{
  ulong Flags;

  if(Screen->flags & SDL_FULLSCREEN)
  {
    SDL_ShowCursor(SDL_ENABLE);
    Flags = SDL_SWSURFACE;
  }
  else
  {
    SDL_ShowCursor(SDL_DISABLE);
    Flags = SDL_SWSURFACE|SDL_FULLSCREEN;
  }

  if(SwitchModeHandler)
    SwitchModeHandler();

  Screen = SDL_SetVideoMode(Res.X, Res.Y, ColorDepth, Flags);

  if(!Screen)
    ABORT("Couldn't toggle fullscreen mode.");

  BlitDBToScreen();
}

#endif

void graphics::LoadDefaultFont(const festring& FileName)
{
  DefaultFont = new rawbitmap(FileName);
}
