#include <systems/MessageSystem.h>
#include <game/Level.h>

MessageSystem::MessageSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::LevelReady>(this );
    m_level->events().subscribe<GameEvents::EntityMove>(this );
    m_level->events().subscribe<GameEvents::EntityOpenClose>(this );
}

void MessageSystem::accept(GameEvents::LevelReady *evt)
{

}

void MessageSystem::accept(GameEvents::EntityMove *evt)
{

}

void MessageSystem::accept(GameEvents::EntityOpenClose *evt)
{

}

