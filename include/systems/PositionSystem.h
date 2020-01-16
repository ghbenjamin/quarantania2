#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

class PositionSystem : public System,
                       public GEventSub<GEvents::EntityMove>,
                       public GEventSub<GEvents::EntityReady>
{
public:
    explicit PositionSystem(Level *parent);
    ~PositionSystem() override = default;

    void accept(GEvents::EntityMove *evt) override;
    void accept(GEvents::EntityReady *evt) override;
};
