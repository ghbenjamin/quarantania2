#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class ActorSystem : public System,
                    public GEventSub<GameEvents::EntityDeath>,
                    public GEventSub<GameEvents::EntityDamage>
{
public:
    explicit ActorSystem(Level* parent);
    ~ActorSystem() override = default;

    void accept(GameEvents::EntityDeath *evt) override;
    void accept(GameEvents::EntityDamage *evt) override;
};
