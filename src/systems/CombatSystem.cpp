#include <systems/CombatSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <game/Damage.h>

CombatSystem::CombatSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::MeleeAttack>( this );
}

void CombatSystem::accept(GEvents::MeleeAttack *evt)
{
    auto atkActor = m_level->getComponents<ActorComponent>(evt->attacker);
    auto defActor = m_level->getComponents<ActorComponent>(evt->defender);

    m_level->addTextLogMessage( fmt::format( "{} attacks {} with the {}!",
            m_level->getDescriptionForEnt(evt->attacker),
            m_level->getDescriptionForEnt(evt->defender),
            evt->weapon->name()
    ));

    // Work out the AC and attack bonuses of our defender and attacke r

    int attackBonus = atkActor->getBAB() + atkActor->getStr();
    int defenceBonus;

    if ( defActor->isFlatFooted() )
    {
        defenceBonus = defActor->getFlatAC();
    }
    else
    {
        defenceBonus = defActor->getAC();
    }

    // Helper for performing a single attack roll
    auto doAttackRoll = [this, attackBonus, defenceBonus, &evt]() -> AttackRollResult {
        AttackRollResult arr;
        arr.roll = diceroll(1, 20, m_level->random() );
        arr.isCritHit = arr.roll >= evt->weapon->critData().lowerRange;
        arr.isCritMiss = arr.roll == 1;
        arr.isHit =  !arr.isCritMiss && ( (attackBonus + arr.roll >= defenceBonus) || arr.isCritHit );
        return arr;
    };

    // Perform the attack roll
    auto attackRoll = doAttackRoll();
    int rawDamage = -1;
    bool isCrit = false;

    if ( attackRoll.isHit )
    {
        if ( attackRoll.isCritHit )
        {
            // Natural crit - try to confirm the crit
            auto confirmRoll = doAttackRoll();
            if ( confirmRoll.isHit )
            {
                // Crit confirmed - calculate critical damage
                int total = 0;
                for ( int i = 0; i < evt->weapon->critData().multiplier; i++ )
                {
                    total += diceroll(
                            evt->weapon->damage().diceCount,
                            evt->weapon->damage().diceSize,
                            m_level->random()
                    );
                }

                rawDamage = total;
                isCrit = true;
            }
        }

        // Not a crit, but is a hit
        if (!isCrit)
        {
            rawDamage = diceroll(
                    evt->weapon->damage().diceCount,
                    evt->weapon->damage().diceSize,
                    m_level->random()
            );
        }

        // TODO Do actual damaging

        Damage dmg;
        dmg.total = rawDamage;
        dmg.type = DamageType::Physical;
        dmg.subType = DamageSubType::Untyped;
        dmg.source = evt->attacker;

        int finalDamage = defActor->acceptDamage( dmg );

        m_level->addTextLogMessage( fmt::format( "{}{} hits for {}",
                (isCrit ? "Critical hit! " : ""),
                m_level->getDescriptionForEnt(evt->attacker),
                finalDamage
        ));

    }
    else
    {
        if ( attackRoll.isCritMiss )
        {
            // Critical miss
            // Maybe do something extra bad?
        }

        m_level->addTextLogMessage( fmt::format( "{} misses",
            m_level->getDescriptionForEnt(evt->attacker)
        ));
    }
}
