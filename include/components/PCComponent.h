#pragma once

#include <engine/Component.h>
#include <engine/Entity.h>
#include <game/Items.h>

struct PCComponent : public Component<PCComponent>
{
    PCComponent() = default;
    ~PCComponent() override = default;
};