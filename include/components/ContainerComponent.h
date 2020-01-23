#pragma once

#include <game/Component.h>
#include <game/Items.h>

struct ContainerComponent : public Component<ContainerComponent>
{
    explicit ContainerComponent( ) = default;
    ~ContainerComponent() override = default;

    int maxItems = 10; // TODO Sensible

    std::vector<ItemPtr> items;
};
