#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

namespace Systems
{

class Collision : public System,
      public GEventSub<GEvents::EntityMove>,
      public GEventSub<GEvents::EntityReady>,
      public GEventSub<GEvents::EntityOpenClose>
{
public:
    explicit Collision(Level *parent);
    ~Collision() override = default;

    void accept(GEvents::EntityMove *evt) override;
    void accept(GEvents::EntityReady *evt) override;
    void accept(GEvents::EntityOpenClose *evt) override;
};

}