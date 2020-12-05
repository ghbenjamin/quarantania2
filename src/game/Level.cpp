#include <utility>

#include <components/All.h>
#include <engine/InputInterface.h>
#include <game/ActionDefs.h>
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
    auto wndSize = ResourceManager::get().getWindow()->getSize();
    auto turnOrderContainer = m_uiManager.createElement<UI::TurnOrderContainer>(nullptr);
    m_uiManager.alignElementToWindow( turnOrderContainer, UI::Alignment::TopLeft, {10, 10} );
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

std::vector<ActionPtr> Level::actionsForTile(EntityRef actor, Vector2i tile)
{
    std::vector<ActionPtr> out;

    if ( grid().pass().valueAt(tile) != Passibility::Impassable )
    {
        auto moveAct = std::make_shared<StepAction>(this, actor, tile);
        out.push_back( std::static_pointer_cast<Action>(moveAct) );
    }

    return out;
}

std::vector<ActionPtr> Level::actionsForEntity(EntityRef actor, EntityRef subject)
{
    std::vector<ActionPtr> out;

    if ( m_ecs.entityHas<ActorComponent>(subject) )
    {
        auto actorC = m_ecs.getComponents<ActorComponent>(subject);

        auto act = std::make_shared<MeleeAttackAction>(this, actor, subject);
        out.push_back( std::static_pointer_cast<Action>(act) );
    }

    if ( m_ecs.entityHas<ItemComponent>(subject) )
    {
        auto itemC = m_ecs.getComponents<ItemComponent>(subject);

        auto act = std::make_shared<PickUpItemAction>(this, actor, subject);
        out.push_back( std::static_pointer_cast<Action>(act) );
    }

    if ( m_ecs.entityHas<OpenableComponent>(subject) )
    {
        auto openable = m_ecs.getComponents<OpenableComponent>(subject);
        if ( openable->isOpen)
        {
            auto act = std::make_shared<CloseAction>(this, actor, subject);
            out.push_back( std::static_pointer_cast<Action>(act) );
        }
        else
        {
            auto act = std::make_shared<OpenAction>(this, actor, subject);
            out.push_back( std::static_pointer_cast<Action>(act) );
        }
    }

    if ( m_ecs.entityHas<LockableComponent>(subject) )
    {
        auto lockable = m_ecs.getComponents<LockableComponent>(subject);
        if ( lockable->isLocked )
        {
            auto act = std::make_shared<UnlockAction>(this, actor, subject);
            out.push_back( std::static_pointer_cast<Action>(act) );
        }
    }

    if ( m_ecs.entityHas<ActionComponent>(subject) )
    {
        auto actionComp = m_ecs.getComponents<ActionComponent>(subject);
        for ( auto const& act : actionComp->actions )
        {
            act->setActor(actor);
            act->setSubject(subject);

            out.push_back( std::static_pointer_cast<Action>(act) );
        }
    }

    out.erase( std::remove_if(out.begin(), out.end(), []( ActionPtr const& item ){
        return !item->canTryAction();
    }), out.end() );

    return out;
}

std::vector<ActionPtr> Level::actionsForPosition(EntityRef actor, Vector2i position)
{
    std::vector<ActionPtr> out;

    auto entsAtTile = grid().entitiesAtTile(position);
    auto tiles = actionsForTile(actor ,position);
    out.insert( out.begin(), tiles.begin(), tiles.end() );

    for ( auto ent : entsAtTile )
    {
        auto ents = actionsForEntity(actor, ent);
        out.insert( out.end(), ents.begin(), ents.end() );
    }

    out.erase( std::remove_if(out.begin(), out.end(), []( ActionPtr const& item ){
        return !item->canTryAction();
    }), out.end() );

    return out;
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

