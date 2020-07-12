#pragma once

#include <memory>
#include <engine/Component.h>
#include <engine/Entity.h>

class Item;

struct ItemComponent : public Component<ItemComponent>
{
    ItemComponent( std::shared_ptr<Item> item );
    ~ItemComponent() override = default;

    std::shared_ptr<Item> item;
};
