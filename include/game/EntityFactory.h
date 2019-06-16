#pragma once

#include <game/Entity.h>
#include <utils/Containers.h>
#include <game/Player.h>

class ECS;

class EntityFactory
{
public:

    EntityFactory( ECS* parent );
    ~EntityFactory() = default;

    std::unique_ptr<Player> createPlayer( ImPlayerData & data, Vector2i startPos ) const;

private:

    ECS* m_parent;

};