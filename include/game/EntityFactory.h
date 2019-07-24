#pragma once

#include <game/Entity.h>
#include <utils/Containers.h>
#include <game/Player.h>

class Level;

enum class PrefabEntityType
{
    Door,
    Decor,
    Container,
    Trap,
};

struct EntityPrefab
{
    std::string name;
    std::string spritesheetName;
    std::string spriteName;
    bool passible = false;
    PrefabEntityType prefabType;
};



class EntityFactory
{
public:

    explicit EntityFactory( Level* parent );
    ~EntityFactory() = default;

    void loadAllPrefabs( std::string const& path );

    std::unique_ptr<Player> createPlayer( ImPlayerData & data, Vector2i startPos ) const;
    EntityRef createPrefabByName(Vector2i pos, std::string const &name) const;

    EntityRef createDoor( Vector2i pos ) const;
    EntityRef createEntrance( Vector2i pos ) const;
    EntityRef createExit( Vector2i pos ) const;


    EntityRef debugHighlight( Vector2i pos, std::string const& tile) const;


private:

    Level* m_parent;
    std::unordered_map<std::string, EntityPrefab> m_prefabs;

};