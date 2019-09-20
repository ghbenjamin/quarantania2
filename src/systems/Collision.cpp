#include <systems/Collision.h>
#include <game/Level.h>

Systems::Collision::Collision(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityReady>( this );
}

void Systems::Collision::accept(GEvents::EntityMove *evt)
{
    if ( m_level->entityHas<Components::Collider>(evt->ent) )
    {
        m_level->grid().pass().setDynamic( evt->newPos, evt->ent, Rules::Passibility::Impassable );
        m_level->grid().pass().removeDynamic(  evt->oldPos, evt->ent, Rules::Passibility::Impassable );
    }
}

void Systems::Collision::accept(GEvents::EntityReady *evt)
{
    if ( m_level->entityHas<Components::Collider, Components::TilePosition>(evt->ent) )
    {
        auto const& pos = m_level->getComponents<Components::TilePosition>(evt->ent);
        m_level->grid().pass().setDynamic( pos->position, evt->ent, Rules::Passibility::Impassable );
    }
}
