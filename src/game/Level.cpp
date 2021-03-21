#include <utility>

#include <components/All.h>
#include <engine/InputInterface.h>
#include <game/GameEventDefs.h>
#include <game/Level.h>
#include <game/ActionDefs.h>
#include <graphics/RenderInterface.h>
#include <resource/ResourceManager.h>
#include <utils/GlobalConfig.h>
#include <utils/Math.h>
#include <ui/level/TextLog.h>
#include <ui/level/PlayerStatusView.h>
#include <ui/level/CreatureEquipView.h>
#include <ui/level/ContainerView.h>
#include <ui/level/Composites.h>


Level::Level(Vector2i size, LevelContextPtr ctx, RandomGenerator const& rg)
: m_ctx(std::move(ctx)),
  m_grid(size),
  m_random(rg),
  m_isComplete(false),
  m_camera( size * GlobalConfig::TileSizePx ),
  m_ecs(this),
  m_controllers{ std::make_shared<DefaultLController>(this) },
  m_uiManager(this),
  m_currentRound(0),
  m_isPlayerTurn(true),
  m_canInteract(true),
  m_tileRenderDirtyBit(true)
{
    setupUI();
    layoutWindows();
}

void Level::setReady()
{
    m_tileRenderObj = generateTileRenderData();
    m_gevents.broadcast<GameEvents::LevelReady>();
    centerCameraOnParty();
}

bool Level::input(IEvent &evt)
{
    if ( m_uiManager.input(evt) )
    {
        return true;
    }

    switch (evt.type)
    {
        case IEventType::WindowResize:
            layoutWindows();
            break;
        default:
            break;
    }

    return m_controllers.back()->input(evt);
}

void Level::render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    if (m_tileRenderDirtyBit)
    {
        rInter.releaseRenderQueue(RenderLayer::Tiles);
        renderTiles(ticks, rInter);
        rInter.holdRenderQueue(RenderLayer::Tiles);
        m_tileRenderDirtyBit = false;
    }
}

void Level::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    // Specify which camera we are using
    rInter.setCameraOffset( m_camera.getOffset() );

    // Handle controller changes if necessary
    
    if ( m_controllers.back()->hasNextController() )
    {
        auto next = m_controllers.back()->getNextController();
        bool shouldPop = m_controllers.back()->shouldPopController();
        
        m_controllers.back()->onExit();
        
        if (shouldPop)
        {
            m_controllers.pop_back();
        }
        
        m_controllers.push_back( next );
        m_controllers.back()->onEnter();
    }
    
    else if ( m_controllers.back()->shouldPopController() )
    {
        m_controllers.back()->onExit();
        m_controllers.pop_back();
        m_controllers.back()->onEnter();
    }
    

    m_controllers.back()->update(ticks, iinter, rInter);

    // Render statics: tiles, etc.
    render(ticks, iinter, rInter);

    // Update + render our entities
    m_ecs.update(ticks, iinter, rInter);

    // Render the GUI
    m_uiManager.update(ticks, iinter, rInter);
}

void Level::renderTiles(uint32_t ticks, RenderInterface &rInter)
{
    rInter.addItem( m_tileRenderObj, RenderLayer::Tiles );
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
    return {
            tile.x() * GlobalConfig::TileSizePx,
            tile.y() * GlobalConfig::TileSizePx
    };
}



void Level::setupUI()
{
    // Fixed UI Elements

    // Widget containing the current party and information
    auto turnOrderContainer = m_uiManager.createElement<UI::PlayerStatusContainer>(nullptr);
    m_uiManager.alignElementToWindow( turnOrderContainer, UI::Alignment::TopLeft, {20, 20} );

    // Widget containing icons representing actions which can be taken
//    auto actionMenu = m_uiManager.createElement<UI::ActionMenu>(nullptr);
    auto actionMenu = m_uiManager.createElement<UI::BottomLeftBar>(nullptr);
    m_uiManager.alignElementToWindow( actionMenu, UI::Alignment::BottomLeft, {20, -20} );

    // Widget containing the global text log
    auto textLog = m_uiManager.createElement<UI::MainTextLog>(nullptr);
    m_uiManager.alignElementToWindow( textLog, UI::Alignment::BottomRight, {-20, -20} );


    // Default hidden elements

    auto equipUi = m_uiManager.createElement<UI::EquipView>(nullptr);
    m_uiManager.alignElementToWindow( equipUi, UI::Alignment::TopRight, {-20, 20} );
    equipUi->hide();

    auto playerInventory = m_uiManager.createElement<UI::ContainerView>(nullptr, Vector2i{6, 4});
    m_uiManager.alignElementToElement( playerInventory, equipUi, UI::Alignment::BottomRight, {0, 10} );
    playerInventory->setId("player-inventory");
    playerInventory->hide();
}

void Level::layoutWindows()
{
    auto wndSize = ResourceManager::get().getWindow()->getSize();
    m_camera.setViewportSize(wndSize);
    m_uiManager.doLayout();
}

UI::Manager& Level::ui()
{
    return m_uiManager;
}

bool Level::isComplete() const
{
    return m_isComplete;
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

void Level::setComplete()
{
    m_isComplete = true;
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

RandomInterface &Level::random()
{
    return m_random;
}

void Level::setLayout(LevelLayout const& llayout)
{
    m_baseTilemap = llayout.tileTypes;
    m_renderTileMap = llayout.tileset;
    m_mapRendering = llayout.mapData;
}


std::vector<GameAction> Level::actionsForActor(EntityRef actor)
{
    auto actorC = ecs().getComponents<ActorComponent>(actor);
    std::vector<GameAction> out;

    // Add the actions which all actors have access to by default
    // TODO: Action costs. Action limits per turn. Enable/disable actions which cannot be performed.

    out.emplace_back(
        "move", TargetingType::SingleTile,
        std::make_shared<ActionMoveStride>()
    );

    out.emplace_back(
        "short-step", TargetingType::SingleTile,
        std::make_shared<ActionMoveStep>()
    );

    out.emplace_back(
        "strike", TargetingType::SingleEntity,
        std::make_shared<ActionMeleeAttack>()
    );
    
    out.emplace_back(
        "full-attack", TargetingType::SingleEntity,
        std::make_shared<ActionFullAttack>()
    );

    // Grab our custom actions, e.g. actions which have been added by feats, items, etc.
    for ( auto const& action : actorC->actor.getAllGameActions() )
    {
        out.push_back(action);
    }


    // For each of our potential actions, work out if our actor can currently perfom them. If not,
    // mark them as disabled.

    for ( auto& act : out )
    {
        ActionSpeedData speedData;
        speedData.action = &act;
        speedData.modified = act.data.speed;

        actorC->actor.applyAllModifiers( &speedData );

        if ( !actorC->actor.actionInfo().canUseAction( speedData.modified ))
        {
            act.enabled = false;
        }
    }

    return out;
}

LevelController* Level::controller()
{
    return m_controllers.back().get();
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

    Vector2f centre = findCentroid( points );
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

bool Level::isInteractable() const
{
    return m_canInteract;
}

void Level::setInteractible(bool val)
{
    m_canInteract = val;
}

void Level::switchTurn()
{
    m_isPlayerTurn = !m_isPlayerTurn;
}

void Level::advanceRound()
{
    m_currentRound++;
}


