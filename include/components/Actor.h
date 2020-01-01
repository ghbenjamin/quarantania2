#pragma once

#include <actions/Action.h>
#include <game/Component.h>
#include <game/Character.h>

namespace Components
{
struct Actor : public Component<Actor>
{
    Actor();
    ~Actor() override = default;

    Character character;

    int initiative;
    int energyPerTick;
    int currentEnergy;
    int maxEnergy;

    ActionPtr nextAction;
};
}
