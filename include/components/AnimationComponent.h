#pragma once

#include <vector>
#include <optional>

#include <utils/Containers.h>
#include <engine/Component.h>

struct TileAnimationPath
{
    std::vector<Vector2i> path;
    Vector2i nextTile;
    Vector2f currentPxPos;
    double totalAnimationTime;
    double timePerStep;
};

struct AnimationComponent : public Component<AnimationComponent>
{
    AnimationComponent() = default;
    ~AnimationComponent() override = default;
    
    
    std::optional<TileAnimationPath> movementPathAnim;
};
