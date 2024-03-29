#include <systems/CollisionSystem.h>
#include <components/ColliderComponent.h>
#include <components/PositionComponent.h>
#include <game/Level.h>

CollisionSystem::CollisionSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityMove>(this );
    m_level->events().subscribe<GameEvents::EntityReady>(this );
    m_level->events().subscribe<GameEvents::EntityDeath>(this );
}

void CollisionSystem::operator()(GameEvents::EntityMove& evt)
{
    // An entity has moved - if it had a collider component, regenerate our information on
    // which tiles are passable and which are not.
    if ( m_level->ecs().entityHas<ColliderComponent>(evt.ent) )
    {
        auto const& collider = m_level->ecs().getComponents<ColliderComponent>(evt.ent);

        if ( collider->blocksMovement )
        {
            m_level->grid().pass().setDynamic( evt.newPos, evt.ent, Passibility::Impassable );
            m_level->grid().pass().removeDynamic(  evt.oldPos, evt.ent, Passibility::Impassable );
        }
    }
}

void CollisionSystem::operator()(GameEvents::EntityReady& evt)
{
    // A new entity has just finished being created - if it has a collider component, regenerate our
    // collision information.
    if ( m_level->ecs().entityHas<ColliderComponent, PositionComponent>(evt.ent) )
    {
        auto const& [tilePos, collider] = m_level->ecs().getComponents<PositionComponent, ColliderComponent>(evt.ent);

        if ( collider->blocksMovement )
        {
            m_level->grid().pass().setDynamic( tilePos->tilePosition, evt.ent, Passibility::Impassable );
        }
    }
}

void CollisionSystem::operator()(GameEvents::EntityDeath& evt)
{
    // An entity died - if it had a collider and was impassible, update the collision map

//    if ( m_level->ecs().entityHas<PositionComponent>())

    auto posC = m_level->ecs().tryGetComponent<PositionComponent>(evt.actor);
    if (posC)
    {
        auto colliderC = m_level->ecs().tryGetComponent<ColliderComponent>(evt.actor);
        if (colliderC->blocksMovement)
        {
            m_level->grid().pass().removeDynamic( posC->tilePosition, evt.actor, Passibility::Impassable );
        }

        m_level->grid().removeEntFromTile( posC->tilePosition, evt.actor );
    }

}
