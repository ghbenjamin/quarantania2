#include <systems/CombatSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <game/Damage.h>

CombatSystem::CombatSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::MeleeAttack>(this );
}

void CombatSystem::accept(GameEvents::MeleeAttack *evt)
{
    auto atkActor = m_level->ecs().getComponents<ActorComponent>(evt->attacker);
    auto defActor = m_level->ecs().getComponents<ActorComponent>(evt->defender);

    m_level->addTextLogMessage( fmt::format( "{} attacks {} with the {}!",
            m_level->getDescriptionForEnt(evt->attacker),
            m_level->getDescriptionForEnt(evt->defender),
            evt->weapon->name()
    ));

    // Work out the AC and attack bonuses of our defender and attacke r

    int attackBonus = atkActor->actor.getBAB() + atkActor->actor.getStr();
    int defenceBonus;

    if ( defActor->actor.isFlatFooted() )
    {
        defenceBonus = defActor->actor.getFlatAC();
    }
    else
    {
        defenceBonus = defActor->actor.getAC();
    }

    // Helper for performing a single attack roll
    auto doAttackRoll = [this, attackBonus, defenceBonus, &evt]() -> AttackRollResult {
        AttackRollResult arr;
        arr.roll = m_level->random().diceroll(1, 20);
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
                    total +=  m_level->random().diceroll(
                        evt->weapon->damage().diceCount,
                        evt->weapon->damage().diceSize
                    );
                }

                rawDamage = total;
                isCrit = true;
            }
        }

        // Not a crit, but is a hit
        if (!isCrit)
        {
            rawDamage = m_level->random().diceroll(
                evt->weapon->damage().diceCount,
                evt->weapon->damage().diceSize
            );
        }

        // TODO Do actual damaging

        Damage dmg;
        dmg.total = rawDamage;
        dmg.type = DamageType::Physical;
        dmg.subType = DamageSubType::Untyped;
        dmg.source = evt->attacker;

        m_level->events().broadcast<GameEvents::EntityDamage>(evt->defender, &dmg );
    }
    else
    {
        if ( attackRoll.isCritMiss )
        {
            // Critical miss
            // Maybe do something extra bad?
        }
    }
}
