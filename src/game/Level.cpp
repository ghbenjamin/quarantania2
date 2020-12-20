#include <utility>

#include <components/All.h>
#include <engine/InputInterface.h>
#include <game/ActionDefs.h>
#include <game/Action.h>
#include <game/GameEventDefs.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <resource/ResourceManager.h>
#include <ui/TextLog.h>
#include <ui/LevelUi.h>
#include <utils/GlobalConfig.h>


Level::Level(Vector2i size, LevelContextPtr ctx, RandomGenerator const& rg)
:   m_ctx(std::move(ctx)),
    m_grid(size),
    m_random(rg),
    m_isComplete(false),
    m_camera( size * GlobalConfig::TileSizePx ),
    m_uiManager(this),
    m_currentTurnEntity(EntityNull),
    m_ecs(this),
    m_controllers{ std::make_shared<DefaultLController>(this) }
{
    setupUI();
    layoutWindows();
}

void Level::setReady()
{
    generateTurnOrder();
    m_gevents.broadcast<GameEvents::TurnChange>(EntityNull, m_currentTurnEntity);
    m_gevents.broadcast<GameEvents::LevelReady>();
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
    renderTiles(ticks, rInter);
}

void Level::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    // Specify which camera we are using
    rInter.setCamera(&m_camera);


    // Handle controller changes if necessary

    if ( m_controllers.back()->shouldPopController() )
    {
        m_controllers.back()->onExit();
        m_controllers.pop_back();
        m_controllers.back()->onEnter();
    }
    else if ( m_controllers.back()->hasNextController() )
    {
        auto next = m_controllers.back()->getNextController();
        m_controllers.back()->onExit();
        m_controllers.push_back( next );
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
            rInter.addWorldItem( m_renderTileMap.get(ref).sprite.renderObject(currPos) );
        }

        col++;
        if ( col >= width )
        {
            col = 0;
            row++;
        }
    }
}

GEventHub &Level::events()
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

Vector2i Level::screenCoordsToWorld(Vector2i const &screen)
{
    return m_camera.screenToWorld(screen);
}

Vector2i Level::worldCoordsToScreen(Vector2i const &world)
{
    return m_camera.worldToScreen(world);
}

void Level::setupUI()
{
    // Widget containing the current party and information
    auto turnOrderContainer = m_uiManager.createElement<UI::TurnOrderContainer>(nullptr);
    m_uiManager.alignElementToWindow( turnOrderContainer, UI::Alignment::TopLeft, {20, 20} );

    // Widget containing icons representing actions which can be taken
    auto actionMenu = m_uiManager.createElement<UI::ActionMenuContainer>(nullptr);
    m_uiManager.alignElementToWindow( actionMenu, UI::Alignment::BottomLeft, {20, -20} );
}

void Level::layoutWindows()
{
    auto wndSize = ResourceManager::get().getWindow()->getSize();
    m_camera.setViewportSize(wndSize);
    m_uiManager.doLayout();
}

Vector2i Level::worldCoordsToTile(Vector2i const& world)
{
    return {
        world.x() / GlobalConfig::TileSizePx,
        world.y() / GlobalConfig::TileSizePx
    };
}

Vector2i Level::screenCoordsToTile(Vector2i const &screen)
{
    auto world = screenCoordsToWorld(screen);
    return worldCoordsToTile(world);
}

Vector2i Level::tileCoordsToScreen( Vector2i const& tile )
{
    auto world = tile * GlobalConfig::TileSizePx;
    return worldCoordsToScreen(world);
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

void Level::setComplete()
{
    m_isComplete = true;
}

void Level::addTextLogMessage(std::string_view sv, Colour const& colour)
{
    m_textLog->addLine(sv, colour);
}

void Level::addTextLogMessage(std::string_view sv)
{
    m_textLog->addLine(sv);
}

std::string_view Level::getDescriptionForEnt(EntityRef ent)
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

std::string_view Level::getDescriptionForItem(ItemPtr item)
{
    return item->getName();
}

RandomInterface &Level::random()
{
    return m_random;
}

void Level::setLayout(const LD::LevelLayout &llayout)
{
    m_baseTilemap = llayout.tileTypes;
    m_renderTileMap = llayout.tileset;
    m_mapRendering = llayout.mapData;
}

void Level::generateTurnOrder()
{
    // TODO: Include the attributes of the entities in this calculation
    auto actors = m_ecs.entitiesHaving<ActorComponent>();
    random().shuffle( actors );

    m_turnOrder = std::move(actors);
    m_currentTurnEntity = m_turnOrder.front();
}

std::vector<EntityRef> const &Level::turnOrder() const
{
    return m_turnOrder;
}

void Level::nextTurn()
{
    bool newRound = false;
    auto it = std::find(m_turnOrder.begin(), m_turnOrder.end(), m_currentTurnEntity);
    it++;

    if (it == m_turnOrder.end())
    {
        it = m_turnOrder.begin();
        newRound = true;
    }

    auto oldEnt = m_currentTurnEntity;
    m_currentTurnEntity = *it;

    events().broadcast<GameEvents::TurnChange>(oldEnt, m_currentTurnEntity);

    if (newRound)
    {
        events().broadcast<GameEvents::RoundChange>();
    }
}

EntityRef Level::getActiveEntity() const
{
    return m_currentTurnEntity;
}

std::vector<std::shared_ptr<Action>> Level::actionsForActor(EntityRef actor)
{
    auto cActor = m_ecs.getComponents<ActorComponent>(actor);
    Assert( cActor->actorType == ActorType::PC );


    std::vector<std::shared_ptr<Action>> out;

    out.push_back( std::make_shared<MoveAction>( this, "short-step", 1 ));
    out.push_back( std::make_shared<MoveAction>( this, "move", 5 ));
    out.push_back( std::make_shared<AttackAction>( this, "strike" ));

    return out;
}

