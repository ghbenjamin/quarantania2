#include <utility>

#include <actions/ActionDefs.h>
#include <components/All.h>
#include <game/GEventDefs.h>
#include <game/InputInterface.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <resource/ResourceManager.h>
#include <state/DefaultLController.h>
#include <systems/All.h>
#include <ui/Layout.h>
#include <ui/TextLog.h>
#include <utils/Assert.h>
#include <utils/Logging.h>
#include <systems/Message.h>
#include <ui/Dialogs.h>
#include <ui/MinimapView.h>
#include <utils/GlobalConfig.h>

Level::Level(Vector2i size, LevelContextPtr ctx, RandomGenerator const& rg)
:   m_ctx(std::move(ctx)),
    m_bounds(size),
    m_grid(size),
    m_tileCount(size.x() * size.y()),
    m_rg(rg),
    m_entFactory(this, &m_rg),
    m_isComplete(false),
    m_minimap{ m_bounds, 5 },
    m_camera( m_bounds * GlobalConfig::TileSizePx ),
    m_controller( std::make_unique<DefaultLController>(this) )
{
    registerComponents<AllComponents>();
    registerSystems<AllSystems>();

    setupUI();
    layoutWindows();
}

void Level::setReady()
{
    m_gevents.broadcast<GEvents::LevelReady>();

    auto ref = getPlayer()->ref();
    auto tpos = getComponents<Components::TilePosition>(ref);
    m_camera.centreOnTile(tpos->position);


    // Debug
    m_textLog->addLine( "Hello, World!" );
    m_textLog->addLine( "Hello, World!" );
    m_textLog->addLine( "It was the best of times, it was the worst of times, it was the age of wisdom, it was the age"
                        " of foolishness, it was the epoch of belief, it was the epoch of incredulity, it was the "
                        "season of Light, it was the season of Darkness" );
    m_textLog->addLine( "Example line" );
    m_textLog->addLine( "Example line1" );
    m_textLog->addLine( "Example line2" );
    m_textLog->addLine( "Example line3" );
    m_textLog->addLine( "Example line4" );
    m_textLog->addLine( "Example line5" );
    m_textLog->addLine( "Example line6" );
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

    return m_controller->input(evt);
}

void Level::render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    renderTiles(ticks, rInter);
}

void Level::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    rInter.setCamera(&m_camera);

    m_controller->update(ticks, iinter, rInter);

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
    int row, col;
    int width = m_bounds.x();


    offset = {0, 0};
    row = 0;
    col = 0;

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

void Level::setPlayer(PlayerPtr &&player)
{
    m_player = std::move(player);
}

Grid& Level::grid( )
{
    return m_grid;
}

Vector2i const &Level::bounds() const
{
    return m_bounds;
}

int Level::tileCount() const
{
    return m_tileCount;
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

    m_textLog = m_uiManager.createElement<UI::TextLog>(nullptr);
    m_textLog->setPreferredOuterSize({wndSize.x() - RightFrameWidth, TextLogHeight});
    m_textLog->setId("global-text-log");

    m_uiManager.alignElementToWindow( m_textLog, UI::Alignment::BottomLeft, 0 );

    auto trframe = m_uiManager.createElement<UI::Element>(nullptr);
    trframe->setLayout<UI::VerticalLayout>( 2, UI::HAlignment::Fill );
    trframe->setId("right-frame");
    trframe->setBackgroundColour(Colour::Black);

    m_uiManager.alignElementToWindow( trframe, UI::Alignment::CentreRight, 0 );

    UI::Element* trframePtr = trframe.get();
    auto minimap = m_uiManager.createElement<UI::MinimapView>(
        trframePtr,
        m_minimap.getTexture(),
        Vector2i{200, 200}
    );
    minimap->setId("minimap");
}

