#pragma once

#include <game/Component.h>
#include <utils/Containers.h>

// Do I have a localised position? E.g. I'm not a trigger or a static effect
struct PositionComponent : public Component<PositionComponent>
{
    explicit PositionComponent(Vector2i const& p );
    ~PositionComponent() override = default;

    Vector2i position;
};
