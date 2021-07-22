
#include <systems/ObjectivesSystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>

ObjectivesSystem::ObjectivesSystem( Level *parent ) : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityDeath>(this);
}

void ObjectivesSystem::operator()( GameEvents::EntityDeath &evt )
{
    auto actorC = m_level->ecs().getComponents<ActorComponent>(evt.actor);

    if ( actorC->actorType == ActorType::PC )
    {
        if ( m_level->partyRemainingCount() == 0 )
        {
            // Party has died! -> Fail
        }
    }
    else
    {
        if ( m_level->enemiesRemainingCount() == 0 )
        {
            // Enemies are all dead -> Success
            m_level->setLevelExitStatus( LevelExitStatus::Completed );
        }
    }
}
