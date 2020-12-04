#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class AISystem : public System,
                 public GEventSub<GameEvents::TurnChange>
{
public:
    explicit AISystem(Level *parent);
    ~AISystem() override = default;

    void accept(GameEvents::TurnChange *evt) override;
};