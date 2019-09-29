#pragma once

#include <game/Component.h>

namespace Components
{

// Can I block the movement of light or other objects?
struct Collider : public Component<Collider>
{
    explicit Collider() = default;
    ~Collider() override = default;
};

}
