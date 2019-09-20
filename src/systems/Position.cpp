#include <systems/Position.h>
#include <game/Level.h>

Systems::Position::Position(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityReady>( this );
}

void Systems::Position::accept(GEvents::EntityMove *evt)
{
    m_level->getComponents<Components::TilePosition>(evt->ent)->position = evt->newPos;

    m_level->grid().removeEntFromTile( evt->oldPos, evt->ent );
    m_level->grid().addEntToTile( evt->newPos, evt->ent );
}

void Systems::Position::accept(GEvents::EntityReady *evt)
{
    if ( m_level->entityHas<Components::TilePosition>(evt->ent) )
    {
        auto const& pos = m_level->getComponents<Components::TilePosition>(evt->ent);
        m_level->grid().addEntToTile( pos->position, evt->ent );
    }
}