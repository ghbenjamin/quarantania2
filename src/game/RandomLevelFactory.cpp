#include <game/RandomLevelFactory.h>

#include <random>
#include <bitset>

#include <utils/Logging.h>
#include <utils/Random.h>
#include <utils/Assert.h>
#include <game/ResourceDatabase.h>

// With help from https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/

using namespace LD;

RandomLevelFactory::RandomLevelFactory() : LevelFactory(), m_regionIndex(0) {}

LevelPtr RandomLevelFactory::create(RandomLevelConfig const &config, LevelContextPtr const &ctx, PartyData const& pdata)
{
    m_level = std::make_unique<Level>( config.size, ctx, RandomGenerator{ m_rd() } );

    /**
     * Construction of the basic level layout: rooms, corridors, doors
     */

    // Start off with a map full of walls
    m_levelLayout.tileTypes = std::vector<BaseTileType>( config.size.area(), BaseTileType::Wall );

    // Attempt to place special rooms (start, exit, boss, etc.)
    // Do this first so that we can be sure that they're definitely there
    placeSpecialRooms();

    // Attempt to add a random selection of rooms of various sizes and locations
    addRooms( config.roomDensity );

    // Fill all of the space not occupied by rooms with randomly generated maze trees
    fillAllMazes();

    // Find all wall tiles which bridge different rooms/corridors. Semi-randomly select from these tiles to
    // create doors, and keep going until the graph of doors and corridors is completely connected.
    connectRooms();

    // Walk over our corridors, iteratively removing dead ends from corridors. Keep going until there are no dead ends
    // left - all corridors connect rooms.
    pruneCorridors();

    // Walk over every tile and work out which of its 8 neighbours are wall tiles, if any
    calcAllAdjacentWalls();

    // Fix our wall tiles as impassable
    setInitialCollisionData();

    // Construct a map of sprites for the prebaked tiles we are going to need
    constructMapRendering(config, ctx);

    // Construct basic door prefabs at most of our junction points
    constructDoors();


    /**
     * Filling the rooms and corridors with things
     */


    // Semi-randomly assign our special rooms
    assignSpecialRooms();

    // Fill each of our rooms with entities
    decorateRooms();

    /**
     * Constucting the player
     */

    // Add the player at an appropriate place
    constructParty(pdata);

    /**
     * Cleanup.
     */

    // Mark the level construction as completed
    m_level->setLayout(m_levelLayout);
    m_level->setReady();
    return std::move(m_level);
}

void RandomLevelFactory::growMaze(Vector2i start)
{
    // The start position can be turned into floor always (caller responsible for assuring this)
    newRegion(RegionType::Corridor);
    tileSet(start, BaseTileType::Floor);

    // Add the start tile to our stack and begin
    std::vector<Vector2i> cells;
    cells.push_back(start);

    while ( !cells.empty() )
    {
        auto currCell = cells.back();

        // For each cardinal direction from our current position, work out whether or not we can grow the maze
        // to that position
        std::vector<Direction> dirs;
        for ( auto &[k, v] : GridUtils::CardinalNeighbours )
        {
            if ( canFloor( currCell, k ))
            {
                dirs.push_back(k);
            }
        }

        // If we can grow the maze to any of the neighbors of our current cell, pick one at random and grow it.
        if ( !dirs.empty() )
        {
            auto dir = *m_level->random().randomElement(dirs);
            auto nextCell = currCell + (GridUtils::CardinalNeighbours[dir] * 2);

            tileSet( currCell + GridUtils::CardinalNeighbours[dir], BaseTileType::Floor);
            tileSet( nextCell, BaseTileType::Floor );

            cells.push_back( nextCell );
        }
        else
        {
            cells.pop_back();
        }
    }
}

void RandomLevelFactory::tileSet(Vector2i tile, BaseTileType ttype)
{
    m_levelLayout.tileTypes[indexFromCoords(tile)] = ttype;

    if ( ttype == BaseTileType::Floor )
    {
        m_regionMap[tile] = m_regionIndex;
    }
}

