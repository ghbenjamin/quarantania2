#pragma once

#include <game/Component.h>
#include <actions/Action.h>

struct ActionComponent : public Component<ActionComponent>
{
    ActionComponent();
    ~ActionComponent() override = default;

    std::vector<std::shared_ptr<EntityAction>> actions;
};
