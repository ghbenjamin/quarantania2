
#include <systems/ObjectivesSystem.h>
#include <game/Level.h>

ObjectivesSystem::ObjectivesSystem( Level *parent ) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityDeath>(this);
}

void ObjectivesSystem::operator()( GameEvents::EntityDeath &evt )
{
    
}
