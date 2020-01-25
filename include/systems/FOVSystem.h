#pragma once

#include <systems/System.h>
#include <game/GEvent.h>
#include <game/GEventDefs.h>
#include <resource/Sprite.h>

class FOVSystem : public System,
                  public GEventSub<GEvents::EntityMove>,
                  public GEventSub<GEvents::LevelReady>,
                  public GEventSub<GEvents::EntityOpenClose>
{
public:
    explicit FOVSystem(Level *parent);
    ~FOVSystem() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;

    void accept(GEvents::EntityMove *evt) override;
    void accept(GEvents::LevelReady *evt) override;
    void accept(GEvents::EntityOpenClose *evt) override;

private:

    void recalculateFOV();

    Sprite m_fovHidden;
    Sprite m_fovFog;
};