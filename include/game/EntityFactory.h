#pragma once

#include <game/Entity.h>
#include <game/Player.h>
#include <game/Prefab.h>
#include <game/Enemy.h>
#include <utils/Containers.h>
#include <utils/Random.h>

class Level;

class EntityFactory
{
public:
    explicit EntityFactory( Level* parent, RandomGenerator* rg );
    ~EntityFactory() = default;

    PlayerPtr createPlayer(ImPlayerData & data, Vector2i startPos) const;
    EntityRef createPrefab(PrefabType ptype, Vector2i pos) const;
    EntityRef createEnemy(std::string const& name, Vector2i pos) const;

private:

    template <typename T, typename... Args>
    void addPrefabType(PrefabType ptype, Args... args )
    {
        m_prefabs.emplace(ptype, std::make_shared<T>(std::forward<Args>(args)... ));
    }

    void createPrefabs();

private:
    Level* m_parent;
    RandomGenerator* m_rg;
    EnemyManager m_enemyManager;

    std::unordered_map<PrefabType, std::shared_ptr<PrefabObj>> m_prefabs;
};