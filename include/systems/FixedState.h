#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

namespace Systems
{

class FixedState : public System,
                   public GEventSub<GEvents::FixedStateChange>
{
public:
    explicit FixedState(Level *parent);
    ~FixedState() override = default;

    void accept(GEvents::FixedStateChange *evt) override;
};

}