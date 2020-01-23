#pragma once

#include <systems/System.h>
#include <game/GEventDefs.h>

class ItemSystem : public System,
                   public GEventSub<GEvents::ItemPickup>
{
public:
    explicit ItemSystem(Level *parent);
    ~ItemSystem() override = default;

    void accept(GEvents::ItemPickup *evt) override;

};