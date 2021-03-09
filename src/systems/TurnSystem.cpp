#include <systems/TurnSystem.h>
#include <game/Level.h>

TurnSystem::TurnSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::TurnChange>(this, GEventTiming::Before);
    m_level->events().subscribe<GameEvents::RoundChange>(this, GEventTiming::Before);
}

void TurnSystem::operator()(GameEvents::TurnChange& evt)
{
    m_level->switchTurn();

    if ( evt.isPlayerTurn )
    {
        // If it's the player turn after the switch, then we need to advance the round
        m_level->events().broadcast<GameEvents::RoundChange>();
    }
}

void TurnSystem::operator()(GameEvents::RoundChange& evt)
{
    m_level->advanceRound();
}