#pragma once

#include <game/Component.h>
#include <game/Entity.h>
#include <game/Items.h>

namespace Components
{
struct Lockable : public Component<Lockable>
{
    Lockable();
    ~Lockable() override = default;

    bool requiresKey = false;
    ItemPtr keyItem = ItemPtr{};
    int lockStrength = 0;
};
}
