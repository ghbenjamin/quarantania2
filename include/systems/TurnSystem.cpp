#include <systems/TurnSystem.h>
#include <game/Level.h>

TurnSystem::TurnSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::TurnChange>(this);
    m_level->events().subscribe<GameEvents::RoundChange>(this);
}

void TurnSystem::accept(GameEvents::TurnChange *evt)
{

}

void TurnSystem::accept(GameEvents::RoundChange *evt)
{

}
