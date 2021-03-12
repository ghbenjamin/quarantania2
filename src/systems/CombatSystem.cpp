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
    doMeleeAttack( evt.attacker, evt.defender );
}

void CombatSystem::doMeleeAttack( EntityRef attackerRef, EntityRef defenderRef )
{
    auto attacker = m_level->ecs().getComponents<ActorComponent>(attackerRef)->actor;
    auto defender = m_level->ecs().getComponents<ActorComponent>(defenderRef)->actor;

    auto const& weapon = attacker.getActiveWeapon();
    
    SingleAttackInstance singleAttack;
    singleAttack.attackerRef = attackerRef;
    singleAttack.defenderRef = defenderRef;
    singleAttack.attacker = &attacker;
    singleAttack.defender = &defender;
    singleAttack.weapon = &weapon;

    auto attackRoll = attacker.makeAttackRoll( singleAttack, false );

    if ( attackRoll.isHit )
    {
        // Trigger: damage dealt
        auto damage = attacker.getDamageForAttack( singleAttack, attackRoll );

        // Actually deal the damage
        m_level->events().broadcast<GameEvents::EntityDamage>( defenderRef, attackerRef, damage );
    }
    else
    {
        m_level->events().broadcast<GameEvents::CombatMissedAttack>( attackerRef, defenderRef );
    }
}