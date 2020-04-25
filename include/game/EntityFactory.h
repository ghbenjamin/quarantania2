#pragma once

#include <game/Entity.h>
#include <game/Player.h>
#include <game/Items.h>
#include <utils/Containers.h>
#include <utils/Random.h>

class Level;

class EntityFactory
{
public:
    explicit EntityFactory( Level* parent, RandomGenerator* rg );
    ~EntityFactory() = default;

    PlayerPtr createPlayer(PlayerData const& data, Vector2i startPos) const;
    EntityRef createObject(std::string const& ptype, Vector2i pos) const;
    EntityRef createEnemy(std::string const& name, Vector2i pos) const;
    EntityRef createItem(std::string const& name, Vector2i pos) const;
    EntityRef createItem(std::shared_ptr<Item>, Vector2i pos) const;

private:
    Level* m_parent;
    RandomGenerator* m_rg;
};