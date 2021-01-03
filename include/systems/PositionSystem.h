#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>


class PositionSystem : public System, public GameEventSub<PositionSystem>
{
public:
    explicit PositionSystem(Level *parent);
    ~PositionSystem() override = default;

    void operator()(GameEvents::EntityMove& evt);
    void operator()(GameEvents::EntityReady& evt);

    template <typename T>
    void operator()(T&& t)
    {
        Logging::log( "F" );
    }
};