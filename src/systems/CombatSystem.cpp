#include <systems/CombatSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <game/Damage.h>

CombatSystem::CombatSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::CombatMeleeAttack>(this, GEventTiming::Before);
}

void CombatSystem::accept(GameEvents::CombatMeleeAttack *evt)
{
    auto actorAttacker = m_level->ecs().getComponents<ActorComponent>(evt->attacker);

}

void CombatSystem::acceptDamage(const Damage& dmg, EntityRef ref)
{
    auto& actor = m_level->ecs().getComponents<ActorComponent>(ref)->actor;

    for (auto const& instance : dmg.instances )
    {
        auto newNp = actor.getCurrentHp() - instance.total;
        actor.setCurrentHp( newNp );
    }


    if ( actor.getCurrentHp() < 0 )
    {
        if ( actor.getCurrentHp() > -actor.abilityScores().getScore(AbilityScoreType::CON).getValue() )
        {
            // Unconscious!
        }
        else
        {
            // Dead! :(
            m_level->events().broadcast<GameEvents::EntityDeath>(ref);
        }
    }
}