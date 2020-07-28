#include <game/FixedLevelFactory.h>
#include <resource/Tiled.h>
#include <utils/Assert.h>

FixedLevelFactory::FixedLevelFactory() : LevelFactory() {}

LevelPtr FixedLevelFactory::create(TiledMap const* map, const LevelContextPtr &ctx, const PartyData &pdata)
{
    m_map = map;
    m_level = std::make_unique<Level>( Vector2i{map->width, map->height}, ctx, RandomGenerator{ m_rd() } );

    constructTiles();
    constructObjects();


    constructParty(pdata);

    m_level->setLayout(m_levelLayout);
    m_level->setReady();
    return std::move(m_level);
}

void FixedLevelFactory::constructTiles()
{
    std::unordered_map<Vector2i, int, Vector2iHash> tilesSeenMap;

    // Initialize empty data structures of the right size
    m_levelLayout.mapData = std::vector<TileRef>( m_map->width * m_map->height, -1 );
    m_levelLayout.tileTypes = std::vector<LD::BaseTileType>( m_map->width * m_map->height, LD::BaseTileType::Floor );

    for ( auto const& tlayer : m_map->tileLayers )
    {
        int xStart = tlayer.xOffset;
        int yStart = tlayer.yOffset;
        int xEnd = xStart + tlayer.width;
        int yEnd = yStart + tlayer.height;
        int idxOffset = xStart + yStart * m_map->width;

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
                    tileRef = m_levelLayout.tileset.addTile(key);
                    tilesSeenMap.emplace(tipVec, tileRef);
                }
                else
                {
                    // Any tile we *have* seen, retrieve from our tileset
                    tileRef = it->second;
                }

                m_levelLayout.mapData[globalIdx] = tileRef;
            }
        }
    }
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
        m_level->entityFactory().createObject( object.name, object.tilePos );
    }
}

void FixedLevelFactory::constructDecor(const TiledObjectLayer &olayer)
{
    for ( auto const& object : olayer.objects )
    {
        Assert( object.gid != 0 );

        SpritesheetKey skey = m_map->keyFromIdPair( object.sprite );
        m_level->entityFactory().createDecor( object.tilePos, skey );
    }
}

void FixedLevelFactory::constructSpawnPoints(TiledObjectLayer const& olayer)
{
    for ( auto const& object : olayer.objects )
    {
        if ( object.name == "player-spawn" )
        {
            m_playerSpawns.push_back(object.tilePos);
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
        m_level->entityFactory().createPlayer( playerIt->generateNewPlayer(), *spawnIt );
    }
}




