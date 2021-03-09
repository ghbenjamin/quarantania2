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
    if ( !evt.isPlayerTurn )
    {
        // The turn has been switched to the AI! Somehow work out what we're going to do now.

        // Until we know, send the turn right back.
        m_level->events().broadcast<GameEvents::TurnChange>(true);
    }
}
