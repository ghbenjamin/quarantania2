#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

namespace Systems
{

class Position : public System,
                 public GEventSub<GEvents::EntityMove>,
                 public GEventSub<GEvents::EntityReady>
{
public:
    explicit Position(Level *parent);
    ~Position() override = default;

    void accept(GEvents::EntityMove *evt) override;
    void accept(GEvents::EntityReady *evt) override;
};

}