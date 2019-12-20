#pragma once

#include <game/Component.h>
#include <actions/Action.h>

namespace Components
{
struct Action : public Component<Action>
{
    Action();
    ~Action() override = default;

    std::vector<ActionPtr> actions;
};
}
