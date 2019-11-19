#pragma once

#include <game/Entity.h>
#include <game/Player.h>
#include <game/Prefab.h>
#include <utils/Containers.h>
#include <utils/Random.h>

class Level;

class EntityFactory
{
public:

    explicit EntityFactory( Level* parent, RandomGenerator* rg );
    ~EntityFactory() = default;

    std::unique_ptr<Player> createPlayer( ImPlayerData & data, Vector2i startPos ) const;
    EntityRef createPrefabByName(std::string const &name, Vector2i pos) const;

private:

    void createPrefabs();

    void prefabDoor();
    void prefabExit();
    void prefabEntrance();
    void prefabContainer();
    void prefabDecor();

private:

    Level* m_parent;
    RandomGenerator* m_rg;
    std::unordered_map<std::string, PrefabList> m_prefabs;
    std::unordered_map<std::string, void(EntityFactory::*)(EntityRef) const> m_prefabDecorators;

};