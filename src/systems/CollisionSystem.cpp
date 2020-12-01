#include <systems/CollisionSystem.h>
#include <components/ColliderComponent.h>
#include <components/PositionComponent.h>
#include <game/Level.h>
#include <components/OpenableComponent.h>

CollisionSystem::CollisionSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityMove>(this );
    m_level->events().subscribe<GameEvents::EntityReady>(this );
    m_level->events().subscribe<GameEvents::EntityOpenClose>(this );
    m_level->events().subscribe<GameEvents::EntityDeath>(this );
}

void CollisionSystem::accept(GameEvents::EntityMove *evt)
{
    // An entity has moved - if it had a collider component, regenerate our information on
    // which tiles are passable and which are not.
    if ( m_level->ecs().entityHas<ColliderComponent>(evt->ent) )
    {
        auto const& collider = m_level->ecs().getComponents<ColliderComponent>(evt->ent);

        if ( collider->blocksMovement )
        {
            m_level->grid().pass().setDynamic( evt->newPos, evt->ent, Passibility::Impassable );
            m_level->grid().pass().removeDynamic(  evt->oldPos, evt->ent, Passibility::Impassable );
        }
    }
}

void CollisionSystem::accept(GameEvents::EntityReady *evt)
{
    // A new entity has just finished being created - if it has a collider component, regenerate our
    // collision information.
    if ( m_level->ecs().entityHas<ColliderComponent, PositionComponent>(evt->ent) )
    {
        auto const& [tilePos, collider] = m_level->ecs().getComponents<PositionComponent, ColliderComponent>(evt->ent);

        if ( collider->blocksMovement )
        {
            m_level->grid().pass().setDynamic( tilePos->position, evt->ent, Passibility::Impassable );
        }
    }
}

void CollisionSystem::accept(GameEvents::EntityOpenClose *evt)
{
    auto [openable, collider, position] = m_level->ecs().getComponents<
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

void CollisionSystem::accept(GameEvents::EntityDeath *evt)
{
    // An entity died - if it had a collider and was impassible, update the collision map

    if ( m_level->ecs().entityHas<ColliderComponent, PositionComponent>(evt->actor) )
    {
        auto const& [posC, colliderC] = m_level->ecs().getComponents<PositionComponent, ColliderComponent>(evt->actor);

        if ( colliderC->blocksMovement )
        {
            m_level->grid().pass().removeDynamic( posC->position, evt->actor, Passibility::Impassable );
        }
    }
}
