#include <game/FixedLevelFactory.h>
#include <resource/Tiled.h>

FixedLevelFactory::FixedLevelFactory() : LevelFactory() {}

LevelPtr FixedLevelFactory::create(TiledMap const& map, const LevelContextPtr &ctx, const PartyData &pdata)
{
    m_level = std::make_unique<Level>( Vector2i{map.width, map.height}, ctx, RandomGenerator{ m_rd() } );

    assembleTiledMap(map);


    m_level->setLayout(m_levelLayout);
    m_level->setReady();
    return std::move(m_level);
}

void FixedLevelFactory::assembleTiledMap(TiledMap const& map)
{

    // Create Tiles


    std::unordered_map<Vector2i, int, Vector2iHash> tilesSeenMap;

    // Initialize empty data structures of the right size
    m_levelLayout.mapData = std::vector<TileRef>( map.width * map.height, -1 );
    m_levelLayout.tileTypes = std::vector<LD::BaseTileType>( map.width * map.height, LD::BaseTileType::Floor );

    for ( auto const& tlayer : map.tileLayers )
    {
        int xStart = tlayer.xOffset;
        int yStart = tlayer.yOffset;
        int xEnd = xStart + tlayer.width;
        int yEnd = yStart + tlayer.height;
        int idxOffset = xStart + yStart * map.width;

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
                    SpritesheetKey key = { map.tilesets.at(tip.tilesheetIdx).sheetName, (int) tip.id };
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

    for ( auto const& olayer : map.objectLayers )
    {
        if ( olayer.name == "Fixed" )
        {
            constructFixedObjects(olayer);
        }
    }


}

void FixedLevelFactory::constructFixedObjects(TiledObjectLayer const& olayer)
{
    for ( auto const& object : olayer.objects )
    {
        m_level->entityFactory().createObject( object.name, {object.tileX, object.tileY} );
    }
}
