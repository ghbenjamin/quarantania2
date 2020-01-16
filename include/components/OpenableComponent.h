#pragma once

#include <game/Component.h>
#include <game/Entity.h>
#include <game/Items.h>

struct OpenableComponent : public Component<OpenableComponent>
{
    OpenableComponent() = default;
    ~OpenableComponent() override = default;

    bool isOpen = false;        // Is the entity open or closed?
    bool lightOnly = false;     // Does opening this entity make it passable as well as allow light through?
};