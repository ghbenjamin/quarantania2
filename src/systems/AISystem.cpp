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

        // Loop over each of our entities which has an AI component and add them to a queue to be processed in the
        // next update tick.
        
        for ( EntityRef eref : m_level->ecs().entitiesHaving<AIComponent, ActorComponent>() )
        {
            m_entsToAct.push_back( eref );
        }
    }
}

void AISystem::update( uint32_t ticks, RenderInterface &rInter )
{
    // While there are entities in the queue, process them.
    if ( !m_entsToAct.empty() )
    {
        while ( !m_entsToAct.empty() )
        {
            EntityRef eref = m_entsToAct.back();
            m_entsToAct.pop_back();
        
            auto const &aiC = m_level->ecs().getComponents<AIComponent>(eref);
        
            bool actionTaken = true;
            while (actionTaken)
            {
                actionTaken = false;
    
                auto action = aiC->behaviour.evaluate(m_level, eref);
                if (action)
                {
                    action->perform(m_level, eref);
                    actionTaken = true;
                }
            }
        }
    
        // Switch the turn back to the player
        m_level->events().broadcast<GameEvents::TurnChange>(true);
    }
}
