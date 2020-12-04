#pragma once

#include <systems/ActorSystem.h>
#include <systems/AISystem.h>
#include <systems/AnimationSystem.h>
#include <systems/CollisionSystem.h>
#include <systems/CombatSystem.h>
#include <systems/FOVSystem.h>
#include <systems/ItemSystem.h>
#include <systems/MessageSystem.h>
#include <systems/UISystem.h>
#include <systems/PositionSystem.h>
#include <systems/RenderSystem.h>
#include <systems/TurnSystem.h>

template <typename ... Ts >
struct SystemTypes {};

using AllSystems = SystemTypes<
    ActorSystem,
    AISystem,
    AnimationSystem,
    CollisionSystem,
    CombatSystem,
    FOVSystem,
    ItemSystem,
    MessageSystem,
    UISystem,
    PositionSystem,
    RenderSystem,
    TurnSystem
        >;