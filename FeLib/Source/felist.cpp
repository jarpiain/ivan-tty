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

#include <fstream>

#include "felist.h"
#include "graphics.h"
#include "save.h"
#include "whandler.h"
#include "festring.h"

truth Redraw = false;

truth felist::NeedRedraw()
{
  truth ret = Redraw;
  Redraw = false;
  return ret;
}

struct felistentry
{
  felistentry() { }
  felistentry(const festring&, col16, uint, truth);
  festring String;
  col16 Color;
  uint Marginal;
  truth Selectable;
};

felistentry::felistentry(const festring& String, col16 Color,
			 uint Marginal, truth Selectable)
: String(String), Color(Color), Marginal(Marginal),
  Selectable(Selectable)
{
}

outputfile& operator<<(outputfile& SaveFile, const felistentry* Entry)
{
  SaveFile << Entry->String << Entry->Color
	   << Entry->Marginal << Entry->Selectable;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, felistentry*& Entry)
{
  Entry = new felistentry;
  SaveFile >> Entry->String >> Entry->Color
	   >> Entry->Marginal >> Entry->Selectable;
  return SaveFile;
}

struct felistdescription
{
  felistdescription() { }
  felistdescription(const festring& String, col16 Color)
  : String(String), Color(Color) { }
  festring String;
  col16 Color;
};

felist::felist(const festring& Topic, col16 TopicColor, uint Maximum,
               uint Row, uint Col, uint Width, uint Height)
: Maximum(Maximum), Selected(0), Pos(Col, Row), Width(Width),
  PageLength(Height), BackColor(0), Flags(SELECTABLE|FADE),
  UpKey(KEY_UP), DownKey(KEY_DOWN)
{
  AddDescription(Topic, TopicColor);
}

felist::~felist()
{
  Empty();

  for(uint c = 0; c < Description.size(); ++c)
    delete Description[c];
}

truth felist::IsEmpty() const
{ return Entry.empty(); }
uint felist::GetLength() const
{ return Entry.size(); }
uint felist::GetLastEntryIndex() const
{ return Entry.size() - 1; }
festring felist::GetEntry(uint I) const
{ return Entry[I]->String; }
col16 felist::GetColor(uint I) const
{ return Entry[I]->Color; }
void felist::SetColor(uint I, col16 What)
{ Entry[I]->Color = What; }
void felist::AddDescription(const festring& Str, col16 Color)
{ Description.push_back(new felistdescription(Str, Color)); }

void felist::Pop()
{
  delete Entry[GetLastEntryIndex()];
  Entry.pop_back();
}

uint felist::Draw()
{
  while(Entry.size() && Entry[GetLastEntryIndex()]->String.IsEmpty())
    Pop();

  if(Entry.empty())
    return LIST_WAS_EMPTY;

  uint c;
  uint Return, Selectables = 0;

  for(c = 0; c < Entry.size(); ++c)
    if(Entry[c]->Selectable)
      ++Selectables;

  if(Selected >= Selectables)
    Selected = Selectables - 1;

  if(Flags & SELECTABLE)
    PageBegin = Selected - Selected % PageLength;
  else if(Flags & INVERSE_MODE)
    PageBegin = GetLastEntryIndex() - GetLastEntryIndex() % PageLength;
  else
    PageBegin = 0;

  for(;;)
  {
    truth AtTheEnd = DrawPage();
    int Pressed = GET_KEY(false);

    if(Flags & SELECTABLE && Pressed >= 'A'
             && Pressed <= 'Z' && Pressed - 'A' < PageLength
             && Pressed - 'A' + PageBegin < Selectables)
    {
      Return = Selected = Pressed - 'A' + PageBegin;
      break;
    }

    if(Flags & SELECTABLE && Pressed >= 'a'
             && Pressed <= 'z' && Pressed - 'a' < PageLength
             && Pressed - 'a' + PageBegin < Selectables)
    {
      Return = Selected = Pressed - 'a' + PageBegin;
      break;
    }

    if(Flags & SELECTABLE && (Pressed == UpKey || Pressed == KEY_UP))
    {
      if(Selected)
      {
        --Selected;
        if(Selected < PageBegin)
        {
          PageBegin -= PageLength;
        }
      }
      else
      {
        for(c = 0, Selected = 0; c < Entry.size(); ++c)
          if(Entry[c]->Selectable)
            ++Selected;

        --Selected;
        PageBegin = Selected - Selected % PageLength;
      }

      continue;
    }

    if(Flags & SELECTABLE && (Pressed == DownKey || Pressed == KEY_DOWN))
    {
      if(!AtTheEnd || Selected != Selectables - 1)
      {
        ++Selected;
        if(Selected > PageBegin + PageLength - 1)
        {
          PageBegin += PageLength;
        }
      }
      else
      {
        Selected = PageBegin = 0;
      }

      continue;
    }

    if(Flags & SELECTABLE && (Pressed == '\n' || Pressed == '\r'))
    {
      Return = Selected;
      break;
    }

    if(Pressed == KEY_ESC)
    {
      Return = ESCAPED;
      break;
    }

    if((AtTheEnd && !(Flags & INVERSE_MODE))
       || (!PageBegin && Flags & INVERSE_MODE))
    {
      Return = NOTHING_SELECTED;
      break;
    }
    else
    {
      if(Flags & INVERSE_MODE)
        PageBegin -= PageLength;
      else
        PageBegin += PageLength;

      if(Flags & SELECTABLE)
        Selected = PageBegin;
    }
  }

  graphics::ClearScreen();
  Redraw = true;
  return Return;
}