BaseTileType RandomLevelFactory::tileGet(Vector2i tile)
{
    return m_levelLayout.tileTypes[indexFromCoords(tile)];
}

int RandomLevelFactory::indexFromCoords(Vector2i coord)
{
    return coord.x() + (coord.y() * m_level->grid().bounds().x());
}

Vector2i RandomLevelFactory::coordsFromIndex(int idx)
{
    return {idx % m_level->grid().bounds().x(), idx / m_level->grid().bounds().x() };
}

bool RandomLevelFactory::canFloor(Vector2i coord, Direction dir)
{
    // Can the given position be grown in the given direction without hitting a room
    // or the edge of the map?

    auto delta = GridUtils::CardinalNeighbours[dir];

    if ( !m_level->grid().inBounds(coord + (delta * 3)) )
        return false;

    return tileGet(coord + (delta * 2)) == BaseTileType::Wall;
}

void RandomLevelFactory::addRooms(int maxTries )
{
    for ( int idx = 0; idx < maxTries; idx++ )
    {
        Vector2i roomSize = generateRandomRoomSize();
        Vector2i randomPosition = randomRoomPosition(roomSize);

        Room room;
        room.bounds = { randomPosition, roomSize };

        if ( !isRoomValid(room) )
        {
            continue;
        }

        newRegion(RegionType::Room);
        m_rooms.insert({m_regionIndex, room});

        for ( int j = room.bounds.y(); j < room.bounds.y() + room.bounds.h(); j++)
        {
            for ( int i = room.bounds.x(); i < room.bounds.x() + room.bounds.w(); i++ )
            {
                tileSet( {i, j}, BaseTileType::Floor );
            }
        }
    }
}

Vector2i RandomLevelFactory::generateRandomRoomSize()
{
    // From 3 to 7
    static const int MIN_ROOM_W = 1;
    static const int MIN_ROOM_H = 3;

    int rawW = m_level->random().randomInt( MIN_ROOM_W, MIN_ROOM_H );
    int rawH = m_level->random().randomInt( MIN_ROOM_W, MIN_ROOM_H );

    return { rawW * 2 + 1, rawH * 2 + 1 };
}

void RandomLevelFactory::fillAllMazes()
{
    // Walk over every tile in the level. For any tile which is still a wall, try to grow a
    // random maze of corridors at that position.

    for ( int y = 1; y < m_level->grid().bounds().y(); y += 2 )
    {
        for ( int x = 1; x < m_level->grid().bounds().x(); x += 2 )
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

void RandomLevelFactory::connectRooms()
{
    // Find every wall tile which is adjacent to two or more different regions.
    // Store the location of the tile against the regions it spans.
    std::unordered_map<Vector2i, Vector2i, Vector2Hash<int>> connectorMap;

    for ( int y = 1; y < m_level->grid().bounds().y() - 1; y++ )
    {
        for ( int x = 1; x < m_level->grid().bounds().x() - 1; x++ )
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
            for ( auto const& [dir, dir_vec] : GridUtils::CardinalNeighbours )
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
                auto p = Vector2i{ *regions.begin(), *(++regions.begin()) };
                connectorMap[pos] = p;
            }

        }
    }

    // Get all of our possible connector positions as a list
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
        auto rand_it = m_level->random().randomElement( allConnectors.begin(), allConnectors.end() );
        auto& rand_regions = connectorMap[*rand_it];

        // Create a new junction
        addJunction(
            *rand_it,
            rand_regions.x(),
            rand_regions.y()
        );

        // Find the regions which have been connected by this new connection
        std::vector<int> regions;

        regions.push_back(mergeMap[rand_regions.x()]);
        regions.push_back(mergeMap[rand_regions.y()]);

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

            // Remove all connectors adjacent to the door we just placed
            // All wall tiles are aligned to odd grid coordinates, so this will never leave a region stranded
            if ( GridUtils::isAdjacent(*rand_it, pos) )
            {
                return true;
            }

            auto cmp = connectorMap[pos];
            if ( mergeMap[cmp.x()] != mergeMap[cmp.y()] )
            {
                return false;
            }

            // A small percentage of the time, add another random door. This will help keep the network of rooms
            // and corridors connected rather than tree-ish
            if ( m_level->random().weightedFlip(30) )
            {
                addJunction( pos, cmp.x(), cmp.y() );
                return true;
            }

            return true;

        }), allConnectors.end() );

    }
}

