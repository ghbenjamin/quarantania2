#pragma once

#include <unordered_map>

#include <game/Entity.h>

enum class GEventType
{

};

struct GEvent
{
    GEventType type;
    union
    {

    };
};


class GEventHub
{
public:
    GEventHub() = default;
    ~GEventHub() = default;

    GEventHub( const GEventHub& ) = delete;
    GEventHub& operator=( const GEventHub& ) = delete;

    void subscribe( GEventType etype, EntityRef subber );
    void unsubsubscribe(  );

    void broadcast( GEvent& evt );

private:

    std::unordered_multimap<GEventType, EntityRef> m_subscriptions;

};