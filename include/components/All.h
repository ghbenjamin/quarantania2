#pragma once

#include <components/ActionComponent.h>
#include <components/ActorComponent.h>
#include <components/AIComponent.h>
#include <components/ColliderComponent.h>
#include <components/ContainerComponent.h>
#include <components/DescriptionComponent.h>
#include <components/ItemComponent.h>
#include <components/LockableComponent.h>
#include <components/OpenableComponent.h>
#include <components/RenderComponent.h>
#include <components/PositionComponent.h>

template <typename ... Ts >
struct ComponentTypes {};

using AllComponents = ComponentTypes<
    ActionComponent,
    ActorComponent,
    AIComponent,
    ColliderComponent,
    ContainerComponent,
    DescriptionComponent,
    ItemComponent,
    LockableComponent,
    OpenableComponent,
    RenderComponent,
    PositionComponent
            >;