#pragma once

#include <vector>
#include <queue>

#include <systems/System.h>
#include <utils/Containers.h>
#include <game/Animation.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>


class AnimationSystem : public System, public GameEventSub<AnimationSystem>
{
public:
    explicit AnimationSystem(Level *parent);
    ~AnimationSystem() override = default;

    void operator()(GameEvents::EntityMove& evt);
    void operator()(GameEvents::EntityDamage& evt);

    template <typename T>
    void operator()(T&& t)
    {}
};