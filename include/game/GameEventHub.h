#pragma once

#include <queue>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>
#include <game/GameEventVariant.h>


class GameEventHub
{
public:
    GameEventHub() = default;
    ~GameEventHub() = default;

    GameEventHub( const GameEventHub& ) = delete;
    GameEventHub& operator=( const GameEventHub& ) = delete;


    template <typename EvtType>
    void subscribe( GEventSubBase* receiver, GEventTiming timing = GEventTiming::On )
    {
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        m_subs.emplace(GameEvent<EvtType>::id(), GEventCallback{receiver, timing} );
    };

    template <typename EvtType>
    void unsubscribe( GEventSubBase* receiver )
    {
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        auto it_range = m_subs.equal_range(GameEvent<EvtType>::id() );

        for (auto it = it_range.first; it != it_range.second; it++)
        {
            if ( it->second == receiver )
            {
                m_subs.erase(it);
                break;
            }
        }
    };

    // Broadcast a game event to everyone who is subscribed to it. If more events are fired as a result of
    // this event, those events are queued and resolved in the order they were raised.
    template <typename EvtType, typename... Args>
    void broadcast( Args... args )
    {
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        EvtType evt { std::forward<Args>(args)... };
        GameEventVariant var = evt;

        // Push the event to the back of the queue
        m_evtQueue.push(evt);

        // If we're already resolving an event, add it to the queue and do nothing - the original event will handle
        // broadcasting this derived event. If not, then take responsiblity for popping events off of the queue
        // untill the queue is empty.
        if (!m_evtInProgress)
        {
            m_evtInProgress = true;
            while ( !m_evtQueue.empty() )
            {
                popQueue<EvtType>();
            }
            m_evtInProgress = false;
        }
    }

private:

    template <typename EvtType>
    void popQueue()
    {
        auto evt = m_evtQueue.back();
        m_evtQueue.pop();

        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        auto it_range = m_subs.equal_range( GameEvent<EvtType>::id() );

        for ( int i = 0; i < GEventTimingCount; i++ )
        {
            for (auto it = it_range.first; it != it_range.second; it++)
            {
                if ( (int)(it->second.timing) == i )
                {
                    GEventSubBase* base = it->second.target;
                    base->accept( evt );
                }
            }
        }
    }


    std::unordered_multimap<GEventId, GEventCallback> m_subs;
    std::queue<GameEventVariant> m_evtQueue;
    bool m_evtInProgress = false;
};