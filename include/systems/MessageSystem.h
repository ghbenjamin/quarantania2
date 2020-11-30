#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class MessageSystem : public System,
                      public GEventSub<GameEvents::LevelReady>,
                      public GEventSub<GameEvents::EntityMove>,
                      public GEventSub<GameEvents::EntityOpenClose>
{
public:
    explicit MessageSystem(Level *parent);
    ~MessageSystem() override = default;

    void accept(GameEvents::LevelReady *evt) override;
    void accept(GameEvents::EntityMove *evt) override;
    void accept(GameEvents::EntityOpenClose *evt) override;
};