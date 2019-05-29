#include <game/LevelFactory.h>

#include <random>
#include <utils/Logging.h>
#include <utils/Random.h>

// With help from https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/

using namespace LF;

LevelFactory::LevelFactory()
    : m_rd(), m_mt( m_rd() ), m_regionIndex(0)
{
}

LevelPtr LevelFactory::create(LevelConfig const &config, LevelContextPtr const &ctx)
{
    m_imdata.tilePixelSize = 16;
    m_imdata.levelSize = config.size;
    m_imdata.tileCount = m_imdata.levelSize.x() * m_imdata.levelSize.y();

    // Constuct actual tiles
    m_tilemap = generateLayout( config, ctx );
    m_imdata.mapLayout = std::vector<TileRef>( m_imdata.tileCount, 0 );

    constructTileMap( config, ctx );

    auto ptr = std::make_unique<Level>( std::move(m_imdata), ctx );
    return std::move(ptr);
}

BaseTileMap LevelFactory::generateLayout(LevelConfig const &config, LevelContextPtr const &ctx)
{
    m_tilemap = std::vector<BaseTileType>( config.size.x() * config.size.y(), BaseTileType::Wall );
    m_mapSize = config.size;

    // Attempt to add a random selection of rooms of various sizes and locations
    addRooms( 500 );

    // Fill all of the space not occupied by rooms with randomly generated maze trees
    fillAllMazes();

    // Find all wall tiles which bridge different rooms/corridors. Semi-randomly select from these tiles to
    // create doors, and keep going until the graph of doors and corridors is completely connected.
    connectRooms();

    // Walk over our corridors, iteratively removing dead ends from corridors. Keep going until there are no dead ends
    // left - all corridors connect rooms.
    pruneCorridors();

    return m_tilemap;
}

void LevelFactory::growMaze(Vector2i start)
{
    newRegion(RegionType::Corridor);
    tileSet(start, BaseTileType::Floor);

    std::vector<Vector2i> cells;
    cells.push_back(start);

    while ( !cells.empty() )
    {
        auto currCell = cells.back();

        std::vector<CardinalDirection> dirs;
        for ( auto &[k, v] : Grid::CardinalNeighbours )
        {
            if ( canFloor( currCell, k ))
            {
                dirs.push_back(k);
            }
        }

        if (!dirs.empty())
        {
            auto dir = *randomElement(dirs.begin(), dirs.end(), m_mt);
            auto nextCell = currCell + (Grid::CardinalNeighbours[dir] * 2);

            tileSet( currCell + Grid::CardinalNeighbours[dir], BaseTileType::Floor);
            tileSet( nextCell, BaseTileType::Floor );

            cells.push_back( nextCell );
        }
        else
        {
            cells.pop_back();
        }
    }
}

void LevelFactory::tileSet(Vector2i tile, BaseTileType ttype)
{
    m_tilemap[indexFromCoords(tile)] = ttype;

    if ( ttype == BaseTileType::Floor )
    {
        m_regionMap[tile] = m_regionIndex;
    }
}

BaseTileType LevelFactory::tileGet(Vector2i tile)
{
    return m_tilemap[indexFromCoords(tile)];
}

int LevelFactory::indexFromCoords(Vector2i coord)
{
    return coord.x() + (coord.y() * m_mapSize.x());
}

bool LevelFactory::canFloor(Vector2i coord, CardinalDirection dir)
{
    auto delta = Grid::CardinalNeighbours[dir];

    if ( !contains(coord + (delta * 3)) )
        return false;

    return tileGet(coord + (delta * 2)) == BaseTileType::Wall;
}

bool LevelFactory::contains(Vector2i coord)
{
    return coord.x() >= 0 && coord.y() >= 0 && coord.x() < m_mapSize.x() && coord.y() < m_mapSize.y();
}

