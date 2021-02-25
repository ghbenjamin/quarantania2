#include <systems/AISystem.h>
#include <game/Level.h>
#include <components/AIComponent.h>

AISystem::AISystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::TurnChange>(this);
}

void AISystem::operator()(GameEvents::TurnChange& evt)
{
    if ( m_level->ecs().entityHas<AIComponent>(evt.current) )
    {
        // This is the turn of an AI - for now just skip it
    }
}
