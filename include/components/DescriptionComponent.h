#pragma once

#include <game/Component.h>

// Do I have a description?
struct DescriptionComponent : public Component<DescriptionComponent>
{
    explicit DescriptionComponent(std::string val);
    explicit DescriptionComponent(std::vector<std::string> const& vals);
    ~DescriptionComponent() override = default;

    int current;
    std::vector<std::string> descriptions;
};
