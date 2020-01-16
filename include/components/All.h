#pragma once

#include <components/Action.h>
#include <components/Actor.h>
#include <components/AI.h>
#include <components/Collider.h>
#include <components/Description.h>
#include <components/Item.h>
#include <components/Lockable.h>
#include <components/Openable.h>
#include <components/Render.h>
#include <components/Tags.h>
#include <components/TilePosition.h>


template <typename ... Ts >
struct ComponentTypes {};

using AllComponents = ComponentTypes<
    Components::Action,
    Components::Actor,
    Components::AI,
    Components::Collider,
    Components::Description,
    Components::Item,
    Components::Lockable,
    Components::Openable,
    Components::Render,
    Components::Tags,
    Components::TilePosition
            >;