#pragma once

#include <vector>
#include <systems/System.h>
#include <utils/Containers.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>


struct TileAnimationPath
{
    std::vector<Vector2i> path;
    Vector2i nextTile;
    Vector2f currentPxPos;
    double totalAnimationTime;
    double timePerStep;
};


class AnimationSystem : public System, public GameEventSub<AnimationSystem>
{
public:
    explicit AnimationSystem(Level *parent);
    ~AnimationSystem() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;

    template <typename T>
    void operator()(T&& t)
    {}

private:
};