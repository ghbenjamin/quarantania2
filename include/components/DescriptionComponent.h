#pragma once

#include <engine/Component.h>

struct DescriptionComponent : public Component<DescriptionComponent>
{
    DescriptionComponent(std::string const&, std::string const&, std::string const&);
    ~DescriptionComponent() override = default;

    std::string title;
    std::string shortDescription;
    std::string longDescription;
};
