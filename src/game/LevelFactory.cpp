#include <game/LevelFactory.h>

#include <random>
#include <bitset>

#include <utils/Logging.h>
#include <utils/Random.h>
#include <utils/Assert.h>

// With help from https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/

using namespace LD;

LevelFactory::LevelFactory()
    : m_rd(), m_mt( m_rd() ), m_regionIndex(0)
{
}

LevelPtr LevelFactory::create(LevelConfig const &config, LevelContextPtr const &ctx)
{

    m_level = std::make_unique<Level>( config.size, ctx );

    /* LAYOUT CREATION  */

    // Start off with a map full of walls
    m_level->m_baseTilemap = std::vector<BaseTileType>( config.size.x() * config.size.y(), BaseTileType::Wall );

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

    calcAllAdjacentWalls();
    generateEntrancesExits();

    setInitialCollisionData();

    constructMapRendering(config, ctx);

    /* CONTENT CREATION */
    m_level->m_entFactory.loadAllPrefabs( "../resource/data/entities.json" );
    m_roomTemplates.loadAllTemplates( "../resource/data/room_templates.json" );

    constructPlayer();
    constructDoors();

    decorateRooms();

    readyAllEntities();

    return std::move(m_level);
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

        std::vector<Direction> dirs;
        for ( auto &[k, v] : GridUtils::CardinalNeighbours )
        {
            if ( canFloor( currCell, k ))
            {
                dirs.push_back(k);
            }
        }

        if (!dirs.empty())
        {
            auto dir = *randomElement(dirs.begin(), dirs.end(), m_mt);
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

void LevelFactory::tileSet(Vector2i tile, BaseTileType ttype)
{
    m_level->m_baseTilemap[indexFromCoords(tile)] = ttype;

    if ( ttype == BaseTileType::Floor )
    {
        m_regionMap[tile] = m_regionIndex;
    }
}

BaseTileType LevelFactory::tileGet(Vector2i tile)
{
    return m_level->m_baseTilemap[indexFromCoords(tile)];
}

int LevelFactory::indexFromCoords(Vector2i coord)
{
    return coord.x() + (coord.y() * m_level->m_bounds.x());
}

Vector2i LevelFactory::coordsFromIndex(int idx)
{
    return {idx % m_level->m_bounds.x(), idx / m_level->m_bounds.x() };
}

bool LevelFactory::canFloor(Vector2i coord, Direction dir)
{
    auto delta = GridUtils::CardinalNeighbours[dir];

    if ( !m_level->m_grid.inBounds(coord + (delta * 3)) )
        return false;

    return tileGet(coord + (delta * 2)) == BaseTileType::Wall;
}

void LevelFactory::addRooms( int maxTries )
{
    for ( int idx = 0; idx < maxTries; idx++ )
    {
        auto roomSize = generateRandomRoomSize();

        std::uniform_int_distribution<> mtRoomX(1, m_level->m_bounds.x() - roomSize.x() - 1);
        std::uniform_int_distribution<> mtRoomY(1, m_level->m_bounds.y() - roomSize.y() - 1);

        int x = (mtRoomX(m_mt) / 2) * 2 + 1;
        int y = (mtRoomY(m_mt) / 2) * 2 + 1;

        Room room;
        room.bounds = { x, y, roomSize.x(), roomSize.y() };

        bool invalid = false;
        for ( auto const &[k, v] : m_rooms )
        {
            if ( room.bounds.intersect(v.bounds) )
            {
                invalid = true;
                break;
            }
        }

        if (invalid)
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
    for ( int y = 1; y < m_level->m_bounds.y(); y += 2 )
    {
        for ( int x = 1; x < m_level->m_bounds.x(); x += 2 )
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
    std::unordered_map<Vector2i, Vector2i, Vector2Hash<int>> connectorMap;

    for ( int y = 1; y < m_level->m_bounds.y() - 1; y++ )
    {
        for ( int x = 1; x < m_level->m_bounds.x() - 1; x++ )
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
        auto rand_it = randomElement(allConnectors.begin(), allConnectors.end(), m_mt);
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
            if ( weightedFlip(30, m_mt) )
            {
                addJunction( pos, cmp.x(), cmp.y() );
                return true;
            }

            return true;

        }), allConnectors.end() );

    }
}

void LevelFactory::addJunction( Vector2i pos, RegionRef r1, RegionRef r2 )
{
    Junction jc;
    jc.pos = pos;
    jc.region1 = r1;
    jc.region2 = r2;

    if ( weightedFlip(5, m_mt) )
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

void LevelFactory::removeJunction(Vector2i pos)
{
    auto jc_it = m_junctions.find( pos );
    AssertMsg( jc_it != m_junctions.end(), "deleting non-existant junction" );

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


void LevelFactory::pruneCorridors()
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
        for ( int y = 1; y < m_level->m_bounds.y() - 1; y++ )
        {
            for ( int x = 1; x < m_level->m_bounds.x() - 1; x++ )
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

void LevelFactory::newRegion(RegionType type)
{
    m_regionIndex++;
    m_regionTypeMap[m_regionIndex] = type;
}

void LevelFactory::constructMapRendering(LevelConfig const &config, LevelContextPtr const &ctx)
{
    // Add some tiles to the tilemap - this is debug for now
    auto floorRef = m_level->m_renderTileMap.addTile({"kenney-tiles", "soil-1"}, true);
    auto enterRef = m_level->m_renderTileMap.addTile({"kenney-tiles", "grey-stairs-up"}, true);
    auto exitRef = m_level->m_renderTileMap.addTile({"kenney-tiles", "grey-stairs-down"}, true);

    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-open-N"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-open-S"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-open-E"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-open-W"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-T-N"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-T-S"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-T-E"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-T-W"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-corner-NE"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-corner-SE"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-corner-SW"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-corner-NW"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-vert"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-horiz"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-plain"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-closed-N"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-closed-E"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-closed-S"}, false);
    m_level->m_renderTileMap.addTile({"kenney-tiles", "wall-grey-closed-W"}, false);

    // Construct the different tile levels - add two for now
    m_level->m_mapRendering.emplace_back( m_level->m_baseTilemap.size(), -1 );
    m_level->m_mapRendering.emplace_back( m_level->m_baseTilemap.size(), -1 );

    for ( size_t i = 0; i < m_level->m_baseTilemap.size(); i++ )
    {
        switch ( m_level->m_baseTilemap[i] )
        {
            case BaseTileType::Wall:
                m_level->m_mapRendering[0][i] = getCorrectWallTile(i);
                break;
            case BaseTileType::Floor:
                m_level->m_mapRendering[0][i] = floorRef;
                break;
            case BaseTileType::Junction:
                m_level->m_mapRendering[0][i] = getCorrectWallTile(i);
                break;
            case BaseTileType::Entrance:
                m_level->m_mapRendering[0][i] = floorRef;
                m_level->m_mapRendering[1][i] = enterRef;
                break;
            case BaseTileType::Exit:
                m_level->m_mapRendering[0][i] = floorRef;
                m_level->m_mapRendering[1][i] = exitRef;
                break;
            default:
                break;
        }
    }
}

TileRef LevelFactory::getCorrectWallTile(int idx)
{

    GridBitmask fullMask = m_wallPositionMasks[idx];
    GridBitmask mask = fullMask & GridUtils::CardinalOnly;

    if (mask == Direction::N)
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-open-N" );
    }
    else if ( mask == Direction::E )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-open-E" );
    }
    else if ( mask == Direction::S )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-open-S" );
    }
    else if ( mask == Direction::W )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-open-W" );
    }
    else if ( mask == (Direction::N | Direction::S) )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-vert" );
    }
    else if ( mask == (Direction::E | Direction::W) )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-horiz" );
    }
    else if ( mask == (Direction::E | Direction::S) )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-corner-NW" );
    }
    else if ( mask == (Direction::E | Direction::N) )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-corner-SW" );
    }
    else if ( mask == (Direction::W | Direction::S) )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-corner-NE" );
    }
    else if ( mask == (Direction::N | Direction::W) )
    {
        return m_level->m_renderTileMap.getRef( "wall-grey-corner-SE" );
    }
    else if ( mask == (Direction::N | Direction::S | Direction::E ) )
    {
        if ( fullMask & (Direction::NE | Direction::SE) )
        {
            return m_level->m_renderTileMap.getRef( "wall-grey-closed-W" );
        }
        else
        {
            return m_level->m_renderTileMap.getRef( "wall-grey-T-E" );
        }
    }
    else if ( mask == (Direction::N | Direction::S | Direction::W ) )
    {
        if ( fullMask & (Direction::NW | Direction::SW) )
        {
            return m_level->m_renderTileMap.getRef( "wall-grey-closed-E" );
        }
        else
        {
            return m_level->m_renderTileMap.getRef( "wall-grey-T-W" );
        }
    }
    else if ( mask == (Direction::E | Direction::W | Direction::N ) )
    {
        if ( fullMask & (Direction::NE | Direction::NW) )
        {
            return m_level->m_renderTileMap.getRef( "wall-grey-closed-S" );
        }
        else
        {
            return m_level->m_renderTileMap.getRef( "wall-grey-T-N" );
        }
    }
    else if ( mask == (Direction::E | Direction::W | Direction::S ) )
    {
        if ( fullMask & (Direction::SW | Direction::SE) )
        {
            return m_level->m_renderTileMap.getRef( "wall-grey-closed-N" );
        }
        else
        {
            return m_level->m_renderTileMap.getRef( "wall-grey-T-S" );
        }
    }


    return m_level->m_renderTileMap.getRef( "wall-grey-plain" );
}

