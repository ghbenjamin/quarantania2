#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>

class MessageSystem : public System,
                      public GEventSub<GEvents::LevelReady>,
                      public GEventSub<GEvents::EntityMove>,
                      public GEventSub<GEvents::EntityOpenClose>
{
public:
    explicit MessageSystem(Level *parent);
    ~MessageSystem() override = default;

    void accept(GEvents::LevelReady *evt) override;
    void accept(GEvents::EntityMove *evt) override;
    void accept(GEvents::EntityOpenClose *evt) override;
};