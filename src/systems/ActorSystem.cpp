#include <systems/ActorSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>

ActorSystem::ActorSystem(Level *parent) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityDeath>(this);
    m_level->events().subscribe<GameEvents::EntityDamage>(this);
    m_level->events().subscribe<GameEvents::EntityAction>(this);
    
//    m_level->events().subscribe<GameEvents::TurnChange>(this);
    m_level->events().subscribe<GameEvents::RoundChange>(this);
}

void ActorSystem::operator()(GameEvents::EntityDeath& evt)
{
    m_level->ecs().deleteEntityDelayed( evt.actor );

    m_level->ecs().entityFactory().createCorpse( evt.actor );
}

void ActorSystem::operator()(GameEvents::EntityDamage& evt)
{
    auto actorC = m_level->ecs().getComponents<ActorComponent>( evt.target );
    actorC->actor.acceptDamage( evt.damage );
}

void ActorSystem::operator()( GameEvents::RoundChange &evt)
{
    for (auto const& [actor] : m_level->ecs().entitiesWith<ActorComponent>() )
    {
        actor->actor.actionInfo().reset();

        // Do something with expired modifiers here
    }
}

void ActorSystem::operator()(GameEvents::EntityAction &evt)
{
    auto& actorC = m_level->ecs().getComponents<ActorComponent>( evt.entity )->actor;
    
    ActionSpeedData speedData(&evt.speed);
    actorC.applyAllModifiers( &speedData );
    
    actorC.actionInfo().useAction( speedData.modified );
}


