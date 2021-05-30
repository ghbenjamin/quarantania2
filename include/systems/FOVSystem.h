#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>
#include <resource/Sprite.h>

class FOVSystem : public System, public GameEventSub<FOVSystem>
{
public:
    explicit FOVSystem(Level *parent);
    ~FOVSystem() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;

    void operator()(GameEvents::EntityMove& evt);
    void operator()(GameEvents::LevelReady& evt);
    void operator()(GameEvents::EntityOpenClose& evt);

    template <typename T>
    void operator()(T&& t)
    {}

private:

    void recalculateFOV();

    Sprite m_fovHidden;
    Sprite m_fovFog;
    RenderObject m_renderObj;
    bool m_renderDirtyBit;
};