void LevelFactory::addRooms( int maxTries )
{
    for ( int idx = 0; idx < maxTries; idx++ )
    {
        auto roomSize = generateRandomRoomSize();

        std::uniform_int_distribution<> mtRoomX(1, m_mapSize.x() - roomSize.x() - 1);
        std::uniform_int_distribution<> mtRoomY(1, m_mapSize.y() - roomSize.y() - 1);

        int x = (mtRoomX(m_mt) / 2) * 2 + 1;
        int y = (mtRoomY(m_mt) / 2) * 2 + 1;

        Room room;
        room.bounds = { x, y, roomSize.x(), roomSize.y() };

        bool invalid = false;
        for ( auto const& r : m_rooms )
        {
            if ( room.bounds.intersect(r.bounds) )
            {
                invalid = true;
                break;
            }
        }

        if (invalid)
        {
            continue;
        }

        m_rooms.push_back(room);
        newRegion(RegionType::Room);

        for ( int j = room.bounds.y(); j < room.bounds.y() + room.bounds.h(); j++)
        {
            for ( int i = room.bounds.x(); i < room.bounds.x() + room.bounds.w(); i++ )
            {
                tileSet( {i, j}, BaseTileType::Floor );
            }
        }
    }
}

Vector2i LevelFactory::generateRandomRoomSize()
{
    std::uniform_int_distribution<> roomSizing(1, 3);
    auto primary = roomSizing(m_mt) * 2 + 1;

    std::uniform_int_distribution<> secSizing(primary - 2, primary);
    auto secondary = secSizing(m_mt) * 2 + 1;

    std::uniform_int_distribution<> coinflip(1, 2);
    if ( coinflip(m_mt) == 1 )
    {
        std::swap(primary, secondary);
    }

    return {primary, secondary};
}

void LevelFactory::fillAllMazes()
{
    for ( int y = 1; y < m_mapSize.y(); y += 2 )
    {
        for ( int x = 1; x < m_mapSize.x(); x += 2 )
        {
            if ( tileGet({x, y}) != BaseTileType::Wall )
            {
                continue;
            }
            else
            {
                growMaze({x, y});
            }
        }
    }
}

void LevelFactory::connectRooms()
{
    // Find every wall tile which is adjacent to two or more different regions.
    // Store the location of the tile against the regions it spans.
    std::unordered_map<Vector2i, std::unordered_set<int>, Vector2Hash<int>> connectorMap;

    for ( int y = 1; y < m_mapSize.y() - 1; y++ )
    {
        for ( int x = 1; x < m_mapSize.x() - 1; x++ )
        {
            auto pos = Vector2i{x, y};

            // Floor tiles are in a region already
            if ( tileGet(pos) == BaseTileType::Floor )
            {
                continue;
            }

            // Found a wall tile: examine all of its neighbours to determine if any of them are floor tiles,
            // and if so, which regions they belong to.
            std::unordered_set<int> regions;
            for ( auto const& [dir, dir_vec] : Grid::CardinalNeighbours )
            {
                auto it = m_regionMap.find( pos + dir_vec );
                if ( it != m_regionMap.end() )
                {
                    regions.insert( it->second );
                }
            }

            if (regions.size() < 2)
            {
                continue;
            }
            else
            {
                connectorMap[pos] = std::move(regions);
            }

        }
    }


    // Get all of our possible connector positions as a map
    std::vector<Vector2i> allConnectors;
    allConnectors.reserve( connectorMap.size() );

    for ( auto const& [k, v] : connectorMap )
    {
        allConnectors.push_back(k);
    }

    // Initialise data structures to detail which regions have not been connected yet
    std::unordered_map<int, int> mergeMap;
    std::unordered_set<int> openRegions;

    for ( int i = 1; i <= m_regionIndex; i++ )
    {
        mergeMap[i] = i;
        openRegions.insert(i);
    }

    // While we have unconnected regions, add connections.
    while ( openRegions.size() > 1 )
    {
        // Select a random tile from the pool of possible connectors
        auto rand_it = randomElement(allConnectors.begin(), allConnectors.end(), m_mt);
        auto& rand_regions = connectorMap[*rand_it];

        setDoor( Door{
            *rand_it,
            *rand_regions.begin(),
            *(++rand_regions.begin())
        });

        // Find the regions which have been connected by this new connection
        std::vector<int> regions;
        for ( auto const& r : connectorMap[*rand_it])
        {
            regions.push_back( mergeMap[r] );
        }

        int dest = regions.back();
        regions.pop_back();

        // Walk our list of regions and mark any regions which have been newly connected.
        for ( int i = 1; i <= m_regionIndex; i++ )
        {
            if ( std::find( regions.begin(), regions.end(), mergeMap[i] ) != regions.end() )
            {
                mergeMap[i] = dest;
            }
        }

        // Remove newly connected regions from the unconnected list
        for ( auto i : regions )
        {
            openRegions.erase(i);
        }

        // Remove any potential connector sizes from the pool which have been made invalid by our choice of
        // connector site above
        allConnectors.erase( std::remove_if(allConnectors.begin(), allConnectors.end(),
        [&](auto const& pos){

            // We don't want adjacent doors
            if ( Grid::isAdjacent(*rand_it, pos) )
            {
                return true;
            }

            // This position connects regions which are now already connected
            std::unordered_set<int> rlist;
            for ( auto cr : connectorMap[pos] )
            {
                rlist.insert(mergeMap[cr]);
            }

            if ( rlist.size() > 1 )
            {
                return false;
            }

            return true;

        }), allConnectors.end() );

    }
}

