#include <game/ActorModGroups.h>
#include <game/ActorModifierDefs.h>


ActorModGroup ActorModFactory::statusSickened(int roundRemove)
{
    ActorModGroup group { "Sickened", roundRemove };

    group.addMod<ActorMods::ModSavingThrowStaticAll>( ActorModType::AttackRolls, -2 );
    group.addMod<ActorMods::ModAttackRollStatic>( ActorModType::SavingThrows, -2 );
    group.addMod<ActorMods::ModAbilityScoreStaticAll>( ActorModType::AbilityScores, -2 );

    return group;
}

ActorModGroup ActorModFactory::statusFatigued(int roundRemove)
{
    ActorModGroup group { "Fatigued", roundRemove };

    // NO RUNNING OR CHARGING

    group.addMod<ActorMods::ModAbilityScoreStatic>( ActorModType::AbilityScores, AbilityScoreType::STR, -2 );
    group.addMod<ActorMods::ModAbilityScoreStatic>( ActorModType::AbilityScores, AbilityScoreType::DEX, -2 );

    return group;
}


ActorModGroup ActorModFactory::statusExhausted(int roundRemove)
{
    ActorModGroup group { "Exhausted", roundRemove };



    group.addMod<ActorMods::ModAbilityScoreStatic>( ActorModType::AbilityScores, AbilityScoreType::STR, -6 );
    group.addMod<ActorMods::ModAbilityScoreStatic>( ActorModType::AbilityScores, AbilityScoreType::DEX, -6 );

    return group;
}
