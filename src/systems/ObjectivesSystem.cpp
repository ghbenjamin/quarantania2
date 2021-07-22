
#include <systems/ObjectivesSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>

ObjectivesSystem::ObjectivesSystem( Level *parent ) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityDeath>(this, GEventTiming::After);
}

void ObjectivesSystem::operator()( GameEvents::EntityDeath &evt )
{
    auto actorC = m_level->ecs().getComponents<ActorComponent>(evt.actor);

    if ( actorC->actorType == ActorType::PC )
    {
        if ( m_level->partyRemainingCount() == 1 )
        {
            // Party has died! -> Fail
        }
    }
    else
    {
        // This event will kick in before
        if ( m_level->enemiesRemainingCount() == 1 )
        {
            // Enemies are all dead -> Success
            m_level->setLevelExitStatus( LevelExitStatus::Completed );
        }
    }
}
