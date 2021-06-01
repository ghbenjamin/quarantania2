#include <systems/AISystem.h>
#include <game/Level.h>
#include <components/AIComponent.h>
#include <components/ActorComponent.h>

AISystem::AISystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::TurnChange>(this);
}

void AISystem::operator()(GameEvents::TurnChange& evt)
{
    if ( !evt.isPlayerTurn )
    {
        // The turn has been switched to the AI! Somehow work out what we're going to do now.

        // Loop over each of our entities which has an AI component. Evaluate which action it wants to take,
        // if any.

        // TODO: More than one action.
        // TODO: Need to flush event queue between NPC actions
        for ( EntityRef eref : m_level->ecs().entitiesHaving<AIComponent, ActorComponent>() )
        {
            auto const &[aiC, actorC] = m_level->ecs().getComponents<AIComponent, ActorComponent>(eref);

            auto action = aiC->behaviour.evaluate(m_level, eref);
            if (action)
            {
                action->perform(m_level, eref);
            }
        }

        // Switch the turn back to the player
        m_level->events().broadcast<GameEvents::TurnChange>(true);
    }
}