void LevelFactory::setDoor(Door door)
{
    m_doors[door.pos] = door;

    if ( weightedFlip(5, m_mt) )
    {
        tileSet( door.pos, BaseTileType::Floor );
    }
    else
    {
        tileSet( door.pos, BaseTileType::Door );
    }
}

void LevelFactory::pruneCorridors()
{
    bool finished = false;

    while (!finished)
    {
        finished = true;

        // Walk over each of our tiles
        for ( int y = 1; y < m_mapSize.y() - 1; y++ )
        {
            for ( int x = 1; x < m_mapSize.x() - 1; x++ )
            {
                auto pos = Vector2i{x, y};

                // We are only interested in floor tiles
                if ( tileGet(pos) == BaseTileType::Wall )
                {
                    continue;
                }

                // Count the number of exits from this tile, e.g. the number of non-wall tiles adjacent to it
                auto count = std::count_if( Grid::CardinalNeighbours.begin(), Grid::CardinalNeighbours.end(),
                [&]( auto& v ){
                    auto c = pos + v.second;
                    return tileGet(c) != BaseTileType::Wall;
                });

                if (count != 1)
                {
                    continue;
                }

                // If this tile has 3 walls around it then it is a dead end - remove the corridor section.
                finished = false;
                tileSet(pos, BaseTileType::Wall);
            }
        }
    }


}

void LevelFactory::newRegion(RegionType type)
{
    m_regionIndex++;
    m_regionTypeMap[m_regionIndex] = type;
}

void LevelFactory::constructTileMap(LevelConfig const &config, LevelContextPtr const &ctx)
{
    auto floorRef = m_imdata.tileMap.addTile({"kenney-tiles", "grass-1"}, true);
    auto wallRef = m_imdata.tileMap.addTile({"kenney-tiles", "wall-1"}, false);
    auto doorRef = m_imdata.tileMap.addTile({"kenney-tiles", "door-1"}, true);
    auto soilRef = m_imdata.tileMap.addTile({"kenney-tiles", "soil-1"}, true);

    for ( size_t i = 0; i < m_tilemap.size(); i++ )
    {
        switch ( m_tilemap[i] )
        {
            case BaseTileType::Wall:
                m_imdata.mapLayout[i] = wallRef;
                break;
            case BaseTileType::Floor:
                m_imdata.mapLayout[i] = floorRef;
                break;
            case BaseTileType::Door:
                m_imdata.mapLayout[i] = doorRef;
                break;
            default:
                m_imdata.mapLayout[i] = soilRef;
                break;
        }
    }
}



