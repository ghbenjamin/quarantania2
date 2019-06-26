#pragma once

#include <unordered_map>
#include <queue>

#include <game/Entity.h>
#include <game/GSubber.h>
#include <utils/Logging.h>


enum class GEventType
{
    EntityMove
};

enum class GEventScope
{
    Before,
    After
};

namespace GEvents
{

struct EntityMove
{
    EntityRef ent = EntityNull;
    Vector2i oldPosition;
    Vector2i newPosition;
};

}



struct GEvent
{
    GEventType type;
    GEventScope scope;

    union
    {
         GEvents::EntityMove entityMove;
    } data;
};


class GEventHub
{
public:
    GEventHub() = default;
    ~GEventHub() = default;

    GEventHub( const GEventHub& ) = delete;
    GEventHub& operator=( const GEventHub& ) = delete;

    template <typename T>
    void subscribe( GEventType etype, T* receiver )
    {
        static_assert( std::is_base_of_v<GSubber, T> );
        auto casted = static_cast<GSubber*>(receiver);

        m_subs.emplace( etype, casted );
    };

    template <typename T>
    void unsubscribe( GEventType etype, T* receiver )
    {
        static_assert( std::is_base_of_v<GSubber, T> );
        auto casted = static_cast<GSubber*>(receiver);

        auto it_range = m_subs.equal_range( etype );

        for (auto it = it_range.first; it != it_range.second; it++)
        {
            if ( it->second == receiver )
            {
                m_subs.erase(it);
                break;
            }
        }

    };

    void broadcast( GEvent evt );

private:

    std::unordered_multimap<GEventType, GSubber*> m_subs;
};


class GEventQueue : private GSubber
{
public:
    explicit GEventQueue( GEventHub& hub );
    virtual ~GEventQueue() = default;

private:

    void acceptGEvent(GEvent &event) override;
    std::queue<GEvent> m_queue;

};
