#include <game/ActorModifierDefs.h>


ActorModGroup ActorModFactory::statusSickened(int roundRemove)
{
    ActorModGroup group { "Sickened", roundRemove };
    
    group.addMod<ActorMods::ModSavingThrowStaticAll>( ActorModType::AttackRolls, -2 );
    group.addMod<ActorMods::ModAttackRollStatic>( ActorModType::SavingThrows, -2 );
    
    return group;
}

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
