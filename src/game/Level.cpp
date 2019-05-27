#include <utility>

#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <utils/Logging.h>

Level::Level(ImmutableLevelData&& imd, LevelContextPtr ctx)
: m_imData(imd), m_ctx(std::move(ctx))
{
    PassibilityData defaultPd {imd.defaultPassibility};
    m_passibilityData = std::vector<PassibilityData>(m_imData.tileCount, defaultPd);

    VisibilityData defaultVd {imd.defaultVisibility};
    m_visibilityData = std::vector<VisibilityData>(m_imData.tileCount, defaultVd);

    LightingData defaultLd {imd.defaultLightLevel};
    m_lightingData = std::vector<LightingData>(m_imData.tileCount, defaultLd);
}

bool Level::input(IEvent &evt)
{
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
    Vector2i offset = {0, 0};
    Vector2i currPos;
    int row = 0;
    int col = 0;
    int width = m_imData.levelSize.x();

    for ( auto const& ref : m_imData.mapLayout )
    {
        currPos = offset + Vector2i{ col * m_imData.tilePixelSize, row * m_imData.tilePixelSize };

        rInter.addWorldItem( m_imData.tileMap.get(ref).sprite.renderObject(currPos) );
        col++;
        if ( col >= width )
        {
            col = 0;
            row++;
        }
    }
}

void Level::updateCamera(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    rInter.camera().setBounds( m_imData.levelSize * m_imData.tilePixelSize );

    float scrollSpeed = 4.0f;

    if ( iinter.anyHeld() )
    {
        if ( iinter.isHeld( SDLK_LEFT ) )
        {
            rInter.camera().moveBy({-scrollSpeed, 0});
        }
        if ( iinter.isHeld( SDLK_RIGHT ) )
        {
            rInter.camera().moveBy({scrollSpeed, 0});
        }
        if ( iinter.isHeld( SDLK_UP ) )
        {
            rInter.camera().moveBy({0, -scrollSpeed});
        }
        if ( iinter.isHeld( SDLK_DOWN ) )
        {
            rInter.camera().moveBy({0, scrollSpeed});
        }
    }
}

