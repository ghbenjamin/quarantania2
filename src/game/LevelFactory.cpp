#include <game/LevelFactory.h>

#include <random>
#include <bitset>

#include <utils/Logging.h>
#include <utils/Random.h>
#include <utils/Assert.h>
#include <db/ResourceDatabase.h>

// With help from https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/

using namespace LD;

LevelFactory::LevelFactory()
    : m_rd(), m_regionIndex(0)
{
}

LevelPtr LevelFactory::create(LevelConfig const &config, LevelContextPtr const &ctx)
{
    m_level = std::make_unique<Level>( config.size, ctx, RandomGenerator{ m_rd() } );

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

    // Walk over every tile and work out which of its 8 neighbours are wall tiles, if any
    calcAllAdjacentWalls();

    // Fix our wall tiles as impassable
    setInitialCollisionData();

    // Construct a map of sprites for the prebaked tiles we are going to need
    constructMapRendering(config, ctx);

    // Load the prefab entity and room data
    m_roomTemplates.loadAllTemplates( "../resource/data/room_templates.json" );

    // Construct basic door prefabs at most of our junction points
    constructDoors();

    // Semi-randomly assign our special rooms
    assignSpecialRooms();

    // Fill each of our rooms with entities
    decorateRooms();

    // Add the player at an appropriate place
    constructPlayer();

    // Mark the level construction as completed
    m_level->setReady();
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
            auto dir = *randomElement(dirs.begin(), dirs.end(), m_level->m_rg);
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

        int x = (mtRoomX(m_level->m_rg) / 2) * 2 + 1;
        int y = (mtRoomY(m_level->m_rg) / 2) * 2 + 1;

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
    auto primary = roomSizing(m_level->m_rg) * 2 + 1;

    std::uniform_int_distribution<> secSizing(primary - 2, primary);
    auto secondary = secSizing(m_level->m_rg) * 2 + 1;

    if ( coinflip(m_level->m_rg) )
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
        auto rand_it = randomElement(allConnectors.begin(), allConnectors.end(), m_level->m_rg);
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
            if ( weightedFlip(30, m_level->m_rg) )
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

    if ( weightedFlip(5, m_level->m_rg) )
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
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_T_N,  {"dawnlike_wall", "Wall_013"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_T_S,  {"dawnlike_wall", "Wall_005"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_T_E,   {"dawnlike_wall", "Wall_008"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_T_W,  {"dawnlike_wall", "Wall_010"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_Corner_NE, {"dawnlike_wall", "Wall_003"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_Corner_SE, {"dawnlike_wall", "Wall_012"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_Corner_SW, {"dawnlike_wall", "Wall_011"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_Corner_NW, {"dawnlike_wall", "Wall_001"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_Vertical, {"dawnlike_wall", "Wall_006"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_Horizontal, {"dawnlike_wall", "Wall_002"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_Cross,  {"dawnlike_wall", "Wall_009"});
    m_level->m_renderTileMap.addTile(TerrainTile::Wall_Center, {"dawnlike_wall", "Wall_004"});
    m_level->m_renderTileMap.addTile(TerrainTile::Floor, {"dawnlike_objects", "Floor_103"});

    m_level->m_mapRendering = std::vector<TileRef>( m_level->m_baseTilemap.size(), -1 );

    for ( size_t i = 0; i < m_level->m_baseTilemap.size(); i++ )
    {
        switch ( m_level->m_baseTilemap[i] )
        {
            case BaseTileType::Wall:
                m_level->m_mapRendering[i] = m_level->m_renderTileMap.getRef(getCorrectWallTile(i) );
                break;
            case BaseTileType::Floor:
                m_level->m_mapRendering[i] = m_level->m_renderTileMap.getRef( TerrainTile::Floor );
                break;
            case BaseTileType::Junction:
                m_level->m_mapRendering[i] = m_level->m_renderTileMap.getRef(getCorrectWallTile(i) );
                break;
            default:
                AssertAlways();
        }
    }
}

TerrainTile LevelFactory::getCorrectWallTile(int idx)
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

void LevelFactory::constructPlayer()
{
    ImPlayerData impData;
    impData.name = "Urist McUrist";

    auto startPos = m_rooms.at( m_specialRooms.at( RoomType::Entrance ) ).centre();

    m_level->setPlayer( m_level->m_entFactory.createPlayer( impData, startPos ) );

    // DEBUG
//    m_level->m_entFactory.createEnemy("Dire Rat", startPos + Vector2{0, 1});
    m_level->m_entFactory.createItem("Battleaxe", startPos + Vector2{1, 0});
    m_level->m_entFactory.createItem("Amulet of Natural Armor", startPos + Vector2{0, 1});
//    m_level->m_entFactory.createItem("Dagger", startPos + Vector2{-1, 0});
//    m_level->m_entFactory.createItem("Sickle", startPos + Vector2{0, -1});

    m_createdEntities.push_back( m_level->m_player->ref() );
}

void LevelFactory::constructDoors()
{
    for ( auto const &[p, j] : m_junctions)
    {
        if ( j.type == JunctionType::Door )
        {
            auto ref = m_level->m_entFactory
                              .createPrefab(PrefabType::Door, p);
            m_createdEntities.push_back(ref);
        }
    }
}

void LevelFactory::decorateRooms()
{
    for ( auto const& [ref, room] : m_rooms )
    {
        switch ( room.roomType )
        {
            case RoomType::Normal:
            {
                auto rt = m_roomTemplates.randomTemplate( room.bounds.right(), m_level->m_rg );
                if ( rt != nullptr )
                {
                    constructRoomFromTemplate(room, rt);
                }

                break;
            }
            case RoomType::Entrance:
            {
                auto eref = m_level->m_entFactory
                                   .createPrefab(PrefabType::Stairs_Up, room.centre());
                m_createdEntities.push_back(eref);

                break;
            }
            case RoomType::Exit:
            {
                auto eref = m_level->m_entFactory
                                   .createPrefab(PrefabType::Stairs_Down, room.centre());
                m_createdEntities.push_back(eref);
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

void LevelFactory::setInitialCollisionData()
{
    // Set the initial fixed collision data for the level
    m_level->grid().pass().disableCache();

    for ( size_t i = 0; i < m_level->m_baseTilemap.size(); i++ )
    {
        switch ( m_level->m_baseTilemap[i] )
        {
            case LD::BaseTileType::Wall:
                m_level->grid().pass().setFixed( i, Rules::Passibility::Impassable );
                break;

            case LD::BaseTileType::Floor:
            case LD::BaseTileType::Junction:
                m_level->grid().pass().setFixed( i, Rules::Passibility::Passable );
                break;
        }
    }

    m_level->grid().pass().enableCache();
}

void LevelFactory::assignSpecialRooms()
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

        auto it = randomElement( terminalRooms.begin(), terminalRooms.end(), m_level->m_rg );
        m_specialRooms.emplace( srt, *it );
        m_rooms.at(*it).roomType = srt;
        terminalRooms.erase(it);
    }
}

void LevelFactory::constructRoomFromTemplate(LD::Room const& room, RoomTemplate* rt)
{
    // Construct prefabs
    for ( auto const& prefab : rt->prefabs )
    {
        Vector2i translated = prefab.position + room.bounds.left();
        m_level->m_entFactory
               .createPrefab(prefab.type, translated);
    }
}
