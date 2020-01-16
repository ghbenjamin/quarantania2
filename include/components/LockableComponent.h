#pragma once

#include <game/Component.h>
#include <game/Entity.h>
#include <game/Items.h>

struct LockableComponent : public Component<LockableComponent>
{
    LockableComponent();
    ~LockableComponent() override = default;

    bool isLocked = false;
    bool requiresKey = false;
    ItemPtr keyItem = ItemPtr{};
    int lockStrength = 0;
};
