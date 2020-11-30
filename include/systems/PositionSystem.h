#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class PositionSystem : public System,
                       public GEventSub<GameEvents::EntityMove>,
                       public GEventSub<GameEvents::EntityReady>
{
public:
    explicit PositionSystem(Level *parent);
    ~PositionSystem() override = default;

    void accept(GameEvents::EntityMove *evt) override;
    void accept(GameEvents::EntityReady *evt) override;
};
