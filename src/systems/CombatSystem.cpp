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
    
    int naturalRoll = m_level->random()->diceRoll(20);

    ActorCalc::AttackRoll result { attacker, defender->getRef() };
    result.defender = defender;
    result.weapon = mainWeapon;

    result.mods.addItem( ActorCalcOperation::Add, naturalRoll );

    // Make independent attack and damage rolls for each attack instance
    int critRange = attacker->getCritRangeForAttack( result );

    // Add STR mod to the attack roll (TODO: This should be modifible, e.g. Weapon Finesse)
    result.mods.addItem(ActorCalcOperation::Add, attacker->getModStr());

    // Apply any modifiers from the type of attack, e.g. reduce to hit from a Power Attack
    evt.attack->modifyAttackRoll( result );

    // Apply any modifiers from the actors, e.g. Weapon Focus feats or status affects
    attacker->applyAllModifiers( &result );

    int final = result.mods.calculate();

    if ( naturalRoll >= critRange )
    {
        result.isCrit = true;
        result.isHit = true;
    }
    else
    {
        // Not a crit - compare the roll against the target value
        if ( final >= result.targetValue )
        {
            // A hit!
            // Trigger: successful attack roll
            result.isHit = true;
        }
        else
        {
            // A miss
            // Trigger: unsuccessful attack roll
            result.isHit = false;
        }
    }

    Logger::get().info("Attack roll: natural={}, modified={}, target={}", naturalRoll, result.mods.calculate(), result.targetValue );

    Damage damage;


    if ( result.isHit )
    {
        missedAll = false;

        auto damageRoll = attacker->makeMeleeDamageRoll(result, evt.attack);

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