#pragma once

#include <actions/Action.h>
#include <game/Component.h>
#include <game/Character.h>

struct ActorComponent : public Component<ActorComponent>
{
    ActorComponent();
    ~ActorComponent() override = default;

    Character character;

    int initiative;
    int energyPerTick;
    int currentEnergy;
    int maxEnergy;

    ActionPtr nextAction;
};
