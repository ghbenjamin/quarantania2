#include <utility>

#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <game/GEventDefs.h>
#include <utils/Logging.h>
#include <utils/Assert.h>
#include <ui/TextNode.h>
#include <graphics/Primatives.h>
#include <resource/ResourceManager.h>
#include <ui/TextLog.h>
#include <ui/Layout.h>
#include <state/DefaultLController.h>

#include <systems/All.h>
#include <components/All.h>
#include <ui/MinimapView.h>
#include <actions/ActionDefs.h>

Level::Level(Vector2i size, LevelContextPtr ctx, RandomGenerator const& rg)
: m_ctx(std::move(ctx)), m_bounds(size), m_grid(size),
  m_tileCount(size.x() * size.y()), m_rg(rg), m_entFactory(this, &m_rg)
{
    registerComponent<Components::Render>();
    registerComponent<Components::TilePosition>();
    registerComponent<Components::Collider>();
    registerComponent<Components::Description>();
    registerComponent<Components::Actor>();
    registerComponent<Components::Action>();
    registerComponent<Components::Lockable>();
    registerComponent<Components::Openable>();
    registerComponent<Components::Tags>();

    registerSystem<Systems::Render>();
    registerSystem<Systems::Position>();
    registerSystem<Systems::Collision>();
    registerSystem<Systems::FOV>();
    registerSystem<Systems::Actors>();

    m_camera.setBounds( m_bounds * 16 );

    setupUI();
    layoutWindows();

    m_controller = std::make_unique<DefaultLController>(this);
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

    // Move the camera if necessary
    updateCamera(ticks, iinter, rInter);

    // Render statics: tiles, etc.
    render(ticks, iinter, rInter);

    // Render everything managed by the ECS
    for ( auto const& sys : m_systems )
    {
        sys->update( ticks, rInter );
    }

    // Render the FOV layer


    // Render the GUI
    m_uiManager.update(ticks, iinter, rInter);
}

void Level::renderTiles(uint32_t ticks, RenderInterface &rInter)
{
    Vector2i offset;
    Vector2i currPos;
    int row, col;
    int width = m_bounds.x();

    for ( auto const& layer : m_mapRendering )
    {
        offset = {0, 0};
        row = 0;
        col = 0;

        for ( auto const& ref : layer )
        {
            if ( ref >= 0 )
            {
                currPos = offset + Vector2i{ col * 16, row * 16 };
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
}

void Level::updateCamera(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( iinter.anyHeld() )
    {
        Vector2f delta = { 0.0, 0.0 };
        if ( iinter.isHeld( SDLK_LEFT ) )
        {
            delta += {-1.0, 0};
        }
        if ( iinter.isHeld( SDLK_RIGHT ) )
        {
            delta += {1.0, 0};
        }
        if ( iinter.isHeld( SDLK_UP ) )
        {
            delta += {0, -1.0};
        }
        if ( iinter.isHeld( SDLK_DOWN ) )
        {
            delta += {0, 1.0};
        }

        if ( delta.x() != 0 || delta.y() != 0 )
        {
            m_camera.moveBy(delta * (float)ticks);
        }
    }
}

EntityRef Level::createEntity()
{
    // Requisition a new ID from the pool, but dont construct any new
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

void Level::disableEntity(EntityRef ent)
{
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            // it->second.DISABLE_ME
        }
    }
}

void Level::enableEntity(EntityRef ent)
{
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            // it->second.ENABLE_ME
        }
    }
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
    auto tlog = m_uiManager.createElement<UI::TextLog>(nullptr);
    tlog->setPreferredContentSize({300, 200});
    tlog->setId("global-text-log");

    auto mmap = m_uiManager.createElement<UI::MinimapView>(nullptr, this);
    mmap->setId("minimap");
    mmap->setBorder( 2, Colour::Blue );

    auto trframe = m_uiManager.createElement<UI::Element>(nullptr);
    trframe->setId("top-right-frame");

    auto trfLabel = m_uiManager.createElement<UI::TextNode>( trframe.get(), UI::TextStyle{ Colour::White } );
    trfLabel->setId( "trf-label" );

    m_uiManager.alignElementToWindow( tlog, UI::Alignment::BottomLeft, 0 );
    m_uiManager.alignElementToWindow( mmap, UI::Alignment::CentreRight, 0 );
    m_uiManager.alignElementToWindow( trframe, UI::Alignment::TopRight, 0 );
}

void Level::layoutWindows()
{
    auto wndSize = ResourceManager::get().getWindow()->getSize();

    auto tlog = m_uiManager.withId("global-text-log");
    auto mmap = m_uiManager.withId("minimap");
    auto trframe = m_uiManager.withId("top-right-frame");

    int rframeW = 300;
    int rframeH = wndSize.y() / 3;

    int levelW = wndSize.x() - rframeW;
    int levelH = wndSize.y() - tlog->outerSize().y();

    int tlogW = levelW + 2;
    int tlogH = 200;

    tlog->setPreferredOuterSize({tlogW, tlogH});
    mmap->setPreferredOuterSize({ rframeW, rframeH });
    trframe->setPreferredOuterSize({rframeW, rframeH - 50});

    m_camera.setViewportSize({ levelW, levelH });
    m_uiManager.doLayout();
}

Vector2i Level::worldCoordsToTile(Vector2i const& world)
{
    return {
        world.x() / 16,
        world.y() / 16
    };
}

Vector2i Level::screenCoordsToTile(Vector2i const &screen)
{
    auto world = screenCoordsToWorld(screen);
    return worldCoordsToTile(world);
}

Vector2i Level::tileCoordsToScreen( Vector2i const& tile )
{
    auto world = tile * 16;
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

void Level::generateMinimap()
{
    m_minimap = { createRectangle( m_bounds, Colour::Black ) };

}

Sprite const &Level::getMinimap() const
{
    return m_minimap;
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

    return std::move(out);
}

std::vector<ActionPtr> Level::actionsForEntity(EntityRef actor, EntityRef subject)
{
    std::vector<ActionPtr> out;

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

    }

    return std::move(out);
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

    return std::move(out);
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


