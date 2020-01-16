#pragma once

#include <game/Component.h>
#include <game/Entity.h>
#include <game/Items.h>

struct AIComponent : public Component<AIComponent>
{
    AIComponent() = default;
    ~AIComponent() override = default;

    // TBD
};

