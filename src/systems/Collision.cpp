#include <systems/Collision.h>
#include <components/Collider.h>
#include <components/TilePosition.h>
#include <game/Level.h>
#include <components/Openable.h>

Systems::Collision::Collision(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityReady>( this );
    m_level->events().subscribe<GEvents::EntityOpenClose>( this );
}

void Systems::Collision::accept(GEvents::EntityMove *evt)
{
    // An entity has moved - if it had a collider component, regenerate our information on
    // which tiles are passable and which are not.
    if ( m_level->entityHas<Components::Collider>(evt->ent) )
    {
        auto const& collider = m_level->getComponents<Components::Collider>(evt->ent);

        if ( collider->blocksMovement )
        {
            m_level->grid().pass().setDynamic( evt->newPos, evt->ent, Rules::Passibility::Impassable );
            m_level->grid().pass().removeDynamic(  evt->oldPos, evt->ent, Rules::Passibility::Impassable );
        }
    }
}

void Systems::Collision::accept(GEvents::EntityReady *evt)
{
    // A new entity has just finished being created - if it has a collider component, regenerate our
    // collision information.
    if ( m_level->entityHas<Components::Collider, Components::TilePosition>(evt->ent) )
    {
        auto const& [tilePos, collider] = m_level->getComponents<Components::TilePosition, Components::Collider>(evt->ent);

        if ( collider->blocksMovement )
        {
            m_level->grid().pass().setDynamic( tilePos->position, evt->ent, Rules::Passibility::Impassable );
        }
    }
}

void Systems::Collision::accept(GEvents::EntityOpenClose *evt)
{
    Logging::log("Open event woooo");

    auto [openable, collider, position] = m_level->getComponents<
            Components::Openable,
            Components::Collider,
            Components::TilePosition>(evt->ent);

    openable->isOpen = evt->isOpen;
    if ( evt->isOpen )
    {
        // This is an 'open' event

        collider->blocksLight = false;

        if ( !openable->lightOnly )
        {
            collider->blocksMovement = false;
            m_level->grid().pass().removeDynamic(position->position, evt->ent, Rules::Passibility::Impassable);
        }
    }
    else
    {
        // This is a 'close' event

        collider->blocksLight = true;

        if ( !openable->lightOnly )
        {
            collider->blocksMovement = true;
            m_level->grid().pass().setDynamic(position->position, evt->ent, Rules::Passibility::Impassable);
        }
    }
}
