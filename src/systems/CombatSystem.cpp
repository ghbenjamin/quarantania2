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

    ActorCalc::AttackRoll attackRoll;
    attackRoll.actor = attacker;
    attackRoll.defender = defender;
    attackRoll.weapon = mainWeapon;
    
    // Make independent attack and damage rolls for each attack instance
    int critRange = attacker->getCritRangeForAttack( attackRoll );

    // Add STR mod to the attack roll (TODO: This should be modifible, e.g. Weapon Finesse)
    attackRoll.mods.addItem(ActorCalcOperation::Add, attacker->getAbilityScoreValue(AbilityScoreType::STR));

    // Apply any modifiers from the type of attack, e.g. reduce to hit from a Power Attack
    evt.attack->modifyAttackRoll( attackRoll );

    // Apply any modifiers from the actors, e.g. Weapon Focus feats or status affects
    attacker->applyAllModifiers( &attackRoll );
    
    int naturalRoll = m_level->random()->diceRoll(20);
    int final = attackRoll.mods.calculate( naturalRoll );

    if ( naturalRoll >= critRange )
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
        
        auto damageModifiers = attacker->getModifiersMeleeDamage(attackRoll, evt.attack);
        
        int damageRoll = m_level->random()->diceRoll( attackRoll.weapon->damage() );
        int damageValue = damageModifiers.calculate( damageRoll );

        // If this is a critical hit, modify the damage
        if ( isCrit )
        {
            damageValue *= attackRoll.weapon->critMultiplier();
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