#pragma once

#include <engine/Component.h>


// Can I block the movement of light or other objects?
struct ColliderComponent : public Component<ColliderComponent>
{
    ColliderComponent();
    ColliderComponent(bool bl, bool bm);
    ~ColliderComponent() override = default;

    bool blocksLight;
    bool blocksMovement;
};
