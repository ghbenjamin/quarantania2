#pragma once

#include <vector>
#include <optional>

#include <utils/Containers.h>
#include <engine/Component.h>
#include <utils/Interpolate.h>
#include <utils/Colour.h>


struct AnimationComponent : public Component<AnimationComponent>
{
    AnimationComponent() = default;
    ~AnimationComponent() override = default;
};
