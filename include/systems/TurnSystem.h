#pragma once

#include <systems/System.h>
#include <game/GameEventDefs.h>

class TurnSystem : public System,
                   public GEventSub<GameEvents::TurnChange>,
                   public GEventSub<GameEvents::RoundChange>
{
public:
    explicit TurnSystem(Level *parent);
    ~TurnSystem() override = default;

    void accept(GameEvents::TurnChange *evt) override;
    void accept(GameEvents::RoundChange *evt) override;
};