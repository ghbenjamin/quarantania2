#pragma once

#include <engine/Component.h>
#include <utils/Containers.h>

// Do I have a localised position? E.g. I'm not a trigger or a static effect
struct PositionComponent : public Component<PositionComponent>
{
    PositionComponent(Vector2i const& p, Vector2i const& pixelPosition);
    PositionComponent(Vector2i const& p);
    ~PositionComponent() override = default;

    Vector2i tilePosition;
    Vector2i pixelPosition;
};
