#include <game/ActorModifierDefs.h>




ActorMods::ModSavingThrowStatic::ModSavingThrowStatic( SavingThrowType type, int modifier )
    : type(type), modifier(modifier) {}
        
void ActorMods::ModSavingThrowStatic::modify( SavingThrowRoll* roll )
{
    if (roll->type == type)
    {
        roll->modList().addItem(ActorCalcOperation::Add, modifier);
    }
}


ActorMods::ModAttackRollStatic::ModAttackRollStatic( int modifier )
    : modifier(modifier) {}

void ActorMods::ModAttackRollStatic::modify( AttackRoll* roll )
{
    roll->modList().addItem(ActorCalcOperation::Add, modifier);
}


ActorMods::ModSavingThrowStaticAll::ModSavingThrowStaticAll( int modifier )
 : modifier(modifier) {}

void ActorMods::ModSavingThrowStaticAll::modify( SavingThrowRoll* roll )
{
    roll->modList().addItem(ActorCalcOperation::Add, modifier);
}

ActorMods::ModAbilityScoreStatic::ModAbilityScoreStatic(AbilityScoreType type, int modifier)
 : type(type), modifier(modifier) {}

void ActorMods::ModAbilityScoreStatic::modify(AbilityScoreBonus *roll)
{
    if ( roll->type == type )
    {
        roll->modList().addItem(ActorCalcOperation::Add, modifier);
    }
}

ActorMods::ModAbilityScoreStaticAll::ModAbilityScoreStaticAll(int modifier)
 : modifier(modifier) {}

void ActorMods::ModAbilityScoreStaticAll::modify(AbilityScoreBonus *roll)
{
    roll->modList().addItem(ActorCalcOperation::Add, modifier);
}


ActorMods::ModMovementSpeedMultiplier::ModMovementSpeedMultiplier( float multiplier )
 : multiplier(multiplier) {}

void ActorMods::ModMovementSpeedMultiplier::modify( MovementSpeedData *data )
{
    data->modList().addItem(ActorCalcOperation::Multiply, multiplier);
}

ActorMods::ModACDodgeBonus::ModACDodgeBonus(int bonus)
 : bonus(bonus) {}

void ActorMods::ModACDodgeBonus::modify(ArmourClassData *data)
{
    data->dodgeBonus += bonus;
}