void Level::layoutWindows()
{
    auto wndSize = ResourceManager::get().getWindow()->getSize();
    auto trframe = m_uiManager.withId("right-frame");

    Vector2 rframeSize = {
        RightFrameWidth,
        wndSize.y()
    };

    Vector2 levelSize = {
        wndSize.x() - RightFrameWidth,
        wndSize.y() - TextLogHeight
    };

    Vector2 textLogSize = {
        wndSize.x() - RightFrameWidth,
        TextLogHeight
    };

    m_textLog->setPreferredOuterSize(textLogSize);
    trframe->setPreferredOuterSize(rframeSize);

    m_camera.setViewportSize(levelSize);
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

PlayerPtr &Level::getPlayer()
{
    return m_player;
}

bool Level::isPlayer(EntityRef ref) const
{
    return ref == m_player->ref();
}

std::vector<ActionPtr> Level::actionsForTile(EntityRef actor, Vector2i tile)
{
    std::vector<ActionPtr> out;

    if ( grid().pass().valueAt(tile) != Rules::Passibility::Impassable )
    {
        auto moveAct = std::make_shared<StepAction>(this, actor, tile);
        out.push_back( std::static_pointer_cast<Action>(moveAct) );
    }

    return out;
}

std::vector<ActionPtr> Level::actionsForEntity(EntityRef actor, EntityRef subject)
{
    std::vector<ActionPtr> out;

    if ( entityHas<Components::Actor>(subject) )
    {
        auto actorC = getComponents<Components::Actor>(subject);

        auto act = std::make_shared<MeleeAttackAction>(this, actor, subject);
        out.push_back( std::static_pointer_cast<Action>(act) );
    }

    if ( entityHas<Components::Openable>(subject) )
    {
        auto openable = getComponents<Components::Openable>(subject);
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

    if ( entityHas<Components::Lockable>(subject) )
    {
        auto lockable = getComponents<Components::Lockable>(subject);
        if ( lockable->isLocked )
        {
            auto act = std::make_shared<UnlockAction>(this, actor, subject);
            out.push_back( std::static_pointer_cast<Action>(act) );
        }
    }

    if ( entityHas<Components::Action>(subject) )
    {
        auto actionComp = getComponents<Components::Action>(subject);
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

ActionPtr Level::getDefaultAction(EntityRef actor, Vector2i position)
{
    auto actions = actionsForPosition(actor, position);

    if ( actions.empty() )
    {
        return ActionPtr();
    }
    else
    {
        return actions.front();
    }
}

bool Level::isComplete() const
{
    return m_isComplete;
}

int Level::squaredEntityDistance(EntityRef a, EntityRef b)
{
    auto transformA = getComponents<Components::TilePosition>(a);
    auto transformB = getComponents<Components::TilePosition>(b);

    Vector2i distance = transformB->position - transformA->position;
    return distance.x() * distance.x() + distance.y() * distance.y();
}

void Level::setComplete()
{
    m_isComplete = true;
}

void Level::generateMinimapData()
{
    for ( std::size_t i = 0; i < m_baseTilemap.size(); i++ )
    {
        // Tiles which we've not explored yet should be black
        if ( m_grid.fov().valueAt(i) == Rules::Visibility::Hidden )
        {
            m_minimap.setTile(i, Colour::Black);
        }
        else
        {
            // Otherwise, set the appropriate colour for the current tile
            switch ( m_baseTilemap[i] )
            {
                case LD::BaseTileType::Wall:
                    m_minimap.setTile(i, Colour::Grey);
                    break;
                case LD::BaseTileType::Floor:
                    m_minimap.setTile(i, Colour::White);
                    break;
                case LD::BaseTileType::Junction:
                    m_minimap.setTile(i, Colour::Blue);
                    break;
            }
        }
    }

    // Place a dot at the position of the player
    auto playerPos = getComponents<Components::TilePosition>( m_player->ref() );
    m_minimap.movePlayer(playerPos->position);

    m_minimap.updateTexture();
}

Camera &Level::camera()
{
    return m_camera;
}