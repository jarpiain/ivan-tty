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

#include "igraph.h"
#include "felist.h"
#include "graphics.h"
#include "iconf.h"
#include "game.h"
#include "save.h"
#include "object.h"
#include "allocate.h"
#include "whandler.h"

rawbitmap* igraph::RawGraphic[RAW_TYPES];
bitmap* igraph::Graphic[GRAPHIC_TYPES];
bitmap* igraph::TileBuffer;
bitmap* igraph::FlagBuffer;
const char* igraph::RawGraphicFileName[] = { "Graphics/GLTerra.pcx", "Graphics/OLTerra.pcx", "Graphics/Item.pcx", "Graphics/Char.pcx", "Graphics/Humanoid.pcx", "Graphics/Effect.pcx", "Graphics/Cursor.pcx" };
const char* igraph::GraphicFileName[] = { "Graphics/WTerra.pcx", "Graphics/FOW.pcx", "Graphics/Symbol.pcx" };
tilemap igraph::TileMap;
uchar igraph::RollBuffer[256];
int** igraph::BodyBitmapValidityMap;
bitmap* igraph::SilhouetteCache[HUMANOID_BODYPARTS][CONDITION_COLORS][SILHOUETTE_TYPES];
rawbitmap* igraph::ColorizeBuffer[2] = { 0, 0 };
bitmap* igraph::Cursor[CURSOR_TYPES];
col16 igraph::CursorColor[CURSOR_TYPES] = { MakeRGB16(40, 40, 40),
					    MakeRGB16(255, 0, 0),
					    MakeRGB16(100, 100, 255),
					    MakeRGB16(200, 200, 0) };
bitmap* igraph::BackGround;
int igraph::CurrentColorType = -1;

void igraph::Init()
{
  static truth AlreadyInstalled = false;

  if(!AlreadyInstalled)
  {
    AlreadyInstalled = true;
    graphics::Init();
    graphics::BlitDBToScreen();
  }
}

void igraph::DeInit()
{
}

void igraph::DrawCursor(v2 Pos, int CursorData)
{

  bitmap* CursorBitmap = Cursor[CursorData&~(TARGET|FLASH)];

  if(!(CursorData & (TARGET|FLASH)))
  {
    return;
  }

  if(!(CursorData & TARGET))
  {
    return;
  }
}

tilemap::iterator igraph::AddUser(const graphicid& GI)
{
  tilemap::iterator Iterator = TileMap.find(GI);

  if(Iterator != TileMap.end())
  {
    tile& Tile = Iterator->second;
    ++Tile.Users;
    return Iterator;
  }
  else
  {
    const int SpecialFlags = GI.SpecialFlags;
    const int BodyPartFlags = SpecialFlags & 0x78;
    const int RotateFlags = SpecialFlags & 0x7;
    const int Frame = GI.Frame;
    v2 SparklePos = v2(GI.SparklePosX, GI.SparklePosY);
    rawbitmap* RawBitmap = RawGraphic[GI.FileIndex];
    v2 RawPos = v2(GI.BitmapPosX, GI.BitmapPosY);

    if(BodyPartFlags && BodyPartFlags < ST_OTHER_BODYPART)
    {
      EditBodyPartTile(RawBitmap, ColorizeBuffer[0], RawPos, BodyPartFlags);
      RawBitmap = ColorizeBuffer[0];
      RawPos.X = RawPos.Y = 0;

      if(RotateFlags)
      {
	SparklePos = RotateTile(RawBitmap, ColorizeBuffer[1], RawPos, SparklePos, RotateFlags);
	RawBitmap = ColorizeBuffer[1];
      }
    }
    else if(RotateFlags)
    {
      SparklePos = RotateTile(RawBitmap, ColorizeBuffer[0], RawPos, SparklePos, RotateFlags);
      RawBitmap = ColorizeBuffer[0];
      RawPos.X = RawPos.Y = 0;
    }

    bitmap* Bitmap = 0;

    const int WobbleData = GI.WobbleData;

    if(WobbleData & WOBBLE)
    {
      int Speed = (WobbleData & WOBBLE_SPEED_RANGE) >> WOBBLE_SPEED_SHIFT;
      int Freq = (WobbleData & WOBBLE_FREQ_RANGE) >> WOBBLE_FREQ_SHIFT;
      int WobbleMask = 7 >> Freq << (6 - Speed);
    }

    if(SpecialFlags & ST_FLAMES)
    {
      ulong SeedNFlags = SpecialFlags >> ST_FLAME_SHIFT & 3 | GI.Seed << 4;
    }

    return TileMap.insert(std::pair<graphicid, tile>(GI, tile(Bitmap))).first;
  }
}

void igraph::EditBodyPartTile(rawbitmap* Source, rawbitmap* Dest, v2 Pos, int BodyPartFlags)
{
}

