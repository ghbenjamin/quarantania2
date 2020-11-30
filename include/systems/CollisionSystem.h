#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class CollisionSystem : public System,
                        public GEventSub<GameEvents::EntityMove>,
                        public GEventSub<GameEvents::EntityReady>,
                        public GEventSub<GameEvents::EntityOpenClose>,
                        public GEventSub<GameEvents::EntityDeath>
{
public:
    explicit CollisionSystem(Level *parent);
    ~CollisionSystem() override = default;

    void accept(GameEvents::EntityMove *evt) override;
    void accept(GameEvents::EntityReady *evt) override;
    void accept(GameEvents::EntityOpenClose *evt) override;
    void accept(GameEvents::EntityDeath *evt) override;
};