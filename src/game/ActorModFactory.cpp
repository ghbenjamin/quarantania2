#include <game/ActorModFactory.h>
#include <game/ActorModifierDefs.h>
#include <game/ActionDefs.h>
#include <fmt/format.h>


ActorModGroup ActorModFactory::statusSickened(int roundRemove)
{
    ActorModGroup group { "status.sickened", "Sickened", roundRemove };
    
    group.addStatMod<ActorMods::ModSavingThrowStaticAll>(ActorStatModType::AttackRolls, -2);
    group.addStatMod<ActorMods::ModAttackRollStatic>(ActorStatModType::SavingThrows, -2);
    group.addStatMod<ActorMods::ModAbilityScoreStaticAll>(ActorStatModType::AbilityScores, -2);

    return group;
}

ActorModGroup ActorModFactory::statusFatigued(int roundRemove)
{
    ActorModGroup group { "status.fatigued", "Fatigued", roundRemove };

    // NO RUNNING OR CHARGING
    
    group.addStatMod<ActorMods::ModAbilityScoreStatic>(ActorStatModType::AbilityScores, AbilityScoreType::STR, -2);
    group.addStatMod<ActorMods::ModAbilityScoreStatic>(ActorStatModType::AbilityScores, AbilityScoreType::DEX, -2);

    return group;
}


ActorModGroup ActorModFactory::statusExhausted(int roundRemove)
{
    ActorModGroup group { "status.exhausted", "Exhausted", roundRemove };
    
    group.addStatMod<ActorMods::ModMovementSpeedMultiplier>(ActorStatModType::MovementSpeed, 0.5f);
    group.addStatMod<ActorMods::ModAbilityScoreStatic>(ActorStatModType::AbilityScores, AbilityScoreType::STR, -6);
    group.addStatMod<ActorMods::ModAbilityScoreStatic>(ActorStatModType::AbilityScores, AbilityScoreType::DEX, -6);

    return group;
}

ActorModGroup ActorModFactory::featPowerAttack()
{
    ActorModGroup group { "feat.power_attack", "Power Attack", -1 };
    
    group.addActionMod( GameAction(
        "power-attack", TargetingType::SingleEntity,
        std::make_shared<ActionPowerAttack>())
    );
    
    return group;
}





ActorModGroup ActorModFactory::fromId( std::string const &id )
{
    return fromId( id, -1 );
}

ActorModGroup ActorModFactory::fromId( std::string const &id, int duration )
{
    if (id == "status.sickened")
    {
        return statusSickened( duration );
    }
    else if ( id == "status.fatigued" )
    {
        return statusFatigued( duration );
    }
    else if ( id == "status.exhausted" )
    {
        return statusExhausted( duration );
    }
    else if ( id == "feat.power_attack" )
    {
        return featPowerAttack();
    }
    
    AssertAlwaysMsg( fmt::format( "Unknown mod id: '{}'", id ) );
    return statusExhausted(-1);
}
