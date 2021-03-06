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

/* Compiled through trapset.cpp */

web::web()
{
  if(!game::IsLoading())
  {
    TrapData.TrapID = game::CreateNewTrapID(this);
    TrapData.VictimID = 0;
    packcol16 Color = MakeRGB16(250, 250, 250);
  }
}

web::~web()
{
  game::RemoveTrapID(TrapData.TrapID);
}

truth web::TryToUnStick(character* Victim, v2)
{
  ulong TrapID = GetTrapID();
  int Modifier = 7 * GetTrapBaseModifier()
		 / Max(Victim->GetAttribute(DEXTERITY)
		       + Victim->GetAttribute(ARM_STRENGTH), 1);

  if(!RAND_N(Max(Modifier, 2)))
  {
    Victim->RemoveTrap(TrapID);
    TrapData.VictimID = 0;

    if(Victim->IsPlayer())
      ADD_MESSAGE("You manage to free yourself from the web.");
    else if(Victim->CanBeSeenByPlayer())
      ADD_MESSAGE("%s manages to free %sself from the web.",
		  Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN);

    Victim->EditAP(-500);
    return true;
  }

  if(!RAND_N(Max(Modifier << 1, 2)))
  {
    Victim->RemoveTrap(TrapID);
    TrapData.VictimID = 0;
    GetLSquareUnder()->RemoveTrap(this);
    SendToHell();

    if(Victim->IsPlayer())
      ADD_MESSAGE("You tear the web down.");
    else if(Victim->CanBeSeenByPlayer())
      ADD_MESSAGE("%s tears the web down.", Victim->CHAR_NAME(DEFINITE));

    Victim->EditAP(-500);
    return true;
  }

  Modifier = GetTrapBaseModifier()
	     * (Victim->GetAttribute(DEXTERITY)
		+ Victim->GetAttribute(ARM_STRENGTH)) / 75;

  if(Victim->CanChokeOnWeb(this) && !RAND_N(Max(Modifier << 3, 2)))
  {
    if(Victim->IsPlayer())
      ADD_MESSAGE("You manage to choke yourself on the web.");
    else if(Victim->CanBeSeenByPlayer())
      ADD_MESSAGE("%s chokes %sself on the web.",
		  Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN);

    Victim->LoseConsciousness(250 + RAND_N(250));
    Victim->EditAP(-1000);
    return true;
  }

  if(!RAND_N(Max(Modifier, 2)))
  {
    int VictimBodyPart = Victim->GetRandomBodyPart(ALL_BODYPART_FLAGS
						   &~TrapData.BodyParts);

    if(VictimBodyPart != NONE_INDEX)
    {
      TrapData.BodyParts |= 1 << VictimBodyPart;
      Victim->AddTrap(GetTrapID(), 1 << VictimBodyPart);

      if(Victim->IsPlayer())
	ADD_MESSAGE("You fail to free yourself from the web "
		    "and your %s is stuck in it in the attempt.",
		    Victim->GetBodyPartName(VictimBodyPart).CStr());
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s tries to free %sself from the web "
		    "but is stuck more tightly in it in the attempt.",
		    Victim->CHAR_NAME(DEFINITE),
		    Victim->CHAR_OBJECT_PRONOUN);

      Victim->EditAP(-1000);
      return true;
    }
  }

  if(Victim->IsPlayer())
    ADD_MESSAGE("You are unable to escape from the web.");

  Victim->EditAP(-1000);
  return false;
}

void web::Save(outputfile& SaveFile) const
{
  trap::Save(SaveFile);
  SaveFile << TrapData << Strength;
}

void web::Load(inputfile& SaveFile)
{
  trap::Load(SaveFile);
  SaveFile >> TrapData >> Strength;
  game::AddTrapID(this, TrapData.TrapID);
}

void web::StepOnEffect(character* Stepper)
{
  if(Stepper->IsImmuneToStickiness())
    return;

  int StepperBodyPart = Stepper->GetRandomBodyPart();

  if(StepperBodyPart == NONE_INDEX)
    return;

  TrapData.VictimID = Stepper->GetID();
  TrapData.BodyParts = 1 << StepperBodyPart;
  Stepper->AddTrap(GetTrapID(), 1 << StepperBodyPart);

  if(Stepper->IsPlayer())
    ADD_MESSAGE("You try to step through the web "
		"but your %s sticks in it.",
		Stepper->GetBodyPartName(StepperBodyPart).CStr());
  else if(Stepper->CanBeSeenByPlayer())
    ADD_MESSAGE("%s gets stuck in the web.", Stepper->CHAR_NAME(DEFINITE));
}

void web::AddDescription(festring& Msg) const
{
  Msg << ". A web envelops the square";
}

void web::AddTrapName(festring& String, int) const
{
  String << "a spider web";
}

void web::Draw() const
{
  int Glyph = '^';
  int Attr = WHITE;
  graphics::PutChar(Glyph, Attr);
}

truth web::IsStuckToBodyPart(int I) const
{
  return 1 << I & TrapData.BodyParts;
}

void web::ReceiveDamage(character*, int, int Type, int)
{
  if(Type & (ACID|FIRE|ELECTRICITY|ENERGY))
    Destroy();
}

void web::Destroy()
{
  character* Char = game::SearchCharacter(GetVictimID());

  if(Char)
    Char->RemoveTrap(GetTrapID());

  TrapData.VictimID = 0;
  GetLSquareUnder()->RemoveTrap(this);
  SendToHell();
}

truth web::CanBeSeenBy(const character* Who) const
{
  return (GetLSquareUnder()->CanBeSeenBy(Who)
	  && Who->GetAttribute(WISDOM) > 4);
}

void web::PreProcessForBone()
{
  trap::PreProcessForBone();
  game::RemoveTrapID(TrapData.TrapID);
  TrapData.TrapID = 0;
}

void web::PostProcessForBone()
{
  trap::PostProcessForBone();
  TrapData.TrapID = game::CreateNewTrapID(this);
}
