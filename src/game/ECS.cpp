#include <game/ECS.h>
#include <game/Level.h>
#include <components/All.h>
#include <systems/All.h>

ECS::ECS(Level* parent)
: m_parent(parent), m_entFactory(parent), m_components(16)
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

    auto id = m_entityPool.next();
    m_allEnts.push_back(id);
    return id;
}

void ECS::deleteEntity(EntityRef ent)
{
    // Delete all the components attached to this entity
    for ( auto m : m_components )
    {
        auto it = m.find(ent);
        if ( it != m.end() )
        {
            m.erase(it);
        }
    }

    // Put the ID back into the pool
    m_entityPool.release(ent);
    m_allEnts.erase( std::remove(m_allEnts.begin(), m_allEnts.end(), ent), m_allEnts.end() );
}


void ECS::deleteEntityDelayed(EntityRef ent)
{
    m_delayedDeleteEnts.push_back( ent );
}

EntityFactory &ECS::entityFactory()
{
    return m_entFactory;
}

std::vector<EntityRef> const &ECS::allEntities() const
{
    return m_allEnts;
}


