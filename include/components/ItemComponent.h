#pragma once

#include <memory>
#include <game/Component.h>
#include <game/Entity.h>
#include <game/Items.h>


struct ItemComponent : public Component<ItemComponent>
{
    ItemComponent();
    ~ItemComponent() override = default;

    std::shared_ptr<Item> item;
};
