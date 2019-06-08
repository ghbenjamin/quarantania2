#pragma once

#include <game/Entity.h>

class EntityFactory
{
public:

    EntityFactory() = default;
    ~EntityFactory() = default;

    EntityRef createPlayer() const;

private:
};