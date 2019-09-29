#pragma once

#include <game/Component.h>

namespace Components
{

struct Tags : public Component<Tags>
{
    Tags();
    ~Tags() override = default;
};

}
