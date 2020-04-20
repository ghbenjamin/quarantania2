#include <systems/CollisionSystem.h>
#include <components/ColliderComponent.h>
#include <components/PositionComponent.h>
#include <game/Level.h>
#include <components/OpenableComponent.h>

CollisionSystem::CollisionSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityReady>( this );
    m_level->events().subscribe<GEvents::EntityOpenClose>( this );
    m_level->events().subscribe<GEvents::EntityDeath>( this );
}

void CollisionSystem::accept(GEvents::EntityMove *evt)
{
    // An entity has moved - if it had a collider component, regenerate our information on
    // which tiles are passable and which are not.
    if ( m_level->entityHas<ColliderComponent>(evt->ent) )
    {
        auto const& collider = m_level->getComponents<ColliderComponent>(evt->ent);

        if ( collider->blocksMovement )
        {
            m_level->grid().pass().setDynamic( evt->newPos, evt->ent, Passibility::Impassable );
            m_level->grid().pass().removeDynamic(  evt->oldPos, evt->ent, Passibility::Impassable );
        }
    }
}

void CollisionSystem::accept(GEvents::EntityReady *evt)
{
    // A new entity has just finished being created - if it has a collider component, regenerate our
    // collision information.
    if ( m_level->entityHas<ColliderComponent, PositionComponent>(evt->ent) )
    {
        auto const& [tilePos, collider] = m_level->getComponents<PositionComponent, ColliderComponent>(evt->ent);

        if ( collider->blocksMovement )
        {
            m_level->grid().pass().setDynamic( tilePos->position, evt->ent, Passibility::Impassable );
        }
    }
}

void CollisionSystem::accept(GEvents::EntityOpenClose *evt)
{
    auto [openable, collider, position] = m_level->getComponents<
            OpenableComponent,
            ColliderComponent,
            PositionComponent
            >(evt->ent);

    openable->isOpen = evt->isOpen;
    if ( evt->isOpen )
    {
        // This is an 'open' event
        collider->blocksLight = false;

        if ( !openable->lightOnly )
        {
            collider->blocksMovement = false;
            m_level->grid().pass().removeDynamic(position->position, evt->ent, Passibility::Impassable);
        }
    }
    else
    {
        // This is a 'close' event
        collider->blocksLight = true;

        if ( !openable->lightOnly )
        {
            collider->blocksMovement = true;
            m_level->grid().pass().setDynamic(position->position, evt->ent, Passibility::Impassable);
        }
    }
}

void CollisionSystem::accept(GEvents::EntityDeath *evt)
{
    // An entity died - if it had a collider and was impassible, update the collision map

    if ( m_level->entityHas<ColliderComponent, PositionComponent>(evt->actor) )
    {
        auto const& [posC, colliderC] = m_level->getComponents<PositionComponent, ColliderComponent>(evt->actor);

        if ( colliderC->blocksMovement )
        {
            m_level->grid().pass().removeDynamic( posC->position, evt->actor, Passibility::Impassable );
        }
    }
}
