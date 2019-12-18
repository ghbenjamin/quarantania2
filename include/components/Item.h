#pragma once

#include <game/Component.h>
#include <game/Entity.h>

namespace Components
{
struct Item : public Component<Item>
{
    Item();
    ~Item() override = default;
};
}