void RandomLevelFactory::addJunction(Vector2i pos, RegionRef r1, RegionRef r2 )
{
    Junction jc;
    jc.pos = pos;
    jc.region1 = r1;
    jc.region2 = r2;

    if ( m_level->random().weightedFlip(5) )
    {
        tileSet( jc.pos, BaseTileType::Floor );
        jc.type = JunctionType::Open;
    }
    else
    {
        tileSet( jc.pos, BaseTileType::Junction );
        jc.type = JunctionType::Door;
    }

    m_junctions[jc.pos] = jc;

    auto it1 = m_rooms.find(jc.region1);
    if (it1 != m_rooms.end())
    {
        it1->second.junctions.push_back(jc.pos);
    }

    auto it2 = m_rooms.find(jc.region2);
    if (it2 != m_rooms.end())
    {
        it2->second.junctions.push_back(jc.pos);
    }

    auto it3 = m_regionToJunctions.find(jc.region1);
    if ( it3 == m_regionToJunctions.end() )
    {
        m_regionToJunctions[jc.region1] = { jc.pos };
    }
    else
    {
        m_regionToJunctions[jc.region1].push_back( jc.pos );
    }

    auto it4 = m_regionToJunctions.find(jc.region2);
    if ( it4 == m_regionToJunctions.end() )
    {
        m_regionToJunctions[jc.region2] = { jc.pos };
    }
    else
    {
        m_regionToJunctions[jc.region2].push_back( jc.pos );
    }
}

void RandomLevelFactory::removeJunction(Vector2i pos)
{
    auto jc_it = m_junctions.find( pos );
    AssertMsg(jc_it != m_junctions.end(), "deleting non-existant junction");

    tileSet( pos, BaseTileType::Wall );
    auto jc = jc_it->second;

    auto room_it1 = m_rooms.find(jc.region1);
    if ( room_it1 != m_rooms.end() )
    {
        auto& lst = room_it1->second.junctions;
        lst.erase( std::remove(lst.begin(), lst.end(), pos), lst.end() );
    }

    auto room_it2 = m_rooms.find(jc.region2);
    if ( room_it2 != m_rooms.end() )
    {
        auto& lst = room_it2->second.junctions;
        lst.erase( std::remove(lst.begin(), lst.end(), pos), lst.end() );
    }

    auto it3 = m_regionToJunctions.find(jc.region1);
    if ( it3 != m_regionToJunctions.end() )
    {
        auto& lst = it3->second;
        lst.erase( std::remove(lst.begin(), lst.end(), pos), lst.end() );
    }

    auto it4 = m_regionToJunctions.find(jc.region2);
    if ( it4 != m_regionToJunctions.end() )
    {
        auto& lst = it4->second;
        lst.erase( std::remove(lst.begin(), lst.end(), pos), lst.end() );
    }

    m_junctions.erase(jc_it);
}


void RandomLevelFactory::pruneCorridors()
{
    std::vector<LD::RegionRef> pointlessCorridors;

    for ( auto const& [k, v] : m_regionToJunctions )
    {
        if ( v.size() == 1 && m_regionTypeMap[k] == LD::RegionType::Corridor )
        {
            removeJunction( *v.begin() );
        }
    }

    bool finished = false;
    while (!finished)
    {
        finished = true;

        // Walk over each of our tiles
        for ( int y = 1; y < m_level->grid().bounds().y() - 1; y++ )
        {
            for ( int x = 1; x < m_level->grid().bounds().x() - 1; x++ )
            {
                auto pos = Vector2i{x, y};

                // We are only interested in floor tiles
                if ( tileGet(pos) == BaseTileType::Wall )
                {
                    continue;
                }

                // Count the number of exits from this tile, e.g. the number of non-wall tiles adjacent to it
                auto count = std::count_if( GridUtils::CardinalNeighbours.begin(), GridUtils::CardinalNeighbours.end(),
                [&]( auto& v ){
                    return tileGet(pos + v.second) == BaseTileType::Wall;
                });

                if (count != 3)
                {
                    continue;
                }
                else
                {
                    // If this tile has 3 walls around it then it is a dead end - remove the corridor section.
                    finished = false;
                    tileSet(pos, BaseTileType::Wall);
                }
            }
        }
    }
}

