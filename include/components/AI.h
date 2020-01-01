#pragma once

#include <game/Component.h>
#include <game/Entity.h>
#include <game/Items.h>

namespace Components
{
struct AI : public Component<AI>
{
    AI() = default;
    ~AI() override = default;
};
}
