#pragma once

#include <engine/Component.h>
#include <game/Action.h>


struct ActionComponent : public Component<ActionComponent>
{
    ActionComponent();
    ~ActionComponent() override = default;

    std::vector<std::shared_ptr<EntityAction>> actions;
};