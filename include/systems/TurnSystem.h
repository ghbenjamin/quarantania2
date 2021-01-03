#pragma once

#include <systems/System.h>
#include <game/GameEventDefs.h>


class TurnSystem : public System, public GameEventSub<TurnSystem>
{
public:
    explicit TurnSystem(Level *parent);
    ~TurnSystem() override = default;

    void operator()(GameEvents::TurnChange& evt);
    void operator()(GameEvents::RoundChange& evt);
    void operator()(GameEvents::EntityAction& evt);

    template <typename T>
    void operator()(T&& t)
    {}
};