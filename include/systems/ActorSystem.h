#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

class ActorSystem : public System,
                    public GEventSub<GEvents::GameTick>,
                    public GEventSub<GEvents::EntityDeath>,
                    public GEventSub<GEvents::EntityDamage>
{
public:
    explicit ActorSystem(Level* parent);
    ~ActorSystem() override = default;

    void accept(GEvents::GameTick *evt) override;
    void accept(GEvents::EntityDeath *evt) override;
    void accept(GEvents::EntityDamage *evt) override;
};
