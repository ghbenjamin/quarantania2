#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>
#include <resource/Sprite.h>

class UISystem : public System,
                 public GEventSub<GEvents::LevelReady>,
                 public GEventSub<GEvents::EntityMove>
{
public:
    explicit UISystem(Level *parent);
    ~UISystem() override = default;

    void accept(GEvents::LevelReady *evt) override;
    void accept(GEvents::EntityMove *evt) override;
};