GridBitmask LevelFactory::adjacentWalls(Vector2i coord)
{
    GridBitmask mask = 0;
    for ( auto const&[k, v] : GridUtils::AllNeighbours )
    {
        Vector2i pos = coord + v;

        if (m_level->m_grid.inBounds(pos) && (tileGet(pos) == BaseTileType::Wall
            || tileGet(pos) == BaseTileType::Junction))
        {
            mask |= k;
        }
    }
    return mask;
}

void LevelFactory::calcAllAdjacentWalls()
{
    m_wallPositionMasks.clear();
    m_wallPositionMasks.reserve( m_level->m_tileCount );

    for ( int j = 0; j < m_level->m_bounds.y(); j++ )
    {
        for ( int i = 0; i < m_level->m_bounds.x(); i++ )
        {
            m_wallPositionMasks.push_back( adjacentWalls({i, j}) );
        }
    }

    Assert(m_wallPositionMasks.size() == m_level->m_tileCount);
}

void LevelFactory::generateEntrancesExits()
{
    std::vector<int> candidates;
    int rIdxEnter = -1;
    int rIdxExit = -1;

    std::for_each(m_rooms.begin(), m_rooms.end(), [&]( auto const& it ) {
        if (it.second.junctions.size() == 1 )
        {
            candidates.push_back(it.first);
        }
    });

    if ( candidates.size() > 1 )
    {
        auto it = randomElement( candidates.begin(), candidates.end(), m_mt );
        rIdxEnter = *it;
        candidates.erase(it);

        it = randomElement( candidates.begin(), candidates.end(), m_mt );
        rIdxExit = *it;
    }
    else
    {
        AssertAlways();
    }

    auto& entranceBounds = m_rooms[rIdxEnter].bounds;
    auto& exitBounds = m_rooms[rIdxExit].bounds;

    int x, y;

    x = entranceBounds.x() + ( entranceBounds.w() / 2 );
    y = entranceBounds.y() + ( entranceBounds.h() / 2 );
    m_level->m_entranceTile = {x, y};
    tileSet( m_level->m_entranceTile, BaseTileType::Entrance );

    x = exitBounds.x() + ( exitBounds.w() / 2 );
    y = exitBounds.y() + ( exitBounds.h() / 2 );
    m_level->m_exitTile = {x, y};
    tileSet( m_level->m_exitTile, BaseTileType::Exit );
}

