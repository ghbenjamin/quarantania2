#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>
#include <resource/Sprite.h>

class FOVSystem : public System,
                  public GEventSub<GameEvents::EntityMove>,
                  public GEventSub<GameEvents::LevelReady>,
                  public GEventSub<GameEvents::EntityOpenClose>
{
public:
    explicit FOVSystem(Level *parent);
    ~FOVSystem() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;

    void accept(GameEvents::EntityMove *evt) override;
    void accept(GameEvents::LevelReady *evt) override;
    void accept(GameEvents::EntityOpenClose *evt) override;

private:

    void recalculateFOV();

    Sprite m_fovHidden;
    Sprite m_fovFog;
};