#pragma once

#include <game/Component.h>
#include <game/Items.h>

namespace Components
{

struct Container : public Component<Container>
{
    explicit Container( ) = default;
    ~Container() override = default;

    int maxItems = -1;
    int maxWeight = -1;

    std::vector<ItemPtr> storedItems;
};

}
