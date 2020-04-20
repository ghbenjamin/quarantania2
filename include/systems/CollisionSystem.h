#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

class CollisionSystem : public System,
                        public GEventSub<GEvents::EntityMove>,
                        public GEventSub<GEvents::EntityReady>,
                        public GEventSub<GEvents::EntityOpenClose>,
                        public GEventSub<GEvents::EntityDeath>
{
public:
    explicit CollisionSystem(Level *parent);
    ~CollisionSystem() override = default;

    void accept(GEvents::EntityMove *evt) override;
    void accept(GEvents::EntityReady *evt) override;
    void accept(GEvents::EntityOpenClose *evt) override;
    void accept(GEvents::EntityDeath *evt) override;
};