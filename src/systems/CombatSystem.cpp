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

void CombatSystem::acceptDamage(const Damage& dmg, EntityRef ref)
{
    auto actorC = m_level->ecs().getComponents<ActorComponent>(ref);
    auto& actor = actorC->actor;

    // Decrease our HP for each instance of damage supplied
    // TODO: Immunities, resistances

    for (auto const& instance : dmg.instances )
    {
        auto newNp = actor.getCurrentHp() - instance.total;
        actor.setCurrentHp( newNp );
    }

    // Handle falling unconsious and death

    if ( actor.getCurrentHp() < 0 )
    {
        if ( actorC->actorType == ActorType::PC &&
             actor.getCurrentHp() > -actor.abilityScores().getScore(AbilityScoreType::CON).getValue() )
        {
            // Unconscious! Allow PCs to fall unconsious but not NPCs
        }
        else
        {
            // The entity died
            m_level->events().broadcast<GameEvents::EntityDeath>(ref);
        }
    }
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

    auto attackRoll = makeAttackRoll( singleAttack, false );

    if ( attackRoll.isHit )
    {
        // Trigger: damage dealt
        auto damage = getDamageForAttack( singleAttack, attackRoll );

        // Actually deal the damage
        acceptDamage( damage, defenderRef );
    }
    else
    {
        // Trigger: missed attack
    }
}

AttackRollResult CombatSystem::makeAttackRoll( SingleAttackInstance& attack, bool isCritConfirm )
{
    AttackRollResult result;
    
    result.naturalRoll = m_level->random().diceRoll(20);
    result.targetValue = getAcForDefender(attack);
    int critRange = getCritRangeForAttack( attack );
    
    // TODO Modify the roll here
    result.modifiedRoll = result.naturalRoll;
    
    if ( result.naturalRoll >= critRange )
    {
        // Potential crit
        if ( !isCritConfirm )
        {
            auto confirmResult = makeAttackRoll( attack, true );
            if ( confirmResult.isHit )
            {
                // Confirmed crit - a hit and crit
                // Trigger: crit confirmed
                result.isCrit = true;
                result.isHit = true;
            }
        }
        else
        {
            // Crit confirmation failed, but still a guaranteed hit
            // Trigger: crit confirmation failed
            result.isHit = true;
            result.isCrit = false;
        }
    }
    else
    {
        // Not a crit - compare the roll against the target value
        if ( result.modifiedRoll >= result.targetValue )
        {
            // A hit!
            // Trigger: successfull attack roll
            result.isHit = true;
        }
        else
        {
            // A miss
            // Trigger: unsuccessful attack roll 
            result.isHit = false;
        }
    }
    
    return result;
}

int CombatSystem::getAcForDefender( SingleAttackInstance& attack )
{
    return attack.defender->getAC();
}

int CombatSystem::getCritRangeForAttack( SingleAttackInstance &attack )
{
    return attack.weapon->critRange();
}

Damage CombatSystem::getDamageForAttack( SingleAttackInstance& attack, AttackRollResult const& roll )
{
    // How much damage?
    int damageRoll = m_level->random().diceRoll( attack.weapon->damage() );

    // If this is a critical hit, modify the damage
    if ( roll.isCrit )
    {
        damageRoll *= attack.weapon->critMultiplier();
    }

    Damage damage;
    DamageInstance dmgInstance{ DamageType::Untyped, DamageSuperType::Physical, damageRoll };
    damage.instances.push_back( dmgInstance );

    return damage;
}
