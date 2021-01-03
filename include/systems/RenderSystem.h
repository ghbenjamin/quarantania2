#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class RenderSystem : public System, public GameEventSub<RenderSystem>
{
public:
    explicit RenderSystem(Level *parent);
    ~RenderSystem() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;

    template <typename T>
    void operator()(T&& t)
    {}
};