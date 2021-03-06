#pragma once

#include <components/ActionComponent.h>
#include <components/ActorComponent.h>
#include <components/AIComponent.h>
#include <components/AnimationComponent.h>
#include <components/ColliderComponent.h>
#include <components/ContainerComponent.h>
#include <components/DescriptionComponent.h>
#include <components/ItemComponent.h>
#include <components/OpenableComponent.h>
#include <components/RenderSystem.h>
#include <components/PositionComponent.h>

template <typename ... Ts >
struct ComponentTypes {};

using AllComponents = ComponentTypes<
    ActionComponent,
    ActorComponent,
    AIComponent,
    AnimationComponent,
    ColliderComponent,
    ContainerComponent,
    DescriptionComponent,
    ItemComponent,
    OpenableComponent,
    RenderComponent,
    PositionComponent
            >;