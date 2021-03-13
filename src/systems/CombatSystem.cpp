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
        singleAttack.attackerRef = evt.attacker;
        singleAttack.defenderRef = evt.defender;
        singleAttack.attacker = &attacker;
        singleAttack.defender = &defender;
        singleAttack.weapon = atk.weapon;
    
        // Make independent attack and damage rolls for each attack instance
        auto attackRoll = attacker.makeMeleeAttackRoll(singleAttack, evt.attack, false);
    
//        Logging::log( "Attack roll: natural={}, modified={}, target={}\n", attackRoll.naturalRoll, attackRoll.modifiedRoll, attackRoll.targetValue );
    
        if ( attackRoll.isHit )
        {
            missedAll = false;
            
            auto damageInstance = attacker.getDamageForMeleeAttack(singleAttack, attackRoll);
            damage.instances.insert( damage.instances.end(), damageInstance.instances.begin(), damageInstance.instances.end() );
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