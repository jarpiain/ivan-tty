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

#define __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "wterra.h"

SYSTEM_SPECIALIZATIONS(gwterrain) SYSTEM_SPECIALIZATIONS(owterrain)

#include "wterras.h"

#undef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__

const int OceanType = ocean::ProtoType.GetIndex();
const int SnowType = snow::ProtoType.GetIndex();
const int GlacierType = glacier::ProtoType.GetIndex();
const int EGForestType = evergreenforest::ProtoType.GetIndex();
const int LForestType = leafyforest::ProtoType.GetIndex();
const int SteppeType = steppe::ProtoType.GetIndex();
const int DesertType = desert::ProtoType.GetIndex();
const int JungleType = jungle::ProtoType.GetIndex();

#include <algorithm>

#include "allocate.h"
#include "char.h"
#include "cont.h"
#include "game.h"
#include "cont.h"
#include "femath.h"
#include "iconf.h"
#include "graphics.h"
#include "whandler.h"
#include "message.h"
#include "igraph.h"
#include "bitmap.h"
#include "save.h"

#include "cont.cpp"
#include "worldmap.cpp"
#include "wsquare.cpp"
#include "wterra.cpp"
#include "wterras.cpp"
