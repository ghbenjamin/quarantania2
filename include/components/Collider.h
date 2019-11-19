#pragma once

#include <game/Component.h>

namespace Components
{

// Can I block the movement of light or other objects?
struct Collider : public Component<Collider>
{
    Collider();
    Collider(bool bl, bool bm);
    ~Collider() override = default;

    bool blocksLight;
    bool blocksMovement;
};

}
