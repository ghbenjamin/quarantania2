#include <utility>

#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <utils/Logging.h>
#include <utils/Assert.h>

Level::Level(ImmutableLevelData&& imd, LevelContextPtr ctx)
: m_imData(imd), m_ctx(std::move(ctx)), m_entityFactory(&m_ecs)
{
    ImPlayerData impData;
    impData.name = "Urist McUrist";
    m_player = m_entityFactory.createPlayer(impData, imd.entranceTile);
}

bool Level::input(IEvent &evt)
{
    switch ( evt.type )
    {
        case IEventType::KeyPress:
            return handleKeyInput(evt.keyPress);
        case IEventType::MouseClick:
            break;
        case IEventType::MouseMove:
            break;
        case IEventType::WindowResize:
            break;
    }

    return false;
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

void Level::render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    renderTiles(ticks, rInter);
}

void Level::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    updateCamera(ticks, iinter, rInter);

    // Render ourself: tiles, etc.
    render(ticks, iinter, rInter);

    // Render everything managed by the ECS
    m_ecs.update(ticks, rInter);

    // Render the GUI
    // TODO Gui
}

void Level::renderTiles(uint32_t ticks, RenderInterface &rInter)
{
    Vector2i offset;
    Vector2i currPos;
    int row, col;
    int width = m_imData.levelSize.x();

    for ( auto const& layer : m_imData.mapRendering )
    {
        offset = {0, 0};
        row = 0;
        col = 0;

        for ( auto const& ref : layer )
        {
            if ( ref >= 0 )
            {
                currPos = offset + Vector2i{ col * m_imData.tilePixelSize, row * m_imData.tilePixelSize };
                rInter.addWorldItem( m_imData.renderTileMap.get(ref).sprite.renderObject(currPos) );
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
    rInter.camera().setBounds( m_imData.levelSize * m_imData.tilePixelSize );

    float scrollSpeed = 0.8f;

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
            rInter.camera().moveBy(delta * (scrollSpeed * ticks));
        }
    }
}

void Level::doMovePlayer(SDL_Keycode kcode)
{
    auto ref = m_player->ref();
    auto tpos = m_ecs.get<Components::TilePosition>(ref);

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



    GEvents::EntityMove evt;
    evt.ent = ref;
    evt.oldPosition = tpos->position;
    evt.newPosition = tpos->position + delta;

    GEvent gevt = { GEventType::EntityMove, GEventScope::Before, evt };
    m_ecs.events().broadcast( gevt );


}

