#pragma once

#include <queue>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class GameEventHub;


class GameEventVariantVisitor
{
public:
    GameEventVariantVisitor(GameEventHub* hub)
    {
        m_hub = hub;
    }

    template <typename T>
    void operator()(T& evt)
    {
        m_hub->broadcastImpl<T>(evt);
    }

private:
    GameEventHub* m_hub;
};


class GameEventHub
{
public:
    friend class GameEventVariantVisitor;

    GameEventHub() = default;
    ~GameEventHub() = default;

    GameEventHub( const GameEventHub& ) = delete;
    GameEventHub& operator=( const GameEventHub& ) = delete;

    template <typename EvtType, typename SubType>
    void subscribe( SubType* receiver, GEventTiming timing )
    {
        static_assert( std::is_base_of_v<GEventSubBase, SubType> );
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        GEventSubBase* base = static_cast<SubType*>(receiver);

        m_subs.emplace(GameEvent<EvtType>::id(), GEventCallback{base, timing} );
    };

    template <typename EvtType, typename SubType>
    void subscribe( SubType* receiver )
    {
        subscribe<EvtType, SubType>( receiver, GEventTiming::On );
    };

    template <typename EvtType, typename SubType>
    void unsubscribe( SubType* receiver )
    {
        static_assert( std::is_base_of_v<GEventSubBase, SubType> );
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        GEventSubBase* base = receiver;
        auto it_range = m_subs.equal_range(GameEvent<EvtType>::id() );

        for (auto it = it_range.first; it != it_range.second; it++)
        {
            if ( it->second == base )
            {
                m_subs.erase(it);
                break;
            }
        }
    };


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
                popQueue();
            }
            m_evtInProgress = false;
        }
    }

private:

    void popQueue()
    {
        auto evt = m_evtQueue.back();
        m_evtQueue.pop();

        std::visit(GameEventVariantVisitor{this}, evt);
    }


    template <typename EvtType>
    void broadcastImpl( EvtType* evt )
    {
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        auto it_range = m_subs.equal_range(GameEvent<EvtType>::id() );

        for ( int i = 0; i < GEventTimingCount; i++ )
        {
            for (auto it = it_range.first; it != it_range.second; it++)
            {
                if ( (int)(it->second.timing) == i )
                {
                    GEventSubBase* base = it->second.target;
                    dynamic_cast<GEventSub<EvtType>*>(base)->accept( evt );
                }
            }
        }
    }

    template <typename EvtType, typename... Args>
    void broadcastImpl( Args... args )
    {
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        EvtType evt { std::forward<Args>(args)... };

        broadcastImpl( &evt );
    }


    std::unordered_multimap<GEventId, GEventCallback> m_subs;
    std::queue<GameEventVariant> m_evtQueue;
    bool m_evtInProgress = false;
};