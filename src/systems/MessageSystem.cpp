#include <systems/MessageSystem.h>
#include <game/Level.h>

MessageSystem::MessageSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GEvents::LevelReady>( this );
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityOpenClose>( this );
}

void MessageSystem::accept(GEvents::LevelReady *evt)
{

}

void MessageSystem::accept(GEvents::EntityMove *evt)
{

}

void MessageSystem::accept(GEvents::EntityOpenClose *evt)
{

}

