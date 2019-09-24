#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

namespace Systems
{

class Actors : public System,
               public GEventSub<GEvents::GameTick>
{
public:
    explicit Actors(Level* parent);
    ~Actors() override = default;

    void accept(GEvents::GameTick *evt) override;
};

}