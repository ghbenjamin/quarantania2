#include <systems/Combat.h>
#include <game/Level.h>

Systems::Combat::Combat(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::MeleeAttack>( this );
}

void Systems::Combat::accept(GEvents::MeleeAttack *evt)
{
    Logging::log( "Attack!" );
}


