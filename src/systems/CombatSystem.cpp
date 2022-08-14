#include <systems/CombatSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <game/Attack.h>


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
    bool isCrit = false;
    bool isHit = false;
   
    int targetValue = 10; // TODO: Actually perform an AC calculation for the defender
    
    auto attackData = attacker->getModifiersAttackRoll( defender, mainWeapon, evt.attack );
    
    // Make independent attack and damage rolls for each attack instance
    auto critRangeData = attacker->getModifiersCritRange( defender, mainWeapon );
    int finalCritRange = critRangeData.mods.calculate( mainWeapon->critRange() );
    
    int naturalRoll = m_level->random()->diceRoll(20);
    int final = attackData.mods.calculate( naturalRoll );

    if ( naturalRoll >= finalCritRange )
    {
        isCrit = true;
        isHit = true;
    }
    else
    {
        // Not a crit - compare the roll against the target value
        if ( final >= targetValue )
        {
            // A hit!
            // Trigger: successful attack roll
            isHit = true;
        }
        else
        {
            // A miss
            // Trigger: unsuccessful attack roll
            isHit = false;
        }
    }

    Logger::get().info("Attack roll: natural={}, modified={}, target={}", naturalRoll, final, targetValue );

    Damage damage;


    if ( isHit )
    {
        missedAll = false;
        
        ActorCalc::DamageRoll damageRollCtx { defender };
        auto damageModifiers = attacker->getModifiersMeleeDamage(damageRollCtx, evt.attack);
        
        int damageRoll = m_level->random()->diceRoll( mainWeapon->damage() );
        int damageValue = damageModifiers.mods.calculate( damageRoll );

        // If this is a critical hit, modify the damage
        if ( isCrit )
        {
            damageValue *= mainWeapon->critMultiplier();
        }
        
        DamageInstance dmgInstance{ DamageType::Untyped, DamageSuperType::Physical, damageValue };
        damage.instances.push_back( dmgInstance );

        Logger::get().info( "Damage roll: natural={}, modified={}", damageRoll, damageValue );
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