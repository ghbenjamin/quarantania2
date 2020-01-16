#include <components/DescriptionComponent.h>

DescriptionComponent::DescriptionComponent(std::string val)
    : descriptions({std::move(val)}), current(0)
{
}

DescriptionComponent::DescriptionComponent(std::vector<std::string> const& vals)
    : descriptions(vals), current(0)
{
}
