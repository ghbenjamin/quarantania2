#include <game/FixedLevelFactory.h>
#include <resource/Tiled.h>
#include <utils/Assert.h>
#include <game/ECS.h>
#include <resource/ResourceManager.h>

FixedLevelFactory::FixedLevelFactory() : LevelFactory() {}

LevelPtr FixedLevelFactory::create(TiledMap const* map, const LevelContextPtr &ctx, const PartyData &pdata)
{
    m_map = map;
    m_level = std::make_unique<Level>( Vector2i{map->width, map->height}, ctx, RandomGenerator{ m_rd() } );

    constructTiles();
    constructObjects();
    constructParty(pdata);
    constructEnemies();

    m_level->setLayout(m_levelLayout);
    m_level->setReady();
    return std::move(m_level);
}

void FixedLevelFactory::constructTiles()
{
    std::unordered_map<Vector2i, int, Vector2iHash> tilesSeenMap;

    // Initialize empty data structures of the right size
    m_levelLayout.mapData = std::vector<TileRef>( m_map->width * m_map->height, -1 );
    m_levelLayout.tileTypes = std::vector<BaseTileType>( m_map->width * m_map->height, BaseTileType::Floor );
    
    std::string sheetName;

    for ( auto const& tlayer : m_map->tileLayers )
    {
        int xStart = tlayer.xOffset;
        int yStart = tlayer.yOffset;
        int xEnd = xStart + tlayer.width;
        int yEnd = yStart + tlayer.height;
        int idxOffset = xStart + yStart * m_map->width;
        bool isWall = tlayer.name == "Walls";

        // Walk over each tile in each layer (some may be blank)
        for ( int y = yStart; y < yEnd; y++ )
        {
            for ( int x = xStart; x < xEnd; x++ )
            {
                int tileRef;
                int localIdx = x + y * tlayer.width;
                int globalIdx = idxOffset + localIdx;
                TiledIdPair tip = tlayer.gids[localIdx];

                // blank tile
                if ( tip == TiledIdPair{0, 0} )
                {
                    continue;
                }

                Vector2i tipVec = { (int) tip.id, tip.tilesheetIdx };

                auto it = tilesSeenMap.find( tipVec );
                if ( it == tilesSeenMap.end() )
                {
                    // Any tile we've not seen yet, construct and add to our tileset
                    SpritesheetKey key = m_map->keyFromIdPair(tip);
                    sheetName = key.sheetName;
                    tileRef = m_levelLayout.tileset.addTile(key);
                    tilesSeenMap.emplace(tipVec, tileRef);
                }
                else
                {
                    // Any tile we *have* seen, retrieve from our tileset
                    tileRef = it->second;
                }

                m_levelLayout.mapData[globalIdx] = tileRef;

                // If this is a wall layer and we have a tile, mark the corresponding tile in the
                // level passibility map as impassible
                if (isWall)
                {
                    m_level->grid().pass().setFixed(globalIdx, Passibility::Impassable);
                }
            }
        }
    }
    
    // Set the tileset texture to the texture of the relevant tilesheet
    m_levelLayout.tileset.setTexture( ResourceManager::get().getSpritesheet(sheetName).get()->getTexture() );
}

void FixedLevelFactory::constructObjects()
{
    for ( auto const& olayer : m_map->objectLayers )
    {
        if ( olayer.name == "Fixed" )
        {
            constructFixedObjects(olayer);
        }
        else if ( olayer.name == "Decor")
        {
            constructDecor(olayer);
        }
        else if ( olayer.name == "Spawns" )
        {
            constructSpawnPoints(olayer);
        }
    }
}

void FixedLevelFactory::constructFixedObjects(TiledObjectLayer const& olayer)
{
    for ( auto const& object : olayer.objects )
    {
        m_level->ecs().entityFactory().createObject(object.tilePos, object.name,
                m_map->keyFromIdPair(object.sprite), object.props);
    }
}

void FixedLevelFactory::constructDecor(const TiledObjectLayer &olayer)
{
    for ( auto const& object : olayer.objects )
    {
        Assert( object.gid != 0 );

        SpritesheetKey skey = m_map->keyFromIdPair( object.sprite );
        m_level->ecs().entityFactory().createDecor( object.tilePos, skey );
    }
}

void FixedLevelFactory::constructSpawnPoints(TiledObjectLayer const& olayer)
{
    for ( auto const& object : olayer.objects )
    {
        if ( object.name == "player-spawn" )
        {
            PlayerSpawnData spawn;
            spawn.pos = object.tilePos;
            m_playerSpawns.push_back(spawn);
        }
        else if ( object.name == "enemy-spawn" )
        {
            EnemySpawnData spawn;
            spawn.pos = object.tilePos;

            for ( auto const& [k, v] : object.props )
            {
                if (k == "power")
                {
                    if ( v == "small" )
                    {
                        spawn.power = RandomEnemyPower::SMALL;
                    }
                    else if (v == "medium")
                    {
                        spawn.power = RandomEnemyPower::MEDIUM;
                    }
                    else if (v == "large")
                    {
                        spawn.power = RandomEnemyPower::LARGE;
                    }
                    else
                    {
                        AssertAlwaysMsg( "Unknown enemy spawn power: '" + v + "'" );
                    }
                }
                else if (k == "name")
                {
                    spawn.name = v;
                }
                else
                {
                    AssertAlwaysMsg( "Unknown enemy spawn prop: '" + k + "'" );
                }
            }

            m_enemySpawns.push_back(spawn);
        }
        else
        {
            AssertAlwaysMsg( "Unknown spawn object name: '" + object.name + "'" );
        }
    }
}

void FixedLevelFactory::constructParty(const PartyData &pdata)
{
    m_level->random().shuffle( m_playerSpawns );

    AssertMsg( pdata.playerChars.size() <= m_playerSpawns.size(),
        "more players than spawns available" );

    auto playerIt = pdata.playerChars.begin();
    auto spawnIt = m_playerSpawns.begin();

    for ( ; playerIt != pdata.playerChars.end(); playerIt++,spawnIt++ )
    {
        m_level->ecs().entityFactory().createPlayer( spawnIt->pos, *playerIt );
    }
}

void FixedLevelFactory::constructEnemies()
{
    for ( auto const& spawn : m_enemySpawns )
    {
        if ( !spawn.name.empty() )
        {
            m_level->ecs().entityFactory().createEnemy( spawn.pos, spawn.name );
        }
        else
        {
            // TODO: Random or semi-random enemy generation based on power level and theme
        }
    }
}