v2 igraph::RotateTile(rawbitmap* Source, rawbitmap* Dest, v2 Pos, v2 SparklePos, int RotateFlags)
{
  if(SparklePos.X != SPARKLE_POS_X_ERROR)
  {
    if(RotateFlags & ROTATE)
    {
      const int T = SparklePos.X;
      SparklePos.X = 15 - SparklePos.Y;
      SparklePos.Y = T;
    }

    if(RotateFlags & MIRROR)
      SparklePos.X = 15 - SparklePos.X;

    if(RotateFlags & FLIP)
      SparklePos.Y = 15 - SparklePos.Y;
  }

  return SparklePos;
}

void igraph::RemoveUser(tilemap::iterator Iterator)
{
  tile& Tile = Iterator->second;

  if(!--Tile.Users)
  {
    delete Tile.Bitmap;
    TileMap.erase(Iterator);
  }
}

outputfile& operator<<(outputfile& SaveFile, const graphicid& Value)
{
  SaveFile.Write(reinterpret_cast<const char*>(&Value), sizeof(Value));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, graphicid& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

graphicdata::~graphicdata()
{
}

void graphicdata::Save(outputfile& SaveFile) const
{
}

void graphicdata::Load(inputfile& SaveFile)
{
}

outputfile& operator<<(outputfile& SaveFile, const graphicdata& Data)
{
  Data.Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, graphicdata& Data)
{
  Data.Load(SaveFile);
  return SaveFile;
}

void graphicdata::Retire()
{
}

const int* igraph::GetBodyBitmapValidityMap(int SpecialFlags)
{
  return BodyBitmapValidityMap[(SpecialFlags & 0x38) >> 3];
}

void igraph::CreateBodyBitmapValidityMaps()
{
  memset(BodyBitmapValidityMap[0], 0xFF, 128 * sizeof(int));
  int* Map = BodyBitmapValidityMap[ST_RIGHT_ARM >> 3];
  int x, y;

  for(x = 8; x < 16; ++x)
    Map[x] = 0;

  Map = BodyBitmapValidityMap[ST_LEFT_ARM >> 3];

  for(x = 0; x < 8; ++x)
    Map[x] = 0;

  Map = BodyBitmapValidityMap[ST_GROIN >> 3];
  memset(Map, 0, 16 * sizeof(int));

  for(y = 10; y < 13; ++y)
    for(x = y - 5; x < 20 - y; ++x)
      Map[x] |= 1 << y;

  Map = BodyBitmapValidityMap[ST_RIGHT_LEG >> 3];

  for(x = 8; x < 16; ++x)
    Map[x] = 0;

  Map[5] &= ~(1 << 10);
  Map[6] &= ~(1 << 10);
  Map[7] &= ~(1 << 10);
  Map[6] &= ~(1 << 11);
  Map[7] &= ~(1 << 11);
  Map[7] &= ~(1 << 12);

  Map = BodyBitmapValidityMap[ST_LEFT_LEG >> 3];

  for(x = 0; x < 8; ++x)
    Map[x] = 0;

  Map[8] &= ~(1 << 10);
  Map[9] &= ~(1 << 10);
  Map[8] &= ~(1 << 11);
}

void igraph::CreateSilhouetteCaches()
{
  int BodyPartSilhouetteMColorIndex[HUMANOID_BODYPARTS] = { 3, 0, 1, 2, 1, 2, 3 };
  col24 ConditionColor[CONDITION_COLORS] = { MakeRGB16(48, 48, 48),
					     MakeRGB16(120, 0, 0),
					     MakeRGB16(180, 0, 0),
					     MakeRGB16(180, 120, 120),
					     MakeRGB16(180, 180, 180) };
  v2 V(8, 64);

  for(int c1 = 0; c1 < HUMANOID_BODYPARTS; ++c1)
  {
    if(c1 == 4)
      V.X = 72;

    for(int c2 = 0; c2 < CONDITION_COLORS; ++c2)
    {
      packcol16 Color[4] = { 0, 0, 0, 0 };
      Color[BodyPartSilhouetteMColorIndex[c1]] = ConditionColor[c2];
    }
  }
}

void igraph::CreateBackGround(int ColorType)
{
}

col16 igraph::GetBackGroundColor(int Element)
{
  switch(CurrentColorType)
  {
   case GRAY_FRACTAL: return MakeRGB16(Element, Element, Element);
   case RED_FRACTAL: return MakeRGB16(Element + (Element >> 1), Element / 3, Element / 3);
   case GREEN_FRACTAL: return MakeRGB16(Element / 3, Element + (Element >> 2), Element / 3);
   case BLUE_FRACTAL: return MakeRGB16(Element / 3, Element / 3, Element + (Element >> 1));
   case YELLOW_FRACTAL: return MakeRGB16(Element + (Element >> 1), Element + (Element >> 1), Element / 3);
  }

  return 0;
}

void igraph::BlitBackGround(v2 Pos, v2 Border)
{
}
