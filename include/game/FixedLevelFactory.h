#pragma once

#include <game/LevelFactory.h>

struct TiledMap;
struct TiledObjectLayer;
class RunState;

enum class RandomEnemyPower
{
    SMALL,
    MEDIUM,
    LARGE
};

struct EnemySpawnData
{
    Vector2i pos;

    RandomEnemyPower power;
    std::string name;
};

struct PlayerSpawnData
{
    Vector2i pos;
};


class FixedLevelFactory : public LevelFactory
{
public:

    FixedLevelFactory(TiledMap const* map, const LevelContextPtr &ctx, std::shared_ptr<RunState> const& runState);
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
    const LevelContextPtr &m_ctx;
    std::shared_ptr<RunState> m_runState;
};