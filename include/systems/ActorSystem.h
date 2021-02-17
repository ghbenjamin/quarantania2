#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class ActorSystem : public System, public GameEventSub<ActorSystem>
{
public:
    explicit ActorSystem(Level* parent);
    ~ActorSystem() override = default;

    void operator()(GameEvents::EntityDeath& evt);
    void operator()(GameEvents::EntityDamage& evt);

    template <typename T>
    void operator()(T&& t)
    {}
    
    
private:



};
