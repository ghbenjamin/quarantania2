#pragma once

#include <game/LevelFactory.h>

struct TiledMap;
struct TiledObjectLayer;
class RunState;
class LuaState;

struct EnemySpawnData
{
    Vector2i pos;
    std::string name;
};

struct PlayerSpawnData
{
    Vector2i pos;
};


class FixedLevelFactory : public LevelFactory
{
public:

    FixedLevelFactory(LuaState& lua, TiledMap const* map, std::shared_ptr<RunState> const& runState);
    ~FixedLevelFactory() override = default;
    
    std::unique_ptr<Level> createLevel() override;


private:

    void constructTiles();
    void constructObjects();

    void constructFixedObjects(TiledObjectLayer const& olayer);
    void constructDecor(TiledObjectLayer const& olayer);
    void constructSpawnPoints(TiledObjectLayer const& olayer);

    void constructParty();
    void constructEnemies();


    std::vector<PlayerSpawnData> m_playerSpawns;
    std::vector<EnemySpawnData> m_enemySpawns;

    TiledMap const* m_map;
    LuaState& m_lua;
    std::shared_ptr<RunState> m_runState;
};