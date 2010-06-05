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

/* Compiled through materset.cpp */

materialprototype::materialprototype(const materialprototype* Base, materialspawner Spawner, materialcloner Cloner, const char* ClassID) : Base(Base), Spawner(Spawner), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<material>::Add(this); }

long material::GetRawPrice() const { return GetPriceModifier() * GetWeight() / 10000; }
truth material::CanBeDug(material* ShovelMaterial) const { return ShovelMaterial->GetStrengthValue() > GetStrengthValue(); }
long material::GetTotalExplosivePower() const { return long(double(Volume) * GetExplosivePower() / 1000000); }
const char* material::GetConsumeVerb() const { return "eating"; }

materialpredicate TrueMaterialPredicate = &material::True;

void material::AddName(festring& Name, truth Articled, truth Adjective) const
{
  if(Articled)
    if(GetNameFlags() & USE_AN)
      Name << "an ";
    else
      Name << "a ";

  Name << (Adjective ? GetAdjectiveStem() : GetNameStem());
}

festring material::GetName(truth Articled, truth Adjective) const
{
  static festring Name;
  Name.Empty();
  AddName(Name, Articled, Adjective);
  return Name;
}

material* material::TakeDipVolumeAway()
{
  if(Volume > 500)
  {
    EditVolume(-500);
    return SpawnMore(500);
  }
  else
    return MotherEntity->RemoveMaterial(this);
}

void material::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
  SaveFile << Volume;
  SaveFile << (ushort)GetConfig();
}

void material::Load(inputfile& SaveFile)
{
  SaveFile >> Volume;
  databasecreator<material>::InstallDataBase(this, ReadType<ushort>(SaveFile));
}

truth material::Effect(character* Eater, long Amount)
{
  /* Receivexxx should return truth! */

  Amount = Amount * GetEffectStrength() / 100;

  if(!Amount)
    return false;

  switch(GetEffect())
  {
   case EFFECT_POISON: Eater->BeginTemporaryState(POISONED, Amount); break;
   case EFFECT_DARKNESS: Eater->ReceiveDarkness(Amount); break;
   case EFFECT_OMMEL_URINE: Eater->ReceiveOmmelUrine(Amount); break;
   case EFFECT_PEPSI: Eater->ReceivePepsi(Amount); break;
   case EFFECT_KOBOLD_FLESH: Eater->ReceiveKoboldFlesh(Amount); break;
   case EFFECT_HEAL: Eater->ReceiveHeal(Amount); break;
   case EFFECT_LYCANTHROPY: Eater->BeginTemporaryState(LYCANTHROPY, Amount); break;
   case EFFECT_SCHOOL_FOOD: Eater->ReceiveSchoolFood(Amount); break;
   case EFFECT_ANTIDOTE: Eater->ReceiveAntidote(Amount); break;
   case EFFECT_CONFUSE: Eater->BeginTemporaryState(CONFUSED, Amount); break;
   case EFFECT_POLYMORPH: Eater->BeginTemporaryState(POLYMORPH, Amount); break;
   case EFFECT_ESP: Eater->BeginTemporaryState(ESP, Amount); break;
   case EFFECT_SKUNK_SMELL: Eater->BeginTemporaryState(POISONED, Amount); break;
   case EFFECT_MAGIC_MUSHROOM:
    {
      v2 Pos = GetMotherEntity()->GetSquareUnderEntity()->GetPos();
      Eater->ActivateRandomState(SRC_MAGIC_MUSHROOM, Amount, Volume % 250 + Pos.X + Pos.Y + 1);
      break;
    }
   case EFFECT_TRAIN_PERCEPTION: Eater->EditExperience(PERCEPTION, Amount, 1 << 14); break;
   case EFFECT_HOLY_BANANA: Eater->ReceiveHolyBanana(Amount); break;
   case EFFECT_EVIL_WONDER_STAFF_VAPOUR:
    {
      v2 Pos = GetMotherEntity()->GetSquareUnderEntity()->GetPos();
      Eater->ActivateRandomState(SRC_EVIL, Amount, Volume % 250 + Pos.X + Pos.Y + 1);
      break;
    }
   case EFFECT_GOOD_WONDER_STAFF_VAPOUR:
    {
      v2 Pos = GetMotherEntity()->GetSquareUnderEntity()->GetPos();
      Eater->ActivateRandomState(SRC_GOOD, Amount, Volume % 250 + Pos.X + Pos.Y + 1);
      break;
    }
   case EFFECT_PEA_SOUP: Eater->ReceivePeaSoup(Amount); break;
   case EFFECT_BLACK_UNICORN_FLESH: Eater->ReceiveBlackUnicorn(Amount); break;
   case EFFECT_GRAY_UNICORN_FLESH: Eater->ReceiveGrayUnicorn(Amount); break;
   case EFFECT_WHITE_UNICORN_FLESH: Eater->ReceiveWhiteUnicorn(Amount); break;
   case EFFECT_TELEPORT_CONTROL: Eater->BeginTemporaryState(TELEPORT_CONTROL, Amount); break;
   case EFFECT_MUSHROOM:
    {
      v2 Pos = GetMotherEntity()->GetSquareUnderEntity()->GetPos();
      Eater->ActivateRandomState(SRC_MUSHROOM, Amount, Volume % 250 + Pos.X + Pos.Y + 1);
      break;
    }
   case EFFECT_OMMEL_CERUMEN: Eater->ReceiveOmmelCerumen(Amount); break;
   case EFFECT_OMMEL_SWEAT: Eater->ReceiveOmmelSweat(Amount); break;
   case EFFECT_OMMEL_TEARS: Eater->ReceiveOmmelTears(Amount); break;
   case EFFECT_OMMEL_SNOT: Eater->ReceiveOmmelSnot(Amount); break;
   case EFFECT_OMMEL_BONE: Eater->ReceiveOmmelBone(Amount); break;
   default: return false;
  }

  return true;
}

