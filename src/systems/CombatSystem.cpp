#include <systems/CombatSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>

CombatSystem::CombatSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::MeleeAttack>( this );
}

void CombatSystem::accept(GEvents::MeleeAttack *evt)
{
    auto atkActor = m_level->getComponents<ActorComponent>(evt->attacker);
    auto defActor = m_level->getComponents<ActorComponent>(evt->defender);

    m_level->addTextLogMessage( fmt::format( "{} attacks {} with their {}!", evt->attacker, evt->defender, "BFG" ) );
}


