#pragma once

#include <engine/Entity.h>
#include <game/Items.h>
#include <utils/Containers.h>
#include <utils/Random.h>
#include <resource/Tiled.h>

class Level;
struct PlayerData;

class EntityFactory
{
public:
    explicit EntityFactory( Level* parent );
    ~EntityFactory() = default;

    EntityRef createPlayer(Vector2i pos, PlayerData const &data) const;
    EntityRef createEnemy(Vector2i pos, std::string const &name) const;

    EntityRef createObject(Vector2i pos, std::string const& name, SpritesheetKey const& sprite,
            std::unordered_map<std::string, std::string> const& data) const;

    EntityRef createItem(Vector2i pos, std::string const &name) const;
    EntityRef createItem(Vector2i pos, std::shared_ptr<Item> item) const;
    
    EntityRef createDecor(Vector2i pos, SpritesheetKey const& key) const;
    EntityRef createCorpse( EntityRef actorRef );

private:
    Level* m_parent;
};