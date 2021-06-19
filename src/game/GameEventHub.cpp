#include <game/GameEventHub.h>
#include <game/Level.h>


GameEventHub::GameEventHub(Level* level)
: m_level(level) {}

void GameEventHub::popQueue()
{
    auto evt = m_evtQueue.front();
    m_evtQueue.pop();
    
    // All the subscribers for this event type, regardless of subscription timing
    auto it_range = m_subs.equal_range( evt.id );
    
    // For each subscription timing (before, on, after)
    for ( int i = 0; i < GEventTimingCount /* 3 */; i++ )
    {
        // Loop over each subscription three times, firing them once each in the correct order.
        for (auto it = it_range.first; it != it_range.second; it++)
        {
            int timing = (int)it->second.timing;
            if ( timing == i )
            {
                GEventSubBase* base = it->second.target;
                base->accept( evt.evt );
            }
        }
    }
}

void GameEventHub::pollAllEvents()
{
    // Pause the event queue while we're waiting for an animation to finish
    while ( !m_evtQueue.empty() && !m_level->isAnimationBlocking() )
    {
        popQueue();
    }
}
