#include <game/ECS.h>
#include <game/Component.h>
#include <game/System.h>


ECS::ECS()
{
    registerComponent<Components::Render>();
    registerComponent<Components::TilePosition>();
    registerComponent<Components::Collider>();

    registerSystem<Systems::Render>();
    registerSystem<Systems::Position>();
    registerSystem<Systems::Collision>();
}

EntityRef ECS::createEntity()
{
    // Requisition a new ID from the pool, but dont construct any new
    // component objects
    return m_entityPool.next();
}

void ECS::deleteEntity(EntityRef ent)
{
    // Delete all the components attached to this entity
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            v.erase(it);
        }
    }

    // Put the ID back into the pool
    m_entityPool.release(ent);
}

void ECS::disableEntity(EntityRef ent)
{
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            // it->second.DISABLE_ME
        }
    }
}

void ECS::enableEntity(EntityRef ent)
{
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            // it->second.ENABLE_ME
        }
    }
}

void ECS::update(uint32_t ticks, RenderInterface &rInter)
{
    for ( auto const& sys : m_systems )
    {
        sys->update( ticks, rInter );
    }
}

GEventHub &ECS::events()
{
    return m_gevents;
}
