#pragma once

#include <systems/Actors.h>
#include <systems/Collision.h>
#include <systems/Combat.h>
#include <systems/FOV.h>
#include <systems/Message.h>
#include <systems/Minimap.h>
#include <systems/Position.h>
#include <systems/Render.h>

template <typename ... Ts >
struct SystemTypes {};

using AllSystems = SystemTypes<
    Systems::Actors,
    Systems::Collision,
    Systems::Combat,
    Systems::FOV,
    Systems::Message,
    Systems::Minimap,
    Systems::Position,
    Systems::Render
        >;