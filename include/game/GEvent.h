#pragma once

#include <unordered_map>

#include <game/Entity.h>

enum class GEventType
{

};

enum class GEventScope
{
    Before,
    After
};

struct GEvent
{
    GEventType type;
    GEventScope scope;
    union
    {

    };
};

using SubRef = std::size_t;

class GEventHub
{
public:
    GEventHub() = default;
    ~GEventHub() = default;

    GEventHub( const GEventHub& ) = delete;
    GEventHub& operator=( const GEventHub& ) = delete;

    SubRef subscribe( GEventType etype, GEventScope escope, EntityRef subber );
    void unsubsubscribe( SubRef ref );

    void broadcast( GEvent& evt );

private:

    std::unordered_multimap<std::pair<GEventType, GEventScope>, EntityRef> m_subscriptions;

};