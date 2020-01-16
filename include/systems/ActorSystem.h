#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

class ActorSystem : public System,
                    public GEventSub<GEvents::GameTick>
{
public:
    explicit ActorSystem(Level* parent);
    ~ActorSystem() override = default;

    void accept(GEvents::GameTick *evt) override;
};
