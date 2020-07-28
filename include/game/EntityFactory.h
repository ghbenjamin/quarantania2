#pragma once

#include <engine/Entity.h>
#include <game/Player.h>
#include <game/Items.h>
#include <utils/Containers.h>
#include <utils/Random.h>

class Level;

class EntityFactory
{
public:
    explicit EntityFactory( Level* parent );
    ~EntityFactory() = default;

    EntityRef createPlayer(PlayerData const& data, Vector2i pos) const;
    EntityRef createObject(std::string const& ptype, Vector2i pos) const;
    EntityRef createEnemy(std::string const& name, Vector2i pos) const;
    EntityRef createItem(std::string const& name, Vector2i pos) const;
    EntityRef createItem(std::shared_ptr<Item>, Vector2i pos) const;

    EntityRef createDecor( Vector2i pos, SpritesheetKey const& key );

private:
    Level* m_parent;
};