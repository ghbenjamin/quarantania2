#include <systems/Position.h>
#include <game/Level.h>
#include <components/TilePosition.h>

Systems::Position::Position(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityReady>( this );
}

void Systems::Position::accept(GEvents::EntityMove *evt)
{
    // Make sure the current position of the moved entity is its new position
    m_level->getComponents<Components::TilePosition>(evt->ent)->position = evt->newPos;

    // Update our cache of which entites are in which tiles
    m_level->grid().removeEntFromTile( evt->oldPos, evt->ent );
    m_level->grid().addEntToTile( evt->newPos, evt->ent );
}

void Systems::Position::accept(GEvents::EntityReady *evt)
{
    // A new entity has been added - update our cache of which entites are in which tiles
    if ( m_level->entityHas<Components::TilePosition>(evt->ent) )
    {
        auto const& pos = m_level->getComponents<Components::TilePosition>(evt->ent);
        m_level->grid().addEntToTile( pos->position, evt->ent );
    }
}