void RandomLevelFactory::newRegion(RegionType type)
{
    m_regionIndex++;
    m_regionTypeMap[m_regionIndex] = type;
}

void RandomLevelFactory::constructMapRendering(RandomLevelConfig const &config, LevelContextPtr const &ctx)
{
    addTileTypeRender(TerrainTile::Wall_T_N,          {"dawnlike_wall", "Wall_013"});
    addTileTypeRender(TerrainTile::Wall_T_S,          {"dawnlike_wall", "Wall_005"});
    addTileTypeRender(TerrainTile::Wall_T_E,          {"dawnlike_wall", "Wall_008"});
    addTileTypeRender(TerrainTile::Wall_T_W,          {"dawnlike_wall", "Wall_010"});
    addTileTypeRender(TerrainTile::Wall_Corner_NE,    {"dawnlike_wall", "Wall_003"});
    addTileTypeRender(TerrainTile::Wall_Corner_SE,    {"dawnlike_wall", "Wall_012"});
    addTileTypeRender(TerrainTile::Wall_Corner_SW,    {"dawnlike_wall", "Wall_011"});
    addTileTypeRender(TerrainTile::Wall_Corner_NW,    {"dawnlike_wall", "Wall_001"});
    addTileTypeRender(TerrainTile::Wall_Vertical,     {"dawnlike_wall", "Wall_006"});
    addTileTypeRender(TerrainTile::Wall_Horizontal,   {"dawnlike_wall", "Wall_002"});
    addTileTypeRender(TerrainTile::Wall_Cross,        {"dawnlike_wall", "Wall_009"});
    addTileTypeRender(TerrainTile::Wall_Center,       {"dawnlike_wall", "Wall_004"});
    addTileTypeRender(TerrainTile::Floor,             {"dawnlike_objects", "Floor_103"});

    m_levelLayout.mapData = std::vector<TileRef>( m_levelLayout.tileTypes.size(), -1 );

    for (size_t i = 0; i < m_levelLayout.tileTypes.size(); i++ )
    {
        TileRef tileIdx;

        switch ( m_levelLayout.tileTypes[i] )
        {
            case BaseTileType::Wall:
            case BaseTileType::Junction:
                tileIdx = m_tileRenderMap.at( getCorrectWallTile(i) );
                break;
            case BaseTileType::Floor:
                tileIdx = m_tileRenderMap.at( TerrainTile::Floor  );
                break;
            default:
                tileIdx = -1;
                AssertAlways();
        }

        m_levelLayout.mapData[i] = tileIdx;
    }
}

