#pragma once

#include <engine/Component.h>
#include <game/Actor.h>

enum class ActorType
{
    PC,
    NPC
};

struct ActorComponent : public Component<ActorComponent>
{
    explicit ActorComponent( Actor&& actor );
    ~ActorComponent() override = default;


    Actor actor;
    ActorType actorType;
};
