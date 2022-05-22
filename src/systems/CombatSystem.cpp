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
    Actor* attacker = &m_level->ecs().getComponents<ActorComponent>(evt.attacker)->actor;
    Actor* defender = &m_level->ecs().getComponents<ActorComponent>(evt.defender)->actor;

    auto [mainWeapon, offWeapon] = attacker->getEquippedWeapons();
    bool missedAll = true;
    
    Damage damage;
    

    SingleMeleeAttackInstance singleAttack { attacker,  m_level->random()->diceRoll(20), defender->getRef() };
    singleAttack.attacker = attacker;
    singleAttack.defender = defender;
    singleAttack.weapon = mainWeapon;

    // Make independent attack and damage rolls for each attack instance
    auto attackRoll = attacker->makeMeleeAttackRoll( singleAttack, evt.attack );

    Logger::get().info("Attack roll: natural={}, modified={}, target={}", attackRoll.baseValue(), attackRoll.calculate(), attackRoll.targetValue );

    if ( attackRoll.isHit )
    {
        missedAll = false;

        auto damageRoll = attacker->makeMeleeDamageRoll(singleAttack, evt.attack, attackRoll);

        DamageInstance dmgInstance{ DamageType::Untyped, DamageSuperType::Physical, damageRoll.modifiedRoll };
        damage.instances.push_back( dmgInstance );

        Logger::get().info( "Damage roll: natural={}, modified={}", damageRoll.naturalRoll, damageRoll.modifiedRoll );
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