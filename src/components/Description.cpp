#include <components/Description.h>

Components::Description::Description(std::string val)
    : descriptions({std::move(val)}), current(0)
{
}

Components::Description::Description(std::vector<std::string> const& vals)
    : descriptions(vals), current(0)
{
}
