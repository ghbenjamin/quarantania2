#pragma once

#include <engine/Component.h>
#include <engine/Entity.h>
#include <game/Items.h>

struct AIComponent : public Component<AIComponent>
{
    AIComponent() = default;
    ~AIComponent() override = default;

    // TBD
};

