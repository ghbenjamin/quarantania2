#include <systems/PositionSystem.h>
#include <game/Level.h>
#include <components/PositionComponent.h>

PositionSystem::PositionSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityMove>(this, GEventTiming::Before);
    m_level->events().subscribe<GameEvents::EntityReady>(this);
}

void PositionSystem::accept(GameEvents::EntityMove *evt)
{
    // Make sure the current position of the moved entity is its new position
    m_level->getComponents<PositionComponent>(evt->ent)->position = evt->newPos;

    // Update our cache of which entites are in which tiles
    m_level->grid().removeEntFromTile( evt->oldPos, evt->ent );
    m_level->grid().addEntToTile( evt->newPos, evt->ent );
}

void PositionSystem::accept(GameEvents::EntityReady *evt)
{
    // A new entity has been added - update our cache of which entities are in which tiles
    if ( m_level->entityHas<PositionComponent>(evt->ent) )
    {
        auto const& pos = m_level->getComponents<PositionComponent>(evt->ent);
        m_level->grid().addEntToTile( pos->position, evt->ent );
    }
}