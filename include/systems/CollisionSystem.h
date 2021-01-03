#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class CollisionSystem : public System, public GameEventSub<CollisionSystem>
{
public:
    explicit CollisionSystem(Level *parent);
    ~CollisionSystem() override = default;

    void operator()(GameEvents::EntityMove& evt);
    void operator()(GameEvents::EntityReady& evt);
    void operator()(GameEvents::EntityOpenClose& evt);
    void operator()(GameEvents::EntityDeath& evt);

    template <typename T>
    void operator()(T&& t)
    {}
};