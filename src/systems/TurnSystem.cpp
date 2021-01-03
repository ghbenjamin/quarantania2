#include <systems/TurnSystem.h>
#include <game/Level.h>

TurnSystem::TurnSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::TurnChange>(this);
    m_level->events().subscribe<GameEvents::RoundChange>(this);
    m_level->events().subscribe<GameEvents::EntityAction>(this);
}

void TurnSystem::operator()(GameEvents::TurnChange& evt)
{
}

void TurnSystem::operator()(GameEvents::RoundChange& evt)
{
}

void TurnSystem::operator()(GameEvents::EntityAction& evt)
{
    // TODO Don't advance the turn for every action
    m_level->nextTurn();
}
