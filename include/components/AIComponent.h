#pragma once

#include <engine/Component.h>
#include <engine/Entity.h>
#include <game/Items.h>
#include <ai/BehaviourTree.h>

struct AIComponent : public Component<AIComponent>
{
    AIComponent();
    ~AIComponent() override = default;

    BehaviourTree behaviour;
};

