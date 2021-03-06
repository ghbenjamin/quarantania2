#include <systems/PositionSystem.h>
#include <game/Level.h>
#include <components/PositionComponent.h>
#include <utils/GlobalConfig.h>

PositionSystem::PositionSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityMove>(this, GEventTiming::Before);
    m_level->events().subscribe<GameEvents::EntityReady>(this);
}

void PositionSystem::operator()(GameEvents::EntityMove& evt)
{
    // Make sure the current position of the moved entity is its new position
    auto posC = m_level->ecs().getComponents<PositionComponent>(evt.ent);
    posC->tilePosition = evt.newPos;
    posC->pixelPosition = evt.newPos * GlobalConfig::TileSizePx;

    // Update our cache of which entites are in which tiles
    m_level->grid().removeEntFromTile( evt.oldPos, evt.ent );
    m_level->grid().addEntToTile( evt.newPos, evt.ent );
}

void PositionSystem::operator()(GameEvents::EntityReady& evt)
{
    // A new entity has been added - update our cache of which entities are in which tiles
    if ( m_level->ecs().entityHas<PositionComponent>(evt.ent) )
    {
        auto const& pos = m_level->ecs().getComponents<PositionComponent>(evt.ent);
        m_level->grid().addEntToTile( pos->tilePosition, evt.ent );
    }
}