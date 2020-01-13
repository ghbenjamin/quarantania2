#include <systems/Combat.h>
#include <game/Level.h>
#include <components/Actor.h>

Systems::Combat::Combat(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::MeleeAttack>( this );
}

void Systems::Combat::accept(GEvents::MeleeAttack *evt)
{
    auto atkActor = m_level->getComponents<Components::Actor>(evt->attacker);
    auto defActor = m_level->getComponents<Components::Actor>(evt->defender);

    m_level->addTextLogMessage( fmt::format( "{} attacks {} with their {}!", evt->attacker, evt->defender, "BFG" ) );
}


