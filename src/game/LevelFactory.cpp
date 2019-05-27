#include <game/LevelFactory.h>

#include <random>
#include <utils/Logging.h>


// With help from https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/


LevelPtr LevelFactory::create(LevelConfig const &config, LevelContextPtr const &ctx)
{
    // Broad debug code for now follows:

    ImmutableLevelData imd;

    imd.tilePixelSize = 16;
    imd.levelSize = config.size;
    imd.tileCount = imd.levelSize.x() * imd.levelSize.y();
    imd.defaultVisibility = false;
    imd.defaultPassibility = 0;
    imd.defaultLightLevel = 1;

    // Construct tilemap
    auto floorRef = imd.tileMap.addTile({"kenney-tiles", "grass-1"}, true);
    auto wallRef = imd.tileMap.addTile({"kenney-tiles", "wall-1"}, false);
    auto doorRef = imd.tileMap.addTile({"kenney-tiles", "door-1"}, true);
    auto soilRef = imd.tileMap.addTile({"kenney-tiles", "soil-1"}, true);

    // Constuct actual tiles
    m_tilemap = generateLayout( config, ctx );

    // Assume that tileref 0 is the default tile - construct our vector to be all default
    imd.mapLayout = std::vector<TileRef>( imd.tileCount, 0 );

    for ( int i = 0; i < m_tilemap.size(); i++ )
    {
        switch ( m_tilemap[i] )
        {
            case BaseTileType::Wall:
                imd.mapLayout[i] = wallRef;
                break;
            case BaseTileType::Floor:
                imd.mapLayout[i] = floorRef;
                break;
            case BaseTileType::Door:
                imd.mapLayout[i] = doorRef;
                break;
            default:
                imd.mapLayout[i] = soilRef;
                break;
        }
    }


    auto ptr = std::make_unique<Level>( std::move(imd), ctx );
    return std::move(ptr);
}

BaseTileMap LevelFactory::generateLayout(LevelConfig const &config, LevelContextPtr const &ctx)
{
    m_tilemap = std::vector<BaseTileType>( config.size.x() * config.size.y(), BaseTileType::Wall );
    m_mapSize = config.size;

    addRooms( 500 );

    for ( int y = 1; y < m_mapSize.y(); y += 2 )
    {
        for ( int x = 1; x < m_mapSize.x(); x += 2 )
        {
            if ( tileGet({x, y}) == BaseTileType::Wall )
            {
                continue;
            }
            else
            {
                growMaze({x, y});
            }
        }
    }

    return m_tilemap;
}

void LevelFactory::growMaze(Vector2i start)
{
    std::vector<Vector2i> cells;
    tileSet(start, BaseTileType::Floor);

    cells.push_back(start);

    while ( !cells.empty() )
    {
        std::vector<CardinalDirection> dirs;
        auto currCell = cells.back();

        for ( auto &[k, v] : Grid::CardinalNeighbours )
        {
            if ( canFloor( currCell, k ))
            {
                dirs.push_back(k);
            }
        }

        if (!dirs.empty())
        {
            std::vector<CardinalDirection> single_dir;
            std::sample( dirs.begin(), dirs.end(), std::back_inserter(single_dir), 1, m_mt );

            auto dir = single_dir.back();
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

LevelFactory::LevelFactory()
: m_rd(), m_mt( m_rd() )
{

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

        RectI room { x, y, roomSize.x(), roomSize.y() };

        bool invalid = false;
        for ( auto const& r : m_rooms )
        {
            if ( room.intersect(r) )
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


        for ( int j = room.y(); j < room.y() + room.h(); j++)
        {
            for ( int i = room.x(); i < room.x() + room.w(); i++ )
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

    // TODO Rectangles

    return {primary, secondary};
}