static festring Str;

truth felist::DrawPage() const
{
  graphics::ClearScreen();
  int row = Description.size() + 1;
  DrawDescription();

  uint c, i; // c == entry index, i == selectable index

  for(c = 0, i = 0; i != PageBegin; ++c)
    if(Entry[c]->Selectable)
      ++i;

  while(!Entry[c]->Selectable && Entry[c]->String.IsEmpty()) ++c;
  std::vector<festring> Chapter;

  for(;;)
  {
    Str.Empty();
    uint Marginal = Entry[c]->Marginal;

    if(Flags & SELECTABLE && Entry[c]->Selectable)
    {
      Str << char('A' + (i - PageBegin)) << ": ";
      Marginal += 3;
    }

    Str << Entry[c]->String;

    uint ChapterSize = festring::SplitString(Str, Chapter, Width, Marginal);
    for(uint l = 0; l < ChapterSize; ++l)
    {
      int attr = LIGHT_GRAY;
      if(Flags & SELECTABLE && Entry[c]->Selectable && Selected == i)
        attr = WHITE;

      graphics::MoveCursor(v2(Pos.X, Pos.Y + row));
      graphics::PutStr(Chapter[l].CStr(), attr);
      ++row;
    }

    if((i - PageBegin == PageLength - 1 && Entry[c]->Selectable)
       || c == Entry.size() - 1)
    {
      if((!(Flags & INVERSE_MODE) && c != Entry.size() - 1)
         || (Flags & INVERSE_MODE && PageBegin))
      {
        graphics::MoveCursor(v2(Pos.X, Pos.Y + row + 1));
        graphics::PutStr("- Press SPACE to continue, ESC to exit -", WHITE);
      }
      break;
    }

    if(Entry[c++]->Selectable)
      ++i;
  }

  return c == Entry.size() - 1;
}

void felist::DrawDescription() const
{
  for(uint c = 0; c < Description.size(); ++c)
  {
    graphics::MoveCursor(v2(Pos.X, Pos.Y + c));
    graphics::PutStrf(Description[c]->Color, "%s",
                      Description[c]->String.CStr());
  }
}

/* We suppose InverseMode != false here */

void felist::QuickDraw(uint X, uint Y, uint W, uint H) const
{
  v2 orig(X,Y);
  std::vector<festring> Chapter;

  uint Index = 0;
  orig.Y += (H - 1);
  uint LineSize = W;

  for(uint c1 = 0; c1 <= Selected; ++c1)
  {
    const felistentry* CurrentEntry = Entry[Selected - c1];
    uint ChapterSize = festring::SplitString(CurrentEntry->String,
                                             Chapter, LineSize,
                                             CurrentEntry->Marginal);

    for(uint c2 = 0; c2 < ChapterSize; ++c2)
    {
      graphics::MoveCursor(orig); orig.Y--;
      const char* msg = Chapter[ChapterSize - c2 - 1].CStr();
      int xpos = 0;
      while(*msg) {
        graphics::PutChar(*msg, WHITE); msg++;
        xpos++;
      }
      while(xpos++ < LineSize) graphics::PutChar(' ', WHITE);
      if(++Index == H) return;
    }
  }
}

void felist::Empty()
{
  for(uint c = 0; c < Entry.size(); ++c)
    delete Entry[c];

  Entry.clear();
}

void felist::AddEntry(const festring& Str, col16 Color,
		      uint Marginal, truth Selectable)
{
  Entry.push_back(new felistentry(Str, Color, Marginal, Selectable));

  if(Maximum && Entry.size() > ulong(Maximum))
  {
    delete Entry[0];
    Entry.erase(Entry.begin());
  }
}

void felist::Save(outputfile& SaveFile) const
{
  SaveFile << Entry << Maximum << Selected;
}

void felist::Load(inputfile& SaveFile)
{
  SaveFile >> Entry >> Maximum >> Selected;
}

void felist::PrintToFile(const festring& FileName)
{
  std::ofstream SaveFile(FileName.CStr(), std::ios::out);

  if(!SaveFile.is_open())
    return;

  uint c;

  for(c = 0; c < Description.size(); ++c)
    SaveFile << Description[c]->String.CStr() << std::endl;

  SaveFile << std::endl;

  for(c = 0; c < Entry.size(); ++c)
  {
    SaveFile << Entry[c]->String.CStr() << std::endl;
  }
}

void felist::EmptyDescription()
{ Description.resize(1); }
