#pragma once

#include <actions/Action.h>
#include <game/Component.h>
#include <game/Character.h>

struct RawCreatureData;

struct ActorComponent : public Component<ActorComponent>
{
    ActorComponent();
    ActorComponent( RawCreatureData&& data );
    ~ActorComponent() override = default;

    std::string name;
    Character character;

    int initiative;
    int energyPerTick;
    int currentEnergy;
    int maxEnergy;

    ActionPtr nextAction;
};
