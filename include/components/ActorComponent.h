#pragma once

#include <actions/Action.h>
#include <game/Component.h>
#include <game/Actor.h>

struct RawCreatureData;
struct PlayerData;

struct ActorComponent : public Component<ActorComponent>
{
    ActorComponent( PlayerData const& pdata );
    ActorComponent( RawCreatureData&& data );
    ~ActorComponent() override = default;

    std::string name;
    Actor character;

    int initiative;
    int energyPerTick;
    int currentEnergy;
    int maxEnergy;

    ActionPtr nextAction;
};
