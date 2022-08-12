#include <game/ActorModFactory.h>
#include <game/ActorModifierDefs.h>
#include <game/ActionDefs.h>
#include <fmt/format.h>

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
    else if ( id == "feat.dodge" )
    {
        return featDodge();
    }
    else if ( id == "feat.debug" )
    {
        return featDebug();
    }
    

    AssertAlwaysMsg( fmt::format( "Unknown mod id: '{}'", id ) );
    return statusExhausted(-1);
}



ActorModGroup ActorModFactory::statusSickened(int roundRemove)
{
    ActorModGroup group { "status.sickened", "Sickened", roundRemove };
    
    group.addDynamicMod<ActorMods::ModSavingThrowStaticAll>(ActorCalculationType::AttackRoll, -2);
    group.addDynamicMod<ActorMods::ModAttackRollStatic>(ActorCalculationType::SavingThrow, -2);
    group.addDynamicMod<ActorMods::ModAbilityScoreStaticAll>(ActorCalculationType::AbilityScore, -2);

    return group;
}

ActorModGroup ActorModFactory::statusFatigued(int roundRemove)
{
    ActorModGroup group { "status.fatigued", "Fatigued", roundRemove };

    // NO RUNNING OR CHARGING
    
    group.addDynamicMod<ActorMods::ModAbilityScoreStatic>(ActorCalculationType::AbilityScore, AbilityScoreType::STR, -2);
    group.addDynamicMod<ActorMods::ModAbilityScoreStatic>(ActorCalculationType::AbilityScore, AbilityScoreType::DEX, -2);
    
    return group;
}


ActorModGroup ActorModFactory::statusExhausted(int roundRemove)
{
    ActorModGroup group { "status.exhausted", "Exhausted", roundRemove };
    
    group.addDynamicMod<ActorMods::ModMovementSpeedMultiplier>(ActorCalculationType::MovementSpeed, 0.5f);
    group.addDynamicMod<ActorMods::ModAbilityScoreStatic>(ActorCalculationType::AbilityScore, AbilityScoreType::STR, -6);
    group.addDynamicMod<ActorMods::ModAbilityScoreStatic>(ActorCalculationType::AbilityScore, AbilityScoreType::DEX, -6);

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

ActorModGroup ActorModFactory::featDodge()
{
    ActorModGroup group { "feat.dodge", "Dodge", -1 };
    group.addDynamicMod<ActorMods::ModACDodgeBonus>(ActorCalculationType::ArmourClass, 1);

    return group;
}

ActorModGroup ActorModFactory::featDebug()
{
    ActorModGroup group { "feat.debug", "Debug", -1 };
    
    return group;
}
