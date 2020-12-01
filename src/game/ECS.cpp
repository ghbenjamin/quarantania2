#include <game/ECS.h>
#include <game/Level.h>
#include <components/All.h>
#include <systems/All.h>

ECS::ECS(Level* parent)
: m_parent(parent), m_entFactory(parent)
{
    registerComponents<AllComponents>();
    registerSystems<AllSystems>();
}


void ECS::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    // Delete our delayed delete entities
    for ( EntityRef ent : m_delayedDeleteEnts )
    {
        deleteEntity( ent );
    }


    // Render everything managed by the ECS
    for ( auto const& sys : m_systems )
    {
        sys->update( ticks, rInter );
    }
}


void ECS::entityReady(EntityRef ent)
{
    m_parent->events().broadcast<GameEvents::EntityReady>(ent);
}

EntityRef ECS::createEntity()
{
    // Requisition a new ID from the pool, but don't construct any new
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


void ECS::deleteEntityDelayed(EntityRef ent)
{
    m_delayedDeleteEnts.push_back( ent );
}

EntityFactory &ECS::entityFactory()
{
    return m_entFactory;
}


