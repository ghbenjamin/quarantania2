#pragma once

#include <unordered_map>
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
    };
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

        auto &[first, last] = m_subs.equal_range( etype );

        for (; first != last; first++)
        {
            if ( (*first).second == receiver )
            {
                m_subs.erase(first);
                break;
            }
        }

    };

    void broadcast( GEvent evt );

private:

    std::unordered_multimap<GEventType, GSubber*> m_subs;


};