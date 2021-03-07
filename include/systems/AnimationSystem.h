#pragma once

#include <vector>
#include <systems/System.h>
#include <utils/Containers.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>


class AnimationSystem : public System, public GameEventSub<AnimationSystem>
{
public:
    explicit AnimationSystem(Level *parent);
    ~AnimationSystem() override = default;
    
    void update(uint32_t ticks, RenderInterface &rInter) override;

    void operator()(GameEvents::EntityMove& evt);

    template <typename T>
    void operator()(T&& t)
    {}
};