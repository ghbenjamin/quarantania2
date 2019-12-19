#pragma once

#include <game/Component.h>
#include <game/Entity.h>
#include <game/Items.h>

namespace Components
{
struct Openable : public Component<Openable>
{
    Openable() = default;
    ~Openable() override = default;

    bool isOpen = false;        // Is the entity open or closed?
    bool lightOnly = false;     // Does opening this entity make it passable as well as allow light through?
};
}
