#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class AISystem : public System, public GameEventSub<AISystem>
{
public:
    explicit AISystem(Level *parent);
    ~AISystem() override = default;

    void operator()(GameEvents::TurnChange& evt);

    template <typename T>
    void operator()(T&& t)
    {}
};