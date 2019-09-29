#pragma once

#include <game/Component.h>

namespace Components
{


// TBD
struct FixedState : public Component<FixedState>
{
    explicit FixedState( std::vector<std::string> const& states );
    ~FixedState() override = default;

    std::vector<std::string> states;
    int current;
};


}
