#include <components/FixedState.h>

Components::FixedState::FixedState(std::vector<std::string>const& states)
    : states(states), current(0)
{
}