#pragma once

#include <components/ActorComponent.h>
#include <components/AIComponent.h>
#include <components/AnimationComponent.h>
#include <components/ColliderComponent.h>
#include <components/ContainerComponent.h>
#include <components/DescriptionComponent.h>
#include <components/ItemComponent.h>
#include <components/LightingComponent.h>
#include <components/RenderComponent.h>
#include <components/PositionComponent.h>

template <typename ... Ts >
struct ComponentTypes {};

using AllComponents = ComponentTypes<
    ActorComponent,
    AIComponent,
    AnimationComponent,
    ColliderComponent,
    ContainerComponent,
    DescriptionComponent,
    ItemComponent,
    LightingComponent,
    RenderComponent,
    PositionComponent
            >;