#include <systems/AISystem.h>
#include <game/Level.h>
#include <components/AIComponent.h>
#include <components/ActorComponent.h>

AISystem::AISystem(Level *parent)
        : System(parent), m_passedCount(0), m_currEnt(0)
{
    m_level->events().subscribe<GameEvents::TurnChange>(this);
}

void AISystem::operator()(GameEvents::TurnChange& evt)
{
    if ( !evt.isPlayerTurn )
    {
        // The turn has been switched to the AI! Somehow work out what we're going to do now.

        // Loop over each of our entities which has an AI component and add them to a queue to be processed in the
        // next update ticks.

        m_entsToAct.clear();
        m_passedCount = 0;
        m_currEnt = 0;

        for ( EntityRef eref : m_level->ecs().entitiesHaving<AIComponent, ActorComponent>() )
        {
            m_entsToAct.push_back( eref );
        }
    }
}

void AISystem::update( uint32_t ticks, RenderInterface &rInter )
{
    // Only update the AI system if there are enemy AIs in the action queue and the level is not
    // currently waiting for an animation to finish
    if ( !m_entsToAct.empty() && !m_level->isAnimationBlocking() )
    {
        EntityRef eref = m_entsToAct[m_currEnt];
        auto const &aiC = m_level->ecs().getComponents<AIComponent>(eref);

        auto action = aiC->behaviour.evaluate(m_level, eref);
        if (action)
        {
            action->perform(m_level, eref);
        }
        else
        {
            m_passedCount++;
        }

        m_currEnt++;
        if ( m_currEnt >= m_entsToAct.size() )
        {
            // If all the AI entities passed their turn in a row, then the enemy turn is over.
            if ( m_passedCount == m_entsToAct.size() )
            {
                // Switch the turn back to the player
                m_entsToAct.clear();
                m_level->events().broadcast<GameEvents::TurnChange>(true);
            }

            m_passedCount = 0;
            m_currEnt = 0;
        }
    }
}