TerrainTile RandomLevelFactory::getCorrectWallTile(int idx)
{
    GridBitmask fullMask = m_wallPositionMasks[idx];
    GridBitmask mask = fullMask & GridUtils::CardinalOnly;

    if (mask == Direction::N )
    {
        return TerrainTile::Wall_Vertical;
    }
    else if ( mask == Direction::E )
    {
        return TerrainTile::Wall_Horizontal;
    }
    else if ( mask == Direction::S )
    {
        return TerrainTile::Wall_Vertical;
    }
    else if ( mask == Direction::W )
    {
        return TerrainTile::Wall_Horizontal;
    }
    else if ( mask == (Direction::N | Direction::S) )
    {
        return TerrainTile::Wall_Vertical;
    }
    else if ( mask == (Direction::E | Direction::W) )
    {
        return TerrainTile::Wall_Horizontal;
    }
    else if ( mask == (Direction::E | Direction::S) )
    {
        return TerrainTile::Wall_Corner_NW;
    }
    else if ( mask == (Direction::E | Direction::N) )
    {
        return TerrainTile::Wall_Corner_SW;
    }
    else if ( mask == (Direction::W | Direction::S) )
    {
        return TerrainTile::Wall_Corner_NE;
    }
    else if ( mask == (Direction::N | Direction::W) )
    {
        return TerrainTile::Wall_Corner_SE;
    }
    else if ( mask == (Direction::N | Direction::S | Direction::E ) )
    {
        if ( fullMask & (Direction::NE | Direction::SE) )
        {
            return TerrainTile::Wall_Vertical;
        }
        else
        {
            return TerrainTile::Wall_T_E;
        }
    }
    else if ( mask == (Direction::N | Direction::S | Direction::W ) )
    {
        if ( fullMask & (Direction::NW | Direction::SW) )
        {
            return TerrainTile::Wall_Vertical;
        }
        else
        {
            return TerrainTile::Wall_T_W;
        }
    }
    else if ( mask == (Direction::E | Direction::W | Direction::N ) )
    {
        if ( fullMask & (Direction::NE | Direction::NW) )
        {
            return TerrainTile::Wall_Horizontal;
        }
        else
        {
            return TerrainTile::Wall_T_N;
        }
    }
    else if ( mask == (Direction::E | Direction::W | Direction::S ) )
    {
        if ( fullMask & (Direction::SW | Direction::SE) )
        {
            return TerrainTile::Wall_Horizontal;
        }
        else
        {
            return TerrainTile::Wall_T_S;
        }
    }

    return TerrainTile::Wall_Center;
}

GridBitmask RandomLevelFactory::adjacentWalls(Vector2i coord)
{
    GridBitmask mask = 0;
    for ( auto const&[k, v] : GridUtils::AllNeighbours )
    {
        Vector2i pos = coord + v;

        if (m_level->grid().inBounds(pos) && (tileGet(pos) == BaseTileType::Wall
            || tileGet(pos) == BaseTileType::Junction))
        {
            mask |= k;
        }
    }
    return mask;
}

void RandomLevelFactory::calcAllAdjacentWalls()
{
    m_wallPositionMasks.clear();
    m_wallPositionMasks.reserve( m_level->grid().bounds().area() );

    for ( int j = 0; j < m_level->grid().bounds().y(); j++ )
    {
        for ( int i = 0; i < m_level->grid().bounds().x(); i++ )
        {
            m_wallPositionMasks.push_back( adjacentWalls({i, j}) );
        }
    }

    Assert(m_wallPositionMasks.size() == m_level->grid().bounds().area());
}

void RandomLevelFactory::constructParty(PartyData const& pdata)
{
    auto startPos = m_rooms.at( m_specialRooms.at( RoomType::Entrance ) ).centre();
    auto it = GridUtils::AllNeighbours.begin();
    for ( auto const& pc : pdata.playerChars )
    {
        auto pos = startPos + (it++)->second;
        m_level->entityFactory().createPlayer( pc.generateNewPlayer(), pos );
    }
}

void RandomLevelFactory::constructDoors()
{
    for ( auto const &[p, j] : m_junctions)
    {
        if ( j.type == JunctionType::Door )
        {
            m_level->entityFactory().createObject( "door", p );
        }
    }
}

void RandomLevelFactory::decorateRooms()
{
    for ( auto const& [ref, room] : m_rooms )
    {
        switch ( room.roomType )
        {
            case RoomType::Normal:
            {
                auto rsize = room.bounds.right();
                bool flip = rsize.x() > rsize.y();
                auto rt = ResourceDatabase::instance().randomRoomTemplate( rsize, m_level->random() );
                constructRoomFromTemplate(room, rt, flip);
                break;
            }
            case RoomType::Entrance:
            {
                auto eref = m_level->entityFactory().createObject( "level_entrance", room.centre() );
                break;
            }
            case RoomType::Exit:
            {
                auto eref = m_level->entityFactory().createObject( "level_exit", room.centre() );
                break;
            }
            case RoomType::Vault:
            {
                break;
            }
            case RoomType::Shop:
            {
                break;
            }
            case RoomType::Boss:
            {
                break;
            }

            default:
                AssertAlways();
        }
    }
}

