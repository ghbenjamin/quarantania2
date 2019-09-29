#pragma once

#include <game/Component.h>

namespace Components
{

// Do I have a description?
struct Description : public Component<Description>
{
    explicit Description(std::string val);
    explicit Description(std::vector<std::string> const& vals);
    ~Description() override = default;

    int current;
    std::vector<std::string> descriptions;
};

}
