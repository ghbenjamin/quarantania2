#pragma once

#include <game/Component.h>

namespace Components
{

// Can I contain items?
struct Container : public Component<Container>
{
    explicit Container( ) = default;
    ~Container() override = default;
};

}
