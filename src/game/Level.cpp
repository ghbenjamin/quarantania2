#include <game/Level.h>

Level::Level(LevelInitData const& ctx)
: m_size(ctx.levelSize)
{
    Assert( m_size.x() > 0 && m_size.y() > 0 );
    m_tileCount = m_size.x() * m_size.y();

    PassibilityData defaultPd {ctx.defaultPassibility};
    m_passibilityData = std::vector<PassibilityData>(m_tileCount, defaultPd);

    VisibilityData defaultVd {ctx.defaultVisibility};
    m_visibilityData = std::vector<VisibilityData>(m_tileCount, defaultVd);

    LightingData defaultLd {ctx.defaultLightLevel};
    m_lightingData = std::vector<LightingData>(m_tileCount, defaultLd);
}

bool Level::input(SDL_Event &evt)
{
    return false;
}

void Level::update(uint32_t ticks, RenderInterface &rInterface)
{
    m_ecs.update(ticks, rInterface);
}

const Vector2i &Level::getSize() const
{
    return m_size;
}