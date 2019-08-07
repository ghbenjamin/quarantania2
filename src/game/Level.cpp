#include <utility>

#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <game/GEventDefs.h>
#include <utils/Logging.h>
#include <utils/Assert.h>
#include <ui/TextNode.h>
#include <ui/Label.h>
#include <graphics/Primatives.h>
#include <resource/ResourceManager.h>
#include <ui/TextLog.h>
#include <ui/Layout.h>
#include <ui/ContextMenu.h>

Level::Level(Vector2i size, LevelContextPtr ctx)
: m_ctx(std::move(ctx)), m_bounds(size), m_grid(size), m_tileCount(size.x() * size.y()), m_entFactory(this)
{
    registerComponent<Components::Render>();
    registerComponent<Components::TilePosition>();
    registerComponent<Components::Collider>();
    registerComponent<Components::FixedState>();
    registerComponent<Components::FixedRenderState>();

    registerSystem<Systems::Render>();
    registerSystem<Systems::Position>();
    registerSystem<Systems::Collision>();
    registerSystem<Systems::FOV>();
    registerSystem<Systems::FixedState>();
    registerSystem<Systems::FixedRenderState>();

    m_camera.setBounds( m_bounds * 16 );

    setupUI();
    layoutWindows();
}

bool Level::input(IEvent &evt)
{
    if ( m_uiManager.input(evt) )
    {
        return true;
    }

    switch ( evt.type )
    {
        case IEventType::KeyPress:
            return handleKeyInput(evt.keyPress);
        case IEventType::MouseDown:
            return handleMouseClickInput(evt.mouseDown);
        case IEventType::MouseMove:
            return handleMouseMoveInput(evt.mouseMove);
        case IEventType::WindowResize:
            layoutWindows();
            return true;
        default:
            return false;
    }
}

bool Level::handleKeyInput(IEventKeyPress &evt)
{
    switch ( evt.keyCode )
    {
        case SDLK_w:
        case SDLK_a:
        case SDLK_s:
        case SDLK_d:
            doMovePlayer( evt.keyCode );
            break;

        default:
            break;
    }

    return false;
}

bool Level::handleMouseMoveInput(IEventMouseMove& evt)
{
    return false;
}

bool Level::handleMouseClickInput(IEventMouseDown& evt)
{
    return false;
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

void Level::doMovePlayer(SDL_Keycode kcode)
{
    auto ref = m_player->ref();
    auto tpos = getComponents<Components::TilePosition>(ref);

    Vector2i delta;

    switch (kcode)
    {
        case SDLK_w:
            delta = {0, -1};
            break;
        case SDLK_a:
            delta = {-1, 0};
            break;
        case SDLK_s:
            delta = {0, 1};
            break;
        case SDLK_d:
            delta = {1, 0};
            break;
        default:
            AssertAlways();
            break;
    }

    Vector2i newPos = tpos->position + delta;

    if ( m_grid.pass().valueAt(newPos) != Rules::Passibility::Impassable )
    {
        m_gevents.broadcast<GEvents::EntityMove>( ref, tpos->position, newPos );
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

void Level::setPlayer(std::unique_ptr<Player> &&player)
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
//    auto ptr = m_uiManager.createElement<UI::Label>( nullptr );
//
//    ptr->setLocalPosition( {10, 10} );
//    ptr->setText("Hello, World!");
//    ptr->setBorder( 2, Colour::Black );
//    ptr->setBackgroundColour( Colour::Grey );
//    ptr->setPadding( 5 );

    auto tlog = m_uiManager.createElement<UI::TextLog>(nullptr);
    tlog->setPreferredContentSize({300, 200});
    tlog->setId("global-text-log");

    auto rframe = m_uiManager.createElement<UI::Element>(nullptr);
    rframe->setId("right-frame");
    rframe->setBorder( 2, Colour::Black );
    rframe->setBackgroundColour( Colour::Grey );

    UI::ContextMenuList cml = {
        "Hello", "World", "", "a", "b", "c", "", "d"
    };

    auto cmenu = m_uiManager.createElement<UI::ContextMenu>(nullptr, cml);

    m_uiManager.alignElementToWindow( tlog, UI::Alignment::BottomLeft, 0 );
    m_uiManager.alignElementToWindow( rframe, UI::Alignment::CentreRight, 0 );
    m_uiManager.alignElementToWindow( cmenu, UI::Alignment::TopLeft, 0 );
}

void Level::layoutWindows()
{
    auto wndSize = ResourceManager::get().getWindow()->getSize();

    auto tlog = m_uiManager.withId("global-text-log");
    auto rframe = m_uiManager.withId("right-frame");

    int rframeW = 300;
    int rframeH = wndSize.y();

    int levelW = wndSize.x() - rframeW;
    int levelH = wndSize.y() - tlog->outerSize().y();

    int tlogW = levelW + 2;
    int tlogH = 200;

    tlog->setPreferredOuterSize({tlogW, tlogH});
    rframe->setPreferredOuterSize({ rframeW, rframeH });

    m_camera.setViewportSize({ levelW, levelH });
    m_uiManager.doLayout();
}
