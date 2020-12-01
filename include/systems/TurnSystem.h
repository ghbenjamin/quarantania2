#pragma once

#include <systems/System.h>
#include <game/GameEventDefs.h>

class TurnSystem : public System,
                   public GEventSub<GameEvents::TurnChange>,
                   public GEventSub<GameEvents::EntityAction>,
                   public GEventSub<GameEvents::RoundChange>
{
public:
    explicit TurnSystem(Level *parent);
    ~TurnSystem() override = default;


    void consume(GEventBase* evt);

    void accept(GameEvents::TurnChange *evt) override;
    void accept(GameEvents::RoundChange *evt) override;
    void accept(GameEvents::EntityAction *evt) override;
};