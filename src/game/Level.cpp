#include <game/Level.h>

#include <components/All.h>
#include <engine/InputInterface.h>
#include <engine/LuaState.h>
#include <graphics/RenderInterface.h>
#include <game/GameEventDefs.h>
#include <resource/ResourceManager.h>
#include <utils/GlobalConfig.h>
#include <utils/Math.h>


Level::Level(LevelInitData& data, LuaState& lua, RandomState* randomState)
: m_lua(lua),
  m_grid(data.size),
  m_randomState(randomState),
  m_camera( data.size * GlobalConfig::TileSizePx ),
  m_ecs(this),
  m_currentRound(0),
  m_isPlayerTurn(true),
  m_tileRenderDirtyBit(true),
  m_exitStatus(LevelExitStatus::None),
  m_gevents(&m_animationQueue)
{
    layoutWindows();
}

void Level::setLuaType(LuaState &lua)
{
    auto levelType = lua.state().new_usertype<Level>("Level");
    levelType["centerCamera"] = &Level::centerCameraOnParty;
}

void Level::setReady()
{
    m_tileRenderObj = generateTileRenderData();
    loadScripts();

    m_gevents.broadcast<GameEvents::LevelReady>();
    centerCameraOnParty();
}

void Level::loadScripts()
{
    m_lua.state()["level"] = this;
    m_lua.runLoadedScript( "level/Level" );
}

bool Level::input(IEvent &evt)
{
    switch (evt.type)
    {
        case IEventType::WindowResize:
            layoutWindows();
            break;
        default:
            break;
    }
    
    return false;
}


void Level::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    // Specify which camera we are using
    rInter.setCameraOffset( m_camera.getOffset() );

    // Render our tiles
    if (m_tileRenderDirtyBit)
    {
        rInter.releaseRenderQueue(RenderLayer::Tiles);
        rInter.addItem( m_tileRenderObj, RenderLayer::Tiles );
        rInter.holdRenderQueue(RenderLayer::Tiles);
        m_tileRenderDirtyBit = false;
    }

    // Advance our animation state if necessary
    m_animationQueue.update(ticks, iinter, rInter);

    // Update + render our entities
    m_ecs.update(ticks, iinter, rInter);

    // Poll our game event queue
    m_gevents.pollAllEvents();
}

RenderObject Level::generateTileRenderData()
{
    RenderObject obj { m_renderTileMap.getTexture()->handle() };
    Vector2i offset;
    Vector2i currPos;
    int row = 0;
    int col = 0;
    int width = m_grid.bounds().x();
    
    offset = {0, 0};
    
    for ( auto const& ref : m_mapRendering )
    {
        if ( ref >= 0 )
        {
            currPos = offset + Vector2i{ col * GlobalConfig::TileSizePx, row * GlobalConfig::TileSizePx };
            obj.merge( m_renderTileMap.get(ref).sprite.renderObject(currPos) );
        }
        
        col++;
        if ( col >= width )
        {
            col = 0;
            row++;
        }
    }
    
    return obj;
}

GameEventHub &Level::events()
{
    return m_gevents;
}

Grid& Level::grid( )
{
    return m_grid;
}

Camera &Level::camera()
{
    return m_camera;
}

ECS &Level::ecs()
{
    return m_ecs;
}

Vector2i Level::screenCoordsToWorld(Vector2i const &screen) const
{
    return m_camera.screenToWorld(screen);
}

Vector2i Level::worldCoordsToScreen(Vector2i const &world) const
{
    return m_camera.worldToScreen(world);
}

Vector2i Level::worldCoordsToTile(Vector2i const& world) const
{
    return {
            world.x() / GlobalConfig::TileSizePx,
            world.y() / GlobalConfig::TileSizePx
    };
}

Vector2i Level::screenCoordsToTile(Vector2i const &screen) const
{
    auto world = screenCoordsToWorld(screen);
    return worldCoordsToTile(world);
}

Vector2i Level::tileCoordsToScreen( Vector2i const& tile ) const
{
    auto world = tile * GlobalConfig::TileSizePx;
    return worldCoordsToScreen(world);
}

Vector2i Level::tileCoordsToWorld( Vector2i const &tile ) const
{
    return tile * GlobalConfig::TileSizePx;
}

Vector2i Level::tileIdxToWorld( int idx ) const
{
    return tileCoordsToWorld( m_grid.idxToPos(idx) );
}

void Level::layoutWindows()
{
    auto wndSize = ResourceManager::get().getWindow()->getSize();
    m_camera.setViewportSize(wndSize);
}


int Level::squaredEntityDistance(EntityRef a, EntityRef b)
{
    auto transformA = m_ecs.getComponents<PositionComponent>(a);
    auto transformB = m_ecs.getComponents<PositionComponent>(b);

    Vector2i distance = transformB->tilePosition - transformA->tilePosition;
    return distance.x() * distance.x() + distance.y() * distance.y();
}

float Level::entityDistance(EntityRef a, EntityRef b)
{
    return (float) std::sqrt( squaredEntityDistance(a, b) );
}

std::string Level::getDescriptionForEnt(EntityRef ent)
{
    if ( m_ecs.entityHas<ActorComponent>(ent) )
    {
        auto comp = m_ecs.getComponents<ActorComponent>(ent);
        return comp->actor.getName();
    }
    else if ( m_ecs.entityHas<ItemComponent>(ent) )
    {
        auto comp = m_ecs.getComponents<ItemComponent>(ent);
        return comp->item->getName();
    }

    return "<unknown>";
}

std::string Level::getDescriptionForItem(ItemPtr item)
{
    return item->getName();
}

RandomState* Level::random()
{
    return m_randomState;
}

void Level::setLayout(LevelLayout const& llayout)
{
    m_baseTilemap = llayout.tileTypes;
    m_renderTileMap = llayout.tileset;
    m_mapRendering = llayout.mapData;
}


void Level::centerCameraOnParty()
{
    std::vector<Vector2i> points;
    auto const& positions = ecs().entitiesWith<PositionComponent, ActorComponent>();

    for (auto const& [pos, actor] : positions )
    {
        if (actor->actorType == ActorType::PC)
        {
            points.push_back( pos->tilePosition );
        }
    }

    Vector2f centre = Math::findCentroid( points );
    m_camera.centreOnTile( centre.convert<int>() );
}

bool Level::isPlayerTurn() const
{
    return m_isPlayerTurn;
}

int Level::getCurrentRound() const
{
    return m_currentRound;
}

void Level::switchTurn()
{
    m_isPlayerTurn = !m_isPlayerTurn;
}

void Level::advanceRound()
{
    m_currentRound++;
}

LevelExitStatus Level::getExitStatus() const
{
    return m_exitStatus;
}

void Level::setExitStatus( LevelExitStatus status )
{
    m_exitStatus = status;
}

AnimationQueue& Level::animation()
{
    return m_animationQueue;
}

int Level::partyRemainingCount()
{
    int count = 0;
    for ( auto const& [c] : m_ecs.entitiesWith<ActorComponent>() )
    {
        if (c->actorType == ActorType::PC)
        {
            count++;
        }
    }

    return count;
}

int Level::enemiesRemainingCount()
{
    int count = 0;
    for ( auto const& [c] : m_ecs.entitiesWith<ActorComponent>() )
    {
        if (c->actorType != ActorType::PC)
        {
            count++;
        }
    }

    return count;
}

