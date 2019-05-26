#include <utility>

#include <game/Level.h>
#include <graphics/RenderInterface.h>

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

bool Level::input(SDL_Event &evt)
{
    return false;
}

void Level::render(uint32_t ticks, RenderInterface &rInter)
{
    renderTiles(ticks, rInter);
}

void Level::update(uint32_t ticks, RenderInterface &rInterface)
{
    // Render ourself: tiles, etc.
    render(ticks, rInterface);

    // Render everything managed by the ECS
    m_ecs.update(ticks, rInterface);

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