material* material::EatEffect(character* Eater, long Amount)
{
  Amount = Volume > Amount ? Amount : Volume;
  Eater->ReceiveNutrition(GetNutritionValue() * Amount / 50);

  if(GetInteractionFlags() & AFFECT_INSIDE)
  {
    head* Head = Eater->GetVirtualHead();
    long NewAmount = Amount;

    if(Head && Amount >= 8)
    {
      Head->AddFluid(static_cast<liquid*>(SpawnMore(Amount >> 3)), CONST_S("throat"), 0, true);
      NewAmount -= Amount >> 3;
    }

    Eater->GetTorso()->AddFluid(static_cast<liquid*>(SpawnMore(NewAmount)), CONST_S("stomach"), 0, true);
  }
  else
  {
    Effect(Eater, Amount);

    if(IsLiquid())
      Eater->EditStamina(int(50. * Amount * Eater->GetMaxStamina() / Eater->GetBodyVolume()), false);
  }

  if(Volume != Amount)
  {
    EditVolume(-Amount);
    return 0;
  }
  else
    return MotherEntity->RemoveMaterial(this);
}

truth material::HitEffect(character* Enemy, bodypart* BodyPart)
{
  if(!Volume)
    return false;

  switch(GetHitMessage())
  {
   case HM_SCHOOL_FOOD: Enemy->AddSchoolFoodHitMessage(); break;
   case HM_FROG_FLESH: Enemy->AddFrogFleshConsumeEndMessage(); break;
   case HM_OMMEL: Enemy->AddOmmelConsumeEndMessage(); break;
   case HM_PEPSI: Enemy->AddPepsiConsumeEndMessage(); break;
   case HM_KOBOLD_FLESH: Enemy->AddKoboldFleshHitMessage(); break;
   case HM_HEALING_LIQUID: Enemy->AddHealingLiquidConsumeEndMessage(); break;
   case HM_ANTIDOTE: Enemy->AddAntidoteConsumeEndMessage(); break;
   case HM_CONFUSE: Enemy->AddConfuseHitMessage(); break;
   case HM_HOLY_BANANA: Enemy->AddHolyBananaConsumeEndMessage(); break;
  }

  long Amount = Max<long>(GetVolume() >> 1, 1);
  truth Success;

  if(GetInteractionFlags() & AFFECT_INSIDE)
  {
    if(BodyPart)
    {
      BodyPart->AddFluid(static_cast<liquid*>(SpawnMore(Amount)), CONST_S(""), 0, true);
      Success = true;
    }
    else
      Success = false;
  }
  else
    Success = Effect(Enemy, Amount);

  if(Amount != Volume)
    EditVolume(-Amount);
  else
    delete MotherEntity->RemoveMaterial(this);

  return Success;
}

void material::AddConsumeEndMessage(character* Eater) const
{
  switch(GetConsumeEndMessage())
  {
   case CEM_SCHOOL_FOOD: Eater->AddSchoolFoodConsumeEndMessage(); break;
   case CEM_BONE: Eater->AddBoneConsumeEndMessage(); break;
   case CEM_FROG_FLESH: Eater->AddFrogFleshConsumeEndMessage(); break;
   case CEM_OMMEL: Eater->AddOmmelConsumeEndMessage(); break;
   case CEM_PEPSI: Eater->AddPepsiConsumeEndMessage(); break;
   case CEM_KOBOLD_FLESH: Eater->AddKoboldFleshConsumeEndMessage(); break;
   case CEM_HEALING_LIQUID: Eater->AddHealingLiquidConsumeEndMessage(); break;
   case CEM_ANTIDOTE: Eater->AddAntidoteConsumeEndMessage(); break;
   case CEM_ESP: Eater->AddESPConsumeMessage(); break;
   case CEM_HOLY_BANANA: Eater->AddHolyBananaConsumeEndMessage(); break;
   case CEM_PEA_SOUP: Eater->AddPeaSoupConsumeEndMessage(); break;
   case CEM_BLACK_UNICORN_FLESH: Eater->AddBlackUnicornConsumeEndMessage(); break;
   case CEM_GRAY_UNICORN_FLESH: Eater->AddGrayUnicornConsumeEndMessage(); break;
   case CEM_WHITE_UNICORN_FLESH: Eater->AddWhiteUnicornConsumeEndMessage(); break;
   case CEM_OMMEL_BONE: Eater->AddOmmelBoneConsumeEndMessage(); break;
  }
}

material* materialprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  material* Material = Spawner(0, 0, true);
  Material->Load(SaveFile);
  return Material;
}

material* material::MakeMaterial(int Config, long Volume)
{
  if(!Config)
    return 0;

  switch(Config >> 12)
  {
   case SOLID_ID >> 12: return solid::Spawn(Config, Volume);
   case ORGANIC_ID >> 12: return organic::Spawn(Config, Volume);
   case GAS_ID >> 12: return gas::Spawn(Config, Volume);
   case LIQUID_ID >> 12: return liquid::Spawn(Config, Volume);
   case FLESH_ID >> 12: return flesh::Spawn(Config, Volume);
   case POWDER_ID >> 12: return powder::Spawn(Config, Volume);
   case IRON_ALLOY_ID >> 12: return ironalloy::Spawn(Config, Volume);
  }

  ABORT("Odd material configuration number %d of volume %ld requested!", Config, Volume);
  return 0;
}

void material::SetVolume(long What)
{
  Volume = What;

  if(MotherEntity)
    MotherEntity->SignalVolumeAndWeightChange();
}

void material::Initialize(int NewConfig, long InitVolume, truth Load)
{
  if(!Load)
  {
    databasecreator<material>::InstallDataBase(this, NewConfig);
    Volume = InitVolume;
    PostConstruct();
  }
}

long material::GetTotalNutritionValue() const
{
  return GetNutritionValue() * GetVolume() / 50;
}

truth material::CanBeEatenByAI(const character* Eater) const
{
  return Eater->GetAttribute(WISDOM) < GetConsumeWisdomLimit()
    && !GetSpoilLevel() && !Eater->CheckCannibalism(this);
}

truth material::BreatheEffect(character* Enemy)
{
  return Effect(Enemy, Max<long>(GetVolume() / 10, 50));
}

const materialdatabase* material::GetDataBase(int Config)
{
  const prototype* Proto = 0;

  switch(Config >> 12)
  {
   case SOLID_ID >> 12: Proto = &solid::ProtoType; break;
   case ORGANIC_ID >> 12: Proto = &organic::ProtoType; break;
   case GAS_ID >> 12: Proto = &gas::ProtoType; break;
   case LIQUID_ID >> 12: Proto = &liquid::ProtoType; break;
   case FLESH_ID >> 12: Proto = &flesh::ProtoType; break;
   case POWDER_ID >> 12: Proto = &powder::ProtoType; break;
   case IRON_ALLOY_ID >> 12: Proto = &ironalloy::ProtoType; break;
  }

  const database* DataBase;
  databasecreator<material>::FindDataBase(DataBase, Proto, Config);

  if(DataBase)
    return DataBase;

  ABORT("Odd material configuration number %d requested!", Config);
  return 0;
}

void material::FinishConsuming(character* Consumer)
{
  if(!Consumer->IsPlayer() && GetConsumeWisdomLimit() != NO_LIMIT)
    Consumer->EditExperience(WISDOM, 150, 1 << 13); /** C **/

  AddConsumeEndMessage(Consumer);
}

void materialdatabase::InitDefaults(const materialprototype* NewProtoType, int NewConfig)
{
  ProtoType = NewProtoType;
  DigProductMaterial = Config = NewConfig;
  CommonFlags |= IS_ABSTRACT; // dummy value for configcontainer
}

item* material::CreateNaturalForm(int Config, long Volume)
{
  item* Item = GetDataBase(Config)->NaturalForm.Instantiate(NO_MATERIALS|NO_PIC_UPDATE);
  Item->InitMaterials(MAKE_MATERIAL(Config, Volume));
  return Item;
}

item* material::CreateNaturalForm(long Volume) const
{
  item* Item = GetNaturalForm().Instantiate(NO_MATERIALS|NO_PIC_UPDATE);
  Item->InitMaterials(SpawnMore(Volume));
  return Item;
}

int material::GetHardenedMaterial(const item* Item) const
{
  const materialdatabase* DB = DataBase;

  if(!Item->FlexibilityIsEssential())
    return DB->HardenedMaterial;

  while(DB->HardenedMaterial != NONE)
  {
    DB = material::GetDataBase(DB->HardenedMaterial);

    if(DataBase->Flexibility <= DB->Flexibility)
      return DB->Config;
  }

  return DB->HardenedMaterial;
}

int material::GetHardenModifier(const item* Item) const
{
  int M = GetFlexibility() << 2;

  if(!Item || !Item->FlexibilityIsEssential())
    M += GetStrengthValue();

  return M;
}

truth material::IsExplosive() const
{
  return DataBase->InteractionFlags & CAN_EXPLODE;
}

truth material::IsSparkling() const
{
  return DataBase->CategoryFlags & IS_SPARKLING;
}

truth material::IsStuckTo(const character* Char) const
{
  return MotherEntity->IsStuckTo(Char);
}