void RandomLevelFactory::setInitialCollisionData()
{
    // Set the initial fixed collision data for the level
    m_level->grid().pass().disableCache();

    for (size_t i = 0; i < m_levelLayout.tileTypes.size(); i++ )
    {
        switch ( m_levelLayout.tileTypes[i] )
        {
            case LD::BaseTileType::Wall:
                m_level->grid().pass().setFixed( i, Passibility::Impassable );
                break;

            case LD::BaseTileType::Floor:
            case LD::BaseTileType::Junction:
                m_level->grid().pass().setFixed( i, Passibility::Passable );
                break;
        }
    }

    m_level->grid().pass().enableCache();
}

void RandomLevelFactory::assignSpecialRooms()
{
    std::unordered_set<RegionRef> toDecorate;
    std::unordered_set<RegionRef> terminalRooms;
    for ( auto& [rref, room] : m_rooms )
    {
        toDecorate.insert(rref);
        if (room.junctions.size() == 1 )
        {
            terminalRooms.insert(rref);
        }
    }

    // AssertMsg( terminalRooms.size() > 2, "Expected more than two terminal rooms" );

    const std::vector<RoomType> specialOrder = {
        RoomType::Entrance, RoomType::Exit,
        RoomType::Boss, RoomType::Shop,
        RoomType::Vault
    };

    for ( auto srt : specialOrder )
    {
        if ( terminalRooms.empty() )
            break;

        auto it = m_level->random().randomElement( terminalRooms.begin(), terminalRooms.end() );
        m_specialRooms.emplace( srt, *it );
        m_rooms.at(*it).roomType = srt;
        terminalRooms.erase(it);
    }
}

void RandomLevelFactory::constructRoomFromTemplate(LD::Room const& room, RawRoomTemplateData const& rt, bool flip)
{
    // Work out whether or not we can use square symmetry

    Matrix2i matrixTransform;
    if ( room.bounds.w() == room.bounds.h() )
    {
        matrixTransform = *m_level->random().randomElement(
                MatrixTransform::squareTransforms.begin(),
                MatrixTransform::squareTransforms.end()
        );
    }
    else
    {
        matrixTransform = *m_level->random().randomElement(
                MatrixTransform::rectangularTransforms.begin(),
                MatrixTransform::rectangularTransforms.end()
        );
    }

    auto centre = room.centre() - room.bounds.left();

    for ( auto const& objs : rt.objects )
    {
        // Move origin to the centre of the room
        Vector2i translated = objs.offset - centre;

        // Apply the transformation(s)
        translated = matrixTransform.transform(translated);
        translated = translated + centre + room.bounds.left();

        // If the transformed vector is adjacent to a door, skip it
        bool doorAdj = false;
        for ( auto const& door : room.junctions )
        {
            if ( GridUtils::isAdjacent( translated, door ))
            {
                doorAdj = true;
                break;
            }
        }

        if ( doorAdj )
        {
            continue;
        }

        // Otherwise construct the new object
        m_level->entityFactory().createObject( objs.name, translated );
    }
}


void RandomLevelFactory::placeSpecialRooms()
{

}

Vector2i RandomLevelFactory::randomRoomPosition(Vector2i roomSize)
{
    int randX = m_level->random().randomInt( 1, m_level->grid().bounds().x() - roomSize.x() - 1 ) / 2;
    int randY = m_level->random().randomInt( 1, m_level->grid().bounds().y() - roomSize.y() - 1 ) / 2;

    randX = randX * 2 + 1;
    randY = randY * 2 + 1;

    return { randX, randY };
}

bool RandomLevelFactory::isRoomValid(const LD::Room &room)
{
    for ( auto const &[k, v] : m_rooms )
    {
        if ( room.bounds.intersect(v.bounds) )
        {
            return false;
        }
    }

    return true;
}

void RandomLevelFactory::addTileTypeRender(TerrainTile tt, SpritesheetKey sprite)
{
   TileRef idx = m_levelLayout.tileset.addTile(sprite);
   m_tileRenderMap.emplace(tt, idx);
}
