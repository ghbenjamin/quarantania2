#include <game/ActorModifierDefs.h>




ActorMods::ModSavingThrowStatic::ModSavingThrowStatic( SavingThrowType type, int modifier )
    : type(type), modifier(modifier) {}
        
void ActorMods::ModSavingThrowStatic::modify( ActorCalc::SavingThrowRoll* roll )
{
    if (roll->type == type)
    {
        roll->mods.addItem(ActorCalcOperation::Add, modifier);
    }
}


ActorMods::ModAttackRollStatic::ModAttackRollStatic( int modifier )
    : modifier(modifier) {}

void ActorMods::ModAttackRollStatic::modify( ActorCalc::AttackRoll* roll )
{
    roll->mods.addItem(ActorCalcOperation::Add, modifier);
}


ActorMods::ModSavingThrowStaticAll::ModSavingThrowStaticAll( int modifier )
 : modifier(modifier) {}

void ActorMods::ModSavingThrowStaticAll::modify( ActorCalc::SavingThrowRoll* roll )
{
    roll->mods.addItem(ActorCalcOperation::Add, modifier);
}

ActorMods::ModAbilityScoreStatic::ModAbilityScoreStatic(AbilityScoreType type, int modifier)
 : type(type), modifier(modifier) {}

void ActorMods::ModAbilityScoreStatic::modify( ActorCalc::AbilityScoreBonus *roll )
{
    if ( roll->type == type )
    {
        roll->mods.addItem(ActorCalcOperation::Add, modifier);
    }
}

ActorMods::ModAbilityScoreStaticAll::ModAbilityScoreStaticAll(int modifier)
 : modifier(modifier) {}

void ActorMods::ModAbilityScoreStaticAll::modify( ActorCalc::AbilityScoreBonus *roll )
{
    roll->mods.addItem(ActorCalcOperation::Add, modifier);
}


ActorMods::ModMovementSpeedMultiplier::ModMovementSpeedMultiplier( float multiplier )
 : multiplier(multiplier) {}

void ActorMods::ModMovementSpeedMultiplier::modify( ActorCalc::MovementSpeedData *data )
{
    data->mods.addItem(ActorCalcOperation::Multiply, multiplier);
}

