#pragma once

#include <memory>
#include <game/Component.h>
#include <game/Entity.h>

class Item;

namespace Components
{
struct Item : public Component<Item>
{
    Item();
    ~Item() override = default;

    std::shared_ptr<Item> item;
};
}
