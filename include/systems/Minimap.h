#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>
#include <resource/Sprite.h>

namespace Systems
{

class Minimap : public System,
            public GEventSub<GEvents::LevelReady>,
            public GEventSub<GEvents::EntityMove>
{
public:
    explicit Minimap(Level *parent);
    ~Minimap() override = default;

    void accept(GEvents::LevelReady *evt) override;
    void accept(GEvents::EntityMove *evt) override;
};

}