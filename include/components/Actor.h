#pragma once

#include <game/Component.h>
#include <game/Action.h>

namespace Components
{

struct Actor : public Component<Actor>
{
    Actor();
    ~Actor() override = default;

    int initiative;
    int energyPerTick;
    int currentEnergy;
    int maxEnergy;

    std::unique_ptr<Action> nextAction;
};


}
