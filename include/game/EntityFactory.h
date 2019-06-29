#pragma once

#include <game/Entity.h>
#include <utils/Containers.h>
#include <game/Player.h>

class Level;

class EntityFactory
{
public:

    EntityFactory( Level* parent );
    ~EntityFactory() = default;

    std::unique_ptr<Player> createPlayer( ImPlayerData & data, Vector2i startPos ) const;

    EntityRef createDoor( Vector2i pos ) const;

    EntityRef debugHighlight( Vector2i pos, std::string const& tile) const;


private:

    Level* m_parent;

};