void LevelFactory::constructPlayer()
{
    ImPlayerData impData;
    impData.name = "Urist McUrist";

    m_level->setPlayer( m_level->m_entFactory.createPlayer( impData, m_level->m_entranceTile ) );
    m_createdEntities.push_back( m_level->m_player->ref() );
}

void LevelFactory::constructDoors()
{
    for ( auto const &[p, j] : m_junctions)
    {
        if ( j.type == JunctionType::Door )
        {
            auto ref = m_level->m_entFactory.createDoor( p );
            m_createdEntities.push_back(ref);
        }
    }
}

void LevelFactory::decorateRooms()
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

    
    // Create entrance + exit rooms

    // Create shop room

    // Create boss room

    // Create miniboss rooms

    // Create trap rooms

    // Create vault rooms



    // Create regular rooms

}

void LevelFactory::readyAllEntities()
{
    for ( auto const& e: m_createdEntities )
    {
        m_level->entityReady(e);
    }
}

void LevelFactory::setInitialCollisionData()
{
    // Set the initial fixed collision data for the level
    m_level->grid().pass().disableCache();

    for ( size_t i = 0; i < m_level->m_baseTilemap.size(); i++ )
    {
        switch ( m_level->m_baseTilemap[i] )
        {
            case LD::BaseTileType::Entrance:
            case LD::BaseTileType::Wall:
                m_level->grid().pass().setFixed( i, Rules::Passibility::Impassable );
                break;

            case LD::BaseTileType::Exit:
            case LD::BaseTileType::Floor:
            case LD::BaseTileType::Junction:
                m_level->grid().pass().setFixed( i, Rules::Passibility::Passable );
                break;
        }
    }

    m_level->grid().pass().enableCache();
}
