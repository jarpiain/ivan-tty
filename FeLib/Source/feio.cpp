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

#include <ctime>
#include <cctype>

#ifdef LINUX
#include <dirent.h>
#include <stddef.h>
#include <cstdio>
#include <sys/types.h>
#include <algorithm>
#endif

#include <curses.h>

#include "graphics.h"
#include "feio.h"
#include "whandler.h"
#include "felist.h"
#include "festring.h"

#define PENT_WIDTH 70

/* Prints screen full of Text in color Color. If GKey is true function
   waits for keypress. */

void iosystem::TextScreen(const festring& Text, col16 Color, truth GKey)
{
  int Bright = Color & BRIGHT ? A_BOLD : 0;
  int Blink = Color & BLINK ? A_BLINK : 0;
  attron(COLOR_PAIR(Color & 0x7) | Bright | Blink);
  clear();

  int Row = 0;
  int Col = 0;
  move(Row, Col);

  for(int i = 0; i < Text.GetSize(); ++i)
  {
    if(Text[i] == '\n')
    {
      ++Row; Col = 0;
      move(Row, Col);
    }
    else
    {
      addch(Text[i]);
      ++Col; move(Row, Col);
    }
  }

  if(GKey)
    GET_KEY();
}

/* Returns amount of chars cSF in string sSH */

int CountChars(char cSF, const festring& sSH)
{
  int iReturnCounter = 0;

  for(festring::sizetype i = 0; i < sSH.GetSize(); ++i)
    if(sSH[i] == cSF)
      ++iReturnCounter;

  return iReturnCounter;
}

/* Asks the user a question requiring a string answer. The answer is saved
   to Input. Input can also already have a default something retyped for
   the user. Topic is the question or other topic for the question. Pos the
   cordinates of where the question is printed on the screen. Color is the
   col of all the fonts in this function. Enter is only accepted when the
   answers length is between MinLetters and MaxLetters. If Fade is true the
   question is asked on a black background and the transition to that is a
   fade. If AllowExit is true the user can abort with the esc-key.

   The function returns ABORTED (when user aborts with esc) or
   NORMAL_EXIT. */

int iosystem::StringQuestion(festring& Input,
			     const festring& Topic,
			     col16 Color,
			     festring::sizetype MinLetters,
			     festring::sizetype MaxLetters,
			     truth AllowExit,
			     stringkeyhandler StringKeyHandler)
{
  for(;;)
  {
    graphics::MoveCursor(v2(0,0));
    graphics::PutStrf(Color, "%60s", "");
    graphics::MoveCursor(v2(0,0));
    graphics::PutStrf(Color, "%s %s", Topic.CStr(), Input.CStr());
    int Key = GET_KEY();

    if(Key == KEY_ESC)
    {
      return ABORTED;
    }
    else if(Key == '\n' || Key == '\r')
    {
      if(Input.GetSize() >= MinLetters
         && Input.GetSize() <= MaxLetters)
      {
        return NORMAL_EXIT;
      }
    }

    else if(Key == 127 || Key == '\b') // delete or backspace
    {
      if(Input.GetSize() > 0)
      {
        Input.Resize(Input.GetSize() - 1);
      }
    }
    else
    {
      Input << (char)Key;
    }
  }

  return NORMAL_EXIT;
}

/* Ask a question defined by Topic. This function only accepts numbers.
   The question is drawn to cordinates given by Pos. All fonts are Color
   coled. If Fade is true the question is asked on a black background
   and the transition to that is a fade. */

long iosystem::NumberQuestion(const festring& Topic, col16 Color,
			      truth ReturnZeroOnEsc)
{
  festring Buf;
  int Sq = StringQuestion(Buf, Topic, Color, 0, 12, true, 0);
  if(ReturnZeroOnEsc && Sq == ABORTED)
    return 0;

  return atoi(Buf.CStr());
}

/* Asks a question defined by Topic and the answer is numeric. The value is
   represented by a scroll bar. The topic is drawn to position Pos. Step is
   the step size. Min and Max are the minimum and maximum values. If the
   player aborts with the esc key AbortValue is returned. Color1 is the
   left portion controls the col of left portion of the scroll bar and
   Color2 the right portion. LeftKey and RightKey are the keys for changing
   the scrollbar. Although '<' and '>' also work always. If Fade is true
   the screen is faded to black before drawing th scrollbar. If Handler is
   set it is called always when the value of the scroll bar changes. */

long iosystem::ScrollBarQuestion(const festring& Topic, v2 Pos,
				 long StartValue, long Step,
				 long Min, long Max, long AbortValue,
				 col16 TopicColor, col16 Color1,
				 col16 Color2, int LeftKey, int RightKey,
				 truth Fade, void (*Handler)(long))
{
  return 0;
}

truth iosystem::IsAcceptableForStringQuestion(char Key)
{
  if(Key == '|' || Key == '<' || Key == '>' || Key == '?' || Key == '*'
     || Key == '/' || Key == '\\' || Key == ':')
    return false;

  if(Key < 0x20
     && !(Key == KEY_BACK_SPACE || Key == KEY_ENTER || Key == KEY_ESC))
    return false;

  return true;
}
