#include <game/ActorModGroups.h>
#include <game/ActorModifierDefs.h>


ActorModGroup ActorModFactory::statusSickened(int roundRemove)
{
    ActorModGroup group { "status.sickened", "Sickened", roundRemove };
    
    group.addStatMod<ActorMods::ModSavingThrowStaticAll>(ActorModType::AttackRolls, -2);
    group.addStatMod<ActorMods::ModAttackRollStatic>(ActorModType::SavingThrows, -2);
    group.addStatMod<ActorMods::ModAbilityScoreStaticAll>(ActorModType::AbilityScores, -2);

    return group;
}

ActorModGroup ActorModFactory::statusFatigued(int roundRemove)
{
    ActorModGroup group { "status.fatigued", "Fatigued", roundRemove };

    // NO RUNNING OR CHARGING
    
    group.addStatMod<ActorMods::ModAbilityScoreStatic>(ActorModType::AbilityScores, AbilityScoreType::STR, -2);
    group.addStatMod<ActorMods::ModAbilityScoreStatic>(ActorModType::AbilityScores, AbilityScoreType::DEX, -2);

    return group;
}


ActorModGroup ActorModFactory::statusExhausted(int roundRemove)
{
    ActorModGroup group { "status.exhausted", "Exhausted", roundRemove };
    
    group.addStatMod<ActorMods::ModMovementSpeedMultiplier>(ActorModType::MovementSpeed, 0.5f);
    group.addStatMod<ActorMods::ModAbilityScoreStatic>(ActorModType::AbilityScores, AbilityScoreType::STR, -6);
    group.addStatMod<ActorMods::ModAbilityScoreStatic>(ActorModType::AbilityScores, AbilityScoreType::DEX, -6);

    return group;
}

ActorModGroup ActorModFactory::featPowerAttack()
{
    ActorModGroup group { "feat.power_attack", "Power Attack", -1 };
    
    // Add the power attack ability somehow
    
    return group;
}
