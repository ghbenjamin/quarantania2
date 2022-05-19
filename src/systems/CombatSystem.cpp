#include <systems/CombatSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>


CombatSystem::CombatSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::CombatMeleeAttack>(this, GEventTiming::Before);
}

void CombatSystem::operator()(GameEvents::CombatMeleeAttack& evt)
{
    auto attacker = m_level->ecs().getComponents<ActorComponent>(evt.attacker)->actor;
    auto defender = m_level->ecs().getComponents<ActorComponent>(evt.defender)->actor;
    
    auto attackList = attacker.getAttackCountForMeleeAttack( evt.attack );
    bool missedAll = true;
    
    Damage damage;
    
    // Iterate over each of the attack instances this attack contains (normally 1-3 attacks)
    for ( auto const& atk : attackList.attacks )
    {
        SingleMeleeAttackInstance singleAttack;
        singleAttack.ctx.source = evt.attacker;
        singleAttack.ctx.target = evt.defender;
        singleAttack.attacker = &attacker;
        singleAttack.defender = &defender;
        singleAttack.weapon = atk.weapon;
    
        // Make independent attack and damage rolls for each attack instance
        auto attackRoll = attacker.makeMeleeAttackRoll( singleAttack, evt.attack );
    
        Logger::get().info( "Attack roll: natural={}, modified={}, target={}", attackRoll.ctx.naturalRoll, attackRoll.ctx.finalRoll, attackRoll.targetValue );
    
        if ( attackRoll.isHit )
        {
            missedAll = false;
            
            auto damageRoll = attacker.makeMeleeDamageRoll(singleAttack, evt.attack, attackRoll);
    
            DamageInstance dmgInstance{ DamageType::Untyped, DamageSuperType::Physical, damageRoll.modifiedRoll };
            damage.instances.push_back( dmgInstance );
    
            Logger::get().info( "Damage roll: natural={}, modified={}", damageRoll.naturalRoll, damageRoll.modifiedRoll );
        }
    }
    
    // If we hit at least once, consider it a successful attack and deal the appropriate damage
    
    if (missedAll)
    {
        m_level->events().broadcast<GameEvents::CombatMissedAttack>( evt.attacker, evt.defender );
    }
    else
    {
        m_level->events().broadcast<GameEvents::CombatAttackSucceeded>( evt.attacker, evt.defender );
    
        // Actually deal the damage
        m_level->events().broadcast<GameEvents::EntityDamage>( evt.defender, evt.attacker, damage );
    }
}