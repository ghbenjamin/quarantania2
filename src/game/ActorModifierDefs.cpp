#include <game/ActorModifierDefs.h>




ActorMods::ModSavingThrowStatic::ModSavingThrowStatic( SavingThrowType type, int modifier )
    : type(type), modifier(modifier) {}
        
void ActorMods::ModSavingThrowStatic::modify( SavingThrowRoll* roll )
{
    if (roll->type == type)
    {
        roll->modified += modifier;
    }
}


ActorMods::ModAttackRollStatic::ModAttackRollStatic( int modifier )
    : modifier(modifier) {}

void ActorMods::ModAttackRollStatic::modify( AttackRoll* roll )
{
    roll->modifiedRoll += modifier;
}


ActorMods::ModSavingThrowStaticAll::ModSavingThrowStaticAll( int modifier )
 : modifier(modifier) {}

void ActorMods::ModSavingThrowStaticAll::modify( SavingThrowRoll* roll )
{
    roll->modified += modifier;
}

ActorMods::ModAbilityScoreStatic::ModAbilityScoreStatic(AbilityScoreType type, int modifier)
 : type(type), modifier(modifier) {}

void ActorMods::ModAbilityScoreStatic::modify(AbilityScoreBonus *roll)
{
    if ( roll->type == type )
    {
        roll->modified += modifier;
    }
}

ActorMods::ModAbilityScoreStaticAll::ModAbilityScoreStaticAll(int modifier)
 : modifier(modifier) {}

void ActorMods::ModAbilityScoreStaticAll::modify(AbilityScoreBonus *roll)
{
    roll->modified += modifier;
}


