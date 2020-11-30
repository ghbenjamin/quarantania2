#include <utility>

#include <components/All.h>
#include <systems/All.h>

#include <engine/InputInterface.h>
#include <game/ActionDefs.h>
#include <game/GameEventDefs.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <resource/ResourceManager.h>
#include <ui/ContainerView.h>
#include <ui/Dialogs.h>
#include <ui/Layout.h>
#include <ui/TextLog.h>
#include <ui/LevelUi.h>
#include <utils/Assert.h>
#include <utils/GlobalConfig.h>
#include <utils/Logging.h>


Level::Level(Vector2i size, LevelContextPtr ctx, RandomGenerator const& rg)
:   m_ctx(std::move(ctx)),
    m_grid(size),
    m_random(rg),
    m_entFactory(this),
    m_isComplete(false),
    m_camera( size * GlobalConfig::TileSizePx ),
    m_uiManager(this),
    m_currentTurnEntity(EntityNull)
{
    m_controllers.push_back( std::make_shared<DefaultLController>(this) );

    registerComponents<AllComponents>();
    registerSystems<AllSystems>();

    setupUI();
    layoutWindows();
}

void Level::setReady()
{
    generateTurnOrder();

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
    rInter.setCamera(&m_camera);

    // Delete our delayed delete entities
    for ( EntityRef ent : m_delayedDeleteEnts )
    {
        deleteEntity( ent );
    }

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

    // Render everything managed by the ECS
    for ( auto const& sys : m_systems )
    {
        sys->update( ticks, rInter );
    }

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

EntityRef Level::createEntity()
{
    // Requisition a new ID from the pool, but don't construct any new
    // component objects
    return m_entityPool.next();
}

void Level::deleteEntity(EntityRef ent)
{
    // Delete all the components attached to this entity
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            v.erase(it);
        }
    }

    // Put the ID back into the pool
    m_entityPool.release(ent);
}

GEventHub &Level::events()
{
    return m_gevents;
}

Grid& Level::grid( )
{
    return m_grid;
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

    if ( entityHas<ActorComponent>(subject) )
    {
        auto actorC = getComponents<ActorComponent>(subject);

        auto act = std::make_shared<MeleeAttackAction>(this, actor, subject);
        out.push_back( std::static_pointer_cast<Action>(act) );
    }

    if ( entityHas<ItemComponent>(subject) )
    {
        auto itemC = getComponents<ItemComponent>(subject);

        auto act = std::make_shared<PickUpItemAction>(this, actor, subject);
        out.push_back( std::static_pointer_cast<Action>(act) );
    }

    if ( entityHas<OpenableComponent>(subject) )
    {
        auto openable = getComponents<OpenableComponent>(subject);
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

    if ( entityHas<LockableComponent>(subject) )
    {
        auto lockable = getComponents<LockableComponent>(subject);
        if ( lockable->isLocked )
        {
            auto act = std::make_shared<UnlockAction>(this, actor, subject);
            out.push_back( std::static_pointer_cast<Action>(act) );
        }
    }

    if ( entityHas<ActionComponent>(subject) )
    {
        auto actionComp = getComponents<ActionComponent>(subject);
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
    auto transformA = getComponents<PositionComponent>(a);
    auto transformB = getComponents<PositionComponent>(b);

    Vector2i distance = transformB->position - transformA->position;
    return distance.x() * distance.x() + distance.y() * distance.y();
}

void Level::setComplete()
{
    m_isComplete = true;
}

Camera &Level::camera()
{
    return m_camera;
}

void Level::entityReady(EntityRef ent)
{
    m_gevents.broadcast<GameEvents::EntityReady>(ent );
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
    if ( entityHas<ActorComponent>(ent) )
    {
        auto comp = getComponents<ActorComponent>(ent);
        return comp->actor.getName();
    }
    else if ( entityHas<ItemComponent>(ent) )
    {
        auto comp = getComponents<ItemComponent>(ent);
        return comp->item->getName();
    }

    return "<unknown>";
}

std::string_view Level::getDescriptionForItem(ItemPtr item)
{
    return item->getName();
}

EntityFactory &Level::entityFactory()
{
    return m_entFactory;
}

RandomInterface &Level::random()
{
    return m_random;
}

void Level::deleteEntityDelayed(EntityRef ent)
{
    m_delayedDeleteEnts.push_back( ent );
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
    auto actors = entitiesHaving<ActorComponent>();
    random().shuffle( actors );

    m_turnOrder = std::move(actors);
    m_currentTurnEntity = m_turnOrder.front();
}

std::vector<EntityRef> const &Level::turnOrder() const
{
    return m_turnOrder;
}

