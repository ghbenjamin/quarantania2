#pragma once


#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class ObjectivesSystem : public System, public GameEventSub<ObjectivesSystem>
{
public:
    explicit ObjectivesSystem(Level* parent);
    ~ObjectivesSystem() override = default;
    
    void operator()(GameEvents::EntityDeath& evt);
    
    template <typename T>
    void operator()(T&& t)
    {}
};
