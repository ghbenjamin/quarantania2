#include <systems/ActorSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>

ActorSystem::ActorSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityDeath>(this);
    m_level->events().subscribe<GameEvents::EntityDamage>(this);
}

void ActorSystem::operator()(GameEvents::EntityDeath& evt)
{
    m_level->ecs().deleteEntityDelayed( evt.actor );
    Logging::log( "ACTOR WAS KILLED" );
}

void ActorSystem::operator()(GameEvents::EntityDamage& evt)
{
    auto actorC = m_level->ecs().getComponents<ActorComponent>( evt.target );
    actorC->actor.acceptDamage( evt.damage );
}

void ActorSystem::operator()(GameEvents::RoundChange &evt)
{
    for (auto const& [actor] : m_level->ecs().entitiesWith<ActorComponent>() )
    {
        // Do something with expired modifiers here
    }
}
