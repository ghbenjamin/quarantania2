#pragma once

#include <engine/Component.h>

struct ColliderComponent : public Component<ColliderComponent>
{
    ColliderComponent();
    ColliderComponent(bool bl, bool bm);
    ~ColliderComponent() override = default;

    // Does this object block light?
    bool blocksLight;

    // Does this object block movement?
    bool blocksMovement;
};
